
set(sources ${sources} 
    logger.h
    logger.cpp
)

add_library(Utils ${sources})
#target_link_libraries(Network ${ARMADILLO_LIBRARIES})

