# FluidSim

- Real-time 3D fluid simulation built in C++ and OpenGL, working towards a ray-marched volumetric render driven by a physical joystick/IMU input.

**Status:** early development stage, just OpenGL fundamentals (camera, cube rendering), fluid files are in progress. Not remotely close to a working fluid simulation yet.

## Goal 
- Build a staggered grid-based fluid solver following Jos Stam's `Stable Fluids` method, rendered via ray-marching through a 3D density texture, with real-time force input from a joystick (eventually ESP32 + MPU6050 IMU)

## Tech Stack
- C++17
- OpenGL 3.3 Core Profile
- GLFW (windowing/input)
- GLAD (OpenGL function loading)
- GLM (math)
- MSYS2 g++ (no CMake yet)
- Git

## Project structure

```
fluidSim/
├── src/
│   ├── main.cpp
│   ├── Window.h / Window.cpp
│   ├── Shader.h / Shader.cpp
│   ├── Camera.h / Camera.cpp
│   ├── Vertex.h
│   ├── Fluid.h / Fluid.cpp        (working on currently)
│   ├── Input.h / Input.cpp
│   ├── Mesh.h / Mesh.cpp
│   └── Renderer.h / Renderer.cpp 
├── output/                         (build output, gitignored)
└── .vscode/                        (build tasks)
```

Sibling folder `../Common/` holds GLFW/GLAD/GLM headers and libs.

## Building
VS Code task (`Ctrl + Shift + B`) runs MSYS2 g++ . No CMake yet.

## Progress / roadmap
- [x] Window creation, RAII wrapped
- [x] Shader class (compile/link, error checking)
- [x] Orbit Camera (spherical coordinates, mouse look + scroll zoom)
- [x] Quad rendering
- [x] Cube rendering (EBO, depth test, GLM transforms)
- [ ] Fluid solver: advection
- [ ] Fluid solver: diffusion
- [ ] Fluid solver: pressure projection
- [ ] 3D density texture + ray marching
- [ ] Joystick input → force injection
- [ ] ESP32 + MPU6050 IMU input (hardware track, separate)

## References

This project is following established techniques from graphics research. Implementation is original, or I hope it is, and underlying methods are:
- Stam, Jos. "`Stable Fluids.`" 1999.
- Stam, Jos. "`Real-time Fluid Dynamic for Games`"
- Harhris, Mark "`Fast Fluid Dynamics Simulation on the GPU.`" *GPU Gems*, Chapter 38

## Learning Approach

This project is built to understand concepts, not just for producing/refining code. Implementation is hand-written, AI is used for concept explanation, debugging guidance, and code review, no code generation or copy + paste.

