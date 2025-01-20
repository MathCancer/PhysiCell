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
# Copyright (c) 2015-2025, Paul Macklin and the BioFVM Project              #
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

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <iostream>

#include <ctime>
#include <string>
#include <cstring>
#include <cstdio>

#ifndef __BioFVM_matlab_h__
#define __BioFVM_matlab_h__

namespace BioFVM{

// #include "Matrix.h"

// Paul Macklin wrote these based on documentation on the web. 
// So far, matlab v4 is (partially) supported. Exceptions:
//   no complex matrices
//   no sparse matrices 
//   no text matrices

// To save in matlab and make it compatible, make sure you use: 
//   save -v4 <filename> <variable_name>

struct named_vector_data{
std::vector<std::string> names; 
std::vector< std::vector<double> > data; 
};

std::vector< std::vector<double> > read_matlab( std::string filename );
named_vector_data read_matlab_with_names( std::string filename );

bool write_matlab( std::vector< std::vector<double> >& input , std::string filename );
bool write_matlab( std::vector< std::vector<double> >& input , std::string filename , std::vector<std::string>& names );

FILE* write_matlab_header( unsigned int rows, unsigned int cols, std::string filename, std::string variable_name );  

// output: FILE pointer, and overwrites rows, cols so you know the size 
FILE* read_matlab_header( unsigned int* rows, unsigned int* cols , std::string filename ); 

};

#endif 
