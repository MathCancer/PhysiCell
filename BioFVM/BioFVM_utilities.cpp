/*
#############################################################################
# If you use BioFVM in your project, please cite BioFVM and the version     #
# number, such as below:                                                    #
#                                                                           #
# We solved the diffusion equations using BioFVM (Version 1.1.7) [1]        #
#                                                                           #
# [1] A. Ghaffarizadeh, S.H. Friedman, and P. Macklin, BioFVM: an efficient #
#    parallelized diffusive transport solver for 3-D biological simulations,#
#    Bioinformatics 32(8): 1256-8, 2016. DOI: 10.1093/bioinformatics/btv730 #
#                                                                           #
#############################################################################
#                                                                           #
# BSD 3-Clause License (see https://opensource.org/licenses/BSD-3-Clause)   #
#                                                                           #
# Copyright (c) 2015-2017, Paul Macklin and the BioFVM Project              #
# All rights reserved.                                                      #
#                                                                           #
# Redistribution and use in source and binary forms, with or without        #
# modification, are permitted provided that the following conditions are    #
# met:                                                                      #
#                                                                           #
# 1. Redistributions of source code must retain the above copyright notice, #
# this list of conditions and the following disclaimer.                     #
#                                                                           #
# 2. Redistributions in binary form must reproduce the above copyright      #
# notice, this list of conditions and the following disclaimer in the       #
# documentation and/or other materials provided with the distribution.      #
#                                                                           #
# 3. Neither the name of the copyright holder nor the names of its          #
# contributors may be used to endorse or promote products derived from this #
# software without specific prior written permission.                       #
#                                                                           #
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       #
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED #
# TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A           #
# PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER #
# OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,  #
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,       #
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR        #
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF    #
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING      #
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS        #
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.              #
#                                                                           #
#############################################################################
*/

#include "BioFVM.h"
#include "BioFVM_utilities.h"

namespace BioFVM{
/*
std::string BioFVM_Version; 
std::string BioFVM_URL; 
*/

std::chrono::steady_clock::time_point tic_time; 
std::chrono::steady_clock::time_point toc_time; 
std::chrono::steady_clock::time_point program_tic_time; 
std::chrono::steady_clock::time_point program_toc_time; 

double total_tictoc_time = 0.0; 

void TIC(void)
{ tic_time = std::chrono::steady_clock::now(); }

void TOC(void)
{
	toc_time = std::chrono::steady_clock::now();
	total_tictoc_time += stopwatch_value(); 
}

void RUNTIME_TIC(void)
{ program_tic_time = std::chrono::steady_clock::now(); }

void RUNTIME_TOC(void)
{ program_toc_time = std::chrono::steady_clock::now(); }

double stopwatch_value(void)
{
	static std::chrono::duration<double> time_span; 
	time_span = std::chrono::duration_cast<std::chrono::duration<double>>(toc_time-tic_time);
	return time_span.count(); 
}

double runtime_stopwatch_value(void)
{
	static std::chrono::duration<double> time_span; 
	time_span = std::chrono::duration_cast<std::chrono::duration<double>>(program_toc_time-program_tic_time);
	return time_span.count(); 
}

void display_stopwatch_value( std::ostream& os, double dIn )
{
	int nDays = (int) floor( (double) (dIn / (60.0*60.0*24.0)) );
	int nHours = (int) floor( (double) ( (dIn - nDays*60*60*24) / (60.0*60.0)) );
	int nMinutes = (int) floor( (double) ( (dIn - nDays*60*60*24 - nHours*60*60 ) / (60.0)) );
	double dSeconds = dIn - nDays*60.0*60.0*24.0 - nHours * 60.0*60.0 - nMinutes * 60.0;

	os << nDays << " days, " << nHours << " hours, " 
	  << nMinutes << " minutes, and " << dSeconds << " seconds ";
	return; 
}

std::string format_stopwatch_value( double dIn)
{
	std::string output; 
	output.resize( 1024 ); 
	int nDays = (int) floor( (double) (dIn / (60.0*60.0*24.0)) );
	int nHours = (int) floor( (double) ( (dIn - nDays*60*60*24) / (60.0*60.0)) );
	int nMinutes = (int) floor( (double) ( (dIn - nDays*60*60*24 - nHours*60*60 ) / (60.0)) );
	double dSeconds = dIn - nDays*60.0*60.0*24.0 - nHours * 60.0*60.0 - nMinutes * 60.0;

	sprintf( (char*) output.c_str(),
	"%d days, %d hours, %d minutes, and %2.4f seconds",
	nDays,nHours,nMinutes,dSeconds);

	return output; 
}

double total_stopwatch_time( void )
{ return total_tictoc_time; }

std::mt19937_64 biofvm_PRNG_generator; 
unsigned int biofvm_random_seed; 

void seed_random( unsigned int new_seed )
{
	biofvm_random_seed = new_seed; 
	biofvm_PRNG_generator.seed( biofvm_random_seed ); 	
}

void seed_random( void )
{ biofvm_PRNG_generator.seed( clock() ); }

double uniform_random( void )
{
	static std::uniform_real_distribution<double> distribution(0.0,1.0); 
	return distribution(biofvm_PRNG_generator); 
}

double compute_mean( std::vector<double>& values )
{
	static double sum; 
	sum = 0.0; 
	for( unsigned int i=0; i < values.size(); i++ )
	{ sum += values[i]; }
	sum /= (double) values.size(); 
	return sum; 	
}

double compute_variance( std::vector<double>& values, double mean )
{
	static double output; 
	output = 0.0; 
	for( unsigned int i=0; i < values.size() ; i++ )
	{
		static double temp; 
		temp = values[i]; 
		temp -= mean; 
		temp *= temp; 
		output += temp; 	
	}
	static int n; 
	n = values.size(); 
	n--; 
	output /= (double) n; 
	return output; 
}
double compute_variance( std::vector<double>& values )
{
	double mean = compute_mean( values ); 
	return compute_variance( values , mean ); 
}	

};