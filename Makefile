VERSION := 1.0.0

PROGRAM_NAME := make_lib

CC := g++
# CC := g++-mp-5 # typical macports compiler name
# CC := g++-5 # typical homebrew compiler name 

ARCH := core2 # a reasonably safe default for most CPUs since 2007
# ARCH := corei7
# ARCH := corei7-avx # earlier i7 
# ARCH := core-avx-i # i7 ivy bridge or newer 
# ARCH := core-avx2 # i7 with Haswell or newer
# ARCH := nehalem
# ARCH := westmere
# ARCH := sandybridge
# ARCH := ivybridge
# ARCH := haswell
# ARCH := broadwell
# ARCH := bonnell
# ARCH := silvermont
# ARCH := nocona #64-bit pentium 4 or later 

CFLAGS := -march=$(ARCH) -O3 -s -fomit-frame-pointer -mfpmath=both -fopenmp -m64 -std=c++11

BioFVM_OBJECTS := BioFVM_vector.o BioFVM_mesh.o BioFVM_microenvironment.o BioFVM_solvers.o BioFVM_matlab.o \
BioFVM_utilities.o BioFVM_basic_agent.o BioFVM_MultiCellDS.o BioFVM_agent_container.o 

PhysiCell_core_OBJECTS := PhysiCell_cell_container.o PhysiCell_cell.o PhysiCell_standard_models.o PhysiCell_digital_cell_line.o \
PhysiCell_custom.o PhysiCell_utilities.o 

PhysiCell_module_OBJECTS := PhysiCell_SVG.o PhysiCell_pathology.o

PhysiCell_custom_module_OBJECTS :=  # put your custom objects here

pugixml_OBJECTS := pugixml.o

liver_OBJECTS := main.o

ALL_OBJECTS := $(BioFVM_OBJECTS) $(PhysiCell_core_OBJECTS) $(PhysiCell_module_OBJECTS) $(pugixml_OBJECTS) $(PhysiCell_custom_module_OBJECTS)


EXAMPLES := ./examples/PhysiCell_test_mechanics_1.cpp ./examples/PhysiCell_test_mechanics_2.cpp \
./examples/PhysiCell_test_DCIS.cpp ./examples/PhysiCell_test_HDS.cpp \
./examples/PhysiCell_test_cell_cycle.cpp ./examples/physicell_test_volume.cpp 

COMPILE_COMMAND := $(CC) $(CFLAGS) 

all: make_lib

make_lib: $(ALL_OBJECTS)

physicell_test_mech1: $(ALL_OBJECTS) ./examples/PhysiCell_test_mechanics_1.cpp 
	$(COMPILE_COMMAND) -o test_mech1 $(ALL_OBJECTS) ./examples/PhysiCell_test_mechanics_1.cpp

physicell_test_mech2: $(ALL_OBJECTS) ./examples/PhysiCell_test_mechanics_2.cpp 
	$(COMPILE_COMMAND) -o test_mech2 $(ALL_OBJECTS) ./examples/PhysiCell_test_mechanics_2.cpp
	
physicell_test_DCIS: $(ALL_OBJECTS) ./examples/PhysiCell_test_DCIS.cpp 
	$(COMPILE_COMMAND) -o test_DCIS $(ALL_OBJECTS) ./examples/PhysiCell_test_DCIS.cpp

physicell_test_HDS: $(ALL_OBJECTS) ./examples/PhysiCell_test_HDS.cpp 
	$(COMPILE_COMMAND) -o test_HDS $(ALL_OBJECTS) ./examples/PhysiCell_test_HDS.cpp

physicell_test_cell_cycle: $(ALL_OBJECTS) ./examples/PhysiCell_test_cell_cycle.cpp 
	$(COMPILE_COMMAND) -o test_cycle $(ALL_OBJECTS) ./examples/PhysiCell_test_cell_cycle.cpp

physicell_test_volume: $(ALL_OBJECTS) ./examples/physicell_test_volume.cpp 
	$(COMPILE_COMMAND) -o test_volume $(ALL_OBJECTS) ./examples/physicell_test_volume.cpp
	
examples: $(ALL_OBJECTS) $(EXAMPLES)
	$(COMPILE_COMMAND) -o ./examples/test_mech1 ./examples/PhysiCell_test_mechanics_1.cpp $(ALL_OBJECTS)
	$(COMPILE_COMMAND) -o ./examples/test_mech2 ./examples/PhysiCell_test_mechanics_2.cpp $(ALL_OBJECTS)
	$(COMPILE_COMMAND) -o ./examples/test_DCIS ./examples/PhysiCell_test_DCIS.cpp $(ALL_OBJECTS)
	$(COMPILE_COMMAND) -o ./examples/test_HDS ./examples/PhysiCell_test_HDS.cpp $(ALL_OBJECTS)
	$(COMPILE_COMMAND) -o ./examples/test_cycle ./examples/PhysiCell_test_cell_cycle.cpp $(ALL_OBJECTS)
	$(COMPILE_COMMAND) -o ./examples/test_volume ./examples/physicell_test_volume.cpp $(ALL_OBJECTS)

# PhysiCell core components	
	
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
	
PhysiCell_custom.o: ./custom_modules/PhysiCell_custom.cpp
	$(COMPILE_COMMAND) -c ./custom_modules/PhysiCell_custom.cpp 	
	
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
	
# user-defined PhysiCell modules

liver_setup.o: ./custom_modules/liver_setup.cpp 
	$(COMPILE_COMMAND) -c ./custom_modules/liver_setup.cpp 

liver_misc.o: ./custom_modules/liver_misc.cpp 
	$(COMPILE_COMMAND) -c ./custom_modules/liver_misc.cpp 

liver_model.o: ./custom_modules/liver_model.cpp 
	$(COMPILE_COMMAND) -c ./custom_modules/liver_model.cpp 
	
clean:
	rm -f *.o
	rm -f $(PROGRAM_NAME).exe
	rm -f ./examples/test_* 
	
zip:
	zip $$(date +%b_%d_%Y_%H%M).zip */*.cpp */*.h *akefile* *.cpp *.h */*.hpp *.xml *.tex *.bib *hanges*.txt config/*.xml *.txt
	zip VERSION_$(VERSION).zip *.cpp *.h *akefile* *.xml *.tex *.bib *hanges*.txt *.txt
	mv *.zip archives/
