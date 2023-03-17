# PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D Multicellular Systems

**Version:** 1.11.0

**Release date:** 18 March 2023

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
Version 1.11.0 ... 





primarily fixes bugs in file output and the ode-energy sample, and refines thread safety in cell phagocytosis. 

The 1.10.0 release introduced major new phenotype functionality, including standardized support for cell-cell interactions (phagocytosis, cell attack that increases a tracked damage variable, and cell fusion), cell transformations, advanced chemotaxis, and cell adhesion affinities for preferential adhesion. This release also includes new, auto-generated "dictionaries" of signals and behaviors to facilitate writing cell behavioral models and intracellular models, as well as standardized Hill and linear response functions for use in intracellular models. Lastly, this release includes a number of bugfixes, most notably pseudorandom number generators with improved thread safety. 

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
  + This automated spring functionality is completely independent of (and does not interfer with) the user-defined contact function and user-manageed `cell.state.attached` data structure. 
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

### Minor new features and changes: 
#### 1.11.0
+ Updated the `paint_by_number_cell_coloring` coloring function to paint the entire cell white if apoptotic, and brown if necrotic. (Previously, we colored the nucleus based on live/dead status.) This improves compatibility with the model GUI. 

+ Changed the default value of `attachment_rate` from 10 to 0 (in the `Mechanics` class) to avoid unexpected triggering of automated spring adheions. 

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
 
### Bugfixes: 

#### 1.11.0
+ Fixed bug in cancer biorobots project that mistakenly set max cancer cell proliferation rate to 0.000072 instead of 0.00072

+ Fixed multiple broken signals/behaviors. 

+ Fixed calcification bug reported in issue [133](https://github.com/MathCancer/PhysiCell/issues/133). Thank you, @JulianoGianlupi! 

+ Fixed typo in cell signals that used `contact with dead dell` instead of `contact with dead cell`

+ Changed default full data output to 60 minutes (to match the SVG output interval) for better compatibility with the model builder GUI. 

+ Fixed incorrect parameters for necrotic cell volume changes that prevented necrotic cell lysis and shrinkage. 

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
