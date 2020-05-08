#include<stdio.h>
#include "../../core/PhysiCell_cell.h"

using namespace PhysiCell;

Cell_Definition celltype1;

int main(int argc, char** argv)
{
    std::cout << "--------  " << argv[0] << "  --------"  << std::endl;
    std::cout << "------- at start of main:" << std::endl;
	display_cell_definitions( std::cout ); 
    std::cout << std::endl;

    std::cout << "\n------- assigning:  celltype1 = cell_defaults" << std::endl;
    celltype1 = cell_defaults;
    celltype1.name = "celltype1";
    celltype1.type = 42;
    celltype1.phenotype.motility.is_motile = true; 

    build_cell_definitions_maps(); 
	display_cell_definitions( std::cout ); 
    std::cout << std::endl;
}
