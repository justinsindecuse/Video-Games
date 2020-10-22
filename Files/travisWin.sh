#!/bin/bash
# CMake into build directory
echo -e "\e[36m***Running CMake..."
mkdir build
cd build
cmake -G "Visual Studio 15 2017" ..
# Compile labs based on build file
while read p; do
  echo -e "\e[36m***Building" $p
  cmake --build . --target $p --config Debug -- -v:q -clp:ForceConsoleColor || exit 1
done < ../BuildTravis.txt
