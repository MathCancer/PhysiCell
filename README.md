# PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D Multicellular Systems
**Versions:** 1.13.0 - 

**Release dates:** 29 July 2023 - 
* 1.13.0 : 29 July 2023
* 1.13.1 : 6 August 2023

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
      * physimess-sample

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

**Support:**      https://join.slack.com/t/physicellcomm-sf93727/shared_invite/zt-qj1av6yd-yVeer8VkQaNDjDz7fF00jA 

**Quick Start:**  Look at QuickStart.md in the documentation folder. 

**User Guide:**   Look at UserGuide.pdf in the documentation folder. 
 
**Setup and Training:**	See this year's workshop and hackathon at https://github.com/PhysiCell-Training/ws2023  
 
**Older Tutorials:**    http://www.mathcancer.org/blog/physicell-tutorials/

**Latest info:**  follow [@PhysiCell](https://twitter.com/PhysiCell) on Twitter (http://twitter.com/PhysiCell)

See changes.md for the full change log. 

* * * 

## Release summary: 
Version 1.13.x  \introduces PhysiMeSS (MicroEnvironment Structures Simulation) as a PhysiCell add-on created to model rod-shaped microenvironment elements such as the matrix fibres (e.g. collagen) of the ECM. These releases also introduce numerous bug fixes, particularly to handling of Dirichlet boundary conditions, while introducing numerous minor feature enhancements such as packing and unpacking user projects (to facilitate code sharing). 

### Version 1.13.1 (6 August 2023): 
Version 1.13.1 primarily introduces bug fixes for smoother addon support, as well as new makefile rules to pack a user project for sharing (`make pack PROJ=name`) and to unpack a shared project (`make unpack PROJ=name`). These will create (pack) or expand (unpack) zipped projects in the `./user_projects` folder. To share, send the zipped file and encourage the recipient to store it in their own `./user_projects` folder. 

### Version 1.13.0 (29 July 2023):
Version 1.13.0 introduces PhysiMeSS (MicroEnvironment Structures Simulation) as a PhysiCell add-on created by Cicely Macnamara, Vincent Noël and collaborators, which allows the user to specify rod-shaped microenvironment elements such as the matrix fibres (e.g. collagen) of the ECM. This allows the PhysiCell user the ability to investigate fine-grained processes between cellular and fibrous ECM agents. We are providing an sample project together with this addon to demonstrate, via many examples, the possibilities of PhysiMeSS. For more information, consult the PhysiMeSS README available in [./addons/PhysiMeSS/README.md](./addons/PhysiMeSS/README.md). Version 1.13.0 also updates the bundled PhysiBoSS addon, introduces a variety of bug fixes (particularly in handling of Dirichlet boundary conditions), and improves SVG plots. 

We are grateful for immense contributions by Cicely Macnamara, Vincent Noël, Randy Heiland, Daniel Bergman, Marco Ruscone, Furkan Kurtoglu, and Elmar Bucher in this release. 

**NOTE 1:** MacOS users need to define a PHYSICELL_CPP environment variable to specify their OpenMP-enabled g++. See the [Quickstart](documentation/Quickstart.md) for details.

**NOTE 2:** Windows users need to follow an updated (from v1.8) MinGW64 installation procedure. This will install an updated version of g++, plus libraries that are needed for some of the intracellular models. See the [Quickstart](documentation/Quickstart.md) for details.

### Major new features and changes in the 1.13.z versions
#### 1.13.1 
+ None in this release
#### 1.13.0
+ Introduced PhysiMeSS, a major addon for modeling fibers of the extracellular matrix. Major thanks to Cicely Macnamara, Vincent Noël, and team!

### Minor new features and changes: 
#### 1.13.1 
+ Continued modernization of sample projects for PhysiCell Studio compatibiltiy. See [PR 198](https://github.com/MathCancer/PhysiCell/pull/198).
+ Updated inhibitor behaviors in PhysiBoSS, and further code cleanup.  See [PR 194](https://github.com/MathCancer/PhysiCell/pull/194). Thanks, Marco Ruscone! 
+ PhysiBoSS cell line example migrated to newer MultiCellDS output. See [PR 193](https://github.com/MathCancer/PhysiCell/pull/193). Thanks, Vincent Noël!
+ Added a new makefile rule to simplify sharing user projects: `make pack PROJ=name` will zip all of the `name` user project in `./user_projects/name.zip`. Send this zip file for sharing your project, and have your recipient: 
  1. Place `name.zip` in thier `./user_projects/` folder (preferably PhysiCell version 1.13.1 or later)
  2. Have them run the new rule `make unpack PROJ=name` to expand the project.
  3. After this, the usual rules apply. `make load PROJ=name` to load the project, and a subsequent `make` to compile it. 
+ Added a new makefile rule to simplify use of shared  user projects: `make unpack PROJ=name` will unzip the contents of `./user_projects/name.zip` into a new user project called `name`. Type `make load PROJ=name` to load this project, and `make` to compile it.  
#### 1.13.0
+ Preparations for a new derived `Cell` class for use in PhysiBoSS, including a new `instantiate_cell` function in `Cell_Functions` to help facilitate this. See [PR 153](https://github.com/MathCancer/PhysiCell/pull/153) (Thanks, Vincent Noël!)
+ Various safety refinements (`const` accessors) in vector operations ([PR 160](https://github.com/MathCancer/PhysiCell/pull/160)). Thanks, Vincent Noël! 
+ Made changes to cell SVG plotting to support broader types of plotting in advance of PhysiMeSS [PR 162](https://github.com/MathCancer/PhysiCell/pull/162). Thanks, Vincent Noēl!
+ Added a safe way to query the current velocity via `Basic_Agent::get_previous_velocity()` in preparation for PhysiMeSS. [PR 163](https://github.com/MathCancer/PhysiCell/pull/163). Thanks, Vincent Noël!
+ Refined control of object counts in SVG for upcoming PhysiMeSS release. [PR 164](https://github.com/MathCancer/PhysiCell/pull/164). Thanks, Vincent!
+ Refined SVG plot options to incorporate substrates. [PR 181](https://github.com/MathCancer/PhysiCell/pull/181). Thanks, Marco Ruscone!
+ Updated PhysiBoSS to Version 2.2.1. See [PR 188](https://github.com/MathCancer/PhysiCell/pull/188). Thanks, Vincent Noël! 
+ Updated unit tests (including `custom_DCs_2substrates`)
+ Added `damage rate` (from effector attack) to supported behaviors in the modeling gramamr 
+ minor cleanup 

### Beta features (not fully supported):
#### 1.13.1 
+ The dFBA addon is considered "beta" and unsupported at this time. Compatability work is underway. Thank you, Miguel Ponce de Leon and team! 
#### 1.13.0
+ None in this release. 
 
### Bugfixes: 
#### 1.13.1 
+ Bugfixes to and refinements to the libRoadrunner setup scripts. See [PR 196](https://github.com/MathCancer/PhysiCell/pull/196). Thanks to Randy Heiland and Furkan Kurtoglu. 
+ Updated PHysiBoSS cell line example project to remove “default phenotype” function. See [PR 195](https://github.com/MathCancer/PhysiCell/pull/195). Thanks, Vincent Noël!
+ Fixed default cell constructor to improve backwards compatibility by including new SVG plot function pointers. See [PR 200](https://github.com/MathCancer/PhysiCell/pull/200). Thanks to John Metzcar for catching this and for careful detective work to isolate the cause!
+ Numerous bugfixes to PhysiCell zip distribution, including a better release protocol. Major thanks to Furkan Kurtoglu, Elmar Bucher, John Metzcar, and Randy Heiland for help! 
#### 1.13.0 
+ Fix typographical errors in Makefiles in sample projects.
+ Set correct value (100) of `cell_BM_repulsion_strength` in `PhysiCell_phenotype.cpp` (Thanks, Elmar Bucher!)
+ Improved handling of `voxel_index` in `remove_agent_from_voxel` in preparation for voxel-spanning objects such as PhysiMeSS. [PR 159](https://github.com/MathCancer/PhysiCell/pull/159). Thanks, Vincent Noël!
+ Fixed bug to ensure cell definitions without `intracellular` defined get a `NULL` intracellular model function. [PR 182](and [PR 182](https://github.com/MathCancer/PhysiCell/pull/182). THanks, Marco Ruscone!
+ Fixed a whitespaced bug in SVG output. [PR 179](https://github.com/MathCancer/PhysiCell/pull/179). Thanks, Vincent Noël!
+ Fixed a PhysiBoSS bug where dead cells could execute models. [PR 180](https://github.com/MathCancer/PhysiCell/pull/180) Thanks, Vincent Noël!
+ Fixed bugs involving Dirichlet conditions and multiple substrates (thanks to Daniel Bergman for pointing it out!) See [Issue 124](rf. https://github.com/MathCancer/PhysiCell/issues/124) and [PR 149](https://github.com/MathCancer/PhysiCell/pull/180). Thank you, Daniel Bergman and Randy Heiland! 
+ `cancer_biorobots` Makefille PROGRAM_NAME is now `cancer_biorobots` instead of `project`
+ Deleted a meaningless line `dt;` in PhysiCell_standard_models.cpp
+ Added missing commas to cell_rules.csv in rules_sample project
+ Fixed typo: `PhyisiCell_rules.o` to `PhysiCell_rules.o` in Makefile-default (thanks to Joseph Abrams for pointing it out!)
+ Fixed errors in SBML ODE models. See [PR 185](https://github.com/MathCancer/PhysiCell/pull/185) and [PR 186](https://github.com/MathCancer/PhysiCell/pull/186). Thanks, Furkan Kurtoglu and Vincent Noël!
+ Fixed errors the PhysiBoSS readme. See [PR 187](https://github.com/MathCancer/PhysiCell/pull/187). Thanks, Vincent Noël!

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
