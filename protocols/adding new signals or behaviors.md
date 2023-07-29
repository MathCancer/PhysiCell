# Adding new signals and behaviors to the dictionaries (introduced in Version 1.10.0)

## To add a new signal: (in 'PhysiCell_signal_behavior.cpp')

Suppose you want to add a new signal "my signal name" to the dictionary, and that you can access that the cell's location at `pCell->something`

1. Make sure the signal you want to add isn't already there! Take a look at the current dictionaries using: 

	```
	void display_signal_dictionary( void );    
	void display_signal_dictionary_with_synonyms( void );
	```    
	
1. Make sure the signal name you have chosen is unique. (See the prior step.)    

1. Add the signal in `setup_signal_behavior_dictionaries()`: 
 
	1. Search for the function: `void setup_signal_behavior_dictionaries( void )`
	
	1. Go to the end of the "signal" section. Just before these lines: 
    
		```
		/* add new signals above this line */ 
		```

	1. Add the signal like this: 
    
		```
		map_index++; 
		signal_to_int["my signal name"] = map_index; 
		int_to_signal[map_index] = "my signal name"; 
		```
	
	1. Add any synonyms (or common typos) to the dictionary. Append these just below your entry. 
   
		```
		// synonyms 
		signal_to_int["my alternate signal name"] = map_index; 
		```
	
1. Add the signal to function to write a full vector of signals 

	1. Search for `std::vector<double> get_signals( Cell* pCell )`

	1. Go to the end of the signals. Just before these lines: 
		```
		// rescale 
		signals /= signal_scales;     
		```
	
	1. Add code to write your signal at the appropriate index, like this: 
   
		```
		static int my_signal_ind = find_signal_index( "my signal name" ); 
		signals[my_signal_ind] = pCell->something; 
		```
	
        Make SURE that `my_signal_ind` is unique!

1. Add the signal to function to write a single individual signal in `get_single_signal()`: 

	1. Search for `double get_single_signal( Cell* pCell, int index )`

	1. Go to the end of signals. Just before these lines: 
		```
		// unknown after here !

		std::cout << "Warning: Requested unknown signal number " << index << "!" << std::endl
		          << "         Returning 0.0, but you should fix this!" << std::endl << std::endl;     

		```
	
	1. Add code to write your signal at the appropriate index, like this: 
		```
		// my signal name 
		static int my_signal_ind = find_signal_index( "my signal name" ); 
		if( index == time_ind )
		{
	   	   out = pCell->something;      
	   	   out /= signal_scales[index]; 
	   	   return out; 
		} 
		```
		
        Make SURE that `my_signal_ind` is unique!

## To add a new behavior: (in 'PhysiCell_signal_behavior.cpp')

Suppose you want to add a new behavior "my behavior name" to the dictionary, and that you can access that at `pCell->something`

1. Make sure the behavior you want to add isn't already there! Take a look at the current dictionaries using: 

	```
	void display_behavior_dictionary( void );    
	void display_behavior_dictionary_with_synonyms( void );
	```    
	
1. Make sure the behavior name you have chosen is unique. (See the prior step.)    

1. Add the behavior in `setup_signal_behavior_dictionaries()`: 
 
	1. Search for the function: `void setup_signal_behavior_dictionaries( void )`
	
	1. Go to the end of the "behavior" section. Just before these lines: 
    
		```
		/* add new behaviors above this line */

		// resize scales; 
    		signal_scales.resize( int_to_signal.size() , 1.0 );   
		```

	1. Add the behavior like this: 
    
		```
		map_index++; 
		map_name = "my behavior name";
		behavior_to_int[map_name ] = map_index;
		int_to_behavior[map_index] = map_name; 
		```	
	
	1. Add any synonyms (or common typos) to the dictionary. Append these just below your entry. 
   
		```
		// synonyms 
		behavior_to_int["my alternate behavior name"] = map_index; 
		```
	
1. Add the behavior to function to write a full vector of behaviors 

	1. Search for `void set_behaviors( Cell* pCell , std::vector<double> parameters )`

	1. Go to the end the function.  
	
	1. Add code to write your behavior from the appropriate index, like this: 
   
		```
		// apoptosis 
		static int my_behavior_ind = find_behavior_index( "my behavior name"); 
		pCell->something = parameters[my_behavior_ind]; 
		```
	
        Make SURE that `my_behavior_ind` is unique!

1. Add the signal to function to write a single individual behavior in `set_single_behavior()`: 

	1. Search for `void set_single_behavior( Cell* pCell, int index , double parameter )`

	1. Go to the end of the function
	
	1. Add code to write your behavior to the cell from at the appropriate index, like this: 
		```
		// my behavior name 
		static int my_behavior_ind = find_behavior_index( "my behavior name" ); 
		if( index == my_behavior_ind )
		{ pCell->something = parameter; return; } 
		```
		
        Make SURE that `my_behavior_ind` is unique!

1. Add the behavior to function to get a full vector of behaviors from a cell 

	1. Search for `std::vector<double> get_behaviors( Cell* pCell )`

	1. Go to the end the function.  
	
	1. Add code to write your behavior to the appropriate index, like this: 
   
		```
		// my behavior
		static int my_behavior_ind = find_behavior_index("my behavior name"); 
		parameters[my_behavior_ind] = pCell->something; 
		```
	
        Make SURE that `my_behavior_ind` is unique!****

1. Add the behavior to function to get a single behaviors from a cell 

	1. Search for `double get_single_behavior( Cell* pCell , int index )`

	1. Go to the end the function.  
	
	1. Add code to write your behavior to the appropriate index, like this: 
   
		```
		// my behavior
		static int my_behavior_ind = find_behavior_index( "my behavior name"); 
		if( index == my_behavior_ind )
		{ return pCell->something; }
		```
	
        Make SURE that `my_behavior_ind` is unique!****

1. Add the behavior to the function to get a vector of all base behaviors from a cell's cell definition

	1. Search for `std::vector<double> get_base_behaviors( Cell* pCell )`
	
	1. Go to the end of the function. 
	
	3. Add code to write your behavior to the , like this: 

		```
		// my behavior
		static int my_behavior_ind = find_behavior_index("my behavior name"); 
		parameters[my_behavior_ind] = pCD->something; 
		```
	Make sure that `my_behavior_ind` is unique. 
	
	Notice that `pCD` is set to `Cell_Definition` of the cell in `pCell`. 
	
1. Add the behavior to the function to get a single base behavior from a cell's cell definition

	1. Search for `double get_single_base_behavior( Cell* pCell , int index )`
	
	1. Go to the end of the function. 
	
	3. Add code to write your behavior to the , like this: 

		```
		// my behavior
		static int my_behavior_ind = find_behavior_index("my behavior name" ); 
		if( index == my_behavior_ind )
		{ return pCD->something; }
		```
	Make sure that `my_behavior_ind` is unique. 
	
	Notice that `pCD` is set to `Cell_Definition` of the cell in `pCell`. 

1. Add the behavior to the function to get a single base behavior from a cell's cell definition (queried by cell definiton)

	1. Search for `double get_single_base_behavior( Cell_Definition* pCD , int index )`
	
	1. Go to the end of the function. 
	
	3. Add code to write your behavior to the , like this: 

		```
		// my behavior
		static int my_behavior_ind = find_behavior_index("my behavior name" ); 
		if( index == my_behavior_ind )
		{ return pCD->something; }
		```
	Make sure that `my_behavior_ind` is unique. 
