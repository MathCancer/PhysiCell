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
# Copyright (c) 2015-2018, Paul Macklin and the PhysiCell Project             #
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

#include "./PhysiCell_POV.h" 
using namespace BioFVM; 
	
Clipping_Plane::Clipping_Plane()
{
	normal = {0,-1,0};
	point_on_plane = {0,0,0}; 
	
	coefficients.resize(4,0.0); 
	for( int i=0; i < 3 ; i++ )
	{ coefficients[i] = normal[i]; }
	
	for( int i=0; i < 3 ; i++ )
	{
		coefficients[3] -= ( normal[i]*point_on_plane[i] ); 
	}

	return; 
}

void Clipping_Plane::normal_point_to_coefficients( void )
{
	normalize( &normal ); 
	
	for( int i=0; i < 3 ; i++ )
	{ coefficients[i] = normal[i]; }
	
	coefficients[3] = 0.0; 
	for( int i=0; i < 3 ; i++ )
	{
		coefficients[3] -= ( normal[i]*point_on_plane[i] ); 
	}

	return; 
}

void Clipping_Plane::coefficients_to_normal_point( void )
{
	for( int i=0; i < 3 ; i++ )
	{ normal[i] = coefficients[i]; }
	
	normalize( &normal ); 
	
	point_on_plane = normal; 
	point_on_plane *= -1.0; 
	point_on_plane *= coefficients[3]; 
	
	return; 
};

double Clipping_Plane::signed_distance_to_plane( std::vector<double>& test_point )
{
	double out = coefficients[3]; 
	// this assumes the normal vector is a unit vector! 
	for( int i=0; i < 3 ; i++ )
	{
		out += ( test_point[i]*coefficients[i] ); 
	}
	return out; 
}

bool Clipping_Plane::is_or_behind_plane( std::vector<double>& test_point )
{
	if( signed_distance_to_plane(test_point) <= 0.0 )
	{ return true; }
	return false; 
}

bool Clipping_Plane::is_in_front_of_plane( std::vector<double>& test_point ) 
{
	if( signed_distance_to_plane(test_point) > 0.0 )
	{ return true; }
	return false; 
}

void POV_Options::set_camera_from_spherical_location( double distance, double theta, double phi )
{
	camera_position = {0,0,0}; 
	
	camera_position[0] = domain_center[0] + distance*cos(theta)*sin(phi); 
	camera_position[1] = domain_center[1] + distance*sin(theta)*sin(phi); 
	camera_position[2] = domain_center[2] + distance*cos(phi); 
	return; 
}

POV_Options::POV_Options()
{
	max_trace_level = 1;
	assumed_gamma = 1.0; 
	
	domain_center = {0,0,0}; 
	domain_size = {2000,2000,2000}; 
	
	double pi = 3.141592653589793; 
	
	double camera_distance = ( domain_size[0] + domain_size[1] + domain_size[2] )*1.25 / 3.0; 
	set_camera_from_spherical_location( camera_distance , 5.0*pi/4.0 , pi/4.0 ); 
	
	background = {1,1,1}; 
	 
	camera_look_at = domain_center; 
	camera_right = {-1,0,0}; // computer graphics are goofy. switch to usual coordinate system
	camera_up = {0,0,1};
	camera_sky = {0,0,1};	
	
	light_position = {0,0,0}; 
	
	light_position[0] = domain_center[0] - 0.5*domain_size[0]; 
	light_position[1] = domain_center[1] - 0.5*domain_size[1]; 
	light_position[2] = domain_center[2] + 1.5*domain_size[2]; 

	light_rgb = 0.75; 
	light_fade_distance = norm( domain_size ); 
	light_fade_power = 2; 
	
	no_shadow = false; 
	no_reflection = false; 
	
	clipping_planes.resize( 0 ); 
	
	return; 
}

POV_Options default_POV_options; 

void Write_POV_start( POV_Options& options , std::ostream& os )
{
	os 	<< "#include \"colors.inc\"" << std::endl 
		<< "#include \"shapes.inc\" " << std::endl << std::endl 
		
		<< "global_settings {" << std::endl 
		<< "  max_trace_level " << options.max_trace_level << std::endl
		<< "  assumed_gamma " << options.assumed_gamma << std::endl << "}" << std::endl << std::endl
		
		<< "background {" << std::endl
		<< "  color rgb <" << options.background[0] << "," << options.background[1] << "," << options.background[2] << ">" << std::endl << "}" << std::endl << std::endl 
		
		<< "camera {" << std::endl 
		<< "  location <" << options.camera_position[0] << "," << options.camera_position[1] << "," << options.camera_position[2] << ">" << std::endl 
		<< "  right x" << std::endl
		<< "  look_at <" << options.camera_look_at[0] << "," << options.camera_look_at[1] << "," << options.camera_look_at[2] << ">" << std::endl
		<< "  right <" << options.camera_right[0] << "," << options.camera_right[1] << "," << options.camera_right[2] << ">" << std::endl
		<< "  up <" << options.camera_up[0] << "," << options.camera_up[1] << "," << options.camera_up[2] << ">" << std::endl 
		<< "  sky <" << options.camera_sky[0] << "," << options.camera_sky[1] << "," << options.camera_sky[2] << ">" << std::endl 
		<< " }" << std::endl << std::endl 
	
		<< "light_source {" << std::endl 
		<< "  <" << options.light_position[0] << "," << options.light_position[1] << "," << options.light_position[2] << ">" << std::endl 
		<< "  color rgb " << options.light_rgb << std::endl 
		<< "  fade_distance " << options.light_fade_distance << std::endl
		<< "  fade_power " << options.light_fade_power << std::endl
		<< "}" << std::endl << std::endl ; 
	
	return; 
}

void Write_POV_start( std::ostream& os )
{
	Write_POV_start( default_POV_options, os ); 
}

void Write_POV_sphere( std::ostream& os, std::vector<double>& center, double radius, std::vector<double>& pigment, std::vector<double>& finish )
{
	os 	<< "sphere" << std::endl << "{" << std::endl 
		<< " <" << center[0] << "," << center[1] << "," << center[2] << ">, " << radius 
		<< " pigment {color rgbf<" << pigment[0] << "," << pigment[1] << "," << pigment[2] << "," << pigment[3] << ">}" << std::endl
		<< " finish {ambient " << finish[0] << " diffuse " << finish[1] << " specular " << finish[2] << "}" << std::endl;
		
		if( default_POV_options.no_shadow )
		{ os << " no_shadow "; }
		if( default_POV_options.no_reflection )
		{ os << " no_reflection "; }
	os 	<< "}" << std::endl; 
	return;
}

					
