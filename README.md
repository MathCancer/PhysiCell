**PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D 
Multicellular Systems.**

**Version:**      1.3.3
**Release date:** 16 September 2018 

**Overview:** 
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

Latest info:  follow [@MathCancer](https://twitter.com/MathCancer) on Twitter (http://twitter.com/MathCancer)

See changes.txt for the full change log. 

-=-=-=-=-

## Release summary:
 
This release introduces simplifications in versioning to facilitate faster release cycles. It also introduces functions to register and list citations for third-part add-ons. The goal is to encourage end-users to properly cite PhysiCell and add-on products used in their projects. 
 
**NOTE:** OSX users must now define PHYSICELL_CPP system variable. 
      See the documentation.
 
### Major new features and changes:
 
+ none
  
### Minor new features and changes: 
 
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

### Beta features (not fully supported):
 
+ Added ./protocols/ directory to include release and other 
  instructions, to help train new developer contributors. 
  Perhaps this should be called "checklists" ? 
  
+ Added the following functions to PhysiCell_utilities to 
  register third-party software citations in a global list, 
  ready for query and display: 
  
  void add_software_citation( std::string name , 
       std::string version, std::string DOI, std::string URL ); 
  
### Bugfixes: 

+ None
  
### Notices for intended changes that may affect backwards compatibility:
 
+ We will probably move from README.txt to README.md to support 
  markdown and improve releases on both SourceForge and GitHub. 
  
+ We will probably move from changes.txt to changes.md to support 
  markdown in the long-term change logs.  
 
+ We intend to merge Custom_Variable and Custom_Vector_Variable in the
  very near future.  
 
### Planned future improvements: 
 
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
  
