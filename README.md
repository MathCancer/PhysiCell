# PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D Multicellular Systems.

**Version:** 1.7.0

**Release date:** 12 May 2020

## Overview: 
PhysiCell is a flexible open source framework for building agent-based multicellular models in 3-D tissue environments.

**Reference:** A Ghaffarizadeh, R Heiland, SH Friedman, SM Mumenthaler, and P Macklin, PhysiCell: an Open Source Physics-Based Cell Simulator for Multicellular Systems, PLoS Comput. Biol. 14(2): e1005991, 2018. DOI: [10.1371/journal.pcbi.1005991](https://dx.doi.org/10.1371/journal.pcbi.1005991)

Visit http://MathCancer.org/blog for the latest tutorials and help. 

**Notable recognition:**
+ [2019 PLoS Computational Biology Research Prize for Public Impact](https://blogs.plos.org/biologue/2019/05/31/announcing-the-winners-of-the-2019-plos-computational-biology-research-prize/)

### Key makefile rules:

**make**               : compiles the current project. If no 
                     project has been defined, it first 
                     populates the cancer heterogeneity 2D 
                     sample project and compiles it 
   
**make \[project-name\]**: populates the indicated sample project. 
                     Use "make" to compile it. 

  \[project-name\] choices:
    template2D 
    template3D
    biorobots-sample
    cancer-biorobots-sample
    heterogeneity-sample
    cancer-immune-sample 
    virus-macrophage-sample
    temlate 

**make list-projects** : list all available sample projects 

**make clean**         : removes all .o files and the executable, so that the next "make" recompiles the entire project 

**make data-cleanup**  : clears out all simulation data 

**make reset**         : de-populates the sample project and returns to the original PhysiCell state. Use this when switching to a new PhysiCell sample project. 


**Homepage:**     http://PhysiCell.MathCancer.org

**Downloads:**    http://PhysiCell.sf.net

**Support:**      https://sourceforge.net/p/physicell/tickets/

**Quick Start:**  Look at QuickStart.pdf in the documentation folder. 

**User Guide:**   Look at UserGuide.pdf in the documentation folder. 
 
**Tutorials:**    http://www.mathcancer.org/blog/physicell-tutorials/

**Latest info:**  follow [@MathCancer](https://twitter.com/MathCancer) on Twitter (http://twitter.com/MathCancer)

See changes.md for the full change log. 

* * * 
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