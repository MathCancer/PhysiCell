# PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D Multicellular Systems.

**Version:** 1.9.1

**Release date:** 13 September 2021

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
    template 
    biorobots-sample 
    cancer-biorobots-sample 
    cancer-immune-sample
    celltypes3-sample 
    heterogeneity-sample 
    pred-prey-farmer 
    virus-macrophage-sample 
    worm-sample
    ode-energy-sample 
    physiboss-cell-lines-sample 
    cancer-metabolism-sample


**make list-projects** : list all available sample projects 

**make clean**         : removes all .o files and the executable, so that the next "make" recompiles the entire project 

**make data-cleanup**  : clears out all simulation data 

**make reset**         : de-populates the sample project and returns to the original PhysiCell state. Use this when switching to a new PhysiCell sample project. 

**make jpeg**          : uses ImageMagick to convert the SVG files in the output directory to JPG (with appropriate sizing to make movies). Supply `OUTPUT=foldername` to select a different folder. 

**make movie**         : uses ffmpeg to convert the JPG files in the output directory an mp4 movie. Supply `OUTPUT=foldername` to select a different folder, or `FRAMERATE=framerate` to override the frame rate.

**make upgrade**       : fetch the latest release of PhysiCell and overwrite the core library and sample projects. 

**Homepage:**     http://PhysiCell.MathCancer.org

**Downloads:**    http://PhysiCell.sf.net

**Support:**      https://sourceforge.net/p/physicell/tickets/

**Quick Start:**  Look at QuickStart.md in the documentation folder. 

**User Guide:**   Look at UserGuide.pdf in the documentation folder. 
 
**Tutorials:**    http://www.mathcancer.org/blog/physicell-tutorials/

**Latest info:**  follow [@PhysiCell](https://twitter.com/PhysiCell) on Twitter (http://twitter.com/PhysiCell)

See changes.md for the full change log. 

* * * 
## Release summary: 

This release focuses primarily on bug fixes. It fixes memory leaks and other bugs in intracellular modeling, as well as several small bugs in parsing cell definitions in the XML configuration file. It also implements a basic_volume_model that only models total volume. (For internal consistency, it treats the entire cell as cytoplasm.) 

**NOTE 1:** MacOS users need to define a PHYSICELL_CPP environment variable to specify their OpenMP-enabled g++. See the [Quickstart](documentation/Quickstart.md) for details.

**NOTE 2:** Windows users need to follow an updated (from v1.8) MinGW64 installation procedure. This will install an updated version of g++, plus libraries that are needed for some of the intracellular models. See the [Quickstart](documentation/Quickstart.md) for details.
 
### Major new features and changes:

+ None in this release. 

### Minor new features and changes: 

+ Implemented basic_volume_model (see standard models), where nuclear volumes are set to zero, and cytoplasmic volumes are updated as normal. 

### Beta features (not fully supported):
 
+ Started writing a standardized set of functions for Hill functions and promoter/inhibitor signaling. 

+ [Model Builder Tool](https://github.com/PhysiCell-Tools/PhysiCell-model-builder/releases) 

+ Added a simple Qt GUI for plotting cells only (plot_cells.py and vis_tab_cells_only.py in /beta)

+ Added a simple Qt GUI for plotting substrates and cells (plot_data.py and vis_tab.py in /beta)

+ Added simple contour plotting of a substrate (anim_substrate2D.py in /beta; copy to /output) 
  
### Bugfixes: 
+ Fixed bug in legend function where on some rare occasions, the temporary cell could adversely interact with other cells prior to deletion. 

+ Remove an old error printout from standard_elastic_contact_function that causes problem with the ANCIENT version of gcc (4.8.x) that nanoHUB refuses to upgrade. 

+ Fixed Libroadrunner memory leak issue.

+ Made minor bugfixes to parsing cell definitions in the XML configuration files: 
  + verify motility enabled flag is present before parsing its value
  + fix bug when parsing multiple death models

### Notices for intended changes that may affect backwards compatibility:
 
+ We intend to merge Custom_Variable and Custom_Vector_Variable in the very near future.  

+ We may change the role of operator() and operator[] in Custom_Variable to more closely mirror the functionality in Parameters<T>. 

+ Some search functions (e.g., to find a substrate or a custom variable) will start to return -1 if no matches are found, rather than 0. 
 
+ We will change the timing of when entry_functions are executed within cycle models. Right now, they are evaluated immediately after the exit from the preceding phase (and prior to any cell division events), which means that only the parent cell executes it, rather htan both daughter cells. Instead, we'll add an internal Boolean for "just exited a phase", and use this to exucte the entry function at the next cycle call. This should make daughter cells independently execute the entry function. 

+ We might make "trigger_death" clear out all the cell's functions, or at least add an option to do this. 
	
+ We will most probably merge all of "core" and "modules" into "core." 

### Planned future improvements: 

+ Further XML-based simulation setup. 

+ Read saved simulation states (as MultiCellDS digital snapshots)
  
+ Add cell differentiation functionality to Phenotype, to be executed during cell division events. 
 
+ Add a new standard phenotype function that uses mechanobiology, where high pressure can arrest cycle progression. (See https://twitter.com/MathCancer/status/1022555441518338048.) 
 
+ Add module for standardized pharmacodynamics, as prototyped in the nanobio project. (See https://nanohub.org/resources/pc4nanobio.) 
 
+ Create an angiogenesis sample project 
 
+ Create a small library of angiogenesis and vascularization codes as an optional standard module in ./modules (but not as a core component)

+ Improved plotting options in SVG 

+ Further update sample projects to make use of more efficient interaction testing available

+ Major refresh of documentation.
