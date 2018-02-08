PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D 
Multicellular Systems.

Version:      1.2.3
Release date: ? February 2018 

Overview: 
PhysiCell is a flexible open source framework for building 
agent-based multicellular models in 3-D tissue environments. 

Reference: Ghaffarizadeh et al., PLoS Comput Biol (2018, accepted)
   preprint URL: https://doi.org/10.1101/088773
   future DOI: 10.1371/journal.pcbi.1005991

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

-=-=-=-=-=-=-=-=-
Release notes: 

PhysiCell: an Open Source Physics-Based Cell Simulator for 3-D 
Multicellular Systems.

Version:      1.2.3
Release date: ? February 2018 

Summary: 
This release ...

NOTE: OSX users must now define PHYSICELL_CPP system variable. 
      See the documentation.

PhysiCell is in press at PLoS. Comput. Biol. 


***** COPY IN 1.2.3 changelog before release! *****


