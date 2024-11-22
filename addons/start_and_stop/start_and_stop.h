
#include "../../core/PhysiCell.h"
#include "../../modules/PhysiCell_standard_modules.h" 

using namespace BioFVM; 
using namespace PhysiCell;

void reset_microenv();

// Function to save cell data
void save_cell_microenv_data(Cell_Container* cell_container);

void reset_cell(double last_cell_cycle_time);

void reset_global_parameters(Cell_Container* cell_container);

void reset_randomness();