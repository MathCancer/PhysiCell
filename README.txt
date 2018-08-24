PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D 
Multicellular Systems.

Version:      1.3.2
Release date: 24 August 2018 

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
