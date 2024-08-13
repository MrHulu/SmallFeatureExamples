cmake_minimum_required(VERSION 3.10)
project(CrashpadExample)

# release build
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Release Build, add /Zi /DEBUG /OPT:REF /OPT:ICF")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
    # set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
endif()

add_executable(CrashpadExample main.cpp) 
target_include_directories(CrashpadExample PRIVATE  
    ${CMAKE_SOURCE_DIR}/crashpad/include
    ${CMAKE_SOURCE_DIR}/crashpad/include/out/Default/gen
    ${CMAKE_SOURCE_DIR}/crashpad/include/mini_chromium
)
target_link_directories(CrashpadExample PRIVATE ${CMAKE_SOURCE_DIR}/crashpad/lib_md)
target_link_libraries(CrashpadExample PRIVATE client util base common Advapi32)


file(COPY ${CMAKE_SOURCE_DIR}/crashpad/bin/crashpad_handler.exe DESTINATION ${CMAKE_BINARY_DIR})