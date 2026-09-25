#include <iostream>
#include <fstream>
#include "./PhysiPKPD_PD.h"

static double tolerance = 0.01 * diffusion_dt; // using this in single_pd_model for determining when to do that
// extern std::vector<double> signal_scales; 
// extern std::map<std::string,int> signal_to_int;
// extern std::map<int,std::string> int_to_signal; 

Pharmacodynamics_Model::Pharmacodynamics_Model()
{ return; }

Pharmacodynamics_Model *create_pd_model(int substrate_index, int cell_definition_index)
{
    Pharmacodynamics_Model *pNew = create_pd_model();
    pNew->substrate_index = substrate_index;
    pNew->cell_definition_index = cell_definition_index;
    return pNew;
}

Pharmacodynamics_Model *create_pd_model(void)
{
    Pharmacodynamics_Model *pNew;
    pNew = new Pharmacodynamics_Model;
    return pNew;
}

static std::vector<std::string> PD_names;
static std::vector<Pharmacodynamics_Model *> all_pd;

bool is_in_pd_list(std::string substrate_name)
{
    for (int i = 0; i < PD_names.size(); i++ )
    {
        if (substrate_name == PD_names[i])
        { return true; }
    }
    return false;
}

/*
void append_to_signal_list(std::string substrate_name)
{
    static int signal_int = signal_to_int.size();
    std::string signal_name = "custom:" + substrate_name + "_damage";
    signal_to_int[signal_name] = signal_int;
    int_to_signal[signal_int] = signal_name;
}
*/

Pharmacodynamics_Model *create_pd_model(int cell_definition_index, std::string cell_definition_name, pugi::xml_node substrate_node)
{
    std::string substrate_name = substrate_node.attribute("name").as_string();
    int substrate_index = microenvironment.find_density_index(substrate_name);
    Pharmacodynamics_Model *pNew = create_pd_model(substrate_index, cell_definition_index);
    pNew->substrate_name = substrate_name;
    pNew->cell_definition_name = cell_definition_name;
    if (!is_in_pd_list(substrate_name))
    {
        PD_names.push_back(substrate_name);
        // append_to_signal_list(substrate_name);
    }
    if (!(substrate_node.child("dt")))
    {
        std::cout << "PhysiPKPD WARNING: No PD time step supplied for " << substrate_name << " effects on " << cell_definition_name << std::endl
                  << "\tWill use the mechanics_dt by default." << std::endl
                  << "\tSpecify one using the dt node within this substrate element:" << std::endl
                  << "  <cell_definition name=\"" << cell_definition_name << "\" ID=\"" << cell_definition_index << "\">" << std::endl
                  << "  ..." << std::endl
                  << "    <PD>" << std::endl
                  << "      <substrate name=\"" << substrate_name << "\">" << std::endl
                  << "        ..." << std::endl
                  << "        <dt>0.1</dt>" << std::endl
                  << "      </substrate>" << std::endl
                  << "    </PD>" << std::endl
                  << "  </cell_definition>" << std::endl
                  << std::endl;

        pNew->dt = mechanics_dt;
    }
    else
    {
        pNew->dt = substrate_node.child("dt").text().as_double();
    }

    setup_pd_advancer(pNew, substrate_node);
    pNew->previous_pd_time = PhysiCell_globals.current_time;
    pNew->next_pd_time = PhysiCell_globals.current_time;
    Cell_Definition *pCD = cell_definitions_by_index[cell_definition_index];
    if (pCD->custom_data.find_variable_index(substrate_name + "_damage") == -1) // make sure a damage variable for this cell was initialized for this substrate
    {
        for (int i = 0; i < cell_definitions_by_index.size(); i++)
        {
            if (cell_definitions_by_index[i]->custom_data.find_variable_index(substrate_name + "_damage") != -1)
            {
                std::cout << "PhysiPKPD ERROR: No damage variable for " << substrate_name << " acting on " << cell_definition_name << " given." << std::endl
                          << "\tBut " << cell_definitions_by_index[i]->name << " does have this damage variable." << std::endl
                          << "\tI cannot guarantee that adding this variable will put it in the right index." << std::endl
                          << "\tSet this with " << substrate_name + "_damage"
                          << "\tas a custom variable for " << cell_definition_name << std::endl
                          << std::endl;
                exit(-1);
            }
        }
        std::cout << "PhysiPKPD WARNING: No damage variable for " << substrate_name << " acting on " << cell_definition_name << " given." << std::endl
                  << "\tSet this with " << substrate_name + "_damage for all cell types affected by this substrate."
                  << "\tOtherwise, you risk changing variable indices and I can't guarantee that won't cause issues." << std::endl
                  << std::endl;
        pCD->custom_data.add_variable(substrate_name + "_damage", 0.0);

#pragma omp parallel for
        for (int i = 0; i < (*all_cells).size(); i++) // loop over all cells to see if they have a type that got moas added to their custom_data
        {
            if ((*all_cells)[i]->type == cell_definition_index) // check if this cell is of the current type
            {
                (*all_cells)[i]->custom_data.add_variable(substrate_name + "_damage", 0.0);
            }
        } // finish looping over each cell
    }

    pNew->damage_index = pCD->custom_data.find_variable_index(substrate_name + "_damage");
    pNew->advance = &single_pd_model;

    return pNew;
}

void setup_pharmacodynamics()
{
    pugi::xml_node cell_defs_node = PhysiCell::physicell_config_root.child("cell_definitions");
    pugi::xml_node cell_def_node = cell_defs_node.child("cell_definition");
    while (cell_def_node)
    {
        std::string cd_name = cell_def_node.attribute("name").as_string();
        int cd_ind = cell_def_node.attribute("ID").as_int();
        pugi::xml_node pd_node = cell_def_node.child("PD");

        // MAKE SURE ANY REFS TO cell_def_node ARE DONE BEFORE THIS LINE!!!
        cell_def_node = cell_def_node.next_sibling("cell_definition");
        if (!pd_node)
        { continue; }

        Cell_Definition* pCD = cell_definitions_by_index[cd_ind];
        Hypothesis_Ruleset* pHRS = find_ruleset(pCD);
        pugi::xml_node substrate_node = pd_node.child("substrate");
        while(substrate_node)
        {
            default_microenvironment_options.track_internalized_substrates_in_each_agent = true; // ensure that internalized substrates are being tracked
            all_pd.push_back(create_pd_model(cd_ind, cd_name, substrate_node));

            substrate_node = substrate_node.next_sibling("substrate");
        }
    }
    return;
}

void PD_model(double current_time)
{
    for (int n = 0; n < all_pd.size(); n++)
    {
        all_pd[n]->advance(all_pd[n], current_time);
    }
}

void setup_pd_advancer(Pharmacodynamics_Model *pPD, pugi::xml_node substrate_node)
{
    void (*setup_function)(Pharmacodynamics_Model *pPD, pugi::xml_node substrate_node);
    std::vector<std::string> current_options = {"AUC","AUC_amount","SBML","MMD","MMD_amount"};
    std::string method;
    if (!(substrate_node.child("model")))
    {
        std::cout << "PhysiPKPD WARNING: No PD model specified for " << pPD->substrate_name << " affecting " << pPD->cell_definition_name << std::endl
                  << "\tSpecify with user parameter " << pPD->substrate_name + "_on_" + pPD->cell_definition_name + "_pd_model"
                  << "\tset to one of " << current_options << std::endl
                  << "\tWill attempt to set up the AUC model." << std::endl
                  << std::endl;
        setup_function = &setup_pd_model_auc;
    }
    else 
    {
        std::vector<void (*)(Pharmacodynamics_Model*, pugi::xml_node)> fns;
        fns.push_back(&setup_pd_model_auc);
        fns.push_back(&setup_pd_model_auc);
        fns.push_back(&setup_pd_model_sbml);
        fns.push_back(&setup_pd_model_auc);
        fns.push_back(&setup_pd_model_auc);

        std::string model = substrate_node.child("model").text().as_string();
        bool model_found = false;
        for ( int i=0; i<current_options.size(); i++)
        {
            if (model==current_options[i])
            {
                setup_function = fns[i];
                model_found = true;
                method = current_options[i];
                break;
            }
        }
        if (!model_found)
        {
            std::cout << "PhysiPKPD ERROR: " << pPD->substrate_name + " acting on " + pPD->cell_definition_name + " is set to follow " + substrate_node.child("model").text().as_string() + " but this is not an allowable option." << std::endl
                      << "\tCurrent options include: " << current_options << std::endl;
            exit(-1);
        }
    }
    setup_function(pPD, substrate_node);
    pPD->use_internalized_amount = (method=="AUC_amount") || (method=="MMD_amount");
    return;
}

void setup_pd_model_auc(Pharmacodynamics_Model *pPD, pugi::xml_node substrate_node)
{
    if (substrate_node.child("precompute")) // then use this value
    {
        pPD->use_precomputed_quantities = substrate_node.child("precompute").text().as_bool();
    }
    else if (parameters.bools.find_index("PKPD_precompute_all_pd_quantities") != -1) // use the value that sets the default for all PD dynamics in this simulation
    {
        pPD->use_precomputed_quantities = parameters.bools("PKPD_precompute_all_pd_quantities");
    }
    else
    {
        std::cout << "PhysiPKPD WARNING: Unspecified whether or not to use pre-computed quantities for solving PD dynamics of " << pPD->substrate_name << " on " << pPD->cell_definition_name << std::endl
                  << "\tWill default to using pre-computations. Set PKPD_precompute_all_pd_quantities to apply to all PD dynamics." << std::endl
                  << "\tOr set " << pPD->substrate_name + "_precompute_pd_for_" + pPD->cell_definition_name << " for this particular pairing." << std::endl;
        
        // default value of true set in .h file
    }

    Cell_Definition *pCD = cell_definitions_by_index[pPD->cell_definition_index];

    if (pPD->use_precomputed_quantities) // setup precomputed quanities (if not using precomputed quantities, there is currently nothing to set up)
    {
        if (fabs(round(pPD->dt / diffusion_dt) - pPD->dt / diffusion_dt) > 0.0001)
        {
            std::cout << "PhysiPKPD ERROR: Your PD time step for " << pPD->substrate_name << " affecting " << pPD->cell_definition_name << " does not appear to be a multiple of your diffusion time step" << std::endl;
            std::cout << "\tThis will cause errors in solving the PD model using precomputed quantities because it assumes that the time step is constant across the simulation" << std::endl;
            std::cout << "\tIf you really want these time steps, restart the simulation with the user parameter " << pPD->substrate_name << "_precompute_pd_for_" << pPD->cell_definition_name << " set to False" << std::endl
                      << std::endl;
            exit(-1);
        }
        double metabolism_rate = substrate_node.child("metabolism_rate").text().as_double();
        double substrate_to_damage_rate = parse_substrate_to_damage_rate(substrate_node);
        double linear_repair_rate = substrate_node.child("linear_repair_rate").text().as_double();
        double constant_repair_rate = substrate_node.child("constant_repair_rate").text().as_double();

        // internalized drug amount (or concentration) simply decreases as A(dt) = A0 * exp(-metabolism_rate * dt);
        pPD->metabolism_reduction_factor = exp(-metabolism_rate * pPD->dt);

        // Damage (D) follows D' = A - linear_rate * D - constant_rate ==> D(dt) = d_00 + d_10 * A0 + d_01 * D0; defining d_00, d_10, and d_01 here
        pPD->initial_damage_coefficient = exp(-linear_repair_rate * pPD->dt); // d_01
        if (linear_repair_rate == 0)
        {
            pPD->damage_constant = -constant_repair_rate * pPD->dt; // d_00
        }
        else
        {
            pPD->damage_constant = (constant_repair_rate / linear_repair_rate) * (pPD->initial_damage_coefficient - 1); // d_00
        }
        if (metabolism_rate != linear_repair_rate)
        {
            pPD->initial_substrate_coefficient = substrate_to_damage_rate * (pPD->metabolism_reduction_factor - pPD->initial_damage_coefficient) / (linear_repair_rate - metabolism_rate); // d_10
        }
        else
        {
            pPD->initial_substrate_coefficient = substrate_to_damage_rate * pPD->initial_damage_coefficient * pPD->dt; // d_10
        }
    }
}

double parse_substrate_to_damage_rate(const pugi::xml_node &substrate_node)
{
    std::string model = substrate_node.child("model").text().as_string();
    if (model.find("AUC") == 0)
    { return 1.0; }
    else if (model.find("MMD") == 0)
    { return substrate_node.child("metabolism_rate").text().as_double(); }
    else
    {
        std::cout << "PhysiPKPD ERROR: The model " << model << " is not a valid option for the substrate_to_damage_rate." << std::endl
                  << "\tValid options include AUC, AUC_amount, MMD, and MMD_amount." << std::endl;
        exit(-1);
    }
}

void setup_pd_model_sbml(Pharmacodynamics_Model *pPD, pugi::xml_node substrate_node)
{
    std::cout << "PhysiPKPD ERROR: SBML-defined PD dynamics are not supported. Let us know if you are interested in this feature and how you would want to use it." << std::endl
              << "\tFor now, you can only use the AUC model. Set " << pPD->substrate_name + "_on_" + pPD->cell_definition_name + "_pd_model"
              << "\tto AUC" << std::endl
              << std::endl;
    exit(-1);
}

void single_pd_model(Pharmacodynamics_Model *pPD, double current_time)
{
    if (current_time > pPD->next_pd_time - tolerance)
    {
        double dt = current_time - pPD->previous_pd_time;
        pPD->previous_pd_time = current_time;
        pPD->next_pd_time = current_time + pPD->dt;

#pragma omp parallel for
        for (int i = 0; i < (*all_cells).size(); i++)
        {
            Cell *pC = (*all_cells)[i];
            if (!pC->phenotype.death.dead && pC->type == pPD->cell_definition_index) // only update for living cells
            {
                Phenotype &p = pC->phenotype;

                if (!pPD->use_precomputed_quantities)
                {
                    std::cout << "PhysiPKPD ERROR: We are currently requiring precomputed quantities. If you cannot use this, let us know." << std::endl
                              << "\tWe anticipate the main reason to not use precomputation is for heterogeneity of PD parameters within a cell_definition." << std::endl
                              << "\tWe need to discuss how best to move PD parameters into custom_data for this to work, we think." << std::endl;
                    exit(-1);
                }
                else
                {
                    pC->custom_data[pPD->damage_index] *= pPD->initial_damage_coefficient;                                                                 // D(dt) = d_01 * D(0)...
                    pC->custom_data[pPD->damage_index] += pPD->damage_constant;                                                                            // + d_00 ...
                    if (pPD->use_internalized_amount)
                    {
                        pC->custom_data[pPD->damage_index] += pPD->initial_substrate_coefficient * p.molecular.internalized_total_substrates[pPD->substrate_index]; // + d_10*A(0)
                    }
                    else
                    {
                        pC->custom_data[pPD->damage_index] += pPD->initial_substrate_coefficient * p.molecular.internalized_total_substrates[pPD->substrate_index] / pC->phenotype.volume.total; // + d_10*C(0)
                    }

                    if (pC->custom_data[pPD->damage_index] <= 0)
                    {
                        pC->custom_data[pPD->damage_index] = 0; // very likely that cells will end up with negative damage without this because the repair rate can have a constant term
                    }
                    p.molecular.internalized_total_substrates[pPD->substrate_index] *= pPD->metabolism_reduction_factor;
                }
            }
        }
    }
}

void intialize_damage_coloring(int nCD, std::vector<std::vector<int>> &default_colors, std::vector<std::vector<int>> &color_diffs_D1, std::vector<std::vector<int>> &color_diffs_D2, std::vector<std::vector<int>> &damage_inds, std::vector<std::vector<int>> &ec50_vals, std::vector<std::vector<int>> &hp_vals)
{
    damage_inds.resize(nCD, {});
    ec50_vals.resize(nCD, {});
    hp_vals.resize(nCD, {});
    for (int i = 0; i < nCD; i++)
    {
        Cell_Definition *pCD = cell_definitions_by_index[i];
        int grey = (int)round(255 * (i + 1) / (nCD + 1)); // all cell types get their own shade of grey when undamaged
        default_colors.push_back({grey, grey, grey});
        default_colors[i].resize(3, grey);

        int k = 0; // number of signals found that target this cell type
        for (int n = 0; n < all_pd.size(); n++)
        {
            if (all_pd[n]->cell_definition_index != i)
            {
                continue;
            }
            Hypothesis_Ruleset *pHRS = find_ruleset(pCD);
            for (int rule_ind = 0; rule_ind < pHRS->rules.size(); rule_ind++) // loop over all rules for this cell definition
            {
                Hypothesis_Rule *pHR = pHRS->rules[rule_ind];
                for (int sig_ind = 0; sig_ind < pHR->signals.size(); sig_ind++) // loop over all signals looking for this PD substrate
                {
                    if (k >= 2)
                    {
                        break;
                    }
                    if (pHR->signals[sig_ind] == "custom:" + PD_names[n] + "_damage")
                    {
                        damage_inds[i].push_back(find_cell_definition(i)->custom_data.find_variable_index(all_pd[n]->substrate_name + "_damage"));
                        ec50_vals[i].push_back(pHR->half_maxes[sig_ind]);
                        hp_vals[i].push_back(pHR->hill_powers[sig_ind]);
                        k++;
                    }
                }
            }
        }

        if (damage_inds[i].size() > 0)
        {
            color_diffs_D1.push_back({(int)round((255 - grey) / 2), (int)round(-grey / 2), (int)round(-grey / 2)}); // if one drug affects cell type i, then set a red shift in the cytoplasm color
        }
        else
        {
            color_diffs_D1.push_back({0, 0, 0}); // if cell type i is NOT affected by any drug, do not change the cytoplasm color
        }

        if (damage_inds[i].size() > 1)
        {
            color_diffs_D2.push_back({(int)round(-grey / 2), (int)round(-grey / 2), (int)round((255 - grey) / 2)}); // if a second drug affects cell type i, then set a blue shift in the nucleus color
        }
        else
        {
            color_diffs_D2.push_back({0, 0, 0}); // if cell type i is NOT affected by a second drug, do not change the nucleus color
        }
    }
}

std::vector<std::string> damage_coloring(Cell *pC)
{
    if (all_pd.size() == 0) // then either at initialization or there are actually no PD effects here
    {
        return paint_by_number_cell_coloring(pC);
    }
    std::vector<std::string> output(4, "black");

    if (pC->phenotype.cycle.current_phase().code == PhysiCell_constants::apoptotic)
    { // apoptotic - black
        return output;
    }

    if (pC->phenotype.cycle.current_phase().code != PhysiCell_constants::apoptotic && get_single_signal(pC, "dead") == true)
    { // necrotic - brown
        std::vector<std::string> output(4, "peru");
        return output;
    }

    static int nCD = cell_definitions_by_index.size(); // number of cell types

    static std::vector<std::vector<int>> default_colors;
    static std::vector<std::vector<int>> color_diffs_D1; // red shift
    static std::vector<std::vector<int>> color_diffs_D2; // blue shift
    static std::vector<std::vector<int>> damage_inds;
    static std::vector<std::vector<int>> ec50_vals;
    static std::vector<std::vector<int>> hp_vals;
    static bool colors_initialized = false;

    if (!colors_initialized)
    {
        intialize_damage_coloring(nCD, default_colors, color_diffs_D1, color_diffs_D2, damage_inds, ec50_vals, hp_vals);
        colors_initialized = true;
    }

    std::vector<int> default_color = default_colors[pC->type];
    std::vector<double> color_diffs;
    char colorTempString[128];

    std::vector<double> d_val = {0, 0};
    for (int i = 0; i < damage_inds[pC->type].size(); i++)
    {
        d_val[i] = pC->custom_data[damage_inds[pC->type][i]];
        d_val[i] = Hill_response_function(d_val[i], ec50_vals[pC->type][i], hp_vals[pC->type][i]);
    }

    int rd = (int)round(d_val[0] * color_diffs_D1[pC->type][0]); // red differential
    int gd = (int)round(d_val[0] * color_diffs_D1[pC->type][1]); // green differential
    int bd = (int)round(d_val[0] * color_diffs_D1[pC->type][2]); // blue differential

    sprintf(colorTempString, "rgb(%u, %u, %u)", default_color[0] + rd, default_color[1] + gd, default_color[2] + bd);
    output[0].assign(colorTempString); // cytoplasm

    rd = (int)round(d_val[1] * color_diffs_D2[pC->type][0]); // red differential
    gd = (int)round(d_val[1] * color_diffs_D2[pC->type][1]); // green differential
    bd = (int)round(d_val[1] * color_diffs_D2[pC->type][2]); // blue differential

    sprintf(colorTempString, "rgb(%u, %u, %u)", default_color[0] + rd, default_color[1] + gd, default_color[2] + bd);
    output[2].assign(colorTempString); // nucleus

    return output;
}