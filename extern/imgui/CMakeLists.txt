project(imgui
    VERSION         1.85
    DESCRIPTION     "Bloat-free Graphical User interface for C++ with minimal dependencies"
    HOMEPAGE_URL    "https://github.com/ocornut/imgui"
    LANGUAGES       CXX
)

add_library(imgui OBJECT
    src/imgui.cpp
    src/imgui_demo.cpp
    src/imgui_draw.cpp
    src/imgui_impl_glfw.cpp
    src/imgui_impl_opengl3.cpp
    src/imgui_tables.cpp
    src/imgui_widgets.cpp
)

add_dependencies(imgui glfw glad)
target_link_libraries(imgui
    PRIVATE glfw
    PRIVATE glad
)
# public and internal headers
target_include_directories(imgui
    PUBLIC include
    PRIVATE src
)
# Use c++17
target_compile_features(imgui PRIVATE cxx_std_17)
target_compile_definitions(imgui
  PRIVATE IMGUI_IMPL_OPENGL_LOADER_CUSTOM
)
