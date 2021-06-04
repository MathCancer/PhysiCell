
on OSX:
$ DYLD_INSERT_LIBRARIES=./addons/libRoadrunner/roadrunner/lib/libroadrunner_c_api.dylib ode1



-----------------------
~/git/rheiland_development-intracellular$ ty VERSION.txt
1.9.0-dev
~/git/PhysiCell-SBML-trials_Furkan/PhysiCell_intracellular_Phenotype$ ty VERSION.txt
1.7.1

~/git/PhysiCell-SBML-trials_Furkan/PhysiCell_intracellular_Phenotype/addons/libRoadrunner$ cp -R src ~/git/rheiland_development-intracellular/addons/libRoadrunner/

~/git/PhysiCell-SBML-trials_Furkan/PhysiCell_intracellular_Phenotype/addons/PhysiBoSSa$ cp -R * ~/git/rheiland_development-intracellular/addons/PhysiBoSSa/

~/git/rheiland_development-intracellular/core$ ll *pheno*
-rw-r--r--  1 heiland  staff  29609 May 25 17:06 PhysiCell_phenotype-0.cpp
-rw-r--r--  1 heiland  staff  29609 May 20 22:03 PhysiCell_phenotype.cpp
-rw-r--r--  1 heiland  staff  18586 May 20 22:03 PhysiCell_phenotype.h
~/git/rheiland_development-intracellular/core$ cp PhysiCell_phenotype.h PhysiCell_phenotype-0.h

~/git/PhysiCell-SBML-trials_Furkan/PhysiCell_intracellular_Phenotype/core$ cp PhysiCell_phenotype.* ~/git/rheiland_development-intracellular/core


be careful not to overwrite relevant sample models?

~/git/PhysiCell-SBML-trials_Furkan/PhysiCell_intracellular_Phenotype$ ls sample_projects/
Arnau_model/                    covid19/                        sorting/
ECM_try/                        drug_AGS/                       spheroid_TNF/
Makefile-default                drug_AGS_baseline/              spheroid_TNF_v2/
TNFpulses_3D/                   full_parameter_spheroid_TNF/    template/
beta_testing/                   heterogeneity/                  template2D/
biorobots/                      mycovid19/                      template3D/
cancer_biorobots/               physiboss_cell_lines/           test_sbml1/
cancer_immune/                  roadrunner_simple1/             virus_macrophage/


~/git/rheiland_development-intracellular/sample_projects$ ls
Makefile-default        cancer_immune/          template/               virus_macrophage/
biorobots/              heterogeneity/          template2D/             worm/
cancer_biorobots/       pred_prey_farmer/       template3D/

-----
~/git/rheiland_development-intracellular$ ls
BioFVM/                 addons/                 documentation/          povray/
CITATION.txt            beta/                   examples/               protocols/
Makefile                changes.md              licenses/               sample_projects/
Quickstart.pdf          config/                 matlab/                 tests/
README.md               core/                   modules/                unit_tests/
VERSION.txt             custom_modules/         output/

~/git/PhysiCell-SBML-trials_Furkan/PhysiCell_intracellular_Phenotype$ cp Make-* ~/git/rheiland_development-intracellular

