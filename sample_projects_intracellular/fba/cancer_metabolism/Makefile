VERSION := $(shell grep . VERSION.txt | cut -f1 -d:)
PROGRAM_NAME := cancer_metabolism

CC := g++

# Check for environment definitions of compiler
ifdef PHYSICELL_CPP
	CC := $(PHYSICELL_CPP)
endif

ARCH := native # best auto-tuning
# ARCH := core2 # a reasonably safe default for most CPUs since 2007
# ARCH := corei7

# For debugging with gdb just add: -g3 -ggdb -O3
# -DCLP_BUILD is an additional flags to link againt clp
# CFLAGS := -march=$(ARCH) -ggdb -fomit-frame-pointer -mfpmath=both -fopenmp -m64 -std=c++11 -DCLP_BUILD
CFLAGS := -march=$(ARCH) -O3 -fomit-frame-pointer -mfpmath=both -fopenmp -m64 -std=c++11 -pipe  -D ADDON_PHYSIDFBA

# Libraries need: SBML and coin-Clp
DEPS_DIR = ./addons/dFBA/ext

DEPS_CFLAGS := -I$(DEPS_DIR)/libsbml/include -I$(DEPS_DIR)/coin-or/include
LD_PATH  := -L$(DEPS_DIR)/libsbml/lib -L$(DEPS_DIR)/coin-or/lib/
LIBS     := -lClp-static -lCoinUtils-static -llapack -lsbml-static -lxml2 -lbz2 -lz

# if we're on Windows, we need to provide a (agreed upon) path to some of the dynamic libs
#ifeq ($(OS),Windows_NT)
#	WIN_LD_FLAG := -LC:\msys64\mingw64\bin
#else
#	WIN_LD_FLAG := 
#endif

ifeq ($(OS),Windows_NT)
	WIN_LD_FLAG := -LC:\msys64\mingw64\bin
else
	WIN_LD_FLAG := 
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Darwin)
		UNAME_P := $(shell uname -p)
		var := $(shell which $(CC) | xargs file)
		ifeq ($(lastword $(var)),arm64)
		  CFLAGS := -march=$(ARCH) -O3 -fomit-frame-pointer -fopenmp -m64 -std=c++11
		endif
	endif
endif

COMPILE_COMMAND := $(CC) $(CFLAGS) $(DEPS_CFLAGS)

BioFVM_OBJECTS := BioFVM_vector.o BioFVM_mesh.o BioFVM_microenvironment.o BioFVM_solvers.o BioFVM_matlab.o \
BioFVM_utilities.o BioFVM_basic_agent.o BioFVM_MultiCellDS.o BioFVM_agent_container.o

PhysiCell_core_OBJECTS := PhysiCell_phenotype.o PhysiCell_cell_container.o PhysiCell_standard_models.o \
PhysiCell_cell.o PhysiCell_custom.o PhysiCell_utilities.o PhysiCell_constants.o PhysiCell_basic_signaling.o \
PhysiCell_signal_behavior.o PhysiCell_rules.o

PhysiCell_module_OBJECTS := PhysiCell_SVG.o PhysiCell_pathology.o PhysiCell_MultiCellDS.o PhysiCell_various_outputs.o \
PhysiCell_pugixml.o PhysiCell_settings.o PhysiCell_geometry.o

PhysiFBA_addon_OBJECTS := PhysiFBA_intracellular.o PhysiFBA_metabolite.o PhysiFBA_reaction.o PhysiFBA_model.o

PCdFBA_OBJECTS := PhysiFBA_intracellular.o FBA_model.o FBA_reaction.o FBA_metabolite.o
# put your custom objects here (they should be in the custom_modules directory)

PhysiCell_custom_module_OBJECTS := cancer_metabolism.o

pugixml_OBJECTS := pugixml.o

PhysiCell_OBJECTS := $(BioFVM_OBJECTS)  $(pugixml_OBJECTS) $(PhysiCell_core_OBJECTS) $(PhysiCell_module_OBJECTS)
ALL_OBJECTS := $(PhysiCell_OBJECTS) $(PhysiFBA_addon_OBJECTS) $(PhysiCell_custom_module_OBJECTS)


# compile the project

all: libFBA main.cpp $(ALL_OBJECTS)
#	$(COMPILE_COMMAND) $(INC) -o $(PROGRAM_NAME) $(ALL_OBJECTS) main.cpp $(LDFLAGS) $(LIBS)
	@echo DEPS_CFLAGS= $(DEPS_CFLAGS)
	@echo LIBS= $(LIBS)
	@echo 
	$(COMPILE_COMMAND) -o $(PROGRAM_NAME) $(ALL_OBJECTS) main.cpp $(LD_PATH) $(WIN_LD_FLAG) $(LIBS)
	@echo
	@echo created $(PROGRAM_NAME)
	@echo

name:
	@echo ""
	@echo "Executable name is" $(PROGRAM_NAME)
	@echo ""

# PhysiCell core components

PhysiCell_phenotype.o: ./core/PhysiCell_phenotype.cpp
	$(COMPILE_COMMAND) -c ./core/PhysiCell_phenotype.cpp

PhysiCell_digital_cell_line.o: ./core/PhysiCell_digital_cell_line.cpp
	$(COMPILE_COMMAND) -c ./core/PhysiCell_digital_cell_line.cpp

PhysiCell_cell.o: ./core/PhysiCell_cell.cpp
	$(COMPILE_COMMAND) -c ./core/PhysiCell_cell.cpp

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

PhysiCell_basic_signaling.o: ./core/PhysiCell_basic_signaling.cpp
	$(COMPILE_COMMAND) -c ./core/PhysiCell_basic_signaling.cpp
	
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
	$(COMPILE_COMMAND) -c ./modules/PhysiCell_settings.cpp

PhysiCell_geometry.o: ./modules/PhysiCell_geometry.cpp
	$(COMPILE_COMMAND) -c ./modules/PhysiCell_geometry.cpp 

# PhysiFBA addon modules
libFBA: 
	python beta/setup_fba.py --pkg libsbml
	python beta/setup_fba.py --pkg coin-or

PhysiFBA_intracellular.o: ./addons/dFBA/src/dfba_intracellular.cpp
	$(COMPILE_COMMAND) $(ADDFLAGS) $(INC) $(LIBS) -c ./addons/dFBA/src/dfba_intracellular.cpp -o ./PhysiFBA_intracellular.o

PhysiFBA_model.o: ./addons/dFBA/src/FBA_model.cpp
	$(COMPILE_COMMAND) $(ADDFLAGS) $(INC) $(LIBS) -c ./addons/dFBA/src/FBA_model.cpp -o ./PhysiFBA_model.o

PhysiFBA_reaction.o: ./addons/dFBA/src/FBA_reaction.cpp
	$(COMPILE_COMMAND) $(ADDFLAGS) $(INC) $(LIBS) -c ./addons/dFBA/src/FBA_reaction.cpp -o ./PhysiFBA_reaction.o

PhysiFBA_metabolite.o: ./addons/dFBA/src/FBA_metabolite.cpp
	$(COMPILE_COMMAND) $(ADDFLAGS) $(INC) $(LIBS) -c ./addons/dFBA/src/FBA_metabolite.cpp -o ./PhysiFBA_metabolite.o

# user-defined PhysiCell modules
cancer_metabolism.o: ./custom_modules/cancer_metabolism.cpp
	$(COMPILE_COMMAND) -c ./custom_modules/cancer_metabolism.cpp


# cleanup

reset:
	rm -f *.cpp 
	cp ./sample_projects/Makefile-default Makefile 
	rm -f ./custom_modules/*
	touch ./custom_modules/empty.txt 
	touch ALL_CITATIONS.txt 
	touch ./core/PhysiCell_cell.cpp
	rm ALL_CITATIONS.txt 
	cp ./config/PhysiCell_settings-backup.xml ./config/PhysiCell_settings.xml 
	touch ./config/empty.csv
	rm ./config/*.csv	

clean:
	rm -f *.o
	rm -f $(PROGRAM_NAME)*

data-cleanup:
	rm -f *.mat
	rm -f *.xml
	rm -f *.svg
	rm -f ./output/*
	touch ./config/empty.txt

# archival

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
