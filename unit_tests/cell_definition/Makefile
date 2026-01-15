VERSION := $(shell grep . ../../VERSION.txt | cut -f1 -d:)
#@echo $(VERSION)

CC := g++
ifdef PHYSICELL_CPP 
	CC := $(PHYSICELL_CPP)
endif

ARCH := native # best auto-tuning

# CFLAGS := -march=$(ARCH) -Ofast -s -fomit-frame-pointer -mfpmath=both -fopenmp -m64 -std=c++11
CFLAGS := -march=$(ARCH) -O3 -fomit-frame-pointer -mfpmath=both -fopenmp -m64 -std=c++11 -U LIBROADRUNNER 

COMPILE_COMMAND := $(CC) $(CFLAGS) 

ODIR := ../..

BioFVM_OBJECTS := $(ODIR)/BioFVM_vector.o $(ODIR)/BioFVM_mesh.o $(ODIR)/BioFVM_microenvironment.o $(ODIR)/BioFVM_solvers.o $(ODIR)/BioFVM_matlab.o \
$(ODIR)/BioFVM_utilities.o $(ODIR)/BioFVM_basic_agent.o $(ODIR)/BioFVM_MultiCellDS.o $(ODIR)/BioFVM_agent_container.o 

PhysiCell_core_OBJECTS := $(ODIR)/PhysiCell_phenotype.o $(ODIR)/PhysiCell_cell_container.o $(ODIR)/PhysiCell_standard_models.o $(ODIR)/PhysiCell_cell.o $(ODIR)/PhysiCell_custom.o $(ODIR)/PhysiCell_utilities.o $(ODIR)/PhysiCell_constants.o

PhysiCell_module_OBJECTS := $(ODIR)/PhysiCell_SVG.o $(ODIR)/PhysiCell_pathology.o \
$(ODIR)/PhysiCell_MultiCellDS.o $(ODIR)/PhysiCell_various_outputs.o \
$(ODIR)/PhysiCell_pugixml.o $(ODIR)/PhysiCell_settings.o

pugixml_OBJECTS := $(ODIR)/pugixml.o

PhysiCell_OBJECTS := $(BioFVM_OBJECTS)  $(pugixml_OBJECTS) $(PhysiCell_core_OBJECTS) $(PhysiCell_module_OBJECTS)
ALL_OBJECTS := $(PhysiCell_OBJECTS) 

all: 
	$(COMPILE_COMMAND) -o test_cell_def0 $(ALL_OBJECTS) test_cell_def0.cpp 
	$(COMPILE_COMMAND) -o test_cell_def1 $(ALL_OBJECTS) test_cell_def1.cpp 
	$(COMPILE_COMMAND) -o test_cell_def2 $(ALL_OBJECTS) test_cell_def2.cpp 

test_cell_def0: 
	$(COMPILE_COMMAND) -o test_cell_def0 $(ALL_OBJECTS) test_cell_def0.cpp 
test_cell_def1: 
	$(COMPILE_COMMAND) -o test_cell_def1 $(ALL_OBJECTS) test_cell_def1.cpp 
test_cell_def2: 
	$(COMPILE_COMMAND) -o test_cell_def2 $(ALL_OBJECTS) test_cell_def2.cpp 
