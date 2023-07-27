# PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D Multicellular Systems
**Versions:** 1.13.0 - 

**Release dates:** 28 July 2023 - 
* 1.13.0 : 28 July 2023 (proposed)

## Overview: 
PhysiCell is a flexible open source framework for building agent-based multicellular models in 3-D tissue environments.

**Reference:** A Ghaffarizadeh, R Heiland, SH Friedman, SM Mumenthaler, and P Macklin, PhysiCell: an Open Source Physics-Based Cell Simulator for Multicellular Systems, PLoS Comput. Biol. 14(2): e1005991, 2018. DOI: [10.1371/journal.pcbi.1005991](https://dx.doi.org/10.1371/journal.pcbi.1005991)

Visit http://MathCancer.org/blog for the latest tutorials and help. 

**Notable recognition:**
+ [2019 PLoS Computational Biology Research Prize for Public Impact](https://blogs.plos.org/biologue/2019/05/31/announcing-the-winners-of-the-2019-plos-computational-biology-research-prize/)

### Key makefile rules:

**`make`**: compiles the current project. If no 
                     project has been defined, it first 
                     populates the cancer heterogeneity 2D 
                     sample project and compiles it 
   
**`make project-name`**: populates the indicated sample project. 
                     Use "make" to compile it. 

   * **`project-name`** choices:
      * template 
      * biorobots-sample 
      * cancer-biorobots-sample 
      * cancer-immune-sample
      * celltypes3-sample 
      * heterogeneity-sample 
      * pred-prey-farmer 
      * virus-macrophage-sample 
      * worm-sample
      * ode-energy-sample 
      * physiboss-cell-lines-sample 
      * cancer-metabolism-sample
      * interaction-sample
      * mechano-sample
      * rules-sample

**`make list-projects`** : list all available sample projects 

**`make clean`**         : removes all .o files and the executable, so that the next "make" recompiles the entire project 

**`make data-cleanup`**  : clears out all simulation data 

**`make reset`**         : de-populates the sample project and returns to the original PhysiCell state. Use this when switching to a new PhysiCell sample project. 

**`make save PROJ=name`**: save the current project (including the `Makefile`, `main.cpp`, and everything in `./config` and `./custom_modules/`) in `./user_projects/name`, where `name` is your choice for the project. If the project already exists, overwrite it. 

**`make load PROJ=name`**: load the user project `name` from `./user_projects/name` (including the `Makefile`, `main.cpp`, and everything in `./config` and `./custom_modules/`).  

**`make list-user-projects`**: list all user projects in `./user_projects/`. (Use these names without the trailing `/` in `make load PROJ=name`.)

**`make jpeg`**          : uses ImageMagick to convert the SVG files in the output directory to JPG (with appropriate sizing to make movies). Supply `OUTPUT=foldername` to select a different folder. 

**`make movie`**         : uses ffmpeg to convert the JPG files in the output directory an mp4 movie. Supply `OUTPUT=foldername` to select a different folder, or `FRAMERATE=framerate` to override the frame rate.

**`make upgrade`**       : fetch the latest release of PhysiCell and overwrite the core library and sample projects. 

### Key Links 
**Homepage:**     http://PhysiCell.MathCancer.org 

**Downloads:**    http://PhysiCell.sf.net

**Support:**      https://sourceforge.net/p/physicell/tickets/

**Quick Start:**  Look at QuickStart.md in the documentation folder. 

**User Guide:**   Look at UserGuide.pdf in the documentation folder. 
 
**Setup and Training:**	See last year's workshop and hackathon at https://github.com/PhysiCell-Training/ws2021  
 
**Older Tutorials:**    http://www.mathcancer.org/blog/physicell-tutorials/

**Latest info:**  follow [@PhysiCell](https://twitter.com/PhysiCell) on Twitter (http://twitter.com/PhysiCell)

See changes.md for the full change log. 

* * * 

## Release summary: 
Version 1.13.0 introduced ... **text here**



**NOTE 1:** MacOS users need to define a PHYSICELL_CPP environment variable to specify their OpenMP-enabled g++. See the [Quickstart](documentation/Quickstart.md) for details.

**NOTE 2:** Windows users need to follow an updated (from v1.8) MinGW64 installation procedure. This will install an updated version of g++, plus libraries that are needed for some of the intracellular models. See the [Quickstart](documentation/Quickstart.md) for details.

### Major new features and changes in the 1.13.z versions
#### 1.13.0
+ Introduced PhysiMeSS, a major addon for modeling fibers of the extracellular matrix. Major thanks to Cicely Macnamara, Vincent Noël, and team!

### Minor new features and changes: 
#### 1.13.0
+ Preparations for a new derived `Cell` class for use in PhysiBoSS, including a new `instantiate_cell` function in `Cell_Functions` to help facilitate this. See [PR 153](https://github.com/MathCancer/PhysiCell/pull/153) (Thanks, Vincent Noël!)
+ Various safety refinements (`const` accessors) in vector operations ([PR 160](https://github.com/MathCancer/PhysiCell/pull/160)). Thanks, Vincent Noël! 
+ Made changes to cell SVG plotting to support broader types of plotting in advance of PhysiMeSS [PR 162](https://github.com/MathCancer/PhysiCell/pull/162). Thanks, Vincent Noēl!
+ Added a safe way to query the current velocity via `Basic_Agent::get_previous_velocity()` in preparation for PhysiMeSS. [PR 163](https://github.com/MathCancer/PhysiCell/pull/163). Thanks, Vincent Noël!
+ Refined control of object counts in SVG for upcoming PhysiMeSS release. [PR 164](https://github.com/MathCancer/PhysiCell/pull/164). Thanks, Vincent!
+ Refined SVG plot options to incorporate substrates. [PR 181](https://github.com/MathCancer/PhysiCell/pull/181). Thanks, Marco Ruscone!

### Beta features (not fully supported):

#### 1.13.0
+ None in this release. 
 
### Bugfixes: 

#### 1.13.0 
+ Fix typographical errors in Makefiles in sample projects.
+ Set correct value (100) of `cell_BM_repulsion_strength` in `PhysiCell_phenotype.cpp` (Thanks, Elmar Bucher!)
+ Improved handling of `voxel_index` in `remove_agent_from_voxel` in preparation for voxel-spanning objects such as PhysiMeSS. [PR 159](https://github.com/MathCancer/PhysiCell/pull/159). Thanks, Vincent Noël!
+ Fixed bug to ensure cell definitions without `intracellular` defined get a `NULL` intracellular model function. [PR 182](and [PR 182](https://github.com/MathCancer/PhysiCell/pull/182). THanks, Marco Ruscone!
+ Fixed a whitespaced bug in SVG output. [PR 179](https://github.com/MathCancer/PhysiCell/pull/179). Thanks, Vincent Noël! 

### Notices for intended changes that may affect backwards compatibility:
+ We intend to deprecate the unused phenotype variables `relative_maximum_attachment_distance`, `relative_detachment_distance`, and `maximum_attachment_rate` from `phenotype.mechanics.` 

+ We intend to merge `Custom_Variable` and `Custom_Vector_Variable` in the future.  

+ We may change the role of `operator()` and `operator[]` in `Custom_Variable` to more closely mirror the functionality in `Parameters<T>`. 

+ Additional search functions (e.g., to find a substrate or a custom variable) will start to return -1 if no matches are found, rather than 0. 
 
+ We will change the timing of when `entry_function`s are executed within cycle models. Right now, they are evaluated immediately after the exit from the preceding phase (and prior to any cell division events), which means that only the parent cell executes it, rather than both daughter cells. Instead, we'll add an internal Boolean for "just exited a phase", and use this to execute the entry function at the next cycle call. This should make daughter cells independently execute the entry function. 

+ We might make `trigger_death` clear out all the cell's functions, or at least add an option to do this. 

+ We might change the behavior of copied Custom Data when a cell changes type (changes to a new cell definition). Currently, all custom data elements in a cell are overwritten based on those in the new cell definition. This is not the best behavior for custom data elements that represent state variables instead of type-dependent parameters. 

### Planned future improvements: 

+ Further XML-based simulation setup. 

+ Read saved simulation states (as MultiCellDS digital snapshots)
 
+ Add a new standard phenotype function that uses mechanobiology, where high pressure can arrest cycle progression. (See https://twitter.com/MathCancer/status/1022555441518338048.) 
  
+ Create an angiogenesis sample project 
 
+ Create a small library of angiogenesis and vascularization codes as an optional standard module in ./modules (but not as a core component)

+ Improved plotting options in SVG 

+ Further update sample projects to make use of more efficient interaction testing available

+ Major refresh of documentation.
