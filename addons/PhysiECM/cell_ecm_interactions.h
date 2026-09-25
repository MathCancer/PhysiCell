#ifndef _cell_ecm_interactions_
#define _cell_ecm_interactions_

#include "../../core/PhysiCell.h"
#include "../../modules/PhysiCell_standard_modules.h"
#include "./extracellular_matrix.h"

using namespace BioFVM; 
using namespace PhysiCell;

void check_ecm_remodel_parameters_provided(pugi::xml_node node, Cell_Definition* pCD);
void initialize_ecm_interactions_v1(pugi::xml_node node, Cell_Definition* pCD);
void initialize_ecm_interactions_v2(pugi::xml_node node, Cell_Definition* pCD);

double sign_function(const double number);

void ecm_based_update_cell_velocity(Cell *pCell, Phenotype &phenotype, double dt);

void ecm_to_cell_interactions_v1(Cell *pCell, Phenotype &phenotype, double dt);
void ecm_to_cell_interactions_v2(Cell *pCell, Phenotype &phenotype, double dt);

// uses cell motility vector for realigning ECM.
void ecm_remodeling_function(Cell *pCell, Phenotype &phenotype, double dt);

void ecm_based_update_motility_vector(Cell *pCell, Phenotype &phenotype, double dt_);

void SVG_plot_custom(std::string filename, Microenvironment &M, double z_slice, double time, std::vector<std::string> (*cell_coloring_function)(Cell *), std::string line_pattern);
void write_ecm_Data_matlab(std::string filename);

#endif