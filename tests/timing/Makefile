PROGRAM_NAME := time_tests

CC := g++
# CC := g++-mp-7 # typical macports compiler name
# CC := g++-7 # typical homebrew compiler name 

# Check for environment definitions of compiler 
# e.g., on CC = g++-7 on OSX
ifdef PHYSICELL_CPP 
	CC := $(PHYSICELL_CPP)
endif

ARCH := native # best auto-tuning

# CFLAGS := -march=$(ARCH) -Ofast -s -fomit-frame-pointer -mfpmath=both -fopenmp -m64 -std=c++11
CFLAGS := -march=$(ARCH) -O3 -fomit-frame-pointer -mfpmath=both -fopenmp -m64 -std=c++11
#CFLAGS := -g -fopenmp -std=c++11

COMPILE_COMMAND := $(CC) $(CFLAGS) 

DIR := ../..
BioFVM_OBJECTS := $(DIR)/BioFVM_vector.o $(DIR)/BioFVM_mesh.o $(DIR)/BioFVM_microenvironment.o $(DIR)/BioFVM_solvers.o $(DIR)/BioFVM_matlab.o \
$(DIR)/BioFVM_utilities.o $(DIR)/BioFVM_basic_agent.o $(DIR)/BioFVM_MultiCellDS.o $(DIR)/BioFVM_agent_container.o 

PhysiCell_core_OBJECTS := $(DIR)/PhysiCell_phenotype.o $(DIR)/PhysiCell_cell_container.o $(DIR)/PhysiCell_standard_models.o $(DIR)/PhysiCell_cell.o $(DIR)/PhysiCell_custom.o $(DIR)/PhysiCell_utilities.o 

PhysiCell_module_OBJECTS := $(DIR)/PhysiCell_SVG.o $(DIR)/PhysiCell_pathology.o $(DIR)/PhysiCell_MultiCellDS.o $(DIR)/PhysiCell_various_outputs.o \
$(DIR)/PhysiCell_pugixml.o $(DIR)/PhysiCell_settings.o


pugixml_OBJECTS := $(DIR)/pugixml.o

PhysiCell_OBJECTS := $(BioFVM_OBJECTS)  $(pugixml_OBJECTS) $(PhysiCell_core_OBJECTS) $(PhysiCell_module_OBJECTS)
ALL_OBJECTS := $(PhysiCell_OBJECTS) $(PhysiCell_custom_module_OBJECTS)

#compile the project 
	
all: main.cpp $(ALL_OBJECTS)
	$(COMPILE_COMMAND) -I$(DIR)/core -o $(PROGRAM_NAME) $(ALL_OBJECTS) main.cpp 

# cleanup

clean:
	rm -f *.o
	rm -f $(PROGRAM_NAME)*
