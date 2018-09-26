# PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D Multicellular Systems. 

**Version:**      1.4.0
**Release date:** 26 September 2018 

## Overview: 
PhysiCell is a flexible open source framework for building agent-based multicellular models in 3-D tissue environments. 

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

make clean         : removes all .o files and the executable, so that 
                     the next "make" recompiles the entire project 

make data-cleanup  : clears out all simulation data 

make reset         : de-populates the sample project and returns to
                     the original PhysiCell state. Use this when 
                     switching to a new PhysiCell sample project. 


**Homepage:**     http://PhysiCell.MathCancer.org

**Downloads:**    http://PhysiCell.sf.net

**Support:**      https://sourceforge.net/p/physicell/tickets/

**Quick Start:**  Look at QuickStart.pdf in the documentation folder. 

**User Guide:**   Look at UserGuide.pdf in the documentation folder. 
 
**Tutorials:**    http://www.mathcancer.org/blog/physicell-tutorials/

**Latest info:**  follow [@MathCancer](https://twitter.com/MathCancer) on Twitter (http://twitter.com/MathCancer)

See changes.txt for the full change log. 

* * * 

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