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

#include "./PhysiCell_geometry.h"

namespace PhysiCell{

// square fills 

void fill_rectangle( std::vector<double> bounds , Cell_Definition* pCD , double compression )
{
	double cell_radius = pCD->phenotype.geometry.radius; 
	double spacing = compression * cell_radius * 2.0; 
	double half_space = 0.5*spacing; 
	double y_offset = sqrt(3.0)*half_space; 
	
	// bounds? [xmin,ymin, zmin , xmax,ymax, zmax] 
	// assume z = 0.5*(zmin+zmax) 
	double Xmin; 
	double Xmax; 
	double Ymin; 
	double Ymax; 
	double Zmin; 
	double Zmax; 
	if( bounds.size() == 4 ) // only gave xmin,ymin,xmax,ymax 
	{
		Xmin = bounds[0]; 
		Ymin = bounds[1]; 
		
		Xmax = bounds[2]; 
		Ymax = bounds[3]; 
		
		Zmin = 0.0; 
		Zmax = 0.0; 
	}
	else
	{
		Xmin = bounds[0]; 
		Ymin = bounds[1]; 
		Zmin = bounds[2]; 
		
		Xmax = bounds[3]; 
		Ymax = bounds[4]; 
		Zmax = bounds[5]; 
	}
	
	double x = Xmin + cell_radius; 
	double y = Ymin + cell_radius; 
	double z = 0.5*( Zmin + Zmax ); 
	
	int n = 0; 
	
	while( y <= Ymax - cell_radius )
	{
		while( x <= Xmax - cell_radius )
		{
			Cell* pC = create_cell( *pCD ); 
			pC->assign_position( x,y,z ); 
			
			x += spacing; 
		}
		x = Xmin + half_space; 
		
		n++; 
		y += y_offset; 
		if( n % 2 == 1 )
		{ x += half_space; }
	}
	return; 
}

void fill_rectangle( std::vector<double> bounds , Cell_Definition* pCD )
{ return fill_rectangle(bounds,pCD,1.0); } 

void fill_rectangle( std::vector<double> bounds , int cell_type , double compression )
{ return fill_rectangle(bounds,find_cell_definition(cell_type),compression); }

void fill_rectangle( std::vector<double> bounds , int cell_type )
{ return fill_rectangle(bounds,find_cell_definition(cell_type),1.0); }

// circle fills 

void fill_circle( std::vector<double> center , double radius , Cell_Definition* pCD , double compression )
{
	double cell_radius = pCD->phenotype.geometry.radius; 
	double spacing = compression * cell_radius * 2.0; 
	double half_space = 0.5*spacing; 
	double y_offset = sqrt(3.0)*half_space; 
	
	double r_m_cr_2 = (radius-cell_radius)*(radius-cell_radius);  
	
	double Xmin = center[0] - radius; 
	double Xmax = center[0] + radius; 

	double Ymin = center[1] - radius; 
	double Ymax = center[1] + radius; 
	
	double x = Xmin + cell_radius; 
	double y = Ymin + cell_radius; 
	double z = center[2]; 
	
	int n = 0; 
	
	while( y <= Ymax - cell_radius )
	{
		while( x <= Xmax - cell_radius )
		{
			double d2 = (center[0]-x)*(center[0]-x) + (center[1]-y)*(center[1]-y); 
			// if we're within the circle, accept position and lay the cell 
			// essentially, we are applying a circular mask 
			if( d2 <= r_m_cr_2 )
			{
				Cell* pC = create_cell( *pCD ); 
				pC->assign_position( x,y,z ); 
			}
			x += spacing; 
		}
		y += y_offset; 
		n++; 
		
		x = Xmin+cell_radius;
		if( n % 2 == 1 )
		{ x += half_space; }
	}
	return; 
}

void fill_circle( std::vector<double> center , double radius , Cell_Definition* pCD )
{ return fill_circle( center,radius,pCD,1.0); } 

void fill_circle( std::vector<double> center , double radius , int cell_type , double compression )
{ return fill_circle( center,radius,find_cell_definition(cell_type),compression); } 

void fill_circle( std::vector<double> center , double radius , int cell_type ) 
{ return fill_circle( center,radius,find_cell_definition(cell_type),1); } 

// annulus 

void fill_annulus( std::vector<double> center , double outer_radius, double inner_radius , Cell_Definition* pCD , double compression )
{
	double cell_radius = pCD->phenotype.geometry.radius; 
	double spacing = compression * cell_radius * 2.0; 
	double half_space = 0.5*spacing; 
	double y_offset = sqrt(3.0)*half_space; 
	
	double ro_m_cr_2 = (outer_radius-cell_radius)*(outer_radius-cell_radius);  
	double ri_p_cr_2 = (inner_radius+cell_radius)*(inner_radius+cell_radius);  
	
	double Xmin = center[0] - outer_radius; 
	double Xmax = center[0] + outer_radius; 

	double Ymin = center[1] - outer_radius; 
	double Ymax = center[1] + outer_radius; 
	
	double x = Xmin + cell_radius; 
	double y = Ymin + cell_radius; 
	double z = center[2]; 
	
	int n = 0; 
	
	while( y <= Ymax - cell_radius )
	{
		while( x <= Xmax - cell_radius )
		{
			double d2 = (center[0]-x)*(center[0]-x) + (center[1]-y)*(center[1]-y); 
			// if we're within the circle, accept position and lay the cell 
			// essentially, we are applying a circular mask 
			if( d2 <= ro_m_cr_2 && d2 >= ri_p_cr_2 )
			{
				Cell* pC = create_cell( *pCD ); 
				pC->assign_position( x,y,z ); 
			}
			x += spacing; 
		}
		y += y_offset; 
		n++; 
		
		x = Xmin+cell_radius;
		if( n % 2 == 1 )
		{ x += half_space; }
	}
	return; 
}

void fill_annulus( std::vector<double> center , double outer_radius , double inner_radius, Cell_Definition* pCD )
{ return fill_annulus( center,outer_radius,inner_radius,pCD,1.0); } 

void fill_annulus( std::vector<double> center , double outer_radius , double inner_radius, int cell_type , double compression )
{ return fill_annulus( center,outer_radius,inner_radius,find_cell_definition(cell_type),1.0); } 

void fill_annulus( std::vector<double> center , double outer_radius , double inner_radius, int cell_type ) 
{ return fill_annulus( center,outer_radius,inner_radius,find_cell_definition(cell_type),1.0); } 

// draw lines 

void draw_line( std::vector<double> start , std::vector<double> end , Cell_Definition* pCD , double compression )
{
	double cell_radius = pCD->phenotype.geometry.radius; 
	double cr2 = cell_radius * cell_radius; 
	double spacing = compression * cell_radius * 2.0; 
	
	std::vector<double> position = start; 
	
	std::vector<double> displacement = end-position; 
	
	// get direction 
	std::vector<double> increment = displacement; 
	normalize( &increment ); // unit vector in correct direction along the line 
	increment *= spacing; // now it's the correct "delta" between cells along the line   
	
	double d2 = norm_squared( displacement ); 
	
	while( d2 > cr2 )
	{
		Cell* pC = create_cell( *pCD ); 
		pC->assign_position( position ); 
		
		position += increment; 
		displacement = end-position; 
		d2 = norm_squared( displacement ); 
	}
	return; 
}

void draw_line( std::vector<double> start , std::vector<double> end , Cell_Definition* pCD )
{ return draw_line(start,end,pCD,1.0); }

void draw_line( std::vector<double> start , std::vector<double> end , int cell_type , double compression )
{ return draw_line(start,end,find_cell_definition(cell_type),compression); }

void load_cells_csv_v1( std::string filename )
{
	std::ifstream file( filename, std::ios::in );
	if( !file )
	{ 
		std::cout << "Error: " << filename << " not found during cell loading. Quitting." << std::endl; 
		exit(-1);
	}
	else
	{
		std::cout << "Loading cells from simple (v1) CSV file " << filename << " ... " << std::endl; 
	}

	std::string line;
	while (std::getline(file, line))
	{
		std::vector<double> data;
		csv_to_vector( line.c_str() , data ); 

		if( data.size() != 4 )
		{
			std::cout << "Error! Importing cells from a CSV file expects each row to be x,y,z,typeID." << std::endl;
			exit(-1);
		}

		std::vector<double> position = { data[0] , data[1] , data[2] };

		int my_type = (int) data[3]; 
		Cell_Definition* pCD = find_cell_definition( my_type );
		if( pCD != NULL )
		{
			std::cout << "Creating " << pCD->name << " (type=" << pCD->type << ") at " 
			<< position << std::endl; 
			Cell* pCell = create_cell( *pCD ); 
			pCell->assign_position( position ); 
		}
		else
		{
			std::cout << "Warning! No cell definition found for index " << my_type << "!" << std::endl
			<< "\tIgnoring cell in " << filename << " at position " << position << std::endl; 
		}

	}

	file.close(); 	
}

bool load_cells_from_pugixml( pugi::xml_node root )
{
	pugi::xml_node node = root.child( "initial_conditions" ); 
	if( !node )
	{ 
		std::cout << "Warning: XML-based cell positions has wrong formating. Ignoring!" << std::endl; 
		return false;
	}

	node = node.child( "cell_positions" ); 
	if( !node )
	{
		std::cout << "Warning: XML-based cell positions has wrong formating. Ignoring!" << std::endl; 
		 return false;
	}

	// enabled? 
	if( node.attribute("enabled").as_bool() == false )
	{ return false; }

	// get filename 

	std::string folder = xml_get_string_value( node, "folder" ); 
	std::string filename = xml_get_string_value( node, "filename" ); 
	std::string input_filename = folder + "/" + filename; 

	std::string filetype = node.attribute("type").value() ; 

	// what kind? 
	if( filetype == "csv" || filetype == "CSV" )
	{
		std::cout << "Loading cells from CSV file " << input_filename << " ... " << std::endl; 
		load_cells_csv( input_filename );
		system("sleep 1");
	}
	else if( filetype == "matlab" || filetype == "mat" || filetype == "MAT" )
	{
		std::cout << "Error: Load cell positions from matlab not yet supported. Try CSV." << std::endl; 
		exit(-1); 
	}
	else if( filetype == "scene" )
	{
		std::cout << "Error: load cell positions from scene not yet supported. Try CSV." << std::endl; 
		exit(-1); 
	}
	else if( filetype == "physicell" || filetype == "PhysiCell" )
	{
		std::cout << "Error: load cell positions from PhysiCell snapshot not yet supported. Try CSV." << std::endl; 
		exit(-1); 
	}
	else
	{
		std::cout << "Error: Unknown cell position filetype " << filetype << " in XML. Try CSV." << std::endl; 
		exit(-1);
	}

	copy_file_to_output(input_filename);
	return true; 
}

bool load_cells_from_pugixml( void )
{ return load_cells_from_pugixml( physicell_config_root ); }


void set_parameters_from_distributions( const pugi::xml_node root )
{
	// find the start of cell definitions 
	pugi::xml_node node = root.child( "cell_definitions" ); 
	
	// find the first cell definition 
	node = node.child( "cell_definition" ); 

	std::string celltype;
	Cell_Definition *pCD;
	while (node)
	{
		pugi::xml_node node_ipd = node.child("initial_parameter_distributions");
		if (node_ipd && (node_ipd.attribute("enabled").empty() || node_ipd.attribute("enabled").as_bool()))
		{
			celltype = node.attribute("name").as_string();
			pCD = find_cell_definition(celltype);
			set_distributed_parameters(node_ipd, pCD);
		}
		node = node.next_sibling("cell_definition");
	}
	return;
}

void set_distributed_parameters(const pugi::xml_node node_ipd, Cell_Definition *pCD)
{
	pugi::xml_node node_dist = node_ipd.child("distribution");
	while (node_dist)
	{
		if (node_dist.attribute("enabled").empty() || node_dist.attribute("enabled").as_bool()) // if enabled attribute is missing or true, set the distribution (I put this here rather than in the function because the logic is clearer this way without negations)
		{
			set_distributed_parameter(node_dist, pCD);
		}
		node_dist = node_dist.next_sibling("distribution");
	}
	return;
}

void set_distributed_parameter(pugi::xml_node node_dist, Cell_Definition *pCD)
{
	static std::vector<std::string> supported_distributions = {"Uniform","LogUniform","Normal","LogNormal","Log10Normal"};
	std::string type = node_dist.attribute("type").as_string();
	std::string behavior = xml_get_string_value(node_dist, "behavior");
	if (!is_in(type, supported_distributions))
	{
		std::cout << "ERROR: Only supporting these distributions:" << std::endl
				  << "\t\t" << supported_distributions << std::endl
				  << "\tBut " << behavior << " for " << pCD->name << " using " << type << "." << std::endl;
		exit(-1);
	}

	if (!strcmpi(behavior,"volume") && find_behavior_index(behavior) == -1)
	{
		std::cout << "ERROR: Initial parameter distributions can only be set for volume and cell behaviors." << std::endl
				  << "\t" << behavior << " is not among these." << std::endl;
		exit(-1);
	}
	set_distributed_parameter(pCD, behavior, type, node_dist);
	return;
}

bool is_in(const std::string x, const std::vector<std::string> A)
{
	// checks if x is in A
	for (unsigned int i = 0; i < A.size(); i++)
	{
		if (strcmpi(x, A[i]))
		{ return true; }
	}
	return false;
}

void set_distributed_parameter(Cell_Definition *pCD, std::string behavior, std::string type, pugi::xml_node node_dist)
{
	double base_value;
	if (strcmpi(behavior, "volume"))
	{
		base_value = pCD->phenotype.volume.total;
	}
	else
	{
		base_value = get_single_base_behavior(pCD, behavior);
	}
	bool check_base = node_dist.attribute("check_base").empty() || node_dist.attribute("check_base").as_bool(); // if check_base not provided, default to true
	if (strcmpi(type,"uniform"))
	{
		double min = xml_get_double_value(node_dist, "min");
		double max = xml_get_double_value(node_dist, "max");
		if (check_base && (base_value < min || base_value > max))
		{
			std::cout << "ERROR: The base value for " << behavior << " in " << pCD->name << " is " << base_value << std::endl
					  << "\tThis value is outside the range of the uniform distribution." << std::endl
					  << "\tmin = " << min << ", max = " << max << "." << std::endl
					  << "\tIf you want to allow the base value to be outside the range, set check_base to false." << std::endl;
			exit(-1);
		}
		double dv = max - min;
		if (dv < 0)
		{
			std::cout << "ERROR: The min and max values for " << behavior << " in " << pCD->name << " do not satisfy min <= max." << std::endl
					  << "\tmin = " << min << ", max = " << max << std::endl;
			exit(-1);
		}
		for (unsigned int i = 0; i < (*all_cells).size(); i++)
		{
			if ((*all_cells)[i]->type_name != pCD->name)
			{
				continue;
			}
			double val = min + dv * UniformRandom();
			set_distributed_parameter((*all_cells)[i], behavior, val);
		}
	}
	else if (strcmpi(type,"loguniform"))
	{
		double min = xml_get_double_value(node_dist, "min");
		if (min <= 0)
		{
			std::cout << "ERROR: The log uniform distirbution must be defined on a positive interval." << std::endl
					  << "\tThe min value for " << behavior << " in " << pCD->name << " is " << min << std::endl
					  << "\tSet the min and max as the bounds on the value you want, not the bounds on the exponent." << std::endl
					  << "\tFor example, if you want a value between 0.1 and 10, set min=0.1 and max=10, not min=-1 and max=1." << std::endl;
			exit(-1);
		}
		double max = xml_get_double_value(node_dist, "max");
		if (check_base && (base_value < min || base_value > max))
		{
			std::cout << "ERROR: The base value for " << behavior << " in " << pCD->name << " is " << base_value << std::endl
					  << "\tThis value is outside the range of the loguniform distribution." << std::endl
					  << "\tmin = " << min << ", max = " << max << "." << std::endl
					  << "\tIf you want to allow the base value to be outside the range, set check_base to false." << std::endl;
			exit(-1);
		}
		min = log(min);
		max = log(max);
		double dv = max - min;
		if (dv < 0)
		{
			std::cout << "ERROR: The min and max values for " << behavior << " in " << pCD->name << " do not satisfy min <= max." << std::endl
					  << "\tmin = " << min << ", max = " << max << std::endl;
			exit(-1);
		}
		for (unsigned int i = 0; i < (*all_cells).size(); i++)
		{
			if ((*all_cells)[i]->type_name != pCD->name)
			{
				continue;
			}
			double val = exp(min + dv * UniformRandom());
			set_distributed_parameter((*all_cells)[i], behavior, val);
		}
	}
	else if (strcmpi(type,"normal"))
	{
		double mu = xml_get_double_value(node_dist, "mu");
		double sigma = xml_get_double_value(node_dist, "sigma");
		double lb = -9e99;
		double ub = 9e99;
		if (node_dist.child("lower_bound"))
		{ lb = xml_get_double_value(node_dist, "lower_bound"); }
		if (node_dist.child("upper_bound"))
		{ ub = xml_get_double_value(node_dist, "upper_bound"); }
		if (lb > ub)
		{
			std::cout << "ERROR: The lower and upper bounds for " << behavior << " in " << pCD->name << " do not satisfy lb <= ub." << std::endl
					  << "\tlb = " << lb << ", ub = " << ub << std::endl;
			exit(-1);
		}
		if (check_base && (base_value < lb || base_value > ub))
		{
			std::cout << "ERROR: The base value for " << behavior << " in " << pCD->name << " is " << base_value << std::endl
					  << "\tThis value is outside the range of the truncated normal distribution." << std::endl
					  << "\tExpecting values between " << lb << " and " << ub << "." << std::endl
					  << "\tIf you want to allow the base value to be outside the range, set check_base to false." << std::endl;
			exit(-1);
		}
		print_drawing_expectations(mu, sigma, lb, ub, (*all_cells).size());
		for (unsigned int i = 0; i < (*all_cells).size(); i++)
		{
			if ((*all_cells)[i]->type_name != pCD->name)
			{
				continue;
			}
			double val=lb;
			while (val<=lb || val>=ub)
			{ val = NormalRandom(mu, sigma); }
			set_distributed_parameter((*all_cells)[i], behavior, val);
		}
	}
	else if (strcmpi(type,"lognormal"))
	{
		double mu = xml_get_double_value(node_dist, "mu");
		double sigma = xml_get_double_value(node_dist, "sigma");
		double lb = 0;
		double ub = 9e99;
		get_log_normal_bounds(node_dist, behavior, pCD, lb, ub, base_value, check_base);
		print_drawing_expectations(mu, sigma, log(lb), log(ub), (*all_cells).size());
		for (unsigned int i = 0; i < (*all_cells).size(); i++)
		{
			if ((*all_cells)[i]->type_name != pCD->name)
			{
				continue;
			}
			double val=lb;
			while (val<=lb || val>=ub)
			{ val = exp(NormalRandom(mu, sigma)); }
			set_distributed_parameter((*all_cells)[i], behavior, val);
		}
	}
	else if (strcmpi(type,"log10normal"))
	{
		static double log10_ = log(10.0);
		double mu = xml_get_double_value(node_dist, "mu");
		double sigma = xml_get_double_value(node_dist, "sigma");
		double lb = -9e99;
		double ub = 9e99;
		get_log_normal_bounds(node_dist, behavior, pCD, lb, ub, base_value, check_base);
		print_drawing_expectations(mu, sigma, log(lb), log(ub), (*all_cells).size());
		for (unsigned int i = 0; i < (*all_cells).size(); i++)
		{
			if ((*all_cells)[i]->type_name != pCD->name)
			{
				continue;
			}
			double val=lb;
			while (val<=lb || val>=ub)
			{ val = exp(log10_ * NormalRandom(mu, sigma)); }
			set_distributed_parameter((*all_cells)[i], behavior, val);
		}
	}
	return;
}

void get_log_normal_bounds(pugi::xml_node node_dist, std::string behavior, Cell_Definition *pCD, double &lb, double &ub, double base_value, bool check_base)
{
	if (node_dist.child("lower_bound"))
	{
		lb = xml_get_double_value(node_dist, "lower_bound");
		if (lb < 0)
		{
			std::cout << "ERROR: The lower bound for a lognormal/log10normal distribution only matters if it is non-negative." << std::endl
					  << "\tThe lower bound for " << behavior << " in " << pCD->name << " is " << lb << "." << std::endl
					  << "\tThe lower bound is for the actual assigned value while the mean and standard deviation are for the log/log10 of the value." << std::endl
					  << "\tSince this seems to imply (understandable!) confusion about the lognormal/log10normal distribution, I'm going to exit." << std::endl;
			exit(-1);
		}
	}
	if (node_dist.child("upper_bound"))
	{
		ub = xml_get_double_value(node_dist, "upper_bound");
	}
	if (lb > ub)
	{
		std::cout << "ERROR: The lower and upper bounds for " << behavior << " in " << pCD->name << " do not satisfy lb <= ub." << std::endl
				  << "\tlb = " << lb << ", ub = " << ub << std::endl;
		exit(-1);
	}
	if (check_base && (base_value < lb || base_value > ub))
	{
		std::cout << "ERROR: The base value for " << behavior << " in " << pCD->name << " is " << base_value << std::endl
				  << "\tThis value is outside the range of the lognormal/log10normal distribution." << std::endl
				  << "\tExpecting values between " << lb << " and " << ub << "." << std::endl
				  << "\tIf you want to allow the base value to be outside the range, set check_base to false." << std::endl;
		exit(-1);
	}
}

void print_drawing_expectations(double mu, double sigma, double lb, double ub, int n)
{
	// calculate the z-scores for lb and ub
    double z_lb = (lb - mu) / sigma;
    double z_ub = (ub - mu) / sigma;

    // calculate the probabilities for lb and ub
    double p_lb = 0.5 * (1 + std::erf(z_lb / std::sqrt(2)));
    double p_ub = 0.5 * (1 + std::erf(z_ub / std::sqrt(2)));

    // the probability of finding a value between lb and ub is the difference between the probabilities for ub and lb
    double success_probability = p_ub - p_lb;
	double num_expected = n / success_probability;
	
	std::cout << "Drawing up to " << n << " values from a normal distribution with mu=" << mu << " and sigma=" << sigma << std::endl
			  << "\tExpecting values between " << lb << " and " << ub << std::endl
			  << "\tIt will take about " << num_expected << " draws to get " << n << " values in the range." << std::endl
			  << "\tIf one draw takes 1 microsecond, this will take about " << num_expected * 1e-6 / 60 << " minutes." << std::endl;
}

void set_distributed_parameter(Cell* pCell, std::string behavior, double val)
{
	if (strcmpi(behavior, "volume"))
	{
		pCell->set_total_volume(val);
	}
	else
	{
		set_single_behavior(pCell, behavior, val);
	}
}

bool strcmpi(std::string x, std::string y)
{
	// case-Insensitive compare strings
	for (auto& a : x) {
        a = tolower(a);
    }
	for (auto& a : y) {
        a = tolower(a);
    }
	return x==y;
}

void set_parameters_from_distributions( void )
{ return set_parameters_from_distributions( physicell_config_root ); }

std::vector<std::string> split_csv_labels( std::string labels_line )
{
	std::vector< std::string > label_tokens; 
	std::string s; 

	std::stringstream stream(labels_line); 
	while( std::getline( stream , s , ',' ) )
	{ label_tokens.push_back(s); }

	return label_tokens;
}

Cell* process_csv_v2_line( std::string line , std::vector<std::string> labels )
{
	// split the line into tokens 
	std::vector< std::string > tokens; 

	std::stringstream stream(line); 
	std::string s; 
	while( std::getline( stream , s , ',' ) )
	{ tokens.push_back(s); }

	// get the cell position 
	std::vector<double> position; 
	char* pTemp;
	for( int i=0; i < 3 ; i++ )
	{ position.push_back( strtod( tokens[i].c_str() , &pTemp ) ); } 

	// the cell type 
	std::string celltype = tokens[3]; 
	Cell_Definition* pCD = find_cell_definition( celltype ); 
	if( pCD == NULL )
	{
		std::cout << "Warning! CSV file requests creating cell type " << celltype << std::endl 
			<< "\tat " << position << "but I don't recognize that type. Skipping cell!" << std::endl << std::endl; 
		return NULL;
	}

	// create the cell IF the definition was found 
	std::cout << "Creating " << pCD->name << " (type=" << pCD->type << ") at " 
		<< position << std::endl; 

	Cell* pCell = create_cell( *pCD ); 
	pCell->assign_position( position ); 

	// now write any extra data 

	for( int k=4 ; k < tokens.size(); k++ )
	{
		double dval = strtod(tokens[k].c_str() , &pTemp ); 
		bool processed = false; 
		bool skip = false; 


		// if the string is empty, skip 
		if( tokens[k].size() == 0 )
		{ skip = true; }
		else
		{
			char c = tokens[k].c_str()[0]; 
			// use 's' or 'S' to skip the entry 
			if( c == 's' || c == 'S' )
			{ skip = true; }
		}

		// special cases: 

			// volume 
		if( labels[k] == "volume" && skip == false )
		{ 
			pCell->set_total_volume( dval ); 
			processed = true; 
		}

		// check behavior dictionary 

		if( processed == false && skip == false )
		{
			// if the behavior is found in the dictionary, process it 
			if( find_behavior_index( labels[k] ) > -1 )
			{
				set_single_behavior( pCell , labels[k] , dval ); 
				processed = true; 
			}
		}

		// warning message for any unprocessed variables 
		if( processed == false && skip == false )
		{
			std::cout << "\tWarning: I don't know how to process " << labels[k] 
			<< " so I skipped it." << std::endl;
		}
		// give a notation for any intentinoally skipped variables 
		if( skip == true )
		{
			std::cout << "\tNote: Skipping " << labels[k] 
			<< " for this cell." << std::endl;
		}


	}

	return pCell;  
}

void load_cells_csv_v2( std::string filename )
{
	// open file 
	std::ifstream file( filename, std::ios::in );
	if( !file )
	{ 
		std::cout << "Error: " << filename << " not found during cell loading. Quitting." << std::endl; 
		exit(-1);
	}
	else
	{
		std::cout << "Loading cells from detailed (v2) CSV file " << filename << " ... " << std::endl; 
	}

	// get the first line (labels)

	std::string line; 
	std::getline( file , line ); 

	// tokenize the labels 

	std::vector<std::string> labels = split_csv_labels( line ); 

	// process all remaining lines 

	while (std::getline(file, line))
	{ process_csv_v2_line(line,labels); }	

	// close the file 

	file.close(); 
	std::cout << "Done! " << std::endl << std::endl; 

	return; 
}

void load_cells_csv( std::string filename )
{
	// open file 
	std::ifstream file( filename, std::ios::in );
	if( !file )
	{ 
		std::cout << "Error: " << filename << " not found during cell loading. Quitting." << std::endl; 
		exit(-1);
	}

	// determine version 
	std::string line; 
	std::getline( file , line );
	char c = line.c_str()[0]; 

	file.close(); 

	if( c == 'X' || c == 'x' )
	{ 
		// v2
		return load_cells_csv_v2( filename ); 
	}
	else
	{
		// v1
		return load_cells_csv_v1( filename ); 
	}

	return; 
}


}; 
