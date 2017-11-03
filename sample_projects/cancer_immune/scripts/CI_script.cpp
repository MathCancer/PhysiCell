#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <cmath>
#include <omp.h>
#include <fstream>

// set number of threads for OpenMP (parallel computing)
int omp_num_threads = 8; // set this to # of CPU cores x 2 (for hyperthreading)

int main( int argc, char* argv[] )
{
	// OpenMP setup
	omp_set_num_threads(omp_num_threads);

	#pragma omp parallel for 
	for( int i= atoi( argv[1] ) ; i <= atoi( argv[2] ); i++ )
	{
		char str [1024]; 
		sprintf( str , "CI_vis %i", i ); 
		system( str ); 
		
		std::cout << i << " " << str << std::endl; 
		
		
	}
	

	
	return 0; 
}