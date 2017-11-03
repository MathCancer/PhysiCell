VERSION := 1.1.2

PROGRAM_NAME := make_lib

CC := g++

# change this to your own CPU archicture. 
# Here is a list for gcc 4.9.0
# https://gcc.gnu.org/onlinedocs/gcc-4.9.0/gcc/i386-and-x86-64-Options.html#i386-and-x86-64-Options 

ARCH := core2 # a reasonably safe default for most CPUs since 2007
# ARCH := corei7
#ARCH := corei7-avx # earlier i7 
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

BioFVM_OBJECTS := BioFVM_vector.o BioFVM_matlab.o BioFVM_utilities.o BioFVM_mesh.o \
BioFVM_microenvironment.o BioFVM_solvers.o BioFVM_basic_agent.o \
BioFVM_agent_container.o BioFVM_MultiCellDS.o 

pugixml_OBJECTS := pugixml.o

EXAMPLES := ./examples/convergence_test1.cpp ./examples/convergence_test2.cpp \
./examples/convergence_test3.cpp ./examples/convergence_test4_1.cpp \
./examples/convergence_test4_2.cpp ./examples/convergence_test5.cpp \
./examples/performance_test_substrates.cpp \
./examples/performance_test_voxels.cpp \
./examples/performance_test_numcells.cpp 

COMPILE_COMMAND := $(CC) $(CFLAGS) 

all: make_lib

make_lib: $(BioFVM_OBJECTS) $(pugixml_OBJECTS) 

BioFVM_vector.o: BioFVM_vector.cpp
	$(COMPILE_COMMAND) -c BioFVM_vector.cpp 

BioFVM_agent_container.o: BioFVM_agent_container.cpp
	$(COMPILE_COMMAND) -c BioFVM_agent_container.cpp 
	
BioFVM_mesh.o: BioFVM_mesh.cpp
	$(COMPILE_COMMAND) -c BioFVM_mesh.cpp 

BioFVM_microenvironment.o: BioFVM_microenvironment.cpp
	$(COMPILE_COMMAND) -c BioFVM_microenvironment.cpp 

BioFVM_solvers.o: BioFVM_solvers.cpp
	$(COMPILE_COMMAND) -c BioFVM_solvers.cpp 

BioFVM_utilities.o: BioFVM_utilities.cpp
	$(COMPILE_COMMAND) -c BioFVM_utilities.cpp 
	
BioFVM_basic_agent.o: BioFVM_basic_agent.cpp
	$(COMPILE_COMMAND) -c BioFVM_basic_agent.cpp 
	
BioFVM_matlab.o: BioFVM_matlab.cpp
	$(COMPILE_COMMAND) -c BioFVM_matlab.cpp

BioFVM_MultiCellDS.o: BioFVM_MultiCellDS.cpp
	$(COMPILE_COMMAND) -c BioFVM_MultiCellDS.cpp

pugixml.o: pugixml.cpp
	$(COMPILE_COMMAND) -c pugixml.cpp

examples: $(BioFVM_OBJECTS) $(pugixml_OBJECTS) $(EXAMPLES)
	$(COMPILE_COMMAND) -o ./examples/conv_test1 ./examples/convergence_test1.cpp $(BioFVM_OBJECTS) $(pugixml_OBJECTS)
	$(COMPILE_COMMAND) -o ./examples/conv_test2 ./examples/convergence_test2.cpp $(BioFVM_OBJECTS) $(pugixml_OBJECTS)
	$(COMPILE_COMMAND) -o ./examples/conv_test3 ./examples/convergence_test3.cpp $(BioFVM_OBJECTS) $(pugixml_OBJECTS)
	$(COMPILE_COMMAND) -o ./examples/conv_test4_1 ./examples/convergence_test4_1.cpp $(BioFVM_OBJECTS) $(pugixml_OBJECTS)
	$(COMPILE_COMMAND) -o ./examples/conv_test4_2 ./examples/convergence_test4_2.cpp $(BioFVM_OBJECTS) $(pugixml_OBJECTS)
	$(COMPILE_COMMAND) -o ./examples/conv_test5 ./examples/convergence_test5.cpp $(BioFVM_OBJECTS) $(pugixml_OBJECTS)
	$(COMPILE_COMMAND) -o ./examples/perf_test_substrates ./examples/performance_test_substrates.cpp $(BioFVM_OBJECTS) $(pugixml_OBJECTS)
	$(COMPILE_COMMAND) -o ./examples/perf_test_voxels ./examples/performance_test_voxels.cpp $(BioFVM_OBJECTS) $(pugixml_OBJECTS)
	$(COMPILE_COMMAND) -o ./examples/perf_test_numcells ./examples/performance_test_numcells.cpp $(BioFVM_OBJECTS) $(pugixml_OBJECTS)
	

tutorial1: ./examples/tutorial1_BioFVM.cpp $(BioFVM_OBJECTS) $(pugixml_OBJECTS) 
	$(COMPILE_COMMAND) -o ./examples/tutorial1 ./examples/tutorial1_BioFVM.cpp $(BioFVM_OBJECTS) $(pugixml_OBJECTS)
tutorial2: ./examples/tutorial2_BioFVM.cpp $(BioFVM_OBJECTS) $(pugixml_OBJECTS) 
	$(COMPILE_COMMAND) -o ./examples/tutorial2 ./examples/tutorial2_BioFVM.cpp $(BioFVM_OBJECTS) $(pugixml_OBJECTS)
tutorial3: ./examples/tutorial3_BioFVM.cpp $(BioFVM_OBJECTS) $(pugixml_OBJECTS) 
	$(COMPILE_COMMAND) -o ./examples/tutorial3 ./examples/tutorial3_BioFVM.cpp $(BioFVM_OBJECTS) $(pugixml_OBJECTS)	
main_experiment: ./examples/main_experiment.cpp $(BioFVM_OBJECTS) $(pugixml_OBJECTS) 
	$(COMPILE_COMMAND) -o ./examples/tutorial3 ./examples/main_experiment.cpp $(BioFVM_OBJECTS) $(pugixml_OBJECTS)
	
clean:
	rm -f *.o
	rm -f $(PROGRAM_NAME)*
	rm -f ./examples/conv_test* 
	rm -f ./examples/perf_test*
	
zip:
	zip $$(date +%b_%d_%Y_%H%M).zip *.cpp *.h *akefile* *.xml *.tex *.bib *hanges*.txt config/*.xml *.txt
	zip VERSION_$(VERSION).zip *.cpp *.h *akefile* *.xml *.tex *.bib *hanges*.txt *.txt
	mv *.zip archives/
