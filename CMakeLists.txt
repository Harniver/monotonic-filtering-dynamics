# CMake 3.9 or newer
cmake_minimum_required(VERSION 3.9)

# Compiling options
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -march=native")
if(WIN32)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_EXE_LINKER_FLAGS "-static")
endif(WIN32)

# Adding fcpp
option(FCPP_BUILD_GL "Build OpenGL features." ON) # this tells CMake to compile fcpp's OpenGL features
add_subdirectory(./fcpp/src/)
target_compile_options(fcpp PUBLIC "-DFCPP_CONFIGURATION=FCPP_CONFIGURATION_DEPENDENT")
#target_compile_options(fcpp PUBLIC "-DPROFILER")

project(monotonic_filtering
    VERSION 0.1.0
    DESCRIPTION "Simulation of G+C combination with and without monotonic filtering."
)
add_executable(monotonic_filtering ./run/gui.cpp)
set_target_properties(monotonic_filtering PROPERTIES VERSION &{PROJECT_VERSION})
target_link_libraries(monotonic_filtering PRIVATE fcpp)
target_include_directories(monotonic_filtering PRIVATE ./)
copyGraphicsFolders(monotonic_filtering $<TARGET_FILE_DIR:fcpp> $<TARGET_FILE_DIR:monotonic_filtering>)
copyTextureFolder(monotonic_filtering ${PROJECT_SOURCE_DIR} $<TARGET_FILE_DIR:monotonic_filtering>)
