# PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D Multicellular Systems.

**Version:**      1.5.0

**Release date:** 7 June 2019

## Overview: 
PhysiCell is a flexible open source framework for building agent-based multicellular models in 3-D tissue environments.

(Incidentally, it won the [2019 PLoS Computational Biology Research Prize for Public Impact](https://blogs.plos.org/biologue/2019/05/31/announcing-the-winners-of-the-2019-plos-computational-biology-research-prize/).) 

**Reference:** A Ghaffarizadeh, R Heiland, SH Friedman, SM Mumenthaler, and P Macklin, PhysiCell: an Open Source Physics-Based Cell Simulator for Multicellular Systems, PLoS Comput. Biol. 14(2): e1005991, 2018. DOI: [10.1371/journal.pcbi.1005991](https://dx.doi.org/10.1371/journal.pcbi.1005991)

Visit http://MathCancer.org/blog for the latest tutorials and help. 

### Key makefile rules:

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
    virus-macrophage-sample

make clean         : removes all .o files and the executable, so that the next "make" recompiles the entire project 

make data-cleanup  : clears out all simulation data 

make reset         : de-populates the sample project and returns to the original PhysiCell state. Use this when switching to a new PhysiCell sample project. 


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
