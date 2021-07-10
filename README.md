# PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D Multicellular Systems.

**Version:** 1.9.0

**Release date:** 11 July 2021

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

**Latest info:**  follow [@MathCancer](https://twitter.com/MathCancer) on Twitter (http://twitter.com/MathCancer)

See changes.md for the full change log. 

* * * 
## Release summary: 

This release introduces intracellular modeling, i.e., models inside individual cells, for PhysiCell. We support three types of intracellular models: boolean networks, ordinary differential equations (ODEs), and dynamic flux balance analysis (dFBA). An intracellular model is part of a cell type's phenotype specification. Currently, we only support a single intracellular model per cell type; however, different *types* of models can be used for different cell types, e.g., a boolean network for cell type A and ODEs for cell type B.

This new functionality has been a collaborative effort with the Institut Curie, the Barcelona Supercomputing Center, and the University of Washington. 
We provide a unified C++ interface between each intracellular model and PhysiCell.

The Systems Biology Markup Language (SBML) is used to define both the ODEs and FBA models; boolean networks are defined using MaBoSS's custom 
configuration (.cfg and .bnd) files. (NOTE: PhysiCell does *not* support the full SBML specification; details are provided elsewhere.)

**NOTE 1:** MacOS users need to define a PHYSICELL_CPP environment variable to specify their OpenMP-enabled g++. See the [Quickstart](documentation/Quickstart.md) for details.

**NOTE 2:** Windows users need to follow an updated (from v1.8) MinGW64 installation procedure. This will install an updated version of g++, plus libraries that are needed for some of the intracellular models. See the [Quickstart](documentation/Quickstart.md) for details.
 
### Major new features and changes:

+ First full support for intracellular models: boolean networks, ordinary differential equations (ODEs), and dynamic flux balance analysis (dFBA).

+ Added an abstract `Intracellular` class in core/PhysiCell_phenotype.h. Concrete classes for the supported intracellular models provide the functionality in the abstract class.

+ Added an `/addons` directory in the root directory. This is where the intracellular concrete classes and code are located.

+ We adopt existing software for intracellular model solvers: MaBoSS for boolean networks, libRoadrunner for ODEs, and Clp for dFBA. However, to make it easier for modelers to use these solvers in PhysiCell, we provide automatic downloads of libraries (see next bullet).

+ If a PhysiCell model uses an intracellular model, the PhysiCell Makefile will run a Python script (in the /beta directory) that checks to see if you have already downloaded the software (library) for the intracellular solver and, if not, downloads it and puts it in a directory within your PhysiCell project where it can be found and linked. The Python script will download the appropriate library for your operating system.

### Minor new features and changes: 

+ Added `intracellular` XML element (inside `phenotype`) that specifies the type of intracellular model, its model definition file, its PhysiCell dt value to be evaluated, and relevant mappings between it and PhysiCell data.

+ Added Python scripts in /beta to download intracellular solver libraries: setup_libroadrunner.py, setup_libmaboss.py, setup_fba.py

+ Added new sample intracellular projects: physiboss_cell_lines, ode_energy, and cancer_metabolism

+ Added parsing of `dt_intracellular` XML element in modules/PhysiCell_settings.cpp (associated with the `intracellular_dt` global parameter in PhysiCell_constants.{h,cpp}). 
However, it is up to each intracellular model as to how, or if, it will be used.

+ Added parsing of `intracellular_data` XML element in modules/PhysiCell_settings.cpp to determine. However, it is not currently used by the intracellular sample models. It may be used for debugging in the future.

+ Updated the [Quickstart](documentation/Quickstart.md) guide, primarily to reflect necessary changes for intracellular solver libraries.

+ Added `UniformInt()` to core/PhysiCell_utilities.cpp (used by intracellular boolean models)

+ Added new functions to ./modules/PhysiCell_geometry to draw (unfilled) circles of cells

+ Added new sample project: celltypes3 

+ Removed sample projects: template2D, template3D 

+ Deleted deprecated code in core/PhysiCell_cell_container.cpp

+ Added a simple Qt GUI for plotting cells (plot_cells.py and vis_tab_cells_only.py in /beta)

+ Bug fix and improvements to /beta/params_run.py to perform parameter explorations of models.

### Beta features (not fully supported):
 
+ Started writing a standardized set of functions for Hill functions and promoter/inhibitor signaling. 

+ [Model Builder Tool](https://github.com/PhysiCell-Tools/PhysiCell-model-builder/releases) 

+ anim_substrate_cmap.py - plot 2D substrates (contours) with a colormap; manually step via arrow keys
  
### Bugfixes: 

+ In core/PhysiCell_cell.cpp, replace `switch` statement with `if`/`else if` to prevent compiler errors related to `static const int` from PhysiCell_constants.

+ core/PhysiCell_cell.cpp: assign_position(double x, double y, double z): make sure the current mechanics voxel is initialized.

+ bug fix to update phenotype geometry when parsing and processing `volume` XML element

+ The Makefile `reset` target now includes a `touch ./core/PhysiCell_cell.cpp` since its `.o` file can have intracellular dependencies.

### Notices for intended changes that may affect backwards compatibility:
 
+ We intend to merge Custom_Variable and Custom_Vector_Variable in the very near future.  

+ We may change the role of operator() and operator[] in Custom_Variable to more closely mirror the functionality in Parameters<T>. 

+ Some search functions (e.g., to find a substrate or a custom variable) will start to return -1 if no matches are found, rather than 0. 
 
+ We will change the timing of when entry_functions are executed within cycle models. Right now, they are evaluated immediately after the exit from the preceding phase (and prior to any cell division events), which means that only the parent cell executes it, rather htan both daughter cells. Instead, we'll add an internal Boolean for "just exited a phase", and use this to exucte the entry function at the next cycle call. This should make daughter cells independently execute the entry function. 

+ We might make "trigger_death" clear out all the cell's functions, or at least add an option to do this. 

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

* * * 
