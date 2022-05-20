# Adding new signals and behaviors to the dictionaries (introduced in Version 1.10.0)

## To add a new signal: (in 'PhysiCell_signal_behavior.cpp')

Suppose you want to add a new signal "signal name" to the dictionary, and that you can access that the cell's location at `pCell->something`

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
		behavior_to_int.clear(); 	
		int_to_behavior.clear();   
		```

	1. Add the signal like this: 
    
		```
		map_index++; 
		signal_to_int["signal name"] = map_index; 
		int_to_signal[map_index] = "signal name"; 
		```
	
	1. Add any synonyms (or common typos) to the dictionary. Append these just below your entry. 
   
		```
		// synonyms 
		signal_to_int["alternate signal name"] = map_index; 
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
		static int my_signal_ind = find_signal_index( "signal name" ); 
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
		static int my_signal_ind = find_signal_index( "signal name" ); 
		if( index == time_ind )
		{
	   	   out = pCell->something;      
	   	   out /= signal_scales[index]; 
	   	   return out; 
		} 
		```
		
        Make SURE that `my_signal_ind` is unique!


