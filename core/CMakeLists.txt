# Here we are creating two libraries:
# 1) PhysiCell-core: includes all core PhysiCell functionality
# 2) PhysiCell-core-no-addon: includes all core PhysiCell functionality, excluding the cell class
# The latter is necessary for projects that define ADDON_* macro for the specific intracellular support

add_library(PhysiCell-core OBJECT)
add_library(PhysiCell-core-no-addon OBJECT)

target_sources(
  PhysiCell-core
  PRIVATE PhysiCell_basic_signaling.cpp
          PhysiCell_cell_container.cpp
          PhysiCell_cell.cpp
          PhysiCell_constants.cpp
          PhysiCell_custom.cpp
          PhysiCell_phenotype.cpp
          PhysiCell_rules.cpp
          PhysiCell_signal_behavior.cpp
          PhysiCell_standard_models.cpp
          PhysiCell_utilities.cpp)

target_sources(
  PhysiCell-core-no-addon
  PRIVATE PhysiCell_basic_signaling.cpp
          PhysiCell_cell_container.cpp
          PhysiCell_constants.cpp
          PhysiCell_custom.cpp
          PhysiCell_phenotype.cpp
          PhysiCell_rules.cpp
          PhysiCell_signal_behavior.cpp
          PhysiCell_standard_models.cpp
          PhysiCell_utilities.cpp)

find_package(OpenMP REQUIRED)
target_link_libraries(PhysiCell-core PUBLIC OpenMP::OpenMP_CXX)
target_link_libraries(PhysiCell-core-no-addon PUBLIC OpenMP::OpenMP_CXX)
