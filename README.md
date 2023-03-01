# PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D Multicellular Systems

**Version:** 1.11.0

**Release date:** ?? February 2023

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
Version 1.11.0 ... 





primarily fixes bugs in file output and the ode-energy sample, and refines thread safety in cell phagocytosis. 

The 1.10.0 release introduced major new phenotype functionality, including standardized support for cell-cell interactions (phagocytosis, cell attack that increases a tracked damage variable, and cell fusion), cell transformations, advanced chemotaxis, and cell adhesion affinities for preferential adhesion. This release also includes new, auto-generated "dictionaries" of signals and behaviors to facilitate writing cell behavioral models and intracellular models, as well as standardized Hill and linear response functions for use in intracellular models. Lastly, this release includes a number of bugfixes, most notably pseudorandom number generators with improved thread safety. 

A blog post and tutorial on the new phenotype elements can be found at http://www.mathcancer.org/blog/introducing-cell-interactions-and-transformations.  
A blog post and tutorial on the new signal and behavior dictionaries can be found at http://www.mathcancer.org/blog/introducing-cell-signal-and-behavior-dictionaries. 

**NOTE 1:** MacOS users need to define a PHYSICELL_CPP environment variable to specify their OpenMP-enabled g++. See the [Quickstart](documentation/Quickstart.md) for details.

**NOTE 2:** Windows users need to follow an updated (from v1.8) MinGW64 installation procedure. This will install an updated version of g++, plus libraries that are needed for some of the intracellular models. See the [Quickstart](documentation/Quickstart.md) for details.
 
### Major new features and changes in the 1.10.z versions
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
  $$\textrm{Probability cell } i \textrm{ attacks cell } j \textrm{ in } [t,t+\Delta t] = \textrm{attack}\_{ij} \cdot \textrm{immunogenicity}\_{ji} \Delta t $$. 
  
  By setting $\textrm{immunogenicity}\_{ji} = 1$ as teh default value, we maintain compatibiltiy with prior models. This is a way to further modulate immunogenic and cytotoxic interactions. 

+ Migrated built-in sample projects to be compatible with the model builder GUI and the upcoming PhysiCell Studio, including: 
  + template
  + biorobots (updates spring constant from 0.05 to 0.5)
  + heterogeneity
  + cancer biorobots (updates spring constant from 0.05 to 0.5)

### Minor new features and changes: 
#### 1.11.0
+ New `apoptotic` and `necrotic` cell signals are available for use in simulation rules. 
+ Added `is_movable` to the set of cell behaviors. Set this to 0 (false) to make an agent rigid: it will exert forces on other cells, but it itself cannot be moved. 


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
 
### Bugfixes: 

#### 1.11.0
+ Fixed bug in cancer biorobots project that mistakenly set max cancer cell proliferation rate to 0.000072 instead of 0.00072


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
