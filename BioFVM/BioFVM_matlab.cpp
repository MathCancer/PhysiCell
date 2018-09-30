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

#include "BioFVM_matlab.h"
#include <ctime>
#include <string>
#include <cstring>
#include <cstdio>

namespace BioFVM{

unsigned int thousands( unsigned int& input )
{ return input / 1000;}

unsigned int hundreds( unsigned int& input )
{ return ( input % 1000 ) / 100; }

unsigned int tens( unsigned int& input )
{ return ( input % 100 ) / 10; }

unsigned int ones( unsigned int& input )
{ return (input % 10); }

// vector< vector<double> > read_matlab4( string filename )

std::vector< std::vector<double> > read_matlab( std::string filename )
{
 // Matrix output;
 std::vector< std::vector<double> > output; 
 
 FILE* fp; 
 fp = fopen( filename.c_str() , "rb" );
 if( fp == NULL )
 {
  std::cout << "Error: could not open file " << filename << "!" << std::endl;
  return output;
 }
 
 typedef unsigned int UINT;
 UINT UINTs = sizeof(UINT);
 
 // read the basic header information 
 
 UINT temp;
 size_t result;
 result = fread( (char*) &temp , UINTs , 1 , fp );
 
 UINT type_numeric_format = thousands(temp);
 UINT type_reserved = hundreds(temp);
 UINT type_data_format = tens(temp);
 UINT type_matrix_type = ones(temp);
/*
	cout << type_numeric_format << " " 
		<< type_reserved << " " 
		<< type_data_format << " " 
		<< type_matrix_type << endl;
*/
	
 // make sure it's a matlab L4 file 
 	  
 if( type_numeric_format != 0 || // 	little-endian
     type_reserved != 0 || // should always be 0
     type_data_format > 5 || // unknown format
     type_matrix_type != 0 ) // want full matrices, not sparse 
 {
  std::cout << "Error reading file " << filename << ": I can't read this format yet!" << std::endl;
  return output;
 } 

 // get the size of the data 
 
 UINT rows;
 result = fread( (char*) &rows , UINTs , 1, fp );
 
 UINT cols;
 result = fread( (char*) &cols, UINTs , 1 , fp );
 
 // resize the output accordingly 

 // output.SetSize( rows, cols );
 


 std::vector<double> TemplateRow(cols,0.0);
 output.resize( rows , TemplateRow );

 // make sure we're not dealing with complex numbers 
 
 UINT imag;
 result = fread( (char*) &imag, UINTs, 1 , fp );
 if( imag != 0 )
 {
  std::cout << "Error: I can't read imaginary matrices yet!" << std::endl;
  return output;
 }
 
 // Get the name of the variable. We don't tend to use this on reading (for now). 
 // But if we were to output a more complex data structure with 
 // vector< vector<double> > and vector<string>, we could! 

 // if we actually use the names, then I'd suggest that we do a little parsing: 
 // is it a MultiCellDS field array? 
 // Make a format for that. Something like this:
 // MultiCellDS_Fields:name1,name2,...,nameN, where N = rows - 3; 

 UINT name_length;
 result = fread( (char*) &name_length, UINTs, 1 , fp );
 char* name;
 name = new char [name_length];

 // this is the end of the 20-byte header 

 // read the name
 
 result = fread( name , name_length , 1 , fp );

 // read the real part of the matrix
 unsigned int i = 0;
 unsigned int j = 0;
 
 switch( type_data_format )
 {
  case 0:
   // all fields are doubles
   
   for( unsigned int n=0; n < rows*cols ; n++ )
   {
	double temp;
	result = fread( (char*) &temp, sizeof(double), 1 , fp );
	(output[i])[j] = temp; 
    i++;
	
	if( i == rows )
    { i=0; j++; }
   }
   break;
   
  case 1:
   // all fields are floats 
   
   for( unsigned int n=0; n < rows*cols ; n++ )
   {
	float temp;
	result = fread( (char*) &temp, sizeof(float), 1 , fp );
	(output[i])[j] = (double) temp; 
    i++;
	
	if( i == rows )
    { i=0; j++; }
   }
   break;
   
  case 2:
   // all fields are signed ints of size 4 bytes  
   
   for( unsigned int n=0; n < rows*cols ; n++ )
   {
	int temp;
	result = fread( (char*) &temp, sizeof(int), 1 , fp );
	(output[i])[j] = (double) temp; 
    i++;
	
	if( i == rows )
    { i=0; j++; }
   }  
   break;
   
  case 3:
   // all fields are signed ints of size 2 bytes  
   
   for( unsigned int n=0; n < rows*cols ; n++ )
   {
	short temp;
	result = fread( (char*) &temp, sizeof(short), 1 , fp );
	(output[i])[j] = (double) temp; 
    i++;
	
	if( i == rows )
    { i=0; j++; }
   }  
   break;
   
  case 4:
   // all fields are unsigned ints of size 2 bytes  
   
   for( unsigned int n=0; n < rows*cols ; n++ )
   {
	unsigned short temp;
	result = fread( (char*) &temp, sizeof(unsigned short), 1 , fp );
	(output[i])[j] = (double) temp; 
    i++;
	
	if( i == rows )
    { i=0; j++; }
   }  
   break;  
  
  case 5:
   // all fields are unsigned ints of size 1 bytes  
   
   for( unsigned int n=0; n < rows*cols ; n++ )
   {
	unsigned char temp;
	result = fread( (char*) &temp, sizeof(unsigned char), 1 , fp );
	(output[i])[j] = (double) temp; 
    i++;
	
	if( i == rows )
    { i=0; j++; }
   }  
   break; 
   
  default:
   std::cout << "Error: Unknown format!" << std::endl;
   break;
 }
 
 // read the imaginary part of the matrix (not supported!)
 
 fclose( fp );
 
 delete name;
 return output;
}

named_vector_data read_matlab_with_names( std::string filename )
{
 named_vector_data output; 
 
 FILE* fp; 
 fp = fopen( filename.c_str() , "rb" );
 if( fp == NULL )
 {
  std::cout << "Error: could not open file " << filename << "!" << std::endl;
  return output;
 }
 
 typedef unsigned int UINT;
 UINT UINTs = sizeof(UINT);
 
 // read the basic header information 
 
 UINT temp;
 size_t result;
 result = fread( (char*) &temp , UINTs , 1 , fp );
 
 UINT type_numeric_format = thousands(temp);
 UINT type_reserved = hundreds(temp);
 UINT type_data_format = tens(temp);
 UINT type_matrix_type = ones(temp);
	
 // make sure it's a matlab L4 file 
 	  
 if( type_numeric_format != 0 || // 	little-endian
     type_reserved != 0 || // should always be 0
     type_data_format > 5 || // unknown format
     type_matrix_type != 0 ) // want full matrices, not sparse 
 {
  std::cout << "Error reading file " << filename << ": I can't read this format yet!" << std::endl;
  return output;
 } 

 // get the size of the data 
 
 UINT rows;
 result = fread( (char*) &rows , UINTs , 1, fp );
 
 UINT cols;
 result = fread( (char*) &cols, UINTs , 1 , fp );
 
 // resize the output accordingly 

 std::vector<double> TemplateRow(cols,0.0);

 output.data.resize( rows , TemplateRow );

 // make sure we're not dealing with complex numbers 
 
 UINT imag;
 result = fread( (char*) &imag, UINTs, 1 , fp );
 if( imag != 0 )
 {
  std::cout << "Error: I can't read imaginary matrices yet!" << std::endl;
  return output;
 }
 
 // Get the name of the variable. We don't tend to use this on reading (for now). 
 // But if we were to output a more complex data structure with 
 // vector< vector<double> > and vector<string>, we could! 

 // if we actually use the names, then I'd suggest that we do a little parsing: 
 // is it a MultiCellDS field array? 
 // Make a format for that. Something like this:
 // MultiCellDS_Fields:name1,name2,...,nameN, where N = rows - 3; 

 UINT name_length;
 result = fread( (char*) &name_length, UINTs, 1 , fp );
 char* name;
 name = new char [name_length];

 // this is the end of the 20-byte header 

 // read the name
 
 result = fread( name , name_length , 1 , fp );
 output.names[0] = name; 

 // read the real part of the matrix
 unsigned int i = 0;
 unsigned int j = 0;
 
 switch( type_data_format )
 {
  case 0:
   // all fields are doubles
   
   for( unsigned int n=0; n < rows*cols ; n++ )
   {
	double temp;
	result = fread( (char*) &temp, sizeof(double), 1 , fp );
	(output.data[i])[j] = temp; 
    i++;
	
	if( i == rows )
    { i=0; j++; }
   }
   break;
   
  case 1:
   // all fields are floats 
   
   for( unsigned int n=0; n < rows*cols ; n++ )
   {
	float temp;
	result = fread( (char*) &temp, sizeof(float), 1 , fp );
	(output.data[i])[j] = (double) temp; 
    i++;
	
	if( i == rows )
    { i=0; j++; }
   }
   break;
   
  case 2:
   // all fields are signed ints of size 4 bytes  
   
   for( unsigned int n=0; n < rows*cols ; n++ )
   {
	int temp;
	result = fread( (char*) &temp, sizeof(int), 1 , fp );
	(output.data[i])[j] = (double) temp; 
    i++;
	
	if( i == rows )
    { i=0; j++; }
   }  
   break;
   
  case 3:
   // all fields are signed ints of size 2 bytes  
   
   for( unsigned int n=0; n < rows*cols ; n++ )
   {
	short temp;
	result = fread( (char*) &temp, sizeof(short), 1 , fp );
	(output.data[i])[j] = (double) temp; 
    i++;
	
	if( i == rows )
    { i=0; j++; }
   }  
   break;
   
  case 4:
   // all fields are unsigned ints of size 2 bytes  
   
   for( unsigned int n=0; n < rows*cols ; n++ )
   {
	unsigned short temp;
	result = fread( (char*) &temp, sizeof(unsigned short), 1 , fp );
	(output.data[i])[j] = (double) temp; 
    i++;
	
	if( i == rows )
    { i=0; j++; }
   }  
   break;  
  
  case 5:
   // all fields are unsigned ints of size 1 bytes  
   
   for( unsigned int n=0; n < rows*cols ; n++ )
   {
	unsigned char temp;
	result = fread( (char*) &temp, sizeof(unsigned char), 1 , fp );
	(output.data[i])[j] = (double) temp; 
    i++;
	
	if( i == rows )
    { i=0; j++; }
   }  
   break; 
   
  default:
   std::cout << "Error: Unknown format!" << std::endl;
   break;
 }
 
 
 // read the imaginary part of the matrix (not supported!)
 
 fclose( fp );
 
 delete name;
 return output;
}

FILE* read_matlab_header( unsigned int* rows, unsigned int* cols , std::string filename )
{
 FILE* fp; 
 fp = fopen( filename.c_str() , "rb" );
 if( fp == NULL )
 {
  std::cout << "Error: could not open file " << filename << "!" << std::endl;
  return NULL;
 }
 
 typedef unsigned int UINT;
 UINT UINTs = sizeof(UINT);
 
 // read the basic header information 
 
 UINT temp;
 
 size_t result;
 result = fread( (char*) &temp , UINTs , 1 , fp );
 
 UINT type_numeric_format = thousands(temp);
 UINT type_reserved = hundreds(temp);
 UINT type_data_format = tens(temp);
 UINT type_matrix_type = ones(temp);
	
 // make sure it's a matlab L4 file 
 	  
 if( type_numeric_format != 0 || // 	little-endian
     type_reserved != 0 || // should always be 0
     type_data_format > 5 || // unknown format
     type_matrix_type != 0 ) // want full matrices, not sparse 
 {
  std::cout << "Error reading file " << filename << ": I can't read this format yet!" << std::endl;
  fclose( fp );
  return NULL;
 } 

 // get the size of the data 
 
// UINT rows;
 result = fread( (char*) rows , UINTs , 1, fp );
 
// UINT cols;
result =  fread( (char*) cols, UINTs , 1 , fp );
 
 // resize the output accordingly 

 // make sure we're not dealing with complex numbers 
 
 UINT imag;
 result = fread( (char*) &imag, UINTs, 1 , fp );
 if( imag != 0 )
 {
  std::cout << "Error: I can't read imaginary matrices yet!" << std::endl;
  fclose( fp );
  return NULL;
 }

 UINT name_length;
 result = fread( (char*) &name_length, UINTs, 1 , fp );
 char* name;
 name = new char [name_length];

 // this is the end of the 20-byte header 

 // read the name
 
 result = fread( name , name_length , 1 , fp );
 delete name; 
  
 return fp; 
}

FILE* write_matlab4_header( int nrows, int ncols, std::string filename, std::string variable_name )
{
 FILE* fp; 
 fp = fopen( filename.c_str() , "wb" );
 if( fp == NULL )
 {
  std::cout << "Error: could not open file " << filename << "!" << std::endl;
  return NULL;
 }
 
 typedef unsigned int UINT;
 UINT UINTs = sizeof(UINT);
 
 UINT temp;
 
 UINT type_numeric_format = 0; // little-endian assumed for now!
 UINT type_reserved = 0;
 UINT type_data_format = 0; // doubles for all entries 
 UINT type_matrix_type = 0; // full matrix, not sparse

 temp = 1000*type_numeric_format + 100*type_reserved + 10*type_data_format + type_matrix_type;
 fwrite( (char*) &temp , UINTs , 1 , fp );
 
// UINT rows = (UINT) number_of_data_entries; // storing data as rows
 UINT rows = (UINT) nrows; // size_of_each_datum; // storing data as cols
 fwrite( (char*) &rows , UINTs , 1, fp );
 
// UINT cols = (UINT) size_of_each_datum; // storing data as rows
 UINT cols = (UINT) ncols; // number_of_data_entries; // storing data as cols
 fwrite( (char*) &cols, UINTs , 1 , fp );
 
 UINT imag = 0; // no complex matrices!
 fwrite( (char*) &imag, UINTs, 1 , fp );
 
 UINT name_length = variable_name.size(); // strlen( variable_name );
 fwrite( (char*) &name_length, UINTs, 1 , fp );

 // this is the end of the 20-byte header 

 // write the name
 
 fwrite( variable_name.c_str() , name_length , 1 , fp );

 return fp; 
}

FILE* write_matlab_header( unsigned int rows, unsigned int cols, std::string filename, std::string variable_name )
{
 return write_matlab4_header( rows, cols, filename, variable_name );  
}

bool write_matlab4( std::vector< std::vector<double> > input, std::string filename , std::string variable_name )
{
 int number_of_data_entries = input.size();
 int size_of_each_datum = input[0].size();
 
 int rows = size_of_each_datum; // storing data as cols
 int cols = number_of_data_entries; // storing data as cols
 
 FILE* fp = write_matlab4_header( rows, cols ,  filename, variable_name ); 

 // // storing data as rows 
 // for( int j=0; j < size_of_each_datum ; j++ )
 // { 
  // for( int i=0; i < number_of_data_entries ; i++ )
  // {
   // fwrite( (char*) &(input[i])[j] , sizeof(double), 1 , fp );
  // }
 // }

 // storing data as cols 
 for( int i=0; i < number_of_data_entries ; i++ )
 {
  for( int j=0; j < size_of_each_datum ; j++ )
  {
   fwrite( (char*) &(input[i])[j] , sizeof(double), 1 , fp );
  }
 }
 
 fclose( fp );
 return true;
}

bool write_matlab( std::vector< std::vector<double> >& input , std::string filename )
{
 return write_matlab4( input, filename , "none" );
}

};
