cmake_minimum_required (VERSION 3.28.3)
project (dots_and_boxes VERSION 1.0.0 LANGUAGES CXX)

if (PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message (
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif ()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_options(-g)
endif()

# ---- Init ----

# qt6: uic system for changing "*.ui" to "ui_*.h"

set (CMAKE_AUTOUIC OFF)

# qt6: qt moc system for dealing Q_OBJECT class

set (CMAKE_AUTOMOC OFF)

# qt6: resource compiler for dealing "*.qrc" file and package resource into
# binary

set (CMAKE_AUTORCC OFF)

# c++20: init
set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
set (CMAKE_CXX_STANDARD_REQUIRED ON) # Forcing using CMAKE_CXX_STANDARD

# Outputting to the file "build/compile_commands.json"
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

# c++20 module:
#set (CMAKE_CXX_SCAN_FOR_MODULES ON)

# cpm: init

include (../cmake/CPM.cmake)
cpmaddpackage ("gh:TheLartians/PackageProject.cmake@1.8.0")

cpmaddpackage(
  NAME magic_enum
  GIT_TAG v0.9.7
  GITHUB_REPOSITORY Neargye/magic_enum
)

cpmaddpackage(
  NAME BitMagic
  GIT_TAG v.8.0.1
  GITHUB_REPOSITORY tlk00/BitMagic
)

# ---- Compile: Global ----

# c++ header: include/*
file (
  GLOB_RECURSE
  headers
  CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
)

# c++ sources: source/...
file (
  GLOB_RECURSE
  sources
  CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cu"
)

# c++ modules: source/...
file (
  GLOB_RECURSE
  modules
  CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cppm"
  "${CMAKE_CURRENT_SOURCE_DIR}/source/*.ixx"
)

# qt6: find library
#find_package (Qt6 REQUIRED COMPONENTS Core Widgets)

# ---- Compile

add_library (${PROJECT_NAME} ${sources})


target_include_directories ( ${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${BitMagic_SOURCE_DIR}/src>
  $<INSTALL_INTERFACE:include>
)

# MSVC: forcing using CMAKE_CXX_STANDARD
target_compile_options (
  ${PROJECT_NAME} PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive->"
)

# qt6 lib

#target_link_libraries (${PROJECT_NAME} PUBLIC Qt6::Core Qt6::Widgets)


#target_include_directories(${PROJECT_NAME})

target_link_libraries (${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:magic_enum::magic_enum>
)

# ---- Install ----

# install: path
install (
  TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

# install: headers
install (FILES ${headers} DESTINATION include)

# ---- Create an installable target ----

string (TOLOWER ${PROJECT_NAME}/version.h VERSION_HEADER_LOCATION)

packageproject (
  NAME ${PROJECT_NAME}
  VERSION
  ${PROJECT_VERSION}
  NAMESPACE
  ${PROJECT_NAME}
  BINARY_DIR
  ${PROJECT_BINARY_DIR}
  INCLUDE_DIR
  ${PROJECT_SOURCE_DIR}/include
  INCLUDE_DESTINATION
  include/${PROJECT_NAME}-${PROJECT_VERSION}
  VERSION_HEADER
  "${VERSION_HEADER_LOCATION}"
  COMPATIBILITY
  SameMajorVersion
  DEPENDENCIES magic_enum
)
# DEPENDENCIES "fmt 10.2.1"
