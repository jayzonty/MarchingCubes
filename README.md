# MarchingCubes

[Japanese (日本語版)](./README.ja-JP.md)

## Overview
This project is a personal attempt at implementing the Marching Cubes algorithm in C++11 and OpenGL to generate smooth, continuous procedural 3D terrain in real-time.

![Cover GIF](./docs/marching-cubes.gif)

## Features
1. Marching Cubes Mesh Generation
   - Extracts smooth 3D polygonal meshes from arbitrary scalar density fields.
   - Uses modified lookup tables based on Eric Lengyel's [Transvoxel Algorithm](https://transvoxel.org/) (16 equivalence classes for regular cells).
   - Linearly interpolates vertex positions along cube edges to produce smooth organic slopes instead of blocky voxels.
2. Multi-threaded Chunk Generation
   - Worker thread pool (4 background threads) handles chunk voxel triangulation asynchronously without stalling the main render thread.
   - Thread-safe job queue assigns pending chunk generation tasks.
3. Procedural 3D Terrain
   - Generates terrain with overhangs, caves, and hills using multiple octaves of simplex noise.
   - Combines 9 layered noise passes with varying frequencies and amplitudes over a height-based density gradient.
   - Includes analytical signed distance functions for testing primitives like spheres and boxes.
4. Dynamic Chunk Streaming
   - Partitions world space into discrete 3D chunks (8x8x8).
   - Streams chunks around the player's position based on a configurable render distance (default: 8x8x8).
   - Loads newly entered chunks and unloads distant ones outside render bounds to keep memory usage in check.
5. Tessellated Water Plane
   - Renders a continuous water plane using OpenGL hardware tessellation (Tessellation Control & Evaluation Shaders).
   - Includes alpha blending and specular highlights.
6. Camera & Debug UI
   - Free-fly first-person camera with keyboard (WASD) navigation and mouse look.
   - In-game text overlay via FreeType showing real-time camera position, current chunk coordinates, pending chunk jobs, and completed chunk counts.

## Controls
| Input | Action |
| :--- | :--- |
| **W / S** | Move Forward / Backward |
| **A / D** | Strafe Left / Right |
| **Mouse Move** | Look around (Yaw / Pitch) |
| **Escape** | Exit Application |

## Tech Stack & Dependencies
- **Language**: C++11
- **Graphics API**: OpenGL 4.1
- **Windowing & Input**: GLFW 3
- **OpenGL Loader**: [GLAD](https://glad.dav1d.de/)
- **Math Library**: [GLM](https://github.com/g-truc/glm) (OpenGL Mathematics)
- **Noise Generation**: [FastNoiseLite](https://github.com/Auburn/FastNoiseLite)
- **Font Rendering**: [FreeType](https://freetype.org/)
- **Build System**: CMake (3.10+)

## Project Structure

```text
|-- include/
|   |-- Engine/
|   |   |-- Font/                 # FreeType font wrapper
|   |   |-- Geometry/
|   |   |   |-- BoundingVolumes/  # Axis-Aligned Bounding Box (AABB)
|   |   |-- Graphics/             # Camera, Renderer, ShaderProgram, Mesh
|   |   |-- UI/                   # Screen text rendering
|   |   |-- Application.hpp       # Core engine application loop
|   |   |-- Input.hpp             # GLFW keyboard and mouse input handling
|   |   |-- ResourceManager.hpp   # Shader caching and lifetime management
|   |   |-- SceneBase.hpp         # Abstract scene interface
|   |   |-- Time.hpp              # Delta time and frame time tracking
|   |-- Chunk.hpp                 # Chunk data structure and bounds
|   |-- Cube.hpp                  # Analytical cube signed distance function
|   |-- Edge.hpp                  # 2D line segment definition
|   |-- MainScene.hpp             # Primary 3D marching cubes terrain scene
|   |-- MarchingCubes.hpp         # Transvoxel lookup tables & isosurface extraction
|   |-- MarchingCubes2DScene.hpp  # 2D marching squares demonstration scene
|   |-- Sphere.hpp                # Analytical sphere signed distance function
|   |-- Terrain.hpp               # 3D multi-octave noise density field
|   |-- Triangle.hpp              # 3D triangle and normal calculation
|-- src/
|   |-- Engine/                   # Custom engine implementations
|   |-- Main.cpp                  # Program entry point
|   |-- MainScene.cpp             # Chunk streaming, thread pool, and rendering
|   |-- MarchingCubes.cpp         # Voxel edge interpolation and triangulation
|   |-- MarchingCubes2DScene.cpp  # 2D scalar contouring implementation
|   |-- SandboxScene.cpp          # UI sandbox test scene
|-- resources/
|   |-- fonts/                    # TrueType font files (SourceCodePro)
|   |-- shaders/                  # GLSL shaders (Terrain, Water Tessellation, Color, Font)
|-- deps/                         # External dependencies (GLAD, GLM, FastNoiseLite)
|-- CMakeLists.txt                # CMake build configuration
|-- compile.sh                    # Build script
```

## Building and Running

### Prerequisites
- CMake 3.10 or higher
- C++11 compliant compiler (GCC, Clang, or MSVC)
- OpenGL, GLFW 3, and FreeType libraries installed on your system

### Build Instructions

#### On Linux / macOS / WSL:
```bash
./compile.sh
cd build
./MarchingCubes
```

#### On Windows (CMake & Visual Studio / Ninja):
```bash
cmake -B build -S .
cmake --build build --config Release
cd build/Release
./MarchingCubes.exe
```