
set(sources ${sources} 
    node.cpp
    node.h
    processingUnit.cpp
    processingUnit.h
)

find_package(Armadillo REQUIRED)

include_directories(${ARMADILLO_INCLUDE_DIRS})

add_library(Network ${sources})
target_link_libraries(Network ${ARMADILLO_LIBRARIES})

