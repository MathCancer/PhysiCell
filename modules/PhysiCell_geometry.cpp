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
