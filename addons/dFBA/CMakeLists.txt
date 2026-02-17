add_library(dFBA OBJECT)

file(GLOB SOURCES "src/*.cpp")
target_sources(
  dFBA PRIVATE ${SOURCES} ${PROJECT_SOURCE_DIR}/core/PhysiCell_cell.cpp
               ${PROJECT_SOURCE_DIR}/modules/PhysiCell_MultiCellDS.cpp)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(DFBA_ARCHIVE_KIND linux64)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  set(DFBA_ARCHIVE_KIND osx64)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  set(DFBA_ARCHIVE_KIND win64)
else()
  message(FATAL_ERROR "Unsupported OS: ${CMAKE_SYSTEM_NAME}")
endif()

find_package(ZLIB REQUIRED)
find_package(BZip2 REQUIRED)
find_package(LAPACK REQUIRED)
find_package(LibXml2 REQUIRED)

include(FetchContent)
FetchContent_Declare(
  coin
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
  URL https://maboss.curie.fr/pub/Clp-1.17.6-${DFBA_ARCHIVE_KIND}.zip)
FetchContent_MakeAvailable(coin)

FetchContent_Declare(
  libsbml
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
  URL https://maboss.curie.fr/pub/libSBML-5.19.0-${DFBA_ARCHIVE_KIND}.zip)
FetchContent_MakeAvailable(libsbml)

target_include_directories(dFBA PUBLIC ${libsbml_SOURCE_DIR}/include
                                       ${coin_SOURCE_DIR}/include)
target_link_directories(dFBA PUBLIC ${libsbml_SOURCE_DIR}/lib
                        ${coin_SOURCE_DIR}/lib)
target_link_libraries(
  dFBA
  PUBLIC Clp-static
         CoinUtils-static
         sbml-static
         BZip2::BZip2
         LAPACK::LAPACK
         ZLIB::ZLIB
         LibXml2::LibXml2)
target_compile_definitions(dFBA PUBLIC ADDON_PHYSIDFBA)

find_package(OpenMP REQUIRED)
target_link_libraries(dFBA PUBLIC OpenMP::OpenMP_CXX)
