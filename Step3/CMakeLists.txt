cmake_minimum_required(VERSION 3.15)

# set the project name
project(Tutorial VERSION 1.0)  

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRC_LIST Tutorial.cpp)

# add an option
option(USE_MYMATH "Use tutorial provided math implementation" ON)

# configure a header file to pass the version number to the source code
configure_file(TutorialConfig.h.in TutorialConfig.h)

# if use mysqrt
if(USE_MYMATH)
  add_subdirectory(MathFunctions)
  list(APPEND EXTRA_LIBS MathFunctions)
  # 向列表变量 EXTRA_LIBS 中追加一个元素 "MathFunctions"
  #list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/MathFunctions")
  # 用EXTRA_LIBS列表来集合需要链接的库
  # 用EXTRA_INCLUDES列表来集合需要链接的头文件
  # 变量 EXTRA_LIBS 被用来收集任意可选的库，以便稍后将其链接到可执行文件中。类似地，变量 EXTRA_INCLUDES 用于收集可选的头文件
endif()

add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(${PROJECT_NAME} PUBLIC ${EXTRA_LIBS})

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
target_include_directories(${PROJECT_NAME} PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           #${EXTRA_INCLUDES}
                           )




