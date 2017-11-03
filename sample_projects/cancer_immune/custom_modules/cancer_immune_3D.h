#include "../core/PhysiCell.h"
#include "../modules/PhysiCell_standard_modules.h" 

using namespace BioFVM; 
using namespace PhysiCell;

// custom cell phenotype function to scale immunostimulatory factor with hypoxia 
void tumor_cell_phenotype_with_and_immune_stimulation( Cell* pCell, Phenotype& phenotype, double dt ); 

extern Cell_Definition immune_cell; 

void create_immune_cell_type( void ); 

// set the tumor cell properties, then call the function 
// to set up the tumor cells 
void create_cell_types( void );

void setup_tissue(); 

void introduce_immune_cells( void ); 

// set up the microenvironment to include the immunostimulatory factor 
void setup_microenvironment( void );   

std::vector<std::string> cancer_immune_coloring_function( Cell* );

// cell rules for extra elastic adhesion

void attach_cells( Cell* pCell_1, Cell* pCell_2 );
void dettach_cells( Cell* pCell_1 , Cell* pCell_2 );

void add_elastic_velocity( Cell* pActingOn, Cell* pAttachedTo , double elastic_constant ); 
void extra_elastic_attachment_mechanics( Cell* pCell, Phenotype& phenotype, double dt );

// immune cell functions for attacking a cell 
Cell* immune_cell_check_neighbors_for_attachment( Cell* pAttacker , double dt ); 
bool immune_cell_attempt_attachment( Cell* pAttacker, Cell* pTarget , double dt ); // only attack if oncoprotein 
bool immune_cell_attempt_apoptosis( Cell* pAttacker, Cell* pTarget, double dt ); 
bool immune_cell_trigger_apoptosis( Cell* pAttacker, Cell* pTarget ); 

void immune_cell_rule( Cell* pCell, Phenotype& phenotype, double dt ); 



void immune_cell_attach( Cell* pAttacker, Cell* pTarget ); // use attach_cells?? 
void immune_cell_dettach( Cell* pAttacker, Cell* pTarget ); // use dettach_cells ?? 


// immune cell functions for motility 

void immune_cell_motility( Cell* pCell, Phenotype& phenotype, double dt ); 


