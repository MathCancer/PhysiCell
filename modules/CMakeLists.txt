# Here we are creating two libraries:
# 1) PhysiCell-modules: includes all modules functionality
# 2) PhysiCell-modules-no-addon: includes all modules functionality, excluding the multicellds class
# The latter is necessary for projects that define ADDON_* macro for the specific intracellular support

add_library(PhysiCell-modules OBJECT)
add_library(PhysiCell-modules-no-addon OBJECT)

target_sources(
  PhysiCell-modules
  PRIVATE PhysiCell_geometry.cpp
          PhysiCell_MultiCellDS.cpp
          PhysiCell_pathology.cpp
          PhysiCell_POV.cpp
          PhysiCell_pugixml.cpp
          PhysiCell_settings.cpp
          PhysiCell_SVG.cpp
          PhysiCell_various_outputs.cpp)

target_sources(
  PhysiCell-modules-no-addon
  PRIVATE PhysiCell_geometry.cpp
          PhysiCell_pathology.cpp
          PhysiCell_POV.cpp
          PhysiCell_pugixml.cpp
          PhysiCell_settings.cpp
          PhysiCell_SVG.cpp
          PhysiCell_various_outputs.cpp)

find_package(OpenMP REQUIRED)
target_link_libraries(PhysiCell-modules PUBLIC OpenMP::OpenMP_CXX)
target_link_libraries(PhysiCell-modules-no-addon PUBLIC OpenMP::OpenMP_CXX)
