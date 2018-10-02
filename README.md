# PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D Multicellular Systems. 

**Version:**      1.4.1
**Release date:** 2 October 2018 

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