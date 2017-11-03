PhysiCell: PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D 
Multicellular Systems.

Version:      1.1.1
Release date: 18 May 2017

Homepage:     http://PhysiCell.MathCancer.org
Downloads:    http://PhysiCell.sf.net

Summary: 
This release includes minor bugfixes and corrections. 

PhysiCell is currently under scientific peer review. 

Major new features and changes:
+ none

Minor new features and changes: 
+ none
 
Bugfixes and Corrections: 
+ Changed "#include "../biofvm/BioFVM_MultiCellDS.h" to 
  #include "../BioFVM/BioFVM_MultiCellDS.h" in modules/PhysiCell_MultiCellDS.h 
  to fix compile errors. (Thanks, Randy Heiland!)
  
+ Replaced instances of "kid" to the more customary "child" (parent and child 
  objects) in core/PhysiCell_cell.cpp
  
+ Modified the "make clean" rule in the Makefile to work better across 
  platforms. (Previously worked fine on the object files, but only deleted 
  $(PROGRAM_NAME).exe on Windows platforms. 
  
+ Modified the "template2D" and "template3D" rules in the Makefile to 
  copy the current Makefile to template_projects/Makefile.backup  
  
+ Corrected the comments in template2D.cpp and template3D.cpp to more 
  accurately note that we are seeding the three initial cells in all the 
  states of the Ki67 advanced model. (Ki67+ premitotic, Ki67+ postmitotic, 
  and Ki67-).   

Notices for intended changes that may affect backwards compatibility: 

+ Will rename the current "Phenotype" class to "Full_Phenotype"

+ Will introduce a new Phenotype class with a much simpler structure. 

+ Will rewrite the represetation of cell cycle and death phases. 

+ Will rewrite the standard phenotype models in the simpler 
  representation. 

+ Will stop requiring use of the Digital_Cell_Line class for initializing 
  simulations. 

+ Will deprecate update_cell_and_death_parameters, and instead use 
  update_phenotype_parameters. 
  
+ Will stop using the oxygen-dependent phenotype rule as default. 

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
  
+ Updated to a pre-release copy of BioFVM 1.1.5. 

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

+ Will rename the current "Phenotype" class to "Full_Phenotype"

+ Will introduce a new Phenotype class with a much simpler structure. 

+ Will rewrite the represetation of cell cycle and death phases. 

+ Will rewrite the standard phenotype models in the simpler representatio. 

+ Will stop requiring use of the Digital_Cell_Line class for initializing 
  simulations. 

+ Will deprecate update_cell_and_death_parameters, and instead use 
  update_phenotype_parameters. 
  
+ Will stop using the oxygen-dependent phenotype rule as default. 

PhysiCell 1.1.1 includes an advance copy of BioFVM 1.1.5. Here are the changes: 
	
/* fixes in BioFVM 1.1.5  */

+ correct typos in citation information in all source files 

+ updated citation information 

+ added void set_default_microenvironment( Microenvironment* M ) declaration to 
  BioFVM_Microenvironment.h
 
+ set volume_is_changed = false in Basic_Agent::set_internal_uptake_constants(); 

+ Set the MultiCellDS options Booleans to extern bool in BioFVM_MultiCellDS.h 
  so that PhysiCell can read these options. 

+ Updated the simlified_data field in MultiCellDS XML output to include a 
  new "source" attribute with value "BioFVM".
