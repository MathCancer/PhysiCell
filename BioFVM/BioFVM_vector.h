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

#ifndef __BioFVM_vector_h__
#define __BioFVM_vector_h__

#include <iostream>
#include <cstdlib>
#include <vector> 
#include <cmath>
#include <cstring>

namespace BioFVM{

/* faster operator overloading. multiplication and division are element-wise (Hadamard) */ 

std::vector<double> operator-( const std::vector<double>& v1 , const std::vector<double>& v2 );
std::vector<double> operator+( const std::vector<double>& v1 , const std::vector<double>& v2 );
std::vector<double> operator*( const std::vector<double>& v1 , const std::vector<double>& v2 );
std::vector<double> operator/( const std::vector<double>& v1 , const std::vector<double>& v2 );

std::vector<double> operator*( double d , const std::vector<double>& v1 );
std::vector<double> operator+( double d , const std::vector<double>& v1 ); 
std::vector<double> operator+( const std::vector<double>& v1 , double d );
std::vector<double> operator-( double d , const std::vector<double>& v1 );
std::vector<double> operator-( const std::vector<double>& v1 , double d  ); 

void operator+=( std::vector<double>& v1, const std::vector<double>& v2 ); 
void operator-=( std::vector<double>& v1, const std::vector<double>& v2 ); 
void operator/=( std::vector<double>& v1, const std::vector<double>& v2 ); 
void operator*=( std::vector<double>& v1, const double& a );
void operator*=( std::vector<double>& v1, const std::vector<double>& v2 ); 
void operator/=( std::vector<double>& v1, const double& a );

/* other commonly needed operations on vectors */ 

std::ostream& operator<<(std::ostream& os, const std::vector<double>& v ); 
std::ostream& operator<<(std::ostream& os, const std::vector<int>& v );
std::ostream& operator<<(std::ostream& os, const std::vector<std::string>& v );

// this one returns a new vector that has been normalized
std::vector<double> normalize( std::vector<double>& v );

// this one normalizes v
void normalize( std::vector<double>* v ); 

double norm_squared( const std::vector<double>& v ); 
double norm( const std::vector<double>& v ); 

double maxabs( const std::vector<double>& v ); 
double max_abs_difference( const std::vector<double>& v1 , const std::vector<double>& v2 ); 

std::vector<double> exponentiate( const std::vector<double>& exponent ); 

// note that the PRNG must be replaced if you are serious about this function
void randomize( std::vector<double>* v ); 

/* axpy and related BLAS-type operations */ 

// y = y + a*x 
void axpy( std::vector<double>* y, const double& a , const std::vector<double>& x );
// y = y + a.*x
void axpy( std::vector<double>* y, const std::vector<double>& a , const std::vector<double>& x ); 

// y = y - a*x 
void naxpy( std::vector<double>* y, const double& a , const std::vector<double>& x );
// y = y - a.*x
void naxpy( std::vector<double>* y, const std::vector<double>& a , const std::vector<double>& x ); 

/* I may cut these from the final version */ 
/* CLEANUP BEFORE RELEASE */ 

//y = y + a.*x  ; y = y ./ d
void axpy_div( std::vector<double>* y, std::vector<double>& a , std::vector<double>& x , std::vector<double>& d );

// y = y + a1.*x1 + a2.*x2  
void double_axpy( std::vector<double>* y, std::vector<double>& a1 , std::vector<double>& a2, std::vector<double>& x1 , std::vector<double>& x2 ); 
// y = y + a.*(x1 + x2)  
void double_axpy( std::vector<double>* y, std::vector<double>& a , std::vector<double>& x1 , std::vector<double>& x2 ); 
// y = y + a.*(x1 + x2)  ; y = y./d
void double_axpy_div( std::vector<double>* y, std::vector<double>& a , std::vector<double>& x1 , std::vector<double>& x2 , std::vector<double>& d); 
// y = y + a1.*x1 + a2.*x2  ; y = y./d
void double_axpy_div( std::vector<double>* y, std::vector<double>& a1 , std::vector<double>& a2, std::vector<double>& x1 , std::vector<double>& x2 , std::vector<double>& d);

// turn a delimited character array (e.g., csv) into a vector of doubles

void csv_to_vector( const char* buffer , std::vector<double>& vect ); 
char* vector_to_csv( const std::vector<double>& vect );
void vector_to_csv_safe( const std::vector<double>& vect , char*& buffer );
void vector_to_csv( const std::vector<double>& vect , char*& buffer );

void list_to_vector( const char* buffer , std::vector<double>& vect , char delim ); // can be same as csv 
char* vector_to_list( const std::vector<double>& vect , char delim );
void vector_to_list_safe( const std::vector<double>& vect , char*& buffer , char delim );
void vector_to_list( const std::vector<double>& vect , char*& buffer , char delim );

void vector3_to_list( const std::vector<double>& vect , char*& buffer , char delim ); 

double dot_product( const std::vector<double>& a , const std::vector<double>& b );
std::vector<double> cross_product( const std::vector<double>& a , const std::vector<double>& b );
    
};

#endif
