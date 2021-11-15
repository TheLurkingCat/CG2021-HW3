#!/bin/bash
if [[ !(-d src) ]]; then
  cd ..
fi

if [[ !(-d src) ]]; then
  echo "Please run this script in project root folder."
  exit 1
fi

git apply --ignore-whitespace script/homework.patch \
&& zip -qr9 HW3.zip \
extern/glad \
extern/glfw/CMake \
extern/glfw/deps/mingw \
extern/glfw/deps/vs2008 \
extern/glfw/include \
extern/glfw/src \
extern/glfw/LICENSE.md \
extern/glfw/CMakeLists.txt \
extern/glm/cmake \
extern/glm/glm \
extern/glm/CMakeLists.txt \
extern/glm/copying.txt \
extern/stb \
extern/imgui \
script/pack.ps1 \
script/pack.sh \
cmake \
vs2019 \
include \
assets \
src \
.clang-format \
CMakeLists.txt \
README.md \
&& git apply -R --ignore-whitespace script/homework.patch
