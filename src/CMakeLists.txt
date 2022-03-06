#define target or something and use all that stuff


add_subdirectory(Network)

# add the executable
add_executable(main main.cpp)

target_link_libraries(main PUBLIC Network)

target_include_directories(main PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/Network"
                          )