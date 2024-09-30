cmake_minimum_required(VERSION 3.10)
project(ScopedQObjectConnection)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)

# release build
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Release Build, add /Zi /DEBUG /OPT:REF /OPT:ICF")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
    # set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
endif()

add_executable(ScopedQObjectConnection)
target_sources(ScopedQObjectConnection PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Test.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ScopedConnection.h
)
set_target_properties(ScopedQObjectConnection PROPERTIES
    AUTOUIC ON
    AUTOMOC ON
    AUTORCC ON
    CXX_STANDARD_REQUIRED ON
    INCLUDE_CURRENT_DIR ON
    OUTPUT_NAME ${PROJECT_NAME})
target_link_libraries(ScopedQObjectConnection PUBLIC Qt5::Core)

if(QT_VERSION_MAJOR EQUAL 6)
qt_import_qml_plugins(ScopedQObjectConnection)
qt_finalize_executable(ScopedQObjectConnection)
endif()
