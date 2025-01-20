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

* * * 

# PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D Multicellular Systems
**Versions:** 1.13.0 - 1.13.1

**Release dates:** 29 July 2023 - 15 September 2024
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
Version 1.13 introduces PhysiMeSS (MicroEnvironment Structures Simulation) as a PhysiCell add-on created to model rod-shaped microenvironment elements such as the matrix fibres (e.g. collagen) of the ECM. These releases also introduce numerous bug fixes, particularly to handling of Dirichlet boundary conditions, while introducing numerous minor feature enhancements such as packing and unpacking user projects (to facilitate code sharing). 

## Release summary: 
Version 1.13.x  introduces PhysiMeSS (MicroEnvironment Structures Simulation) as a PhysiCell add-on created to model rod-shaped microenvironment elements such as the matrix fibres (e.g. collagen) of the ECM. These releases also introduce numerous bug fixes, particularly to handling of Dirichlet boundary conditions, while introducing numerous minor feature enhancements such as packing and unpacking user projects (to facilitate code sharing). 

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

* * * 

# PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D Multicellular Systems
**Versions:** 1.12.0 - 1.12.0

**Release dates:** 15 May 2023 - 15 May 2023
* 1.12.0 : 15 May 2023

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

**Support:**      https://sourceforge.net/p/physicell/tickets/

**Quick Start:**  Look at QuickStart.md in the documentation folder. 

**User Guide:**   Look at UserGuide.pdf in the documentation folder. 
 
**Setup and Training:**	See last year's workshop and hackathon at https://github.com/PhysiCell-Training/ws2021  
 
**Older Tutorials:**    http://www.mathcancer.org/blog/physicell-tutorials/

**Latest info:**  follow [@PhysiCell](https://twitter.com/PhysiCell) on Twitter (http://twitter.com/PhysiCell)

See changes.md for the full change log. 

* * * 

## Release summary: 
Version 1.12.0 introduces rules-based modeling: human-interpretable statements of the form 

> In cell type T, signal S increases/decreases behavior B

are represented with a CSV format that can _directly_ and _uniquely_ map onto a Hill response function to auto-generate simulation code. `T` is any cell type in the simulation, `S` can be any signal in the signal dictionary, and `B` any supported behavior in the behavior dictionary. For example: 

* In malignant epithelial cells, pressure decreases cycle entry. 
* In M0 macrophages, necrotic debris increases transformation to M1 macrophage. 
* In effector T cells, contact with malignant epithelial cell decreases migration speed. 
* In effector T cells, IFN-gamma increases attack of malignant epithelial cells. 

The CSV version of these statements can be parsed and transformed into code dynamically at runtime, without additional user-written C++ or recompiling. This will be the basis of a pre-compiled PhysiCell Studio (model design, execution, and visualization in one package) and similar PhysiCell Cloud (install-free, browser-based model design, execution, and visualization). This allows modelers to focus on choosing their hypotheses--how signals (stimuli) change cell behavior--and less on coding and debugging. It is our hope that this language is sufficiently expressive to write most models without additional user code. However, users can still write custom phenotype functions that can be integrated with rules-based modeling, allowing further fine-tuning of individual cell behavior. 

**NOTE 1:** MacOS users need to define a PHYSICELL_CPP environment variable to specify their OpenMP-enabled g++. See the [Quickstart](documentation/Quickstart.md) for details.

**NOTE 2:** Windows users need to follow an updated (from v1.8) MinGW64 installation procedure. This will install an updated version of g++, plus libraries that are needed for some of the intracellular models. See the [Quickstart](documentation/Quickstart.md) for details.
 
### Major new features and changes in the 1.12.z versions
#### 1.12.0
+ **Rules-based modeling:** See introduction above. 

+ **Automated annotation of the model hypotheses:** Upon parsing the rules, PhysiCell auto-generates HTML-formatted text annotating all model hypotheses, for use in a paper's method section. This is to encourage better model interoperability and reproducibility. 

+ **CSV-based specification of model rules:**
  + Version 1: 
    ```cell_type , signal , direction , behavior , base_value , max_response_value , half_max , Hill_power , applies_to_dead```

    + `cell_type`: The (human-readable) name of any cell type in the simulation, matching their declarations in the XML configuration file. 
      + Allowed values: Any named cell type in the simulation. 
    + `signal`: Any signal in the simulation's signal dictionary that can be queried to modulate a behavior. 
      + Allowed values: Any signal that is known to the signal dictionary. 
    + `direction`: Tells whether the signal increases or decreases the behavior. 
      + Allowed values: `increases` or `decreases` 
    + `behavior`: Any behavioral parameter in the simulation's behavior dictionary that can be edited to modulate a behavior. 
      + Allowed values: Any behavioral parameter that is known in the behavior dictionary. 
    + `base_value`: The value of the behavioral parameter in the absence of any signals 
      + Allowed value: Must match the behavior's parameter value in the cell definition 
    + `max_response_value`: The maximally changed behavior when acting under high values of signal
      + Allowed values (for rules that increase the behavior): Any positive value equalling or exceeding the `base_value`. E.g., ten times the base value. 
      + Allowed values (for rules that decrease the behavior): Any positive value equal to or less than the `base_value`. E.g., one tenth the base value. 
    + `half_max`: Value of the signal at which the behavior undergoes half of its maximal change. 
      + Allowed values: Non-zero positive numbers. 
    + `Hill_power`: The Hill coefficient in a Hill response function. 
      + Allowed values: Any non-zero positive number. **Integer values are MUCH more computationally efficient.** 
    + 'applies_to_dead': Indicates if the rule should also be applied to dead cells. 
      + Allowed values: 0 (for false) or 1 (for true).    

  + Version 2: 
    ```cell_type , signal , direction , behavior , max_response_value , half_max , Hill_power , applies_to_dead```
    This version always copies the `base_values` from the corresponding cell definition. 
    + `cell_type`: The (human-readable) name of any cell type in the simulation, matching their declarations in the XML configuration file. 
      + Allowed values: Any named cell type in the simulation. 
    + `signal`: Any signal in the simulation's signal dictionary that can be queried to modulate a behavior. 
      + Allowed values: Any signal that is known to the signal dictionary. 
    + `direction`: Tells whether the signal increases or decreases the behavior. 
      + Allowed values: `increases` or `decreases` 
    + `behavior`: Any behavioral parameter in the simulation's behavior dictionary that can be edited to modulate a behavior. 
      + Allowed values: Any behavioral parameter that is known in the behavior dictionary. 
    + `max_response_value`: The maximally changed behavior when acting under high values of signal
      + Allowed values (for rules that increase the behavior): Any positive value equalling or exceeding the `base_value`. E.g., ten times the base value. 
      + Allowed values (for rules that decrease the behavior): Any positive value equal to or less than the `base_value`. E.g., one tenth the base value. 
    + `half_max`: Value of the signal at which the behavior undergoes half of its maximal change. 
      + Allowed values: Non-zero positive numbers. 
    + `Hill_power`: The Hill coefficient in a Hill response function. 
      + Allowed values: Any non-zero positive number. **Integer values are MUCH more computationally efficient.** 
    + `applies_to_dead`: Indicates if the rule should also be applied to dead cells. 
      + Allowed values: 0 (for false) or 1 (for true).    

+ Support for both rules-based behavior and traditional phenotype functions: If both are specified, then rules-based phenotype are applied first, followed by user-supplied phenotype functions that can further fine-tune cell behavior (as needed). 

+ Code-free model specification by PhysiCell Studio. 

+ Updated PhysiBoSS to remove cell definition "inheritance," (with "flat", self-standing cell definitions), to make it compatible with PhysiCell Studio. Hereafter, all properties of each cell definition must be explicitely defined. 

+ New section in `PhysiCell_settings.xml` to indicate a rules CSV file file: 
```
<cell_rules>
    <rulesets>
        <ruleset protocol="CBHG" version="2.0" format="csv" enabled="true">
            <folder>./config</folder>
            <filename>cell_rules.csv</filename>
        </ruleset>
    </rulesets>
    <settings />
</cell_rules>
```
  + `protocol`: This value should always be `CBHG` (cell behavior hypothesis grammar)
  + `version`: Use `0.0` (or none for pre-beta files, but migrate away from this.) Use `1.0` for v1 rules as specified above. Use `2.0` for v2 rules as specified above. 
  + `format`: For now, only `csv` is supported. 
  + `enabled`: Set `true` to apply the rules, and `false` otherwise. 
  * `folder`: Set the folder containing the rules file. This should typically be `./config`. 
  * `filename`: Set the name of the rules file. e.g., `cell_rules.csv`. 

### Minor new features and changes: 
#### 1.12.0
+ Added new functions to `PhysiCell_basic_signaling`: 
  + `multivariate_Hill_response_function` combines multiple signals (`std::vector<double> signals`) with individual half-maxes (`std::vector<double> half_maxes`) and Hill powers (`std::vector<double> hill_powers`) into a multivariate Hill response function, such that if only supplied with a single nonzero signal, then it returns the regular single-variable Hill function for that corresponding signal. 

  + `multivariate_linear_response_function` combines multiple signals (`std::vector<double> signals`) with independent minimal thresholds (`std::vector<double> min_thresholds`: values below which individual linear responses are zero) and maximum thresholds (`std::vector<double> max_thresholds )`: values above which individual linear responses are one) into a multivariate linear response, such that if only supplied with a single nonzero signal, then it returns the regular single-variable linear response function for that corresponding signal. This function is "capped" between 0 and 1. 
 
  + `linear_response_to_Hill_parameters` determines a half-maximum and Hill power to approximate a linear response function (with minimum threshold `s0` and maximum threshold `s1`) with a Hill response function. 

  + `Hill_response_to_linear_parameters` determins minimum and maximum thresholds to approximate a Hill response function (with half-maximum `half_max` and Hill power `double Hill_power`) with a linear response function.

+ Added `double get_single_base_behavior( Cell_Definition* pCD , std::string name )` to `PhysiCell_signal_behavior` to extract single base behaviors directly from a `Cell_Definition`. 

+ Added `double get_single_base_behavior( Cell* pCD , std::string name )` to `PhysiCell_signal_behavior` to extract single base behaviors directly from a cell's corresponding `Cell_Definition`. 

+ PhysiCell outputs `dictionary.txt` at runtime with the current list of known signals and behaviors (for use in rules-based modeling). 

+ `BioFVM_vector` now includes `double dot_product( std::vector<double>& a , std::vector<double>& b );` for a standardized dot product. 

+ `BioFVM_vector` now includes `std::vector<double> cross_product( std::vector<double>& a , std::vector<double>& b );` for a standardized cross product. 

+ Added new `rules-sample` sample project to demonstrate rules-based modeling. It's a "toy model" with tumor cells, macrophages, and T cells. 

+ Updated sample projects for compatibility. 

+ Added `make list-user-projects` rule to Makefile to list all user projects available for loading 

### Beta features (not fully supported):
#### 1.12.0
+ None in this release. 
 
### Bugfixes: 

#### 1.12.0
+ None in this release. 

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

* * * 

# PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D Multicellular Systems
**Versions:** 1.11.0 - 1.11.0 

**Release dates:** 20 March 2023 - 20 March 2023
* 1.11.0 : 20 March 2023

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
Version 1.11.0 adds several notable features, fixes bugs, and further expands the "signals" and "behaviors" that can be read and written with a simple API to facilitate building models. In particular, we add a brand new CSV format for initial cell positions (with more robust naming of cells by their human-readable names, a "header" line, and ability to extensively add and specificy individual cell properties), a new ability to save and load user projects in the `user_projects` directory, automated dynamic formation and breakage of spring-based cell-cell adhesions (based upon the cell-cell adhesion affinities, attachment rates, and detachment rates), automated inclusion of spring-based adhesions (at the mechanics time step) without need for the user to explicitly supply a spring function, a new "mechano" sample project to illustrate the new automated spring functionality, and updates to PhysiBoSS to ensure compatibility with the rapidly improving PhysiCell Studio. In addition, there is new capability of adding a background coloring (e.g., an oxygen heatmap) to SVG ouptuts--see the `interaction-sample` for an illustration (use the alternate XML config file to enable). This release includes several bugfixes, the most critical of which is to update the parameters for necrotic cells (which had previously been misset in the XML files, thus disabling necrotic cell lysis and shrinking). 

**NOTE 1:** MacOS users need to define a PHYSICELL_CPP environment variable to specify their OpenMP-enabled g++. See the [Quickstart](documentation/Quickstart.md) for details.

**NOTE 2:** Windows users need to follow an updated (from v1.8) MinGW64 installation procedure. This will install an updated version of g++, plus libraries that are needed for some of the intracellular models. See the [Quickstart](documentation/Quickstart.md) for details.
 
### Major new features and changes in the 1.11.z versions
#### 1.11.0
+ New and improved (v2) cell CSV format for cell import. This allows more intuitive statement of initial cell positions. The first line of your CSV file must be: 

   ```x,y,z,cell type```

  Every subsequent line is a single cell, now referencing cell types by their human-readable names (as defined in your XML configuration file) rather than requiring the integer `ID`. So, a sample second line to place a CD8 T cell at (30,-10,12) would be: 

   ```30,-10,12,CD8 T cell```

  Moreover, the new format allows you to initialize a variety of individual cell properties, including (total) `volume` and any supported cell beheavior. For example, if your cell definitions have custom variables `GFP` ond `oncoprotein`, then you can extend the first header line to: 

  ```x,y,z,cell type,custom:GFP,custom:oncoprotein```

  And then subsequent cells look like this: 

    ```30,-10,12,CD8 T cell,0.5,3.2```
 
  You can tell our parser to skip specifying a specific variable with `s` or `S` or an empty entry. Here, the first cell would skip writing the initial value of the GFP, and teh second would skip initializing the oncoprotein: 

    ```30,-10,12,CD8 T cell,,3.2```  
    ```50,13,-4,M0 Macrophage,0.5,s```

  We will continue to automatically support older CSV cell files; any cells CSV file missing the first line of headers will be processed in the old format.  

+ Ability to save and load user projects
   + Use `make save PROJ=project_name` to save your project to a folder in `./user_projects` named `project_name`. For example: 

       ```make save PROJ=new_tumor_sample```

     saves your project as `new_tumor_sample`. In particular, it saves your `Makefile`, `main.cpp`, everything in `./config`, and everything in `./custom_modules`. 

   + Use `make load PROJ=project_name` to load your project from a folder in `./user_projects` named `project_name`. For example: 

        ```make load PROJ=new_tumor_sample```

     loads your project from `new_tumor_sample`. In particular, it loads your `Makefile`, `main.cpp`, everything in `./config`, and everything in `./custom_modules`. 
     
+ Extended `cell_interactions` to include a vector `immunogenicities`: for a cell, `immunogenicity[j]` is how immunogenic this cell is to the jth cell type. By default, these will all be set to 1. (See next point.) 

+ Updated the built-in "attack" model: 
  $$\textrm{Probability cell } i \textrm{ attacks cell } j \textrm{ in } [t,t+\Delta t] = \textrm{attack}\_{ij} \cdot \textrm{immunogenicity}\_{ji} \Delta t $$ 
  By setting $\textrm{immunogenicity}\_{ji} = 1$ as teh default value, we maintain compatibiltiy with prior models. This is a way to further modulate immunogenic and cytotoxic interactions. 

+ Began migrating built-in sample projects to be compatible with the model builder GUI and the upcoming PhysiCell Studio, including: 
  + template
  + biorobots (updates spring constant from 0.05 to 0.5)
  + heterogeneity
  + cancer biorobots (updates spring constant from 0.05 to 0.5)

+ Added new signals: 
  + `apoptotic` returns 1 if a cell is apoptotic, and 0 otherwise 
  + `necrotic` returns 1 if a cell is necrotic, and 0 otherwise 

  As always, access these via `double get_single_signal(Cell* pCell,std::string sig_name)`. 

+ Added new behaviors: 
  + `immunogenicity to [cell type]` is the cell's immunogenicity to a specific cell type. The probability that cell `i` attacks cell `j` in $[t,t+\Delta t]$ is $\textrm{attack}\_{ij} \cdot \textrm{immunogenicity}\_{ji} \Delta t.$
  + `cell attachment rate` is the rate at which the cell forms spring links with other cells. 
  + `cell detachment rate` is the rate at which spring links break. 
  + `maximum number of cell attachments` is the maximum number of spring links. 
  + `is_movable` can be set to 0 (false) to make an agent rigid: it will exert forces on other cells, but it itself cannot be moved. This behavior right now is somewhat fragile if used dynmaically, but can reliably be used during tissue setup. 

  As always, access these via `double get_single_behavior(Cell* pCell,std::string beh_name)` and `void set_single_behavior(Cell* pCell,std::string beh_name,double new_value)`.

+ Added new standard model `void dynamic_attachments(Cell*, Phenotype& ,double);` This function can automate dynamic attachments and detachments. When calling this function for cell $i$: 
  + For each current attachment, it detaches with probability $\textrm{detachment rate}\_i \Delta t$ 
  + For each cell $j$ in the neighbors list, it forms an attachment with probability 
  
    $$\textrm{Prob attach } i \textrm{ to cell } j = \textrm{adhesion affinity}\_j \cdot \textrm{attachment rate}\_i \cdot \Delta t.$$
    
    The attachment is only formed if both cell $i$ and $j$ have not exceeded their maximum number of 
    attachments. 

+ Added a new `spring_attachments` (of type `std::vector<Cell*>`) to cell `state` to track automated formation and removal of spring-link adhesions separately of the user-focused `attached` data struture. This will allow users to continue managing the `attachments` structure on their own for custom contact functions, without interference from automated springs (see more below).

+ Added new standard model `void dynamic_spring_attachments(Cell*, Phenotype& ,double);` This functions exactly as the `dynamic_attachments` function, except it stores attached cells to `cell.state.spring_attachments` to avoid interfering with the user-managed `cell.state.attachments` data struture. 

+ **Automated spring attachments / detachments:** the new `dynamic_spring_attachments` function is automatically called at every mechancis time step, with cell-cell spring attachment and detachment based on the cells' current rates. Each cell evaluates spring-like elastic adhesion betwen itslef and cells in `cell.state.spring_attachments` to add to its own velocity. Some notes: 
  + Each cell automatically removes all its spring attachments during division 
  + Each cell automatically removes all its spring attachments at the *end* of death. If you want dead cells to have increased detachment, add a rule accordingly using the built-in behavior dictionary. 
  + If a cell is not movable (`is_movable = false`), then it is not moved by springs, but it can exert spring forces on other cells, allowing it to act as an "anchor". 
  + This automated spring functionality is completely independent of (and does not interfere with) the user-defined contact function and user-manageed `cell.state.attached` data structure. 
  + **WARNING:** If in a past life you set `phenotype.mechanics.attachment_rate` to a nonzero rate, you may find yourself surprised with unintended spring adhesions as this new automation kicks in. Please review and revise your configuration file as necessary. 
  + You can disable this behavior in the XML configuration file: 
  ```
      <options>
          <!-- other options -->
          <disable_automated_spring_adhesions>true</disable_automated_spring_adhesions>
      </options>

+ Added a new `mechano-sample` project that shows automated dynamic attachment and detachment of cells: 
  + Constant cancer cell birth and death
  + Basic mechano feedback: high-pressure sets cancer cell birth to zero
  + Cancer cell phenotype sets high detachment rate upon death. 
  + Automated connection of cancer, basement membrane (BM) agents with spring links using the built-ins noted above. No user intervention or code required beyond setting nonzero rates. 
  + Cancer cells manually set to apoptose at 10000 min. 

+ Updated PhysiBoSS to remove cell definition "inheritance," (with "flat", self-standing cell definitions), to make it compatible with PhysiCell Studio. Hereafter, all properties of each cell definition must be explicitely defined. 

### Minor new features and changes: 
#### 1.11.0
+ Updated the `paint_by_number_cell_coloring` coloring function to paint the entire cell white if apoptotic, and brown if necrotic. (Previously, we colored the nucleus based on live/dead status.) This improves compatibility with the model GUI. 

+ Changed the default value of `attachment_rate` from 10 to 0 (in the `Mechanics` class) to avoid unexpected triggering of automated spring adheions. 

+ Added a safety check to `operator[]` for Parameters, based on [PR145](https://github.com/MathCancer/PhysiCell/pull/145/). Thanks, Vincent Noel!! 

+ In PhysiBoSS, introduced a new state inheritance mechanism (global, and node-specific). 

+ PhisBoSS has a new optional start time, to initiate the intracellular model at t > 0.

+ Updated PhysiBoSS Cell Lines sample project (flatten XML, initial positions as CSV).

+ Started combining change log into a more compact format: Each release family (1.y.z, such as 1.10.z) receives an extended entry with new changes grouped by minor release. This allows major releases to be grouped with subsequent minor feature enhancements and bug fixes, for a much shorter change log that's easier to read. README will document all changes of the current release family. 

### Beta features (not fully supported):
#### 1.11.0
+ Added `bool read_microenvironment_from_matlab( std::string mat_filename )` to `BioFVM_MultiCellDS`. This will read and overwrite from a stored microenvironment (in `mat_filename`, saved as a level 4 Matlab file) if it has the following format: 
  + Number of columns = number of voxels (must match the size as configured in the `PhysiCell_settings.xml` file)
  + Number of rows = 3 + 1 + number of diffusing substrates (must match the size and ordering as configured in the `PhysiCell_settings.xml` file)
  + Row 0: x coordinate of each voxel
  + Row 1: y coordinate of each voxel 
  + Row 2: z coordinate of each voxel
  + Row 3: volume of each voxel 
  + Rows j to end: value of (j-4)th substrate in each voxel 
  + Column ordering: 
    * For each z from low to high:
      * For each y from low to high: 
        * for each x from low to high: 
          store voxel X[i] , Y[j], Z[k]. 
  + **Note:** This matches how PhysiCell saves the microenvironment. This will read any PhysiCell-saved microenvironment, so long as its sizes matches your current settings.  
  + **Note:** This may be fragile. It has only minimal error-checking. 

  + Set default cell attachment rate to 0 in the template project and most sample projects to avoid unexpectedly triggering the new autmoated spring adhesions; users must affirmatively set a nonzero attachment rate to trigger this new automation in an individual cell. 

  + In repsonse to [PR 123](https://github.com/MathCancer/PhysiCell/pull/123), `parameters.TYPE.find_index(search_name)` now returns -1 if the searched term isn't found. Thanks, Daniel Bergman! 
 
### Bugfixes: 

#### 1.11.0
+ Fixed bug in cancer biorobots project that mistakenly set max cancer cell proliferation rate to 0.000072 instead of 0.00072

+ Fixed multiple broken signals/behaviors. 

+ Fixed calcification bug reported in issue [133](https://github.com/MathCancer/PhysiCell/issues/133). Thank you, @JulianoGianlupi! 

+ Fixed typo in cell signals that used `contact with dead dell` instead of `contact with dead cell`

+ Changed default full data output to 60 minutes (to match the SVG output interval) for better compatibility with the model builder GUI. 

+ Fixed incorrect parameters for necrotic cell volume changes that prevented necrotic cell lysis and shrinkage. 

+ Merged Daniel Bergman's [PR 126](https://github.com/MathCancer/PhysiCell/pull/126), which fixes cell legend colors. Thank's Daniel! 

+ Improved safety checks in the cell orientation function, thanks to Randy Heiland's [PR 122](https://github.com/MathCancer/PhysiCell/pull/122). Thanks, Randy!

+ Now forcing Mersenne Twister as random generator in PhysiBoSS (use or /dev/random by MaBoSS would max out system descriptor)

+ MaBoSS BND/CFG parsing is now in an OpenMP critical block (flex/bison parser is not thread safe)

+ Remove duplicate initialization of maximum attachment rate from the Phenotype.Mechanics constructor.

+ Fixed bug in neighbor/attached graph output filenames (previously double-appended a suffix to the filenames). 

### Notices for intended changes that may affect backwards compatibility:
+ We intend to deprecate the unused phenotype variables `relative_maximum_attachment_distance`, `relative_detachment_distance`, and `maximum_attachment_rate` from `phenotype.mechanics.` 

+ We intend to merge `Custom_Variable` and `Custom_Vector_Variable` in the future.  

+ We may change the role of `operator()` and `operator[]` in `Custom_Variable` to more closely mirror the functionality in `Parameters<T>`. 

+ Additional search functions (e.g., to find a substrate or a custom variable) will start to return -1 if no matches are found, rather than 0. 
 
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

* * * 

# PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D Multicellular Systems

**Versions:** 1.10.0 - 1.10.4

**Release dates:** 13 May 2022 - 18 July 2022
* 1.10.0 : 13 May 2022
* 1.10.1 : 15 May 2022
* 1.10.2 : 24 May 2022
* 1.10.3 : 25 June 2022
* 1.10.4 : 18 July 2022

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
## Release summaries:
The 1.10.z releases introduced major new phenotype functionality, including standardized support for cell-cell interactions (phagocytosis, cell attack that increases a tracked damage variable, and cell fusion), cell transformations, advanced chemotaxis, and cell adhesion affinities for preferential adhesion. This release also includes new, auto-generated "dictionaries" of signals and behaviors to facilitate writing cell behavioral models and intracellular models, as well as standardized Hill and linear response functions for use in intracellular models. Lastly, this release includes a number of bugfixes, most notably pseudorandom number generators with improved thread safety. 

**NOTE 1:** MacOS users need to define a PHYSICELL_CPP environment variable to specify their OpenMP-enabled g++. See the [Quickstart](documentation/Quickstart.md) for details.

**NOTE 2:** Windows users need to follow an updated (from v1.8) MinGW64 installation procedure. This will install an updated version of g++, plus libraries that are needed for some of the intracellular models. See the [Quickstart](documentation/Quickstart.md) for details.

### 1.10.4 (18 July 2022):  
Version 1.10.4 primarily fixes bugs in file output and the ode-energy sample, and refines thread safety in cell phagocytosis. 

### 1.10.3 (25 June 2022): 
Version 1.10.3 primarily fixes bugs and further refines the signal and behavior dictionaries, particularly with access to custom variables. It also allows users to designate custom variables as _conserved quantities_ that are divided evenly among daughter cells as division (e.g., melanosomes). Lastly, this release continues updates to PhysiBoSS and libRoadrunner to leverage newer core features and improve compatibiltiy, while also improving support for newer Mac (M1 and M2) architectures. 

### 1.10.2 (24 May 2022): 
Version 1.10.2 introduces bugfixes to the behavior "dictionary" functiouns, data saves, and updating neighbor lists for nearby non-adhesive cells. It also introduces a number of ease-of-access functions to the phenotype for death rates, secretion, and internalized substrates. 

### 1.10.1 (15 May 2022): 
Version 1.10.1 introduces bugfixes to increase XML parser robustness and to fix missing PhysiBoSS makefiles. 

### 1.10.0 (13 May 2022): 
The 1.10.0 release introduced major new phenotype functionality, including standardized support for cell-cell interactions (phagocytosis, cell attack that increases a tracked damage variable, and cell fusion), cell transformations, advanced chemotaxis, and cell adhesion affinities for preferential adhesion. This release also includes new, auto-generated "dictionaries" of signals and behaviors to facilitate writing cell behavioral models and intracellular models, as well as standardized Hill and linear response functions for use in intracellular models. Lastly, this release includes a number of bugfixes, most notably pseudorandom number generators with improved thread safety. 

A blog post and tutorial on the new phenotype elements can be found at http://www.mathcancer.org/blog/introducing-cell-interactions-and-transformations. 

A blog post and tutorial on the new signal and behavior dictionaries can be found at http://www.mathcancer.org/blog/introducing-cell-signal-and-behavior-dictionaries. 
 
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

* * * 

# PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D Multicellular Systems

**Versions:** 1.9.0 - 1.9.1

**Release dates:** 12 July 2021 - 13 September 2021

## Release summaries: 
Version 1.9.z introduces intracellular modeling, i.e., models inside individual cells, for PhysiCell. We support three types of intracellular models: boolean networks, ordinary differential equations (ODEs), and dynamic flux balance analysis (dFBA). An intracellular model is part of a cell type's phenotype specification. Currently, we only support a single intracellular model per cell type; however, different *types* of models can be used for different cell types, e.g., a boolean network for cell type A and ODEs for cell type B.

### Version 1.9.1 (13 September 2021): 
Version 1.9.1 release focuses primarily on bug fixes. It fixes memory leaks and other bugs in intracellular modeling, as well as several small bugs in parsing cell definitions in the XML configuration file. It also implements a basic_volume_model that only models total volume. (For internal consistency, it treats the entire cell as cytoplasm.) 

### Version 1.9.0 (12 July 2021): 
Version 1.9.0 introduces intracellular modeling, i.e., models inside individual cells, for PhysiCell. We support three types of intracellular models: boolean networks, ordinary differential equations (ODEs), and dynamic flux balance analysis (dFBA). An intracellular model is part of a cell type's phenotype specification. Currently, we only support a single intracellular model per cell type; however, different *types* of models can be used for different cell types, e.g., a boolean network for cell type A and ODEs for cell type B.

This new functionality has been a collaborative effort with the Institut Curie, the Barcelona Supercomputing Center, and the University of Washington. We provide a unified C++ interface between each intracellular model and PhysiCell.

The Systems Biology Markup Language (SBML) is used to define both the ODEs and FBA models; boolean networks are defined using MaBoSS's custom 
configuration (.cfg and .bnd) files. (NOTE: PhysiCell does *not* support the full SBML specification; details are provided elsewhere.)

**NOTE 1:** MacOS users need to define a PHYSICELL_CPP environment variable to specify their OpenMP-enabled g++. See the [Quickstart](documentation/Quickstart.md) for details.

**NOTE 2:** Windows users need to follow an updated (from v1.8) MinGW64 installation procedure. This will install an updated version of g++, plus libraries that are needed for some of the intracellular models. See the [Quickstart](documentation/Quickstart.md) for details.

### Major new features and changes:

#### Version 1.9.1: 
+ None in this release. 

#### Version 1.9.0: 
+ First full support for intracellular models: boolean networks, ordinary differential equations (ODEs), and dynamic flux balance analysis (dFBA).

+ Added an abstract `Intracellular` class in core/PhysiCell_phenotype.h. Concrete classes for the supported intracellular models provide the functionality in the abstract class.

+ Added an `/addons` directory in the root directory. This is where the intracellular concrete classes and code are located.

+ We adopt existing software for intracellular model solvers: MaBoSS for boolean networks, libRoadrunner for ODEs, and Clp for dFBA. However, to make it easier for modelers to use these solvers in PhysiCell, we provide automatic downloads of libraries (see next bullet).

+ If a PhysiCell model uses an intracellular model, the PhysiCell Makefile will run a Python script (in the /beta directory) that checks to see if you have already downloaded the software (library) for the intracellular solver and, if not, downloads it and puts it in a directory within your PhysiCell project where it can be found and linked. The Python script will download the appropriate library for your operating system.

### Minor new features and changes: 
#### Version 1.9.1:
+ Implemented basic_volume_model (see standard models), where nuclear volumes are set to zero, and cytoplasmic volumes are updated as normal. 

#### Version 1.9.0:
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

+ Bug fix and improvements to /beta/params_run.py to perform parameter explorations of models.

### Beta features (not fully supported):
#### Version 1.9.1: 
+ No new beta features this release. 

#### Version 1.9.0: 
+ Started writing a standardized set of functions for Hill functions and promoter/inhibitor signaling. 

+ [Model Builder Tool](https://github.com/PhysiCell-Tools/PhysiCell-model-builder/releases) 

+ Added a simple Qt GUI for plotting cells only (plot_cells.py and vis_tab_cells_only.py in /beta)

+ Added a simple Qt GUI for plotting substrates and cells (plot_data.py and vis_tab.py in /beta)

+ Added simple contour plotting of a substrate (anim_substrate2D.py in /beta; copy to /output) 
  
### Bugfixes: 
#### Version 1.9.1: 
+ Fixed bug in legend function where on some rare occasions, the temporary cell could adversely interact with other cells prior to deletion. 

+ Remove an old error printout from standard_elastic_contact_function that causes problem with the ANCIENT version of gcc (4.8.x) that nanoHUB refuses to upgrade. 

+ Fixed Libroadrunner memory leak issue.

+ Made minor bugfixes to parsing cell definitions in the XML configuration files: 
  + verify motility enabled flag is present before parsing its value
  + fix bug when parsing multiple death models

#### Version 1.9.0: 
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

* * * 

**Version:** 1.8.0

**Release date:** 9 March 2021

## Release summary: 

This release formally introduces Cell Definitions: a way to fully create cell types in the XML configuration file, including each cell type's initial phenotype and custom variables. This extends our recent work to shift specification of the microenvironment and boundary conditions to XML, and continues our trend towards a future release when many models can be designed and run without compiling any C++ at all. Most of the sample projects have been updated to use this new paradigm, including the a unified 2D/3D `template` project. We recommend using that template as the starting point for any new PhysiCell model.

This release also introduces contact functions: a way to specify cell-cell contact interactions for any cells that you attach (using new, standardized attach and detach functions). Look at the `cancer-biorobots` and `biorobots` sample projects for examples for examples of the newly-introduced, standardized "spring" contact functions. The new `worm` sample project shows a more sophisticated example where cells exchange a differentiation factor across their contacts to model juxtacrine signaling. To help support contact interaction modeling, we include new search functions to easily report a vector of Cells that are nearby for use in your contact interactions. The default mechanics function also records a list of all currently (mechanically) interacting cells in state.neighbors.  

The release also add a number of features to improve the ease of code use: a copy of the XML configuration file is now saved in your output directory to help keep track of what parameters and settings generated the data. We auto-generate a `legend.svg` file (also in output) based on your coloring function and cell defintions. The sample projects' Makefiles now include new rules to create animated GIFs, convert SVG to JPG, create a MP4 movie, and even auto-download the latest version of PhysiCell to update an existing project. A key new feature is the ability to pre-specify cell locations in a CSV file. The template projects will auto-parse this list if enabled to place cells at the start of the simulation. We also provide new functionality to add a virtual wall to the simulation domain to help keep cells from leaving; this can be enabled or disabled from the XML configuration file. 

**NOTE:** OSX users must now define PHYSICELL_CPP system variable. See the documentation.
 
### Major new features and changes:

+ Full rollout of `Cell_Definition` in the XML configuration files. Many basic models can now be fully defined in XML with minimal or no C++. 

+ Unified the 2D and 3D template projects into a single "template" project. 

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

**Version:** 1.7.1

**Release date:** 2 June 2020

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

# PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D Multicellular Systems.

**Version:** 1.7.0

**Release date:** 12 May 2020

## Release summary: 

This release (1) adds "net export" as a new form of more generalized substrate secretion, (2) adds helper funtions for cell size and volume for esier configuration, (3) adds a new, standardized chemotaxis function, (4) adds 1D diffusion, and (5) introduces XML-based cell definitions as a beta feature. It also incorporates a variety of bugfixes.   

**NOTE:** OSX users must now define PHYSICELL_CPP system variable. See the documentation.
 
### Major new features and changes:

+ Added "net_export_rates" to "secretion" part of Phenotype, and to the Basic_Agent class in BioFVM. This is in response to SourceForge ticket 19. 

+ Added new helper functions to help users to more easily set the *target* cell size: 
   + void Cell::set_target_volume( double new_volume ) sets the target total cell volume, while preserving the desired nuclear:cytoplasmic ratio and desired fluid fraction. In the default cell volume model, the cell will now approach this value by shrinking or growing. 
   + void Cell::set_target_radius( double new_radius ) behaves similarly, but using a radius instead. 

+ Added Cell::set_radius( double new_radius ) to set the cell's current radius to the new value, preserving the nuclear:cytoplasmic ratio and fluid fraction. Note that this does not change the target values, so the cell will shrink or grow back towards its current target size. 

+ Added 1-D diffusion solvers to BioFVM (useful for some coarse-grained problems). It solves for diffusion in the x-direction only. Use it by setting: 

  microenvironment.diffusion_decay_solver = diffusion_decay_solver__constant_coefficients_LOD_1D
  
  Use this right after setup_microenvironment() in your main.cpp file. Future versions will include an XML option to use 1D. Most users will never need this. 
  
+ Added a standardized chemotaxis function to the standard models: 

     void chemotaxis_direction( Cell* pCell , Phenotype& phenotype , double dt ); 

  This sets:
  
     phenotype.motility.motility_bias_direction = direction * grad( index ), where 
    
     direction = phenotype.motility.chemotaxis_direction 
        (1 to go up gradient, -1 to go down gradient)
     index = phenotype.motility.chemotaxis_index 
        (the index of one of hte diffusing substrates)

+ Added Cell_Definitions in XML (beta feature) in response to SourceForge ticket 5. Users will be able to set the cell defaults definition by XML, as well as additional cell definitions that "inherit" phenotype parameters from cell defaults. This vastly reduces teh amount of necessary C++ to define a model. The new "template" sample project unifies 2D and 3D model specification using the new XML-based cell definitions. The next few releases will refine documentation and roll teh new XML-based cell definitions out to all the other sample projects. 

### Minor new features and changes: 

+ Created get_cell_definition(std::string) to return by reference the matching cell definition (search by name). Returns cell_defaults if nothing found. 

+ Created get_cell_definition(int) to return by reference the matching cell definition (search by type). Returns cell_defaults if nothing found. 

+ added int chemotaxis_index and chemotaxis_direction to the Motility class to assist with a new standard chemotaxis function. 
 
+ scale_all_secretion_by_factor also scales net_export_rates.

+ sync_to_current_microenvironment and sync_to_microenvironment set up net_export_rates 

+ Secretion::advance now updates net_export processes 

+ Secretion::set_all_secretion_to_zero and Secretion::scale_all_secretion_by_factor act on net_export_rates as well. 

+ Cell::turn_off_reactions acts on net_export_rates as well. 

+ BioFVM::Basic_Agent::simulate_secretion_and_uptake now updates net_export processes, including impact on internal tracked substrate totals. (And all appropriate initializatoin functions have been updated. 

+ Updated documentation to reflect the new net export rates. 
 
+ Updated the documentation to fully state the biotransport PDEs (for better clarity), including notes on the dimensions of the parameters. 

+ Deprecated the following (unimplemented) function from the Volume class definition, as promised: 
```
 void update( Cell* pCell, Phenotype& phenotype, double dt )
```

+ Added a new registry (unsorted map) of all cell definitions called cell_definitions_by_name and a vector of cell definitions called cell_definitions_by_index.  

+ The Cell_Definition default constructor and copy constructor automatically register all new cell definitions in cell_definitions_by_index; 

+ Created a display_cell_definitions(std::ostream&) function to quickly list all cell definitions and key information.  

+ Created build_cell_definitions_maps() to create cell_definitions_by_name and cell_definitions_by_type. This should go at the end of create_cell_types(). 

+ Created find_cell_definition(std::string) to return a pointer to the matching cell definition (search by name). Returns NULL if nothing found. 

+ Created find_cell_definition(int) to return a pointer to the matching cell definition (search by type). Returns NULL if nothing found. 

+ Created unit tests for cell definitions

+ Deprecated oxygen_index, glucose_index, TUMOR_TYPE, and VESSEL_TYPE from PhysiCell_Constants as promised. 

+ Minor source code cleanup in PhysiCell_settings.cpp. 

+ All sample projects now automatically build (and display) the registries of cell definitions via build_cell_definitions_maps() and display_cell_definitions(). 

+ added the following std::vector<bool> to Microenvironment_Options to facilitate setting Dirichlet conditions on specific boundaries for specific substates: Dirichlet_all, Dirichlet_xmin, Dirichlet_xmax, Dirichlet_ymin, Dirichlet_ymax, Dirichlet_zmin, Dirichlet_zmax, Dirichlet_interior. 

+ Minor cleanup in BioFVM_microenvironment.cpp 

+ Microenvironment::update_dirichlet_node(voxel_index,substrate_index,value) now sets dirichlet_activation_vectors[voxel_index][substrate_index] = true; 

+ Microenvironment::set_substrate_dirichlet_activation( int substrate_index , bool new_value ) now sets dirichlet_activation_vectors[voxel_index][substrate_index] for ALL Dirichlet nodes. 

+ Microenvironment::apply_dirichlet_conditions() now checks the Dirichlet activation vector of the individual voxel. 

+ Microenvironment::resize_voxels() and the various Microenvironment::resize_space() functions now resize dirichlet_activation_vectors, using the default dirichlet_activation_vector as the initial uniform activation vector. 

+ Microenvironment::resize_densities() and the various Microenvironment::add_density() functions now resize dirichlet_activation_vector and use it to intialize dirichlet_activation_vectors at every voxel. 

+ The various Microenvironment::add_density() functions now 

+ Added function Microenvironment::set_substrate_dirichlet_activation( int index, std::vector<bool>& new_value ) to set the entire vector of activation at a specific voxel. 


### Beta features (not fully supported):
 
+ Cell definitions can now be defined by XML files. See the note above. This functionality may be additionally refined in the next few releases while still in beta. 

  
### Bugfixes: 

+ In response to SourceForge ticket 25, added cell_defaults.phenotype.molecular.sync_to_microenvironment( &microenvironment ); to the create_cell_types() functions in the 2D and 3D template projects. 

+ In response to SourceForge ticket 18, update_cell_and_death_parameters_O2_based() now checks for deterministic necrosis. 

+ In response to GitHub issue 33, fixed issue where data-cleanup makefile rule gets a list of too many files. Rolled the new rule through to all the sample Makefiles as well. 

```
data-cleanup:
	rm -f *.mat
	rm -f *.xml
	rm -f *.svg
	rm -rf ./output
	mkdir ./output
	touch ./output/empty.txt
```
+ Updated Cell::Cell(), create_cell(), create_cell(Cell_Defintion), and convert_to_cell_definition() to call set_total_volume( phenotype.volume.total ). This makes sure that BioFVM knows the correct volume at the time of creation (or major update) so that it can save the correct values to outputs. This is in response to GitHub issue 22. 

+ Removed the false statement from the user manual that stated that the cytoplasmic:nuclear ratio is between 0 and 1. 

+ Removed the false statement from the user manual that stated that relative cell rupture volume is between 0 and 1. 

+ Updated the list of PhysiCell_Constants in response to SourceForge ticket 11. 

+ The various Microenvironment::add_density() functions now only set dirichlet_activation_vector = true for the newly added substrate, rather than *all* of them. This new vector is then used to initialize the activation vectors at every voxel. 

+ Microenvironment::get_substrate_dirichlet_activation() mistakenly returned a double. Now it returns bool. 

### Notices for intended changes that may affect backwards compatibility:
 
+ We intend to merge Custom_Variable and Custom_Vector_Variable in the very near future.  

+ We may change the role of operator() and operator[] in Custom_Variable to more closely mirror the functionality in Parameters<T>. 

+ We will introduce improvements to placement of daughter cells after division. 

+ Some search functions (e.g., to find a substrate or a custom variable) will start to return -1 if no matches are found, rather than 0. 
 
+ We will change the timing of when entry_functions are executed within cycle models. Right now, they are evaluated immediately after the exit from the preceding phase (and prior to any cell division events), which means that only the parent cell executes it, rather htan both daughter cells. Instead, we'll add an internal Boolean for "just exited a phase", and use this to exucte the entry function at the next cycle call. This should make daughter cells independently execute the entry function. 

+ We might make "trigger_death" clear out all the cell's functions, or at least add an option to do this. 

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

# PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D Multicellular Systems.

**Version:** 1.6.1

**Release date:** 26 January 2020

## Release summary: 

This release fixes minor bugs and improves the documentation. It also adds some minor new capabilities, such as setting time step sizes in the XML configuration file. 

**NOTE:** OSX users must now define PHYSICELL_CPP system variable. See the documentation.
 
### Major new features and changes:

+ List here. 
 
### Minor new features and changes: 
 
+ "make list-projects" now displayed to standard output a list of all the sample projects. 

+ dt_diffusion, dt_mechanics, and dt_phenotype can now be set via the XML configuration file in the options section. 

+ Added documentation on the time step sizes to the User Guide. 

+ Preliminary work to support Travis CI testing. 

+ Updated documentation to note that Cell::start_death is the preferred method to trigger cell death, and NOT Death::trigger_death. 

+ Updated Microenvironment::compute_all_gradient_vectors to now compute one-sized gradients on edge voxels. (Previously, no gradient was computed here.) 

+ Updated Microenvironment::compute_all_gradient_vectors to check if there is no z-direction (i.e., 2D) and exit early if so. 

+ Updated Microenvironment::compute_all_gradient_vectors to check if there is no y-direction (i.e., 1D) and exit early if so. 

+ Made PhysiCell_constants.cpp (and added this to the core of all project makefiles) so that dt and other variables can be non-static (i.e., set by XML options). 

+ Added "make checkpoint" rule to makefiles. This zips up the user-custom stuff (./config, ./, ./custom_modules) into a timestamped zip file. Use this before upgrading PhysiCell to make sure you keep your own Makefile, etc. 
 
### Beta features (not fully supported):
 
+ List here. 
  
### Bugfixes: 

+ BioFVM's diffusion_decay_solver__constant_coefficients_LOD_3D, diffusion_decay_solver__constant_coefficients_LOD_2D check for regular meshes instead of uniform meshes. 

+ Biorobots sample project fixed bugs on searching for substrates vs. searching for cell types. 

+ In BioFVM_vectors, the normalize functions now return a zero vector if the vector's norm is less than 1e-16. This is for John Metzcar. 

+ In PhysiCell_Cell.cpp, made fixes to Cell::divide() and Cell::assign_position() to fix a bug where cells dividing on the edge of the domain woudl place a daughter cell at (0,0,0). Thanks, Andrew Eckel!

+ Code cleanup in PhysiCell_cell_container in Cell_Container::update_all_cells() as suggested by Andrew Eckel. Thanks! 
 
### Notices for intended changes that may affect backwards compatibility:
 
+ We intend to merge Custom_Variable and Custom_Vector_Variable in the very near future.  

+ We may change the role of operator() and operator[] in Custom_Variable to more closely mirror the functionality in Parameters<T>. 

+ We will introduce improvements to placement of daughter cells after division. 

+ Some search functions (e.g., to find a substrate or a custom variable) will start to return -1 if no matches are found, rather than 0. 
 
+ We will change the timing of when entry_functions are executed within cycle models. Right now, they are evaluated immediately after the exit from the preceding phase (and prior to any cell division events), which means that only the parent cell executes it, rather htan both daughter cells. Instead, we'll add an internal Boolean for "just exited a phase", and use this to exucte the entry function at the next cycle call. This should make daughter cells independently execute the entry function. 

+ We might make "trigger_death" clear out all the cell's functions, or at least add an option to do this. 

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

**Version:** 1.6.0

**Release date:** 20 August 2019

## Release summary: 

This release introduces a new XML-based configuration for the chemical microenvironment. All 
the sample projects have been updated to use this new functionality. There is no change 
in APIs or high-level usage / syntax for end users; old projects should continue to work without 
modification, although we highly recommend migrating to the simplified microenvironment setup. 
A short blog tutorial on this new functionality can be found at 

http://mathcancer.org/blog/setting-up-the-physicell-microenvironment-with-xml

**NOTE:** OSX users must now define PHYSICELL_CPP system variable. See the documentation.
 
### Major new features and changes:

+ XML-based setup of the chemical microenvironment.  
 
### Minor new features and changes: 
 
+ Updated template2D sample project: 
  + Refined "reset" and "data-cleanup" rules in Makefile
  + Converted project to use the new XML-based microenvironment setup. 

+ Updated template3D sample project: 
  + Refined "reset" and "data-cleanup" rules in Makefile
  + Converted project to use the new XML-based microenvironment setup. 

+ Updated heterogeneity sample project: 
  + Refined "reset" and "data-cleanup" rules in Makefile
  + Converted project to use the new XML-based microenvironment setup. 

+ Updated cancer immune sample rpoject: 
  + Refined "reset" and "data-cleanup" rules in Makefile
  + Converted project to use the new XML-based microenvironment setup. 

+ Updated virus macrophage sample project: 
  + Refined "reset" and "data-cleanup" rules in Makefile
  + Converted project to use the new XML-based microenvironment setup. 
  + Enabled gradient calculations (were previously off, although we wanted macrophage chemotaxis) 

+ Updated biorobots sample project: 
  + Refined "reset" and "data-cleanup" rules in Makefile. 
  + Converted project to use the new XML-based microenvironment setup.
  + Note that values in user_parameters will override values in microenvironment_setup. 
  + Improved project to properly search for substrate indices instead of hard coding them. 

+ Updated cancer biorobots sample project: 
  + Refined "reset" rule in Makefile. 
  + Converted project to use the new XML-based microenvironment setup.
  + Improved project to properly search for substrate indices instead of hard coding them. 

+ Refined "reset" and "data-cleanup" rules in default Makefile 

+ Created new function to access the (private) microenvironment dirichlet_activation_vector: 
 
  double Microenvironment::get_substrate_dirichlet_activation( int substrate_index ); 

+ Updated the main microenvironment display function Microenvironment::display_information to summarize the initial and boundary conditions for each substrate 

+ Wrote two new functions to parse the XML in microenvironment_setup to add substrates and 
options:  
  + bool setup_microenvironment_from_XML( pugi::xml_node root_node )
  + bool setup_microenvironment_from_XML( void )
The second one assumes you already defined the root node and access the 
global (pugi)xml node for it. 

+ The main XML parsing function now calls setup_microenvironment_from_XML(), just before processing user-defined parameters. 
 
### Beta features (not fully supported):
 
+ anim_svg.py - now plots correctly sized cells; manually step via arrow keys

+ anim_svg_cycle.py - same as above, but automatically cycles through .svg files
  
### Bugfixes: 

+ None.
 
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
 
**Version:** 1.5.2

**Release date:** 11 June 2019

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

# PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D Multicellular Systems.
 
**Version:** 1.5.1

**Release date:** 7 June 2019

## Release summary: 

This minor release fixes bugs in the new virus-macrophage sample project. Users should also consult the reslease notes for 1.5.0. 

**NOTE:** OSX users must now define PHYSICELL_CPP system variable. See the documentation.
 
### Major new features and changes:

+ None 
 
### Minor new features and changes: 
 
+ None 
 
### Beta features (not fully supported):
 
+ None 
  
### Bugfixes: 

+ In the virus-macrophage sample project, switch cell death (in epithelial_function) from apoptosis to cell_lysis to demonstrate the new function. 

+ In the virus-macrophage sample project, enable internalized substrate tracking in the setup_microenvironment() function. 

+ In the virus-macrophage sample project, use a slower viral replication rate. (Should take 240 minutes to reach the lysis threshold.) 

+ In the virus-macrophage sample project, switched to a maximum simulation time of 24 hours (1440 minutes). 
 
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

# PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D Multicellular Systems.
 
**Version:** 1.5.0

**Release date:** 7 June 2019

## Release summary: 
 
This release introduces the ability to track the net internalization of diffusible substrates from the microenvironment, conserving mass with the diffusion-reaction partial differentiatial equations in BioFVM. This is part of longer-term plans to support molecular-scale models (e.g., as encoded by SBML). It also introduces the ability for cells to ingest other cells (and acquire their internalized substrates and volume), the ability for cells to release their internalized substrates back to the microenvironment after death, and a new cell death model (lysis). 

To illustrate these new capabilities, this release introduces a new sample project called virus-macrophage-sample. In this project, virus particles diffuse through the microenvironment, are uptaken by cells, replicate within cells, and trigger lytic death after reaching a threshold. Lysed cells release their virus particles to further diffuse through the environment. Macrophages move by random migration, test for contact with cells, and ingest / phagocytose cells based upon their viral load. Macrophages degrade their internalized viral particles. 

This release also added clearer methods to specify the microenvironment initial conditions, and improved documentation. 

**NOTE:** OSX users must now define PHYSICELL_CPP system variable. See the documentation.
 
### Major new features and changes:

+ Added functionality in BioFVM to (optionally) track the total amount of substrates in each cell, based upon tracking uptake and secretion. Note that without additional, user-defined functions to internally create or consume substrate (e.g., synthesizing proteins, or using oxygen in metabolism), this can result in negative internal values (if cells only secrete but no internal creation functions have been set) or unbounded positive values (if cells uptake a substrate but do not consume it). In particular, Basic_Agents (and hence Cells) now have: 
++ std::vector<double>* internalized_substrates (tracks internalized substrates)
++ std::vector<double>* fraction_released_at_death (sets the fraction of each substrate that is released at cell death)
++ std::vector<double>* fraction_transferred_when_ingested (sets the fraction of each substrate that is transferred to a predatory cell if the cell is eaten). 

Users should access these via the cell's (new) molecular portion of the phenotype. See below. 

In BioFVM::Microenvironment_Options class, we added: 
++ bool track_internalized_substrates_in_each_agent. Set this to true to enable the tracking. 

+ In BioFVM::Microenvironment_Options, we added the ability to set the (uniform) microenvironment initial conditions, via: 
++ std::vector<double> initial_condition_vector. 

If this has size zero, then BioFVM will use the std::vector<double> Dirichlet_condition_vector as a reasonable guess at initial conditions as in prior versions. 

+ We added a new function to Basic_Agent (and hence Cell) to facilitate registering with the microenvironment: 
++ void Basic_Agent::register_microenvironment( Microenvironment* microenvironment_in )

This function will ensure that the secretion, uptake, and internalization tracking data structures of the individual cell agent are properly matched to the microenvironment. 

+ Added new "Molecular" block to Phenotype, for storing internalized substrates and for eventual inclusion of molecular-scale models (via SBML). The major elements are: 
++ std::vector<double> internalized_substrates (tracks internalized substrates)
++ std::vector<double> fraction_released_at_death (sets the fraction of each substrate that is released at cell death)
++ std::vector<double> fraction_transferred_when_ingested (sets the fraction of each substrate that is transferred to a predatory cell if the cell is eaten). 

+ Added lyse_cell() to Cell, to allow a cell to immediately be lysed and potentially release its internalized substrates. Lysed cells are removed from the simulation. 
 
+ Added ingest_cell(Cell*) to Cell, to allow a cell to ingest (e.g., phagocytose) another cell, acquire its volume, and also (optionally) acquire its internalized substrates. This should be useful for immunology.  
 
### Minor new features and changes: 
 
+ Added void Microenvironment::update_dirichlet_node( int voxel_index , int substrate_index , double new_value ) based on pc4nanobio changes, which allows you to update a single substrate's dirichlet condition at specific voxel, rather than all of them. 

+ Added void sync_to_microenvironment( Microenvironment* pMicroenvironment ) to Phenotype to facilitate syncing the cell's phenotype to the microenvironment (and subsequently calls the functions in phenotype.secretion and phenotype.molecular). This isn't used yet, but perhaps it should be. 
 
### Beta features (not fully supported):
 
+ None 
  
### Bugfixes: 

+ Updated BioFVM's operator<< on std::vector<double> so that it doesn't output x="value", y="value", z = "value" for 3-D vectors. 

+ Fixed the search for cycle phase indices in the 2D and 3D template projects, to make sure it searches teh flow_cytometry_separated_cycle_model model and not the Ki67_advanced model, as part of the create_cell_types() function in the custom.cpp files. 

+ In PhysiCell_standard_models, standard_volume_update_function is now fixed to update phenotype.volume.fluid. (This was not used in any mechanics or other calculations, so it does not affect prior modeling results.) 

+ Removed repeated parameters (attached_worker_migration_bias, unattached_worker_migration_bias) in the cancer biorobots sample project.

+ trigger_death() now works. 
 
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

* * * 

# PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D Multicellular Systems.
 
**Version:** 1.4.1

**Release date:** 2 October 2018

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

* * * 

# PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D Multicellular Systems.
 
**Version:**      1.4.0

**Release date:** 26 September 2018

## Release summary: 
 
This release improves the use of XML parsing in configuring simulations, notably (1) reading the domain parameters instead of hard-coded values, and (2) parsing a <user_parameters> block in the XML config files to populate a global parameters data structure of Boolean, integer, double, and std::string variables. Users can efficiently query these from within any function anywhere in a PhysiCell project. 
 
**NOTE:** OSX users must now define PHYSICELL_CPP system variable. See the documentation.
 
### Major new features and changes:

+ User Parameters! 

+ Parsing XML to set domain size for all sample projects. 
  
### Minor new features and changes: 
 
+ Updated all the sample projects to use the improved XML parsing; 
  
+ New functions in PhysiCell_pugixml: 

  1) std::string xml_get_my_name( pugi::xml_node node ); 
  
     This helps to easily extract the name of an XML node. (e.g., <bob units="none"/> returns bob.) 

  2) bool xml_get_my_bool_value( pugi::xml_node node ); 
  
     This gets the Boolean value of an XML node. (e.g., <bob units="none">true</bob> returns true.) 
  
  3) int xml_get_my_int_value( pugi::xml_node node ); 
  
     This gets the integer value of an XML node. (e.g., <bob units="none">42</bob> returns 42.)
  
  4) double xml_get_my_double_value( pugi::xml_node node ); 

     This gets the double value of an XML node. (e.g., <bob units="none">42.03</bob> returns 42.03.)

  5) std::string xml_get_my_string_value( pugi::xml_node node ); 
  
+ Updated all Makefiles to copy main.cpp, the Makefile, and ./config/PhysiCell_settings.xml to backup copies prior to populating any sample project.      

+ Updated the heterogeneity sample project: 

  1) Use the domain settings from the XML config file
  
  2) Use the XML config file options to set the initial tumor size and oncoprotein distribution. 
  
  3) Get the random seed from the XML config file. 
  
  4) Rewrote the custom coloring function to scale from min oncoprotein value (blue) to max oncoprotein value (yellow). 
  
+ Updated template2D sample project:

  1) Use the domain settings from the XML config file
  
  2) Use the XML config file to set the motile cell parameters
  
  3) Get the random seed from the XML config file. 
  
  4) Updated to use the my_coloring_function coloring function. Made sure the my_coloring_function used false_cell_coloring_cytometry as its starting point. 
  
+ Updated template3D sample project:

  1) Use the domain settings from the XML config file
  
  2) Use the XML config file to set the motile cell parameters
  
  3) Get the random seed from the XML config file. 
  
  4) Updated to use the my_coloring_function coloring function. Made sure the my_coloring_function used false_cell_coloring_cytometry as its starting point. 
  
+ Updated biorobots sample project:   

  1) Use the domain settings from the XML config file
  
  2) Use the XML config file to set parameters and colors throughout the biorobots.cpp file 
  
  3) Get the random seed from the XML config file
  
+ Updated cancer immune sample project:   

  1) Use the domain settings from the XML config file
  
  2) Use the XML config file to set parameters throughout the cancer_immune_3D.cpp file 
  
  3) Get the random seed from the XML config file

+ New function in ./core/PhysiCell_utilities: 

  int choose_event( std::vector<double probabilities> );
  
  If probabilities is a vector of n probabilities (for n events), and the sum of the probabilities is 1, then this chooses one of those n events according to these probabilities and returns the index of the selected event. 

+ Moved from README.txt to README.md to support markdown and improve releases on both SourceForge and GitHub. 
  
+ Moved from changes.txt to changes.md to support markdown and improve releases on both SourceForge and GitHub. 
  
### Beta features (not fully supported):
 
+ None 
  
### Bugfixes: 

+ Updated the "reset" rules so that the default config file is restored (in all the sample makefiles). 
  
+ Removed a cout from Mechanics::set_relative_equilibrium_distance() from ./core/PhysiCell_phenotype.*
  
### Notices for intended changes that may affect backwards compatibility:
 
+ We intend to merge Custom_Variable and Custom_Vector_Variable in the very near future.  
 
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

* * * 

PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D 
Multicellular Systems.
 
Version:      1.3.3
Release date: 16 September 2018

Release summary: 
 
This release introduces simplifications in versioning to facilitate 
faster release cycles. It also introduces functions to register and 
list citations for third-part add-ons. The goal is to encourage end-
users to properly cite PhysiCell and add-on products used in their 
projects. 
 
NOTE: OSX users must now define PHYSICELL_CPP system variable. 
      See the documentation.
 
Major new features and changes:
 
+ none
  
Minor new features and changes: 
 
+ Moved all version information to VERSION.txt to facilitate 
  faster release cycles. 
  
+ Updated all the Makefiles to set VERSION via 
  
  VERSION := $(shell grep . VERSION.txt | cut -f1 -d:)
  
+ Moved detailed citation information to CITATION.txt to 
  facilitate faster release cycles.
  
+ Changed std::string PhysiCell_version to 
  std::string PhysiCell_Version for more consistency with 
  prior BioFVM work. 
  
+ Added new static string to PhysiCell.h: 
  static std::string PhysiCell_DOI
  
+ Added new functions to PhysiCell_utilities to query 
  PhysiCell versioning: 

  std::string get_PhysiCell_version( void ); 
 
  void get_PhysiCell_version( std::string& pString ); 
  
+ Added the following function to PhysiCell_utilities to 
  display a list of all software versions and citations 
  used in the code:   
  
  void display_citations( std::ostream& os ); 
  void display_citations( void ); 
  
  For example, use display_citations( std::cout ), or use 
  an output file stream. Note that display_citations(void) 
  calls display_citation(std::cout);   
  
+ Updated all the sample projects to use display_citations(); 

Beta features (not fully supported):
 
+ Added ./protocols/ directory to include release and other 
  instructions, to help train new developer contributors. 
  Perhaps this should be called "checklists" ? 
  
+ Added the following functions to PhysiCell_utilities to 
  register third-party software citations in a global list, 
  ready for query and display: 
  
  void add_software_citation( std::string name , 
       std::string version, std::string DOI, std::string URL ); 
  
Bugfixes: 

+ None
  
Notices for intended changes that may affect backwards compatibility:
 
+ We will probably move from README.txt to README.md to support 
  markdown and improve releases on both SourceForge and GitHub. 
  
+ We will probably move from changes.txt to changes.md to support 
  markdown in the long-term change logs.  
 
+ We intend to merge Custom_Variable and Custom_Vector_Variable in the
  very near future.  
 
Planned future improvements: 
 
+ Further XML-based simulation setup. 
 
+ read saved simulation states (as MultiCellDS digital snapshots)
 
+ "mainline" prototype cell attach/detach mechanics as standard models 
  (currently in the biorobots and immune examples)
 
+ integrate SBML-encoded systems of ODEs as custom data and functions 
  for molecular-scale modeling 
  
+ integrate Boolean network support from PhysiBoSS into the mainline code 
  (See http://dx.doi.org/10.1093/bioinformatics/bty766. )
  
+ Develop contact-based cell-cell interactions. (Likely in next release.)
 
+ Add a new standard phenotype function that uses mechanobiology, 
  where high pressure can arrest cycle progression. 
  (See https://twitter.com/MathCancer/status/1022555441518338048.) 
  (Likely in next release.) 
 
+ Add module for standardized pharmacodynamics, as prototyped in the 
  nanobio project. (See https://nanohub.org/resources/pc4nanobio.) 
 
+ create an angiogenesis sample project 
 
+ create a small library of angiogenesis and vascularization codes as 
  an optional standard module in ./modules (but not as a core component)

=======================================================================

PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D 
Multicellular Systems.
 
Version:      1.3.2
Release date: 24 August 2018

Release summary: 
 
This release fixes a small gradient bug that I swear I had fixed before. 
 
NOTE: OSX users must now define PHYSICELL_CPP system variable. 
      See the documentation.
 
Major new features and changes:
 
+ none
  
Minor new features and changes: 
 
+ none
  
Beta features (not fully supported):
 
+ none
  
Bugfixes: 

+ In BioFVM, Microenvironment::compute_gradient_vector(int), removed "static"
  from "static std::vector<int> indices(3,0)" to prevent rare segfaults. 
  
+ In BioFVM, Microenvironment::compute_gradient_vector(int), replaced "static"
  for "bool gradient_constants_defined = false". Yep, I removed static from 
  the wrong line in 1.3.1 

+ Correct some errors in both the README.txt and changes.txt files. 
  
Notices for intended changes that may affect backwards compatibility:
 
+ None.
 
Planned future improvements: 
 
+ Further XML-based simulation setup. 
 
+ read saved simulation states (as MultiCellDS digital snapshots)
 
+ "mainline" prototype cell attach/detach mechanics as standard models 
  (currently in the biorobots and immune examples)
 
+ integrate SBML-encoded systems of ODEs as custom data and functions 
  for molecular-scale modeling 
  
+ integrate Boolean network support from PhysiBoSS into the mainline code 
  (See https://doi.org/10.1101/267070.)
  
+ Develop contact-based cell-cell interactions. (Likely in next release.)
 
+ Add a new standard phenotype function that uses mechanobiology, 
  where high pressure can arrest cycle progression. 
  (See https://twitter.com/MathCancer/status/1022555441518338048.) 
  (Likely in next release.) 
 
+ Add module for standardized pharmacodynamics, as prototyped in the 
  nanobio project. (See https://nanohub.org/resources/pc4nanobio.) 
 
+ create an angiogenesis sample project 
 
+ create a small library of angiogenesis and vascularization codes as 
  an optional standard module in ./modules (but not as a core component)

=======================================================================
 
PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D 
Multicellular Systems.
 
Version:      1.3.1
Release date: 31 July 2018
 
Release summary: 
 
This release introduces a new cycle model with an active cycling 
state and a quiescent state. It also adds new functions to the 
Mechanics class to make it easier to modify the cell-cell interaction 
distance (while maintaining equilibrium cell-cell spacing), or 
to modify the equilibrium cell-cell spacing. The release also 
includes major reliability and performance improvements to 
how gradients are calculated.
 
As usual, the release also contains minor bugfixes and 
improvements. 
 
NOTE: OSX users must now define PHYSICELL_CPP system variable. 
      See the documentation.
 
Major new features and changes:
 
+ Implemented a new cell cycle model (cycling_quiescent), 
  where quiescent cells can enter a cycling state. This 
  model uses identical parameters to the Ki67-basic cycle 
  model, but decouples the conceptual model from Ki67 
  immunohistochemistry. 
  
  Updated the documentation and coloring functions accordingly.
   
  Updated update_cell_and_death_parameters_O2_based() to 
  support the new cycling_quiescent cycle model.
 
+ update_all_cells(t,dt,dt,dt) now checks to see if gradient 
  calculations are enabled, and if so, computes it once per dt_mechanics.
 
  This improves code performance by 2x to 3x in gradient-enabled codes, 
  because we no longer need to calculate gradients once per dt_diffusion 
  time step. (Gradients are only needed for cell velocity calculations, 
  and occasionally for phenotype decisions.)
 
  All sample projects have been updated to make use of this.    
  
  Also, removed the explicit gradient calculations from the template 
  and sample projects. 
  
+ Added a new function to Mechanics class to simplify changes to cell 
  interaction distances: 
  
     void set_relative_maximum_adhesion_distance( double new_value ); 
   
  Here, new_value is a multiple of the cell's (mean equivalent) radius. 
  Our default is 1.25. This function preserves the cell's "repulsion" 
  strength and adjusts the strength of cell-cell adhesion to maintain 
  its prior equilibrium cell-cell spacing. 
  
+ Added a new function to Mechanics class to simplify changes to cell 
  equilibrium distances. 
  
     void set_relative_equilibrium_distance( double new_value );   
  
  Here, new_value is a multiple of the cell's (mean equivalent) radius. 
  The default is around 1.9. This function preserves the cell's "repulsion" 
  and the maximum interaction distance, and it adjusts the strength of 
  cell-cell adhesion to match the new equilibrium cell-cell spacing. 
  
  Note that this function performs a "sanity check" to ensure that you have 
  not given a value greater than 2.0. 
  
+ Added a new function to Mechanics class to simplify changes to cell 
  equilibrium distances. 
  
     void Mechanics::set_absolute_equilibrium_distance( Phenotype& phenotype, 
     double new_value )
 
  Here, new_value is the new equilibrium spacing (in units of microns). 
  The function internally requires the cell's mean equivalent radius, 
  and so you pass the cell's phenotype (by reference) as an additional 
  argument. 
  
  Note that this function performs a "sanity check" to ensure that you have 
  not given a value greater than 2.0 * mean_cell_radius.  	
  
  Also note that PhysiCell internally uses a relative spacing, so the 
  absolute spacing will change from the value you set, over time.  
  
+ Updated User_Guide to reflect new cell cycle model, 
  including reference parameters chosen for consistency with 
  the other cycle models. 
  
Minor new features and changes: 
 
+ Added the following constants to support the new cycle model
 
     static const int cycling = 17; 
     static const int quiescent = 18; 
     static const int cycling_quiescent_model = 7; 
 
+ Added new coloring function: false_cell_coloring_cycling_quiescent 
 
+ Removed the (never-used) Mechanics.maximum_adhesion_distance. 
 
+ Removed the legacy template_projects folder.
 
Beta features (not fully supported):
 
+ Added a function pointer to the Cell_Functions class for intended 
  contact-based cell-cell interaction, like delta-Notch signaling. 
  
     void (*contact_function)(Cell* pMyself, Phenotype& my_phenotype, 
       Cell* pOther, Phenotype& other_phenotype, double dt ); 
   
  It would be up to the user to define these functions and decide 
  if the functions modify pMyself, pOther, or both. For now, 
  the code will initialize the pointer to NULL and won't use it. 
 
+ Open to comments on handling cell-cell contact functions. Here's what 
  I have in mind: 
  
  notation: cell i interacts with cell j with function f(i,j). 
  
  Each cell agent can hold one contact-based interaction function f, 
  to be stored as a pointer in the cell's instance of the Cell_Functions 
  class. 
  
  We use the containers (and their interaction testing structures) to  
  identify all interactions (i,j,f), and add it to a vector of interactions. 
  The interaction (i,j,f) is added to the list so long as (j,i,f) is not 
  already in the list, to avoid double-counting the interaction. 
  
  The code will seek through the "container" interaction testing 
  data structure, probably at the cell mechanics time scale, and update /
  recreate the vector of contact-based interactions (i,j,f). 
  
  The code would likely go through the vector of interactions and 
  execute the codes once per dt_diffusion time step, since I would 
  imagine molecular biology (with faster time scales) is intended here.
  
  Since f(i,j) can potentially modify both cell i and cell j, it's probably 
  not thread-safe. So we'll probably need that in a non-parallel loop. 
  
  We will probably add a new time scale for interactions, dt_interactions, 
  and update the interaction list on that time scale. 
  
  For faster checking if (i,j,f) or (j,i,f) is already in the vector, 
  we'll probably want some sort of hash map in addition to the vector 
  of interactions. 
  
  We'll probably implement this all in something like 
  PhysiCell_contact_interctions.*, and add a global enable/disable option. 
  I'd imagine we'd add code to the "update_all_cells" to keep this 
  as simple to the users as possible. 
  
  We should probably update each cell's "neighbors" data structure at 
  when we're doing all this testing. 
  
  In a longer-term update, we could leverage that for simpler interaction 
  testing during velocity updates. 
  
Bugfixes: 

+ Added missing "omp_set_num_threads(PhysiCell_settings.omp_num_threads)" 
  in the main-heterogeneity.cpp file for the heterogeneity sample project. 

+ In BioFVM, Microenvironment::compute_gradient_vector(int), removed "static"
  from "static std::vector<int> indices(3,0)" to prevent rare segfaults. 
  
+ Changed <xml> root above the comment lines in output files for 
  better Python parsing compatibility. Thanks, rheiland! 

+ MultiCellDS outputs used size 3 for all custom vector variables, instead 
  of actual size. Fixed to read the size from the vector. 

+ Fixed the ostream operator for Vector_Variable to correctly output all 
  the vector.value elements, rather than the original hard-coded output 
  of 3 elements regardless of size. 
  
Notices for intended changes that may affect backwards compatibility:
 
+ None
 
Planned future improvements: 
 
+ Further XML-based simulation setup. 
 
+ read saved simulation states (as MultiCellDS digital snapshots)
 
+ "mainline" prototype cell attach/detach mechanics as standard models 
  (currently in the biorobots and immune examples)
 
+ integrate SBML-encoded systems of ODEs as custom data and functions 
  for molecular-scale modeling 
  
+ integrate Boolean network support from PhysiBoSS into the mainline code 
  (See https://doi.org/10.1101/267070.)
  
+ Develop contact-based cell-cell interactions. (Likely in next release.)
 
+ Add a new standard phenotype function that uses mechanobiology, 
  where high pressure can arrest cycle progression. 
  (See https://twitter.com/MathCancer/status/1022555441518338048.) 
  (Likely in next release.) 
 
+ Add module for standardized pharmacodynamics, as prototyped in the 
  nanobio project. (See https://nanohub.org/resources/pc4nanobio.) 
 
+ create an angiogenesis sample project 
 
+ create a small library of angiogenesis and vascularization codes as 
  an optional standard module in ./modules (but not as a core component)
 
=======================================================================
 
PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D 
Multicellular Systems.

Version:      1.3.0
Release date: 23 February 2018 

Overview: 
PhysiCell is a flexible open source framework for building 
agent-based multicellular models in 3-D tissue environments. 

Reference: A Ghaffarizadeh, R Heiland, SH Friedman, 
   SM Mumenthaler, and P Macklin, PhysiCell: an Open Source 
   Physics-Based Cell Simulator for Multicellular Systems, 
   PLoS Comput. Biol. 14(2): e1005991, 2018. 
   DOI: 10.1371/journal.pcbi.1005991                    

Visit http://MathCancer.org/blog for the latest tutorials and help. 

Release summary: 
This release introduces two new cell cycle models (G0/G1 -> 
S -> G2/M) and (G0/G1 -> S -> G2 -> M), introduces XML-based 
configuration files, and allows new user control on how 
often and where data are stored. 

As usual, the release also contains minor bugfixes and 
improvements. 

NOTE: OSX users must now define PHYSICELL_CPP system variable. 
      See the documentation.

Major new features and changes:
+ implemented new cell cycle models: 

     flow_cytometry_cycle_model: G0/G1 -> S -> G2/M
     flow_cytometry_separated_cycle_model: G0/G1 -> S -> G2 -> M 
	 
   along with documentation and updated coloring functions 
   
+ The oxygen-based phenotype models now support the new cycle
  model. 
  
+ Added XML file parsing for use in settings files. 

+ XML configuration file (in ./config/PhysiCell_settings.xml) 
  sets data destination (as a subfolder) and save 
  frequency. Legacy saves are now off by default. 
  
+ Users can select a different XML file at command line
 
    ./project_name settings_file.xml 
 
  (Assuming you preserved the structure of the template 
   projects.)
  
+ Updated User_Guide to reflect new XML parsing capabilities.
 
+ Updated User_Guide to reflect new cell cycle models, 
  including reference parameters chosen for consistency with 
  the other cycle models. 
   
Minor new features and changes: 
+ Added PhysiCell_pugixml.* for easier extraction of parameter 
  arguments from XML files
 
+ Added PhysiCell_settings.* to include parsing of XML parameter 
  files, and parameter values stored in a unified data structure. 
  
+ Added parsing of settings file to separately set frequency of 
  saving full output (MultiCellDS), SVG outputs, and "legacy" 
  outputs from USC era.
  
+ Added options to specify the folder of saved data.
 
+ Added option to read the number of OMP threads from a setting file.
 
+ Added "beta" directory where we will put new features that are still
  undergoing testing 
 
+ Minor updates to the Quickstart guide. 
 
+ Added new function: to display the simulation status: 
 
     void display_simulation_status( std::ostream& os ); 
 
     e.g., display_simulation_status( std::cout ); 
 
+ writePov() uses a user-specified output folder.

+ log_output() uses a user-specified output folder. 

+ added "beta-testing" sample project. Populate it by the rule: 
     make beta-testing
 
+ added flow_cytometry_separated_cycle_model to the PhysiCell constants. 
 
+ added G1pm_phase and G1ps_phase to the PhysiCell constants. 
 
+ Added new coloring function: false_cell_coloring_cytometry
 
+ added support for the new cytometry cycle models to the oxygen-based 
     phenotype model (update_cell_and_death_parameters_O2_based)
 
+ updated user manual to reflect new cytometry models 

+ updated template2D and template3D projects to use to use the 
  new cytometry models and coloring schemes. Also reduced to 
  1 mm x 1 mm (2D) and 1 mm^3 (3D) for faster demos. 
  
+ removed archives directory 

Beta features (not fully supported):

+ XML functions moved from beta to production. 

Bugfixes: 

+ Changed instances of uniform_random() (from BioFVM) to 
  UniformRandom() (from PhysiCell) so that all calls to the PRNG
  used the same random seed and same PRNG. Thanks, olliemcdonald!

+ Fixed typo in "Dirichlet" in user documentation. Thanks, luissv7! 

+ Removed .git directory that was accidentally included in releases

+ Updated PhysiCell_MultiCellDS.cpp 
  (add_PhysiCell_cells_to_open_xml_pugi) so that we exit(-1) with a 
  meaningful error message if we cannot open a matlab subfile 
  for writing. 
  
+ Updated PhysiCell_pathology.cpp (SVG_plot) so that we exit(-1) 
  with a meaningful error message if we cannot open an SVG file 
  for writing. 
  
Notices for intended changes that may affect backwards compatibility:

+ template_projects folder will be removed

Planned future improvements: 

+ Further XML-based simulation setup. 

+ read saved simulation states (as MultiCellDS digital snapshots)

+ "mainline" prototype cell attach/detach mechanics as standard models 
  (currently in the biorobots and immune examples)

+ integrate SBML-encoded systems of ODEs as custom data and functions 
  for molecular-scale modeling 

+ create an angiogenesis sample project 

+ create a small library of angiogenesis and vascularization codes as 
  an optional standard module in ./modules (but not as a core component)
  
=======================================================================

PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D 
Multicellular Systems.

Version:      1.2.2
Release date: 4 November 2017

Overview: 
PhysiCell is a flexible open source framework for building 
agent-based multicellular models in 3-D tissue environments. 

Reference: Ghaffarizadeh et al., PLoS Comput Biol (2017)
   preprint URL: https://doi.org/10.1101/088773

Visit http://MathCancer.org/blog for the latest tutorials and help. 

Key makefile rules: 

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

make clean         : removes all .o files and the executable, so that 
                     the next "make" recompiles the entire project 

make data-cleanup  : clears out all simulation data 

make reset         : de-populates the sample project and returns to
                     the original PhysiCell state. Use this when 
                     switching to a new PhysiCell sample project. 


Homepage:     http://PhysiCell.MathCancer.org
Downloads:    http://PhysiCell.sf.net
Support:      https://sourceforge.net/p/physicell/tickets/

Quick Start:  Look at QuickStart.pdf in the documentation folder. 
User Guide:   Look at UserGuide.pdf in the documentation folder. 
 
Tutorials:    http://www.mathcancer.org/blog/physicell-tutorials/

Latest info:  follow @MathCancer on Twitter (http://twitter.com/MathCancer)

See changes.txt for the full change log. 

Release summary: 
This release reduces the complexity of Makefiles (especially for 
OSX users), restructures the 2D and 3D project templates as 
sample projects, fixes a minor bug in SVG pathology outputs, 
improves copying of Cell_Definitions, and fixes minor bugs in 
BioFVM (primarily related to Dirichlet conditions). 

NOTE: OSX users must now define PHYSICELL_CPP system variable. 
      See the documentation.

PhysiCell is currently under scientific peer review. 

Major new features and changes:
+ none 

Minor new features and changes: 
+ Restructured the 2D template project to have the same structure as a 
  typical project, with setup functions related functions in 
  ./custom_modules/*, etc. Moved it to ./sample_projects/template2D/
  ./template_projects/ will be deprecated. 
  
  To populate this project, use: 
  
    make template2D 

  To compile: 
  
    make 
	
  To de-populate the sample project and return to the "clean"PhysiCell state:
  
    make reset 
    make clean (to remove object files)
  
+ Restructured the 3D template project to have the same structure as a 
  typical project, with setup functions related functions in 
  ./custom_modules/*, etc. Moved it to ./sample_projects/template3D/
  ./template_projects/ will be deprecated. 
  
  To populate this project, use: 
  
    make template3D 

  To compile: 
  
    make 
	
  To de-populate the sample project and return to the "clean"PhysiCell state:
  
    make reset 
    make clean (to remove object files)  

+ Simplified Makefiles: populating a sample project and compiling it 

    make <sample_project>

  To compile:

    make 

  To reset to original state:

    make reset 
	
  Current <sample_project> values: 	
    template2D 
    template3D
    biorobots-sample
    cancer-biorobots-sample
    heterogeneity-sample
    cancer-immune-sample 

+ Simplified Makefiles: Makefiles check for system variable 
  PHYSICELL_CPP to set the compiler (CC). OSX users must set 
  this environment variables. See the online tutorials and the 
  user guide. 
  
+ Simplified Makefiles: "make data-cleanup" removes .svg, .mat, .xml, and 
  data inside ./data 

+ Updated documentation on how to add new substrates. 

+ Updated documentation on applying Dirichlet conditions to only 
  specific substrates. 

+ Added new function to copy the properties of a Cell Definition to 
  the cell. 
  
  void Cell::convert_to_cell_definition( Cell_Definition& cd )

Bugfixes: 

+ Fixed a small error in SVG plots, where tissues were flipped with 
  y was vertically flipped. 
  
+ Used register_microenvironment(Microenvironment*) to improve 
  compatibiltiy with other operating systems and compilers. 
  
+ Added copy constructor and copy assignnment functions to the 
  Cell_Definition is. 

+ Removed the unnecessary "wha???" from BioFVM_microenvironment.cpp.

+ Updated Dirichlet_condition_vector = ones (instead of zeros) in 
  Microenvironment_Options::Microenvironment_Options() default 
  constructor. 

+ Microenvironment::resize_densities( int new_size ) no longer 
  overwrites previous dirichlet values when extending the size. 

+ No longer overwrites existing Dirichlet_condition_vector elements 
  or set default_microenvironment_options.use_oxygen_as_first_field 
  to false. 

+ Microenvironment::set_density(int,std::string,std::string) and 
  Microenvironment::set_density(int,std::string,std::string,double,double) 
  were modified to be compatibility. 
  
+ Only set default_microenvironment_options.use_oxygen_as_first_field = false
  if index = 0, when samplign the oxygen. 
  
+ Updated save_PhysiCell_to_MultiCellDS_xml_pugi() to save much more 
  phenotype information and all custom variables for each cell. 

+ Updated read_MultiCellDS_XML.m (in ./matlab) to read these 
  newly expanded data files. 

+ Includes a sneak preview of BioFVM 1.1.7, which includes bugfixes 
  mentioned above. 
  
Notices for intended changes that may affect backwards compatibility:

+ None at this time

Planned future improvements: 

+ parse XML configuration files

+ read saved simulation states (as MultiCellDS digital snapshots)

+ "mainline" prototype cell attach/detach mechanics as standard models 
  (currently in the biorobots and immune examples)

+ integrate SBML-encoded systems of ODEs as custom data and functions 
  for molecular-scale modeling 

+ create an angiogenesis sample project 

+ create a small library of angiogenesis and vascularization codes as 
  an optional standard module in ./modules (but not as a core component)

=======================================================================

PhysiCell: PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D 
Multicellular Systems.

Version:      1.2.1
Release date: 1 September 2017

Homepage:     http://PhysiCell.MathCancer.org
Downloads:    http://PhysiCell.sf.net

Release summary: 
This release introduces a improved MultiCellDS outputs, new matlab 
functions to read the MultiCellDS outputs, experimental functions 
for povray routines (for 3-D raytracing), some bugfixes, and 
new sample projects in cancer heterogeneity, synthetic multicellular 
bioengineering, and cancer immunology. 

PhysiCell is currently under scientific peer review. 

Major new features and changes:
+ Updated save_PhysiCell_to_MultiCellDS_xml_pugi() to save much more 
  phenotype information and all custom variables for each cell. 

+ Updated read_MultiCellDS_XML.m (in ./matlab) to read these 
  newly expanded data files. 

+ Included new matlab functions for fast 3-D data exploration: 

  simple_plot.m  -- quickly plots (as a surface) all the cells 
  simple_cutaway_plot.m -- same as above, but with a cutaway view
  composite_cutaway_plot.m -- plots cutaway view of live and dead
     cells, colored separately. 

+ Wrote new POV-ray functions for future raytracing utilities. See 
  ./modules/PhysiCell_POV.*

+ Add the following new sample projects: 

  biorobots: simulates a system of director, worker, and cargo 
   cells that coordinate to deliver cellular cargo to the 
   directors. Uses motility, custom mechanics, and other functions. 
  
       build via: make biorobots-sample && make project 

  cancer biorobots: modifies the above project so that worker 
    cells take cargo to hypoxic tumor regions. cargo cells 
    release a drug once they are detached from worker cells. 
    tumor cells have a damage and apoptosis model. 

       build via: make cancer-biorobots-sample && make project 

  heterogeneity: simulates proliferation of a heterogeneous 
    tumor. Note the loss of symmetry and selection/evolution.  

       build via: make heterogeneity-sample && make project 

  cancer immunology: simulates an immune system attack on a 
    tumor with heterogeneous proliferation and immunogenicity 
    characteristics. In 3D!  

       build via: make cancer-immune-sample && make project 

Minor new features and changes:

+ Added double NormalRandom( double mean, double standard_deviation ) 
  to PhysiCell_utilities.* This will generate normally-distributed 
  random numbers. 

+ Includes a sneak preview of BioFVM 1.1.7, which includes bugfixes on 
  how boundary conditions were initialized. 

bugfixes: 

+ Removed annoying "cout" lines in:

  void standard_live_phase_entry_function(Cell*,Phenotype&,double). 

  They were leftover from debugging. Sorry! 

+ In the templates, we updated to: 

		while( t < t_max + 0.1*diffusion_dt )
		{

  so that the simulations run all the way to the final time, 
  instead of exiting too early by dt time. 

+ Fixed a bug in Motility::Motility() where the migration bias was 
  initialized to (1,0,0). 

+ Made adhesion and repulsion symmetric across cells (in add_potentials)

+ got rid of debug_temp_a in  (in add_potentials)

+ if default_microenvironment_options.simulate_2D == true, we now 
  set velocity[2] = 0.0 in the velocity functions. 

=======================================================================

Version:      1.2.0
Release date: 30 July 2017

Homepage:     http://PhysiCell.MathCancer.org
Downloads:    http://PhysiCell.sf.net

Release summary: 
This release introduces a major reworking to simplify cell pheno-
types and cells. It includes a generalized cell cycle model that 
can also be used to represent death cycle models. It includes an 
improved motility representation. Changing cell secretion and 
uptake rates is more straightforward, because all interfacing 
with BioFVM is automated. 

A new, comprehensive user guide is included on all the major 
functions and classes in PhysiCell, including examples. 

This release also includes bugfixes and corrections. Due 
to the extensive redevelopment, this change log is not 100% 
fine-grained. 

PhysiCell is currently under scientific peer review. 

Major new features and changes:
+ Completely reworked the Phenoytype class to be much more 
  straightforward, and safe to change throughout the 
  simulation.
  
+ The Phenotype class includes elements on cell cycling, death, 
  volume, geometry, mechanics, motility, and secretion (and uptake). 
  
+ Completely reworked representation of cell cycle models as 
  Cycle_Model: a directed graph of Phases, linked by Phase_Links. 
  A Cycle_Model is bundled with key parameters (Cycle_Data) into 
  the Cycle element of the Phenotype.  

+ Cycle models can include user-specified arrest functions between 
  Phases (e.g., contact inhibition). 

+ Cycle Phases can have user-specified "entry" and "exit" functions 
  to be executed upon entering or leaving a Phase (e.g., mutations). 
 
+ Created new classes Death_Parameters and Death to represent 
  cell death models as Cycle_Model. 
  
+ Updated the classes for Mechanics, Volume, and Geometry. 
 
+ Completely reworked the Motility class to represent cell 
  migration as a combination of random and directed motility. 
  Notions of persistence time are included. Users can supply 
  custom functions to update the cell's migration bias. 
  
+ Added the cell calcification model to the general volume update 
  function. 
  
+ Deprecated all code in core/PhysiCell_digital_cell_line.*
  
+ Added apoptosis_death_model, necrosis_death_model,  
  autophagy_death_model, and live_cells_cycle_model to 
  PhysiCell_Constants.
  
+ Added new function Cell::flag_for_division() for simpler 
  construction of cycle models, and to allow users to push a cell 
  to divide at any time. 
  
+ Added new function Cell::flag_for_removal() for simpler 
  construction of cycle models, and to allow users to push a cell 
  to be removed at any time. 
  
+ Volume::multiply_by_ratio() now also divides the target volumes by 
  this ratio. 
  
+ Created a new Cell_Functions class to more readily bundle the 
  Cell object's function pointers. 
  
+ Rewrote the Custom_Cell_Data class, so that custom data are no 
  longer hard-coded by the user in the class, but instead added 
  during runtime. Moved PhysiCell_custom.h/cpp into the core 
  directory. 
  
+ The Cell_Definition class includes a pointer to the default 
  Microenvironment, as well as a default Cell_Functions, 
  Cell_Parameters, Custom_Cell_Data, and Phenotype. Newly created 
  cells all copy from the Cell_Defaults. 
  
+ Bundled the following phenotype update functions into a single 
  Phenotype::advance_all_nonmechanics_models( Cell* , double ) 
  function: 
    Phenotype::update_function() // update cell phenotype parameters
    Phenotype::volume.update() // update cell volume 
    Phenotype::geometry.update() // update radius, etc. 
    Phenotype::death.check_for_death() // check for death
    Phenotype::cycle.advance_cycle() // advance the current cycle model 
	
+ PhysiCell_Container uses the above function on each cell to 
  ensure that the correct custom functions are used. It also uses 
  the cell's Cell::Cell_Functions::update_velocity() to update 
  the cell's state. For now, users need to remember to include 
  the motility update functions in their update_velocity() function 
  if they replace the default. Lastly, PhysiCell_Container calls teh 
  Cell::Cell_Functions::custom_cell_rule at the end of the main loop.  
  
+ Created new functions to directly trigger cell death. 

+ Created new functions to access cell neighbor information. 

+ Updated the examples for compatibility. 

Minor new features and changes: 
+ Added PhysiCell_constants::custom_phase to the constants. 

+ Changed Cell:is_movable to Cell_is_movable to better express its 
  purpose of setting a cell to rigid (unmovable) or movable. 

+ Fixed Cell:turn_off_reactions(double) to completely turn off all 
  BioFVM uptake/secretion, rather than cut them by an order of 
  magnitude. 
  
+ Replaced scores of (if some_double == 0) conditionals with 
  if( fabs( some_double ) < tolerance ), since you it is best practice 
  to never check for equality of a floating point or double. 
  
+ Started substituting more efficient axpy and += operators 
  (from BioFVM) for many of the vector operations in PhysiCell_cell.cpp. 
  
+ Where possible, inserted { } into if / then / else logic for 
  increased code clarity and robustness to newline and whitespace 
  changes. 
  
+ Moved most default / standard mechanics functions from PhysiCell_cell.* to
  PhysiCell_standard_models.*. 
  
+ Added constants for time step sizes, time units, and spatial units to 
  PhysiCell_constants.h, and removed isolated (and sometimes non-
  synchronized) timestep constants throughout the code. 

+ Added State::simple_pressure for future use in mechanics-induced cycle 
  regulation. 
  
+ Added std::vector<Cell*>& Cell:cells_in_my_container(void) to more 
  easily access a list of cells in the cell's current mechanics voxel. 
  
+ and more. 

+ Simplified the Makefile for easier cross-platform compiling. Users 
  do not need to change MARCH any more. 
 
Bugfixes and Corrections: 

+ Fixed typo "max_cell_interactive_ditstance_in_voxel" to 
  "max_cell_interactive_ditstance_in_voxel" in the Cell_Container class. 

+ Throughout the code, replaced any logic of the form 
  (if some_double == some_other_double ) with better practice of 
  if( fabs( some_double - some_other_double ) < tolerance ), since 
  floating point numbers aren't often equal. 

+ In PhysiCell_cell_container, the function find_escaping_face_index() 
  had a return type of int, but it was possible for none of the statements 
  to evaulate "true". The function now returns -1 for the case that 
  there is no escaping_face_index. Thus, the return type is always defined. 

+ Cell::assign_position() now sets: 
    is_out_of_domain = true; 
    is_active = false; 
    is_movable = false; 
  if the cell is out of bounds. This should prevent segfaults when cells 
  are assigned positions out of bounds (e.g., during division). 
  
+ Cell::update_position() now sets: 
    is_out_of_domain = true; 
    is_active = false; 
    is_movable = false;
  if the cell moves out of bounds. This should prevent segfaults when cells 
  move out of bounds (e.g., by mechanics). 

=======================================================================

Version:      1.1.1
Release date: 16 May 2017

Homepage:     http://PhysiCell.MathCancer.org
Downloads:    http://PhysiCell.sf.net

Release summary: 
This release includes usability fixes and enhancements, including  
more basic "template" projects, simpler project startup, improved SVG 
visualization support, and cleanup on the Makefile. 

PhysiCell is currently under scientific peer review. 

Major new features and changes:
+ Added template3D.cpp and template2D.cpp template projects. See the 
  template_projects directory. 
	
+ Added Makefile rules to seed the 2D projects. To create and compile 
  the 2D template:
		make template2D && make
	To create and compile the 3D template:
		make template3D && make
		
	To further edit your project, modify main.cpp in the root PhysiCell 
	directory. Follow the online tutorials for further functionality. 

+ Added preliminary MultiCellDS support, with MultiCellDS outputs. These 
  are added via modules/PhysiCell_MultiCellDS.cpp

+ Many usability improvements listed in "minor new features" below. 

+ Finished implementation of SVG support, to plot the simulation through a 
  cross-section (fixed z-value). 
  
+ Digital pathology: Improved coloring functions for the main cell cycle 
  models, and virtual H&E (hematoxylin and eosin): 

Minor new features and changes: 
+ Created new function in PhysiCell_cell_container to simplify initialization 
  of the mechanics data structures:

  Cell_Container* create_cell_container_for_microenvironment( 
    BioFVM::Microenvironment& m , double mechanics_voxel_size );

+ Usability feature: If BioFVM::default_microenvironment has not yet been 
  declared when calling create_cell_container_for_microenvironment(), then 
	it is set to "m" in the new function above. 
		
+ Usability feature: If the BioFVM::default_microenvironment has been set
  then Cell* create_cell( void ) now uses this to call 
	Cell::register_microenvironment(&microenvironment). 
	
+ Changed Cell_Parameters from a struct to a class. 
	
+ Usability feature: Created a new Cell_Defaults class, with a 
  global PhysiCell::cell_defaults. Now, you can set these 
  default functions and parameters at the start of your program, and 
  all new cells are set to use these defaults. 
	
+ Usability feature: Traced code and determined that calling Cell::set_phenotype() 
  calls Basic_Agent::set_total_volume() (Cell extends Basic_Agent), which sets 
  Basic_Agent::volume_is_changed to true. This, in turn, makes the next call 
  to Basic_Agent::simulate_secretion_and_uptake() call  
  Basic_Agent::set_internal_uptake_constants(). So, it is unnecessary to 
  call this function in typical initialization. 
	
+ Usability feature: Cell:set_phenotype() now automatically calls  
	Basic_Agent::set_internal_uptake_constants(). You no longer need to 
	manually call this function *if* using the set_phenotype() function. 
	
+ Usability feature: The default Cell constructor (Cell::Cell) uses 
  the default functions in PhysiCell::default_cell_functions, instead 
  of hard-coded defaults. The default constructor for Default_Cell_Functions
  has sensible defaults for cell mechanics and volume regulation to 
  match the PhysiCell method paper. Phenotype-related functions are left 
  empty. 
	
+ Usability feature: The create_cell() function now assigns the default 
  microenvironment to the newly created cell, and assigns the 
  cell functions in PhysiCell::default_cell_functions. 

+ Changed default -march flag to -march=native, according to benchmarks 
  on gcc 5x at phoronix: 
  http://www.phoronix.com/scan.php?page=news_item&px=GCC-Optimizations-E3V5-Levels
  
+ Changed the -O3 flag to -Ofast, which tends to produce slightly faster 
  code by the phoronix link above. 
  
+ Updated to a pre-release copy of BioFVM 1.1.6. 

+ Included the matlab functions first created for BioFVM, which can now be 
  found in the matlab directory. 

+ Fixed read_MultiCellDS_xml.m function to work when there are no cells.

+ Fixed read_MultiCellDS_xml.m function to display the current number of 
  cells, when there are < 3 cells.
  
+ Added modules/PhysiCell_standard_modules.h to start organizing non-core, 
  standard parts of MultiCellDS. This will inintially include SVG, 
  pathology, and MultiCellDS modules. 
  
+ Removed matlab output from log_output in PhysiCell_utilities.cpp. This 
  only saved the microenvironment (but not cells), and it is no longer 
  needed with new MultiCellDS output support.   

+ Changed the default SVG length scale bar in PhysiCell_pathology to 
  100 microns (previously 1000 microns). 
  
+ Updated the archive rules to use the more-common "tar" command. Use 
  "make tar" and "make untar". Archives are stored in the archives 
  directory. 
	
+ Added a void up_orientation( Cell* pCell, double dt ) to 
  PhysiCell_standard_models.cpp, which sets orientation = [0,0,1]
  and polarity = 1.0. This is useful for 2-D simulations. 

+ Digital Pathology coloring functions: 
    simple_cell_coloring: cell nucleus is blue, cytoplasm is red, and 
      outlines are black

    false_cell_coloring_Ki67: for any Ki67-based cell cycle model, 
      green cells are Ki67+ prior to mitosis (or any Ki67+ cell in 
      the simplified Ki67 models), magenta cells are Ki67+ after 
      mitosis, red cells are apoptotic, and brown cells are necrotic. 

    hematoxylin_and_eosin_cell_coloring: "stains" nuclear solids with 
      hematoxylin, "stains" cytoplasmic solids with eosin, and simulates
      light transmission / absorbtion through a thin slice to approximate 
      microscopy and image acquisition. Note that cells with little water 
      will appear dark (e.g., apoptotic debris, especially after the 
      cytoplasm has blebbed), and cells with lots of water (e.g., onsosis 
      in early necrotic cells) will appear faint.  
   
    false_cell_coloring_live_dead: live cells are green, apoptotic cells 
      are red, and necrotic cells are brown. 

+ Added Phenotype::get_current_phase_code(void) to 
  PhysiCell_digital_cell_line.cpp to more easily get the cell's current 
  phenotypic state. (Especially useful for virtual pathology.) 
	
Bugfixes and Corrections: 
+ Fixed typo Time_Settings.cell_cylce_dt_default to 
  Time_Settings.cell_cycle_dt_default in PhysiCell_cell_container.

+ Removed unused declaration Cell::initialize_functions( void );

+ Changed the default "update_cell_and_death_parameters" function from 
  "update_cell_and_death_parameters_O2_based" to "empty_function". 
  Examples and models should choose this *explicitly*. 

+ Cell::set_orientation( Cell* pCell ) changed to   
  Cell::set_orientation( Cell* pCell, double dt ) to be consistent with other 
  cell member functions. 
	
+ In void Cell::assign_orientation(), set polarity = 0.0 if a  
  set_orientation(Cell*,double) function is not set (NULL).
	
+ Removed irrelevant data elements in the Custom_Cell_Data class. 
	
Notices for intended changes that may affect backwards compatibility: 

+ Will rename the current "Phenotype" class to "Old_Phenotype"

+ Will introduce a new Phenotype class with a much simpler structure. 

+ Will rewrite the represetation of cell cycle and death phases. 

+ Will rewrite the standard phenotype models in the simpler representatio. 

+ Will stop requiring use of the Digital_Cell_Line class for initializing 
  simulations. 

+ Will deprecate update_cell_and_death_parameters, and instead use 
  update_phenotype_parameters. 
  
+ Will stop using the oxygen-dependent phenotype rule as default. 

PhysiCell 1.1.1 includes an advance copy of BioFVM 1.1.6. Here are the changes: 
	
/* fixes and changes in BioFVM 1.1.6  */

+ correct typos in citation information in all source files 

+ updated citation information 

+ added void set_default_microenvironment( Microenvironment* M ) declaration to 
  BioFVM_Microenvironment.h
 
+ set volume_is_changed = false in Basic_Agent::set_internal_uptake_constants(); 

+ Set the MultiCellDS options Booleans to extern bool in BioFVM_MultiCellDS.h 
  so that PhysiCell can read these options. 

+ Updated the simlified_data field in MultiCellDS XML output to include a 
  new "source" attribute with value "BioFVM".

+ Added Microenvironment::find_substrate_index( std::string ) to make it 
  easier to find . 
  
+ Added Basic_Agent::nearest_gradient( int substrate_index ) to directly 
  access the gradient of the indicated substrate at the agent's  
  current_voxel_index. 
  
+ Added Basic_Agent::nearest_gradient_vector( void ) to directly 
  access a vector of gradients (one for each substrate in the 
  microenvironment) at the agent's current_voxel_index. 
 
+ Added Microenvironment::is_dirichlet_node( int voxel_index ) to 
  easily check if the voxel is a Dirichlet node. 

+ Updated Microenvironment::update_dirichlet_node() and 
  Microenvironment::remove_dirichlet_node() to check against 
  mesh.voxels[].is_Dirichlet to provide a cheap and early exit 
  if the node isn't Dirichlet in the first place. 
  
+ Changed to a thread-safe data structure for Dirichlet nodes 
  (e.g., if a custom cell function in an OMP loop adds or removes 
  Dirichlet nodes). 
  
+ Added new class Microenvironment_Options, with a default 
  default_microenvironment_options, to simplify Microenvironment
  setup. The defaults are dx = dy = dz = 20 microns, on 1 cubic mm. 
  
+ Added function initialize_microenvironment() to set up the 
  microenvironment using the options in 
  default_microenvironment_options. The code sets oxygen as the 
  default field, with D = 1e5 micron^2/min, and decay rate = 0.1 1/min
  (a 1 mm diffusion length scale). If 
  default_microenvironment_options.outer_Dirichlet_conditions = true, 
  then we set a 38 mmHg condition on the outer edges, corresponding to
  5% oxygenation (physioxic conditions). 
  
=======================================================================

PhysiCell: PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D 
Multicellular Systems.

Version:      1.1.0
Release date: 16 May 2017

Homepage:     http://PhysiCell.MathCancer.org
Downloads:    http://PhysiCell.sf.net

Release summary: 
This release includes usability fixes and enhancements, including  
more basic "template" projects, simpler project startup, improved SVG 
visualization support, and cleanup on the Makefile. 

PhysiCell is currently under scientific peer review. 

Major new features and changes:
+ Added template3D.cpp and template2D.cpp template projects. See the 
  template_projects directory. 
	
+ Added Makefile rules to seed the 2D projects. To create and compile 
  the 2D template:
		make template2D && make
	To create and compile the 3D template:
		make template3D && make
		
	To further edit your project, modify main.cpp in the root PhysiCell 
	directory. Follow the online tutorials for further functionality. 

+ Added preliminary MultiCellDS support, with MultiCellDS outputs. These 
  are added via modules/PhysiCell_MultiCellDS.cpp

+ Many usability improvements listed in "minor new features" below. 

+ Finished implementation of SVG support, to plot the simulation through a 
  cross-section (fixed z-value). 
  
+ Digital pathology: Improved coloring functions for the main cell cycle 
  models, and virtual H&E (hematoxylin and eosin): 

Minor new features and changes: 
+ Created new function in PhysiCell_cell_container to simplify initialization 
  of the mechanics data structures:

  Cell_Container* create_cell_container_for_microenvironment( 
    BioFVM::Microenvironment& m , double mechanics_voxel_size );

+ Usability feature: If BioFVM::default_microenvironment has not yet been 
  declared when calling create_cell_container_for_microenvironment(), then 
	it is set to "m" in the new function above. 
		
+ Usability feature: If the BioFVM::default_microenvironment has been set
  then Cell* create_cell( void ) now uses this to call 
	Cell::register_microenvironment(&microenvironment). 
	
+ Changed Cell_Parameters from a struct to a class. 
	
+ Usability feature: Created a new Cell_Defaults class, with a 
  global PhysiCell::cell_defaults. Now, you can set these 
  default functions and parameters at the start of your program, and 
  all new cells are set to use these defaults. 
	
+ Usability feature: Traced code and determined that calling Cell::set_phenotype() 
  calls Basic_Agent::set_total_volume() (Cell extends Basic_Agent), which sets 
  Basic_Agent::volume_is_changed to true. This, in turn, makes the next call 
  to Basic_Agent::simulate_secretion_and_uptake() call  
  Basic_Agent::set_internal_uptake_constants(). So, it is unnecessary to 
  call this function in typical initialization. 
	
+ Usability feature: Cell:set_phenotype() now automatically calls  
	Basic_Agent::set_internal_uptake_constants(). You no longer need to 
	manually call this function *if* using the set_phenotype() function. 
	
+ Usability feature: The default Cell constructor (Cell::Cell) uses 
  the default functions in PhysiCell::default_cell_functions, instead 
  of hard-coded defaults. The default constructor for Default_Cell_Functions
  has sensible defaults for cell mechanics and volume regulation to 
  match the PhysiCell method paper. Phenotype-related functions are left 
  empty. 
	
+ Usability feature: The create_cell() function now assigns the default 
  microenvironment to the newly created cell, and assigns the 
  cell functions in PhysiCell::default_cell_functions. 

+ Changed default -march flag to -march=native, according to benchmarks 
  on gcc 5x at phoronix: 
  http://www.phoronix.com/scan.php?page=news_item&px=GCC-Optimizations-E3V5-Levels
  
+ Changed the -O3 flag to -Ofast, which tends to produce slightly faster 
  code by the phoronix link above. 
  
+ Updated to a pre-release copy of BioFVM 1.1.5. 

+ Included the matlab functions first created for BioFVM, which can now be 
  found in the matlab directory. 

+ Fixed read_MultiCellDS_xml.m function to work when there are no cells.

+ Fixed read_MultiCellDS_xml.m function to display the current number of 
  cells, when there are < 3 cells.
  
+ Added modules/PhysiCell_standard_modules.h to start organizing non-core, 
  standard parts of MultiCellDS. This will inintially include SVG, 
  pathology, and MultiCellDS modules. 
  
+ Removed matlab output from log_output in PhysiCell_utilities.cpp. This 
  only saved the microenvironment (but not cells), and it is no longer 
  needed with new MultiCellDS output support.   

+ Changed the default SVG length scale bar in PhysiCell_pathology to 
  100 microns (previously 1000 microns). 
  
+ Updated the archive rules to use the more-common "tar" command. Use 
  "make tar" and "make untar". Archives are stored in the archives 
  directory. 
	
+ Added a void up_orientation( Cell* pCell, double dt ) to 
  PhysiCell_standard_models.cpp, which sets orientation = [0,0,1]
  and polarity = 1.0. This is useful for 2-D simulations. 

+ Digital Pathology coloring functions: 
    simple_cell_coloring: cell nucleus is blue, cytoplasm is red, and 
      outlines are black

    false_cell_coloring_Ki67: for any Ki67-based cell cycle model, 
      green cells are Ki67+ prior to mitosis (or any Ki67+ cell in 
      the simplified Ki67 models), magenta cells are Ki67+ after 
      mitosis, red cells are apoptotic, and brown cells are necrotic. 

    hematoxylin_and_eosin_cell_coloring: "stains" nuclear solids with 
      hematoxylin, "stains" cytoplasmic solids with eosin, and simulates
      light transmission / absorbtion through a thin slice to approximate 
      microscopy and image acquisition. Note that cells with little water 
      will appear dark (e.g., apoptotic debris, especially after the 
      cytoplasm has blebbed), and cells with lots of water (e.g., onsosis 
      in early necrotic cells) will appear faint.  
   
    false_cell_coloring_live_dead: live cells are green, apoptotic cells 
      are red, and necrotic cells are brown. 

+ Added Phenotype::get_current_phase_code(void) to 
  PhysiCell_digital_cell_line.cpp to more easily get the cell's current 
  phenotypic state. (Especially useful for virtual pathology.) 
	
Bugfixes and Corrections: 
+ Fixed typo Time_Settings.cell_cylce_dt_default to 
  Time_Settings.cell_cycle_dt_default in PhysiCell_cell_container.

+ Removed unused declaration Cell::initialize_functions( void );

+ Changed the default "update_cell_and_death_parameters" function from 
  "update_cell_and_death_parameters_O2_based" to "empty_function". 
  Examples and models should choose this *explicitly*. 

+ Cell::set_orientation( Cell* pCell ) changed to   
  Cell::set_orientation( Cell* pCell, double dt ) to be consistent with other 
  cell member functions. 
	
+ In void Cell::assign_orientation(), set polarity = 0.0 if a  
  set_orientation(Cell*,double) function is not set (NULL).
	
+ Removed irrelevant data elements in the Custom_Cell_Data class. 
	
Notices for intended changes that may affect backwards compatibility: 

+ Will rename the current "Phenotype" class to "Full_Phenotype"

+ Will introduce a new Phenotype class with a much simpler structure. 

+ Will rewrite the represetation of cell cycle and death phases. 

+ Will rewrite the standard phenotype models in the simpler representatio. 

+ Will stop requiring use of the Digital_Cell_Line class for initializing 
  simulations. 

+ Will deprecate update_cell_and_death_parameters, and instead use 
  update_phenotype_parameters. 
  
+ Will stop using the oxygen-dependent phenotype rule as default. 

PhysiCell 1.1.0 includes an advance copy of BioFVM 1.1.5. Here are the changes: 
	
/* fixes in BioFVM 1.1.5  */

+ correct typos in citation information in all source files 

+ updated citation information 

+ added void set_default_microenvironment( Microenvironment* M ) declaration to 
  BioFVM_Microenvironment.h
 
+ set volume_is_changed = false in Basic_Agent::set_internal_uptake_constants(); 

+ Set the MultiCellDS options Booleans to extern bool in BioFVM_MultiCellDS.h 
  so that PhysiCell can read these options. 

+ Updated the simlified_data field in MultiCellDS XML output to include a 
  new "source" attribute with value "BioFVM".
  
=======================================================================
	
PhysiCell: PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D 
Multicellular Systems.

Version:      1.0.0
Release date: 12 September 2016

Homepage:     http://PhysiCell.MathCancer.org
Downloads:    http://PhysiCell.sf.net

Release summary: 
This is the initial public release of PhysiCell: an Open Source Physics-Based 
Cell Simulator for 3-D Multicellular Systems. It is currently under scientific 
peer review. 

Major new features:
+ Simulating large systems of cells in 3-D tissues on desktop machine
+ Cells move and change based on biomechanical and physical rules
+ Built upon BioFVM to couple processes with the substrates' concentrations 
  in the microenvironment 
+ Implements multiple realistic cell cycle and cell death models
+ Performance linearly scales with the number of the cells
+ Preliminary support for MultiCellDS (http://MultiCellDS.org)
+ Includes examples for 3-D ductal carcinoma in situ (DCIS) and hanging 
  drop tumor spheroids 

Bugfixes: 
+ Not applicable. 

=======================================================================

PhysiCell: an open source physics-based multicell simulator

Version:      0.5.0
Release date: 2 Auguust

Homepage:     http://PhysiCell.MathCancer.org
Downloads:    http://PhysiCell.sf.net

Release summary:  
Pre-release. Not for public use. Not supported.  

New features:
+ First code bundling. 

Bugfixes: 
+ Nothing to see here. Move along. 

