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
#include "BioFVM_mesh.h" 

namespace BioFVM{
	
/* voxels */
const int mesh_min_x_index=0;
const int mesh_min_y_index=1;
const int mesh_min_z_index=2;
const int mesh_max_x_index=3;
const int mesh_max_y_index=4;
const int mesh_max_z_index=5;	
Voxel::Voxel()
{
	mesh_index = 0; 
	volume = 10*10*10;
	center.assign( 3 , 0.0 ); 
	is_Dirichlet = false;
}

std::ostream& operator<<(std::ostream& os, const Voxel& v)  
{
	static std::string tabbing = "\t\t\t\t"; 
	static std::string tabbing2 = "\t\t\t\t\t"; 
	os	<< tabbing << "<voxel ID=\"" << v.mesh_index << "\">"  << std::endl
		<< tabbing2 << "<center " << v.center << " />" << std::endl  
		<< tabbing2 << "<volume>" << v.volume << "</volume>" << std::endl  
		<< tabbing  << "</voxel>"; 

 return os; 
}

void Voxel::stream_output_with_units( std::ostream& os , std::string units ) const 
{
	static std::string tabbing = "\t\t\t\t"; 
	static std::string tabbing2 = "\t\t\t\t\t"; 
	os	<< tabbing << "<voxel ID=\"" << mesh_index << "\">"  << std::endl
		<< tabbing2 << "<center " << center << " units=\"" << units << "\" />" << std::endl 
		<< tabbing2 << "<volume units=\"cubic " << units << "\">" << volume << "</volume>" << std::endl
		<< tabbing  << "</voxel>"; 
	return; 
}

/* voxel faces */ 

Voxel_Face::Voxel_Face()
{
	mesh_index = 0;  
	
	surface_area = 10*10; 
	center.assign( 3 , 0.0  ); 
	outward_normal.assign( 3 , 0.0 ); 
	inward_normal.assign( 3 , 0.0 ); 
}

std::ostream& operator<<(std::ostream& os, const Voxel_Face& vf)  
{
	static std::string tabbing = "\t\t\t\t"; 
	static std::string tabbing2 = "\t\t\t\t\t"; 
	os	<< tabbing << "<voxel_face ID=\"" << vf.mesh_index << "\">"  << std::endl
		<< tabbing2 << "<center " << vf.center << " />" << std::endl  
		<< tabbing2 << "<outward_normal " << vf.outward_normal << " />" << std::endl  
		<< tabbing2 << "<inward_normal " << vf.inward_normal << " />" << std::endl  
		<< tabbing2 << "<surface_area>" << vf.surface_area << "</surface_area>" << std::endl  
		<< tabbing  << "</voxel_face>"; 

 return os; 
}

void Voxel_Face::stream_output_with_units( std::ostream& os , std::string units ) const
{
	static std::string tabbing = "\t\t\t\t"; 
	static std::string tabbing2 = "\t\t\t\t\t"; 
	os	<< tabbing << "<voxel_face ID=\"" << mesh_index << "\">"  << std::endl
		<< tabbing2 << "<center units=\"" << units << "\" " << center << " />" << std::endl  
		<< tabbing2 << "<outward_normal units=\"" << units << "\" " << outward_normal << " />" << std::endl  
		<< tabbing2 << "<inward_normal units=\"" << units << "\" " << inward_normal << " />" << std::endl  
		<< tabbing2 << "<surface_area units=\"square " << units << "\">" << surface_area << "</surface_area>" << std::endl  
		<< tabbing  << "</voxel_face>"; 
}

/* general meshes */ 

General_Mesh::General_Mesh()
{
	// x1, x2, y1, y2, z1, z2 
	bounding_box.assign(6,0.0); 
	bounding_box[mesh_min_x_index] = -0.5; 
	bounding_box[mesh_min_y_index] = -0.5; 
	bounding_box[mesh_min_z_index] = -0.5; 
	bounding_box[mesh_max_x_index] = 0.5; 
	bounding_box[mesh_max_y_index] = 0.5; 
	bounding_box[mesh_max_z_index] = 0.5; 
	
	voxels.resize( 1 );  
	voxel_faces.resize( 0 ); 
	
	connected_voxel_indices.resize( 1 ); 
	connected_voxel_indices[0].clear(); 

	// voxel_face_index_mapping.clear();
	
	Cartesian_mesh = false; 
	uniform_mesh = false; 
	regular_mesh = false; 
	use_voxel_faces = true; 	
	units = "none"; 
}
 
std::ostream& operator<<(std::ostream& os, const General_Mesh& mesh)  
{
	std::boolalpha( os ); 
	static std::string tabbing = "\t"; 
	static std::string tabbing2 = "\t\t"; 
	static std::string tabbing3 = "\t\t\t"; 
	os	<< tabbing << "<mesh type=\"general\" uniform=\"" << mesh.uniform_mesh << "\" regular=\"" << mesh.regular_mesh << "\" units=\"" << mesh.units << "\">"  << std::endl
		<< tabbing2 << "<voxels>" << std::endl;
	for( unsigned int i=0; i < mesh.voxels.size() ; i++ )
	{ os << mesh.voxels[i] << std::endl; } 
	os 	<< tabbing2 << "</voxels>" << std::endl 
		<< tabbing2 << "<voxel_faces>" << std::endl;
	for( unsigned int i=0; i < mesh.voxel_faces.size() ; i++ )
	{ os << mesh.voxel_faces[i] << std::endl; } 
	os 	<< tabbing2 << "</voxel_faces>" << std::endl; 
	
	os	<< tabbing2 << "<voxel_connections>" << std::endl;
	for( unsigned int i=0 ; i < mesh.connected_voxel_indices.size() ; i++ )
	{
		os << tabbing3 << "<connected_voxel_indices ID=\"" << i << "\">" << std::endl; 
		for( unsigned int j=0; j < mesh.connected_voxel_indices[i].size() ; j++ )
		{
			os 	<< tabbing3 << "\t<index>" << (mesh.connected_voxel_indices[i])[j] << "</index>" << std::endl; 
		}
		os << tabbing3 << "</connected_voxel_indices>" << std::endl; 
	}
	
	os << tabbing2 << "</voxel_connections>" << std::endl; 
	
	
	os	<< tabbing  << "</mesh>"; 
		
		// later: output information on connected faces 
		
		// later: 

 return os; 
}

bool General_Mesh::is_position_valid(double x, double y, double z)
{
	if(x< bounding_box[mesh_min_x_index] || x>bounding_box[mesh_max_x_index])
		return false;
	if(y< bounding_box[mesh_min_y_index] || y>bounding_box[mesh_max_y_index])
		return false;
	if(z< bounding_box[mesh_min_z_index] || z>bounding_box[mesh_max_z_index])
		return false;
	return true;
}

void General_Mesh::connect_voxels_faces_only(int i,int j, double SA) // done 
{
	// check to see if the voxels are connected -- implement later!

	// create a new Voxel_Face connecting i to j

	Voxel_Face VF1; 
	int k = voxel_faces.size(); 
	VF1.mesh_index = k; 
	VF1.surface_area = SA; 
	VF1.outward_normal = voxels[j].center - voxels[i].center ; 
	normalize( &(VF1.outward_normal) );
	VF1.inward_normal = VF1.outward_normal; 
	VF1.inward_normal *= -1.0; 
	
	// convention: face is oriented from lower index to higher index 
	if( j < i )
	{ VF1.outward_normal *= -1.0; VF1.inward_normal *= -1.0; }

	// add it to the vector of voxel faces 
	
	voxel_faces.push_back( VF1 ); 

	return; 
}

void General_Mesh::connect_voxels_indices_only(int i,int j, double SA) // done
{
	// check to see if the voxels are connected -- implement later!
	
	// add j to the list of connected voxels for voxel i 
	
	connected_voxel_indices[i].push_back( j ); 
	connected_voxel_indices[j].push_back( i ); 

	return;
}

void General_Mesh::connect_voxels(int i,int j, double SA)
{
	// check to see if the voxels are connected -- implement later!

	// create a new Voxel_Face connecting i to j

	Voxel_Face VF1; 
	int k = voxel_faces.size(); 
	VF1.mesh_index = k; 
	VF1.surface_area = SA; 
	VF1.outward_normal = voxels[j].center - voxels[i].center ; 
	normalize( &(VF1.outward_normal) );
	VF1.inward_normal = VF1.outward_normal; 
	VF1.inward_normal *= -1.0; 
	
	// convention: face is oriented from lower index to higher index 
	if( j < i )
	{ VF1.outward_normal *= -1.0; VF1.inward_normal *= -1.0; }

	// add it to the vector of voxel faces 
	
	voxel_faces.push_back( VF1 ); 

	return; 
}

void General_Mesh::display_information( std::ostream& os )
{
	os << std::endl << "Mesh information: " << std::endl 
	<< "type: general mesh" << std::endl 
	<< "Domain: " 
	<< "[" << bounding_box[0] << "," << bounding_box[3] << "] " <<  units << " x " 
	<< "[" << bounding_box[1] << "," << bounding_box[4] << "] " <<  units << " x " 
	<< "[" << bounding_box[2] << "," << bounding_box[5] << "] " <<  units << std::endl
	<< "   voxels: " << voxels.size() << std::endl
	<< "   voxel faces: " << voxel_faces.size() << std::endl
	<< "   volume: "; 

	double total_volume = 0.0; 
	for( unsigned int i=0; i < voxels.size(); i++ )
	{ total_volume += voxels[i].volume; }
	os << total_volume << " cubic " << units << std::endl; 
 
	return; 
}

void General_Mesh::write_to_matlab( std::string filename )
{ 
	unsigned int number_of_data_entries = voxels.size();
	unsigned int size_of_each_datum = 3 + 1; // x,y,z, volume 

	FILE* fp = write_matlab_header( size_of_each_datum, number_of_data_entries,  filename, "mesh" );  

	// storing data as cols 
	for( unsigned int i=0; i < number_of_data_entries ; i++ )
	{
		fwrite( (char*) &( voxels[i].center[0] ) , sizeof(double) , 1 , fp ); 
		fwrite( (char*) &( voxels[i].center[1] ) , sizeof(double) , 1 , fp ); 
		fwrite( (char*) &( voxels[i].center[2] ) , sizeof(double) , 1 , fp ); 
		fwrite( (char*) &( voxels[i].volume ) , sizeof(double) , 1 , fp ); 
	}

	fclose( fp ); 
} 

void General_Mesh::read_from_matlab( std::string filename )
{
	unsigned int size_of_each_datum; 
	unsigned int number_of_data_entries; 
	FILE* fp = read_matlab_header( &size_of_each_datum, &number_of_data_entries,  filename ); 

	voxel_faces.resize( 0 ); 
	
	connected_voxel_indices.resize( 1 ); 
	connected_voxel_indices[0].clear(); 
	
	Cartesian_mesh = false; 
	uniform_mesh = false; 
	regular_mesh = false; 
	use_voxel_faces = false; 

	// resize the internal data structure 

	voxels.resize( number_of_data_entries );
	connected_voxel_indices.resize( voxels.size() ); 
	
	// read in the data
	// assumes each column has: x,y,z, dV
	
	bounding_box[0] = 9e99; 
	bounding_box[1] = 9e99; 
	bounding_box[2] = 9e99; 

	bounding_box[3] = -9e99; 
	bounding_box[4] = -9e99; 
	bounding_box[5] = -9e99; 
 
        size_t result;
	for( unsigned int i=0; i < number_of_data_entries ; i++ )
	{
		result = fread( (char*) &( voxels[i].center[0] ) , sizeof(double) , 1 , fp ); 
		result = fread( (char*) &( voxels[i].center[1] ) , sizeof(double) , 1 , fp ); 
		result = fread( (char*) &( voxels[i].center[2] ) , sizeof(double) , 1 , fp ); 
		result = fread( (char*) &( voxels[i].volume ) , sizeof(double) , 1 , fp ); 
		
		// estimate the bounding box; 
		if( voxels[i].center[0] < bounding_box[0] )
		{ bounding_box[0] = voxels[i].center[0]; }
		if( voxels[i].center[0] > bounding_box[3] )
		{ bounding_box[3] = voxels[i].center[0]; }

		if( voxels[i].center[1] < bounding_box[1] )
		{ bounding_box[1] = voxels[i].center[1]; }
		if( voxels[i].center[1] > bounding_box[4] )
		{ bounding_box[4] = voxels[i].center[1]; }

		if( voxels[i].center[2] < bounding_box[2] )
		{ bounding_box[2] = voxels[i].center[2]; }
		if( voxels[i].center[2] > bounding_box[5] )
		{ bounding_box[5] = voxels[i].center[2]; }
	} 
	
	std::cout << "Warning: General_Mesh::read_from_matlab is incomplete. No connection information read." << std::endl; 

	fclose( fp) ; 
	return; 
}

/* Cartesian meshes */

Cartesian_Mesh::Cartesian_Mesh()
{
	Cartesian_mesh = true; 
	uniform_mesh = true; 
	regular_mesh = true; 
	use_voxel_faces = false; 
	
	x_coordinates.assign( 1 , 0.0 ); 
	y_coordinates.assign( 1 , 0.0 ); 
	z_coordinates.assign( 1 , 0.0 ); 
	
	dx = bounding_box[3] - bounding_box[0]; 
	dy = bounding_box[4] - bounding_box[1]; 
	dz = bounding_box[5] - bounding_box[2]; 
	
	static double tolerance = 1e-12; 
	
	if( fabs( dx - dy ) > tolerance ||
		fabs( dy - dz ) > tolerance ||
		fabs( dx - dz ) > tolerance )
	{ uniform_mesh = false; }
	
	dV = dx*dy*dz; 
	dS = dx*dy; 
	dS_xy = dx*dy; 
	dS_yz = dy*dz; 
	dS_xz = dx*dz; 
	
	Voxel template_voxel;
	template_voxel.volume = dV; 
 
	voxels.assign( x_coordinates.size() * y_coordinates.size() * z_coordinates.size() , template_voxel ); 
	voxels[0].center[0] = x_coordinates[0]; 
	voxels[0].center[1] = y_coordinates[0]; 
	voxels[0].center[2] = z_coordinates[0]; 
}

void Cartesian_Mesh::create_voxel_faces( void )
{
	// make connections 
	
	connected_voxel_indices.resize( voxels.size() ); 
	
	int i_jump = 1; 
	int j_jump = x_coordinates.size(); 
	int k_jump = x_coordinates.size() * y_coordinates.size(); 
		
	// x-aligned connections 
	for( unsigned int k=0 ; k < z_coordinates.size() ; k++ )
	{
		for( unsigned int j=0 ; j < y_coordinates.size() ; j++ )
		{
			for( unsigned int i=0 ; i < x_coordinates.size()-1 ; i++ )
			{
				int n = voxel_index(i,j,k); 
				connect_voxels_faces_only(n,n+i_jump, dS_yz ); 
			}
		}
	}
	// y-aligned connections 
	for( unsigned int k=0 ; k < z_coordinates.size() ; k++ )
	{
		for( unsigned int i=0 ; i < x_coordinates.size() ; i++ )
		{
			for( unsigned int j=0 ; j < y_coordinates.size()-1 ; j++ )
			{
				int n = voxel_index(i,j,k); 
				connect_voxels_faces_only(n,n+j_jump, dS_xz ); 
			}
		}
	}	
	// z-aligned connections 
	for( unsigned int j=0 ; j < y_coordinates.size() ; j++ )
	{
		for( unsigned int i=0 ; i < x_coordinates.size() ; i++ )
		{
			for( unsigned int k=0 ; k < z_coordinates.size()-1 ; k++ )
			{
				int n = voxel_index(i,j,k); 
				connect_voxels_faces_only(n,n+k_jump, dS_xy ); 
			}
		}
	}	

	return; 
}

Cartesian_Mesh::Cartesian_Mesh( int xnodes, int ynodes, int znodes )
{
	x_coordinates.assign( xnodes , 0.0 ); 
	y_coordinates.assign( ynodes , 0.0 ); 
	z_coordinates.assign( znodes , 0.0 ); 
	
	dx = 1;
	dy = 1; 
	dz = 1; 
	
	dV = dx*dx*dz; 
	dS = dx*dy; 
	
	dS_xy = dS; 
	dS_yz = dS; 
	dS_xz = dS; 

	uniform_mesh = true; 
	regular_mesh = true; 
	use_voxel_faces = false; 
 
	for( unsigned int i=0; i < x_coordinates.size() ; i++ )
	{ x_coordinates[i] = i*dx; }
	for( unsigned int i=0; i < y_coordinates.size() ; i++ )
	{ y_coordinates[i] = i*dy; }
	for( unsigned int i=0; i < z_coordinates.size() ; i++ )
	{ z_coordinates[i] = i*dz; }	
	
	bounding_box[0] = x_coordinates[0]-dx/2.0; 
	bounding_box[3] = x_coordinates[x_coordinates.size()-1]+dx/2.0; 
	bounding_box[1] = y_coordinates[0]-dy/2.0; 
	bounding_box[4] = y_coordinates[y_coordinates.size()-1]+dy/2.0; 
	bounding_box[2] = z_coordinates[0]-dz/2.0; 
	bounding_box[5] = z_coordinates[z_coordinates.size()-1]+dz/2.0; 
	
	Voxel template_voxel;
	template_voxel.volume = dV; 

	units = "none"; 
	
	voxels.assign( x_coordinates.size() * y_coordinates.size() * z_coordinates.size() , template_voxel ); 

	// initializing and connecting voxels 
 
	int n=0; 
	for( unsigned int k=0 ; k < z_coordinates.size() ; k++ )
	{
		for( unsigned int j=0 ; j < y_coordinates.size() ; j++ )
		{
			for( unsigned int i=0 ; i < x_coordinates.size() ; i++ )
			{
				voxels[n].center[0] = x_coordinates[i]; 
				voxels[n].center[1] = y_coordinates[j]; 
				voxels[n].center[2] = z_coordinates[k]; 
				voxels[n].mesh_index = n; 
				voxels[n].volume = dV; 

				n++; 
			}
		}
	}
	
	// make connections 
	
	connected_voxel_indices.resize( voxels.size() ); 
	
	int i_jump = 1; 
	int j_jump = x_coordinates.size(); 
	int k_jump = x_coordinates.size() * y_coordinates.size(); 
		
	// x-aligned connections 
	for( unsigned int k=0 ; k < z_coordinates.size() ; k++ )
	{
		for( unsigned int j=0 ; j < y_coordinates.size() ; j++ )
		{
			for( unsigned int i=0 ; i < x_coordinates.size()-1 ; i++ )
			{
				int n = voxel_index(i,j,k); 
				connect_voxels_indices_only(n,n+i_jump, dS_yz ); 
			}
		}
	}
	// y-aligned connections 
	for( unsigned int k=0 ; k < z_coordinates.size() ; k++ )
	{
		for( unsigned int i=0 ; i < x_coordinates.size() ; i++ )
		{
			for( unsigned int j=0 ; j < y_coordinates.size()-1 ; j++ )
			{
				int n = voxel_index(i,j,k); 
				connect_voxels_indices_only(n,n+j_jump, dS_xz ); 
			}
		}
	}	
	// z-aligned connections 
	for( unsigned int j=0 ; j < y_coordinates.size() ; j++ )
	{
		for( unsigned int i=0 ; i < x_coordinates.size() ; i++ )
		{
			for( unsigned int k=0 ; k < z_coordinates.size()-1 ; k++ )
			{
				int n = voxel_index(i,j,k); 
				connect_voxels_indices_only(n,n+k_jump, dS_xy ); 
			}
		}
	}	
	
	if( use_voxel_faces )
	{ create_voxel_faces(); }
}	 

void Cartesian_Mesh::create_moore_neighborhood()
{
	moore_connected_voxel_indices.resize( voxels.size() );
	for( unsigned int j=0 ; j < y_coordinates.size() ; j++ )
	{
		for( unsigned int i=0 ; i < x_coordinates.size() ; i++ )
		{
			for( unsigned int k=0 ; k < z_coordinates.size() ; k++ )
			{
				int center_inex = voxel_index(i,j,k); 
				for(int ii=-1;ii<=1;ii++)
					for(int jj=-1;jj<=1;jj++)
						for(int kk=-1;kk<=1;kk++)
							if(i+ii>=0 && i+ii<x_coordinates.size() &&
								j+jj>=0 && j+jj<y_coordinates.size() &&
								k+kk>=0 && k+kk<z_coordinates.size() &&
								!(ii==0 && jj==0 && kk==0))
								{
									int neighbor_index= voxel_index(i+ii,j+jj,k+kk);
									moore_connected_voxel_indices[center_inex].push_back( neighbor_index );
								}
			}
		}
	}
}
unsigned int Cartesian_Mesh::voxel_index( unsigned int i, unsigned int j, unsigned int k )
{
 return ( k*y_coordinates.size() + j )*x_coordinates.size() + i; 
}

std::vector<unsigned int> Cartesian_Mesh::cartesian_indices( unsigned int n )
{
	std::vector<unsigned int> out(3, -1 ); 

	// figure out i; 
	unsigned int XY = x_coordinates.size() * y_coordinates.size();
	out[2] = (unsigned int) floor( n/XY ); 
 
	// figure out j; 
	out[1] = (unsigned int) floor(   (n - out[2]*XY) / x_coordinates.size() );
 
	// figure out k; 
	out[0] = n - x_coordinates.size()*(   out[1] + y_coordinates.size()*out[2] ); 

	return out; 
}

void Cartesian_Mesh::resize( double x_start, double x_end, double y_start, double y_end, double z_start, double z_end , int x_nodes, int y_nodes, int z_nodes )
{
	x_coordinates.assign( x_nodes , 0.0 ); 
	y_coordinates.assign( y_nodes , 0.0 ); 
	z_coordinates.assign( z_nodes , 0.0 ); 

	dx = ( x_end - x_start )/( (double) x_nodes ); 
	if( x_nodes < 2 )
	{ dx = 1; }
	dy = ( y_end - y_start )/( (double) y_nodes ); 
	if( y_nodes < 2 )
	{ dy = 1; }
	dz = ( z_end - z_start )/( (double) z_nodes  ); 
	if( z_nodes < 2 )
	{ dz = 1; }

	uniform_mesh = true; 
	regular_mesh = true; 
	static double tol = 1e-16; 
 
	if( fabs( dx - dy ) > tol && x_nodes > 1 && y_nodes > 1 )
	{ uniform_mesh = false; }
	if( fabs( dy - dz ) > tol && y_nodes > 1 && z_nodes > 1 )
	{ uniform_mesh = false; }
	if( fabs( dx - dz ) > tol && x_nodes > 1 && z_nodes > 1 )
	{ uniform_mesh = false; }

	for( unsigned int i=0; i < x_coordinates.size() ; i++ )
	{ x_coordinates[i] = x_start + (i+0.5)*dx; }
	for( unsigned int i=0; i < y_coordinates.size() ; i++ )
	{ y_coordinates[i] = y_start + (i+0.5)*dy; }
	for( unsigned int i=0; i < z_coordinates.size() ; i++ )
	{ z_coordinates[i] = z_start + (i+0.5)*dz; }

	bounding_box[0] = x_start; 
	bounding_box[3] = x_end; 
	bounding_box[1] = y_start; 
	bounding_box[4] = y_end; 
	bounding_box[2] = z_start; 
	bounding_box[5] = z_end; 

	dV = dx*dy*dz; 
	dS = dx*dy; 
	
	dS_xy = dx*dy; 
	dS_yz = dy*dz; 
	dS_xz = dx*dz; 

	Voxel template_voxel;
	template_voxel.volume = dV; 

	voxels.assign( x_coordinates.size() * y_coordinates.size() * z_coordinates.size() , template_voxel ); 

	int n=0; 
	for( unsigned int k=0 ; k < z_coordinates.size() ; k++ )
	{
		for( unsigned int j=0 ; j < y_coordinates.size() ; j++ )
		{
			for( unsigned int i=0 ; i < x_coordinates.size() ; i++ )
			{
				voxels[n].center[0] = x_coordinates[i]; 
				voxels[n].center[1] = y_coordinates[j]; 
				voxels[n].center[2] = z_coordinates[k]; 
				voxels[n].mesh_index = n; 
				voxels[n].volume = dV; 

				n++; 
			}
		}
	}
	
	// make connections 
	
	connected_voxel_indices.resize( voxels.size() ); 
	voxel_faces.clear(); 
	
	for( unsigned int i=0; i < connected_voxel_indices.size() ; i++ )
	{ connected_voxel_indices[i].clear(); }
	int i_jump = 1; 
	int j_jump = x_coordinates.size(); 
	int k_jump = x_coordinates.size() * y_coordinates.size(); 
	
	// x-aligned connections 
	int count = 0; 
	for( unsigned int k=0 ; k < z_coordinates.size() ; k++ )
	{
		for( unsigned int j=0 ; j < y_coordinates.size() ; j++ )
		{
			for( unsigned int i=0 ; i < x_coordinates.size()-1 ; i++ )
			{
				int n = voxel_index(i,j,k); 
				connect_voxels_indices_only(n,n+i_jump, dS_yz ); 
				count++; 
			}
		}
	}
	// y-aligned connections 
	for( unsigned int k=0 ; k < z_coordinates.size() ; k++ )
	{
		for( unsigned int i=0 ; i < x_coordinates.size() ; i++ )
		{
			for( unsigned int j=0 ; j < y_coordinates.size()-1 ; j++ )
			{
				int n = voxel_index(i,j,k); 
				connect_voxels_indices_only(n,n+j_jump, dS_xz ); 
			}
		}
	}	
	// z-aligned connections 
	for( unsigned int j=0 ; j < y_coordinates.size() ; j++ )
	{
		for( unsigned int i=0 ; i < x_coordinates.size() ; i++ )
		{
			for( unsigned int k=0 ; k < z_coordinates.size()-1 ; k++ )
			{
				int n = voxel_index(i,j,k); 
				connect_voxels_indices_only(n,n+k_jump, dS_xy ); 
			}
		}
	}
	
	if( use_voxel_faces )
	{ create_voxel_faces(); }
	
	create_moore_neighborhood();
	return; 
}

void Cartesian_Mesh::resize( double x_start, double x_end, double y_start, double y_end, double z_start, double z_end , double dx_new, double dy_new , double dz_new )
{
	dx = dx_new;
	dy = dy_new; 
	dz = dz_new; 

	double eps = 1e-16; 
	int x_nodes = (int) ceil( eps + (x_end-x_start)/dx ); 
	int y_nodes = (int) ceil( eps + (y_end-y_start)/dy ); 
	int z_nodes = (int) ceil( eps + (z_end-z_start)/dz ); 

	x_coordinates.assign( x_nodes , 0.0 ); 
	y_coordinates.assign( y_nodes , 0.0 ); 
	z_coordinates.assign( z_nodes , 0.0 ); 

	uniform_mesh = true; 
	regular_mesh = true; 
	double tol = 1e-16; 
	if( fabs( dx - dy ) > tol || fabs( dy - dz ) > tol || fabs( dx - dz ) > tol )
	{ uniform_mesh = false; }

	for( unsigned int i=0; i < x_coordinates.size() ; i++ )
	{ x_coordinates[i] = x_start + (i+0.5)*dx; }
	for( unsigned int i=0; i < y_coordinates.size() ; i++ )
	{ y_coordinates[i] = y_start + (i+0.5)*dy; }
	for( unsigned int i=0; i < z_coordinates.size() ; i++ )
	{ z_coordinates[i] = z_start + (i+0.5)*dz; }

	bounding_box[0] = x_start; 
	bounding_box[3] = x_end; 
	bounding_box[1] = y_start; 
	bounding_box[4] = y_end; 
	bounding_box[2] = z_start; 
	bounding_box[5] = z_end; 

	dV = dx*dy*dz; 
	dS = dx*dy; 

	dS_xy = dx*dy; 
	dS_yz = dy*dz; 
	dS_xz = dx*dz; 
	
	Voxel template_voxel;
	template_voxel.volume = dV; 

	voxels.assign( x_coordinates.size() * y_coordinates.size() * z_coordinates.size() , template_voxel ); 
	
	int n=0; 
	for( unsigned int k=0 ; k < z_coordinates.size() ; k++ )
	{
		for( unsigned int j=0 ; j < y_coordinates.size() ; j++ )
		{
			for( unsigned int i=0 ; i < x_coordinates.size() ; i++ )
			{
				voxels[n].center[0] = x_coordinates[i]; 
				voxels[n].center[1] = y_coordinates[j]; 
				voxels[n].center[2] = z_coordinates[k]; 
				voxels[n].mesh_index = n; 
				voxels[n].volume = dV; 

				n++; 
			}
		}
	}
	
	// make connections 
	
	connected_voxel_indices.resize( voxels.size() ); 
	voxel_faces.clear(); 
	
	for( unsigned int i=0; i < connected_voxel_indices.size() ; i++ )
	{ connected_voxel_indices[i].clear(); }
	
	int i_jump = 1; 
	int j_jump = x_coordinates.size(); 
	int k_jump = x_coordinates.size() * y_coordinates.size(); 
	
	// x-aligned connections 
	int count = 0; 
	for( unsigned int k=0 ; k < z_coordinates.size() ; k++ )
	{
		for( unsigned int j=0 ; j < y_coordinates.size() ; j++ )
		{
			for( unsigned int i=0 ; i < x_coordinates.size()-1 ; i++ )
			{
				int n = voxel_index(i,j,k); 
				connect_voxels_indices_only(n,n+i_jump, dS_yz ); 
				count++; 
			}
		}
	}

	// y-aligned connections 
	for( unsigned int k=0 ; k < z_coordinates.size() ; k++ )
	{
		for( unsigned int i=0 ; i < x_coordinates.size() ; i++ )
		{
			for( unsigned int j=0 ; j < y_coordinates.size()-1 ; j++ )
			{
				int n = voxel_index(i,j,k); 
				connect_voxels_indices_only(n,n+j_jump, dS_xz ); 
			}
		}
	}	

	// z-aligned connections 
	for( unsigned int j=0 ; j < y_coordinates.size() ; j++ )
	{
		for( unsigned int i=0 ; i < x_coordinates.size() ; i++ )
		{
			for( unsigned int k=0 ; k < z_coordinates.size()-1 ; k++ )
			{
				int n = voxel_index(i,j,k); 
				connect_voxels_indices_only(n,n+k_jump, dS_xy ); 
			}
		}
	}
	
	if( use_voxel_faces )
	{ create_voxel_faces(); }
	
	create_moore_neighborhood();
	return; 
}

void Cartesian_Mesh::resize( int x_nodes, int y_nodes, int z_nodes )
{ return resize( 0-.5, x_nodes-1+.5 , 0-.5 , y_nodes-1+.5 , 0-.5 , z_nodes - 1+.5 , x_nodes, y_nodes, z_nodes ); } 

void Cartesian_Mesh::resize_uniform( double x_start, double x_end, double y_start, double y_end, double z_start, double z_end , double dx_new )
{ return resize( x_start, x_end, y_start, y_end, z_start, z_end , dx_new, dx_new , dx_new ); }

int Cartesian_Mesh::nearest_voxel_index( std::vector<double>& position )
{
	unsigned int i = (unsigned int) floor( (position[0]-bounding_box[0])/dx ); 
	unsigned int j = (unsigned int) floor( (position[1]-bounding_box[1])/dy ); 
	unsigned int k = (unsigned int) floor( (position[2]-bounding_box[2])/dz ); 

	//  add some bounds checking -- truncate to inside the computational domain   

	if( i >= x_coordinates.size() ){ i = x_coordinates.size()-1; }
	if( i < 0 ){ i = 0; }

	if( j >= y_coordinates.size() ){ j = y_coordinates.size()-1; }
	if( j < 0 ){ j = 0; }

	if( k >= z_coordinates.size() ){ k = z_coordinates.size()-1; }
	if( k < 0 ){ k = 0; }

	return ( k*y_coordinates.size() + j )*x_coordinates.size() + i; 
}

std::vector<unsigned int> Cartesian_Mesh::nearest_cartesian_indices( std::vector<double>& position )
{
	std::vector<unsigned int> out; 
	out.assign(3, 0 ); 
	out[0] = (unsigned int) floor( (position[0]-bounding_box[0])/dx ); 
	out[1] = (unsigned int) floor( (position[1]-bounding_box[1])/dy ); 
	out[2] = (unsigned int) floor( (position[2]-bounding_box[2])/dz ); 

	//  add some bounds checking -- truncate to inside the computational domain  

	if( out[0] >= x_coordinates.size() ){ out[0] = x_coordinates.size()-1; }
	if( out[0] < 0 ){ out[0] = 0; }

	if( out[1] >= y_coordinates.size() ){ out[1] = y_coordinates.size()-1; }
	if( out[1] < 0 ){ out[1] = 0; }

	if( out[2] >= z_coordinates.size() ){ out[2] = z_coordinates.size()-1; }
	if( out[2] < 0 ){ out[2] = 0; }

	return out; 
}

Voxel& Cartesian_Mesh::nearest_voxel( std::vector<double>& position )
{ return voxels[ nearest_voxel_index( position ) ]; }

void Cartesian_Mesh::display_information( std::ostream& os )
{
	os << std::endl << "Mesh information: " << std::endl;
	if( uniform_mesh ) 
	{ os << "type: uniform Cartesian" << std::endl; }
	else
	{
		if( regular_mesh )
		{ os << "type: regular Cartesian" << std::endl; }
		else
		{ os << "type: general Cartesian" << std::endl; }
	}
	os << "Domain: " 
	<< "[" << bounding_box[0] << "," << bounding_box[3] << "] " <<  units << " x " 
	<< "[" << bounding_box[1] << "," << bounding_box[4] << "] " <<  units << " x " 
	<< "[" << bounding_box[2] << "," << bounding_box[5] << "] " <<  units << std::endl
	<< "   resolution: dx = " << dx << " " << units; 
	if( !uniform_mesh )
	{
		os	<< ", dy = " << dy << " " << units 
			<< ", dz = " << dz << " " << units ; 
	}
	os << std::endl 
	<< "   voxels: " << voxels.size() << std::endl
	<< "   voxel faces: " << voxel_faces.size() << std::endl
	<< "   volume: " << ( bounding_box[3]-bounding_box[0] )*( bounding_box[4]-bounding_box[1] )*( bounding_box[5]-bounding_box[2] ) 
		<< " cubic " << units << std::endl; 	

	return; 
}

void Cartesian_Mesh::read_from_matlab( std::string filename )
{
	unsigned int size_of_each_datum; 
	unsigned int number_of_data_entries; 
	FILE* fp = read_matlab_header( &size_of_each_datum, &number_of_data_entries,  filename ); 

	voxel_faces.resize( 0 ); 
	
	connected_voxel_indices.resize( 1 ); 
	connected_voxel_indices[0].clear(); 
		
	Cartesian_mesh = true; 
	uniform_mesh = false; 
	regular_mesh = true; 
	use_voxel_faces = false; 

	// resize the internal data structure 

	voxels.resize( number_of_data_entries );
	connected_voxel_indices.resize( voxels.size() ); 
	
	x_coordinates.resize( number_of_data_entries );
	y_coordinates.resize( number_of_data_entries );
	z_coordinates.resize( number_of_data_entries );
	
	// read in the data
	// assumes each column has: x,y,z, dV
	
	bounding_box[0] = 9e99; 
	bounding_box[1] = 9e99; 
	bounding_box[2] = 9e99; 

	bounding_box[3] = -9e99; 
	bounding_box[4] = -9e99; 
	bounding_box[5] = -9e99; 
 
        size_t result;
	for( unsigned int i=0; i < number_of_data_entries ; i++ )
	{
		result = fread( (char*) &( voxels[i].center[0] ) , sizeof(double) , 1 , fp ); 
		result = fread( (char*) &( voxels[i].center[1] ) , sizeof(double) , 1 , fp ); 
		result = fread( (char*) &( voxels[i].center[2] ) , sizeof(double) , 1 , fp ); 
		result = fread( (char*) &( voxels[i].volume ) , sizeof(double) , 1 , fp ); 
		
		// estimate the bounding box; 
		if( voxels[i].center[0] < bounding_box[0] )
		{ bounding_box[0] = voxels[i].center[0]; }
		if( voxels[i].center[0] > bounding_box[3] )
		{ bounding_box[3] = voxels[i].center[0]; }

		if( voxels[i].center[1] < bounding_box[1] )
		{ bounding_box[1] = voxels[i].center[1]; }
		if( voxels[i].center[1] > bounding_box[4] )
		{ bounding_box[4] = voxels[i].center[1]; }

		if( voxels[i].center[2] < bounding_box[2] )
		{ bounding_box[2] = voxels[i].center[2]; }
		if( voxels[i].center[2] > bounding_box[5] )
		{ bounding_box[5] = voxels[i].center[2]; }
	} 
	
	// figure out dx, dy, dz 

	double xmin = bounding_box[0]; // voxels[0].center[0]; 
	double ymin = bounding_box[1]; // voxels[0].center[1]; 
	double zmin = bounding_box[2]; // voxels[0].center[2]; 

	// int n = voxels.size(); 
	double xmax = bounding_box[3]; // voxels[n-1].center[0]; 
	double ymax = bounding_box[4]; // voxels[n-1].center[1]; 
	double zmax = bounding_box[5]; // voxels[n-1].center[2]; 

	// figure out number of x nodes  
	int xnodes = 0; 
	while( fabs( voxels[xnodes].center[0] - xmax ) > 1e-15 )
	{ xnodes++; }
	xnodes++; 

	// figure out number of y nodes 
	int ynodes = 0; 

	while( fabs( voxels[ynodes*xnodes].center[1] - ymax ) > 1e-15 )
	{ ynodes += 1; }
	ynodes++;

	// figure out number of z nodes 

	int znodes = 0; 

	while( fabs( voxels[ynodes*xnodes*znodes].center[2] - zmax ) > 1e-15 )
	{ znodes += 1; }
	znodes++;

	// figure out differentials

	dx = ( xmax - xmin ) / ( (double) xnodes - 1.0  ); 
	dy = ( ymax - ymin ) / ( (double) ynodes - 1.0  ); 
	dz = ( zmax - zmin ) / ( (double) znodes - 1.0  ); 
	
	dV = dx*dy*dz; 
	dS = dx*dy;

	dS_xy = dx*dy;
	dS_yz = dy*dz; 
	dS_xz = dx*dz;

	uniform_mesh = true; 
	double tol = 1e-16; 
	if( fabs( dx - dy ) > tol || fabs( dy - dz ) > tol || fabs( dx - dz ) > tol )
	{ uniform_mesh = false; }
	
	// correct the bounding box 
	
	double half_step = dx * 0.5; 
	
	bounding_box[0] -= half_step; 
	bounding_box[3] += half_step;
	
	half_step = dy * 0.5; 
	bounding_box[1] -= half_step; 
	bounding_box[4] += half_step;
	
	half_step = dz * 0.5; 
	bounding_box[2] -= half_step; 
	bounding_box[5] += half_step;
 
	// write out the x,y,z coordinates; 
	x_coordinates.resize( xnodes ); 
	y_coordinates.resize( ynodes ); 
	z_coordinates.resize( znodes ); 

	for( unsigned int i=0; i < x_coordinates.size() ; i++ )
	{ x_coordinates[i] = xmin + i*dx ;   }

	for( unsigned int i=0; i < y_coordinates.size() ; i++ )
	{ y_coordinates[i] = ymin + i*dy ; }

	for( unsigned int i=0; i < z_coordinates.size() ; i++ )
	{ z_coordinates[i] = zmin + i*dz ; }

	dV = dx*dy*dz; 
	
	units = "none";  
	
	// still need to figure out connected indices later 
	std::cout << "Warning: Cartesian_Mesh::read_from_matlab is incomplete. No connection information read." << std::endl; 

	fclose( fp) ; 
	return; 
}

void Voronoi_Mesh::display_information( std::ostream& os )
{
	os << std::endl << "Mesh information: " << std::endl 
	<< "type: Voronoi (not implemented!)" << std::endl
	<< "Domain: " 
	<< "[" << bounding_box[0] << "," << bounding_box[3] << "] " <<  units << " x " 
	<< "[" << bounding_box[1] << "," << bounding_box[4] << "] " <<  units << " x " 
	<< "[" << bounding_box[2] << "," << bounding_box[5] << "] " <<  units << std::endl
	<< "   voxels: " << voxels.size() << std::endl
	<< "   voxel faces: " << voxel_faces.size() << std::endl 
	<< "   volume: "; 

	double total_volume = 0.0; 
	for( unsigned int i=0; i < voxels.size(); i++ )
	{ total_volume += voxels[i].volume; }
	os << total_volume << " cubic " << units << std::endl; 

	return; 
}

};
