set(ALL_EXAMPLES cell_cycle DCIS HDS mechanics_1 mechanics_2 volume)

foreach(example ${ALL_EXAMPLES})
  add_executable(${example})
  target_sources(${example} PRIVATE PhysiCell_test_${example}.cpp)

  target_link_libraries(${example} PRIVATE BioFVM PhysiCell-core
                                           PhysiCell-modules)
endforeach()
