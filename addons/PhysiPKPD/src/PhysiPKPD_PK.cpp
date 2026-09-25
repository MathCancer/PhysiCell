#include <iostream>
#include <fstream>
#include "./PhysiPKPD_PK.h"

#ifdef ADDON_ROADRUNNER // librr_intracellular.h will protect against redefining this
#include "../../libRoadrunner/src/librr_intracellular.h"
SBML_PK_Solver::SBML_PK_Solver()
{ return; }

void SBML_PK_Solver::advance(Pharmacokinetics_Model *pPK, double current_time)
{
	// simulate SBML
	rrc::simulateEx(rrHandle, current_time, current_time+diffusion_dt, 2);

    return;
}
#endif

static double tolerance = 0.01 * diffusion_dt; // using this in PK_model and write_cell_data_for_plots for determining when to do these

AnalyticConstant_PK_Solver::AnalyticConstant_PK_Solver()
{ return; }

Analytic2C_PK_Solver::Analytic2C_PK_Solver()
{ return; }

Analytic1C_PK_Solver::Analytic1C_PK_Solver()
{ return; }

Pharmacokinetics_Solver::Pharmacokinetics_Solver()
{ return; }

Pharmacokinetics_Model::Pharmacokinetics_Model()
{ return; }

Pharmacokinetics_Model *create_pk_model(int substrate_index, std::string substrate_name)
{
    Pharmacokinetics_Model *pNew = create_pk_model(substrate_index);
    pNew->substrate_name = substrate_name;

    return pNew;
}

Pharmacokinetics_Model *create_pk_model(int substrate_index)
{
    Pharmacokinetics_Model *pNew = create_pk_model();
    pNew->substrate_index = substrate_index;
    return pNew;
}

Pharmacokinetics_Model *create_pk_model(void)
{
    Pharmacokinetics_Model *pNew;
    pNew = new Pharmacokinetics_Model;
    return pNew;
}

void setup_pk_model(Pharmacokinetics_Model *pNew, pugi::xml_node pk_node)
{
    pNew->biot_number = pk_node.child("biot_number").text().as_double();

    void (*setup_function)(Pharmacokinetics_Model *pNew, pugi::xml_node pk_node);
    std::string model = pk_node.child("model").text().as_string();

    std::vector<std::string> current_options = {"Constant","2C", "1C", "SBML"};
    std::vector<void (*)(Pharmacokinetics_Model *, pugi::xml_node pk_node)> fns;
    fns.push_back(&setup_pk_model_constant);
    fns.push_back(&setup_pk_model_two_compartment);
    fns.push_back(&setup_pk_model_one_compartment);
    fns.push_back(NULL); // handle SBML case separately

    bool model_found = false;
    for (int i = 0; i < current_options.size(); i++)
    {
        if (model == current_options[i])
        {
            setup_function = fns[i];
            model_found = true;
            break;
        }
    }
    if (!model_found)
    {
        std::cout << "PhysiPKPD ERROR: " << pNew->substrate_name + " is set to follow " + pk_node.child("model").text().as_string() + " but this is not an allowable option." << std::endl
                  << "  Current options include: " << current_options << std::endl;
        exit(-1);
    }

    if (setup_function)
    {
        setup_function(pNew, pk_node);
        // return pNew;
        return;
    }

#ifdef ADDON_ROADRUNNER
    if (model == "SBML")
    {
        SBML_PK_Solver *pSolver;
        pSolver = new SBML_PK_Solver;

        // Read SBML for PK model
        pSolver->rrHandle = createRRInstance(); // creating rrHandle to save SBML in it
        rrc::RRCDataPtr result;

        // reading given SBML
        std::string sbml_folder = "./config/"; // default folder for SBML files
        if (!(pk_node.child("sbml_folder")))
        {
            std::cout << "PhysiPKPD WARNING: No SBML folder provided for " << pNew->substrate_name << "." << std::endl
                      << "  You may include a folder as a string in " << "sbml_folder" << std::endl
                      << "  For now, PhysiPKPD will use " << sbml_folder << " as the default folder for SBML files." << std::endl
                      << std::endl;
        }
        else
        {
            sbml_folder = pk_node.child("sbml_folder").text().as_string();
            // check that sbml_folder ends in a file separator
            if (sbml_folder.back() != '/')
            {
                sbml_folder += "/";
            }
        }
        std::string sbml_filename = "PK_default.xml";
        if (!(pk_node.child("sbml_filename")))
        {
            std::cout << "PhysiPKPD WARNING: No SBML filename provided for " << pNew->substrate_name << "." << std::endl
                      << "  You may include a filename as a string in " << "sbml_filename" << std::endl
                      << "  Place that file in " << sbml_folder << " for PhysiPKPD to properly locate it." << std::endl
                      << "  For now, PhysiPKPD will use " + sbml_folder + "/" + sbml_filename << std::endl
                      << std::endl;
        }
        else
        {
            sbml_filename = pk_node.child("sbml_filename").text().as_string();
        }
        sbml_filename = sbml_folder + sbml_filename;
        char sbml[sbml_filename.length()+1];
        strcpy(sbml, sbml_filename.c_str());

        if (!rrc::loadSBML(pSolver->rrHandle, sbml)) //------------- To PhysiPKPD Team : please provide PK model in here -------------
        {
            std::cout << "PhysiPKPD ERROR: Could not load SBML file for " << pNew->substrate_name << ". " << std::endl
                      << "  Make sure that " + sbml_filename << " is the correct filename for your SBML model." << std::endl
                      << std::endl;

            exit(-1);
        }

        // get species names for dosing purposes and for connecting to PhysiCell
        std::string species_names_str = stringArrayToString(rrc::getFloatingSpeciesIds(pSolver->rrHandle));
        std::cerr << species_names_str << "\n"
                  << std::endl;
        std::stringstream iss(species_names_str);
        std::string species_name;
        int idx = 0;
        while (iss >> species_name)
        {
            pSolver->species_result_column_index[species_name] = idx;
            std::cout << species_name << " -> " << idx << std::endl;
            idx++;
        }

        // make sure circulation_concentration is one of the species
        auto out = pSolver->species_result_column_index.find("circulation_concentration");
        if (out == pSolver->species_result_column_index.end())
        {
            std::cout << "PhysiPKPD ERROR: No species named circulation_concentration in the SBML for PK dynamics of " << pNew->substrate_name << std::endl
                      << "  Either change " << std::endl
                      << "    " << pNew->substrate_name + "_pk_model"
                      << " to not be SBML," << std::endl
                      << "    point to the correct SBML file with " << pNew->substrate_name + "_sbml_filename" << std::endl
                      << "    or fix the SBML file here: " << sbml_filename << std::endl
                      << std::endl;
            exit(-1);
        }

        // add dosing events?
        if (pk_node.child("schedule").attribute("format").as_string()=="csv")
        {
            // read in csv into events for the xml file
            std::cout << "PhysiPKPD WARNING: Reading in a dosing schedule from a CSV is not yet supported." << std::endl
                      << "  Will use " << sbml_filename << " as is for the PK dynamics of " << pNew->substrate_name << std::endl
                      << std::endl;

        }
        pNew->dosing_schedule_setup_done = true;


        pNew->pk_solver = pSolver;
        // return pNew;
        return;
    }
#endif

    std::cout << "PhysiPKPD ERROR: No PK solver found for " << pNew->substrate_name << std::endl
              << "  We tried looking for " + model + ", but somehow failed." << std::endl;
    exit(-1);
}

void PK_model(double current_time)
{
    static std::vector<Pharmacokinetics_Model *> all_pk;
    static bool need_to_setup = true;
    if (need_to_setup)
    {
        parse_config_for_pk(all_pk);
        need_to_setup = false;
    }

    std::vector<double> dc_temp;
    for (int n = 0; n < all_pk.size(); n++)
    {
        if (!all_pk[n]->dosing_schedule_setup_done)
        {
            setup_pk_single_dosing_schedule(all_pk[n], current_time);
        }
        all_pk[n]->pk_solver->advance(all_pk[n], current_time);
        dc_temp.push_back(all_pk[n]->get_circulation_concentration() * all_pk[n]->biot_number);
    }

#pragma omp parallel for
    for (unsigned int i = 0; i < microenvironment.mesh.voxels.size(); i++)
    {
        if (microenvironment.mesh.voxels[i].is_Dirichlet == true)
        {
            for (unsigned int n = 0; n < all_pk.size(); n++)
            {
                if (microenvironment.get_substrate_dirichlet_activation(all_pk[n]->substrate_index, i)) // no guarantee that the ordering in the user parameters matches the indexing order
                {
                    microenvironment.update_dirichlet_node(i, all_pk[n]->substrate_index, dc_temp[n]);
                }
            }
        }
    }
}

void parse_config_for_pk(std::vector<Pharmacokinetics_Model *> &all_pk)
{
    std::vector<std::string> PK_names;
    static std::vector<int> PK_ind;
    pugi::xml_node me_setup_node = PhysiCell::physicell_config_root.child("microenvironment_setup");
    pugi::xml_node variable_node = me_setup_node.child("variable");
    while (variable_node)
    {
        pugi::xml_node pk_node = variable_node.child("PK");
        std::string pk_enabled = pk_node.attribute("enabled").as_string();
        if (pk_node && pk_enabled == "true")
        {
            std::string substrate_name = variable_node.attribute("name").as_string();
            int substrate_index = variable_node.attribute("ID").as_int();
            Pharmacokinetics_Model *pNew = create_pk_model(substrate_index, substrate_name);
            setup_pk_model(pNew, pk_node);
            all_pk.push_back(pNew);
        }
        variable_node = variable_node.next_sibling("variable");
    }
}

void setup_pk_model_constant(Pharmacokinetics_Model *pPK, pugi::xml_node pk_node)
{
    AnalyticConstant_PK_Solver *pSolver;
    pSolver = new AnalyticConstant_PK_Solver;

    pSolver->circulation_concentration = 0;
    pPK->pk_solver = pSolver;
    return;
}

void setup_pk_model_two_compartment(Pharmacokinetics_Model *pPK, pugi::xml_node pk_node)
{
    Analytic2C_PK_Solver *pSolver;
    pSolver = new Analytic2C_PK_Solver;

    // pk parameters
    double k12 = pk_node.child("k12").text().as_double();
    double k21 = pk_node.child("k21").text().as_double();
    double R = pk_node.child("volume_ratio").text().as_double();
    double l = pk_node.child("elimination_rate").text().as_double();

    if (k12==0 || k21==0 || R==0)
    {
        std::cout << "PhysiPKPD WARNING: Because at least one of the following PK parameters for " << pPK->substrate_name << " is 0: k12=" << k12 << ", k21=" << k21 << ", R=" << R << std::endl
                  << "  This model will be treated as a 1-compartment model by updating the current central elimination rate to lambda = lambda + k12." << std::endl;

        delete pSolver;

        Analytic1C_PK_Solver *pSolver_1C;
        pSolver_1C = new Analytic1C_PK_Solver;

        pSolver_1C->M = exp(-(l+k12) * diffusion_dt); // M is defined as a vector of vectors, hence this expression

        pSolver_1C->circulation_concentration = 0.0;
        pPK->pk_solver = pSolver_1C;

        return;
    }

    // pre-computed quantities to express solution to matrix exponential
    double beta = sqrt((k12 + k21) * (k12 + k21) + 2 * l * (k12 - k21) + l * l);
    if (beta == 0) // with the above if statement, this block should never trigger
    { 
        std::cout << "PhysiPKPD WARNING: " << pPK->substrate_name << " has PK parameters that cannot used by this framework." << std::endl;
        std::cout << "  This is because k12=0 and k21=elimination rate." << std::endl;
        std::cout << "  Since k12=0 means the periphery never fills up, k21 is meaningless." << std::endl;
        std::cout << "  Will change k21 to make this work." << std::endl;
        std::cout << "  By the way, this should have triggered a switch to a 1-compartment model. Why did you end up here?" << std::endl;
        k21 += 1.0;
        beta = sqrt((k12 + k21) * (k12 + k21) + 2 * l * (k12 - k21) + l * l);
    }
    double alpha = k12 - k21 + l;
    double a = -0.5 * (k12 + k21 + l);
    double b = 0.5 * beta;
    std::vector<double> ev = {a - b, a + b}; // eigenvalues
    std::vector<double> decay = {exp(ev[0] * diffusion_dt), exp(ev[1] * diffusion_dt)};
    pSolver->M[0][0] = -0.5 * (alpha * (decay[1] - decay[0]) - beta * (decay[0] + decay[1])) / beta;
    pSolver->M[0][1] = k21 * (decay[1] - decay[0]) / (beta * R);
    pSolver->M[1][0] = R * k12 * (decay[1] - decay[0]) / beta;
    pSolver->M[1][1] = -0.5 * (alpha * (decay[0] - decay[1]) - beta * (decay[0] + decay[1])) / beta;
    // std::cout << "M = [" << pSolver->M[0][0] << "," << pSolver->M[0][1] << ";" << pSolver->M[1][0] << "," << pSolver->M[1][1] << "]" << std::endl;
    pSolver->compartment_concentrations = {0, 0};

    pPK->pk_solver = pSolver;
    return;
}

void setup_pk_model_one_compartment(Pharmacokinetics_Model *pPK, pugi::xml_node pk_node)
{
    Analytic1C_PK_Solver *pSolver;
    pSolver = new Analytic1C_PK_Solver;

    // pk parameters
    double l = pk_node.child("elimination_rate").text().as_double();

    // pre-computed quantities to express solution to matrix exponential
    pSolver->M = exp(-l * diffusion_dt);
    // std::cout << "M = " << pSolver->M << std::endl;

    pSolver->circulation_concentration = 0;
    pPK->pk_solver = pSolver;
    return;
}

void setup_pk_single_dosing_schedule(Pharmacokinetics_Model *pPK, double current_time)
{
    static pugi::xml_node me_setup_node = PhysiCell::physicell_config_root.child("microenvironment_setup");
    if (!pPK->dosing_schedule_setup_done)
    {
        pugi::xml_node var_node = me_setup_node.child("variable");

        while (var_node)
        {
            std::string name = var_node.attribute("name").as_string();
            if (name == pPK->substrate_name)
            {
                break;
            }
            var_node = var_node.next_sibling("variable");
        }
        pugi::xml_node pk_node = var_node.child("PK");
        pugi::xml_node schedule_node = pk_node.child("schedule");
        bool setup_dosing_now;
        std::string format = schedule_node.attribute("format").as_string();
        bool read_csv = format == "csv";
        // read_csv = parameters.bools.find_index(pPK->substrate_name + "_read_dose_schedule_from_csv") != -1 && parameters.bools(pPK->substrate_name + "_read_dose_schedule_from_csv");
        setup_dosing_now = read_csv // read dose schedule from csv
                           || (!(schedule_node.child("confluence_start"))) && (!(schedule_node.child("confluence_condition"))) // start if user did not specify any of the parameters to determine when to start
                           || (schedule_node.child("confluence_start")) && !(schedule_node.child("confluence_start").text().as_bool())                                                                                                                                // start if user parameter says to set first dose time
                           || ((schedule_node.child("confluence_condition")) && (current_time > pPK->pk_solver->confluence_check_time - tolerance) && (confluence_computation() > schedule_node.child("confluence_condition").text().as_double()));                         // start if confluence check is given, its time for it, and if the confluence condition is met
        // setup_dosing_now = read_csv // read dose schedule from csv
        //                    || (parameters.bools.find_index(pPK->substrate_name + "_set_first_dose_time") == -1) && (parameters.doubles.find_index(pPK->substrate_name + "_confluence_condition") == -1) // start if user did not specify any of the parameters to determine when to start
        //                    || (parameters.bools.find_index(pPK->substrate_name + "_set_first_dose_time") != -1) && parameters.bools(pPK->substrate_name + "_set_first_dose_time")                                                                                                                                   // start if user parameter says to set first dose time
        //                    || (parameters.doubles.find_index(pPK->substrate_name + "_confluence_condition") != -1 && (current_time > pPK->pk_solver->confluence_check_time - tolerance) && (confluence_computation() > parameters.doubles(pPK->substrate_name + "_confluence_condition")));                         // start if confluence check is given, its time for it, and if the confluence condition is met
        if (setup_dosing_now)
        {
            if (read_csv)
            {
                std::string filename;
                if (!(schedule_node.child("folder")) || !(schedule_node.child("filename")))
                {
                    std::cout << "PhysiPKPD WARNING: Reading in a dosing schedule from a CSV file, but no folder or filename is given." << std::endl
                              << "\tWill look for it in ./config/" << pPK->substrate_name + "_dose_schedule.csv" << std::endl
                              << std::endl
                              << "\tSpecify this in the config by updating the <schedule> element to:" << std::endl
                              << "\t<schedule format=\"csv\">" << std::endl
                              << "\t  <folder>path/to/folder</folder>" << std::endl
                              << "\t  <filename>file.csv</filename>" << std::endl
                              << "\t</schedule>" << std::endl;

                    filename = "./config/" + pPK->substrate_name + "_dose_schedule.csv";
                }
                else
                {
                    std::string folder = schedule_node.child("folder").text().as_string();
                    filename = folder + "/" + schedule_node.child("filename").text().as_string();
                }

                std::cout << "PhysiPKPD Update: Reading in dosing schedule from " << filename << "." << std::endl;

                std::ifstream file(filename, std::ios::in);

                if (!file)
                {
                    std::cout << "PhysiPKPD ERROR: " << filename << " not found in ./config/. " << pPK->substrate_name + "_read_dose_schedule_from_csv" << " set to true in config file." << std::endl;
                    exit(-1);
                }

                std::string line;
                while (std::getline(file, line))
                {
                    std::vector<double> data;
                    csv_to_vector(line.c_str(), data);

                    if (data.size() != 2)
                    {
                        std::cout << "PhysiPKPD Error: Importing dosing schedule from a CSV file expects each row to be time,amount." << std::endl;
                        exit(-1);
                    }

                    pPK->pk_solver->dose_times.push_back(data[0]);
                    pPK->pk_solver->dose_amounts.push_back(data[1]);
                }
                pPK->pk_solver->max_doses = pPK->pk_solver->dose_times.size();

                std::cout << "\tPhysiPKPD Update: Finished reading in dosing schedule from " << filename << "." << std::endl;
                std::cout << "\tPhysiPKPD Update: Found " << pPK->pk_solver->max_doses << " doses." << std::endl;
                std::cout << "\tPhysiPKPD Update: Dose times are: " << pPK->pk_solver->dose_times << std::endl;
                std::cout << "\tPhysiPKPD Update: Dose amounts are: " << pPK->pk_solver->dose_amounts << std::endl;

                file.close();
            }
            else
            {
                pPK->pk_solver->max_doses = schedule_node.child("total_doses").text().as_int();

                if (pPK->pk_solver->max_doses == 0)
                {
                    pPK->dosing_schedule_setup_done = true;
                    return;
                }

                pPK->pk_solver->dose_times.resize(pPK->pk_solver->max_doses, 0);
                pPK->pk_solver->dose_amounts.resize(pPK->pk_solver->max_doses, 0);
                if ((schedule_node.child("confluence_start")) && schedule_node.child("confluence_start").text().as_bool() && !(schedule_node.child("first_dose_time")))
                {
                    std::cout << "PhysiPKPD WARNING: " << pPK->substrate_name << " has a set time for the first dose, but the first time is not supplied. Assuming to begin now = " << current_time << std::endl
                              << "  This can be set by using " << pPK->substrate_name << "_first_dose_time" << std::endl;
                }
                double first_dose_time = current_time;
                read_time_parameter(first_dose_time, "first_dose_time", schedule_node);
                double dose_interval = 1440;
                read_time_parameter(dose_interval, "dose_interval", schedule_node);

                pPK->pk_solver->dose_times[0] = ((schedule_node.child("confluence_start")) && !(schedule_node.child("confluence_start").text().as_bool())) ? ((schedule_node.child("first_dose_time")) ? schedule_node.child("first_dose_time").text().as_double() : current_time) : current_time; // if not setting the first dose time, then the confluence condition is met and start dosing now; also if the defining parameters are not set, then set it to be the current time
                for (unsigned int i = 1; i < pPK->pk_solver->max_doses; i++)
                {
                    if (!(schedule_node.child("dose_interval"))) // put this in here so that if only one dose is given, then this won't be checked
                    {
                        std::cout << "PhysiPKPD ERROR: " << pPK->substrate_name << " has multiple doses but no dose interval is given." << std::endl
                                  << "  Set " << pPK->substrate_name << "_dose_interval" << std::endl;
                        exit(-1);
                    }
                    pPK->pk_solver->dose_times[i] = pPK->pk_solver->dose_times[i - 1] + dose_interval;
                }
                int num_loading = (schedule_node.child("loding_doses")) ? schedule_node.child("loading_doses").text().as_int(): 0;
                double loading_dose;
                if (num_loading != 0)
                {
                    if (schedule_node.child("loading_dose"))
                    {
                        loading_dose = schedule_node.child("loading_dose").text().as_double();
                    }
                    else
                    {
                        std::cout << "PhysiPKPD ERROR: " << pPK->substrate_name << " has loading doses but no loading dose amount is given." << std::endl
                                  << "  Set " << pPK->substrate_name << "_central_increase_on_loading_dose" << std::endl;
                        exit(-1);
                    }
                }
                double dose;
                if (pPK->pk_solver->max_doses > num_loading)
                {
                    if (schedule_node.child("regular_dose"))
                    {
                        dose = schedule_node.child("regular_dose").text().as_double();
                    }
                    else
                    {
                        std::cout << "PhysiPKPD ERROR: " << pPK->substrate_name << " has normal doses but no normal dose amount is given." << std::endl
                                  << "  Set " << pPK->substrate_name << "_central_increase_on_dose" << std::endl;
                        exit(-1);
                    }
                }
                for (unsigned int i = 0; i < pPK->pk_solver->max_doses; i++)
                {
                    pPK->pk_solver->dose_amounts[i] = i < num_loading ? loading_dose : dose;
                }
            }
            pPK->dosing_schedule_setup_done = true;
        }
        else if (current_time > pPK->pk_solver->confluence_check_time - tolerance)
        {
            if (!(schedule_node.child("confluence_condition")))
            {
                std::cout << "PhysiPKPD ERROR: Not setting first dose time of " << pPK->substrate_name << " means to use a confluence check to start dosing." << std::endl
                          << "  However, no confluence condition is supplied. Set " << pPK->substrate_name + "_confluence_condition to a value between 0 and 1." << std::endl
                          << std::endl;
                exit(-1);
            }
            pPK->pk_solver->confluence_check_time += phenotype_dt;
        }
    }
    return;
}

void read_time_parameter(double &time, const pugi::char_t* par_name, pugi::xml_node node)
{
    if (node.child(par_name))
    {
        pugi::xml_node time_node = node.child(par_name);
        time = time_node.text().as_double();
        std::string units = "min";
        if (time_node.attribute("units"))
        {
            units = time_node.attribute("units").as_string();
        }
        if (units == "min" || units == "minutes" || units == "minute")
        {
        } // assume it is in units
        else if (units == "hours" || units == "h" || units == "hour")
        {
            time *= 60.0;
        }
        else if (units == "days" || units == "d" || units == "day")
        {
            time *= 1440.0;
        }
        else
        {
            std::cout << "PhysiPKPD ERROR: Units specified for " << par_name << " is not recognized." << std::endl
                      << "\tThe following are accepted: {min, minutes, minute, hours, h, hour, days, d, day}" << std::endl;
            exit(-1);
        }
    }
    return;
}

void AnalyticConstant_PK_Solver::advance(Pharmacokinetics_Model *pPK, double current_time)
{
    // set value if time for that
    if (dose_count < max_doses && current_time > dose_times[dose_count] - tolerance)
    {
        circulation_concentration = dose_amounts[dose_count];
        dose_count++;
        std::cout << "PhysiPKPD Update: Dosing " << dose_amounts[dose_count] << " to " << pPK->substrate_name << " at time " << current_time << "." << std::endl
                  << "\tUsing the constant PK model, so setting the circulation concentration to this amount." << std::endl;
    }
    return;
}

void Analytic2C_PK_Solver::advance(Pharmacokinetics_Model *pPK, double current_time)
{
    // add dose if time for that
    if (dose_count < max_doses && current_time > dose_times[dose_count] - tolerance)
    {
        compartment_concentrations[0] += dose_amounts[dose_count];
        dose_count++;
        std::cout << "PhysiPKPD Update: Dosing " << dose_amounts[dose_count] << " to " << pPK->substrate_name << " at time " << current_time << "." << std::endl
                  << "\tUsing the 2-compartment PK model, so adding this amount to the central compartment concentration resulting in a central concentration of " << compartment_concentrations[0] << "." << std::endl;
    }

    // store previous quantities for computation
    std::vector<double> previous_compartment_concentrations = compartment_concentrations;

    compartment_concentrations[0] = M[0][0] * previous_compartment_concentrations[0] + M[0][1] * previous_compartment_concentrations[1];
    compartment_concentrations[1] = M[1][0] * previous_compartment_concentrations[0] + M[1][1] * previous_compartment_concentrations[1];

    return;
}

void Analytic1C_PK_Solver::advance(Pharmacokinetics_Model *pPK, double current_time)
{
    // add dose if time for that
    if (dose_count < max_doses && current_time > dose_times[dose_count] - tolerance)
    {
        circulation_concentration += dose_amounts[dose_count];
        dose_count++;
        std::cout << "PhysiPKPD Update: Dosing " << dose_amounts[dose_count] << " to " << pPK->substrate_name << " at time " << current_time << "." << std::endl
                  << "\tUsing the 1-compartment PK model, so adding this amount to the circulation concentration resulting in a concentration of " << circulation_concentration << "." << std::endl;
    }

    circulation_concentration = M * circulation_concentration;
    return;
}

// compute confluence as total cellular volume divided by 2D area of TME
double confluence_computation(void)
{
    // this function is not made for 3D simulations
    double output = 0;
    Cell *pC = NULL;
    double cV;
    for (int i = 0; i < (*all_cells).size(); i++)
    {
        pC = (*all_cells)[i];
        cV = pC->phenotype.volume.total; // stop here if using cell volume for confluence
        if (!std::isnan(cV))             // only do these calculations for cells that have a volume
        {
            cV *= 0.75;              // (3/4)V
            cV *= cV;                // ( (3/4)V )^2
            cV *= 3.141592653589793; // since not all computers know what pi is @drbergman M_PI; // pi * ( (3/4)V )^2
            cV = cbrt(cV);           // pi^(1/3) * ( (3/4)V )^(2/3) <--formula for converting volume of sphere with radius r to area of circle with radius r
            output += cV;
        }
    }

    output /= microenvironment.mesh.bounding_box[3] - microenvironment.mesh.bounding_box[0];
    output /= microenvironment.mesh.bounding_box[4] - microenvironment.mesh.bounding_box[1];
    //    output /= microenvironment.mesh.bounding_box[5] - microenvironment.mesh.bounding_box[2]; // use this if doing a 3D check for confluence (see choice of cell volume/area above)
    
    std::cout << "PhysiPKPD Update: Confluence currently at " << output << "." << std::endl << std::endl;

    return output;
}

void write_cell_data_for_plots(double current_time, char delim = ',')
{
    // Write cell number data to a CSV file format time,tumor_cell_count
    // Can add different classes of tumor cells - apoptotic, necrotic, hypoxic, etc to this

    static double next_write_time = 0;
    static double csv_data_interval = parameters.doubles.find_index("csv_data_interval") == -1 ? 9e9 : parameters.doubles("csv_data_interval");
    if (current_time > next_write_time - tolerance)
    {
        // std::cout << "TIMEEEE" << current_time << std::endl;
        double data_time = current_time;
        char dataFilename[256];
        sprintf(dataFilename, "%s/cell_counts.csv", PhysiCell_settings.folder.c_str());

        int tumorCount = 0;
        Cell *pC = NULL;

        for (int i = 0; i < (*all_cells).size(); i++)
        {
            pC = (*all_cells)[i];
            if ((pC->type == 0 || pC->type == 1) && get_single_signal(pC, "dead") == false)
            {
                tumorCount += 1;
            }
        }

        char dataToAppend[1024];
        sprintf(dataToAppend, "%0.2f%c%d", data_time, delim, tumorCount);
        // std::cout << "DATAAAAAA::: " << dataToAppend << std::endl;

        // append to file
        std::ofstream file_out;

        file_out.open(dataFilename, std::ios_base::app);
        if (!file_out)
        {
            std::cout << "PhysiPKPD ERROR: Could not open file " << dataFilename << "!" << std::endl;
            return;
        }
        file_out << dataToAppend << std::endl;
        file_out.close();
        next_write_time += csv_data_interval;
    }
    return;
}

/* these could be used in the future if any one is ever desperate to get numerical errors in their 1- and 2-compartment models
void pk_explicit_euler_one_compartment( double dt, double &central_concentration, double elimination_rate )
{
    central_concentration -= dt * elimination_rate * central_concentration;
    if (central_concentration < 0) {central_concentration = 0;}
}

void pk_explicit_euler_two_compartment( double dt, double &periphery_concentration, double &central_concentration, double elimination_rate, double k12, double k21, double central_to_periphery_volume_ratio )
{
    double central_change_rate = -1 * elimination_rate * central_concentration;
    central_change_rate -= k12 * central_concentration;
    central_change_rate += k21 * periphery_concentration / central_to_periphery_volume_ratio;

    double periphery_change_rate = -k21 * periphery_concentration;
    periphery_change_rate += k12 * central_to_periphery_volume_ratio * central_concentration;

    central_concentration += central_change_rate * dt;
    periphery_concentration += periphery_change_rate * dt;

    if (central_concentration < 0) {central_concentration = 0;}
    if (periphery_concentration < 0) {periphery_concentration = 0;}
}
*/
