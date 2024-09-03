cmake_minimum_required(VERSION 3.10)
project(QQmlPropertyMapExample)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Qml Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Qml Quick REQUIRED)

# release build
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Release Build, add /Zi /DEBUG /OPT:REF /OPT:ICF")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
    # set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
endif()

add_executable(QQmlPropertyMapExample)
target_sources(QQmlPropertyMapExample PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Parameter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/qml.qrc
)
set_target_properties(QQmlPropertyMapExample PROPERTIES
    AUTOUIC ON
    AUTOMOC ON
    AUTORCC ON
    CXX_STANDARD_REQUIRED ON
    INCLUDE_CURRENT_DIR ON
    OUTPUT_NAME ${PROJECT_NAME})
target_link_libraries(QQmlPropertyMapExample PUBLIC 
    Qt5::Core 
    Qt5::Qml
    Qt5::Quick)
# target_include_directories(QQmlPropertyMapExample PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
# add_subdirectory(src)

if(QT_VERSION_MAJOR EQUAL 6)
qt_import_qml_plugins(QQmlPropertyMapExample)
qt_finalize_executable(QQmlPropertyMapExample)
endif()
