
set(sources ${sources} 
    node.cpp
    node.h
    processingUnit.cpp
    processingUnit.h
    parameterServer.cpp
    parameterServer.h
    event.h
    environment.cpp
    environment.h
)

#find_package(Armadillo REQUIRED)

include_directories("${PROJECT_SOURCE_DIR}/Utils")

add_library(Network ${sources})
target_link_libraries(Network PUBLIC Utils)

target_include_directories(Network PUBLIC
                          "${PROJECT_SOURCE_DIR}/Utils"
                          )