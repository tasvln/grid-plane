# Infinite Grid Plane

A simple infinite grid plane built using **SDL3**, **OpenGL**, and **GLEW**, inspired by the viewport grids in **Unity**, **Unreal Engine**, and **Blender**.

## Features
- Infinite procedural grid rendering
- FPS-style camera movement
- Orbit camera movement
- Basic FPS counter
- Lightweight and minimal dependencies

## Demo

<img width="1200" alt="Screenshot 2025-05-14 at 1 46 56 PM" src="https://github.com/user-attachments/assets/e0d565c6-1ea4-4a2e-b219-10d6a3150984" />

<img width="1200" alt="Screenshot 2025-05-14 at 1 55 16 PM" src="https://github.com/user-attachments/assets/624e6254-9dfc-40cd-a0a7-17d9f7dfa322" />

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
- **WASD** — Move camera (FPS movement)
- **Right Mouse + Drag** — Orbit around point
- **Mouse Scroll** — Zoom in/out

### Build Instructions
```bash
git clone https://github.com/tasvln/infinite-grid-plane.git
cd infinite-grid-plane
./build.sh
```
> Ensure SDL3, OpenGL, and GLEW are properly installed and linked.
