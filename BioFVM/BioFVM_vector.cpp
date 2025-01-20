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

#include "BioFVM_vector.h" 

/* some global BioFVM strings */ 

namespace BioFVM{

/* faster operator overloading. multiplication and division are element-wise (Hadamard) */ 

std::vector<double> operator-( const std::vector<double>& v1 , const std::vector<double>& v2 )
{
 std::vector<double> v = v1;
 for( unsigned int i=0; i < v1.size() ; i++ )
 { v[i] -= v2[i]; }
 return v; 
}

std::vector<double> operator+( const std::vector<double>& v1 , const std::vector<double>& v2 )
{
 std::vector<double> v = v1;
 for( unsigned int i=0; i < v1.size() ; i++ )
 { v[i] += v2[i]; }
 return v; 
}

std::vector<double> operator*( const std::vector<double>& v1 , const std::vector<double>& v2 )
{
 std::vector<double> v = v1;
 for( unsigned int i=0; i < v1.size() ; i++ )
 { v[i] *= v2[i]; }
 return v; 
}

std::vector<double> operator/( const std::vector<double>& v1 , const std::vector<double>& v2 )
{
 std::vector<double> v = v1;
 for( unsigned int i=0; i < v1.size() ; i++ )
 { v[i] /= v2[i]; }
 return v; 
}

std::vector<double> operator*( double d , const std::vector<double>& v1 )
{
 std::vector<double> v = v1;
 for( unsigned int i=0; i < v1.size() ; i++ )
 { v[i] *= d; }
 return v; 
}

std::vector<double> operator+( double d , const std::vector<double>& v1 )
{
 std::vector<double> v = v1;
 for( unsigned int i=0; i < v1.size() ; i++ )
 { v[i] += d; }
 return v; 
}

std::vector<double> operator+( const std::vector<double>& v1 , double d )
{
 std::vector<double> v = v1;
 for( unsigned int i=0; i < v1.size() ; i++ )
 { v[i] += d; }
 return v; 
}

std::vector<double> operator-( double d , const std::vector<double>& v1 )
{
 std::vector<double> v = v1;
 for( unsigned int i=0; i < v1.size() ; i++ )
 { v[i] = d - v1[i]; }
 return v; 
}

std::vector<double> operator-( const std::vector<double>& v1 , double d  )
{
 std::vector<double> v = v1;
 for( unsigned int i=0; i < v1.size() ; i++ )
 { v[i] -= d; }
 return v; 
}

void operator+=( std::vector<double>& v1, const std::vector<double>& v2 )
{
 for( unsigned int i=0; i < v1.size() ; i++ )
 { v1[i] += v2[i]; }
 return; 
}

void operator-=( std::vector<double>& v1, const std::vector<double>& v2 )
{
 for( unsigned int i=0; i < v1.size() ; i++ )
 { v1[i] -= v2[i]; }
 return; 
}

void operator/=( std::vector<double>& v1, const std::vector<double>& v2 )
{
 for( unsigned int i=0; i < v1.size() ; i++ )
 { v1[i] /= v2[i]; }
 return;  
} 

void operator*=( std::vector<double>& v1, const double& a )
{
 for( unsigned int i=0; i < v1.size() ; i++ )
 { v1[i] *= a; }
 return; 
}

void operator*=( std::vector<double>& v1, const std::vector<double>& v2 )
{
 for( unsigned int i=0; i < v1.size() ; i++ )
 { v1[i] *= v2[i]; }
 return;  
}

void operator/=( std::vector<double>& v1, const double& a )
{
 for( unsigned int i=0; i < v1.size() ; i++ )
 { v1[i] /= a; }
 return;  
}

/* other commonly needed operations on vectors */ 

std::ostream& operator<<(std::ostream& os, const std::vector<double>& v )
{
 /*
 if( v.size() == 3 )
 {
  os << "x=\"" << v[0] << "\" y=\"" << v[1] << "\" z=\"" << v[2] << "\"" ; 
  return os; 
 }
 */

 for( unsigned int i=0; i < v.size(); i++ )
 { os << v[i] << " " ; }
 return os; 
}

std::ostream& operator<<(std::ostream& os, const std::vector<int>& v )
{
 for( unsigned int i=0; i < v.size(); i++ )
 { os << v[i] << " " ; }
 return os; 
}

std::ostream& operator<<(std::ostream& os, const std::vector<std::string>& v )
{
 for( unsigned int i=0; i < v.size(); i++ )
 { os << v[i] << " " ; }
 return os; 
}

// this one returns a new vector that has been normalized
std::vector<double> normalize( std::vector<double>& v )
{
 std::vector<double> output = v ;

 double norm = 0.0; 
 norm = 0.0; 

 for( unsigned int i=0; i < v.size(); i++ )
 { norm += ( v[i]*v[i] ); }
 norm = sqrt( norm ); 

 for( unsigned int i=0; i < v.size(); i++ )
 { output[i] /= norm ; }

 // If the norm is small, normalizing doens't make sense. 
 // Just set the entire vector to zero. 
 static bool I_warned_you = false; 
 if( norm <= 1e-16 )
 { 
  if( I_warned_you == false )
  {
   std::cout << "Warning and FYI: Very small vector are normalized to 0 vector" << std::endl << std::endl; 
   I_warned_you = true; 
  }

  for( unsigned int i=0; i < v.size(); i++ )
  { output[i] = 0.0; }
 }

 return output; 
}

// this one normalizes v
void normalize( std::vector<double>* v )
{
 double norm = 1e-32; 

 for( unsigned int i=0; i < v->size(); i++ )
 { norm += ( (*v)[i] * (*v)[i] ); }
 norm = sqrt( norm ); 

 for( unsigned int i=0; i < v->size(); i++ )
 { (*v)[i] /=  norm ; }

 // If the norm is small, normalizing doens't make sense. 
 // Just set the entire vector to zero. 
 static bool I_warned_you = false; 
 if( norm <= 1e-16 )
 { 
  if( I_warned_you == false )
  {
   std::cout << "Warning and FYI: Very small vectors are normalized to 0 vector" << std::endl << std::endl; 
   I_warned_you = true; 
  }

  for( unsigned int i=0; i < v->size(); i++ )
  { (*v)[i] = 0.0; }
 }

 return; 
}

double norm_squared( const std::vector<double>& v )
{
 double out = 0.0; 
 for( unsigned int i=0 ; i < v.size() ; i++ )
 { out += ( v[i] * v[i] ); }
 return out; 
}

double norm( const std::vector<double>& v )
{
 return sqrt( norm_squared( v ) ); 
}

double maxabs( const std::vector<double>& v )
{
 double out = 0.0; 
 for( unsigned int i=0; i < v.size() ; i++ )
 {
  if( fabs( v[i] ) > out )
  { out = v[i]; }
 }
 return out; 
}

double max_abs_difference( const std::vector<double>& v1 , const std::vector<double>& v2 )
{
 double out = 0.0; 
 for( unsigned int i=0; i < v1.size() ; i++ )
 {
  if( fabs( v1[i] -v2[i] ) > out )
  { out = fabs( v1[i] - v2[i] ); }
 }
 return out; 
}

std::vector<double> exponentiate( const std::vector<double>& exponent )
{
 std::vector<double> out( exponent.size() , 0.0 );
 
 for( unsigned int i=0 ; i < out.size() ; i++ )
 { out[i] = exp( exponent[i] ); }

 return out; 
}
 
void randomize( std::vector<double>* v )
{
 static double d1 = 2.0 / (double) RAND_MAX; 

 for( unsigned int i=0; i < v->size(); i++ )
 { (*v)[i] =  -1 + d1 * rand(); }
 
 return; 
}

/* axpy and related BLAS-type operations */ 

void axpy( std::vector<double>* y, const double& a , const std::vector<double>& x )
{
 for( unsigned int i=0; i < (*y).size() ; i++ )
 {
  (*y)[i] += a * x[i] ; 
 }
 return ; 
}

void axpy( std::vector<double>* y, const std::vector<double>& a , const std::vector<double>& x )
{
 for( unsigned int i=0; i < (*y).size() ; i++ )
 {
  (*y)[i] += a[i] * x[i] ; 
 }
 return; 
}

void naxpy( std::vector<double>* y, const double& a , const std::vector<double>& x )
{
 for( unsigned int i=0; i < (*y).size() ; i++ )
 {
  (*y)[i] -= a * x[i] ; 
 }
 return ; 
}

void naxpy( std::vector<double>* y, const std::vector<double>& a , const std::vector<double>& x )
{
 for( unsigned int i=0; i < (*y).size() ; i++ )
 {
  (*y)[i] -= a[i] * x[i] ; 
 }
 return; 
}

// turn a delimited character array (e.g., csv) into a vector of doubles

void csv_to_vector( const char* buffer , std::vector<double>& vect )
{
	vect.resize(0); 
	unsigned int i=0;
	while( i < strlen( buffer )  )
	{
		// churn through delimiters, whitespace, etc. to reach the next numeric term
		while( isdigit( buffer[i] ) == false && buffer[i] != '.' && buffer[i] != '-' && buffer[i] != 'e' && buffer[i] != 'E' )
		{ i++; } 
		char* pEnd; 
		if( i < strlen(buffer) ) // add this extra check in case of a final character, e.g., ']'
		{
			vect.push_back( strtod( buffer+i , &pEnd ) ); 
			i = pEnd - buffer; 
		}
	}			
	return; 
}

char* vector_to_csv( const std::vector<double>& vect )
{ 
	static int datum_size = 16;  // format = %.7e, 1 (sign) + 1 (lead) + 1 (decimal) + 7 (figs) + 2 (e, sign) + 3 (exponent) + 1 (delimiter) = 16
	// this is approximately the same at matlab long for single precision. 
	// If you want better precision, use a binary data format like matlab, or (in the future) HDF 

	char* buffer; 
	buffer = new char[ datum_size * vect.size() ];
	
	int position = 0; 
	for( unsigned int j=0; j < vect.size()-1 ; j++ )
	{
		position += sprintf( buffer+position , "%.7e," , vect[j] ); 
	}
	sprintf( buffer + position , "%.7e" , vect[ vect.size()-1 ] ); 
	
	return buffer; 
}

void vector_to_csv_safe( const std::vector<double>& vect , char*& buffer )
{ 
	static int datum_size = 16;  // format = %.7e, 1 (sign) + 1 (lead) + 1 (decimal) + 7 (figs) + 2 (e, sign) + 3 (exponent) + 1 (delimiter) = 16
	// this is approximately the same at matlab long for single precision. 
	// If you want better precision, use a binary data format like matlab, or (in the future) HDF 

	if( buffer )
	{ delete [] buffer; } 
	buffer = new char[ datum_size * vect.size() ];
	std::cout << __LINE__ << std::endl; 
	
	int position = 0; 
	for( unsigned int j=0; j < vect.size()-1 ; j++ )
	{
		position += sprintf( buffer+position , "%.7e," , vect[j] ); 
	}
	sprintf( buffer + position , "%.7e" , vect[ vect.size()-1 ] ); 
	return; 
}

void vector_to_csv( const std::vector<double>& vect , char*& buffer )
{ 
	// %.7e is approximately the same at matlab longe for single precision. 
	// If you want better precision, use a binary data format like matlab, or (in the future) HDF 

	int position = 0; 
	for( unsigned int j=0; j < vect.size()-1 ; j++ )
	{
		position += sprintf( buffer+position , "%.7e," , vect[j] ); 
	}
	sprintf( buffer + position , "%.7e" , vect[ vect.size()-1 ] ); 
	return; 
}

// turn a delimited character array (e.g., csv) into a vector of doubles

void list_to_vector( const char* buffer , std::vector<double>& vect , char delim ) //
{
	csv_to_vector( buffer , vect ); 
	return; 
}

char* vector_to_list( const std::vector<double>& vect , char delim )
{ 
	static int datum_size = 16;  // format = %.7e, 1 (sign) + 1 (lead) + 1 (decimal) + 7 (figs) + 2 (e, sign) + 3 (exponent) + 1 (delimiter) = 16
	// this is approximately the same at matlab long for single precision. 
	// If you want better precision, use a binary data format like matlab, or (in the future) HDF 

	char* buffer; 
	buffer = new char[ datum_size * vect.size() ];
	
	int position = 0; 
	for( unsigned int j=0; j < vect.size()-1 ; j++ )
	{
		position += sprintf( buffer+position , "%.7e%c" , vect[j] , delim ); 
	}
	sprintf( buffer + position , "%.7e" , vect[ vect.size()-1 ] ); 
	
	return buffer; 
}

void vector_to_list_safe( const std::vector<double>& vect , char*& buffer , char delim )
{ 
	static int datum_size = 16;  // format = %.7e, 1 (sign) + 1 (lead) + 1 (decimal) + 7 (figs) + 2 (e, sign) + 3 (exponent) + 1 (delimiter) = 16
	// this is approximately the same at matlab long for single precision. 
	// If you want better precision, use a binary data format like matlab, or (in the future) HDF 

	if( buffer )
	{ delete [] buffer; } 
	buffer = new char[ datum_size * vect.size() ];
	
	int position = 0; 
	for( unsigned int j=0; j < vect.size()-1 ; j++ )
	{
		position += sprintf( buffer+position , "%.7e%c" , vect[j] , delim ); 
	}
	sprintf( buffer + position , "%.7e" , vect[ vect.size()-1 ] ); 
	return; 
}

void vector_to_list( const std::vector<double>& vect , char*& buffer , char delim )
{ 
	// %.7e is approximately the same at matlab longe for single precision. 
	// If you want better precision, use a binary data format like matlab, or (in the future) HDF 

	int position = 0; 
	for( unsigned int j=0; j < vect.size()-1 ; j++ )
	{
		position += sprintf( buffer+position , "%.7e%c" , vect[j] , delim ); 
	}
	sprintf( buffer + position , "%.7e" , vect[ vect.size()-1 ] ); 
	return; 
}

// faster version if you know there are only 3 components
void vector3_to_list( const std::vector<double>& vect , char*& buffer , char delim )
{ 
	// %.7e is approximately the same at matlab longe for single precision. 
	// If you want better precision, use a binary data format like matlab, or (in the future) HDF 
	sprintf( buffer, "%.7e%c%.7e%c%.7e", vect[0] , delim, vect[1] , delim , vect[2] );
	return; 
}

double dot_product( const std::vector<double>& a , const std::vector<double>& b )
{
	double out = 0.0; 
	for( unsigned int i=0 ; i < a.size() ; i++ )
	{ out += ( a[i] * b[i] ); }
	return out; 
}

std::vector<double> cross_product( const std::vector<double>& a , const std::vector<double>& b )
{
	std::vector<double> out( 3, 0.0 ); 
	out[0] = a[1]*b[2] - a[2]*b[1]; 
	out[1] = a[2]*b[0] - a[0]*b[2];
	out[2] = a[0]*b[1] - a[1]*b[0];

	return out; 
} 

};
