# PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D Multicellular Systems
**Versions:** 1.12.0 - 

**Release dates:** 15 May 2023 - 
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
