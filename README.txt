PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D 
Multicellular Systems.

Version:      1.3.1
Release date: 31 July 2018 

Overview: 
PhysiCell is a flexible open source framework for building 
agent-based multicellular models in 3-D tissue environments. 

Reference: A Ghaffarizadeh, R Heiland, SH Friedman, 
   SM Mumenthaler, and P Macklin, PhysiCell: an Open Source 
   Physics-Based Cell Simulator for Multicellular Systems, 
   PLoS Comput. Biol. 14(2): e1005991, 2018. 
   DOI: 10.1371/journal.pcbi.1005991                    

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

-=-=-=-=-

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
 
+ Removed the (never-used) Mechanics.maximum_adhesion_distance. 
 
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
  
Notices for intended changes that may affect backwards compatibility:
 
+ template_projects folder will be removed
 
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
