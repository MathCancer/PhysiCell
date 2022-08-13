# PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D Multicellular Systems

**Version:** 1.10.4

**Release date:** 18 July 2022

## Overview: 
PhysiCell is a flexible open source framework for building agent-based multicellular models in 3-D tissue environments.

**Reference:** A Ghaffarizadeh, R Heiland, SH Friedman, SM Mumenthaler, and P Macklin, PhysiCell: an Open Source Physics-Based Cell Simulator for Multicellular Systems, PLoS Comput. Biol. 14(2): e1005991, 2018. DOI: [10.1371/journal.pcbi.1005991](https://dx.doi.org/10.1371/journal.pcbi.1005991)

Visit http://MathCancer.org/blog for the latest tutorials and help. 

**Notable recognition:**
+ [2019 PLoS Computational Biology Research Prize for Public Impact](https://blogs.plos.org/biologue/2019/05/31/announcing-the-winners-of-the-2019-plos-computational-biology-research-prize/)

### Key makefile rules:

**`make`**               : compiles the current project. If no 
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

**`make list-projects`** : list all available sample projects 

**`make clean`**         : removes all .o files and the executable, so that the next "make" recompiles the entire project 

**`make data-cleanup`**  : clears out all simulation data 

**`make reset`**         : de-populates the sample project and returns to the original PhysiCell state. Use this when switching to a new PhysiCell sample project. 

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
Version 1.10.4 primarily fixes bugs in file output and the ode-energy sample, and refines thread safety in cell phagocytosis. 

The 1.10.0 release introduced major new phenotype functionality, including standardized support for cell-cell interactions (phagocytosis, cell attack that increases a tracked damage variable, and cell fusion), cell transformations, advanced chemotaxis, and cell adhesion affinities for preferential adhesion. This release also includes new, auto-generated "dictionaries" of signals and behaviors to facilitate writing cell behavioral models and intracellular models, as well as standardized Hill and linear response functions for use in intracellular models. Lastly, this release includes a number of bugfixes, most notably pseudorandom number generators with improved thread safety. 

A blog post and tutorial on the new phenotype elements can be found at http://www.mathcancer.org/blog/introducing-cell-interactions-and-transformations.  
A blog post and tutorial on the new signal and behavior dictionaries can be found at http://www.mathcancer.org/blog/introducing-cell-signal-and-behavior-dictionaries. 

**NOTE 1:** MacOS users need to define a PHYSICELL_CPP environment variable to specify their OpenMP-enabled g++. See the [Quickstart](documentation/Quickstart.md) for details.

**NOTE 2:** Windows users need to follow an updated (from v1.8) MinGW64 installation procedure. This will install an updated version of g++, plus libraries that are needed for some of the intracellular models. See the [Quickstart](documentation/Quickstart.md) for details.
 
### Major new features and changes in the 1.10.z versions
#### 1.10.4
+ None in this version. See 1.10.0
#### 1.10.3
+ None in this version. See 1.10.0
#### 1.10.2
+ None in this version. See 1.10.0
#### 1.10.1
+ None in this version. See 1.10.0
#### 1.10.0
+ Created `Cell_Interactions` in `Phenotype` as a standard representation of essential cell-cell interactions, including phagocytosis, "attack", and fusion. 
  +  Users can set phagocytosis rates for dead cells via `phenotype.cell_interactions.dead_phagocytosis_rate`. Cells automatically phagocytose live and dead neighbors at each mechancis time step based upon the phagocytosis rates. 
  +  Users can set phagocytosis rates for each live cell type via `phenotype.cell_interactions.live_phagocytosis_rates`. There is one rate for each cell type in the simulation. Cells automatically phagocytose live and dead neighbors at each mechancis time step based upon the phagocytosis rates. Phagocytosis absorbs the target cell's volume and internal contents and flags the target for removal. The cell will eventually shrink back towards its target volume. 
  +  For convenience, the phagocytosis rates can be accessed (read or written) via `phenotype.cell_interactions.live_phagocytosis_rate(name)` where `name` (a `std::string`) is the human-readable name of a cell type. 
  +  Users can set attack rates for live cells via `phenotype.cell_interactions.attack_rates`. There is one rate for each cell type in the simulation. Cells automaticaly attack neighbors at each mechanics time step based upon the rates. An attack event increases the target cell's `cell.state.damage` by `damage_rate * dt_mechanics` and `cell.state.total_attack_time` by `dt_mechanics`. It is up to the scientist user to set additional hypotheses that increases cell death with accumulated damage or attack time. 
  +  For convenience, the attack rates can be accessed via `phenotype.cell_interactions.attack_rate(name)` where `name` (a `std::string`) is the human-readable name of a cell type.   
  +  Users can set fusion rates for live cells via `phenotype.cell_interactions.fusion_rates`. There is one rate for each cell type in the simulation. Cells automaticaly fuse with at each mechanics time step based upon the rates. Fusion will merge the two cells' volumes and internal contents, add their nuclei (recorded in `cell.state.number_of_nuclei`), and move the combine cell to the prior center of volume. The combined cell's new target volume is the sum of the two original cells' target volumes. 
  +  For convenience, the fusion rates can be accessed via `phenotype.cell_interactions.fusion_rate(name)` where `name` (a `std::string`) is the human-readable name of a cell type.   

+ Created `Cell_Transformations` in `Phenotype` as a standard representation of cell transformations such as differentation or transdifferentiation. 
  +  Users can set transformation rates for each live cell type via `phenotype.cell_transformations_transformation_rates`. There is one rate for each cell type in the simulation. Cells automatically attempt to transform to these types at each phenotype time step based upon the phagocytosis rates. 
  +  For convenience, the transformation rates can be accessed (read or written) via `phenotype.cell_transformations.transformation_rate(name)` where `name` (a `std::string`) is the human-readable name of a cell type. 

+ Updated `Cell_State` to track the number of nuclei (for fusion), total damage (e.g., for cell attack) and total attack time. 

+ Added a new `advanced_chemotaxis` function with data stored in `phenotype.motility` to allow chemotaxis up a linear combination of gradients. 
  + `cell.phenotype.motility.chemotactic_sensitivities` is a vector of chemotactic sensitivies, one for each substrate in the environment. By default, these are all zero for backwards compatibility. A positive sensitivity denotes chemotaxis up a corresponding substrate's gradient (towards higher values), whereas a negative sensitivity gives chemotaxis against a gradient (towards lower values). 
  + For convenience, you can access (read and write) a substrate's chemotactic sensitivity via `phenotype.motility.chemotactic_sensitivity(name)`, where `name` is the human-readable name of a substrate in the simulation. 
  + If the user sets `cell.cell_functions.update_migration_bias = advanced_chemotaxis_function`, then these sensitivities are used to set the migration bias direction via  `d_mot = sensitivity_0 * grad(rho_0) + sensitivity_1 * grad(rho_1) + ... + sensitivity_n * grad(rho_n)`. 
  + If the user sets `cell.cell_functions.update_migration_bias = advanced_chemotaxis_function_normalized`, then these sensitivities are used to set the migration bias direction via  `d_mot = sensitivity_0 * |grad(rho_0)| + sensitivity_1 * |grad(rho_1)| + ... + sensitivity_n * |grad(rho_n)|.` 

+ Added a new `adhesion_affinities` to `phenotype.mechanics` to allow preferential adhesion.
  + `cell.phenotype.mechanics.adhesion_affinities` is a vector of adhesive affinities, one for each cell type in the simulation. By default, these are all one for backwards compatibility.  
  + For convenience, you can access (read and write) a cell's adhesive affinity for a specific cell type via `phenotype.mechanics.adhesive_affinity(name)`, where `name` is the human-readable name of a cell type in the simulation. 
  + The standard mechanics function (based on potentials) uses this as follows. If cell `i` has an cell-cell adhesion strength `a_i` and an adhesive affinity `p_ij` to cell type `j` , and if cell `j` has a cell-cell adhesion strength of `a_j` and an adhesive affinity `p_ji` to cell type `i`, then the strength of their adhesion is `sqrt(  a_i p_ij a_j p_ji )`. Notice that if `a_i = a_j` and `p_ij = p_ji`, then this reduces to `a_i a_pj`. 
  + The standard elastic spring function (`standard_elastic_contact_function`) uses this as follows. If cell `i` has an elastic constant `a_i` and an adhesive affinity `p_ij` to cell type `j` , and if cell `j` has an elastic constant `a_j` and an adhesive affinity `p_ji` to cell type `i`, then the strength of their adhesion is `sqrt(  a_i p_ij a_j p_ji )`. Notice that if `a_i = a_j` and `p_ij = p_ji`, then this reduces to `a_i a_pj`. 

+ `PhysiCell_basic_signaling` now includes standard Hill and linear response functions: 
   + `Hill_response_function( double s, double half_max , double hill_power )` is a Hill function responding to signal `s` with a half-max of `half_max` and Hill coefficient of `hill_power`. We note that this function is an order of magnitude faster when the `hill_power` is an integer (e.g., 1 or 2) rather than a non-integer power (e.g., 1.4). 
   + `double linear_response_function( double s, double s_min , double s_max )` is a linear ramping from 0.0 (for inputs `s` below `s_min`) to 1.0  (for inputs `s` above `s_max`). The outputs are clamped to the range [0,1].
   + `double decreasing_linear_response_function( double s, double s_min , double s_max )` is a linear ramping from 1.0 (for inputs `s` below `s_min`) to 0.0  (for inputs `s` above `s_max`). The outputs are clamped to the range [0,1].

+ We introduced a "dictionary" of standard signals that can be used as inputs to intracellular and rule-based models. This dictionary is automatically constructed at the start of each simulation based upon the combinations of signaling substrates and cell types. 
  + Major classes of signals include:
    + extracellular and intracellular substrate concentrations
    + substrate gradients
    + contact with dead cells
    + contact with cells (of type X)
    + damage
    + pressure
    + Use `display_signal_dictionary()` to quickly display a list of available signals. 
  + Substantial functionality to query signals
    + `int find_signal_index( std::string signal_name )` : get the index of the named signal 
    + `std::vector<int> find_signal_indices( std::vector<std::string> signal_names );` get a vector of indices for a vector of named signals 
    + `std::string signal_name( int i );` display the name of the signal with the given index 
    + `std::vector<double> get_signals( Cell* pCell );` get a vector of all known signals for the cell 
    + `std::vector<double> get_cell_contact_signals( Cell* pCell );` get a vector of the cell contact associated signals for the cell 
    + `std::vector<double> get_selected_signals( Cell* pCell , std::vector<int> indices );` get a vector of signals for the cell, with the supplied indices
    + `std::vector<double> get_selected_signals( Cell* pCell , std::vector<std::string> names );` get a vector of signals for the cell, with the supplied human-readable names of the signals 
    + `double get_single_signal( Cell* pCell, int index );` get a single signal for the cell with the indicated index
    + `double get_single_signal( Cell* pCell, std::string name );` get a single signal for the cell with the indicated human-readable name 

+ We introduced a "dictionary" of standard behaviors that can be used as outputs to intracellular and rule-based models. This dictionary is automatically constructed at the start of each simulation based upon the combinations of signaling substrates and cell types. 
  + Major classes of behaviors include:
    + secretion, secretion target, uptake, and export rates 
    + cycle progression
    + death rates
    + motility parameters 
    + chemotactic parameters 
    + cell-cell adhesion and repulsion parameters
    + cell adhesion affinities 
    + cell-BM adhesion and repulsion parameters
    + phagocytosis rates 
    + attack rates
    + fusion rates
    + transformation rates 
    + Use `display_behavior_dictionary()` to quickly see a list of posible behaviors.
  + Substantial functionality to query and set behaviors 
    + `int find_behavior_index( std::string response_name )` : get the index of the named behavior 
    + `std::vector<int> find_behavior_indices( std::vector<std::string> behavior_names )` get the indices for the given vector of behavior names. 
    + `std::string behavior_name( int i );` get the name of the behavior with the given index 
    + `std::vector<double> create_empty_behavior_vector();` create an empty vector for the full set of behaviors 
    + `void set_behaviors( Cell* pCell , std::vector<double> parameters );` write the full set of behaviors to the cell's phentoype
    + `void set_selected_behaviors( Cell* pCell , std::vector<int> indices , std::vector<double> parameters );` write the selected set of behaviors (with supplied indices) to the cell's phenotype
    + `void set_selected_behaviors( Cell* pCell , std::vector<std::string> names , std::vector<double> parameters );` write the selected set of behaviors (with supplied names) to the cell's phenotype 
    + `void set_single_behavior( Cell* pCell, int index , double parameter );` write a single behavior (by index) to the cell phentoype
    + `void set_single_behavior( Cell* pCell, std::string name , double parameter );` write a single behavior (by name) to the cell phentoype
  + Substantial functionality to query the cell's current behavior
    + `std::vector<double> get_behaviors( Cell* pCell );` get all the cell's current behaviors 
    + `std::vector<double> get_behaviors( Cell* pCell , std::vector<int> indices );` get a subset of behaviors (with given indices) 
    + `std::vector<double> get_behaviors( Cell* pCell , std::vector<std::string> names );` get a subset of behaviors (with given names)
    + `double get_single_behavior( Cell* pCell , int index );` get a single behavior (by index)
    + `double get_single_behavior( Cell* pCell , std::string name );` get a single behavior (by name)
  + Substantial functionality to query the cell's referece behaviors (from its cell definition) 
    + `std::vector<double> get_base_behaviors( Cell* pCell );` get all the cell's base behaviors 
    + `std::vector<double> get_base_behaviors( Cell* pCell , std::vector<int> indices );` get a subset of base behaviors (with given indices) 
    + `std::vector<double> get_base_behaviors( Cell* pCell , std::vector<std::string> names );` get a subset of base behaviors (with given names)
    + `double get_single_base_behavior( Cell* pCell , int index );` get a single base behavior (by index)
    + `double get_single_base_behavior( Cell* pCell , std::string name );` get a single base behavior (by name)

+ Created a new `interaction-sample` project to illustrate the new interactions and transformations: 
  + Blood vessels release resource
  + Virulet bacteria colonize near vessels (by chemotaxis up towards a secreted quorum factor and resource)
  + Stem cells divide and differentiate into differentiated cells
  + Differentiated cells divide until experiencing elevated pressure (to detect confluence)
  + Bacteria-secreted virulence factor kills stem and differentiated cells. Dead cells release debris.
  + Macrophages chemotax towards quorum factor and debris and secrete pro-inflammatory factor in presence of dead cells or bacteria
  + Macrophages phagocytose dead cells
  + CD8+ T cells chemotax towards pro-inflamatory factor and attack bacteria
  + Neutrophils chemotax towards pro-inflammatory factor and phagocytose live bacteria
  + Accumulated damage kills bacteria. 
  + With default parameters, bacteria kill off cells ot form abscesses, until death attracts macrophages to activate immune response to kill the invaders, after which the tissue can regrow. 

### Minor new features and changes: 
#### 1.10.4
+ None in this version. 
#### 1.10.3
+ Added `attachment_rate` and `detachment_rate` to `phenotype.mechanics` for use in a future standard attachment and detachment model. 
+ Modernized output format: 
  + More complete cell data saved for each cell agent. 
  + Merged the previously separate cell matlab files for each time save
  + Added more metadata to outputs
+ `Variables` and `Vector_Variables` in `Custom_Cell_Data` now have a new Boolean attribute `conserved_quantity` (defaulted to false). If this value is set to true, then the custom variable is divided evenly between daughter cells at division. 
+ Custom cell data can now be designated as conserved by settings an attribute `conserved="true"` in the XMO configuration file. 
+ Improved support for Apple M1 and M2 chips. 
+ Refinements to PhysiBoSS. 

#### 1.10.2
+ Added `operator<<` for vectors of ints and vectors of strings. So that `std::cout << v << std::endl;` will work if `v` is `std::vector<int>` of `std::vector<std::string>`. It was truly annoying that these were missing, so sorry!
+ Added `dead` to the signals dictionaries, which returns 0.0 or 1.0 based on `phenotype.death.dead`. 
+ Added `time` to the signals dictionaries, which returns the current simulation time based on `PhysiCell_Globals.current_time`. 
+ Added a brief protocol on how to add new signals and behaviors to the dictionaries in the `/protocols` directory. 
+ Added new functions `double& apoptosis_rate()` and `double& necrosis_rate()` to easily read and write these rates. Access via `cell.phenotype.death.apoptosis_rate()` and `cell.phenotype.death.necrosis_rate()`. 
+ Added new ease of access functions for secretion: 
  + `double& Secretion::secretion_rate( std::string name )` allows you to easily read/write the secretion rate of a substrate by name. For example: 
	```pCell->phenotype.secretion.secretion_rate("oxygen") = 0.1```
  + `double& Secretion::uptake_rate( std::string name )` allows you to easily read/write the uptake rate of a substrate by name. For example: 
	```pCell->phenotype.secretion.uptake_rate("oxygen") = 0.1```
  + `double& Secretion::saturation_density( std::string name )` allows you to easily read/write the secretion target of a substrate by name. For example: 
	```pCell->phenotype.secretion.saturation_density("oxygen") = 38```
  + `double& Secretion::net_export_rate( std::string name )` allows you to easily read/write the net export rate of a substrate by name. For example: 
	```pCell->phenotype.secretion.net_export_rate("oxygen") = -100```

+ Added new ease of access function for internalized substrates: 
  + `double& Molecular::internalized_total_substrate( std::string name )` allows you to easily read/write the total amount of internalized substrate by name. For example: 
	```pCell->phenotype.molecular.internalized_total_substrate( "oxygen" ) = 0.01``
#### 1.10.1
+ None in this version. See 1.10.0. 
#### 1.10.0
+ All sample projects have a new rule "make name" to tell you the name of the executable. 

+ All sample projects output the executable name to screen for easier reference. 

+ `Cell_Definition` has a new Boolean `is_movable`, so that all cells of a type can be set to non-movable. (Default: `is_movable = true`;) This allows you to use agents as rigid objects or barriers. 

+ `create_cell( Cell_Definition )` now uses "`is_movable`" from the cell definition.  

### Beta features (not fully supported):
#### 1.10.4
+ None in this version. 
#### 1.10.3
+ Each time outputs two cell interaction graphs (as text files): 
  + neighbor graph records which cells are within interaction distance for each cell agent, with format; 
    ID: ID1, ID2, ID3, ... (Cell ID: and the IDs of interacting cells)
  + attached cell graph records which cells are attached for each cell agent, with format; 
    ID: ID1, ID2, ID3, ... (Cell ID: and the IDs of attached cells)
  + We might split these into 3 files: 
    + an ID list that has the ID of each cell in order of appearence. 
    + neighbor list omits the preceding "ID:" since now each row corresponds to the index in the ID list
    + attached cell list omits the preceding "ID:" since now each row corresponds to the index in the ID list
  + Began experimenting with a planned `integrity` subclass to `phenotype` that will record multiple types of cell damage and associated damage and repair rates. It is not yet clear if we wil provide built-in support for damaged-driven apoptotic death and cycle arrest, as these are generally better left to modeler-driven hypotheses. 

None in this version. See 1.10.0. 
#### 1.10.2
+ None in this version. See 1.10.0.  
#### 1.10.1
 + None in this version. See 1.10.0. 
 #### 1.10.0
+ Started writing a standardized set of functions for Hill functions and promoter/inhibitor signaling. 

+ [Model Builder Tool](https://github.com/PhysiCell-Tools/PhysiCell-model-builder/releases) 

+ Added a simple Qt GUI for plotting cells only (plot_cells.py and vis_tab_cells_only.py in /beta)

+ Added a simple Qt GUI for plotting substrates and cells (plot_data.py and vis_tab.py in /beta)

+ Added simple contour plotting of a substrate (anim_substrate2D.py in /beta; copy to /output) 
  
### Bugfixes: 
#### 1.10.4
+ Fixed vectorized outputs in MultiCellDS that incorrectly assumed linear data ordering in std::vector. Thank you Randy Heiland! 
+ Fixed errors in the ode-energy-sample project. Thank you Randy Heiland, Furkan Kurtoglu, and John Metzcar!
+ Improved thread safety in phagocytosis. Thank you Michael Getz! 

#### 1.10.3
+ Fixed bug in `get_single_behavior` and `get_single_base_behavior` where querying any cycle exit rate or cycle entry mistakenly returned -1. 
+ Corrected declaration of `standard_add_basement_membrane_interactions` in `PhysiCell_standard_models.h` to properly use phenotype by reference. Thank you Inês Gonçalves!
+ Removed the OpenMP pragma in `void Microenvironment::apply_dirichlet_conditions( void )` (around line 272) that tends to perform more poorly than serial code. 

#### 1.10.2
+ Fixed error in `double get_single_behavior()` where the `cell-cell adhesion elastic constant` behavior was not found.  

+ Fixed error in `double get_single_base_behavior()` where the `cell-cell adhesion elastic constant` behavior was not found.  

+ Fixed bug in `add_PhysiCell_cells_to_open_xml_pugi()` that mistakenly used the wrong size (number of cell species rather than number of substrate species) when writing the chemotactic sensitivities. 

+ The cell `neighbors` list did not add non-adhesive cells within interaction distance. This is now fixed. 

#### 1.10.1
+ XML parsing has been made more robust to "survive" using an incorrect substrate in the `chemotactic_sensitivities` section.

+ Missing PhysiBoSS makefiles have been replaced. 

+ Fixed broken makefile for worms sample project. 

#### 1.10.0
+ When the `cell_defaults` definition has been altered, new cell types may unwittingly copy nonzero parameter values from this default. Now, immediately after copying `cell_defaults`, the XML parsing will reset motility to off (with `NULL` function for bias direction), reset all secretion/uptake/export to zero, reset all cell interactions and transformations to zero. It will then continue to parse the XML file. Set `legacy_cell_defaults_copy = true` in the config file to override this bugfix. 

+ We refactored the pseudorandom number generator (at the basis of `UniformRandom()`) to improve thread safety. Previously, all threads shared a single PRNG, which was not thread safe. For newer fast processors with many threads, this could lead to sufficiently many "collisions" to introduce subtle biases in some cases (particularly for purely Brownian motion that is not dominated by chemotaxis, proliferation, and other behaviors). This is now corrected by creating a PRNG for each thread, each with its own seed. We used `std::seed_seq` to determinstically set a good spread of seeds to prevent correlation between the PRNGs, with the convention that the 0th thread's seed is either the user-specified seed or a random seed. This preserves original single-thread behavior from prior versions. 

+ Random motility now uses `UniformOnUnitCircle()` (in 2D) and `UniformOnUnitSphere()` (in 3D) to choose the random component of the migration direction, rather than hand-coding selection of the random vector. 

+ In response to PR 91 (https://github.com/MathCancer/PhysiCell/pull/91): Previoulsy, if the make jpeg rule fails, the `__*.txt` temporary files are left in place, so a subsequent "make jpeg" fails until these files are manually removed. Replacing `>>` (append) with `>` (overwrite) fixes the problem. Thanks [saikiRA1011](https://github.com/saikiRA1011)!

### Notices for intended changes that may affect backwards compatibility:
+ We intend to deprecate the unused phenotype variables `relative_maximum_attachment_distance`, `relative_detachment_distance`, and `maximum_attachment_rate` from `phenotype.mechanics.` 

+ We intend to merge `Custom_Variable` and `Custom_Vector_Variable` in the very near future.  

+ We may change the role of `operator()` and `operator[]` in `Custom_Variable` to more closely mirror the functionality in `Parameters<T>`. 

+ Some search functions (e.g., to find a substrate or a custom variable) will start to return -1 if no matches are found, rather than 0. 
 
+ We will change the timing of when `entry_function`s are executed within cycle models. Right now, they are evaluated immediately after the exit from the preceding phase (and prior to any cell division events), which means that only the parent cell executes it, rather than both daughter cells. Instead, we'll add an internal Boolean for "just exited a phase", and use this to execute the entry function at the next cycle call. This should make daughter cells independently execute the entry function. 

+ We might make `trigger_death` clear out all the cell's functions, or at least add an option to do this. 

### Planned future improvements: 

+ Further XML-based simulation setup. 

+ Read saved simulation states (as MultiCellDS digital snapshots)
 
+ Add a new standard phenotype function that uses mechanobiology, where high pressure can arrest cycle progression. (See https://twitter.com/MathCancer/status/1022555441518338048.) 
 
+ Add module for standardized pharmacodynamics, as prototyped in the nanobio project. (See https://nanohub.org/resources/pc4nanobio.) 
 
+ Create an angiogenesis sample project 
 
+ Create a small library of angiogenesis and vascularization codes as an optional standard module in ./modules (but not as a core component)

+ Improved plotting options in SVG 

+ Further update sample projects to make use of more efficient interaction testing available

+ Major refresh of documentation.
