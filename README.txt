PhysiCell: PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D 
Multicellular Systems.

Version:      1.2.1
Release date: 1 September 2017

Homepage:     http://PhysiCell.MathCancer.org
Downloads:    http://PhysiCell.sf.net

Summary: 
This release introduces a improved MultiCellDS outputs, new matlab 
functions to read the MultiCellDS outputs, experimental functions 
for povray routines (for 3-D raytracing), some bugfixes, and 
new sample projects in cancer heterogeneity, synthetic multicellular 
bioengineering, and cancer immunology. 

PhysiCell is currently under scientific peer review. 

Major new features and changes:
+ Updated save_PhysiCell_to_MultiCellDS_xml_pugi() to save much more 
  phenotype information and all custom variables for each cell. 

+ Updated read_MultiCellDS_XML.m (in ./matlab) to read these 
  newly expanded data files. 

+ Included new matlab functions for fast 3-D data exploration: 

  simple_plot.m  -- quickly plots (as a surface) all the cells 
  simple_cutaway_plot.m -- same as above, but with a cutaway view
  composite_cutaway_plot.m -- plots cutaway view of live and dead
     cells, colored separately. 

+ Wrote new POV-ray functions for future raytracing utilities. See 
  ./modules/PhysiCell_POV.*

+ Add the following new sample projects: 

  biorobots: simulates a system of director, worker, and cargo 
   cells that coordinate to deliver cellular cargo to the 
   directors. Uses motility, custom mechanics, and other functions. 
  
       build via: make biorobots-sample && make project 

  cancer biorobots: modifies the above project so that worker 
    cells take cargo to hypoxic tumor regions. cargo cells 
    release a drug once they are detached from worker cells. 
    tumor cells have a damage and apoptosis model. 

       build via: make cancer-biorobots-sample && make project 

  heterogeneity: simulates proliferation of a heterogeneous 
    tumor. Note the loss of symmetry and selection/evolution.  

       build via: make heterogeneity-sample && make project 

  cancer immunology: simulates an immune system attack on a 
    tumor with heterogeneous proliferation and immunogenicity 
    characteristics. In 3D!  

       build via: make cancer-immune-sample && make project 

Minor new features and changes:

+ Added double NormalRandom( double mean, double standard_deviation ) 
  to PhysiCell_utilities.* This will generate normally-distributed 
  random numbers. 

+ Includes a sneak preview of BioFVM 1.1.7, which includes bugfixes on 
  how boundary conditions were initialized. 

bugfixes: 

+ Removed annoying "cout" lines in:

  void standard_live_phase_entry_function(Cell*,Phenotype&,double). 

  They were leftover from debugging. Sorry! 

+ In the templates, we updated to: 

		while( t < t_max + 0.1*diffusion_dt )
		{

  so that the simulations run all the way to the final time, 
  instead of exiting too early by dt time. 

+ Fixed a bug in Motility::Motility() where the migration bias was 
  initialized to (1,0,0). 

+ Made adhesion and repulsion symmetric across cells (in add_potentials)

+ got rid of debug_temp_a in  (in add_potentials)

+ if default_microenvironment_options.simulate_2D == true, we now 
  set velocity[2] = 0.0 in the velocity functions. 

-=-=-

Version:      1.2.0
Release date: 30 July 2017

Homepage:     http://PhysiCell.MathCancer.org
Downloads:    http://PhysiCell.sf.net

Summary: 
This release introduces a major reworking to simplify cell pheno-
types and cells. It includes a generalized cell cycle model that 
can also be used to represent death cycle models. It includes an 
improved motility representation. Changing cell secretion and 
uptake rates is more straightforward, because all interfacing 
with BioFVM is automated. 

A new, comprehensive user guide is included on all the major 
functions and classes in PhysiCell, including examples. 

This release also includes bugfixes and corrections. Due 
to the extensive redevelopment, this change log is not 100% 
fine-grained. 

PhysiCell is currently under scientific peer review. 

Major new features and changes:
+ Completely reworked the Phenoytype class to be much more 
  straightforward, and safe to change throughout the 
  simulation.
  
+ The Phenotype class includes elements on cell cycling, death, 
  volume, geometry, mechanics, motility, and secretion (and uptake). 
  
+ Completely reworked representation of cell cycle models as 
  Cycle_Model: a directed graph of Phases, linked by Phase_Links. 
  A Cycle_Model is bundled with key parameters (Cycle_Data) into 
  the Cycle element of the Phenotype.  

+ Cycle models can include user-specified arrest functions between 
  Phases (e.g., contact inhibition). 

+ Cycle Phases can have user-specified "entry" and "exit" functions 
  to be executed upon entering or leaving a Phase (e.g., mutations). 
 
+ Created new classes Death_Parameters and Death to represent 
  cell death models as Cycle_Model. 
  
+ Updated the classes for Mechanics, Volume, and Geometry. 
 
+ Completely reworked the Motility class to represent cell 
  migration as a combination of random and directed motility. 
  Notions of persistence time are included. Users can supply 
  custom functions to update the cell's migration bias. 
  
+ Added the cell calcification model to the general volume update 
  function. 
  
+ Deprecated all code in core/PhysiCell_digital_cell_line.*
  
+ Added apoptosis_death_model, necrosis_death_model,  
  autophagy_death_model, and live_cells_cycle_model to 
  PhysiCell_Constants.
  
+ Added new function Cell::flag_for_division() for simpler 
  construction of cycle models, and to allow users to push a cell 
  to divide at any time. 
  
+ Added new function Cell::flag_for_removal() for simpler 
  construction of cycle models, and to allow users to push a cell 
  to be removed at any time. 
  
+ Volume::multiply_by_ratio() now also divides the target volumes by 
  this ratio. 
  
+ Created a new Cell_Functions class to more readily bundle the 
  Cell object's function pointers. 
  
+ Rewrote the Custom_Cell_Data class, so that custom data are no 
  longer hard-coded by the user in the class, but instead added 
  during runtime. Moved PhysiCell_custom.h/cpp into the core 
  directory. 
  
+ The Cell_Definition class includes a pointer to the default 
  Microenvironment, as well as a default Cell_Functions, 
  Cell_Parameters, Custom_Cell_Data, and Phenotype. Newly created 
  cells all copy from the Cell_Defaults. 
  
+ Bundled the following phenotype update functions into a single 
  Phenotype::advance_all_nonmechanics_models( Cell* , double ) 
  function: 
    Phenotype::update_function() // update cell phenotype parameters
    Phenotype::volume.update() // update cell volume 
    Phenotype::geometry.update() // update radius, etc. 
    Phenotype::death.check_for_death() // check for death
    Phenotype::cycle.advance_cycle() // advance the current cycle model 
	
+ PhysiCell_Container uses the above function on each cell to 
  ensure that the correct custom functions are used. It also uses 
  the cell's Cell::Cell_Functions::update_velocity() to update 
  the cell's state. For now, users need to remember to include 
  the motility update functions in their update_velocity() function 
  if they replace the default. Lastly, PhysiCell_Container calls teh 
  Cell::Cell_Functions::custom_cell_rule at the end of the main loop.  
  
+ Created new functions to directly trigger cell death. 

+ Created new functions to access cell neighbor information. 

+ Updated the examples for compatibility. 

Minor new features and changes: 
+ Added PhysiCell_constants::custom_phase to the constants. 

+ Changed Cell:is_movable to Cell_is_movable to better express its 
  purpose of setting a cell to rigid (unmovable) or movable. 

+ Fixed Cell:turn_off_reactions(double) to completely turn off all 
  BioFVM uptake/secretion, rather than cut them by an order of 
  magnitude. 
  
+ Replaced scores of (if some_double == 0) conditionals with 
  if( fabs( some_double ) < tolerance ), since you it is best practice 
  to never check for equality of a floating point or double. 
  
+ Started substituting more efficient axpy and += operators 
  (from BioFVM) for many of the vector operations in PhysiCell_cell.cpp. 
  
+ Where possible, inserted { } into if / then / else logic for 
  increased code clarity and robustness to newline and whitespace 
  changes. 
  
+ Moved most default / standard mechanics functions from PhysiCell_cell.* to
  PhysiCell_standard_models.*. 
  
+ Added constants for time step sizes, time units, and spatial units to 
  PhysiCell_constants.h, and removed isolated (and sometimes non-
  synchronized) timestep constants throughout the code. 

+ Added State::simple_pressure for future use in mechanics-induced cycle 
  regulation. 
  
+ Added std::vector<Cell*>& Cell:cells_in_my_container(void) to more 
  easily access a list of cells in the cell's current mechanics voxel. 
  
+ and more. 

+ Simplified the Makefile for easier cross-platform compiling. Users 
  do not need to change MARCH any more. 
 
Bugfixes and Corrections: 

+ Fixed typo "max_cell_interactive_ditstance_in_voxel" to 
  "max_cell_interactive_ditstance_in_voxel" in the Cell_Container class. 

+ Throughout the code, replaced any logic of the form 
  (if some_double == some_other_double ) with better practice of 
  if( fabs( some_double - some_other_double ) < tolerance ), since 
  floating point numbers aren't often equal. 

+ In PhysiCell_cell_container, the function find_escaping_face_index() 
  had a return type of int, but it was possible for none of the statements 
  to evaulate "true". The function now returns -1 for the case that 
  there is no escaping_face_index. Thus, the return type is always defined. 

+ Cell::assign_position() now sets: 
    is_out_of_domain = true; 
    is_active = false; 
    is_movable = false; 
  if the cell is out of bounds. This should prevent segfaults when cells 
  are assigned positions out of bounds (e.g., during division). 
  
+ Cell::update_position() now sets: 
    is_out_of_domain = true; 
    is_active = false; 
    is_movable = false;
  if the cell moves out of bounds. This should prevent segfaults when cells 
  move out of bounds (e.g., by mechanics). 

----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- 
----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- ----- 

Version:      1.1.1
Release date: 16 May 2017

Homepage:     http://PhysiCell.MathCancer.org
Downloads:    http://PhysiCell.sf.net

Summary: 
This release includes usability fixes and enhancements, including  
more basic "template" projects, simpler project startup, improved SVG 
visualization support, and cleanup on the Makefile. 

PhysiCell is currently under scientific peer review. 

Major new features and changes:
+ Added template3D.cpp and template2D.cpp template projects. See the 
  template_projects directory. 
	
+ Added Makefile rules to seed the 2D projects. To create and compile 
  the 2D template:
		make template2D && make
	To create and compile the 3D template:
		make template3D && make
		
	To further edit your project, modify main.cpp in the root PhysiCell 
	directory. Follow the online tutorials for further functionality. 

+ Added preliminary MultiCellDS support, with MultiCellDS outputs. These 
  are added via modules/PhysiCell_MultiCellDS.cpp

+ Many usability improvements listed in "minor new features" below. 

+ Finished implementation of SVG support, to plot the simulation through a 
  cross-section (fixed z-value). 
  
+ Digital pathology: Improved coloring functions for the main cell cycle 
  models, and virtual H&E (hematoxylin and eosin): 

Minor new features and changes: 
+ Created new function in PhysiCell_cell_container to simplify initialization 
  of the mechanics data structures:

  Cell_Container* create_cell_container_for_microenvironment( 
    BioFVM::Microenvironment& m , double mechanics_voxel_size );

+ Usability feature: If BioFVM::default_microenvironment has not yet been 
  declared when calling create_cell_container_for_microenvironment(), then 
	it is set to "m" in the new function above. 
		
+ Usability feature: If the BioFVM::default_microenvironment has been set
  then Cell* create_cell( void ) now uses this to call 
	Cell::register_microenvironment(&microenvironment). 
	
+ Changed Cell_Parameters from a struct to a class. 
	
+ Usability feature: Created a new Cell_Defaults class, with a 
  global PhysiCell::cell_defaults. Now, you can set these 
  default functions and parameters at the start of your program, and 
  all new cells are set to use these defaults. 
	
+ Usability feature: Traced code and determined that calling Cell::set_phenotype() 
  calls Basic_Agent::set_total_volume() (Cell extends Basic_Agent), which sets 
  Basic_Agent::volume_is_changed to true. This, in turn, makes the next call 
  to Basic_Agent::simulate_secretion_and_uptake() call  
  Basic_Agent::set_internal_uptake_constants(). So, it is unnecessary to 
  call this function in typical initialization. 
	
+ Usability feature: Cell:set_phenotype() now automatically calls  
	Basic_Agent::set_internal_uptake_constants(). You no longer need to 
	manually call this function *if* using the set_phenotype() function. 
	
+ Usability feature: The default Cell constructor (Cell::Cell) uses 
  the default functions in PhysiCell::default_cell_functions, instead 
  of hard-coded defaults. The default constructor for Default_Cell_Functions
  has sensible defaults for cell mechanics and volume regulation to 
  match the PhysiCell method paper. Phenotype-related functions are left 
  empty. 
	
+ Usability feature: The create_cell() function now assigns the default 
  microenvironment to the newly created cell, and assigns the 
  cell functions in PhysiCell::default_cell_functions. 

+ Changed default -march flag to -march=native, according to benchmarks 
  on gcc 5x at phoronix: 
  http://www.phoronix.com/scan.php?page=news_item&px=GCC-Optimizations-E3V5-Levels
  
+ Changed the -O3 flag to -Ofast, which tends to produce slightly faster 
  code by the phoronix link above. 
  
+ Updated to a pre-release copy of BioFVM 1.1.6. 

+ Included the matlab functions first created for BioFVM, which can now be 
  found in the matlab directory. 

+ Fixed read_MultiCellDS_xml.m function to work when there are no cells.

+ Fixed read_MultiCellDS_xml.m function to display the current number of 
  cells, when there are < 3 cells.
  
+ Added modules/PhysiCell_standard_modules.h to start organizing non-core, 
  standard parts of MultiCellDS. This will inintially include SVG, 
  pathology, and MultiCellDS modules. 
  
+ Removed matlab output from log_output in PhysiCell_utilities.cpp. This 
  only saved the microenvironment (but not cells), and it is no longer 
  needed with new MultiCellDS output support.   

+ Changed the default SVG length scale bar in PhysiCell_pathology to 
  100 microns (previously 1000 microns). 
  
+ Updated the archive rules to use the more-common "tar" command. Use 
  "make tar" and "make untar". Archives are stored in the archives 
  directory. 
	
+ Added a void up_orientation( Cell* pCell, double dt ) to 
  PhysiCell_standard_models.cpp, which sets orientation = [0,0,1]
  and polarity = 1.0. This is useful for 2-D simulations. 

+ Digital Pathology coloring functions: 
    simple_cell_coloring: cell nucleus is blue, cytoplasm is red, and 
      outlines are black

    false_cell_coloring_Ki67: for any Ki67-based cell cycle model, 
      green cells are Ki67+ prior to mitosis (or any Ki67+ cell in 
      the simplified Ki67 models), magenta cells are Ki67+ after 
      mitosis, red cells are apoptotic, and brown cells are necrotic. 

    hematoxylin_and_eosin_cell_coloring: "stains" nuclear solids with 
      hematoxylin, "stains" cytoplasmic solids with eosin, and simulates
      light transmission / absorbtion through a thin slice to approximate 
      microscopy and image acquisition. Note that cells with little water 
      will appear dark (e.g., apoptotic debris, especially after the 
      cytoplasm has blebbed), and cells with lots of water (e.g., onsosis 
      in early necrotic cells) will appear faint.  
   
    false_cell_coloring_live_dead: live cells are green, apoptotic cells 
      are red, and necrotic cells are brown. 

+ Added Phenotype::get_current_phase_code(void) to 
  PhysiCell_digital_cell_line.cpp to more easily get the cell's current 
  phenotypic state. (Especially useful for virtual pathology.) 
	
Bugfixes and Corrections: 
+ Fixed typo Time_Settings.cell_cylce_dt_default to 
  Time_Settings.cell_cycle_dt_default in PhysiCell_cell_container.

+ Removed unused declaration Cell::initialize_functions( void );

+ Changed the default "update_cell_and_death_parameters" function from 
  "update_cell_and_death_parameters_O2_based" to "empty_function". 
  Examples and models should choose this *explicitly*. 

+ Cell::set_orientation( Cell* pCell ) changed to   
  Cell::set_orientation( Cell* pCell, double dt ) to be consistent with other 
  cell member functions. 
	
+ In void Cell::assign_orientation(), set polarity = 0.0 if a  
  set_orientation(Cell*,double) function is not set (NULL).
	
+ Removed irrelevant data elements in the Custom_Cell_Data class. 
	
Notices for intended changes that may affect backwards compatibility: 

+ Will rename the current "Phenotype" class to "Old_Phenotype"

+ Will introduce a new Phenotype class with a much simpler structure. 

+ Will rewrite the represetation of cell cycle and death phases. 

+ Will rewrite the standard phenotype models in the simpler representatio. 

+ Will stop requiring use of the Digital_Cell_Line class for initializing 
  simulations. 

+ Will deprecate update_cell_and_death_parameters, and instead use 
  update_phenotype_parameters. 
  
+ Will stop using the oxygen-dependent phenotype rule as default. 

PhysiCell 1.1.1 includes an advance copy of BioFVM 1.1.6. Here are the changes: 
	
/* fixes and changes in BioFVM 1.1.6  */

+ correct typos in citation information in all source files 

+ updated citation information 

+ added void set_default_microenvironment( Microenvironment* M ) declaration to 
  BioFVM_Microenvironment.h
 
+ set volume_is_changed = false in Basic_Agent::set_internal_uptake_constants(); 

+ Set the MultiCellDS options Booleans to extern bool in BioFVM_MultiCellDS.h 
  so that PhysiCell can read these options. 

+ Updated the simlified_data field in MultiCellDS XML output to include a 
  new "source" attribute with value "BioFVM".

+ Added Microenvironment::find_substrate_index( std::string ) to make it 
  easier to find . 
  
+ Added Basic_Agent::nearest_gradient( int substrate_index ) to directly 
  access the gradient of the indicated substrate at the agent's  
  current_voxel_index. 
  
+ Added Basic_Agent::nearest_gradient_vector( void ) to directly 
  access a vector of gradients (one for each substrate in the 
  microenvironment) at the agent's current_voxel_index. 
 
+ Added Microenvironment::is_dirichlet_node( int voxel_index ) to 
  easily check if the voxel is a Dirichlet node. 

+ Updated Microenvironment::update_dirichlet_node() and 
  Microenvironment::remove_dirichlet_node() to check against 
  mesh.voxels[].is_Dirichlet to provide a cheap and early exit 
  if the node isn't Dirichlet in the first place. 
  
+ Changed to a thread-safe data structure for Dirichlet nodes 
  (e.g., if a custom cell function in an OMP loop adds or removes 
  Dirichlet nodes). 
  
+ Added new class Microenvironment_Options, with a default 
  default_microenvironment_options, to simplify Microenvironment
  setup. The defaults are dx = dy = dz = 20 microns, on 1 cubic mm. 
  
+ Added function initialize_microenvironment() to set up the 
  microenvironment using the options in 
  default_microenvironment_options. The code sets oxygen as the 
  default field, with D = 1e5 micron^2/min, and decay rate = 0.1 1/min
  (a 1 mm diffusion length scale). If 
  default_microenvironment_options.outer_Dirichlet_conditions = true, 
  then we set a 38 mmHg condition on the outer edges, corresponding to
  5% oxygenation (physioxic conditions). 
  