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

void load_cells_csv( std::string filename )
{
	std::ifstream file( filename, std::ios::in );
	if( !file )
	{ 
		std::cout << "Error: " << filename << " not found during cell loading. Quitting." << std::endl; 
		exit(-1);
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
		return true; 
	}
	if( filetype == "matlab" || filetype == "mat" || filetype == "MAT" )
	{
		std::cout << "Error: Load cell positions from matlab not yet supported. Try CSV." << std::endl; 
		exit(-1); 
		std::cout << "Loading cells from matlab file " << input_filename << " ... " << std::endl; 
		return false; 
	}
	if( filetype == "scene" )
	{
		std::cout << "Error: load cell positions from scene not yet supported. Try CSV." << std::endl; 
		exit(-1); 
		std::cout << "Loading cells from scene file " << input_filename << " ... " << std::endl; 
		return false; 
	}
	if( filetype == "physicell" || filetype == "PhysiCell" )
	{
		std::cout << "Error: load cell positions from PhysiCell snapshot not yet supported. Try CSV." << std::endl; 
		exit(-1); 
		std::cout << "Loading cells from PhysiCell file " << input_filename << " ... " << std::endl; 
		return false; 
	}

	return false; 
}

bool load_cells_from_pugixml( void )
{ return load_cells_from_pugixml( physicell_config_root ); }

}; 
