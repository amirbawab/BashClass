cmake_minimum_required(VERSION 3.5)
project(bashclass)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(BASH_PROJECT_EXEC "bashc")

# Add easycc CMakeLists.txt to build it automatically
add_subdirectory(EasyCC-CPP)

# Compile with -std=c++11 flag
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Include bashc header files (e.g. same as -I flag)
include_directories(include)
include_directories(EasyCC-CPP/include)
include_directories(EasyCC-CPP/rapidjson/include)

# Store cpp files in a variable
file(GLOB_RECURSE BASH_PROJECT_SOURCE_FILES src/*/*.cpp)

# Add the primary executable
add_executable(${BASH_PROJECT_EXEC} ${BASH_PROJECT_SOURCE_FILES})

# Link library to the executable
target_link_libraries(${BASH_PROJECT_EXEC} easyccdev)
