/*
 * tnf_receptor_dynamics.cpp
 *
 *  Created on: 15 jun. 2020
 *      Author: Miguel Ponce-de-Leon
 *      Contributor: Thalia Diniaco
 *  Created on: 15 jun. 2020
 *  Author: Miguel Ponce-de-Leon (miguel.ponce@bsc.es)
 *  Contributor: Gerard Pradas
 *  Contributor: Arnau Montagud
 *  Contributor: Thalia Diniaco
 *  Cite as: arXiv:2103.14132 [q-bio.QM]
 *  Description: 
 *      SSimplified model of the TNF receptor (TNFR) dynamics. The models has threed states:
 *      1- TNFR unbounded (exposed to the extracellular space)
 * 		2- TNFR with a TNF bounded
 * 		3- TNFR with a TNF bounded internalized to be recycled by removeing the TNF ligand.
 * 
 * 		It is an ODE-based model that has the following equations
 * 		d[unbound_external_TNFR]/dt = TNFR_recycling_rate * [bound_internal_TNFR] - TNFR_binding_rate * [unbound_external_TNFR] * [TNF]
 *      d[bound_external_TNFR]/dt = NFR_binding_rate * [unbound_external_TNFR] * [TNF] - TNFR_endocytosis_rate * [bound_external_TNFR]
 * 		d[bound_internal_TNFR]/dt = TNFR_endocytosis_rate * [bound_external_TNFR] - TNFR_recycling_rate * [bound_internal_TNFR]
 * 
 *  Cite as: arXiv:2103.14132 [q-bio.QM]
 */

#include "../core/PhysiCell.h"
#include "../modules/PhysiCell_standard_modules.h" 

using namespace BioFVM; 
using namespace PhysiCell;

#include "./submodel_data_structures.h" 

void tnf_receptor_model_setup();

void tnf_receptor_model( Cell* pCell, Phenotype& phenotype, double dt );

void tnf_receptor_model_main( double dt );
