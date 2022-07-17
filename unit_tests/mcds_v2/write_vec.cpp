// write binary data from a C++ vector: 
// testing add_PhysiCell_cells_to_open_xml_pugi_v2() in modules/PhysiCell_MultiCellDS.cpp 
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdio>

int main()
{
    std::vector<double> position = {42.1, 42.2, 42.3};

    FILE* fp; 
    std::string filename = "test.mat";  // just mimic the binary Matlab files output for MCDS
    fp = fopen( filename.c_str() , "wb" );
    if( fp == NULL )
    {
        std::cout << "Error: could not open file " << filename << "!" << std::endl;
        return 0;
    }

    fwrite( (char*) &( position ) , sizeof(double) , 3 , fp );   // incorrect
    // std::fwrite( position.data(), sizeof(position[0]) , 3 , fp );   // correct
    return 0;
}