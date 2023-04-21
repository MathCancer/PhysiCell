#ifndef __PHYSIMESS__
#define __PHYSIMESS__

#include "../../core/PhysiCell_cell.h"
#include "../../modules/PhysiCell_pathology.h"

#include <list>

using namespace PhysiCell;

static std::string PhysiMeSS_Version = "1.0.0"; 

bool isFibre(Cell* pCell);
bool isFibre(Cell_Definition * cellDef);

void initialize_physimess_fibre(Cell* pCell, std::vector<double>& position, double Xmin, double Xmax, double Ymin, double Ymax, double Zmin, double Zmax);
void initialize_physimess_cell(Cell* pCell);
void remove_physimess_out_of_bounds_fibres();
void initialize_crosslinkers(Cell* pCell);
void initialize_crosslink_points(Cell* pCell);
void initialize_neighbors(Cell* pCell);
std::vector<Cell*>& get_crosslinkers(Cell*);
std::vector<double>& get_crosslink_point(Cell*);
std::vector<Cell*>& get_neighbors(Cell*);


void add_potentials_cell_to_fibre(Cell* cell, Cell* other_agent);
void add_potentials_fibre_to_cell(Cell* cell, Cell* other_agent);
void add_potentials_fibre_to_fibre(Cell* cell, Cell* other_agent);

std::vector<double> nearest_point_on_fibre(std::vector<double> point, Cell* fibre_agent , std::vector<double>& displacement);
void force_update_motility_vector(Cell* pCell, double dt_);

void add_crosslinks( Cell* pCell );
void check_fibre_crosslinks(Cell* fibre, Cell* fibre_neighbor);

std::list<int> register_fibre_voxels( Cell* pCell );
void deregister_fibre_voxels( Cell* pCell );

std::list<int> find_agent_voxels(Cell * pCell );
void find_agent_neighbors( Cell* pCell );

void physimess_mechanics( double dt );
void physimess_update_cell_velocity( Cell* pCell, Phenotype& phenotype, double dt);


void fibre_agent_SVG(std::ofstream& os, PhysiCell::Cell* pCell, double z_slice, std::vector<std::string> (*cell_coloring_function)(Cell*), double X_lower, double Y_lower);
void fibre_agent_legend(std::ofstream& os, Cell_Definition* cell_definition, double& cursor_x, double& cursor_y, std::vector<std::string> (*cell_coloring_function)(Cell*), double temp_cell_radius);

#endif