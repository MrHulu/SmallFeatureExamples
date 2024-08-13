cmake_minimum_required(VERSION 3.10)
project(CMakeBuildPresetsExample)

# 定义构建信息变量
function(get_target_architecture target_architecture)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(${target_architecture} "x64" PARENT_SCOPE)
    else()
        set(${target_architecture} "x86" PARENT_SCOPE)
    endif()
endfunction()
get_target_architecture(ARCHITECTURE)
add_compile_definitions(
    BUILD_TYPE="${CMAKE_BUILD_TYPE}"
    CMAKE_GENERATOR="${CMAKE_GENERATOR}"   
    ARCHITECTURE="${ARCHITECTURE}"
)

# 添加源文件
add_executable(CMakeBuildPresetsExample main.cpp) 
