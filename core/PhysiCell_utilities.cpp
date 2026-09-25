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
# Copyright (c) 2015-2025, Paul Macklin and the PhysiCell Project             #
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

// for the process id, which SeedRandom() mixes into the clock-based seed
#ifdef _WIN32
#include <process.h>
#define PhysiCell_getpid _getpid
#else
#include <unistd.h>
#define PhysiCell_getpid getpid
#endif

namespace PhysiCell{

thread_local std::mt19937_64 physicell_PRNG_generator; 

// The finalizing mix of Sebastiano Vigna's splitmix64 (public domain). Every step is a bijection 
// on 64 bits -- an xor-shift by more than half the width, then a multiply by an odd constant -- so 
// distinct inputs can never collide, and flipping one input bit flips about half the output bits. 
static std::uint64_t mix64( std::uint64_t z )
{
	z = ( z ^ ( z >> 30 ) ) * 0xBF58476D1CE4E5B9ull; 
	z = ( z ^ ( z >> 27 ) ) * 0x94D049BB133111EBull; 
	return z ^ ( z >> 31 ); 
}

// The RNG's seed state. reseed() is the only way to change it, and it installs the base seed and 
// every thread's generator in one step, so the seed recorded in random_seed.txt and the streams 
// the threads actually draw from cannot disagree. 
class RNG_State
{
 public:
	void reseed( std::uint64_t new_base_seed, int num_threads )
	{
		base_seed = new_base_seed; 
		if( num_threads < 1 )
		{ num_threads = 1; } // there is always a thread 0 

		// physicell_PRNG_generator is thread_local, so a thread can only be seeded by itself. The 
		// encountering thread is this team's thread 0, so this seeds the caller too. 
		#pragma omp parallel num_threads( num_threads )
		{ physicell_PRNG_generator.seed( seed_for_thread( omp_get_thread_num() ) ); } 
	}

	std::uint64_t seed( void ) const 
	{ return base_seed; } 

	// The seed for one thread. 0x9E3779B97F4A7C15 is the odd 64-bit integer nearest to 2^64/phi, 
	// the constant splitmix64 advances its state by, so this is exactly splitmix64's 
	// (thread_index+1)-th output from the base seed, written in closed form so it costs the same 
	// for every thread. It depends only on the base seed and the index -- never on how many 
	// threads are running -- and it fills the full 64-bit width of std::mt19937_64. 
	std::uint64_t seed_for_thread( int thread_index ) const
	{ 
		return mix64( base_seed 
			+ ( (std::uint64_t) thread_index + 1 ) * 0x9E3779B97F4A7C15ull ); 
	}

 private:
	std::uint64_t base_seed = 0; 
};

static RNG_State physicell_rng; 

std::uint64_t get_random_seed( void )
{ return physicell_rng.seed(); }

std::uint64_t get_thread_random_seed( int thread_index )
{ return physicell_rng.seed_for_thread( thread_index ); }

static void setup_rng( std::uint64_t seed )
{
	static bool setup_done = false;
	static bool warned = false;
	if (!warned && setup_done)
	{
		std::cout << "WARNING: Setting the random seed again." << std::endl
				  << "\tYou probably have set a user parameter called random_seed." << std::endl
				  << "\tHere, we will use the random seed set in user parameters." << std::endl	
				  << "\tHOWEVER, as of PhysiCell 1.14.0, you should set the random seed in the <options><random_seed> element in the config file." << std::endl
				  << "\tFuture versions of PhysiCell may throw an error here. Kindly remove the user parameter and just use the <options><random_seed> element." << std::endl;
		warned = true;
	}
	physicell_rng.reseed( seed, PhysiCell_settings.omp_num_threads ); 

	std::cout << "Setting up RNG with seed " << physicell_rng.seed() << std::endl;

	// save the seed to random_seed.txt
	std::string seed_filename = PhysiCell_settings.folder + "/random_seed.txt";
	std::ofstream out(seed_filename);
	if (!out)
	{
		std::cout << "WARNING: Could not write " << seed_filename << "." << std::endl
 				  << "\tCheck that the output folder exists and is writable. The random seed will not be recorded." << std::endl;
	}
	else
	{
		out << physicell_rng.seed() << std::endl;
		out.close();
	}

	setup_done = true;
	return; 
}

void SeedRandom( std::uint64_t input )
{ 
	return setup_rng( input );
}

void SeedRandom( void )
{ 
	// The system clock on its own is not enough entropy: it advances in ~1 microsecond steps on 
	// macOS, so processes launched together (parallel replicates, for example) routinely read the 
	// same count and end up sharing a seed. Absorb three independent sources instead: 
	//   - std::random_device, the OS entropy pool, which is the main source; 
	//   - the system clock, which separates runs even where random_device is a deterministic stub 
	//     (as it was on older MinGW toolchains); 
	//   - the process id, which separates processes launched within the same clock tick. 
	// Each source is xor-ed in and then run through mix64. Because mix64 is a bijection, two runs 
	// that differ in any one source still differ in the seed; folding after each source also stops 
	// a small process id from cancelling against the clock's low bits, which a flat xor of all 
	// three would allow. 
	std::uint64_t seed = 0; 
	try
	{ 
		std::random_device rd; 
		seed = ( ( (std::uint64_t) rd() ) << 32 ) | (std::uint64_t) rd(); 
	}
	catch( const std::exception& e )
	{ /* no OS entropy source; the clock and process id below still separate runs */ } 

	seed = mix64( seed ^ (std::uint64_t) std::chrono::system_clock::now().time_since_epoch().count() ); 
	seed = mix64( seed ^ (std::uint64_t) PhysiCell_getpid() ); 

	return setup_rng( seed );
}

double UniformRandom_old_not_thread_safe()
{
	return std::generate_canonical<double, 10>(physicell_PRNG_generator);
}

double UniformRandom( void )
{
	// helpful info: https://stackoverflow.com/a/29710970
	// uniform_real_distribution holds only its parameters and its reset() is a no-op, so a plain 
	// local costs nothing to build and avoids a thread_local access on the hottest RNG path 
	std::uniform_real_distribution<double> distribution(0.0,1.0);
    return distribution(physicell_PRNG_generator);
}


int UniformInt()
{
	// likewise stateless; it was a shared static, which every thread mutated through operator() 
	std::uniform_int_distribution<int> int_dis;
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

void copy_file_to_output(const std::string &filename, const std::string &default_basename)
{
	std::cout << "Copying " << filename << " to output folder." << std::endl;
	// copy the file to the output folder
	std::string basename = filename;
	size_t found = basename.find_last_of("/\\");
	if (found != std::string::npos)
	{
		basename = basename.substr(found + 1);
	}

	std::string output_filename = PhysiCell_settings.folder + "/" + basename;

	// copy filename to output_filename
	char copy_command[1024];
	sprintf(copy_command, "cp %s %s", filename.c_str(), output_filename.c_str());
	(void)system(copy_command); // make it explicit that we are ignoring the return value

	if (default_basename.empty() || default_basename == basename) {
		return;
	}

	// copy the file to the output folder with the default basename
	std::string default_output_filename = PhysiCell_settings.folder + "/" + default_basename;
	sprintf(copy_command, "cp %s %s", filename.c_str(), default_output_filename.c_str());
	(void)system(copy_command); // make it explicit that we are ignoring the return value
	return;
}

};
