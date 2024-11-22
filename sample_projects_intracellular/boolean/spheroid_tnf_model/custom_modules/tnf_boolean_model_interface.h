/*
 * tnf_boolean_model_interface.cpp
 *
 *  Created on: 15 jun. 2020
 *  Author: Miguel Ponce-de-Leon (miguel.ponce@bsc.es)
 *  Contributor: Gerard Pradas
 *  Contributor: Arnau Montagud
 *  Contributor: Thalia Diniaco
 *  Cite as: arXiv:2103.14132 [q-bio.QM]
 *  Description: 
 *      Submodel that work as an interface 
 *      between the Boolean Network (BN) and PhysiCell (PC). 
 *      The submodel run the following steps:
 *      1- updates BN input nodes based on custom cell variables (see receptor model)
 *      2- updates the BN intracellular model by running MaBoSS
 *      3- updates cell state/behaviour based on the state of the BN readout nodes
 *  
 *      The update_monitor_variables funtion is just used to keep track of some relevand
 *      BN nodes' values that are stored as custom variables
 */

#include "../core/PhysiCell.h"
#include "../modules/PhysiCell_standard_modules.h" 
#include "../addons/PhysiBoSS/src/maboss_intracellular.h" 

using namespace BioFVM; 
using namespace PhysiCell;

#include "./submodel_data_structures.h" 

void tnf_boolean_model_interface_setup();

void update_boolean_model_inputs( Cell* pCell, Phenotype& phenotype, double dt );

void update_behaviors(Cell* pCell, Phenotype& phenotype, double dt);

void update_cell_from_boolean_model(Cell* pCell, Phenotype& phenotype, double dt);

// helper function to keep updated some cell custom variables
void update_monitor_variables( Cell* pCell );

extern int myCounter;
