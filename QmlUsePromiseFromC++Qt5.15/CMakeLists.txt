cmake_minimum_required(VERSION 3.10)
project(QmlUsePromiseFromC++)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Qml Quick Concurrent REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Qml Quick Concurrent REQUIRED)

# release build
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Release Build, add /Zi /DEBUG /OPT:REF /OPT:ICF")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
    # set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
endif()

add_executable(QmlUsePromiseFromC++)
target_sources(QmlUsePromiseFromC++ PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Test.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ControllerControl.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ControllerControl.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qml.qrc
)
set_target_properties(QmlUsePromiseFromC++ PROPERTIES
    AUTOUIC ON
    AUTOMOC ON
    AUTORCC ON
    CXX_STANDARD_REQUIRED ON
    INCLUDE_CURRENT_DIR ON
    OUTPUT_NAME ${PROJECT_NAME})
target_link_libraries(QmlUsePromiseFromC++ PUBLIC 
    Qt5::Core 
    Qt5::Qml
    Qt5::Quick
    Qt5::Concurrent)
# target_include_directories(QmlUsePromiseFromC++ PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
# add_subdirectory(src)

if(QT_VERSION_MAJOR EQUAL 6)
qt_import_qml_plugins(QmlUsePromiseFromC++)
qt_finalize_executable(QmlUsePromiseFromC++)
endif()
