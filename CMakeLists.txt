cmake_minimum_required(VERSION 3.28)

project(GameOfLife LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)


# --------------------------------------------------
# SFML
# --------------------------------------------------

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.1.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)

set(SFML_BUILD_NETWORK OFF)

FetchContent_MakeAvailable(SFML)


# --------------------------------------------------
# Dear ImGui
# --------------------------------------------------

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.91.9b
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)

FetchContent_MakeAvailable(imgui)


# --------------------------------------------------
# ImGui-SFML
# --------------------------------------------------

# Tell ImGui-SFML where Dear ImGui was downloaded.
set(IMGUI_DIR ${imgui_SOURCE_DIR})

# We already created SFML using FetchContent,
# so ImGui-SFML should use that target instead of find_package().
set(IMGUI_SFML_FIND_SFML OFF)

# Do not build ImGui-SFML's own examples and tests.
set(IMGUI_SFML_BUILD_EXAMPLES OFF)
set(IMGUI_SFML_BUILD_TESTING OFF)

FetchContent_Declare(
    ImGuiSFML
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
    GIT_TAG v3.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)

FetchContent_MakeAvailable(ImGuiSFML)


# --------------------------------------------------
# GameOfLife
# --------------------------------------------------

file(GLOB_RECURSE GAME_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
)

add_executable(GameOfLife
   ${GAME_SOURCES}
)

target_compile_features(GameOfLife PRIVATE cxx_std_23)

target_link_libraries(
    GameOfLife
    PRIVATE
        SFML::Graphics
        ImGui-SFML::ImGui-SFML
)

if(MSVC)
    target_compile_options(GameOfLife PRIVATE /W4)
endif()

set_property(
    TARGET GameOfLife
    PROPERTY COMPILE_WARNING_AS_ERROR ON
)

set_property(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    PROPERTY VS_STARTUP_PROJECT GameOfLife
)