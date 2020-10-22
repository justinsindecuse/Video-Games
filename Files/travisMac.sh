#!/bin/bash
# CMake into build directory
echo -e "***Running CMake..."
mkdir build
cd build
cmake -G Xcode ..
# Compile labs based on build file
while read p; do
  echo -e "***Building $p..."
  cmake --build . --target $p --config Debug -- -quiet || exit 1
done < ../BuildTravis.txt
