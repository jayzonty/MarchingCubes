cmake_minimum_required(VERSION 3.10)

# Project name
project(MarchingCubes)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Freetype REQUIRED)

# C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Generate compile_commands.json for YouCompleteMe (YCM)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Specify include directories
include_directories(deps/glad/include deps/glm/include ${FREETYPE_INCLUDE_DIRS} include)

# Set SOURCES to contain all the source files
set(SOURCES
    # Deps
    deps/glad/src/glad.c

    # Engine
    src/Engine/Graphics/Mesh.cpp
    src/Engine/Graphics/MeshBuilder.cpp
    src/Engine/Graphics/OrbitCamera.cpp
    src/Engine/Graphics/Renderer.cpp
    src/Engine/Graphics/ShaderProgram.cpp

    src/Engine/Font/Font.cpp

    src/Engine/UI/Text.cpp

    src/Engine/Application.cpp
    src/Engine/Input.cpp
    src/Engine/ResourceManager.cpp
    src/Engine/Time.cpp

    src/MarchingCubes2DScene.cpp
    src/MarchingCubes3DScene.cpp
    src/Main.cpp
)

# Executable
add_executable(MarchingCubes ${SOURCES})

target_compile_options(MarchingCubes PUBLIC -Wall)

# Link libraries
target_link_libraries(MarchingCubes ${OPENGL_gl_LIBRARY} ${FREETYPE_LIBRARIES} glfw ${CMAKE_DL_LIBS})

# Post-build copy command
add_custom_command(TARGET MarchingCubes POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources/ $<TARGET_FILE_DIR:MarchingCubes>/resources/
)
