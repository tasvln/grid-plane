# Infinite Grid Plane

A simple infinite grid plane built using **SDL3**, **OpenGL**, and **GLEW**, inspired by the viewport grids in **Unity**, **Unreal Engine**, and **Blender**.

## Features
- Infinite procedural grid rendering
- FPS-style camera movement (WIP)
- Orbit camera movement (WIP)
- Basic FPS counter
- Lightweight and minimal dependencies

## Demo
*(GIF or screenshot)*

## Getting Started

### Prerequisites
- SDL3
- OpenGL
- GLEW
- C++ Compiler (g++, clang++, etc.)

### Project Structure
```
/lib           -> Source (.cpp) and header (.h) files
build.sh       -> Build script
main.cpp       -> Entry point
main.h         -> Main header
.gitignore     -> Git ignore rules
CMakeLists.txt -> CMake build configuration
README.md      -> Project documentation
```

## Controls
- **WASD** or **Arrow Keys** — Move camera (FPS movement)
- **Right Mouse + Drag** — Orbit around point (WIP)
- **Mouse Scroll** — Zoom in/out

### Build Instructions
```bash
git clone https://github.com/yourusername/infinite-grid-plane.git
cd infinite-grid-plane
./build.sh
```
> Ensure SDL3, OpenGL, and GLEW are properly installed and linked.
