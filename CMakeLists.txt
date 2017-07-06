cmake_minimum_required(VERSION 3.5)
project(bashclass)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(BASH_PROJECT_DEV_EXEC "bashcdev")
set(BASH_PROJECT_PRO_EXEC "bashc")

# Add easycc CMakeLists.txt to build it automatically
add_subdirectory(EasyCC-CPP)

# Compile with -std=c++11 flag
add_compile_options(-std=c++11)

# Configure directory of output file
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include bashc header files (e.g. same as -I flag)
include_directories(include)
include_directories(EasyCC-CPP/include)
include_directories(EasyCC-CPP/rapidjson/include)

# Store cpp files in a variable
file(GLOB_RECURSE BASH_PROJECT_SOURCE_FILES src/*/*.cpp)
list(REMOVE_ITEM BASH_PROJECT_SOURCE_FILES
        "${PROJECT_SOURCE_DIR}/src/bashclass/maindev.cpp"
        "${PROJECT_SOURCE_DIR}/src/bashclass/mainpro.cpp")

# Add the executables
add_executable(${BASH_PROJECT_DEV_EXEC} ${BASH_PROJECT_SOURCE_FILES} src/bashclass/maindev.cpp)
add_executable(${BASH_PROJECT_PRO_EXEC} ${BASH_PROJECT_SOURCE_FILES} src/bashclass/mainpro.cpp)

# Link library to the executable
target_link_libraries(${BASH_PROJECT_DEV_EXEC} easyccdev)
target_link_libraries(${BASH_PROJECT_PRO_EXEC} easyccpro)
