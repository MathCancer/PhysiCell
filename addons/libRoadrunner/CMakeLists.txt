add_library(libRoadrunner OBJECT)

file(GLOB SOURCES "src/*.cpp")
target_sources(
  libRoadrunner
  PRIVATE ${SOURCES} ${SOURCES} ${PROJECT_SOURCE_DIR}/core/PhysiCell_cell.cpp
          ${PROJECT_SOURCE_DIR}/modules/PhysiCell_MultiCellDS.cpp)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(LIBROADRUNNER_ARCHIVE_URL
      https://sourceforge.net/projects/libroadrunner/files/libroadrunner-1.3.3/alpha-rhel-6.6/roadrunner-1.3.3-alpha.tar.gz/download
  )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin" AND CMAKE_SYSTEM_PROCESSOR STREQUAL
                                               "x86_64")
  set(LIBROADRUNNER_ARCHIVE_URL
      https://sourceforge.net/projects/libroadrunner/files/libroadrunner-1.4.18/roadrunner-osx-10.9-cp36m.tar.gz/download
  )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin" AND CMAKE_SYSTEM_PROCESSOR STREQUAL
                                               "arm64")
  set(LIBROADRUNNER_ARCHIVE_URL
      https://github.com/PhysiCell-Tools/intracellular_libs/raw/main/ode/roadrunner_macos_arm64.tar.gz
  )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  set(LIBROADRUNNER_ARCHIVE_URL
      https://sourceforge.net/projects/libroadrunner/files/libroadrunner-1.4.18/roadrunner-win64-vs14-cp35m.zip/download
  )
else()
  message(FATAL_ERROR "Unsupported OS: ${CMAKE_SYSTEM_NAME}")
endif()

include(FetchContent)
FetchContent_Declare(
  roadrunner
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
  URL ${LIBROADRUNNER_ARCHIVE_URL})
FetchContent_MakeAvailable(roadrunner)

target_include_directories(libRoadrunner
                           PUBLIC ${roadrunner_SOURCE_DIR}/include/rr/C)
target_link_directories(libRoadrunner PUBLIC ${roadrunner_SOURCE_DIR}/lib)
target_link_libraries(libRoadrunner PUBLIC roadrunner_c_api)
target_compile_definitions(libRoadrunner PUBLIC ADDON_ROADRUNNER)

find_package(OpenMP REQUIRED)
target_link_libraries(libRoadrunner PUBLIC OpenMP::OpenMP_CXX)
