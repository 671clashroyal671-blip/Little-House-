cmake_minimum_required(VERSION 3.25)
project(LittleHousEngine VERSION 0.1.0 DESCRIPTION "Little Hous Engine" LANGUAGES C CXX)

option(LHE_BUILD_EDITOR "Build the desktop editor" ON)
option(LHE_FETCH_DEPENDENCIES "Fetch pinned dependencies with CMake FetchContent" ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

if(LHE_BUILD_EDITOR)
    if(NOT LHE_FETCH_DEPENDENCIES)
        message(FATAL_ERROR "The initial editor shell currently uses FetchContent for SDL3 and Dear ImGui. Configure with LHE_FETCH_DEPENDENCIES=ON.")
    endif()

    set(SDL_SHARED OFF CACHE BOOL "" FORCE)
    set(SDL_STATIC ON CACHE BOOL "" FORCE)
    set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
    set(SDL_TESTS OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-3.2.10
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(SDL3)

    FetchContent_Declare(imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.91.9b-docking
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(imgui)

    find_package(OpenGL REQUIRED)

    add_library(lhe_imgui STATIC
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    )
    target_include_directories(lhe_imgui PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)
    target_link_libraries(lhe_imgui PUBLIC SDL3::SDL3)

    add_executable(little_hous_editor editor/src/main.cpp)
    target_link_libraries(little_hous_editor PRIVATE SDL3::SDL3 OpenGL::GL lhe_imgui)
    target_compile_definitions(little_hous_editor PRIVATE LHE_VERSION="${PROJECT_VERSION}")

    if(MSVC)
        target_compile_options(little_hous_editor PRIVATE /W4 /permissive-)
    else()
        target_compile_options(little_hous_editor PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endif()
