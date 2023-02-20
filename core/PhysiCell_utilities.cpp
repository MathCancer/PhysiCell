/*
###############################################################################
# If you use PhysiCell in your project, please cite PhysiCell and the version #
# number, such as below:                                                      #
#                                                                             #
# We implemented and solved the model using PhysiCell (Version x.y.z) [1].    #
#                                                                             #
# [1] A Ghaffarizadeh, R Heiland, SH Friedman, SM Mumenthaler, and P Macklin, #
#     PhysiCell: an Open Source Physics-Based Cell Simulator for Multicellu-  #
#     lar Systems, PLoS Comput. Biol. 14(2): e1005991, 2018                   #
#     DOI: 10.1371/journal.pcbi.1005991                                       #
#                                                                             #
# See VERSION.txt or call get_PhysiCell_version() to get the current version  #
#     x.y.z. Call display_citations() to get detailed information on all cite-#
#     able software used in your PhysiCell application.                       #
#                                                                             #
# Because PhysiCell extensively uses BioFVM, we suggest you also cite BioFVM  #
#     as below:                                                               #
#                                                                             #
# We implemented and solved the model using PhysiCell (Version x.y.z) [1],    #
# with BioFVM [2] to solve the transport equations.                           #
#                                                                             #
# [1] A Ghaffarizadeh, R Heiland, SH Friedman, SM Mumenthaler, and P Macklin, #
#     PhysiCell: an Open Source Physics-Based Cell Simulator for Multicellu-  #
#     lar Systems, PLoS Comput. Biol. 14(2): e1005991, 2018                   #
#     DOI: 10.1371/journal.pcbi.1005991                                       #
#                                                                             #
# [2] A Ghaffarizadeh, SH Friedman, and P Macklin, BioFVM: an efficient para- #
#     llelized diffusive transport solver for 3-D biological simulations,     #
#     Bioinformatics 32(8): 1256-8, 2016. DOI: 10.1093/bioinformatics/btv730  #
#                                                                             #
###############################################################################
#                                                                             #
# BSD 3-Clause License (see https://opensource.org/licenses/BSD-3-Clause)     #
#                                                                             #
# Copyright (c) 2015-2022, Paul Macklin and the PhysiCell Project             #
# All rights reserved.                                                        #
#                                                                             #
# Redistribution and use in source and binary forms, with or without          #
# modification, are permitted provided that the following conditions are met: #
#                                                                             #
# 1. Redistributions of source code must retain the above copyright notice,   #
# this list of conditions and the following disclaimer.                       #
#                                                                             #
# 2. Redistributions in binary form must reproduce the above copyright        #
# notice, this list of conditions and the following disclaimer in the         #
# documentation and/or other materials provided with the distribution.        #
#                                                                             #
# 3. Neither the name of the copyright holder nor the names of its            #
# contributors may be used to endorse or promote products derived from this   #
# software without specific prior written permission.                         #
#                                                                             #
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" #
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE   #
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE  #
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE   #
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR         #
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF        #
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS    #
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN     #
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)     #
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE  #
# POSSIBILITY OF SUCH DAMAGE.                                                 #
#                                                                             #
###############################################################################
*/

#include "PhysiCell_utilities.h"
#include "PhysiCell_constants.h"

#include "PhysiCell.h" 

namespace PhysiCell{

thread_local std::mt19937_64 physicell_PRNG_generator; 
thread_local bool local_pnrg_setup_done = false; 

unsigned int physicell_random_seed = 0; 
std::vector<unsigned int> physicell_random_seeds; 

void SeedRandom( unsigned int input )
{ 
	physicell_random_seed = input;
	physicell_PRNG_generator.seed( physicell_random_seed );

	// now get number of threads and set up a seed for each thread 
	int num_threads = PhysiCell_settings.omp_num_threads; 
	physicell_random_seeds.resize( num_threads, 0 ); 

	// use std::seed_seq to create a sequence of seeds 
	// first, use the base seed 
	std::vector<unsigned int> initial_sequence( num_threads , 0 ); 
	// int* initial_sequence; 
	// initial_sequence = new int [num_threads]; 
	for( int i=0; i < num_threads ; i++ )
	{ initial_sequence[i] = physicell_random_seed+i; } 
	
	// now we use std::seed_seq 
	std::seed_seq seq(initial_sequence.begin() , initial_sequence.end() ); 

	// now we call the generator 
    std::vector<std::uint32_t> seeds(num_threads);
    seq.generate(seeds.begin(), seeds.end());

	// now transfer these into the seeds for each thread 
	physicell_random_seeds[0] = physicell_random_seed; 
	for( int i=1; i < num_threads ; i++ )
	{ physicell_random_seeds[i] = seeds[i]; }

	return; 
}

void SeedRandom( void )
{ 
	physicell_random_seed = std::chrono::system_clock::now().time_since_epoch().count();
	physicell_PRNG_generator.seed( physicell_random_seed );

	// now get number of threads and set up a seed for each thread 
	int num_threads = PhysiCell_settings.omp_num_threads; 
	physicell_random_seeds.resize( num_threads, 0 ); 

	// use std::seed_seq to create a sequence of seeds 
	// first, use the base seed 
	std::vector<unsigned int> initial_sequence( num_threads , 0 ); 
	// int* initial_sequence; 
	// initial_sequence = new int [num_threads]; 
	for( int i=0; i < num_threads ; i++ )
	{ initial_sequence[i] = physicell_random_seed+i; } 
	
	// now we use std::seed_seq 
	std::seed_seq seq(initial_sequence.begin() , initial_sequence.end() ); 

	// now we call the generator 
    std::vector<std::uint32_t> seeds(num_threads);
    seq.generate(seeds.begin(), seeds.end());

	// now transfer these into the seeds for each thread 
	physicell_random_seeds[0] = physicell_random_seed; 
	for( int i=1; i < num_threads ; i++ )
	{ physicell_random_seeds[i] = seeds[i]; }

	return; 
}

double UniformRandom_old_not_thread_safe()
{
	return std::generate_canonical<double, 10>(physicell_PRNG_generator);
}

double UniformRandom( void )
{
	thread_local std::uniform_real_distribution<double> distribution(0.0,1.0);
	if( local_pnrg_setup_done == false )
	{
		// get my thread number 
		int i = omp_get_thread_num(); 
    	physicell_PRNG_generator.seed( physicell_random_seeds[i] ); 
		local_pnrg_setup_done = true; 
/*
		#pragma omp critical 
		{
		std::cout << "thread: " << i 
		<< " seed: " << physicell_random_seeds[i]  << std::endl; 
		std::cout << "\t first call: " << distribution(physicell_PRNG_generator) << std::endl; 
		}
*/
	}
    return distribution(physicell_PRNG_generator);

	// helpful info: https://stackoverflow.com/a/29710970
/*

	static std::uniform_real_distribution<double> distribution(0.0,1.0); 
	double out;
	out = distribution(physicell_PRNG_generator);
	return out; 
*/	
}


int UniformInt()
{
	static std::uniform_int_distribution<int> int_dis;
	return int_dis(physicell_PRNG_generator);
}


double NormalRandom( double mean, double standard_deviation )
{
	std::normal_distribution<double> d(mean,standard_deviation);
	return d(physicell_PRNG_generator); 
}

double LogNormalRandom( double mean, double standard_deviation )
{
	return exp(NormalRandom(log(mean), standard_deviation));
}

std::vector<double> UniformOnUnitSphere( void )
{
	std::vector<double> output = {0,0,0}; 

	double z = UniformRandom();
	z *= 2.0; 
	z -= 1.0; // Choose z uniformly distributed in [-1,1].

	static double two_pi = 6.283185307179586476925286766559; 
	double theta = UniformRandom();
	theta *= two_pi; // Choose theta uniformly distributed on [0, 2*pi).

	double r = z; 
	r *= z; 
	r *= -1;
	r += 1; 
	r = sqrt(r); // Let r = sqrt(1-z^2).

	output[0] = cos(theta); 
	output[1] = sin(theta);
	output *= r; 
	output[2] = z; // (r*cos(theta) , r*sin(theta) , z )

	return output; 
}

std::vector<double> UniformOnUnitCircle( void )
{
	std::vector<double> output = {0,0,0}; 

	static long double two_pi = 6.283185307179586476925286766559;  
	                       
	long double theta = UniformRandom(); //  BioFVM::uniform_random();
	theta *= two_pi; // Choose theta uniformly distributed on [0, 2*pi).

	output[0] = cos(theta); 
	output[1] = sin(theta); // (cos(t) , sin(t) , 0 )

	return output; 
}

std::vector<double> LegacyRandomOnUnitSphere( void )
{
	static bool warned = false; 
	if( warned == false )
	{
		std::cout << "Warning! LegacyRandomOnUnitSphere() has bad random properties. " << std::endl 
				  << "         It generates points that aren't uniform on the unit sphere," << std::endl 
				  << "         but instead are concentrated towards the poles." << std::endl 
				  << "         Use UniformOnUnitSphere() instead!" << std::endl << std::endl; 
		warned = true; 
	}
	
	std::vector<double> output = {0,0,0}; 

	static double pi = 3.1415926535897932384626433832795; 
	static double two_pi = 6.283185307179586476925286766559; 
	double theta = UniformRandom();
	
	
	double temp_angle = two_pi*UniformRandom();
	double temp_phi = pi*UniformRandom();
	
	
	output[0]= cos( temp_angle );
	output[1]= sin( temp_angle );
	output *= sin( temp_phi );
	output[2]= cos( temp_phi );
	
	return output; 
}


// Squared distance between two points
// This is already in BioFVM_vector as: 
// double norm_squared( const std::vector<double>& v ); 
// The following function will be deprecated. 
double dist_squared(std::vector<double> p1, std::vector<double> p2)
{
	return (p1[0]-p2[0])*(p1[0]-p2[0]) + (p1[1]-p2[1])*(p1[1]-p2[1]) + (p1[2]-p2[2])*(p1[2]-p2[2]);
}

// Distance between two points
// This is already in BioFVM_vector as: 
// double norm( const std::vector<double>& v ); 
// The following function will be deprecated. 
double dist(std::vector<double> p1, std::vector<double> p2)
{
	return sqrt(dist_squared(p1, p2));
}

std::string get_PhysiCell_version( void )
{
//	extern std::string PhysiCell_version; 
	return PhysiCell_Version; 
}	
void get_PhysiCell_version( std::string& pString )
{
//	extern std::string PhysiCell_version; 
	pString.assign( PhysiCell_Version ); 
}

std::vector<std::string> software_versions; 
std::vector<std::string> software_names; 
std::vector<std::string> software_DOIs; 
std::vector<std::string> software_URLs; 

void display_citations( std::ostream& os )
{
	static bool PhysiCell_citation_added = false; 
	if( PhysiCell_citation_added == false )
	{
		add_software_citation( "PhysiCell" , get_PhysiCell_version() , 
			PhysiCell_DOI , PhysiCell_URL ); 
		PhysiCell_citation_added = true; 
	}
	
	std::ofstream of( "ALL_CITATIONS.txt" , std::ios::out );
	for( int i=0; i < software_versions.size() ; i++ )
	{
		os << "Using " << software_names[i] 
		<< " version " << software_versions[i] 
		<< std::endl << "\tPlease cite DOI: " << software_DOIs[i] 
		<< std::endl << "\tProject website: " << software_URLs[i] 
		<< std::endl; 
		
		of << "Using " << software_names[i] 
		<< " version " << software_versions[i] 
		<< std::endl << "\tPlease cite DOI: " << software_DOIs[i] 
		<< std::endl << "\tProject website: " << software_URLs[i] 
		<< std::endl; 
	}
	os << std::endl << "See ALL_CITATIONS.txt for this list." << std::endl; 
	
	of << std::endl; 
	of.close(); 
	
	return; 
}

void display_citations( void )
{
	return display_citations( std::cout ); 
}

void add_software_citation( std::string name , std::string version, std::string DOI , std::string URL )
{
	software_names.push_back( name ); 
	software_versions.push_back( version ); 
	software_DOIs.push_back( DOI ); 
	software_URLs.push_back( URL ); 
	return; 
}

int choose_event( std::vector<double>& probabilities )
{
	double rand_number = UniformRandom(); 
	
	for( int i=0 ; i < probabilities.size() ; i++ )
	{
		if( rand_number <= probabilities[i] )
		{ return i; }
		else
		{
			rand_number -= probabilities[i]; 
		}
	}
	
	return probabilities.size(); 
}

};
