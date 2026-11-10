cmake_minimum_required(VERSION 3.16)

project(fourier_series LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SDL2_ROOT "C:/Vishrut/Paths/SDL2")

# ---------------------------------------------------------------- Dear ImGui
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/external/imgui)

add_library(imgui STATIC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdl2.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdlrenderer2.cpp
)

target_include_directories(imgui PUBLIC
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    "${SDL2_ROOT}/include"
    "${SDL2_ROOT}/include/SDL2" # ImGui backends use #include <SDL.h>
)

target_compile_definitions(imgui PUBLIC SDL_MAIN_HANDLED)

target_link_directories(imgui PUBLIC "${SDL2_ROOT}/lib")
target_link_libraries(imgui PUBLIC SDL2)

# ---------------------------------------------------------------- App
add_executable(main
    src/main.cpp
    src/Application.cpp
    src/Window.cpp
    src/Graph.cpp
    src/WaveComposer.cpp
)

target_include_directories(main PRIVATE
    include
)

target_link_libraries(main PRIVATE
    imgui
)
