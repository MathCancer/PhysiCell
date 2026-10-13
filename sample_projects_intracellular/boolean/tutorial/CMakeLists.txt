get_filename_component(project ${CMAKE_CURRENT_SOURCE_DIR} NAME)
add_executable(${project})

file(GLOB MODULES "custom_modules/*.cpp")

target_sources(${project} PRIVATE main.cpp ${MODULES})

target_link_libraries(${project} PRIVATE BioFVM PhysiCell-core-no-addon
                                         PhysiCell-modules-no-addon)

target_include_directories(${project}
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../..)
target_include_directories(${project} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../..)

add_custom_command(
  TARGET ${project}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_CURRENT_SOURCE_DIR}/config ${CMAKE_CURRENT_BINARY_DIR}/config)

target_link_libraries(${project} PRIVATE PhysiBoSS_128n)
