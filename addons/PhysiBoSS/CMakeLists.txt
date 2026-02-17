set(MABOSS_VERSION v2.5.6)

function(create_physiboss_target MAX_NODES)
  set(TARGET_NAME PhysiBoSS_${MAX_NODES}n)
  add_library(${TARGET_NAME} OBJECT)

  target_sources(
    ${TARGET_NAME}
    PRIVATE src/maboss_intracellular.cpp src/maboss_network.cpp
            ${PROJECT_SOURCE_DIR}/core/PhysiCell_cell.cpp
            ${PROJECT_SOURCE_DIR}/modules/PhysiCell_MultiCellDS.cpp)

  if(NOT MAX_NODES EQUAL 64)
    set(MABOSS_LIB_NAME MaBoSS_${MAX_NODES}n-static)
  else()
    set(MABOSS_LIB_NAME MaBoSS-static)
  endif()

  if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(MABOSS_ARCHIVE libMaBoSS-linux64.tar.gz)
  elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin" AND CMAKE_SYSTEM_PROCESSOR
                                                 STREQUAL "x86_64")
    set(MABOSS_ARCHIVE libMaBoSS-osx64.tar.gz)
  elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin" AND CMAKE_SYSTEM_PROCESSOR
                                                 STREQUAL "arm64")
    set(MABOSS_ARCHIVE libMaBoSS-macos-arm64.tar.gz)
  elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(MABOSS_ARCHIVE libMaBoSS-win64.tar.gz)
  else()
    message(FATAL_ERROR "Unsupported OS: ${CMAKE_SYSTEM_NAME}")
  endif()

  include(FetchContent)
  FetchContent_Declare(
    maboss
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    URL https://github.com/sysbio-curie/MaBoSS/releases/download/${MABOSS_VERSION}/${MABOSS_ARCHIVE}
  )
  FetchContent_MakeAvailable(maboss)

  target_include_directories(${TARGET_NAME}
                             PUBLIC ${maboss_SOURCE_DIR}/engine/include)
  target_link_directories(${TARGET_NAME} PUBLIC ${maboss_SOURCE_DIR}/engine/lib)
  target_link_libraries(${TARGET_NAME} PUBLIC ${MABOSS_LIB_NAME})
  target_compile_definitions(${TARGET_NAME} PUBLIC MAXNODES=${MAX_NODES}
                                                   ADDON_PHYSIBOSS)
  find_package(OpenMP REQUIRED)
  target_link_libraries(${TARGET_NAME} PUBLIC OpenMP::OpenMP_CXX)
endfunction(create_physiboss_target)

create_physiboss_target(64)
create_physiboss_target(128)
create_physiboss_target(256)

add_library(PhysiBoSS ALIAS PhysiBoSS_64n)
