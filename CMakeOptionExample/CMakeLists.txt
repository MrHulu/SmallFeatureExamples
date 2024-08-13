cmake_minimum_required(VERSION 3.10)
project(CMakeOptionExample)

# 定义选项
option(ENABLE_LOGGING "Enable logging functionality" ON)

# 根据选项设置编译定义
if(ENABLE_LOGGING)
    add_definitions(-DENABLE_LOGGING)
endif()

# 添加源文件
add_executable(CMakeOptionExample main.cpp) 
