# PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D Multicellular Systems.

**Version:** 1.8.0

**Release date:** 9 March 2021

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
    pred-prey-farmer
    worm-sample

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

**Quick Start:**  Look at QuickStart.pdf in the documentation folder. 

**User Guide:**   Look at UserGuide.pdf in the documentation folder. 
 
**Tutorials:**    http://www.mathcancer.org/blog/physicell-tutorials/

**Latest info:**  follow [@MathCancer](https://twitter.com/MathCancer) on Twitter (http://twitter.com/MathCancer)

See changes.md for the full change log. 

* * * 
## Release summary: 

This release formally introduces Cell Definitions: a way to fully create cell types in the XML configuration file, including each cell type's initial phenotype and custom variables. This extends our recent work to shift specification of the microenvironment and boundary conditions to XML, and continues our trend towards a future release when many models can be designed and run without compiling any C++ at all. Most of the sample projects have been updated to use this new paradigm, including the a unified 2D/3D `template` project. We recommend using that template as the starting point for any new PhysiCell model.

This release also introduces contact functions: a way to specify cell-cell contact interactions for any cells that you attach (using new, standardized attach and detach functions). Look at the `cancer-biorobots` and `biorobots` sample projects for examples for examples of the newly-introduced, standardized "spring" contact functions. The new `worm` sample project shows a more sophisticated example where cells exchange a differentiation factor across their contacts to model juxtacrine signaling. To help support contact interaction modeling, we include new search functions to easily report a vector of Cells that are nearby for use in your contact interactions. The default mechanics function also records a list of all currently (mechanically) interacting cells in state.neighbors.  

The release also add a number of features to improve the ease of code use: a copy of the XML configuration file is now saved in your output directory to help keep track of what parameters and settings generated the data. We auto-generate a `legend.svg` file (also in output) based on your coloring function and cell defintions. The sample projects' Makefiles now include new rules to create animated GIFs, convert SVG to JPG, create a MP4 movie, and even auto-download the latest version of PhysiCell to update an existing project. A key new feature is the ability to pre-specify cell locations in a CSV file. The template projects will auto-parse this list if enabled to place cells at the start of the simulation. We also provide new functionality to add a virtual wall to the simulation domain to help keep cells from leaving; this can be enabled or disabled from the XML configuration file. 

**NOTE:** OSX users must now define PHYSICELL_CPP system variable. See the documentation.
 
### Major new features and changes:

+ Full rollout of `Cell_Definition` in the XML configuration files. Many basic models can now be fully defined in XML with minimal or no C++. 

+ Unified the 2D and 3D template projects into a single `template` project. 

+ New `predator-prey-farmer` sample project. Prey look for and consume food that's released by farmers. Prey avoid predators, predators hunt and eat prey.  

+ Improved thread safety, particularly when cells ingest cells. 

+ Introduced new cell-cell contact functions with syntax: 

`void contact( Cell* pME, Phenotype& my_phenotype , Cell* pOther, Phenotype& other_phenotype, double dt )`

These are exexcuted once per mechanics time step. Best practice is to either only read `pOther` and `other_phenotype`, or use OMP critical locks if writing data to `pOther` and `other_phenotype`. 

For any cell (`this`), the contact function will be executed for any other cell (other) in `this->state.attached_cells`. The modeler will still need to decide which cells to attach. 

All attached cells are automatically removed when a cell dies or divides. 

+ Added new attachment and detachment functions to the `Cell` class: 
++ `void attach_cell( Cell* pAddMe );` Add `pAddme` to the cell's `state.attached_cells` for use in contact functions. 
++ `void detach_cell( Cell* pRemoveMe );` Remove `pRemoveMe` from the cell's `state.attached_cells` list.  
++ `void remove_all_attached_cells( void );` Remove all attached cells.  

+ Added additional attachment and detachment functions outside the `Cell` class: 
++ `void attach_cells( Cell* pCell_1, Cell* pCell_2 );` Add `pCell_2` to `pCell_1->state.attached_cells` and add `pCell_1` to `pCell_2->state.attached_cells`
++ `void detach_cells( Cell* pCell_1 , Cell* pCell_2 );` Remove the attachments. 

+ Introduced a standardized cell-cell spring-like adhesion contact function: `standard_elastic_contact_function.`

This will add an additional Hookean spring attraction to cells in `state.attached_cells`. The modeler will still need to decide when to attach or detach cells. (Recommended practice: use the `custom` function that is evaluated once per mechanics time step.) 

+ Introduced two new member search funtions for cells to facilitate contact functions: 
++ `std::vector<Cell*> nearby_cells(void)` returns a vector of all cells in the nearby mechanics voxels, excluding the cell (`this`). 
Users should still test the distance to these cells in their interaction functions. 
++ `std::vector<Cell*> nearby_interacting_cells(void)` returns a vector of all cells in the nearby mechanics voxels that are within interaction distance, excluding the cell (`this`). This uses the same distance testing as in the default mechanics functions. 

+ Introduced two new search funtions outside the `Cell` class to facilitate contact functions: 
++ `std::vector<Cell*> nearby_cells(Cell* pCell)` returns a vector of all cells in the nearby mechanics voxels, excluding the cell (`pCell`). 
Users should still test the distance to these cells in their interaction functions. 
++ `std::vector<Cell*> nearby_interacting_cells(Cell* pCell)` returns a vector of all cells in the nearby mechanics voxels that are within interaction distance, excluding the cell (`pCell`). This uses the same distance testing as in the default mechanics functions. 

+ The default mechanics function now automatically updates `state.neighbors` with a list of all cells which had non-zero mechanical interactions in the last mechanics time step. Use this as an inexpensive (``prepaid``) method to find nearby cells for your own contact functions. 

+ Introduced a new sample project `worm` which shows advanced interaction testing and contact testing. Individual cells aggregate based on chemotaxis towards a secreted quorum factor and test for contacts. Cells can form a maximum of `n` (default: 2) attachments with the built-in spring functions. Cells on the ends (1 attachment) hold a steady expression of a differentiation function (`head`). This factor is exchanged between interior cells (2 attachments) to model juxtacrine signaling, using a contact function. End cells determine if they are a head or a tail based by comparing their expresoin with their linked neibhbor. This introduces asymmmetry that allows the "worms" to crawl directionally. 

+ All sample projects now copy the configuration file to the output directory, to help keep track of settings and parameters used to create a simulation result. 

+ Updated the sample projects to use the new Cell_Definitions and contact functions. 

+ Users can now pre-specify cell positions by creating a CSV file: 
++ Each row corresponds to a cell:     x,y,z,typeID
++ TypeID is the integer index of a `Cell_Definition` (ideally defined in XML!)
++ Call the function `load_cells_csv( std::string filename )` to load these possitions and place the cells in corresponding positions. Ideally, cally this function at the end of `setup_tissue()`. The template projects will call this function automatically if a cell CSV file is specified in the `initial_conditions` section of the XML configuration file. 
++ The `template` project already uses this function in the right place. See `worm-sample` for project that uses it. 

### Minor new features and changes: 

+ Cell definitions can now be defined by XML files. See the note above. This functionality may be additionally refined or modified in the next few releases while still in beta. 

+ All sample projects have a `make jpeg` rule that uses ImageMagick to convert SVG snapshots into JPG files. 

+ All sample projects have a `make movie` that uses ffmepg to convert JPG snapshots to an mp4 animation. 

+ The `make upgrade` rule will check for and download the most recent version of PhysiCell, overwriting these core functions (and sample projects) without overwriting your project code. 

+ A new `paint_by_number_cell_coloring` coloring function colors each cell type with a unique color. (Currently there is a maximum of 13 pre-defined colors for 13 cell types.) Apoptotic cells are black, and necrotic cells are brown. 

+ Cycle and Death in the XML `Cell_Definitions` no longer require a `code` as long as the `name` is correct. 

+ Revised template project to a barebones minimum. 

+ Removed beta-testing sample project. 

+ Added functionality to auto-generate an SVG legend based on the currently defined XML funtions and coloring function. 

### Beta features (not fully supported):
 
+ Started writing a standardized set of functions for Hill functions and promoter/inhibitor signaling. 

+ Started creating new functions to fill geometric shapes with cells of a chosen type. 
  
### Bugfixes: 

+ In response to SourceForge ticket #43, fixed the bug where Dirichlet conditions weren't properly applied to individual boundaries. 

+ Cleaned up code as suggested in SourceForge Ticket #42.

### Notices for intended changes that may affect backwards compatibility:
 
+ We intend to merge Custom_Variable and Custom_Vector_Variable in the very near future.  

+ We may change the role of operator() and operator[] in Custom_Variable to more closely mirror the functionality in Parameters<T>. 

+ Some search functions (e.g., to find a substrate or a custom variable) will start to return -1 if no matches are found, rather than 0. 
 
+ We will change the timing of when entry_functions are executed within cycle models. Right now, they are evaluated immediately after the exit from the preceding phase (and prior to any cell division events), which means that only the parent cell executes it, rather htan both daughter cells. Instead, we'll add an internal Boolean for "just exited a phase", and use this to exucte the entry function at the next cycle call. This should make daughter cells independently execute the entry function. 

+ We might make "trigger_death" clear out all the cell's functions, or at least add an option to do this. 

### Planned future improvements: 

+ Further XML-based simulation setup. 

+ Read saved simulation states (as MultiCellDS digital snapshots)
 
+ Integrate SBML-encoded systems of ODEs as custom data and functions for molecular-scale modeling 
  
+ Integrate Boolean network support from PhysiBoSS into the mainline code (See http://dx.doi.org/10.1093/bioinformatics/bty766. )
  
+ Add cell differentiation functionality to Phenotype, to be executed during cell division events. 
 
+ Add a new standard phenotype function that uses mechanobiology, where high pressure can arrest cycle progression. (See https://twitter.com/MathCancer/status/1022555441518338048.) 
 
+ Add module for standardized pharmacodynamics, as prototyped in the nanobio project. (See https://nanohub.org/resources/pc4nanobio.) 
 
+ Create an angiogenesis sample project 
 
+ Create a small library of angiogenesis and vascularization codes as an optional standard module in ./modules (but not as a core component)

+ Improved plotting options in SVG 

+ Further update sample projects to make use of more efficient interaction testing available

+ Major refresh of documentation.

* * * 
