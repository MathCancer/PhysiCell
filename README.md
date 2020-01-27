# PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D Multicellular Systems.

**Version:** 1.6.1

**Release date:** 26 January 2020

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
