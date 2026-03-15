cmake_minimum_required(VERSION 4.1)
project(cs2_radar_crossover)

set(CMAKE_CXX_STANDARD 20)

find_package(glfw3 CONFIG REQUIRED)

add_executable(cs2_radar_crossover main.cpp
        cocoa_bridge.mm
        imgui/imconfig.h
        imgui/imgui.cpp
        imgui/imgui.h
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_internal.h
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        imgui/imstb_rectpack.h
        imgui/imstb_textedit.h
        imgui/imstb_truetype.h
        imgui/backends/imgui_impl_opengl3.cpp
        imgui/backends/imgui_impl_opengl3_loader.h
        imgui/backends/imgui_impl_opengl3.h
        glad/include/glad/glad.h
        glad/include/KHR/khrplatform.h
        glad/src/glad.c
        imgui/backends/imgui_impl_glfw.cpp
        src/memory/memory.cpp
        src/memory/memory.hpp
        src/core.cpp
        src/core.hpp
        src/offsets.hpp
        src/common.hpp
        src/maps/maps.cpp
        src/maps/maps.hpp
        src/game/game.cpp
        src/game/game.hpp
        imgui/stb_image.hpp)

target_include_directories(cs2_radar_crossover PUBLIC
        src
        imgui
        imgui/backends
        glad
        glad/include)

target_link_libraries(cs2_radar_crossover glfw "-framework CoreFoundation" "-framework OpenGL"
        "-framework Cocoa"
        glfw)