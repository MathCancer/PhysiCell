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

Summary: 
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





  

	
