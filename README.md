# PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D Multicellular Systems

**Versions:** 1.14.0 - 

**Release dates:** 15 September 2024 - 
* 1.14.0 : 15 September 2024
* 1.14.1 : 13 December 2024
* 1.14.2 : 20 January 2025

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
      * custom-division-sample
      * asymmetric-division-sample
      * immune-function-sample episode-sample

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
**Homepage:**     http://PhysiCell.org

**Setup Guide:**  https://github.com/physicell-training/ws2023/blob/main/agenda.md#set-up-physicell 

**Downloads:**    https://PhysiCell.sf.net AND https://github.com/MathCancer/PhysiCell/releases 

**Support:**      https://join.slack.com/t/physicellcomm-sf93727/shared_invite/zt-qj1av6yd-yVeer8VkQaNDjDz7fF00jA 

**User Guide:**   Look at UserGuide.pdf in the documentation folder. 
 
**Setup and Training:**	See this year's workshop and hackathon at https://github.com/PhysiCell-Training/ws2023  
 
**Older Tutorials:**    http://www.mathcancer.org/blog/physicell-tutorials/

**Latest info:**  follow [@PhysiCell](https://twitter.com/PhysiCell) on Twitter (http://twitter.com/PhysiCell)

See changes.md for the full change log. 

* * * 

## Release summary: 
Version 1.14 upgrades the Cell Beheavior Hypothesis Grammar (to version 3), including refinements to cell phagocytosis, effector attack, and cell damage/integrity in response to community discussions and peer review. It also introduces numerous refinements to cell division, random seeds, and randomized parameter initialization, as well as upgrades to PhysiBoSS and PhysiMeSS and bug fixes. Other refinements are "under the hood," including new GitHub actions and improved automation of testing, as well as improvements to MultiCellDS output. 

### Version 1.14.2 (20 Jan 2025): 
Version 1.14.2 primarily introduces bugfixes and stability refinements, closer matching to the cell behavior grammar (including the new `transition to X` synonym for `transform to X` behavior and better support for asymmetric division), a new script to more easily download PhysiCell Studio, and improvements to allow parallel "episodes" of PhysiCell in machine learning environments, such as the upcoming PhysiGym addon. 

We are grateful for contributions by Vincent Noël, Randy Heiland, Daniel Bergman, Heber Rocha, and Elmar Bucher in this release. 

### Version 1.14.1 (13 Dec 2024): 
Version 1.14.1 primarily introduces bug fixes as noted below, but also introduces the first implementation of asymmetric division. 

Among the notable changes includes a more consistent handling of internalized substrates and conserved custom data on transformation and phagocytosis: they are now conserved in these processes. Several other features and changes are included (see below) as well as additional bug fixes (major and minor). Finally, the test suite will no test on MacOS 13 instead of the now-deprecated-in-GitHub-Actions MacOS 12.

Please report any bugs or issues in Issues or in the PhysiCell community Slack workspace. Also, feel free to suggest new features in either location as well.

We are grateful for contributions by Vincent Noël, Randy Heiland, Daniel Bergman, Heber Rocha, and Elmar Bucher in this release. 

### Version 1.14.0 (15 Sep 2024):
Version 1.14.0 Introduces Cell Behavior Hypothesis Grammar (CBHG) 3.0, enhancing the modeling of cellular behaviors with the addition of a new `Cell_Integrity` class and refined phagocytosis behaviors (now split into separate rates for apoptotic, necrotic, and other dead cells). The built-in "attack" model has been refined to include formation of a persistent synapse (with a spring adhesion) throughout the attack (which is tunable via the `attack_duration` parameter), and a clarified `attack_damage_rate` to denote the rate at which an attacker damages its target cell. The attacking cell also tracks how long it has attacked (may be useful for exhaustion modeling), whether it is or is not attacking, and the identity (cell pointer) of the cell it is attacking. 

The new `Cell_Integrity` class (within `Phenotype`) allows more control over cell damage. Attacking cells (see above) can increase damage, as well as a new generalized `damage_rate` that can (for example) be used to model damage from other sources such as cytotoxic drugs or toxins. A built-in model for damage repair (with default rate `damage_repair_rate` = 0) can be used for simple modeling of damage repair (e.g., DNA damage response during a cycle damage checkpoint). 

This release also includes an option to set the random number generator seed value, new capabilities to draw initial parameters from a random distribution, and support for user-defined custom functions the evaluated during cell division (which allow users to individually set properties of daughter cells, such as during asymmetric division). Beyond bug fixes, the release includes a systematic testing package, utilizing scripts and GitHub Actions for automated testing.

We are grateful for contributions by Vincent Noël, Randy Heiland, Daniel Bergman, Heber Rocha, and Elmar Bucher in this release. 

**NOTE 1:** MacOS users need to define a PHYSICELL_CPP environment variable to specify their OpenMP-enabled g++. See the [Setup Guides](https://github.com/physicell-training/ws2023/blob/main/agenda.md#set-up-physicell) for details.

**NOTE 2:** Windows users need to follow an updated (from v1.8) MinGW64 installation procedure. This will install an updated version of g++, plus libraries that are needed for some of the intracellular models. See the [Setup Guides](https://github.com/physicell-training/ws2023/blob/main/agenda.md#set-up-physicell) for details.

### Major new features and changes in the 1.14.z versions
#### 1.14.2
+ In anticipation with the upcoming `PhysiGym` addon (for machine learning / reinforcement learning), it is now possible to run multiple consecutive episodes from a single PhysiCell model within a runtime. The episode sample project demonstrates this possibility.

#### 1.14.1 
+ asymmetric division is now possible through the config file
  + try the sample project with make `asymmetric-division-sample`
  + on division, one (and only one) of the daughter cells can be assigned a new cell type
  + set probabilities for each cell type in the config file
  + control these probabilities with rules using the behavior `asymmetric division to [cell_type]`
+ create full path to output folder if it does not exist
+ write `random_seed` to `output/random_seed.txt` for reproducibility even when using `system_clock` for setting the seed
+ copy the rules file(s) to the output folder and write the parsed rules (v3) `to cell_rules_parsed.csv` in the output folder
+ preserve internalized substrates and conserved custom data on cell transformation
+ default to 100% (instead of 0%) of internalized substrates being transferred on phagocytosis
+ transfer `conserved` custom data on phagocytosis from eaten to eater cell

#### 1.14.0 
+ Introduced changes to Rules:
  + `damage rate` (a part of `Cell_Integrity`) is now a generalized term for a rate of damage caused by non-attack means 
  + `attack damage rate` means what `damage rate` used to mean: how fast an attacking cell deals damage to a target cell throughout the duration of the atttack
  + `phagocytose dead cell` is replaced by death-model-specific rates:
    + `phagocytose apoptotic cell`
    + `phagocytose necrotic cell`
    + `phagocytose other dead cell`
+ New `Cell_Integrity` class in PhysiCell_phenotype.h. `damage` was moved from Cell_State into Cell_Integrity 
  + the cancer-biorobots-sample `custom.cpp` was updated to reflect this change.
+ `contact with dead cell` has been supplemented with additional (refined) signals `contact with apoptotic cell`, `contact with necrotic cell`, and `contact with other dead cell`
+ Seed for random numbers: in the top most <options> tag of a config file (for options that apply to the overall simulation), there is now a <random_seed>. Traditionally, this has been provided in <user_parameters> and if it is still present there, it will override the one in <options>. Users are encouraged to migrate away from its use in <user_parameters> as this will likely be removed from sample projects in a future release.
  + Setting as an integer will have the same behavior as the `user_parameter`
      + <random_seed>0</random_seed>
  + Setting as “”, “random”, or “system_clock” will use the system clock to set the random seed
      + <random_seed />
      + <random_seed></random_seed>
      + <random_seed>random</random_seed>
      + <random_seed>system_clock</random_seed>
+ New option for a user-defined custom function for cell division. If provided, the custom function will receive pointers to the two daughter cells. A new sample project, `custom-division-sample`, is provided.
+ Initial parameter distributions
  + Users can now start cells with heterogeneity in any behavior or also the total volume
  + For ease of access, in studio navigate to Cell Types > Misc > Parameter Distributions
  + Five distributions supported:
    + Uniform
      + Set min and max; behavior ~ U(min, max)
    + Log uniform
      + Set min and max; z ~ U(log(min), log(max)); behavior ~ exp(z)
      + Note: min and max are on the behavior scale, not the logarithmic scale
    + Normal
      + Set μ and σ; behavior ~ N(μ, σ)
      + Optionally set lb, ub to impose lb <= behavior <= ub
    + Log normal
      + Set μ and σ; z ~ N(μ, σ); behavior ~ exp(z)
      + Optionally set lb, ub to impose lb <= behavior <= ub
      + Note: μ and σ are on the logarithmic scale
      + Note: lb and ub are on the behavior scale
    + Log10 normal
      + Same as log normal, except behavior ~ 10^z
      + Implemented because log10 values are more human-interpretable
  + Can enforce that the base value is within the distribution to help constrain parameter sweeps
  + “Enable” attributes make it easy to toggle on/off individual distributions or for an entire cell type
+ MultiCellDS update:
  + PhysiBoSS intracellular data is now part of data export
  + Spring attachments are now part of data export
  + Streamlined MultiCellDS with incorporation of more single-cell parameters/state variables
+ Update to PhysiBoSS 2.2.3
  + Added steepness parameter to output mapping, controlling the Hill coefficient used.
  + Added use_for_dead parameter to input and output mapping, to define if this mapping should be used on dead cells.
  + Added three new sample projects: 
    + template_BM: adaptation of the template project of PhysiCell, with PhysiBoSS support
    + physiboss-tutorial: three toy models presented in the PhysiBoSS tutorial ([10.48550/arXiv.2406.18371](https://doi.org/10.48550/arXiv.2406.18371)).
    + physiboss-tutorial-invasion: update of the cancer invasion model by Ruscone et al., also presented in the PhysiBoSS tutorial.
+ Update to PhysiMeSS 1.0.1
  + Most parameters are now defined in custom_data, to make them specific to a cell definition. This introduces the possibility to have multiple types of fibers.
+ Introducing experimental pre-compiled binaries, available via python beta/download_binary.py.
+ Non-monotonic rules: a single signal can now both cause an increase *and* a decrease in a behavior for a cell type (bringing the implementation in better compliance with the specification at https://www.biorxiv.org/content/10.1101/2023.09.17.557982) 
+ Initialize substrate initial conditions using a .mat or .csv file
  + implemented in PhysiCell Studio; see output there for formatting of the csv
+ Substrate heatmaps on SVGs improvements:
  + set colormaps in the config file
  + set the svg substrate color function by default for config-only based implementation

### Minor new features and changes: 
#### 1.14.2
- [PR349](https://github.com/MathCancer/PhysiCell/pull/349) (minor improvement): `load_PhysiCell_config_file()` was split into `load_PhysiCell_config_file()` and `read_PhysiCell_config_file()` functions
- [PR349](https://github.com/MathCancer/PhysiCell/pull/349) (minor improvement): `BioFVM BioFVM_microenvironment::initialize_microenvironment()` was split into `initialize_microenvironment()` and `set_microenvironment_initial_condition()` functions.
- [PR349](https://github.com/MathCancer/PhysiCell/pull/349) (minor improvement): In `BioFVM/BioFVM_MultiCellDS.*`, a new `reset_BioFVM_substrates_initialized_in_dom()` function was added.
- [PR349](https://github.com/MathCancer/PhysiCell/pull/349) (minor improvement): A new `BioFVM::BioFVM_basic_agent::reset_max_basic_agent_ID()` function was added. 
- Switched `setup_cell_rules( void )` to output the full list of signals and behaviors with synonyms in `./output/dictionaries.txt` for fuller reference. 
- Added new functions:
  - `void display_signal_dictionary_with_synonyms( std::ostream& os )`
  - `void display_response_dictionary_with_synonyms( std::ostream& os )`
- Added `transition to X` and `transition to cell type N` as synonyms for the behavior `transform to X` (with synonym `transform to cell type X`), at the request of the cancer community who regard `transformation` as synonymous with `cancerous transformation`. (And `transition to X` is now the "primary" name for the behavior.) 
- [PR352](https://github.com/MathCancer/PhysiCell/pull/352) (minor improvement): more robust macro to check for windows machines when creating directories
- [PR353](https://github.com/MathCancer/PhysiCell/pull/353) (minor feature): Python script to download latest release of Studio (and create /studio).

  To use it, go to the root directory and run:

  ```
  python beta/get_physicell.py
  ```

  and then to run studio (with the template project):

  ```
    make reset && make template && make
    python studio/bin/studio.py & 
  ```
  
#### 1.14.1
- PhysiBoSS PDFs removed from repo, links provided in tutorial README.md
- build binaries on release `published` instead of `created`
- `beta/download_binary.py` gets PhysiCell Version 1.14.1
- add reference to PhysiMeSS article
- `make immune-function-sample` to make sample project showing new (1.14.0) phagocytosis and attack modules
- add `rules_sample` to tests
- test and build on macos-13 instead of macos-12 as that is being deprecated by GitHub actions

#### 1.14.0 
+ Scripts in `/beta` to help with testing, both manually and via GitHub Actions: `test_build_samples.sh` and `test*.py`
+ The Makefiles for all sample projects now do a recursive copy (`cp -r`) for files in the /config directory
+ throw error if duplicate substrate or user_parameter name found
 
### Bugfixes: 
#### 1.14.2 
+ [PR350](https://github.com/MathCancer/PhysiCell/pull/350) (minor fix): use standard save event triggers in asymmetric division example
+ [PR351](https://github.com/MathCancer/PhysiCell/pull/351) (minor fix): re-round template project cycle durations

#### 1.14.1
- store value of `attack_duration` when parsing config file
- set rules to Version 3.0 for all projects
- store the `conserved` boolean when parsing `custom_data` in the config
- when a cell dies, make sure it is removed from `neighbors` lists for all neighboring cells
- use `std::stoul` in place of `std::stoi` to properly read in large ints for random seeds
- use `std::ostringstream` to write voxel coordinates to avoid overflowing the buffer
- fix `custom_division` `sample_project` so it does not require `random_seed` as a `user_parameter`
- actually record `applies_to_dead` boolean for exported rules
- initialize `total_attack_time = 0.0;` to avoid random initial values
- update all sample projects to 1.14.x
  - update phagocytosis parameters
  - update attack parameters
- remove extraneous call to `SeedRandom()` in PhysiBoSS cell lines project
- do not let random detachment occur when one cell is attacking another

#### 1.14.0 
+ `sample_projects_intracellular/ode/ode_energy/main.cpp` was updated to use `save_PhysiCell_to_MultiCellDS_v2`
+ `Cell::convert_to_cell_definition` now retains the cell volume
+ fix bug in storing rules that occasionally resulted in seg faults

### Notices for intended changes that may affect backwards compatibility:
+ Future releases may further refine `Cell_Integrity` with more specific forms of damage (and accompanying damage and repair rates).

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
  
+ Create an angiogenesis sample project 
 
+ Create a small library of angiogenesis and vascularization codes as an optional standard module in ./modules (but not as a core component)

+ Further update sample projects to make use of more efficient interaction testing available

+ Major refresh of documentation.
