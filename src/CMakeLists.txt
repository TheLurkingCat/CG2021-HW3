project(HW3 C CXX)

set(HW3_SOURCE
  ${HW3_SOURCE_DIR}/buffer/buffer.cpp
  ${HW3_SOURCE_DIR}/buffer/vertexarray.cpp
  ${HW3_SOURCE_DIR}/camera/camera.cpp
  ${HW3_SOURCE_DIR}/camera/quat_camera.cpp
  ${HW3_SOURCE_DIR}/context_manager.cpp
  ${HW3_SOURCE_DIR}/shader/program.cpp
  ${HW3_SOURCE_DIR}/shader/shader.cpp
  ${HW3_SOURCE_DIR}/shape/cube.cpp
  ${HW3_SOURCE_DIR}/shape/plane.cpp
  ${HW3_SOURCE_DIR}/shape/sphere.cpp
  ${HW3_SOURCE_DIR}/texture/cubemap.cpp
  ${HW3_SOURCE_DIR}/texture/framebuffertexture.cpp
  ${HW3_SOURCE_DIR}/texture/texture.cpp
  ${HW3_SOURCE_DIR}/texture/texture2d.cpp
  ${HW3_SOURCE_DIR}/main.cpp
)

set(HW3_INCLUDE_DIR ${HW3_SOURCE_DIR}/../include)

set(HW3_HEADER
  ${HW3_INCLUDE_DIR}/buffer/buffer.h
  ${HW3_INCLUDE_DIR}/buffer/vertexarray.h
  ${HW3_INCLUDE_DIR}/camera/camera.h
  ${HW3_INCLUDE_DIR}/camera/quat_camera.h
  ${HW3_INCLUDE_DIR}/context_manager.h
  ${HW3_INCLUDE_DIR}/graphics.h
  ${HW3_INCLUDE_DIR}/shader/program.h
  ${HW3_INCLUDE_DIR}/shader/shader.h
  ${HW3_INCLUDE_DIR}/shape/cube.h
  ${HW3_INCLUDE_DIR}/shape/plane.h
  ${HW3_INCLUDE_DIR}/shape/shape.h
  ${HW3_INCLUDE_DIR}/shape/sphere.h
  ${HW3_INCLUDE_DIR}/texture/cubemap.h
  ${HW3_INCLUDE_DIR}/texture/framebuffertexture.h
  ${HW3_INCLUDE_DIR}/texture/texture.h
  ${HW3_INCLUDE_DIR}/texture/texture2d.h
  ${HW3_INCLUDE_DIR}/utils.h

)
add_executable(HW3 ${HW3_SOURCE} ${HW3_HEADER})
target_include_directories(HW3 PRIVATE ${HW3_INCLUDE_DIR})

add_dependencies(HW3 glad glfw glm stb imgui)
# Can include glfw and glad in arbitrary order
target_compile_definitions(HW3 PRIVATE GLFW_INCLUDE_NONE)
# More warnings
if (NOT MSVC)
  target_compile_options(HW3
    PRIVATE "-Wall"
    PRIVATE "-Wextra"
    PRIVATE "-Wpedantic"
  )
endif()
# Prefer std c++20, at least need c++17 to compile
set_target_properties(HW3 PROPERTIES
  CXX_STANDARD 20
  CXX_EXTENSIONS OFF
)

target_link_libraries(HW3
  PRIVATE glad
  PRIVATE glfw
  PRIVATE stb
  PRIVATE imgui
)

if (TARGET glm::glm_shared)
  target_link_libraries(HW3 PRIVATE glm::glm_shared)
elseif(TARGET glm::glm_static)
  target_link_libraries(HW3 PRIVATE glm::glm_static)
else()
  target_link_libraries(HW3 PRIVATE glm::glm)
endif()
