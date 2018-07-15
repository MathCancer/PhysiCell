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

Summary: 
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

