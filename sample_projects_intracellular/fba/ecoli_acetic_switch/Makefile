VERSION := $(shell grep . VERSION.txt | cut -f1 -d:)
PROGRAM_NAME := ecoli-dfba

CC := g++
# CC := g++-mp-7 # typical macports compiler name
# CC := g++-7 # typical homebrew compiler name

# Check for environment definitions of compiler
# e.g., on CC = g++-7 on OSX
ifdef PHYSICELL_CPP
	CC := $(PHYSICELL_CPP)
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


# For debugging with gdb just add: -g3 -ggdb -O3
# -DCLP_BUILD is an additional flags to link againt clp
# CFLAGS := -march=$(ARCH) -ggdb -fomit-frame-pointer -mfpmath=both -fopenmp -m64 -std=c++11 -DCLP_BUILD
CFLAGS := -march=$(ARCH) -fomit-frame-pointer -mfpmath=both -fopenmp -m64 -std=c++11 -DCLP_BUILD


# Libraries need: SBML and coin-Clp

CLP_FOLDER := ./addons/coin-clp


INC := -I/usr/local/include -DADDON_PHYSIDFBA -I$(CLP_FOLDER)/include -I$(CLP_FOLDER)/include/coin-or/glpk
LIBS := -L/usr/local/lib -L$(CLP_FOLDER)/lib -ldl -lm -lClp -lCoinUtils -lcoinasl -lcoinglpk -lcoinmumps -lsbml

OSFLAG 	:=
ifeq ($(OS),Windows_NT)
	OSFLAG += -D WIN32
	OMP_LIB := 
#	LIBFBA_DIR := C:\Users\heiland\libroadrunner\roadrunner-win64-vs14-cp35m
#	LIBFBA_LIBS := C:\Users\heiland\libroadrunner\roadrunner-win64-vs14-cp35m/bin
#	LIBFBA_CFLAGS := -I${LIBFBA_DIR}/include/rr/C
	CFLAGS := -march=$(ARCH) -fomit-frame-pointer -fopenmp -m64 -std=c++11 -D ADDON_PHYSIDFBA
	ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
		OSFLAG += -D AMD64
	endif
	ifeq ($(PROCESSOR_ARCHITECTURE),x86)
		OSFLAG += -D IA32
	endif
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		OSFLAG += -D LINUX
		OMP_LIB :=
#		LIBFBA_DIR := $(HOME)/libroadrunner/libroadrunner
#		LIBFBA_DIR := $(shell pwd)/intracellular_libs/roadrunner
#		LIBFBA_CFLAGS := -I${LIBFBA_DIR}/include/rr/C
		LIBFBA_LIBS := ${LIBFBA_DIR}/lib
		# CFLAGS := -march=$(ARCH) -g  -fomit-frame-pointer -fopenmp -m64 -std=c++11 -D ADDON_PHYSIDFBA 
		CFLAGS := -march=$(ARCH) -fomit-frame-pointer -m64 -std=c++11 -D ADDON_PHYSIDFBA  
	endif
	ifeq ($(UNAME_S),Darwin)
#		CC := clang++
		OSFLAG += -D OSX
#	 	LIBFBA_DIR := $(HOME)/libroadrunner/roadrunner-osx-10.9-cp36m
#		LIBFBA_DIR := $(shell pwd)/intracellular_libs/roadrunner
		LIBFBA_DIR := ./addons/dFBA
#		LIBFBA_CFLAGS := -I${LIBFBA_DIR}/include/rr/C
#		LIBFBA_CFLAGS := -I${LIBFBA_DIR}/include/rr/C
#		LIBFBA_CFLAGS := -I${LIBFBA_DIR}/coin-or/include/coin-or
		LIBFBA_CFLAGS := -I${LIBFBA_DIR}/coin-or/include
		LIBFBA_LIBS := ${LIBFBA_DIR}/lib/coin-or/lib
	endif
		UNAME_P := $(shell uname -p)
	ifeq ($(UNAME_P),x86_64)
		OSFLAG += -D AMD64
#		LIBFBA_DIR := $(HOME)/libroadrunner/libroadrunner
#		LIBFBA_CFLAGS := -I${LIBFBA_DIR}/include/rr/C
		CFLAGS := -march=$(ARCH) -fomit-frame-pointer -fopenmp -m64 -std=c++11 -D ADDON_PHYSIDFBA  
	endif
		ifneq ($(filter %86,$(UNAME_P)),)
	OSFLAG += -D IA32
		endif
	ifneq ($(filter arm%,$(UNAME_P)),)
		OSFLAG += -D ARM
	endif
endif

COMPILE_COMMAND := $(CC) $(CFLAGS) $(LIBFBA_CFLAGS)

BioFVM_OBJECTS := BioFVM_vector.o BioFVM_mesh.o BioFVM_microenvironment.o BioFVM_solvers.o BioFVM_matlab.o \
BioFVM_utilities.o BioFVM_basic_agent.o BioFVM_MultiCellDS.o BioFVM_agent_container.o

PhysiCell_core_OBJECTS := PhysiCell_phenotype.o PhysiCell_cell_container.o PhysiCell_standard_models.o \
PhysiCell_cell.o PhysiCell_custom.o PhysiCell_utilities.o PhysiCell_constants.o PhysiCell_basic_signaling.o \
PhysiCell_signal_behavior.o PhysiCell_rules.o

PhysiCell_module_OBJECTS := PhysiCell_SVG.o PhysiCell_pathology.o PhysiCell_MultiCellDS.o PhysiCell_various_outputs.o \
PhysiCell_pugixml.o PhysiCell_settings.o  PhysiCell_geometry.o

FBA_addon_OBJECTS := FBA_metabolite.o FBA_reaction.o FBA_model.o

# put your custom objects here (they should be in the custom_modules directory)

PhysiCell_custom_module_OBJECTS := ecoli_acetic_switch.o

pugixml_OBJECTS := pugixml.o

PhysiCell_OBJECTS := $(BioFVM_OBJECTS)  $(pugixml_OBJECTS) $(PhysiCell_core_OBJECTS) $(PhysiCell_module_OBJECTS)
ALL_OBJECTS := $(PhysiCell_OBJECTS) $(PhysiCell_custom_module_OBJECTS) $(FBA_addon_OBJECTS) 

# compile the project

# all: libFBA main.cpp $(ALL_OBJECTS)

all: main.cpp $(ALL_OBJECTS)
	$(COMPILE_COMMAND) $(INC) -o $(PROGRAM_NAME) $(ALL_OBJECTS) main.cpp $(LIBS)
	make name 

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
	
PhysiCell_geometry.o: ./modules/PhysiCell_geometry.cpp
	$(COMPILE_COMMAND) -c ./modules/PhysiCell_geometry.cpp 

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

# user-defined PhysiCell modules

# dFBA addon modules

libFBA: 
ifeq ($(OS), Windows_NT)
	python beta/setup_coin_clp.py
else
	python3 beta/setup_coin_clp.py
endif

FBA_model.o: ./addons/dFBA/FBA_model.cpp
	$(COMPILE_COMMAND) $(ADDFLAGS) $(INC) $(LIBS) -c ./addons/dFBA/FBA_model.cpp -o ./FBA_model.o
	#export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:addons/coin-clp/lib

FBA_reaction.o: ./addons/dFBA/FBA_reaction.cpp
	$(COMPILE_COMMAND) $(ADDFLAGS) $(INC) $(LIBS) -c ./addons/dFBA/FBA_reaction.cpp -o ./FBA_reaction.o

FBA_metabolite.o: ./addons/dFBA/FBA_metabolite.cpp
	$(COMPILE_COMMAND) $(ADDFLAGS) $(INC) $(LIBS) -c ./addons/dFBA/FBA_metabolite.cpp -o ./FBA_metabolite.o

ecoli_acetic_switch.o: ./custom_modules/ecoli_acetic_switch.cpp
	$(COMPILE_COMMAND) $(ADDFLAGS) $(INC) $(LIBS) -c ./custom_modules/ecoli_acetic_switch.cpp


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
	touch ./config/empty.txt

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
