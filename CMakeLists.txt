# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)

project ("libProp")
set(CMAKE_CXX_STANDARD 11)

add_library(${PROJECT_NAME} SHARED libProp.h libProp.cpp ) 
#add_executable(${PROJECT_NAME} libProp.h libProp.cpp main.cpp) #test
