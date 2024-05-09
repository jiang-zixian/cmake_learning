# cmake学习笔记
## cmake 安装
略
## 跟着官方教程学习
进入学习文件夹
```bash
cd /home/jiangzixian/cmake_learning
```
### A Basic Starting Point (Step1)
1. 新建Step1文件夹
2. 新建Step1_build文件夹
3. 在Step1文件夹中编写CMakeLists.txt和Tutorial.cpp文件
CMakeLists.txt如下
```cmake
cmake_minimum_required(VERSION 3.15)

# set the project name 设置项目名字和项目版本号
project(Tutorial VERSION 1.0)

# add the executable 指定了要构建的可执行文件的名称（Tutorial）和源代码文件（Tutorial.cpp）。它告诉CMake将Tutorial.cpp编译为一个可执行文件
add_executable(Tutorial Tutorial.cpp)

# configure a header file to pass the version number to the source code 这一行用于配置一个头文件，将版本号传递给源代码。它会根据输入文件TutorialConfig.h.in生成输出文件TutorialConfig.h
configure_file(TutorialConfig.h.in TutorialConfig.h)

# 将构建目标Tutorial的编译目录（PROJECT_BINARY_DIR）添加到目标的包含目录中。这样，在编译过程中，TutorialConfig.h文件将能够被正确地包含到源代码中（上一行命令生成的.h文件就在PROJECT_BINARY_DIR文件里）
target_include_directories(Tutorial PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )
```

Tutorial.cpp如下,用于求一个数字的平方根
```cpp
// tutorial.cpp

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " number" << std::endl;
        return 1;
    }

    // convert input to double
    const double inputValue = atof(argv[1]);

    // calculate square root
    const double outputValue = sqrt(inputValue);
    std::cout << "The square root of " << inputValue
              << " is " << outputValue
              << std::endl;
    return 0;
}
```
1. Step1文件夹下编写TutorialConfig.h.in
```cpp
// the configured options and settings for Tutorial
#define Tutorial_VERSION_MAJOR @PROJECT_VERSION_MAJOR@
#define Tutorial_VERSION_MINOR @PROJECT_VERSION_MINOR@
```
当使用 CMake 构建项目后，会在 Step1_build 中生成一个 TutorialConfig.h 文件，内容如下：
```cpp
// the configured options and settings for Tutorial
#define Tutorial_VERSION_MAJOR 1
#define Tutorial_VERSION_MINOR 0
```

1. 在Step1_build文件夹下cmake:
```bash
cmake ../Step1
```
这样可获得CMakeFile

5. 然后编译：
```bash
cmake --build .
```
或者
```bash
make
```

#### Specify the C++ Standard
指定C++版本
1. 修改Tutorial.cpp文件
replacing atof with std::stod in tutorial.cxx.
将`const double inputValue = atof(argv[1]);`修改成`const double inputValue = std::stod(argv[1]);`

后者是C++11的语句

2. 修改CMakeLists.txt
在`add_executable`前添加：
```cmake
# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
```
3. 按步骤编译

### Adding a Library (Step 2)
我们将向项目中添加一个库，这个库包含计算数字平方根的实现，可执行文件使用这个库，而不是编译器提供的标准平方根函数

此实验文件夹结构如下：
```bash
root@LAPTOP-LFRB2EHI:/home/jiangzixian/cmake_learning/Step2# tree
.
├── CMakeLists.txt
└── MathFunctions
    ├── CMakeLists.txt
    ├── MathFunctions.h
    └── mysqrt.cpp
```

MathFunctions文件夹的CMakeLists.txt如下：
```cmake
# MathFunctions/CMakeLists.txt
add_library(MathFunctions mysqrt.cpp)
# 表示添加一个叫 MathFunctions 的库文件
```

Step2/CMakeListes.txt如下：
```cmake
cmake_minimum_required(VERSION 3.15)

# set the project name
project(Tutorial VERSION 1.0)  

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRC_LIST Tutorial.cpp)

# 指定库所在子目录，该子目录下应包含 CMakeLists.txt 文件和代码文件
add_subdirectory(MathFunctions)

# 添加可执行文件
add_executable(${PROJECT_NAME} ${SRC_LIST})

# find library files that should be included
# 找到库文件
# 将库文件MathFunctions中的函数和数据与目标${PROJECT_NAME}组合在一起，生成最终的可执行文件
target_link_libraries(${PROJECT_NAME} PUBLIC MathFunctions)

# configure a header file to pass the version number to the source code
configure_file(TutorialConfig.h.in TutorialConfig.h)

# 找到对应的头文件
# find header files that should be included
target_include_directories(${PROJECT_NAME} PUBLIC
                           ${PROJECT_BINARY_DIR}
                           ${PROJECT_SOURCE_DIR}/MathFunctions
                           )

```

#### 添加库文件选项(make the MathFunctions library optional)
使用USE_MYMATH变量来控制用sqrt还是自己写的mysqrt

1. 顶端CMakeLists.txt：
```cmake
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
  list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/MathFunctions")
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
                           ${EXTRA_INCLUDES}
                           )

```
**注意**：
> 1:用`option(USE_MYMATH "Use tutorial provided math implementation" ON)`来添加了USE_MYMATH的变量选项
> 2:用if来判断USE_MTMATH变量的值
> 3:用list关键字来建立和添加列表项来灵活管理需要链接的库文件和头文件

2. Tutorial.cpp文件修改如下：添加一些判断：
```cpp
// tutorial.cpp

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <TutorialConfig.h>
#ifdef USE_MYMATH
#  include "MathFunctions.h"
#endif

int main(int argc, char* argv[])
{
    if (argc < 2) {
    	// report version
    	std::cout << argv[0] << " Version " << Tutorial_VERSION_MAJOR << "."
              << Tutorial_VERSION_MINOR << std::endl;
    	std::cout << "Usage: " << argv[0] << " number" << std::endl;
    	return 1;
    }
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " number" << std::endl;
        return 1;
    }

    // convert input to double
    //const double inputValue = atof(argv[1]);
     const double inputValue = std::stod(argv[1]);

    // calculate square root
    #ifdef USE_MYMATH
    const double outputValue = mysqrt(inputValue);
    std::cout << "The mysquare root of " << inputValue
              << " is " << outputValue
              << std::endl;
    #else
    const double outputValue = sqrt(inputValue);
    std::cout << "The square root of " << inputValue
              << " is " << outputValue
              << std::endl;
    #endif

    
    return 0;
}

```

3. 最后要在TutorialConfig.h.in文件添加以下行：
```in
#cmakedefine USE_MYMATH
```

4. 设置编译选项
```bash
cmake .. -DUSE_MYMATH=OFF
```

### Adding Usage Requirements for Library (Step 3)
更好的控制库文件和头文件的链接，常常用到以下关键字：
- target_compile_definitions
- target_compile_options
- target_include_directories
- target_link_libraries

1. 在MathFunctions/CMakeLists.txt末尾添加以下行
```cmake
# MathFunctions/CMakeLists.txt
target_include_directories(MathFunctions
          INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
          )
        
```
CMAKE_CURRENT_SOURCE_DIR 表示 MathFunctions 库所在目录

现在只要是链接了 MathFunctions 库，就会**自动包含 MathFunctions 所在目录的头文件**，简洁而优雅

> INTERFACE:链接 MathFunctions 库的任何可执行文件/库文件都需要包含 MathFunctions 目录作为头文件路径，而 MathFunctions 本身不需要包含，这被称为 INTERFACE 使用要求


> 注意：这里使用INTERFACE，还有PRIVATE 和 PUBLIC。INTERFACE表示消费者需要生产者不需要，PRIVATE表示消费者不需要生产者需要，PUBLIC 表示消费者和生产者都需要

2. 那么可以从顶级 CMakeLists.txt 中删除EXTRA_INCLUDES变量的相关使用
```cmake
if(USE_MYMATH)
  add_subdirectory(MathFunctions)
  list(APPEND EXTRA_LIBS MathFunctions)
  list(APPEND EXTRA_INCLUDES ${PROJECT_SOURCE_DIR}/MathFunctions)   # 删除此行
endif()

...

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
target_include_directories(${PROJECT_NAME} PUBLIC
                           ${PROJECT_BINARY_DIR}
                           ${EXTRA_INCLUDES}   # 删除此行
                           )
```




## CMakeLists.txt语法
### cmake_minimum_required(VERSION 3.15)

### project(Tutorial VERSION 1.0)
//set the project name 设置项目名字和项目版本号

### add_ the _executable 生成可执行文件
指定了要构建的可执行文件的名称（Tutorial）和源代码文件（Tutorial.cpp）。它告诉CMake将Tutorial.cpp编译为一个可执行文件
```cmake
add_executable(Tutorial Tutorial.cpp)
```

### add_library 生成库文件
```cmake
add_library(MathFunctions mysqrt.cpp)
# 表示添加一个叫 MathFunctions 的库文件
```

### configure_file 

构建过程中生成文件。它的主要作用是根据给定的输入文件，在构建时生成一个输出文件，并可以在输出文件中替换一些预定义的变量。

```cmake
configure_file(TutorialConfig.h.in TutorialConfig.h)
# 这一行用于配置一个头文件，将版本号传递给源代码。它会根据输入文件TutorialConfig.h.in生成输出文件TutorialConfig.h
```
### *** target_link_libraries

- 在add_executable之后使用

- CMake中用于将目标（例如可执行文件、静态库或共享库）与其他库文件进行链接的关键字。它用于指定目标所依赖的库文件，使得在构建过程中可以将这些库文件与目标一起链接以生成最终的可执行文件或库

```cmake
# 将库文件MathFunctions中的函数和数据与目标${PROJECT_NAME}组合在一起，生成最终的可执行文件
target_link_libraries(${PROJECT_NAME} PUBLIC MathFunctions)
```


### *** target_include_directories

- 在add_executable之后使用

- CMake中用于指定目标（例如可执行文件、静态库或共享库）的**头文件搜索路径**的关键字。它用于告诉CMake编译器在构建目标时应该搜索哪些目录以包含头文件

- 第一个参数是：目标的名称，可以是可执行文件、静态库或共享库的名称

 ```cmake
 target_include_directories(Tutorial PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )
# 将构建目标Tutorial的编译目录（PROJECT_BINARY_DIR）添加到目标的包含目录中。这样，在编译过程中，TutorialConfig.h文件将能够被正确地包含到源代码中（生成的.h文件就在PROJECT_BINARY_DIR文件里） 
# 第一个参数可以用${PROJECT_NAME}代替
 ```


### CMAKE_CXX_STANDARD
指定C++标准版本
```cmake
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
```

### if() ....endif()
判断语句

### option
用于定义用户可配置的选项。它允许用户在构建过程中选择性地启用或禁用某些功能。option关键字提供一个布尔值，可以设置为ON或OFF，通过cmake或命令行选项进行配置。
示例：
```cmake
option(USE_MATH_FUNCTIONS "Enable math functions" ON)
```
USE_MATH_FUNCTIONS 是一个选项的名称，用于启用或禁用数学函数。ON表示默认启用该选项，用户可以通过命令行或cmake-gui来更改该值
对应的cmake设置语句为：
```bash
cmake .. -DUSE_MYMATH=OFF
```
### list
对列表进行操作和处理。
例如，list(APPEND)命令用于将一个或多个元素追加到列表的末尾，list(GET)命令用于获取列表中指定索引位置的元素，list(LENGTH)命令用于获取列表的长度等等



## 头文件和库文件
当涉及到C++项目时，库文件和头文件是两个不同的概念，它们在项目中具有不同的作用。

**头文件（Header Files）**：
头文件通常具有`.h`或`.hpp`的扩展名，在C++项目中用于声明类、函数、变量和常量等的接口。它们包含了函数原型、类的定义、宏定义等代码片段，用于告知编译器这些实体的存在。头文件不包含实际的实现代码，只是提供了对这些实体的声明和描述。头文件通常用于在多个源文件之间共享代码。

例如，假设你有一个名为`mylib`的库，其中定义了一个名为`myfunc`的函数。你可以在头文件`mylib.h`中声明该函数：
```cpp
// mylib.h
#ifndef MYLIB_H
#define MYLIB_H

void myfunc();

#endif
```
然后，在需要使用该函数的源文件中，通过包含头文件`mylib.h`，可以访问到`myfunc`的声明：
```cpp
// main.cpp
#include "mylib.h"

int main() {
    myfunc();
    return 0;
}
```
头文件在编译时被包含到源文件中，以便编译器了解函数的声明，但实际的函数定义和实现可能在另一个源文件中。

**库文件（Library Files）**：
库文件是已编译和链接的二进制文件，包含了函数和数据结构的实现代码。它们可以是静态库（`.lib`或`.a`文件）或共享库/动态库（`.dll`、`.so`或`.dylib`文件）。库文件可以包含一个或多个目标文件的代码，以及必要的元数据，用于在链接时将它们与其他目标文件和库文件组合成最终的可执行文件。

库文件通常由开发者提供，供其他开发者在其项目中使用。通过将库文件链接到项目中，可以使用库中定义的函数、类和变量等功能。

例如，如果你有一个名为`mylib`的库，其中包含了`myfunc`函数的实现，你可以将库文件`mylib.lib`链接到你的可执行文件中：
```cmake
add_executable(myapp main.cpp)
target_link_libraries(myapp mylib)
```
这样，在构建`myapp`时，链接器会将`mylib.lib`中的函数和数据与`myapp`的目标文件组合在一起，生成最终的可执行文件。


# 学习资料
[CMake官方教程](https://cmake.org/cmake/help/book/mastering-cmake/cmake/Help/guide/tutorial/index.html#adding-usage-requirements-for-library-step-3)
[CMake官方练习题](https://cmake.org/cmake/help/latest/guide/tutorial/A%20Basic%20Starting%20Point.html#exercise-1-building-a-basic-project)
