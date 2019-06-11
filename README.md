# PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D Multicellular Systems.

**Version:**      1.5.2

**Release date:** 11 June 2019

## Overview: 
PhysiCell is a flexible open source framework for building agent-based multicellular models in 3-D tissue environments.

**Reference:** A Ghaffarizadeh, R Heiland, SH Friedman, SM Mumenthaler, and P Macklin, PhysiCell: an Open Source Physics-Based Cell Simulator for Multicellular Systems, PLoS Comput. Biol. 14(2): e1005991, 2018. DOI: [10.1371/journal.pcbi.1005991](https://dx.doi.org/10.1371/journal.pcbi.1005991)

Visit http://MathCancer.org/blog for the latest tutorials and help. 

**Notable recognition:**
+ [2019 PLoS Computational Biology Research Prize for Public Impact](https://blogs.plos.org/biologue/2019/05/31/announcing-the-winners-of-the-2019-plos-computational-biology-research-prize/)

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