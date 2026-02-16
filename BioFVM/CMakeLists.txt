add_library(BioFVM OBJECT)

file(GLOB SOURCES "*.cpp")
target_sources(BioFVM PRIVATE ${SOURCES})

find_package(OpenMP REQUIRED)
target_link_libraries(BioFVM PUBLIC OpenMP::OpenMP_CXX)
