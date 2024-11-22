/*
 * tnf_receptor_dynamics.cpp
 *
 *  Created on: 15 jun. 2020
 *  Author: Miguel Ponce-de-Leon (miguel.ponce@bsc.es)
 *  Contributor: Gerard Pradas
 *  Contributor: Arnau Montagud
 *  Contributor: Thalia Diniaco
 *  Cite as: arXiv:2103.14132 [q-bio.QM]
 *  Description: 
 *      Simplified model of the TNF receptor (TNFR) dynamics. The models has threed states:
 *      1- TNFR unbounded (exposed to the extracellular space)
 * 		2- TNFR with a TNF bounded
 * 		3- TNFR with a TNF bounded internalized to be recycled by removeing the TNF ligand.
 * 
 * 		It is an ODE-based model that has the following equations
 * 		d[unbound_external_TNFR]/dt = TNFR_recycling_rate * [bound_internal_TNFR] - TNFR_binding_rate * [unbound_external_TNFR] * [TNF]
 *      d[bound_external_TNFR]/dt = NFR_binding_rate * [unbound_external_TNFR] * [TNF] - TNFR_endocytosis_rate * [bound_external_TNFR]
 * 		d[bound_internal_TNFR]/dt = TNFR_endocytosis_rate * [bound_external_TNFR] - TNFR_recycling_rate * [bound_internal_TNFR]
 */

#include "./tnf_receptor_dynamics.h" 

using namespace PhysiCell; 

Submodel_Information tnf_receptor_info;

void tnf_receptor_model_setup()
{
    tnf_receptor_info.name = "TNF trasnporter model"; 
	tnf_receptor_info.version = "0.1.0";
	
    tnf_receptor_info.main_function = tnf_receptor_model; 

	// what custom data do I need?
	tnf_receptor_info.cell_variables.push_back( "TNFR_activation_threshold" );

    tnf_receptor_info.cell_variables.push_back( "unbound_external_TNFR" );
	tnf_receptor_info.cell_variables.push_back( "bound_external_TNFR" );
	tnf_receptor_info.cell_variables.push_back( "bound_internal_TNFR" );
    
    tnf_receptor_info.cell_variables.push_back( "TNFR_binding_rate" ); 
	tnf_receptor_info.cell_variables.push_back( "TNFR_endocytosis_rate" );
    tnf_receptor_info.cell_variables.push_back( "TNFR_recycling_rate" );
	tnf_receptor_info.cell_variables.push_back( "TNF_net_production_rate" );

	tnf_receptor_info.register_model();

	return;
}

void tnf_receptor_model( Cell* pCell, Phenotype& phenotype, double dt )
{
	static int nTNF_external = microenvironment.find_density_index( "tnf" );

    static int nR_EU = pCell->custom_data.find_variable_index( "unbound_external_TNFR" ); 
	static int nR_EB = pCell->custom_data.find_variable_index( "bound_external_TNFR" );
	static int nR_IB = pCell->custom_data.find_variable_index( "bound_internal_TNFR" );

    static int nR_bind = pCell->custom_data.find_variable_index( "TNFR_binding_rate" );
	static double R_binding_rate = pCell->custom_data[nR_bind];

	static int nR_endo = pCell->custom_data.find_variable_index( "TNFR_endocytosis_rate" ); 
	static double R_endo_rate = pCell->custom_data[nR_endo];

	static int nR_recycle = pCell->custom_data.find_variable_index( "TNFR_recycling_rate" ); 
	static double R_recyc_rate = pCell->custom_data[nR_recycle];
	

	if( phenotype.death.dead == true )
	{ return; } 
		
    // internalized TNF tells us how many have recently bound to receptors
	// TNF is internalized at:
	// phenotype.secretion.uptake_rates[nTNF_external] = 
	// 					pCell->custom_data[nR_bind] * pCell->custom_data[nR_EU];
	
	// The internalization is only used to track the TNF
	// The following part of the code takes care of correcly managed
	double dR_EB = phenotype.molecular.internalized_total_substrates[nTNF_external];
	//std::cout << "nTNF_external: " << nTNF_external << std::endl;
	//std::cout << "dR_EB: " << dR_EB << std::endl;
	
	// if it tries to bind more TNF than there are receptors, compensate 
	if( dR_EB > pCell->custom_data[nR_EU] )
	{
		double excess_binding = dR_EB - pCell->custom_data[nR_EU];
		dR_EB = pCell->custom_data[nR_EU]; 
		// dump any excess back into the microenvironment
		static double to_density = 1.0 / microenvironment.mesh.dV; 
		// this needs omp critical because 2 cells writing to 1 voxel is not thread safe 
		#pragma omp critical 
		{ pCell->nearest_density_vector()[nTNF_external] += excess_binding * to_density; }
	}

	// Remove all the internalized TNF from cell
	phenotype.molecular.internalized_total_substrates[nTNF_external] = 0.0; 
	
	// Endocytosis 
	// The bounded receptor is internalized at a rate R_endo_rate
	double dR_IB = dt * R_endo_rate  * pCell->custom_data[nR_EB];
    if( dR_IB > pCell->custom_data[nR_EB] )
	{ dR_IB = pCell->custom_data[nR_EB]; }

	// Recylcing
	// The internalized bounded TNFR release the TNF
    // The TNF is instantaneously degraded by the cell
    // The TNF receptor is recycled as an unbounded external receptor
	double dR_EU = dt * R_recyc_rate * pCell->custom_data[nR_IB];
	if( dR_EU > pCell->custom_data[nR_IB] )
	{ dR_EU = pCell->custom_data[nR_IB]; }

	pCell->custom_data[nR_EU] -= dR_EB; // remove newly bound receptor from R_EU 
	pCell->custom_data[nR_EB] += dR_EB; // add newly bound receptor to R_EB

	pCell->custom_data[nR_EB] -= dR_IB; // move from external bound
	pCell->custom_data[nR_IB] += dR_IB; // move to internal bound

	pCell->custom_data[nR_IB] -= dR_EU; // move from internal unbound 
	pCell->custom_data[nR_EU] += dR_EU; // move to external unbound 

	// update the TNF uptake rate 
	//std::cout << "uptake_pre: " << phenotype.secretion.uptake_rates[nTNF_external] << std::endl;
	phenotype.secretion.uptake_rates[nTNF_external] = R_binding_rate * pCell->custom_data[nR_EU]; 
	//std::cout << "uptake_post: " << phenotype.secretion.uptake_rates[nTNF_external] << std::endl;

	return;
}

void tnf_receptor_model_main( double dt )
{
	#pragma omp parallel for 
	for( int i=0; i < (*all_cells).size() ; i++ )
	{
		Cell* pC = (*all_cells)[i]; 
		if (  pC->is_out_of_domain )
		{ continue; }
		if( pC->phenotype.death.dead )
		{ continue; }
		tnf_receptor_model( pC, pC->phenotype , dt );
	}
	return;
}