# PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D Multicellular Systems.

**Version:** 1.7.1

**Release date:** 2 June 2020

## Release summary: 

This release introduces bug fixes (particularly the placement of daughter cells after division), introduces new functions for uniformly random sampling of the unit circle and unit sphere, and refines the beta implementation of XML-based cell definitions. 

**NOTE:** OSX users must now define PHYSICELL_CPP system variable. See the documentation.
 
### Major new features and changes:

+ No major changes. See 1.7.0 for most recent major changes. 

### Minor new features and changes: 

+ Created new function std::vector<double> UniformOnUnitSphere( void ) that returns a (uniformly) random vector (x,y,z) on the unit sphere. 

+ Created new function std::vector<double> UniformOnUnitCircle( void ) that returns a (uniformly) random vector (x,y,0) on the unit circle (in the z = 0 plane).  

+ Created std::vector<double> LegacyRandomOnUnitSphere() that reproduces old behaviors of creating a random vector on the unit sphere. Never use this except if trying to replicate old results. Always use UniformOnUnitSphere() instead. 

+ Changed default placement of daughter cells to use UniformOnUnitCircle(), in response to longstanding "future plan" to "introduce improvements to placement of daughter cells after division."

+ All sample projects now check for <options> in their XML config files. 

+ Template projects calculate gradients and perform internal substrate tracking by default. 

+ Moved the bool is_active from "protected" to "public" in the Basic_Agent class in BioFVM_basic_agent.h, so that cells be be moved back into the domain and reactivated as needed. 

+ Changed beta implementation of XML cell definitions: 
  + In cycle, transition_rates renamed to phase_transition_rates. PhysiCell will give a deprecatoin warning for transition_rates until the official release of XML cell definitions. 
  + In death, rates renamed to death_rates. PhysiCell will give a deprecatoin warning for transition_rates until the official release of XML cell definitions. 
  + In cycle and death, "phase_durations" can now be used in place of phase_transition rates. This may be more intuitive for some modelers. 

+ See 1.7.0 for other recent minor changes.

### Beta features (not fully supported):
 
+ Cell definitions can now be defined by XML files. See the note above. This functionality may be additionally refined or modified in the next few releases while still in beta. 
  
### Bugfixes: 

+ In response to SourceForge ticket 26, fixed placement of parent cell in Cell::divide()

+ Removed errant Cell_Definition in the new template sample project. 

+ Added an extra check for bad chemotaxis definitions in response ot SourceForge ticket 28. 

+ Fixed bugs in processing of the "death" section of XML cell definitions.  

### Notices for intended changes that may affect backwards compatibility:
 
+ We intend to merge Custom_Variable and Custom_Vector_Variable in the very near future.  

+ We may change the role of operator() and operator[] in Custom_Variable to more closely mirror the functionality in Parameters<T>. 

+ Some search functions (e.g., to find a substrate or a custom variable) will start to return -1 if no matches are found, rather than 0. 
 
+ We will change the timing of when entry_functions are executed within cycle models. Right now, they are evaluated immediately after the exit from the preceding phase (and prior to any cell division events), which means that only the parent cell executes it, rather htan both daughter cells. Instead, we'll add an internal Boolean for "just exited a phase", and use this to exucte the entry function at the next cycle call. This should make daughter cells independently execute the entry function. 

+ We might make "trigger_death" clear out all the cell's functions, or at least add an option to do this. 

### Planned future improvements: 

+ Methods or scripts to make "upgrading" PhysiCell easier for existing projects (to avoid overwriting the config file, Makefile, or custom files. 
 
+ Current "template" project will be rolled into a new "predator-prey" sample project, and "template" will be tidied up. 

+ Further XML-based simulation setup. 

+ current sample projects will be refactored to use XML cdell definitions. 
 
+ read saved simulation states (as MultiCellDS digital snapshots)
 
+ "mainline" prototype cell attach/detach mechanics as standard models (currently in the biorobots and immune examples)
 
+ integrate SBML-encoded systems of ODEs as custom data and functions for molecular-scale modeling 
  
+ integrate Boolean network support from PhysiBoSS into the mainline code (See http://dx.doi.org/10.1093/bioinformatics/bty766. )
  
+ Develop contact-based cell-cell interactions. 

+ Add cell differentiation functionality to Phenotype, to be executed during cell division events. 
 
+ Add a new standard phenotype function that uses mechanobiology, where high pressure can arrest cycle progression. (See https://twitter.com/MathCancer/status/1022555441518338048.) 
 
+ Add module for standardized pharmacodynamics, as prototyped in the nanobio project. (See https://nanohub.org/resources/pc4nanobio.) 
 
+ create an angiogenesis sample project 
 
+ create a small library of angiogenesis and vascularization codes as an optional standard module in ./modules (but not as a core component)

+ improved plotting options in SVG 

* * * 

# PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D Multicellular Systems.

**Version:** 1.7.0

**Release date:** 12 May 2020

## Release summary: 

This release (1) adds "net export" as a new form of more generalized substrate secretion, (2) adds helper funtions for cell size and volume for esier configuration, (3) adds a new, standardized chemotaxis function, (4) adds 1D diffusion, and (5) introduces XML-based cell definitions as a beta feature. It also incorporates a variety of bugfixes.   

**NOTE:** OSX users must now define PHYSICELL_CPP system variable. See the documentation.
 
### Major new features and changes:

+ Added "net_export_rates" to "secretion" part of Phenotype, and to the Basic_Agent class in BioFVM. This is in response to SourceForge ticket 19. 

+ Added new helper functions to help users to more easily set the *target* cell size: 
   + void Cell::set_target_volume( double new_volume ) sets the target total cell volume, while preserving the desired nuclear:cytoplasmic ratio and desired fluid fraction. In the default cell volume model, the cell will now approach this value by shrinking or growing. 
   + void Cell::set_target_radius( double new_radius ) behaves similarly, but using a radius instead. 

+ Added Cell::set_radius( double new_radius ) to set the cell's current radius to the new value, preserving the nuclear:cytoplasmic ratio and fluid fraction. Note that this does not change the target values, so the cell will shrink or grow back towards its current target size. 

+ Added 1-D diffusion solvers to BioFVM (useful for some coarse-grained problems). It solves for diffusion in the x-direction only. Use it by setting: 

  microenvironment.diffusion_decay_solver = diffusion_decay_solver__constant_coefficients_LOD_1D
  
  Use this right after setup_microenvironment() in your main.cpp file. Future versions will include an XML option to use 1D. Most users will never need this. 
  
+ Added a standardized chemotaxis function to the standard models: 

     void chemotaxis_direction( Cell* pCell , Phenotype& phenotype , double dt ); 

  This sets:
  
     phenotype.motility.motility_bias_direction = direction * grad( index ), where 
    
     direction = phenotype.motility.chemotaxis_direction 
        (1 to go up gradient, -1 to go down gradient)
     index = phenotype.motility.chemotaxis_index 
        (the index of one of hte diffusing substrates)

+ Added Cell_Definitions in XML (beta feature) in response to SourceForge ticket 5. Users will be able to set the cell defaults definition by XML, as well as additional cell definitions that "inherit" phenotype parameters from cell defaults. This vastly reduces teh amount of necessary C++ to define a model. The new "template" sample project unifies 2D and 3D model specification using the new XML-based cell definitions. The next few releases will refine documentation and roll teh new XML-based cell definitions out to all the other sample projects. 

### Minor new features and changes: 

+ Created get_cell_definition(std::string) to return by reference the matching cell definition (search by name). Returns cell_defaults if nothing found. 

+ Created get_cell_definition(int) to return by reference the matching cell definition (search by type). Returns cell_defaults if nothing found. 

+ added int chemotaxis_index and chemotaxis_direction to the Motility class to assist with a new standard chemotaxis function. 
 
+ scale_all_secretion_by_factor also scales net_export_rates.

+ sync_to_current_microenvironment and sync_to_microenvironment set up net_export_rates 

+ Secretion::advance now updates net_export processes 

+ Secretion::set_all_secretion_to_zero and Secretion::scale_all_secretion_by_factor act on net_export_rates as well. 

+ Cell::turn_off_reactions acts on net_export_rates as well. 

+ BioFVM::Basic_Agent::simulate_secretion_and_uptake now updates net_export processes, including impact on internal tracked substrate totals. (And all appropriate initializatoin functions have been updated. 

+ Updated documentation to reflect the new net export rates. 
 
+ Updated the documentation to fully state the biotransport PDEs (for better clarity), including notes on the dimensions of the parameters. 

+ Deprecated the following (unimplemented) function from the Volume class definition, as promised: 
```
 void update( Cell* pCell, Phenotype& phenotype, double dt )
```

+ Added a new registry (unsorted map) of all cell definitions called cell_definitions_by_name and a vector of cell definitions called cell_definitions_by_index.  

+ The Cell_Definition default constructor and copy constructor automatically register all new cell definitions in cell_definitions_by_index; 

+ Created a display_cell_definitions(std::ostream&) function to quickly list all cell definitions and key information.  

+ Created build_cell_definitions_maps() to create cell_definitions_by_name and cell_definitions_by_type. This should go at the end of create_cell_types(). 

+ Created find_cell_definition(std::string) to return a pointer to the matching cell definition (search by name). Returns NULL if nothing found. 

+ Created find_cell_definition(int) to return a pointer to the matching cell definition (search by type). Returns NULL if nothing found. 

+ Deprecated oxygen_index, glucose_index, TUMOR_TYPE, and VESSEL_TYPE from PhysiCell_Constants as promised. 

+ Minor source code cleanup in PhysiCell_settings.cpp. 

+ All sample projects now automatically build (and display) the registries of cell definitions via build_cell_definitions_maps() and display_cell_definitions(). 

+ added the following std::vector<bool> to Microenvironment_Options to facilitate setting Dirichlet conditions on specific boundaries for specific substates: Dirichlet_all, Dirichlet_xmin, Dirichlet_xmax, Dirichlet_ymin, Dirichlet_ymax, Dirichlet_zmin, Dirichlet_zmax, Dirichlet_interior. 

+ Minor cleanup in BioFVM_microenvironment.cpp 

+ Microenvironment::update_dirichlet_node(voxel_index,substrate_index,value) now sets dirichlet_activation_vectors[voxel_index][substrate_index] = true; 

+ Microenvironment::set_substrate_dirichlet_activation( int substrate_index , bool new_value ) now sets dirichlet_activation_vectors[voxel_index][substrate_index] for ALL Dirichlet nodes. 

+ Microenvironment::apply_dirichlet_conditions() now checks the Dirichlet activation vector of the individual voxel. 

+ Microenvironment::resize_voxels() and the various Microenvironment::resize_space() functions now resize dirichlet_activation_vectors, using the default dirichlet_activation_vector as the initial uniform activation vector. 

+ Microenvironment::resize_densities() and the various Microenvironment::add_density() functions now resize dirichlet_activation_vector and use it to intialize dirichlet_activation_vectors at every voxel. 

+ The various Microenvironment::add_density() functions now 

+ Added function Microenvironment::set_substrate_dirichlet_activation( int index, std::vector<bool>& new_value ) to set the entire vector of activation at a specific voxel. 


### Beta features (not fully supported):
 
+ Cell definitions can now be defined by XML files. See the note above. This functionality may be additionally refined in the next few releases while still in beta. 

  
### Bugfixes: 

+ In response to SourceForge ticket 25, added cell_defaults.phenotype.molecular.sync_to_microenvironment( &microenvironment ); to the create_cell_types() functions in the 2D and 3D template projects. 

+ In response to SourceForge ticket 18, update_cell_and_death_parameters_O2_based() now checks for deterministic necrosis. 

+ In response to GitHub issue 33, fixed issue where data-cleanup makefile rule gets a list of too many files. Rolled the new rule through to all the sample Makefiles as well. 

```
data-cleanup:
	rm -f *.mat
	rm -f *.xml
	rm -f *.svg
	rm -rf ./output
	mkdir ./output
	touch ./output/empty.txt
```
+ Updated Cell::Cell(), create_cell(), create_cell(Cell_Defintion), and convert_to_cell_definition() to call set_total_volume( phenotype.volume.total ). This makes sure that BioFVM knows the correct volume at the time of creation (or major update) so that it can save the correct values to outputs. This is in response to GitHub issue 22. 

+ Removed the false statement from the user manual that stated that the cytoplasmic:nuclear ratio is between 0 and 1. 

+ Removed the false statement from the user manual that stated that relative cell rupture volume is between 0 and 1. 

+ Updated the list of PhysiCell_Constants in response to SourceForge ticket 11. 

+ The various Microenvironment::add_density() functions now only set dirichlet_activation_vector = true for the newly added substrate, rather than *all* of them. This new vector is then used to initialize the activation vectors at every voxel. 

+ Microenvironment::get_substrate_dirichlet_activation() mistakenly returned a double. Now it returns bool. 

### Notices for intended changes that may affect backwards compatibility:
 
+ We intend to merge Custom_Variable and Custom_Vector_Variable in the very near future.  

+ We may change the role of operator() and operator[] in Custom_Variable to more closely mirror the functionality in Parameters<T>. 

+ We will introduce improvements to placement of daughter cells after division. 

+ Some search functions (e.g., to find a substrate or a custom variable) will start to return -1 if no matches are found, rather than 0. 
 
+ We will change the timing of when entry_functions are executed within cycle models. Right now, they are evaluated immediately after the exit from the preceding phase (and prior to any cell division events), which means that only the parent cell executes it, rather htan both daughter cells. Instead, we'll add an internal Boolean for "just exited a phase", and use this to exucte the entry function at the next cycle call. This should make daughter cells independently execute the entry function. 

+ We might make "trigger_death" clear out all the cell's functions, or at least add an option to do this. 

### Planned future improvements: 
 
+ Further XML-based simulation setup. 
 
+ read saved simulation states (as MultiCellDS digital snapshots)
 
+ "mainline" prototype cell attach/detach mechanics as standard models (currently in the biorobots and immune examples)
 
+ integrate SBML-encoded systems of ODEs as custom data and functions for molecular-scale modeling 
  
+ integrate Boolean network support from PhysiBoSS into the mainline code (See http://dx.doi.org/10.1093/bioinformatics/bty766. )
  
+ Develop contact-based cell-cell interactions. 

+ Add cell differentiation functionality to Phenotype, to be executed during cell division events. 
 
+ Add a new standard phenotype function that uses mechanobiology, where high pressure can arrest cycle progression. (See https://twitter.com/MathCancer/status/1022555441518338048.) 
 
+ Add module for standardized pharmacodynamics, as prototyped in the nanobio project. (See https://nanohub.org/resources/pc4nanobio.) 
 
+ create an angiogenesis sample project 
 
+ create a small library of angiogenesis and vascularization codes as an optional standard module in ./modules (but not as a core component)

+ improved plotting options in SVG 

* * * 

# PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D Multicellular Systems.

**Version:** 1.6.1

**Release date:** 26 January 2020

## Release summary: 

This release fixes minor bugs and improves the documentation. It also adds some minor new capabilities, such as setting time step sizes in the XML configuration file. 

**NOTE:** OSX users must now define PHYSICELL_CPP system variable. See the documentation.
 
### Major new features and changes:

+ List here. 
 
### Minor new features and changes: 
 
+ "make list-projects" now displayed to standard output a list of all the sample projects. 

+ dt_diffusion, dt_mechanics, and dt_phenotype can now be set via the XML configuration file in the options section. 

+ Added documentation on the time step sizes to the User Guide. 

+ Preliminary work to support Travis CI testing. 

+ Updated documentation to note that Cell::start_death is the preferred method to trigger cell death, and NOT Death::trigger_death. 

+ Updated Microenvironment::compute_all_gradient_vectors to now compute one-sized gradients on edge voxels. (Previously, no gradient was computed here.) 

+ Updated Microenvironment::compute_all_gradient_vectors to check if there is no z-direction (i.e., 2D) and exit early if so. 

+ Updated Microenvironment::compute_all_gradient_vectors to check if there is no y-direction (i.e., 1D) and exit early if so. 

+ Made PhysiCell_constants.cpp (and added this to the core of all project makefiles) so that dt and other variables can be non-static (i.e., set by XML options). 

+ Added "make checkpoint" rule to makefiles. This zips up the user-custom stuff (./config, ./, ./custom_modules) into a timestamped zip file. Use this before upgrading PhysiCell to make sure you keep your own Makefile, etc. 
 
### Beta features (not fully supported):
 
+ List here. 
  
### Bugfixes: 

+ BioFVM's diffusion_decay_solver__constant_coefficients_LOD_3D, diffusion_decay_solver__constant_coefficients_LOD_2D check for regular meshes instead of uniform meshes. 

+ Biorobots sample project fixed bugs on searching for substrates vs. searching for cell types. 

+ In BioFVM_vectors, the normalize functions now return a zero vector if the vector's norm is less than 1e-16. This is for John Metzcar. 

+ In PhysiCell_Cell.cpp, made fixes to Cell::divide() and Cell::assign_position() to fix a bug where cells dividing on the edge of the domain woudl place a daughter cell at (0,0,0). Thanks, Andrew Eckel!

+ Code cleanup in PhysiCell_cell_container in Cell_Container::update_all_cells() as suggested by Andrew Eckel. Thanks! 
 
### Notices for intended changes that may affect backwards compatibility:
 
+ We intend to merge Custom_Variable and Custom_Vector_Variable in the very near future.  

+ We may change the role of operator() and operator[] in Custom_Variable to more closely mirror the functionality in Parameters<T>. 

+ We will introduce improvements to placement of daughter cells after division. 

+ Some search functions (e.g., to find a substrate or a custom variable) will start to return -1 if no matches are found, rather than 0. 
 
+ We will change the timing of when entry_functions are executed within cycle models. Right now, they are evaluated immediately after the exit from the preceding phase (and prior to any cell division events), which means that only the parent cell executes it, rather htan both daughter cells. Instead, we'll add an internal Boolean for "just exited a phase", and use this to exucte the entry function at the next cycle call. This should make daughter cells independently execute the entry function. 

+ We might make "trigger_death" clear out all the cell's functions, or at least add an option to do this. 

### Planned future improvements: 
 
+ Further XML-based simulation setup. 
 
+ read saved simulation states (as MultiCellDS digital snapshots)
 
+ "mainline" prototype cell attach/detach mechanics as standard models (currently in the biorobots and immune examples)
 
+ integrate SBML-encoded systems of ODEs as custom data and functions for molecular-scale modeling 
  
+ integrate Boolean network support from PhysiBoSS into the mainline code (See http://dx.doi.org/10.1093/bioinformatics/bty766. )
  
+ Develop contact-based cell-cell interactions. 

+ Add cell differentiation functionality to Phenotype, to be executed during cell division events. 
 
+ Add a new standard phenotype function that uses mechanobiology, where high pressure can arrest cycle progression. (See https://twitter.com/MathCancer/status/1022555441518338048.) 
 
+ Add module for standardized pharmacodynamics, as prototyped in the nanobio project. (See https://nanohub.org/resources/pc4nanobio.) 
 
+ create an angiogenesis sample project 
 
+ create a small library of angiogenesis and vascularization codes as an optional standard module in ./modules (but not as a core component)

+ improved plotting options in SVG 

* * * 

**Version:** 1.6.0

**Release date:** 20 August 2019

## Release summary: 

This release introduces a new XML-based configuration for the chemical microenvironment. All 
the sample projects have been updated to use this new functionality. There is no change 
in APIs or high-level usage / syntax for end users; old projects should continue to work without 
modification, although we highly recommend migrating to the simplified microenvironment setup. 
A short blog tutorial on this new functionality can be found at 

http://mathcancer.org/blog/setting-up-the-physicell-microenvironment-with-xml

**NOTE:** OSX users must now define PHYSICELL_CPP system variable. See the documentation.
 
### Major new features and changes:

+ XML-based setup of the chemical microenvironment.  
 
### Minor new features and changes: 
 
+ Updated template2D sample project: 
  + Refined "reset" and "data-cleanup" rules in Makefile
  + Converted project to use the new XML-based microenvironment setup. 

+ Updated template3D sample project: 
  + Refined "reset" and "data-cleanup" rules in Makefile
  + Converted project to use the new XML-based microenvironment setup. 

+ Updated heterogeneity sample project: 
  + Refined "reset" and "data-cleanup" rules in Makefile
  + Converted project to use the new XML-based microenvironment setup. 

+ Updated cancer immune sample rpoject: 
  + Refined "reset" and "data-cleanup" rules in Makefile
  + Converted project to use the new XML-based microenvironment setup. 

+ Updated virus macrophage sample project: 
  + Refined "reset" and "data-cleanup" rules in Makefile
  + Converted project to use the new XML-based microenvironment setup. 
  + Enabled gradient calculations (were previously off, although we wanted macrophage chemotaxis) 

+ Updated biorobots sample project: 
  + Refined "reset" and "data-cleanup" rules in Makefile. 
  + Converted project to use the new XML-based microenvironment setup.
  + Note that values in user_parameters will override values in microenvironment_setup. 
  + Improved project to properly search for substrate indices instead of hard coding them. 

+ Updated cancer biorobots sample project: 
  + Refined "reset" rule in Makefile. 
  + Converted project to use the new XML-based microenvironment setup.
  + Improved project to properly search for substrate indices instead of hard coding them. 

+ Refined "reset" and "data-cleanup" rules in default Makefile 

+ Created new function to access the (private) microenvironment dirichlet_activation_vector: 
 
  double Microenvironment::get_substrate_dirichlet_activation( int substrate_index ); 

+ Updated the main microenvironment display function Microenvironment::display_information to summarize the initial and boundary conditions for each substrate 

+ Wrote two new functions to parse the XML in microenvironment_setup to add substrates and 
options:  
  + bool setup_microenvironment_from_XML( pugi::xml_node root_node )
  + bool setup_microenvironment_from_XML( void )
The second one assumes you already defined the root node and access the 
global (pugi)xml node for it. 

+ The main XML parsing function now calls setup_microenvironment_from_XML(), just before processing user-defined parameters. 
 
### Beta features (not fully supported):
 
+ anim_svg.py - now plots correctly sized cells; manually step via arrow keys

+ anim_svg_cycle.py - same as above, but automatically cycles through .svg files
  
### Bugfixes: 

+ None.
 
### Notices for intended changes that may affect backwards compatibility:
 
+ We intend to merge Custom_Variable and Custom_Vector_Variable in the very near future.  

+ We may change the role of operator() and operator[] in Custom_Variable to more closely mirror the functionality in Parameters<T>. 

+ We will introduce improvements to placement of daughter cells after division. 

+ Some search functions (e.g., to find a substrate or a custom variable) will start to return -1 if no matches are found, rather than 0. 

### Planned future improvements: 
 
+ Further XML-based simulation setup. 
 
+ read saved simulation states (as MultiCellDS digital snapshots)
 
+ "mainline" prototype cell attach/detach mechanics as standard models (currently in the biorobots and immune examples)
 
+ integrate SBML-encoded systems of ODEs as custom data and functions for molecular-scale modeling 
  
+ integrate Boolean network support from PhysiBoSS into the mainline code (See http://dx.doi.org/10.1093/bioinformatics/bty766. )
  
+ Develop contact-based cell-cell interactions. 

+ Add cell differentiation functionality to Phenotype, to be executed during cell division events. 
 
+ Add a new standard phenotype function that uses mechanobiology, where high pressure can arrest cycle progression. (See https://twitter.com/MathCancer/status/1022555441518338048.) 
 
+ Add module for standardized pharmacodynamics, as prototyped in the nanobio project. (See https://nanohub.org/resources/pc4nanobio.) 
 
+ create an angiogenesis sample project 
 
+ create a small library of angiogenesis and vascularization codes as an optional standard module in ./modules (but not as a core component)

+ improved plotting options in SVG 

* * * 
 
**Version:** 1.5.2

**Release date:** 11 June 2019

## Release summary: 

This minor release fixes bugs that affected the release of internalized substrates at cell death on Linux and OSX operating systems, relating to system differences in order of evaluating destructor functions. The release of internalized substrates has been moved to a new function, and placed in cell death functions. There is no change in APIs or high-level usage / syntax for end users. 

Users should also consult the release notes for 1.5.0. 

**NOTE:** OSX users must now define PHYSICELL_CPP system variable. See the documentation.
 
### Major new features and changes:

+ None 
 
### Minor new features and changes: 
 
+ Introduced new function Basic_Agent::release_internalized_substrates() to explicitly release a cell's internalized substrates, rather assuming it can be properly done in the Basic_Agent destructor function. 

+ Removed the Basic_Agent destructor function to allow the compiler to automatically generate this. 

+ Very minor revisions to the release protocol. 

+ Minor updates to the user guide to reflect the release_internalized_substrates() function. 
 
### Beta features (not fully supported):
 
+ anim_svg.py - now plots correctly sized cells; manually step via arrow keys

+ anim_svg_cycle.py - same as above, but automatically cycles through .svg files
  
### Bugfixes: 

+ Move code for internalized substrate release from the Basic_Agent destructor to the new Basic_Agent::release_internalized_substrates() function. 

+ Basic_Agent::release_internalized_substrates() is now called from delete_cell(int) in PhysiCell_cell.cpp. 

+ Basic_Agent::release_internalized_substrates() explicitly sets internalized_substrates to a zero vector, just in case users want to call this function on non-dead cells. 

+ Cell::Cell() now initializes updated_current_mechanics_voxel_index = 0 (avoids a possible segfault in GDB)
 
### Notices for intended changes that may affect backwards compatibility:
 
+ We intend to merge Custom_Variable and Custom_Vector_Variable in the very near future.  

+ We may change the role of operator() and operator[] in Custom_Variable to more closely mirror the functionality in Parameters<T>. 

+ We will introduce improvements to placement of daughter cells after division. 

+ Some search functions (e.g., to find a substrate or a custom variable) will start to return -1 if no matches are found, rather than 0. 

### Planned future improvements: 
 
+ Further XML-based simulation setup. 
 
+ read saved simulation states (as MultiCellDS digital snapshots)
 
+ "mainline" prototype cell attach/detach mechanics as standard models (currently in the biorobots and immune examples)
 
+ integrate SBML-encoded systems of ODEs as custom data and functions for molecular-scale modeling 
  
+ integrate Boolean network support from PhysiBoSS into the mainline code (See http://dx.doi.org/10.1093/bioinformatics/bty766. )
  
+ Develop contact-based cell-cell interactions. 

+ Add cell differentiation functionality to Phenotype, to be executed during cell division events. 
 
+ Add a new standard phenotype function that uses mechanobiology, where high pressure can arrest cycle progression. (See https://twitter.com/MathCancer/status/1022555441518338048.) 
 
+ Add module for standardized pharmacodynamics, as prototyped in the nanobio project. (See https://nanohub.org/resources/pc4nanobio.) 
 
+ create an angiogenesis sample project 
 
+ create a small library of angiogenesis and vascularization codes as an optional standard module in ./modules (but not as a core component)

+ improved plotting options in SVG 

* * * 

# PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D Multicellular Systems.
 
**Version:** 1.5.1

**Release date:** 7 June 2019

## Release summary: 

This minor release fixes bugs in the new virus-macrophage sample project. Users should also consult the reslease notes for 1.5.0. 

**NOTE:** OSX users must now define PHYSICELL_CPP system variable. See the documentation.
 
### Major new features and changes:

+ None 
 
### Minor new features and changes: 
 
+ None 
 
### Beta features (not fully supported):
 
+ None 
  
### Bugfixes: 

+ In the virus-macrophage sample project, switch cell death (in epithelial_function) from apoptosis to cell_lysis to demonstrate the new function. 

+ In the virus-macrophage sample project, enable internalized substrate tracking in the setup_microenvironment() function. 

+ In the virus-macrophage sample project, use a slower viral replication rate. (Should take 240 minutes to reach the lysis threshold.) 

+ In the virus-macrophage sample project, switched to a maximum simulation time of 24 hours (1440 minutes). 
 
### Notices for intended changes that may affect backwards compatibility:
 
+ We intend to merge Custom_Variable and Custom_Vector_Variable in the very near future.  

+ We may change the role of operator() and operator[] in Custom_Variable to more closely mirror the functionality in Parameters<T>. 

+ We will introduce improvements to placement of daughter cells after division. 

+ Some search functions (e.g., to find a substrate or a custom variable) will start to return -1 if no matches are found, rather than 0. 

### Planned future improvements: 
 
+ Further XML-based simulation setup. 
 
+ read saved simulation states (as MultiCellDS digital snapshots)
 
+ "mainline" prototype cell attach/detach mechanics as standard models (currently in the biorobots and immune examples)
 
+ integrate SBML-encoded systems of ODEs as custom data and functions for molecular-scale modeling 
  
+ integrate Boolean network support from PhysiBoSS into the mainline code (See http://dx.doi.org/10.1093/bioinformatics/bty766. )
  
+ Develop contact-based cell-cell interactions. 

+ Add cell differentiation functionality to Phenotype, to be executed during cell division events. 
 
+ Add a new standard phenotype function that uses mechanobiology, where high pressure can arrest cycle progression. (See https://twitter.com/MathCancer/status/1022555441518338048.) 
 
+ Add module for standardized pharmacodynamics, as prototyped in the nanobio project. (See https://nanohub.org/resources/pc4nanobio.) 
 
+ create an angiogenesis sample project 
 
+ create a small library of angiogenesis and vascularization codes as an optional standard module in ./modules (but not as a core component)

+ improved plotting options in SVG 

* * * 

# PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D Multicellular Systems.
 
**Version:** 1.5.0

**Release date:** 7 June 2019

## Release summary: 
 
This release introduces the ability to track the net internalization of diffusible substrates from the microenvironment, conserving mass with the diffusion-reaction partial differentiatial equations in BioFVM. This is part of longer-term plans to support molecular-scale models (e.g., as encoded by SBML). It also introduces the ability for cells to ingest other cells (and acquire their internalized substrates and volume), the ability for cells to release their internalized substrates back to the microenvironment after death, and a new cell death model (lysis). 

To illustrate these new capabilities, this release introduces a new sample project called virus-macrophage-sample. In this project, virus particles diffuse through the microenvironment, are uptaken by cells, replicate within cells, and trigger lytic death after reaching a threshold. Lysed cells release their virus particles to further diffuse through the environment. Macrophages move by random migration, test for contact with cells, and ingest / phagocytose cells based upon their viral load. Macrophages degrade their internalized viral particles. 

This release also added clearer methods to specify the microenvironment initial conditions, and improved documentation. 

**NOTE:** OSX users must now define PHYSICELL_CPP system variable. See the documentation.
 
### Major new features and changes:

+ Added functionality in BioFVM to (optionally) track the total amount of substrates in each cell, based upon tracking uptake and secretion. Note that without additional, user-defined functions to internally create or consume substrate (e.g., synthesizing proteins, or using oxygen in metabolism), this can result in negative internal values (if cells only secrete but no internal creation functions have been set) or unbounded positive values (if cells uptake a substrate but do not consume it). In particular, Basic_Agents (and hence Cells) now have: 
++ std::vector<double>* internalized_substrates (tracks internalized substrates)
++ std::vector<double>* fraction_released_at_death (sets the fraction of each substrate that is released at cell death)
++ std::vector<double>* fraction_transferred_when_ingested (sets the fraction of each substrate that is transferred to a predatory cell if the cell is eaten). 

Users should access these via the cell's (new) molecular portion of the phenotype. See below. 

In BioFVM::Microenvironment_Options class, we added: 
++ bool track_internalized_substrates_in_each_agent. Set this to true to enable the tracking. 

+ In BioFVM::Microenvironment_Options, we added the ability to set the (uniform) microenvironment initial conditions, via: 
++ std::vector<double> initial_condition_vector. 

If this has size zero, then BioFVM will use the std::vector<double> Dirichlet_condition_vector as a reasonable guess at initial conditions as in prior versions. 

+ We added a new function to Basic_Agent (and hence Cell) to facilitate registering with the microenvironment: 
++ void Basic_Agent::register_microenvironment( Microenvironment* microenvironment_in )

This function will ensure that the secretion, uptake, and internalization tracking data structures of the individual cell agent are properly matched to the microenvironment. 

+ Added new "Molecular" block to Phenotype, for storing internalized substrates and for eventual inclusion of molecular-scale models (via SBML). The major elements are: 
++ std::vector<double> internalized_substrates (tracks internalized substrates)
++ std::vector<double> fraction_released_at_death (sets the fraction of each substrate that is released at cell death)
++ std::vector<double> fraction_transferred_when_ingested (sets the fraction of each substrate that is transferred to a predatory cell if the cell is eaten). 

+ Added lyse_cell() to Cell, to allow a cell to immediately be lysed and potentially release its internalized substrates. Lysed cells are removed from the simulation. 
 
+ Added ingest_cell(Cell*) to Cell, to allow a cell to ingest (e.g., phagocytose) another cell, acquire its volume, and also (optionally) acquire its internalized substrates. This should be useful for immunology.  
 
### Minor new features and changes: 
 
+ Added void Microenvironment::update_dirichlet_node( int voxel_index , int substrate_index , double new_value ) based on pc4nanobio changes, which allows you to update a single substrate's dirichlet condition at specific voxel, rather than all of them. 

+ Added void sync_to_microenvironment( Microenvironment* pMicroenvironment ) to Phenotype to facilitate syncing the cell's phenotype to the microenvironment (and subsequently calls the functions in phenotype.secretion and phenotype.molecular). This isn't used yet, but perhaps it should be. 
 
### Beta features (not fully supported):
 
+ None 
  
### Bugfixes: 

+ Updated BioFVM's operator<< on std::vector<double> so that it doesn't output x="value", y="value", z = "value" for 3-D vectors. 

+ Fixed the search for cycle phase indices in the 2D and 3D template projects, to make sure it searches teh flow_cytometry_separated_cycle_model model and not the Ki67_advanced model, as part of the create_cell_types() function in the custom.cpp files. 

+ In PhysiCell_standard_models, standard_volume_update_function is now fixed to update phenotype.volume.fluid. (This was not used in any mechanics or other calculations, so it does not affect prior modeling results.) 

+ Removed repeated parameters (attached_worker_migration_bias, unattached_worker_migration_bias) in the cancer biorobots sample project.

+ trigger_death() now works. 
 
### Notices for intended changes that may affect backwards compatibility:
 
+ We intend to merge Custom_Variable and Custom_Vector_Variable in the very near future.  

+ We may change the role of operator() and operator[] in Custom_Variable to more closely mirror the functionality in Parameters<T>. 

+ We will introduce improvements to placement of daughter cells after division. 

+ Some search functions (e.g., to find a substrate or a custom variable) will start to return -1 if no matches are found, rather than 0. 

### Planned future improvements: 
 
+ Further XML-based simulation setup. 
 
+ read saved simulation states (as MultiCellDS digital snapshots)
 
+ "mainline" prototype cell attach/detach mechanics as standard models (currently in the biorobots and immune examples)
 
+ integrate SBML-encoded systems of ODEs as custom data and functions for molecular-scale modeling 
  
+ integrate Boolean network support from PhysiBoSS into the mainline code (See http://dx.doi.org/10.1093/bioinformatics/bty766. )
  
+ Develop contact-based cell-cell interactions. 

+ Add cell differentiation functionality to Phenotype, to be executed during cell division events. 
 
+ Add a new standard phenotype function that uses mechanobiology, where high pressure can arrest cycle progression. (See https://twitter.com/MathCancer/status/1022555441518338048.) 
 
+ Add module for standardized pharmacodynamics, as prototyped in the nanobio project. (See https://nanohub.org/resources/pc4nanobio.) 
 
+ create an angiogenesis sample project 
 
+ create a small library of angiogenesis and vascularization codes as an optional standard module in ./modules (but not as a core component)

* * * 

# PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D Multicellular Systems.
 
**Version:** 1.4.1

**Release date:** 2 October 2018

## Release summary: 
 
This release improves includes minor bug fixes for compiling in older versions of MinGW, and simplified XML MultiCellDS outputs that no longer record the relative pathing to .mat files. (This allows users to read data from their actual locations, rather than from a parent directory.) This release includes minor code cleanups in BioFVM for cleaner compiling in Ubuntu. Lastly, we have make small refinements to the sample projects and makefiles to default data saving to the ./output directory, and to prevent future releases from excluding the output directory from the zip releases. 

**NOTE:** OSX users must now define PHYSICELL_CPP system variable. See the documentation.
 
### Major new features and changes:

+ None 

### Minor new features and changes: 
 
+ Changed the MultiCellDS outputs to only store the filename, and not the full relative path, in the <filename> tags. This makes it simpler to load MultiCellDS outputs from matlab and other platforms. (No longer need to read from a directory higher up to make the relative pathing correct.) 

+ Did major cleanup on BioFVM so that it compiles cleanly on Ubuntu. 

+ All sample projects output to the ./output directory 
 
### Beta features (not fully supported):
 
+ None 
  
### Bugfixes: 

+ Updated the Parameter<T> constructor functions to create a specialized version for std::string, to fix odd compiling bugs on older versions of MinGW. (Worked in 7.1.0, but not in 5.3.0.) Now, Parameter<T> for T = bool, int, or double get initialized to value = (T) 0. And Parameter<T> for T = std::string gets initialized to "none". I had hoped to do a unified version, but value = (T) 0 for std::string acts like a NULL pointer. 

+ All Makefiles ensure that the reset and data-cleanup rules leave at least empty.txt in ./output, so that future releases are never missing the output directory. 
  
### Notices for intended changes that may affect backwards compatibility:
 
+ We intend to merge Custom_Variable and Custom_Vector_Variable in the very near future.  

+ We may change the role of operator() and operator[] in Custom_Variable to more closely mirror the functionality in Parameters<T>. 

### Planned future improvements: 
 
+ Further XML-based simulation setup. 
 
+ read saved simulation states (as MultiCellDS digital snapshots)
 
+ "mainline" prototype cell attach/detach mechanics as standard models (currently in the biorobots and immune examples)
 
+ integrate SBML-encoded systems of ODEs as custom data and functions for molecular-scale modeling 
  
+ integrate Boolean network support from PhysiBoSS into the mainline code (See http://dx.doi.org/10.1093/bioinformatics/bty766. )
  
+ Develop contact-based cell-cell interactions. (Likely in next release.)

+ Add cell differentiation functionality to Phenotype, to be executed during cell division events. 
 
+ Add a new standard phenotype function that uses mechanobiology, where high pressure can arrest cycle progression. (See https://twitter.com/MathCancer/status/1022555441518338048.) 
 
+ Add module for standardized pharmacodynamics, as prototyped in the nanobio project. (See https://nanohub.org/resources/pc4nanobio.) 
 
+ create an angiogenesis sample project 
 
+ create a small library of angiogenesis and vascularization codes as an optional standard module in ./modules (but not as a core component)

+ (optionally) track internalized substrate, coupled with BioFVM

* * * 

# PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D Multicellular Systems.
 
**Version:**      1.4.0

**Release date:** 26 September 2018

## Release summary: 
 
This release improves the use of XML parsing in configuring simulations, notably (1) reading the domain parameters instead of hard-coded values, and (2) parsing a <user_parameters> block in the XML config files to populate a global parameters data structure of Boolean, integer, double, and std::string variables. Users can efficiently query these from within any function anywhere in a PhysiCell project. 
 
**NOTE:** OSX users must now define PHYSICELL_CPP system variable. See the documentation.
 
### Major new features and changes:

+ User Parameters! 

+ Parsing XML to set domain size for all sample projects. 
  
### Minor new features and changes: 
 
+ Updated all the sample projects to use the improved XML parsing; 
  
+ New functions in PhysiCell_pugixml: 

  1) std::string xml_get_my_name( pugi::xml_node node ); 
  
     This helps to easily extract the name of an XML node. (e.g., <bob units="none"/> returns bob.) 

  2) bool xml_get_my_bool_value( pugi::xml_node node ); 
  
     This gets the Boolean value of an XML node. (e.g., <bob units="none">true</bob> returns true.) 
  
  3) int xml_get_my_int_value( pugi::xml_node node ); 
  
     This gets the integer value of an XML node. (e.g., <bob units="none">42</bob> returns 42.)
  
  4) double xml_get_my_double_value( pugi::xml_node node ); 

     This gets the double value of an XML node. (e.g., <bob units="none">42.03</bob> returns 42.03.)

  5) std::string xml_get_my_string_value( pugi::xml_node node ); 
  
+ Updated all Makefiles to copy main.cpp, the Makefile, and ./config/PhysiCell_settings.xml to backup copies prior to populating any sample project.      

+ Updated the heterogeneity sample project: 

  1) Use the domain settings from the XML config file
  
  2) Use the XML config file options to set the initial tumor size and oncoprotein distribution. 
  
  3) Get the random seed from the XML config file. 
  
  4) Rewrote the custom coloring function to scale from min oncoprotein value (blue) to max oncoprotein value (yellow). 
  
+ Updated template2D sample project:

  1) Use the domain settings from the XML config file
  
  2) Use the XML config file to set the motile cell parameters
  
  3) Get the random seed from the XML config file. 
  
  4) Updated to use the my_coloring_function coloring function. Made sure the my_coloring_function used false_cell_coloring_cytometry as its starting point. 
  
+ Updated template3D sample project:

  1) Use the domain settings from the XML config file
  
  2) Use the XML config file to set the motile cell parameters
  
  3) Get the random seed from the XML config file. 
  
  4) Updated to use the my_coloring_function coloring function. Made sure the my_coloring_function used false_cell_coloring_cytometry as its starting point. 
  
+ Updated biorobots sample project:   

  1) Use the domain settings from the XML config file
  
  2) Use the XML config file to set parameters and colors throughout the biorobots.cpp file 
  
  3) Get the random seed from the XML config file
  
+ Updated cancer immune sample project:   

  1) Use the domain settings from the XML config file
  
  2) Use the XML config file to set parameters throughout the cancer_immune_3D.cpp file 
  
  3) Get the random seed from the XML config file

+ New function in ./core/PhysiCell_utilities: 

  int choose_event( std::vector<double probabilities> );
  
  If probabilities is a vector of n probabilities (for n events), and the sum of the probabilities is 1, then this chooses one of those n events according to these probabilities and returns the index of the selected event. 

+ Moved from README.txt to README.md to support markdown and improve releases on both SourceForge and GitHub. 
  
+ Moved from changes.txt to changes.md to support markdown and improve releases on both SourceForge and GitHub. 
  
### Beta features (not fully supported):
 
+ None 
  
### Bugfixes: 

+ Updated the "reset" rules so that the default config file is restored (in all the sample makefiles). 
  
+ Removed a cout from Mechanics::set_relative_equilibrium_distance() from ./core/PhysiCell_phenotype.*
  
### Notices for intended changes that may affect backwards compatibility:
 
+ We intend to merge Custom_Variable and Custom_Vector_Variable in the very near future.  
 
### Planned future improvements: 
 
+ Further XML-based simulation setup. 
 
+ read saved simulation states (as MultiCellDS digital snapshots)
 
+ "mainline" prototype cell attach/detach mechanics as standard models (currently in the biorobots and immune examples)
 
+ integrate SBML-encoded systems of ODEs as custom data and functions for molecular-scale modeling 
  
+ integrate Boolean network support from PhysiBoSS into the mainline code (See http://dx.doi.org/10.1093/bioinformatics/bty766. )
  
+ Develop contact-based cell-cell interactions. (Likely in next release.)

+ Add cell differentiation functionality to Phenotype, to be executed during cell division events. 
 
+ Add a new standard phenotype function that uses mechanobiology, where high pressure can arrest cycle progression. (See https://twitter.com/MathCancer/status/1022555441518338048.) 
 
+ Add module for standardized pharmacodynamics, as prototyped in the nanobio project. (See https://nanohub.org/resources/pc4nanobio.) 
 
+ create an angiogenesis sample project 
 
+ create a small library of angiogenesis and vascularization codes as an optional standard module in ./modules (but not as a core component)

* * * 

PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D 
Multicellular Systems.
 
Version:      1.3.3
Release date: 16 September 2018

Release summary: 
 
This release introduces simplifications in versioning to facilitate 
faster release cycles. It also introduces functions to register and 
list citations for third-part add-ons. The goal is to encourage end-
users to properly cite PhysiCell and add-on products used in their 
projects. 
 
NOTE: OSX users must now define PHYSICELL_CPP system variable. 
      See the documentation.
 
Major new features and changes:
 
+ none
  
Minor new features and changes: 
 
+ Moved all version information to VERSION.txt to facilitate 
  faster release cycles. 
  
+ Updated all the Makefiles to set VERSION via 
  
  VERSION := $(shell grep . VERSION.txt | cut -f1 -d:)
  
+ Moved detailed citation information to CITATION.txt to 
  facilitate faster release cycles.
  
+ Changed std::string PhysiCell_version to 
  std::string PhysiCell_Version for more consistency with 
  prior BioFVM work. 
  
+ Added new static string to PhysiCell.h: 
  static std::string PhysiCell_DOI
  
+ Added new functions to PhysiCell_utilities to query 
  PhysiCell versioning: 

  std::string get_PhysiCell_version( void ); 
 
  void get_PhysiCell_version( std::string& pString ); 
  
+ Added the following function to PhysiCell_utilities to 
  display a list of all software versions and citations 
  used in the code:   
  
  void display_citations( std::ostream& os ); 
  void display_citations( void ); 
  
  For example, use display_citations( std::cout ), or use 
  an output file stream. Note that display_citations(void) 
  calls display_citation(std::cout);   
  
+ Updated all the sample projects to use display_citations(); 

Beta features (not fully supported):
 
+ Added ./protocols/ directory to include release and other 
  instructions, to help train new developer contributors. 
  Perhaps this should be called "checklists" ? 
  
+ Added the following functions to PhysiCell_utilities to 
  register third-party software citations in a global list, 
  ready for query and display: 
  
  void add_software_citation( std::string name , 
       std::string version, std::string DOI, std::string URL ); 
  
Bugfixes: 

+ None
  
Notices for intended changes that may affect backwards compatibility:
 
+ We will probably move from README.txt to README.md to support 
  markdown and improve releases on both SourceForge and GitHub. 
  
+ We will probably move from changes.txt to changes.md to support 
  markdown in the long-term change logs.  
 
+ We intend to merge Custom_Variable and Custom_Vector_Variable in the
  very near future.  
 
Planned future improvements: 
 
+ Further XML-based simulation setup. 
 
+ read saved simulation states (as MultiCellDS digital snapshots)
 
+ "mainline" prototype cell attach/detach mechanics as standard models 
  (currently in the biorobots and immune examples)
 
+ integrate SBML-encoded systems of ODEs as custom data and functions 
  for molecular-scale modeling 
  
+ integrate Boolean network support from PhysiBoSS into the mainline code 
  (See http://dx.doi.org/10.1093/bioinformatics/bty766. )
  
+ Develop contact-based cell-cell interactions. (Likely in next release.)
 
+ Add a new standard phenotype function that uses mechanobiology, 
  where high pressure can arrest cycle progression. 
  (See https://twitter.com/MathCancer/status/1022555441518338048.) 
  (Likely in next release.) 
 
+ Add module for standardized pharmacodynamics, as prototyped in the 
  nanobio project. (See https://nanohub.org/resources/pc4nanobio.) 
 
+ create an angiogenesis sample project 
 
+ create a small library of angiogenesis and vascularization codes as 
  an optional standard module in ./modules (but not as a core component)

=======================================================================

PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D 
Multicellular Systems.
 
Version:      1.3.2
Release date: 24 August 2018

Release summary: 
 
This release fixes a small gradient bug that I swear I had fixed before. 
 
NOTE: OSX users must now define PHYSICELL_CPP system variable. 
      See the documentation.
 
Major new features and changes:
 
+ none
  
Minor new features and changes: 
 
+ none
  
Beta features (not fully supported):
 
+ none
  
Bugfixes: 

+ In BioFVM, Microenvironment::compute_gradient_vector(int), removed "static"
  from "static std::vector<int> indices(3,0)" to prevent rare segfaults. 
  
+ In BioFVM, Microenvironment::compute_gradient_vector(int), replaced "static"
  for "bool gradient_constants_defined = false". Yep, I removed static from 
  the wrong line in 1.3.1 

+ Correct some errors in both the README.txt and changes.txt files. 
  
Notices for intended changes that may affect backwards compatibility:
 
+ None.
 
Planned future improvements: 
 
+ Further XML-based simulation setup. 
 
+ read saved simulation states (as MultiCellDS digital snapshots)
 
+ "mainline" prototype cell attach/detach mechanics as standard models 
  (currently in the biorobots and immune examples)
 
+ integrate SBML-encoded systems of ODEs as custom data and functions 
  for molecular-scale modeling 
  
+ integrate Boolean network support from PhysiBoSS into the mainline code 
  (See https://doi.org/10.1101/267070.)
  
+ Develop contact-based cell-cell interactions. (Likely in next release.)
 
+ Add a new standard phenotype function that uses mechanobiology, 
  where high pressure can arrest cycle progression. 
  (See https://twitter.com/MathCancer/status/1022555441518338048.) 
  (Likely in next release.) 
 
+ Add module for standardized pharmacodynamics, as prototyped in the 
  nanobio project. (See https://nanohub.org/resources/pc4nanobio.) 
 
+ create an angiogenesis sample project 
 
+ create a small library of angiogenesis and vascularization codes as 
  an optional standard module in ./modules (but not as a core component)

=======================================================================
 
PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D 
Multicellular Systems.
 
Version:      1.3.1
Release date: 31 July 2018
 
Release summary: 
 
This release introduces a new cycle model with an active cycling 
state and a quiescent state. It also adds new functions to the 
Mechanics class to make it easier to modify the cell-cell interaction 
distance (while maintaining equilibrium cell-cell spacing), or 
to modify the equilibrium cell-cell spacing. The release also 
includes major reliability and performance improvements to 
how gradients are calculated.
 
As usual, the release also contains minor bugfixes and 
improvements. 
 
NOTE: OSX users must now define PHYSICELL_CPP system variable. 
      See the documentation.
 
Major new features and changes:
 
+ Implemented a new cell cycle model (cycling_quiescent), 
  where quiescent cells can enter a cycling state. This 
  model uses identical parameters to the Ki67-basic cycle 
  model, but decouples the conceptual model from Ki67 
  immunohistochemistry. 
  
  Updated the documentation and coloring functions accordingly.
   
  Updated update_cell_and_death_parameters_O2_based() to 
  support the new cycling_quiescent cycle model.
 
+ update_all_cells(t,dt,dt,dt) now checks to see if gradient 
  calculations are enabled, and if so, computes it once per dt_mechanics.
 
  This improves code performance by 2x to 3x in gradient-enabled codes, 
  because we no longer need to calculate gradients once per dt_diffusion 
  time step. (Gradients are only needed for cell velocity calculations, 
  and occasionally for phenotype decisions.)
 
  All sample projects have been updated to make use of this.    
  
  Also, removed the explicit gradient calculations from the template 
  and sample projects. 
  
+ Added a new function to Mechanics class to simplify changes to cell 
  interaction distances: 
  
     void set_relative_maximum_adhesion_distance( double new_value ); 
   
  Here, new_value is a multiple of the cell's (mean equivalent) radius. 
  Our default is 1.25. This function preserves the cell's "repulsion" 
  strength and adjusts the strength of cell-cell adhesion to maintain 
  its prior equilibrium cell-cell spacing. 
  
+ Added a new function to Mechanics class to simplify changes to cell 
  equilibrium distances. 
  
     void set_relative_equilibrium_distance( double new_value );   
  
  Here, new_value is a multiple of the cell's (mean equivalent) radius. 
  The default is around 1.9. This function preserves the cell's "repulsion" 
  and the maximum interaction distance, and it adjusts the strength of 
  cell-cell adhesion to match the new equilibrium cell-cell spacing. 
  
  Note that this function performs a "sanity check" to ensure that you have 
  not given a value greater than 2.0. 
  
+ Added a new function to Mechanics class to simplify changes to cell 
  equilibrium distances. 
  
     void Mechanics::set_absolute_equilibrium_distance( Phenotype& phenotype, 
     double new_value )
 
  Here, new_value is the new equilibrium spacing (in units of microns). 
  The function internally requires the cell's mean equivalent radius, 
  and so you pass the cell's phenotype (by reference) as an additional 
  argument. 
  
  Note that this function performs a "sanity check" to ensure that you have 
  not given a value greater than 2.0 * mean_cell_radius.  	
  
  Also note that PhysiCell internally uses a relative spacing, so the 
  absolute spacing will change from the value you set, over time.  
  
+ Updated User_Guide to reflect new cell cycle model, 
  including reference parameters chosen for consistency with 
  the other cycle models. 
  
Minor new features and changes: 
 
+ Added the following constants to support the new cycle model
 
     static const int cycling = 17; 
     static const int quiescent = 18; 
     static const int cycling_quiescent_model = 7; 
 
+ Added new coloring function: false_cell_coloring_cycling_quiescent 
 
+ Removed the (never-used) Mechanics.maximum_adhesion_distance. 
 
+ Removed the legacy template_projects folder.
 
Beta features (not fully supported):
 
+ Added a function pointer to the Cell_Functions class for intended 
  contact-based cell-cell interaction, like delta-Notch signaling. 
  
     void (*contact_function)(Cell* pMyself, Phenotype& my_phenotype, 
       Cell* pOther, Phenotype& other_phenotype, double dt ); 
   
  It would be up to the user to define these functions and decide 
  if the functions modify pMyself, pOther, or both. For now, 
  the code will initialize the pointer to NULL and won't use it. 
 
+ Open to comments on handling cell-cell contact functions. Here's what 
  I have in mind: 
  
  notation: cell i interacts with cell j with function f(i,j). 
  
  Each cell agent can hold one contact-based interaction function f, 
  to be stored as a pointer in the cell's instance of the Cell_Functions 
  class. 
  
  We use the containers (and their interaction testing structures) to  
  identify all interactions (i,j,f), and add it to a vector of interactions. 
  The interaction (i,j,f) is added to the list so long as (j,i,f) is not 
  already in the list, to avoid double-counting the interaction. 
  
  The code will seek through the "container" interaction testing 
  data structure, probably at the cell mechanics time scale, and update /
  recreate the vector of contact-based interactions (i,j,f). 
  
  The code would likely go through the vector of interactions and 
  execute the codes once per dt_diffusion time step, since I would 
  imagine molecular biology (with faster time scales) is intended here.
  
  Since f(i,j) can potentially modify both cell i and cell j, it's probably 
  not thread-safe. So we'll probably need that in a non-parallel loop. 
  
  We will probably add a new time scale for interactions, dt_interactions, 
  and update the interaction list on that time scale. 
  
  For faster checking if (i,j,f) or (j,i,f) is already in the vector, 
  we'll probably want some sort of hash map in addition to the vector 
  of interactions. 
  
  We'll probably implement this all in something like 
  PhysiCell_contact_interctions.*, and add a global enable/disable option. 
  I'd imagine we'd add code to the "update_all_cells" to keep this 
  as simple to the users as possible. 
  
  We should probably update each cell's "neighbors" data structure at 
  when we're doing all this testing. 
  
  In a longer-term update, we could leverage that for simpler interaction 
  testing during velocity updates. 
  
Bugfixes: 

+ Added missing "omp_set_num_threads(PhysiCell_settings.omp_num_threads)" 
  in the main-heterogeneity.cpp file for the heterogeneity sample project. 

+ In BioFVM, Microenvironment::compute_gradient_vector(int), removed "static"
  from "static std::vector<int> indices(3,0)" to prevent rare segfaults. 
  
+ Changed <xml> root above the comment lines in output files for 
  better Python parsing compatibility. Thanks, rheiland! 

+ MultiCellDS outputs used size 3 for all custom vector variables, instead 
  of actual size. Fixed to read the size from the vector. 

+ Fixed the ostream operator for Vector_Variable to correctly output all 
  the vector.value elements, rather than the original hard-coded output 
  of 3 elements regardless of size. 
  
Notices for intended changes that may affect backwards compatibility:
 
+ None
 
Planned future improvements: 
 
+ Further XML-based simulation setup. 
 
+ read saved simulation states (as MultiCellDS digital snapshots)
 
+ "mainline" prototype cell attach/detach mechanics as standard models 
  (currently in the biorobots and immune examples)
 
+ integrate SBML-encoded systems of ODEs as custom data and functions 
  for molecular-scale modeling 
  
+ integrate Boolean network support from PhysiBoSS into the mainline code 
  (See https://doi.org/10.1101/267070.)
  
+ Develop contact-based cell-cell interactions. (Likely in next release.)
 
+ Add a new standard phenotype function that uses mechanobiology, 
  where high pressure can arrest cycle progression. 
  (See https://twitter.com/MathCancer/status/1022555441518338048.) 
  (Likely in next release.) 
 
+ Add module for standardized pharmacodynamics, as prototyped in the 
  nanobio project. (See https://nanohub.org/resources/pc4nanobio.) 
 
+ create an angiogenesis sample project 
 
+ create a small library of angiogenesis and vascularization codes as 
  an optional standard module in ./modules (but not as a core component)
 
=======================================================================
 
PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D 
Multicellular Systems.

Version:      1.3.0
Release date: 23 February 2018 

Overview: 
PhysiCell is a flexible open source framework for building 
agent-based multicellular models in 3-D tissue environments. 

Reference: A Ghaffarizadeh, R Heiland, SH Friedman, 
   SM Mumenthaler, and P Macklin, PhysiCell: an Open Source 
   Physics-Based Cell Simulator for Multicellular Systems, 
   PLoS Comput. Biol. 14(2): e1005991, 2018. 
   DOI: 10.1371/journal.pcbi.1005991                    

Visit http://MathCancer.org/blog for the latest tutorials and help. 

Release summary: 
This release introduces two new cell cycle models (G0/G1 -> 
S -> G2/M) and (G0/G1 -> S -> G2 -> M), introduces XML-based 
configuration files, and allows new user control on how 
often and where data are stored. 

As usual, the release also contains minor bugfixes and 
improvements. 

NOTE: OSX users must now define PHYSICELL_CPP system variable. 
      See the documentation.

Major new features and changes:
+ implemented new cell cycle models: 

     flow_cytometry_cycle_model: G0/G1 -> S -> G2/M
     flow_cytometry_separated_cycle_model: G0/G1 -> S -> G2 -> M 
	 
   along with documentation and updated coloring functions 
   
+ The oxygen-based phenotype models now support the new cycle
  model. 
  
+ Added XML file parsing for use in settings files. 

+ XML configuration file (in ./config/PhysiCell_settings.xml) 
  sets data destination (as a subfolder) and save 
  frequency. Legacy saves are now off by default. 
  
+ Users can select a different XML file at command line
 
    ./project_name settings_file.xml 
 
  (Assuming you preserved the structure of the template 
   projects.)
  
+ Updated User_Guide to reflect new XML parsing capabilities.
 
+ Updated User_Guide to reflect new cell cycle models, 
  including reference parameters chosen for consistency with 
  the other cycle models. 
   
Minor new features and changes: 
+ Added PhysiCell_pugixml.* for easier extraction of parameter 
  arguments from XML files
 
+ Added PhysiCell_settings.* to include parsing of XML parameter 
  files, and parameter values stored in a unified data structure. 
  
+ Added parsing of settings file to separately set frequency of 
  saving full output (MultiCellDS), SVG outputs, and "legacy" 
  outputs from USC era.
  
+ Added options to specify the folder of saved data.
 
+ Added option to read the number of OMP threads from a setting file.
 
+ Added "beta" directory where we will put new features that are still
  undergoing testing 
 
+ Minor updates to the Quickstart guide. 
 
+ Added new function: to display the simulation status: 
 
     void display_simulation_status( std::ostream& os ); 
 
     e.g., display_simulation_status( std::cout ); 
 
+ writePov() uses a user-specified output folder.

+ log_output() uses a user-specified output folder. 

+ added "beta-testing" sample project. Populate it by the rule: 
     make beta-testing
 
+ added flow_cytometry_separated_cycle_model to the PhysiCell constants. 
 
+ added G1pm_phase and G1ps_phase to the PhysiCell constants. 
 
+ Added new coloring function: false_cell_coloring_cytometry
 
+ added support for the new cytometry cycle models to the oxygen-based 
     phenotype model (update_cell_and_death_parameters_O2_based)
 
+ updated user manual to reflect new cytometry models 

+ updated template2D and template3D projects to use to use the 
  new cytometry models and coloring schemes. Also reduced to 
  1 mm x 1 mm (2D) and 1 mm^3 (3D) for faster demos. 
  
+ removed archives directory 

Beta features (not fully supported):

+ XML functions moved from beta to production. 

Bugfixes: 

+ Changed instances of uniform_random() (from BioFVM) to 
  UniformRandom() (from PhysiCell) so that all calls to the PRNG
  used the same random seed and same PRNG. Thanks, olliemcdonald!

+ Fixed typo in "Dirichlet" in user documentation. Thanks, luissv7! 

+ Removed .git directory that was accidentally included in releases

+ Updated PhysiCell_MultiCellDS.cpp 
  (add_PhysiCell_cells_to_open_xml_pugi) so that we exit(-1) with a 
  meaningful error message if we cannot open a matlab subfile 
  for writing. 
  
+ Updated PhysiCell_pathology.cpp (SVG_plot) so that we exit(-1) 
  with a meaningful error message if we cannot open an SVG file 
  for writing. 
  
Notices for intended changes that may affect backwards compatibility:

+ template_projects folder will be removed

Planned future improvements: 

+ Further XML-based simulation setup. 

+ read saved simulation states (as MultiCellDS digital snapshots)

+ "mainline" prototype cell attach/detach mechanics as standard models 
  (currently in the biorobots and immune examples)

+ integrate SBML-encoded systems of ODEs as custom data and functions 
  for molecular-scale modeling 

+ create an angiogenesis sample project 

+ create a small library of angiogenesis and vascularization codes as 
  an optional standard module in ./modules (but not as a core component)
  
=======================================================================

PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D 
Multicellular Systems.

Version:      1.2.2
Release date: 4 November 2017

Overview: 
PhysiCell is a flexible open source framework for building 
agent-based multicellular models in 3-D tissue environments. 

Reference: Ghaffarizadeh et al., PLoS Comput Biol (2017)
   preprint URL: https://doi.org/10.1101/088773

Visit http://MathCancer.org/blog for the latest tutorials and help. 

Key makefile rules: 

make               : compiles the current project. If no 
                     project has been defined, it first 
                     populates the cancer heterogeneity 2D 
                     sample project and compiles it 
   
make <project-name>: populates the indicated sample project. 
                     Use "make" to compile it. 

  <project_name> choices:
    template2D 
    template3D
    biorobots-sample
    cancer-biorobots-sample
    heterogeneity-sample
    cancer-immune-sample 

make clean         : removes all .o files and the executable, so that 
                     the next "make" recompiles the entire project 

make data-cleanup  : clears out all simulation data 

make reset         : de-populates the sample project and returns to
                     the original PhysiCell state. Use this when 
                     switching to a new PhysiCell sample project. 


Homepage:     http://PhysiCell.MathCancer.org
Downloads:    http://PhysiCell.sf.net
Support:      https://sourceforge.net/p/physicell/tickets/

Quick Start:  Look at QuickStart.pdf in the documentation folder. 
User Guide:   Look at UserGuide.pdf in the documentation folder. 
 
Tutorials:    http://www.mathcancer.org/blog/physicell-tutorials/

Latest info:  follow @MathCancer on Twitter (http://twitter.com/MathCancer)

See changes.txt for the full change log. 

Release summary: 
This release reduces the complexity of Makefiles (especially for 
OSX users), restructures the 2D and 3D project templates as 
sample projects, fixes a minor bug in SVG pathology outputs, 
improves copying of Cell_Definitions, and fixes minor bugs in 
BioFVM (primarily related to Dirichlet conditions). 

NOTE: OSX users must now define PHYSICELL_CPP system variable. 
      See the documentation.

PhysiCell is currently under scientific peer review. 

Major new features and changes:
+ none 

Minor new features and changes: 
+ Restructured the 2D template project to have the same structure as a 
  typical project, with setup functions related functions in 
  ./custom_modules/*, etc. Moved it to ./sample_projects/template2D/
  ./template_projects/ will be deprecated. 
  
  To populate this project, use: 
  
    make template2D 

  To compile: 
  
    make 
	
  To de-populate the sample project and return to the "clean"PhysiCell state:
  
    make reset 
    make clean (to remove object files)
  
+ Restructured the 3D template project to have the same structure as a 
  typical project, with setup functions related functions in 
  ./custom_modules/*, etc. Moved it to ./sample_projects/template3D/
  ./template_projects/ will be deprecated. 
  
  To populate this project, use: 
  
    make template3D 

  To compile: 
  
    make 
	
  To de-populate the sample project and return to the "clean"PhysiCell state:
  
    make reset 
    make clean (to remove object files)  

+ Simplified Makefiles: populating a sample project and compiling it 

    make <sample_project>

  To compile:

    make 

  To reset to original state:

    make reset 
	
  Current <sample_project> values: 	
    template2D 
    template3D
    biorobots-sample
    cancer-biorobots-sample
    heterogeneity-sample
    cancer-immune-sample 

+ Simplified Makefiles: Makefiles check for system variable 
  PHYSICELL_CPP to set the compiler (CC). OSX users must set 
  this environment variables. See the online tutorials and the 
  user guide. 
  
+ Simplified Makefiles: "make data-cleanup" removes .svg, .mat, .xml, and 
  data inside ./data 

+ Updated documentation on how to add new substrates. 

+ Updated documentation on applying Dirichlet conditions to only 
  specific substrates. 

+ Added new function to copy the properties of a Cell Definition to 
  the cell. 
  
  void Cell::convert_to_cell_definition( Cell_Definition& cd )

Bugfixes: 

+ Fixed a small error in SVG plots, where tissues were flipped with 
  y was vertically flipped. 
  
+ Used register_microenvironment(Microenvironment*) to improve 
  compatibiltiy with other operating systems and compilers. 
  
+ Added copy constructor and copy assignnment functions to the 
  Cell_Definition is. 

+ Removed the unnecessary "wha???" from BioFVM_microenvironment.cpp.

+ Updated Dirichlet_condition_vector = ones (instead of zeros) in 
  Microenvironment_Options::Microenvironment_Options() default 
  constructor. 

+ Microenvironment::resize_densities( int new_size ) no longer 
  overwrites previous dirichlet values when extending the size. 

+ No longer overwrites existing Dirichlet_condition_vector elements 
  or set default_microenvironment_options.use_oxygen_as_first_field 
  to false. 

+ Microenvironment::set_density(int,std::string,std::string) and 
  Microenvironment::set_density(int,std::string,std::string,double,double) 
  were modified to be compatibility. 
  
+ Only set default_microenvironment_options.use_oxygen_as_first_field = false
  if index = 0, when samplign the oxygen. 
  
+ Updated save_PhysiCell_to_MultiCellDS_xml_pugi() to save much more 
  phenotype information and all custom variables for each cell. 

+ Updated read_MultiCellDS_XML.m (in ./matlab) to read these 
  newly expanded data files. 

+ Includes a sneak preview of BioFVM 1.1.7, which includes bugfixes 
  mentioned above. 
  
Notices for intended changes that may affect backwards compatibility:

+ None at this time

Planned future improvements: 

+ parse XML configuration files

+ read saved simulation states (as MultiCellDS digital snapshots)

+ "mainline" prototype cell attach/detach mechanics as standard models 
  (currently in the biorobots and immune examples)

+ integrate SBML-encoded systems of ODEs as custom data and functions 
  for molecular-scale modeling 

+ create an angiogenesis sample project 

+ create a small library of angiogenesis and vascularization codes as 
  an optional standard module in ./modules (but not as a core component)

=======================================================================

PhysiCell: PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D 
Multicellular Systems.

Version:      1.2.1
Release date: 1 September 2017

Homepage:     http://PhysiCell.MathCancer.org
Downloads:    http://PhysiCell.sf.net

Release summary: 
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

=======================================================================

Version:      1.2.0
Release date: 30 July 2017

Homepage:     http://PhysiCell.MathCancer.org
Downloads:    http://PhysiCell.sf.net

Release summary: 
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

=======================================================================

Version:      1.1.1
Release date: 16 May 2017

Homepage:     http://PhysiCell.MathCancer.org
Downloads:    http://PhysiCell.sf.net

Release summary: 
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
  
=======================================================================

PhysiCell: PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D 
Multicellular Systems.

Version:      1.1.0
Release date: 16 May 2017

Homepage:     http://PhysiCell.MathCancer.org
Downloads:    http://PhysiCell.sf.net

Release summary: 
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

PhysiCell 1.1.0 includes an advance copy of BioFVM 1.1.5. Here are the changes: 
	
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
  
=======================================================================
	
PhysiCell: PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D 
Multicellular Systems.

Version:      1.0.0
Release date: 12 September 2016

Homepage:     http://PhysiCell.MathCancer.org
Downloads:    http://PhysiCell.sf.net

Release summary: 
This is the initial public release of PhysiCell: an Open Source Physics-Based 
Cell Simulator for 3-D Multicellular Systems. It is currently under scientific 
peer review. 

Major new features:
+ Simulating large systems of cells in 3-D tissues on desktop machine
+ Cells move and change based on biomechanical and physical rules
+ Built upon BioFVM to couple processes with the substrates' concentrations 
  in the microenvironment 
+ Implements multiple realistic cell cycle and cell death models
+ Performance linearly scales with the number of the cells
+ Preliminary support for MultiCellDS (http://MultiCellDS.org)
+ Includes examples for 3-D ductal carcinoma in situ (DCIS) and hanging 
  drop tumor spheroids 

Bugfixes: 
+ Not applicable. 

=======================================================================

PhysiCell: an open source physics-based multicell simulator

Version:      0.5.0
Release date: 2 Auguust

Homepage:     http://PhysiCell.MathCancer.org
Downloads:    http://PhysiCell.sf.net

Release summary:  
Pre-release. Not for public use. Not supported.  

New features:
+ First code bundling. 

Bugfixes: 
+ Nothing to see here. Move along. 

