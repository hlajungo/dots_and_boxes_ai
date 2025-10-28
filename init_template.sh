#!/bin/bash

script_path="$(realpath ${BASH_SOURCE[0]})"

# get CPM
if [[ ! -f "$(dirname $script_path)/cmake/CPM.cmake" ]]; then
  echo "$(dirname $script_path)/cmake/CPM.cmake"
  wget -O cmake/CPM.cmake https://github.com/cpm-cmake/CPM.cmake/releases/latest/download/get_cpm.cmake
else
  echo "Found cmake/CPM.cmake, skip download CPM"
fi

