VERSION := $(shell grep . VERSION.txt | cut -f1 -d:)
PROGRAM_NAME := project

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
# ARCH := core-avx-i # i7 ivy bridge or newer 
# ARCH := core-avx2 # i7 with Haswell or newer
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

PhysiCell_custom_module_OBJECTS := .o

pugixml_OBJECTS := pugixml.o

PhysiCell_OBJECTS := $(BioFVM_OBJECTS)  $(pugixml_OBJECTS) $(PhysiCell_core_OBJECTS) $(PhysiCell_module_OBJECTS)
ALL_OBJECTS := $(PhysiCell_OBJECTS) $(PhysiCell_custom_module_OBJECTS)

EXAMPLES := ./examples/PhysiCell_test_mechanics_1.cpp ./examples/PhysiCell_test_mechanics_2.cpp \
 ./examples/PhysiCell_test_DCIS.cpp ./examples/PhysiCell_test_HDS.cpp \
 ./examples/PhysiCell_test_cell_cycle.cpp ./examples/PhysiCell_test_volume.cpp 

all: 
	make heterogeneity-sample
	make 

name:
	@echo ""
	@echo "Executable name is" $(PROGRAM_NAME)
	@echo ""

# sample projects 	
list-projects:
	@echo "Sample projects: template biorobots-sample cancer-biorobots-sample cancer-immune-sample"
	@echo "                 celltypes3-sample heterogeneity-sample pred-prey-farmer virus-macrophage-sample"
	@echo "                 worm-sample interaction-sample mechano-sample rules-sample physimess-sample custom-division-sample"
	@echo "                 asymmetric-division-sample immune-function-sample episode-sample"
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

immune-function-sample:
	cp -r ./sample_projects/immune_function/custom_modules/* ./custom_modules/
	touch main.cpp && cp main.cpp main-backup.cpp
	cp ./sample_projects/immune_function/main.cpp ./main.cpp 
	cp Makefile Makefile-backup
	cp ./sample_projects/immune_function/Makefile .
	cp ./config/PhysiCell_settings.xml ./config/PhysiCell_settings-backup.xml 
	cp -r ./sample_projects/immune_function/config/* ./config/

episode-sample:
	cp -r ./sample_projects/episode/custom_modules/* ./custom_modules/
	touch main.cpp && cp main.cpp main-backup.cpp
	cp ./sample_projects/episode/main.cpp ./main.cpp
	cp Makefile Makefile-backup
	cp ./sample_projects/episode/Makefile .
	cp -r ./sample_projects/episode/config/* ./config

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
	
PhysiCell_basic_signaling.o: ./core/PhysiCell_basic_signaling.cpp
	$(COMPILE_COMMAND) -c ./core/PhysiCell_basic_signaling.cpp 

PhysiCell_geometry.o: ./modules/PhysiCell_geometry.cpp
	$(COMPILE_COMMAND) -c ./modules/PhysiCell_geometry.cpp 

# user-defined PhysiCell modules

# cleanup

reset:
	rm -f *.cpp PhysiCell_cell.o
	cp ./sample_projects/Makefile-default Makefile 
	rm -rf ./custom_modules/*
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
	rm -rf ./output
	mkdir ./output
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

# easier animation 

FRAMERATE := 24
OUTPUT := output

jpeg: 
	@magick identify -format "%h" $(OUTPUT)/initial.svg > __H.txt 
	@magick identify -format "%w" $(OUTPUT)/initial.svg > __W.txt 
	@expr 2 \* \( $$(grep . __H.txt) / 2 \) > __H1.txt 
	@expr 2 \* \( $$(grep . __W.txt) / 2 \) > __W1.txt 
	@echo "$$(grep . __W1.txt)!x$$(grep . __H1.txt)!" > __resize.txt 
	@magick mogrify -format jpg -resize $$(grep . __resize.txt) $(OUTPUT)/s*.svg
	rm -f __H*.txt __W*.txt __resize.txt 
	
gif: 
	magick convert $(OUTPUT)/s*.svg $(OUTPUT)/out.gif 
	 
movie:
	ffmpeg -r $(FRAMERATE) -f image2 -i $(OUTPUT)/snapshot%08d.jpg -vcodec libx264 -pix_fmt yuv420p -strict -2 -tune animation -crf 15 -acodec none $(OUTPUT)/out.mp4
	
# upgrade rules 

SOURCE := PhysiCell_upgrade.zip 
get-upgrade: 
	@echo $$(curl https://raw.githubusercontent.com/MathCancer/PhysiCell/master/VERSION.txt) > VER.txt 
	@echo https://github.com/MathCancer/PhysiCell/releases/download/$$(grep . VER.txt)/PhysiCell_V.$$(grep . VER.txt).zip > DL_FILE.txt 
	rm -f VER.txt
	$$(curl -L $$(grep . DL_FILE.txt) --output PhysiCell_upgrade.zip)
	rm -f DL_FILE.txt 

PhysiCell_upgrade.zip: 
	make get-upgrade 

upgrade: $(SOURCE)
	unzip $(SOURCE) PhysiCell/VERSION.txt
	mv -f PhysiCell/VERSION.txt . 
	unzip $(SOURCE) PhysiCell/core/* 
	cp -r PhysiCell/core/* core 
	unzip $(SOURCE) PhysiCell/modules/* 
	cp -r PhysiCell/modules/* modules 
	unzip $(SOURCE) PhysiCell/sample_projects/* 
	cp -r PhysiCell/sample_projects/* sample_projects 
	unzip $(SOURCE) PhysiCell/BioFVM/* 
	cp -r PhysiCell/BioFVM/* BioFVM
	unzip $(SOURCE) PhysiCell/documentation/User_Guide.pdf
	mv -f PhysiCell/documentation/User_Guide.pdf documentation
	rm -f -r PhysiCell
	rm -f $(SOURCE) 

# use: make save PROJ=your_project_name
PROJ := my_project

save: 
	echo "Saving project as $(PROJ) ... "
	mkdir -p ./user_projects
	mkdir -p ./user_projects/$(PROJ)
	mkdir -p ./user_projects/$(PROJ)/custom_modules
	mkdir -p ./user_projects/$(PROJ)/config 
	cp main.cpp ./user_projects/$(PROJ)
	cp Makefile ./user_projects/$(PROJ)
	cp VERSION.txt ./user_projects/$(PROJ)
	cp -r ./config/* ./user_projects/$(PROJ)/config
	cp -r ./custom_modules/* ./user_projects/$(PROJ)/custom_modules

load: 
	echo "Loading project from $(PROJ) ... "
	cp ./user_projects/$(PROJ)/main.cpp .
	cp ./user_projects/$(PROJ)/Makefile .
	cp -r ./user_projects/$(PROJ)/config/* ./config/ 
	cp -r ./user_projects/$(PROJ)/custom_modules/* ./custom_modules/ 

pack:
	@echo " "
	@echo "Preparing project $(PROJ) for sharing ... "
	@echo " " 
	cd ./user_projects && zip -r $(PROJ).zip $(PROJ)
	@echo " "
	@echo "Share ./user_projects/$(PROJ).zip ... "
	@echo "Other users can unzip $(PROJ).zip in their ./user_projects, compile, and run."
	@echo " " 

unpack:
	@echo " "
	@echo "Preparing shared project $(PROJ).zip for use ... "
	@echo " " 
	cd ./user_projects && unzip $(PROJ).zip 
	@echo " "
	@echo "Load this project via make load PROJ=$(PROJ) ... "
	@echo " " 	

list-user-projects:
	@echo "user projects::"
	@cd ./user_projects && ls -dt1 * | grep . | sed 's!empty.txt!!'
