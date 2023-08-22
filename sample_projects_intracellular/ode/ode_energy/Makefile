VERSION := $(shell grep . VERSION.txt | cut -f1 -d:)
PROGRAM_NAME := ode_energy

CC := g++

# Check for environment definitions of compiler 
# e.g., on CC = g++-10 on OSX
ifdef PHYSICELL_CPP 
	CC := $(PHYSICELL_CPP)
endif

ARCH := native # best auto-tuning 

CFLAGS := -march=$(ARCH) -O3 -fomit-frame-pointer -fopenmp -m64 -std=c++11 -D ADDON_ROADRUNNER

OSFLAG 	:=
ifeq ($(OS),Windows_NT)
	OSFLAG += -D WIN32
	OMP_LIB := 
#	LIBRR_DIR := C:\Users\heiland\libroadrunner\roadrunner-win64-vs14-cp35m
#	LIBRR_LIBS := C:\Users\heiland\libroadrunner\roadrunner-win64-vs14-cp35m/bin
#	LIBRR_CFLAGS := -I${LIBRR_DIR}/include/rr/C
#	CFLAGS := -march=$(ARCH) -fomit-frame-pointer -fopenmp -m64 -std=c++11 -D ADDON_ROADRUNNER 
	LIBRR_DIR := .\addons\libRoadrunner\roadrunner
	LIBRR_CFLAGS := -I${LIBRR_DIR}\include\rr\C
	LIBRR_LIBS := ${LIBRR_DIR}\lib
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
#		LIBRR_DIR := $(shell pwd)/intracellular_libs/roadrunner
		LIBRR_DIR := ./addons/libRoadrunner/roadrunner
		LIBRR_CFLAGS := -I${LIBRR_DIR}/include/rr/C
		LIBRR_LIBS := ${LIBRR_DIR}/lib
	endif
	ifeq ($(UNAME_S),Darwin)
		OSFLAG += -D OSX
		LIBRR_DIR := ./addons/libRoadrunner/roadrunner
		LIBRR_CFLAGS := -I${LIBRR_DIR}/include/rr/C
		LIBRR_LIBS := ${LIBRR_DIR}/lib
	endif

#	 Not sure if useful later or not.
#	UNAME_P := $(shell uname -p)
#	ifeq ($(UNAME_P),x86_64)
#		OSFLAG += -D AMD64
#		LIBRR_DIR := $(shell pwd)/intracellular_libs/roadrunner
#		LIBRR_CFLAGS := -I${LIBRR_DIR}/include/rr/C
#		# CFLAGS := -march=$(ARCH) -fomit-frame-pointer -fopenmp -m64 -std=c++11 -D LIBROADRUNNER 
#		CFLAGS := -march=$(ARCH) -fomit-frame-pointer -fopenmp -m64 -std=c++11 -D ADDON_ROADRUNNER 
#	endif
#	ifneq ($(filter %86,$(UNAME_P)),)
#		OSFLAG += -D IA32
#	endif
#	ifneq ($(filter arm%,$(UNAME_P)),)
#		OSFLAG += -D ARM
#	endif
endif

COMPILE_COMMAND := $(CC) $(CFLAGS) $(LIBRR_CFLAGS) 

BioFVM_OBJECTS := BioFVM_vector.o BioFVM_mesh.o BioFVM_microenvironment.o BioFVM_solvers.o BioFVM_matlab.o \
BioFVM_utilities.o BioFVM_basic_agent.o BioFVM_MultiCellDS.o BioFVM_agent_container.o 

PhysiCell_core_OBJECTS := PhysiCell_phenotype.o PhysiCell_cell_container.o PhysiCell_standard_models.o \
PhysiCell_cell.o PhysiCell_custom.o PhysiCell_utilities.o PhysiCell_constants.o PhysiCell_basic_signaling.o \
PhysiCell_signal_behavior.o PhysiCell_rules.o

PhysiCell_module_OBJECTS := PhysiCell_SVG.o PhysiCell_pathology.o PhysiCell_MultiCellDS.o PhysiCell_various_outputs.o \
PhysiCell_pugixml.o PhysiCell_settings.o PhysiCell_geometry.o

# put your custom objects here (they should be in the custom_modules directory)
PhysiCell_custom_module_OBJECTS := custom.o 

pugixml_OBJECTS := pugixml.o

ROADRUNNER_OBJECTS := librr_intracellular.o 

PhysiCell_OBJECTS := $(BioFVM_OBJECTS)  $(pugixml_OBJECTS) $(PhysiCell_core_OBJECTS) $(PhysiCell_module_OBJECTS)
ALL_OBJECTS := $(PhysiCell_OBJECTS) $(PhysiCell_custom_module_OBJECTS) $(ROADRUNNER_OBJECTS)

# compile the project 

#all: main.cpp $(ALL_OBJECTS)
#	$(COMPILE_COMMAND) -o $(PROGRAM_NAME) $(ALL_OBJECTS) main.cpp 

all: libRoadrunner main.cpp $(ALL_OBJECTS)
	@echo Your OS= $(OSFLAG)
	@echo LIBRR_CFLAGS= $(LIBRR_CFLAGS)
	@echo LIBRR_LIBS= $(LIBRR_LIBS)
	@echo 
	$(COMPILE_COMMAND) $(OMP_LIB) -o $(PROGRAM_NAME) $(ALL_OBJECTS) main.cpp -L$(LIBRR_LIBS) -lroadrunner_c_api
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

libRoadrunner: 
ifeq ($(OS), Windows_NT)
	python beta/setup_libroadrunner.py
else
	python3 beta/setup_libroadrunner.py
endif

custom.o: ./custom_modules/custom.cpp 
	$(COMPILE_COMMAND) -c ./custom_modules/custom.cpp

librr_intracellular.o: ./addons/libRoadrunner/src/librr_intracellular.cpp ./addons/libRoadrunner/src/librr_intracellular.h
	$(COMPILE_COMMAND) -c ./addons/libRoadrunner/src/librr_intracellular.cpp


# cleanup
	
clean:
	rm -f *.o
	rm -f $(PROGRAM_NAME)*
	
data-cleanup:
	rm -f *.mat
	rm -f *.xml
	rm -f *.svg
	rm -rf ./output
	mkdir ./output
	touch ./output/empty.txt

reset:
	rm -f *.cpp PhysiCell_cell.o
	cp ./sample_projects/Makefile-default Makefile 
	rm -f ./custom_modules/*
	touch ./custom_modules/empty.txt 
	touch ALL_CITATIONS.txt 
	touch ./core/PhysiCell_cell.cpp
	rm ALL_CITATIONS.txt 
	cp ./config/PhysiCell_settings-backup.xml ./config/PhysiCell_settings.xml 
	touch ./config/empty.csv
	rm ./config/*.csv
	rm ./config/*.m
	rm ./config/*.cps
	rm ./config/Toy_Metabolic_Model.xml


FOLDER := output
FRAMERATE := 24
movie:
#	mencoder "mf://snapshot*.jpg" -ovc lavc -lavcopts vcodec=mpeg4:vbitrate=10000:mbd=2:trell -mf fps=$(FRAMERATE):type=jpg -nosound -o out.avi	
#	ffmpeg -i out.avi out.mp4 
	ffmpeg -r $(FRAMERATE) -f image2 -i snapshot%08d.jpg -vcodec libx264 -pix_fmt yuv420p -strict -2 -tune animation -crf 15 -acodec aac out.mp4
	#del snap*.jpg
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
