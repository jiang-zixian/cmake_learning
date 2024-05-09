cmake_minimum_required(VERSION 3.15)

# set the project name
project(Tutorial VERSION 1.0)  

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRC_LIST Tutorial.cpp)

add_executable(${PROJECT_NAME} ${SRC_LIST})

# configure a header file to pass the version number to the source code
configure_file(TutorialConfig.h.in TutorialConfig.h)

target_include_directories(${PROJECT_NAME} PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )
