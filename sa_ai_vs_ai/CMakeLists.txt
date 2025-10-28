cmake_minimum_required (VERSION 3.14...3.22)

project (sa_ai_vs_ai LANGUAGES CXX)

# cpm: init
include (../cmake/CPM.cmake)

# ---- Package ----

# Using dots_and_boxes with installed version at ./install/dots_and_boxes or using source code
option (dots_and_boxes_INSTALLED_VERSION "Use installed package or use source code" OFF)

if (dots_and_boxes_INSTALLED_VERSION)
  # "find_package" for using compiled package
  # CONFIG for finding helloConfig.cmake Not finding FindHello.cmake
  # REQUIRED for jump error when not found
  # list(APPEND CMAKE_PREFIX_PATH "/PATH") for simple path
  find_package (
    #hello CONFIG REQUIRED PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../install/project
    dots_and_boxes CONFIG REQUIRED
  )
else ()
  # "CPMAddPackage" for adding source code
  CPMAddPackage(
    NAME dots_and_boxes
    SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/../dots_and_boxes
  )
endif ()

# package: cxxopts
cpmaddpackage (
  GITHUB_REPOSITORY
  jarro2783/cxxopts
  VERSION
  3.0.0
  OPTIONS
  "CXXOPTS_BUILD_EXAMPLES NO"
  "CXXOPTS_BUILD_TESTS NO"
  "CXXOPTS_ENABLE_INSTALL YES"
)

# ---- Compile & Install ----

file (
  GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp
)

add_executable (${PROJECT_NAME} ${sources})

set_target_properties (${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)

target_link_libraries (${PROJECT_NAME} dots_and_boxes::dots_and_boxes cxxopts)

install (
  TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)
