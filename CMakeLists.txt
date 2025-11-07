cmake_minimum_required(VERSION 3.10)

# 项目设置
project(ImageProcessor)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加额外的编译标志
if(MSVC)
    # 为Visual Studio添加编译标志
    add_compile_options(/std:c++17 /EHsc)
else()
    # 为其他编译器（如MinGW、GCC）添加编译标志
    add_compile_options(-std=c++17 -pthread)
endif()

# 确保编译器使用正确的C++标准库
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")

# 指定构建类型
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE})

# 设置链接器标志
if(MSVC)
    # 为Visual Studio添加链接器标志
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
else()
    # 为其他编译器（如MinGW、GCC）添加链接器标志以确保线程库被正确链接
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")
endif()

# 添加OpenCV依赖
# 使用系统安装的OpenCV
# 根据查看的文件结构，系统上的OpenCV是为Visual Studio 2019构建的版本
set(OpenCV_DIR "F:/opencv/build") # 这是OpenCVConfig.cmake所在的路径
find_package(OpenCV REQUIRED)

# 添加Crow库依赖
# 添加头文件目录
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/3rdparty
    ${CMAKE_SOURCE_DIR}/3rdparty/crow/include
    ${CMAKE_SOURCE_DIR}/3rdparty/asio/include
    ${OpenCV_INCLUDE_DIRS}
)

# 添加源文件
file(GLOB_RECURSE SOURCES
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

# 创建可执行文件
add_executable(${PROJECT_NAME} ${SOURCES})

# 链接库
target_link_libraries(${PROJECT_NAME} 
    ${OpenCV_LIBS}
    -lpthread
)