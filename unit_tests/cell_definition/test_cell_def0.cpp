#include<stdio.h>
#include "../../core/PhysiCell_cell.h"

using namespace PhysiCell;

int main(int argc, char** argv)
{
    std::cout << "--------  " << argv[0] << "  --------"  << std::endl;
    // build_cell_definitions_maps(); 
	display_cell_definitions( std::cout ); 
    std::cout << std::endl;
}
