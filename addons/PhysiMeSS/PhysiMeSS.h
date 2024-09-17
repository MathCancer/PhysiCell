#ifndef __PHYSIMESS__
#define __PHYSIMESS__

#include "../../core/PhysiCell_cell.h"
#include "../../modules/PhysiCell_pathology.h"
#include "PhysiMeSS_cell.h"
#include "PhysiMeSS_fibre.h"

#include <list>

using namespace PhysiCell;

static std::string PhysiMeSS_Version = "1.0.1"; 

void remove_physimess_out_of_bounds_fibres();

void physimess_mechanics( double dt );
void physimess_update_cell_velocity( Cell* pCell, Phenotype& phenotype, double dt);

void fibre_agent_SVG(std::ofstream& os, PhysiCell::Cell* pCell, double z_slice, std::vector<std::string> (*cell_coloring_function)(Cell*), double X_lower, double Y_lower);
void fibre_agent_legend(std::ofstream& os, Cell_Definition* cell_definition, double& cursor_x, double& cursor_y, std::vector<std::string> (*cell_coloring_function)(Cell*), double temp_cell_radius);

#endif
