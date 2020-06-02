# PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D Multicellular Systems.

**Version:** 1.7.1

**Release date:** 2 June 2020

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
    template

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

This release introduces bug fixes (particularly the placement of daughter cells after division), introduces new functions for uniformly random sampling of the unit circle and unit sphere, and refines the beta implementation of XML-based cell definitions. 

**NOTE:** OSX users must now define PHYSICELL_CPP system variable. See the documentation.
 
### Major new features and changes:

+ No major changes. See 1.7.0 for most recent major changes. 

### Minor new features and changes: 

+ Created new function std::vector<double> UniformOnUnitSphere( void ) that returns a (uniformly) random vector (x,y,z) on the unit sphere. 

+ Created new function std::vector<double> UniformOnUnitCircle( void ) that returns a (uniformly) random vector (x,y,0) on the unit circle (in the z = 0 plane).  

+ Created std::vector<double> LegacyRandomOnUnitSphere() that reproduces old behaviors of creating a random vector on the unit sphere. Never use this except if trying to replicate old results. Always use UniformOnUnitSphere() instead. 

+ Changed default placement of daughter cells to use UniformOnUnitCircle(), in response to longstanding "future plan" to "introduce improvements to placement of daughter cells after division."

+ All sample projects now check for <options> in their XML config files. 

+ Template projects calculate gradients and perform internal substrate tracking by default. 

+ Moved the bool is_active from "protected" to "public" in the Basic_Agent class in BioFVM_basic_agent.h, so that cells be be moved back into the domain and reactivated as needed. 

+ Changed beta implementation of XML cell definitions: 
  + In cycle, transition_rates renamed to phase_transition_rates. PhysiCell will give a deprecatoin warning for transition_rates until the official release of XML cell definitions. 
  + In death, rates renamed to death_rates. PhysiCell will give a deprecatoin warning for transition_rates until the official release of XML cell definitions. 
  + In cycle and death, "phase_durations" can now be used in place of phase_transition rates. This may be more intuitive for some modelers. 

+ See 1.7.0 for other recent minor changes.

### Beta features (not fully supported):
 
+ Cell definitions can now be defined by XML files. See the note above. This functionality may be additionally refined or modified in the next few releases while still in beta. 
  
### Bugfixes: 

+ In response to SourceForge ticket 26, fixed placement of parent cell in Cell::divide()

+ Removed errant Cell_Definition in the new template sample project. 

+ Added an extra check for bad chemotaxis definitions in response ot SourceForge ticket 28. 

+ Fixed bugs in processing of the "death" section of XML cell definitions.  

### Notices for intended changes that may affect backwards compatibility:
 
+ We intend to merge Custom_Variable and Custom_Vector_Variable in the very near future.  

+ We may change the role of operator() and operator[] in Custom_Variable to more closely mirror the functionality in Parameters<T>. 

+ Some search functions (e.g., to find a substrate or a custom variable) will start to return -1 if no matches are found, rather than 0. 
 
+ We will change the timing of when entry_functions are executed within cycle models. Right now, they are evaluated immediately after the exit from the preceding phase (and prior to any cell division events), which means that only the parent cell executes it, rather htan both daughter cells. Instead, we'll add an internal Boolean for "just exited a phase", and use this to exucte the entry function at the next cycle call. This should make daughter cells independently execute the entry function. 

+ We might make "trigger_death" clear out all the cell's functions, or at least add an option to do this. 

### Planned future improvements: 

+ Methods or scripts to make "upgrading" PhysiCell easier for existing projects (to avoid overwriting the config file, Makefile, or custom files. 
 
+ Current "template" project will be rolled into a new "predator-prey" sample project, and "template" will be tidied up. 

+ Further XML-based simulation setup. 

+ current sample projects will be refactored to use XML cdell definitions. 
 
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