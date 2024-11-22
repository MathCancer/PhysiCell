VERSION := $(shell grep . VERSION.txt | cut -f1 -d:)
PROGRAM_NAME := spheroid_TNF_model

CC := g++
# CC := g++-mp-7 # typical macports compiler name
# CC := g++-7 # typical homebrew compiler name 

# Check for environment definitions of compiler 
# e.g., on CC = g++-7 on OSX
ifdef PHYSICELL_CPP 
	CC := $(PHYSICELL_CPP)
endif

### MaBoSS configuration 
# MaBoSS max nodes
ifndef MABOSS_MAX_NODES
MABOSS_MAX_NODES = 64
endif

# MaBoSS directory
MABOSS_DIR = addons/PhysiBoSS/MaBoSS/engine
CUR_DIR = $(shell pwd)

ifneq ($(OS), Windows_NT)
	LDL_FLAG = -ldl
endif

LIB := -L$(CUR_DIR)/$(MABOSS_DIR)/lib -lMaBoSS-static $(LDL_FLAG)
INC := -DADDON_PHYSIBOSS -I$(CUR_DIR)/$(MABOSS_DIR)/include -DMAXNODES=$(MABOSS_MAX_NODES)


# If max nodes > 64, change lib path 
ifeq ($(shell expr $(MABOSS_MAX_NODES) '>' 64), 1)
LIB := -L$(CUR_DIR)/$(MABOSS_DIR)/lib -lMaBoSS_$(MABOSS_MAX_NODES)n-static $(LDL_FLAG)
endif

ARCH := native # best auto-tuning
# ARCH := core2 # a reasonably safe default for most CPUs since 2007
# ARCH := corei7
# ARCH := corei7-avx # earlier i7 
# ARCH := core-avx-i # i7 ivy bridge or newer 
# ARCH := core-avx2 # i7 with Haswell or newer
# ARCH := nehalem
# ARCH := westmere
# ARCH := sandybridge # circa 2011
# ARCH := ivybridge   # circa 2012
# ARCH := haswell     # circa 2013
# ARCH := broadwell   # circa 2014
# ARCH := skylake     # circa 2015
# ARCH := bonnell
# ARCH := silvermont
# ARCH := skylake-avx512
# ARCH := nocona #64-bit pentium 4 or later 

# CFLAGS := -march=$(ARCH) -Ofast -s -fomit-frame-pointer -mfpmath=both -fopenmp -m64 -std=c++11
CFLAGS := -march=$(ARCH) -O3 -fomit-frame-pointer -mfpmath=both -fopenmp -m64 -std=c++11

ifeq ($(OS),Windows_NT)
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Darwin)
		UNAME_P := $(shell uname -p)
		var := $(shell which $(CC) | xargs file)
		ifeq ($(lastword $(var)),arm64)
		  CFLAGS := -march=$(ARCH) -O3 -fomit-frame-pointer -fopenmp -m64 -std=c++11
		endif
	endif
endif

COMPILE_COMMAND := $(CC) $(CFLAGS) 

BioFVM_OBJECTS := BioFVM_vector.o BioFVM_mesh.o BioFVM_microenvironment.o BioFVM_solvers.o BioFVM_matlab.o \
BioFVM_utilities.o BioFVM_basic_agent.o BioFVM_MultiCellDS.o BioFVM_agent_container.o 

PhysiCell_core_OBJECTS := PhysiCell_phenotype.o PhysiCell_cell_container.o PhysiCell_standard_models.o \
PhysiCell_cell.o PhysiCell_custom.o PhysiCell_utilities.o PhysiCell_constants.o PhysiCell_basic_signaling.o \
PhysiCell_signal_behavior.o PhysiCell_rules.o

PhysiCell_module_OBJECTS := PhysiCell_SVG.o PhysiCell_pathology.o PhysiCell_MultiCellDS.o PhysiCell_various_outputs.o \
PhysiCell_pugixml.o PhysiCell_settings.o PhysiCell_geometry.o

# put your custom objects here (they should be in the custom_modules directory)  
MaBoSS := ./addons/PhysiBoSS/MaBoSS/engine/src/BooleanNetwork.h

PhysiBoSS_OBJECTS := maboss_network.o maboss_intracellular.o

PhysiCell_custom_module_OBJECTS := custom.o submodel_data_structures.o tnf_receptor_dynamics.o tnf_boolean_model_interface.o

pugixml_OBJECTS := pugixml.o

PhysiCell_OBJECTS := $(BioFVM_OBJECTS)  $(pugixml_OBJECTS) $(PhysiCell_core_OBJECTS) $(PhysiCell_module_OBJECTS)

start_and_stop_OBJECTS := start_and_stop.o
ALL_OBJECTS := $(PhysiCell_OBJECTS) $(PhysiCell_custom_module_OBJECTS) $(PhysiBoSS_OBJECTS) $(PhysiBoSS_module_OBJECTS) $(start_and_stop_OBJECTS)

# compile the project 

all: main.cpp $(ALL_OBJECTS) $(MaBoSS)
	$(COMPILE_COMMAND) $(INC)  -o $(PROGRAM_NAME) $(ALL_OBJECTS) main.cpp $(LIB)
	@echo ""
	@echo "Executable name is" $(PROGRAM_NAME)
	@echo ""

# sample projects 	
list-projects:
	@echo "Sample projects: template biorobots-sample cancer-biorobots-sample cancer-immune-sample"
	@echo "                 celltypes3-sample heterogeneity-sample pred-prey-farmer virus-macrophage-sample"
	@echo "                 worm-sample interaction-sample mechano-sample rules-sample physimess-sample custom-division-sample"
	@echo "					asymmetric-division-sample"
	@echo ""
	@echo "Sample intracellular projects: template_BM ode-energy-sample physiboss-cell-lines-sample"
	@echo "                 cancer-metabolism-sample physiboss-tutorial physiboss-tutorial-invasion"
	@echo ""
	
template:
	cp -r ./sample_projects/template/custom_modules/* ./custom_modules/
	touch main.cpp && cp main.cpp main-backup.cpp
	cp ./sample_projects/template/main.cpp ./main.cpp 
	cp Makefile Makefile-backup
	cp ./sample_projects/template/Makefile .
	cp -r ./sample_projects/template/config/* ./config 
	
# sample projects 

# ---- non-intracellular projects 
biorobots-sample:
	cp ./sample_projects/biorobots/custom_modules/* ./custom_modules/
	touch main.cpp && cp main.cpp main-backup.cpp
	cp ./sample_projects/biorobots/main.cpp ./main.cpp 
	cp Makefile Makefile-backup
	cp ./sample_projects/biorobots/Makefile .
	cp ./config/PhysiCell_settings.xml ./config/PhysiCell_settings-backup.xml 
	cp ./sample_projects/biorobots/config/* ./config/
	
cancer-biorobots-sample:
	cp ./sample_projects/cancer_biorobots/custom_modules/* ./custom_modules/
	touch main.cpp && cp main.cpp main-backup.cpp
	cp ./sample_projects/cancer_biorobots/main.cpp ./main.cpp 
	cp Makefile Makefile-backup
	cp ./sample_projects/cancer_biorobots/Makefile .
	cp ./config/PhysiCell_settings.xml ./config/PhysiCell_settings-backup.xml 
	cp ./sample_projects/cancer_biorobots/config/* ./config/
	
cancer-immune-sample:
	cp ./sample_projects/cancer_immune/custom_modules/* ./custom_modules/
	touch main.cpp && cp main.cpp main-backup.cpp
	cp ./sample_projects/cancer_immune/main-cancer_immune_3D.cpp ./main.cpp 
	cp Makefile Makefile-backup
	cp ./sample_projects/cancer_immune/Makefile .
	cp ./config/PhysiCell_settings.xml ./config/PhysiCell_settings-backup.xml 
	cp ./sample_projects/cancer_immune/config/* ./config/

celltypes3-sample:
	cp ./sample_projects/celltypes3/custom_modules/* ./custom_modules/
	touch main.cpp && cp main.cpp main-backup.cpp
	cp ./sample_projects/celltypes3/main.cpp ./main.cpp 
	cp Makefile Makefile-backup
	cp ./sample_projects/celltypes3/Makefile .
	cp ./config/PhysiCell_settings.xml ./config/PhysiCell_settings-backup.xml 
	cp ./sample_projects/celltypes3/config/* ./config/	
	
heterogeneity-sample:
	cp ./sample_projects/heterogeneity/custom_modules/* ./custom_modules/
	touch main.cpp && cp main.cpp main-backup.cpp
	cp ./sample_projects/heterogeneity/main.cpp ./main.cpp 
	cp Makefile Makefile-backup
	cp ./sample_projects/heterogeneity/Makefile .
	cp ./config/PhysiCell_settings.xml ./config/PhysiCell_settings-backup.xml 
	cp ./sample_projects/heterogeneity/config/* ./config/
	
pred-prey-farmer:
	cp ./sample_projects/pred_prey_farmer/custom_modules/* ./custom_modules/
	touch main.cpp && cp main.cpp main-backup.cpp
	cp ./sample_projects/pred_prey_farmer/main.cpp ./main.cpp 
	cp Makefile Makefile-backup
	cp ./sample_projects/pred_prey_farmer/Makefile .
	cp ./config/PhysiCell_settings.xml ./config/PhysiCell_settings-backup.xml 
	cp ./sample_projects/pred_prey_farmer/config/* ./config/	
	
virus-macrophage-sample:
	cp ./sample_projects/virus_macrophage/custom_modules/* ./custom_modules/
	touch main.cpp && cp main.cpp main-backup.cpp
	cp ./sample_projects/virus_macrophage/main.cpp ./main.cpp 
	cp Makefile Makefile-backup
	cp ./sample_projects/virus_macrophage/Makefile .
	cp ./config/PhysiCell_settings.xml ./config/PhysiCell_settings-backup.xml 
	cp ./sample_projects/virus_macrophage/config/* ./config/
	
worm-sample:
	cp ./sample_projects/worm/custom_modules/* ./custom_modules/
	touch main.cpp && cp main.cpp main-backup.cpp
	cp ./sample_projects/worm/main.cpp ./main.cpp 
	cp Makefile Makefile-backup
	cp ./sample_projects/worm/Makefile .
	cp ./config/PhysiCell_settings.xml ./config/PhysiCell_settings-backup.xml 
	cp ./sample_projects/worm/config/* ./config/
	
interaction-sample:
	cp ./sample_projects/interactions/custom_modules/* ./custom_modules/
	touch main.cpp && cp main.cpp main-backup.cpp
	cp ./sample_projects/interactions/main.cpp ./main.cpp 
	cp Makefile Makefile-backup
	cp ./sample_projects/interactions/Makefile .
	cp ./config/PhysiCell_settings.xml ./config/PhysiCell_settings-backup.xml 
	cp ./sample_projects/interactions/config/* ./config/

mechano-sample:
	cp ./sample_projects/mechano/custom_modules/* ./custom_modules/
	touch main.cpp && cp main.cpp main-backup.cpp
	cp ./sample_projects/mechano/main.cpp ./main.cpp 
	cp Makefile Makefile-backup
	cp ./sample_projects/mechano/Makefile .
	cp ./sample_projects/mechano/config/* ./config/

rules-sample:
	cp ./sample_projects/rules_sample/custom_modules/* ./custom_modules/
	touch main.cpp && cp main.cpp main-backup.cpp
	cp ./sample_projects/rules_sample/main.cpp ./main.cpp 
	cp Makefile Makefile-backup
	cp ./sample_projects/rules_sample/Makefile .
	cp ./config/PhysiCell_settings.xml ./config/PhysiCell_settings-backup.xml 
	cp ./sample_projects/rules_sample/config/* ./config/

physimess-sample:
	cp ./sample_projects/physimess/custom_modules/* ./custom_modules/
	touch main.cpp && cp main.cpp main-backup.cpp
	cp ./sample_projects/physimess/main.cpp ./main.cpp 
	cp Makefile Makefile-backup
	cp ./sample_projects/physimess/Makefile .
	cp ./config/PhysiCell_settings.xml ./config/PhysiCell_settings-backup.xml 
	cp -r ./sample_projects/physimess/config/* ./config/

custom-division-sample:
	cp -r ./sample_projects/custom_division/custom_modules/* ./custom_modules/
	touch main.cpp && cp main.cpp main-backup.cpp
	cp ./sample_projects/custom_division/main.cpp ./main.cpp 
	cp Makefile Makefile-backup
	cp ./sample_projects/custom_division/Makefile .
	cp ./config/PhysiCell_settings.xml ./config/PhysiCell_settings-backup.xml 
	cp -r ./sample_projects/custom_division/config/* ./config/

asymmetric-division-sample:
	cp -r ./sample_projects/asymmetric_division/custom_modules/* ./custom_modules/
	touch main.cpp && cp main.cpp main-backup.cpp
	cp ./sample_projects/asymmetric_division/main.cpp ./main.cpp 
	cp Makefile Makefile-backup
	cp ./sample_projects/asymmetric_division/Makefile .
	cp ./config/PhysiCell_settings.xml ./config/PhysiCell_settings-backup.xml 
	cp -r ./sample_projects/asymmetric_division/config/* ./config/

# ---- intracellular projects 
ode-energy-sample:
	cp ./sample_projects_intracellular/ode/ode_energy/custom_modules/* ./custom_modules/
	touch main.cpp && cp main.cpp main-backup.cpp
	cp ./sample_projects_intracellular/ode/ode_energy/main.cpp ./main.cpp 
	cp Makefile Makefile-backup
	cp ./sample_projects_intracellular/ode/ode_energy/Makefile .
	cp ./config/PhysiCell_settings.xml ./config/PhysiCell_settings-backup.xml 
	cp ./sample_projects_intracellular/ode/ode_energy/config/* ./config/	

physiboss-cell-lines-sample:
	cp ./sample_projects_intracellular/boolean/physiboss_cell_lines/custom_modules/* ./custom_modules/
	touch main.cpp && cp main.cpp main-backup.cpp
	cp ./sample_projects_intracellular/boolean/physiboss_cell_lines/main.cpp ./main.cpp 
	cp Makefile Makefile-backup
	cp ./sample_projects_intracellular/boolean/physiboss_cell_lines/Makefile .
	cp ./config/PhysiCell_settings.xml ./config/PhysiCell_settings-backup.xml 
	cp ./sample_projects_intracellular/boolean/physiboss_cell_lines/config/* ./config/

physiboss-tutorial:
	cp ./sample_projects_intracellular/boolean/tutorial/custom_modules/* ./custom_modules/
	touch main.cpp && cp main.cpp main-backup.cpp
	cp ./sample_projects_intracellular/boolean/tutorial/main.cpp ./main.cpp 
	cp Makefile Makefile-backup
	cp ./sample_projects_intracellular/boolean/tutorial/Makefile .
	cp ./config/PhysiCell_settings.xml ./config/PhysiCell_settings-backup.xml 
	cp -r ./sample_projects_intracellular/boolean/tutorial/config/* ./config/

physiboss-tutorial-invasion:
	cp ./sample_projects_intracellular/boolean/cancer_invasion/custom_modules/* ./custom_modules/
	touch main.cpp && cp main.cpp main-backup.cpp
	cp ./sample_projects_intracellular/boolean/cancer_invasion/main.cpp ./main.cpp 
	cp Makefile Makefile-backup
	cp ./sample_projects_intracellular/boolean/cancer_invasion/Makefile .
	cp ./config/PhysiCell_settings.xml ./config/PhysiCell_settings-backup.xml 
	cp -r ./sample_projects_intracellular/boolean/cancer_invasion/config/* ./config/

ecoli-acetic-switch-sample:
	cp ./sample_projects_intracellular/fba/ecoli_acetic_switch/custom_modules/* ./custom_modules/
	touch main.cpp && cp main.cpp main-backup.cpp
	cp ./sample_projects_intracellular/fba/ecoli_acetic_switch/main_ecoli_acetic_switch.cpp ./main.cpp
	cp Makefile Makefile-backup
	cp ./sample_projects_intracellular/fba/ecoli_acetic_switch/Makefile ./
	cp ./config/PhysiCell_settings.xml ./config/PhysiCell_settings-backup.xml
	cp ./sample_projects_intracellular/fba/ecoli_acetic_switch/config/* ./config/

cancer-metabolism-sample:
	cp ./sample_projects_intracellular/fba/cancer_metabolism/custom_modules/* ./custom_modules/
	touch main.cpp && cp main.cpp main-backup.cpp
	cp ./sample_projects_intracellular/fba/cancer_metabolism/main.cpp ./main.cpp
	cp Makefile Makefile-backup
	cp ./sample_projects_intracellular/fba/cancer_metabolism/Makefile ./
	cp ./config/PhysiCell_settings.xml ./config/PhysiCell_settings-backup.xml
	cp ./sample_projects_intracellular/fba/cancer_metabolism/config/* ./config/

template_BM:
	cp ./sample_projects_intracellular/boolean/template_BM/custom_modules/* ./custom_modules/
	touch main.cpp && cp main.cpp main-backup.cpp
	cp ./sample_projects_intracellular/boolean/template_BM/main.cpp ./main.cpp 
	cp Makefile Makefile-backup
	cp ./sample_projects_intracellular/boolean/template_BM/Makefile .
	cp ./config/PhysiCell_settings.xml ./config/PhysiCell_settings-backup.xml 
	cp -r ./sample_projects_intracellular/boolean/template_BM/config/* ./config/
	mkdir ./scripts/
	cp ./sample_projects_intracellular/boolean/template_BM/scripts/* ./scripts/

# early examples for convergence testing 

physicell_test_mech1: $(PhysiCell_OBJECTS) ./examples/PhysiCell_test_mechanics_1.cpp 
	$(COMPILE_COMMAND) -o test_mech1 $(PhysiCell_OBJECTS) ./examples/PhysiCell_test_mechanics_1.cpp

physicell_test_mech2: $(PhysiCell_OBJECTS) ./examples/PhysiCell_test_mechanics_2.cpp 
	$(COMPILE_COMMAND) -o test_mech2 $(PhysiCell_OBJECTS) ./examples/PhysiCell_test_mechanics_2.cpp
	
physicell_test_DCIS: $(PhysiCell_OBJECTS) ./examples/PhysiCell_test_DCIS.cpp 
	$(COMPILE_COMMAND) -o test_DCIS $(PhysiCell_OBJECTS) ./examples/PhysiCell_test_DCIS.cpp

physicell_test_HDS: $(PhysiCell_OBJECTS) ./examples/PhysiCell_test_HDS.cpp 
	$(COMPILE_COMMAND) -o test_HDS $(PhysiCell_OBJECTS) ./examples/PhysiCell_test_HDS.cpp

physicell_test_cell_cycle: $(PhysiCell_OBJECTS) ./examples/PhysiCell_test_cell_cycle.cpp 
	$(COMPILE_COMMAND) -o test_cycle $(PhysiCell_OBJECTS) ./examples/PhysiCell_test_cell_cycle.cpp

PhysiCell_test_volume: $(PhysiCell_OBJECTS) ./examples/PhysiCell_test_volume.cpp 
	$(COMPILE_COMMAND) -o test_volume $(PhysiCell_OBJECTS) ./examples/PhysiCell_test_volume.cpp
	
examples: $(PhysiCell_OBJECTS) 
	$(COMPILE_COMMAND) -o ./examples/test_mech1 ./examples/PhysiCell_test_mechanics_1.cpp $(PhysiCell_OBJECTS)
	$(COMPILE_COMMAND) -o ./examples/test_mech2 ./examples/PhysiCell_test_mechanics_2.cpp $(PhysiCell_OBJECTS)
	$(COMPILE_COMMAND) -o ./examples/test_DCIS ./examples/PhysiCell_test_DCIS.cpp $(PhysiCell_OBJECTS)
	$(COMPILE_COMMAND) -o ./examples/test_HDS ./examples/PhysiCell_test_HDS.cpp $(PhysiCell_OBJECTS)
	$(COMPILE_COMMAND) -o ./examples/test_cycle ./examples/PhysiCell_test_cell_cycle.cpp $(PhysiCell_OBJECTS)
	$(COMPILE_COMMAND) -o ./examples/test_volume ./examples/PhysiCell_test_volume.cpp $(PhysiCell_OBJECTS)

# PhysiCell core components	

PhysiCell_phenotype.o: ./core/PhysiCell_phenotype.cpp
	$(COMPILE_COMMAND) -c ./core/PhysiCell_phenotype.cpp
	
PhysiCell_digital_cell_line.o: ./core/PhysiCell_digital_cell_line.cpp
	$(COMPILE_COMMAND) -c ./core/PhysiCell_digital_cell_line.cpp

PhysiCell_cell.o: ./core/PhysiCell_cell.cpp $(MaBoSS)
	$(COMPILE_COMMAND) $(INC) -c ./core/PhysiCell_cell.cpp 

PhysiCell_cell_container.o: ./core/PhysiCell_cell_container.cpp
	$(COMPILE_COMMAND) -c ./core/PhysiCell_cell_container.cpp 
	
PhysiCell_standard_models.o: ./core/PhysiCell_standard_models.cpp
	$(COMPILE_COMMAND) -c ./core/PhysiCell_standard_models.cpp 
	
PhysiCell_utilities.o: ./core/PhysiCell_utilities.cpp
	$(COMPILE_COMMAND) -c ./core/PhysiCell_utilities.cpp 
	
PhysiCell_custom.o: ./core/PhysiCell_custom.cpp
	$(COMPILE_COMMAND) -c ./core/PhysiCell_custom.cpp 

PhysiCell_constants.o: ./core/PhysiCell_constants.cpp
	$(COMPILE_COMMAND) -c ./core/PhysiCell_constants.cpp
	
PhysiCell_signal_behavior.o: ./core/PhysiCell_signal_behavior.cpp
	$(COMPILE_COMMAND) -c ./core/PhysiCell_signal_behavior.cpp 

PhysiCell_rules.o: ./core/PhysiCell_rules.cpp
	$(COMPILE_COMMAND) -c ./core/PhysiCell_rules.cpp 

# BioFVM core components (needed by PhysiCell)
	
BioFVM_vector.o: ./BioFVM/BioFVM_vector.cpp
	$(COMPILE_COMMAND) -c ./BioFVM/BioFVM_vector.cpp 

BioFVM_agent_container.o: ./BioFVM/BioFVM_agent_container.cpp
	$(COMPILE_COMMAND) -c ./BioFVM/BioFVM_agent_container.cpp 
	
BioFVM_mesh.o: ./BioFVM/BioFVM_mesh.cpp
	$(COMPILE_COMMAND) -c ./BioFVM/BioFVM_mesh.cpp 

BioFVM_microenvironment.o: ./BioFVM/BioFVM_microenvironment.cpp
	$(COMPILE_COMMAND) -c ./BioFVM/BioFVM_microenvironment.cpp 

BioFVM_solvers.o: ./BioFVM/BioFVM_solvers.cpp
	$(COMPILE_COMMAND) -c ./BioFVM/BioFVM_solvers.cpp 

BioFVM_utilities.o: ./BioFVM/BioFVM_utilities.cpp
	$(COMPILE_COMMAND) -c ./BioFVM/BioFVM_utilities.cpp 
	
BioFVM_basic_agent.o: ./BioFVM/BioFVM_basic_agent.cpp
	$(COMPILE_COMMAND) -c ./BioFVM/BioFVM_basic_agent.cpp 
	
BioFVM_matlab.o: ./BioFVM/BioFVM_matlab.cpp
	$(COMPILE_COMMAND) -c ./BioFVM/BioFVM_matlab.cpp

BioFVM_MultiCellDS.o: ./BioFVM/BioFVM_MultiCellDS.cpp
	$(COMPILE_COMMAND) -c ./BioFVM/BioFVM_MultiCellDS.cpp
	
pugixml.o: ./BioFVM/pugixml.cpp
	$(COMPILE_COMMAND) -c ./BioFVM/pugixml.cpp
	
# standard PhysiCell modules

PhysiCell_SVG.o: ./modules/PhysiCell_SVG.cpp
	$(COMPILE_COMMAND) -c ./modules/PhysiCell_SVG.cpp

PhysiCell_pathology.o: ./modules/PhysiCell_pathology.cpp
	$(COMPILE_COMMAND) -c ./modules/PhysiCell_pathology.cpp

PhysiCell_MultiCellDS.o: ./modules/PhysiCell_MultiCellDS.cpp
	$(COMPILE_COMMAND) -c ./modules/PhysiCell_MultiCellDS.cpp

PhysiCell_various_outputs.o: ./modules/PhysiCell_various_outputs.cpp
	$(COMPILE_COMMAND) -c ./modules/PhysiCell_various_outputs.cpp

	
PhysiCell_pugixml.o: ./modules/PhysiCell_pugixml.cpp
	$(COMPILE_COMMAND) -c ./modules/PhysiCell_pugixml.cpp
	
PhysiCell_settings.o: ./modules/PhysiCell_settings.cpp
	$(COMPILE_COMMAND)  -c ./modules/PhysiCell_settings.cpp	
		
PhysiCell_basic_signaling.o: ./core/PhysiCell_basic_signaling.cpp
	$(COMPILE_COMMAND) -c ./core/PhysiCell_basic_signaling.cpp
	
PhysiCell_geometry.o: ./modules/PhysiCell_geometry.cpp
	$(COMPILE_COMMAND) -c ./modules/PhysiCell_geometry.cpp 

# start_and_stop
start_and_stop.o: ./addons/start_and_stop/start_and_stop.cpp
	$(COMPILE_COMMAND) $(INC) -c ./addons/start_and_stop/start_and_stop.cpp
	
# user-defined PhysiCell modules

Compile_MaBoSS: ./addons/PhysiBoSS/MaBoSS/engine/src/BooleanNetwork.h
	cd ./addons/PhysiBoSS/MaBoSS/engine/src;make CXX=$(CC) install_alib;make clean; cd ../../../../..

$(MaBoSS):  
ifeq ($(OS), Windows_NT)
	python addons/PhysiBoSS/setup_libmaboss.py
else
	python3 addons/PhysiBoSS/setup_libmaboss.py
endif

maboss_network.o: ./addons/PhysiBoSS/src/maboss_network.cpp $(MaBoSS)
	$(COMPILE_COMMAND) $(INC) -c ./addons/PhysiBoSS/src/maboss_network.cpp

maboss_intracellular.o: ./addons/PhysiBoSS/src/maboss_intracellular.cpp $(MaBoSS)
	$(COMPILE_COMMAND) $(INC) -c ./addons/PhysiBoSS/src/maboss_intracellular.cpp

custom.o: ./custom_modules/custom.cpp $(MaBoSS)
	$(COMPILE_COMMAND) $(INC)  -c ./custom_modules/custom.cpp

submodel_data_structures.o: ./custom_modules/submodel_data_structures.cpp
	$(COMPILE_COMMAND) $(INC) -c ./custom_modules/submodel_data_structures.cpp

tnf_receptor_dynamics.o: ./custom_modules/tnf_receptor_dynamics.cpp
	$(COMPILE_COMMAND) $(INC) -c ./custom_modules/tnf_receptor_dynamics.cpp

tnf_boolean_model_interface.o: ./custom_modules/tnf_boolean_model_interface.cpp $(MaBoSS)
	$(COMPILE_COMMAND) $(INC) -c ./custom_modules/tnf_boolean_model_interface.cpp
	@echo "compiling"
# cleanup

reset:
	rm -f *.cpp 
	cp ./sample_projects/Makefile-default Makefile 
	rm -f ./custom_modules/*
	touch ./custom_modules/empty.txt 
	rm ALL_CITATIONS.txt 
	rm -f ./config/PhysiCell_settings.xml 
	rm -f ./config/PhysiCell_settings_2D.xml 
	rm -f ./config/PhysiCell_settings_3D.xml 
	rm -f ./config/init.tsv 
	rm -f ./config/TNF_conf.cfg
	rm -f ./config/TNF_nodes.bnd
	rm -fr ./scripts
	cp ./config/PhysiCell_settings-backup.xml ./config/PhysiCell_settings.xml

MaBoSS-clean:
	rm -fr addons/PhysiBoSS/MaBoSS
	
clean: MaBoSS-clean
	rm -f *.o
	rm -f $(PROGRAM_NAME)*
	
data-cleanup:
	rm -f *.mat
	rm -f *.xml
	rm -f *.svg
	rm -f ./output/*
	touch ./output/empty.txt
	
# archival 

checkpoint: 
	zip -r $$(date +%b_%d_%Y_%H%M).zip Makefile *.cpp *.h config/*.xml custom_modules/* 

zip:
	zip -r latest.zip Makefile* *.cpp *.h BioFVM/* config/* core/* custom_modules/* matlab/* modules/* sample_projects/* 
	cp latest.zip $$(date +%b_%d_%Y_%H%M).zip
	cp latest.zip VERSION_$(VERSION).zip 
	mv *.zip archives/
	
tar:
	tar --ignore-failed-read -czf latest.tar Makefile* *.cpp *.h BioFVM/* config/* core/* custom_modules/* matlab/* modules/* sample_projects/* 
	cp latest.tar $$(date +%b_%d_%Y_%H%M).tar
	cp latest.tar VERSION_$(VERSION).tar
	mv *.tar archives/

unzip: 
	cp ./archives/latest.zip . 
	unzip latest.zip 
	
untar: 
	cp ./archives/latest.tar .
	tar -xzf latest.tar
