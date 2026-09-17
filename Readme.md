
# 128x128 2d vector field standard spectrometry colorscheme

https://github.com/user-attachments/assets/9613bf6f-e0ce-4615-a5d5-ebb9a6e48352





# FluidSim

- Real-time 3D fluid simulation built in C++ and OpenGL, working towards a ray-marched volumetric render driven by a physical joystick/IMU input.

**Status:** Fluid simulation vector arrows working. Fluid simulation only up to diffusion as of now 9/11/26.

## Goal
- Build a staggered grid-based fluid solver following Jos Stam's `Stable Fluids` method, rendered via ray-marching through a 3D density texture, with real-time force input from a joystick (eventually ESP32 + MPU6050 IMU)

## Tech Stack
- C++17
- OpenGL 3.3 Core Profile
- GLFW (windowing/input)
- GLAD (OpenGL function loading)
- GLM (math)
- MSYS2 g++
- Git
- Cmake

## Project structure

```
fluidSim/
├── build/ (CMake, gitignored)
├── include/
│   ├── Window.h
│   ├── Shader.h
│   ├── Camera.h
│   ├── Vertex.h
│   ├── Input.h
│   ├── Mesh.h
│   ├── Renderer.h
│   ├── meshShape.h
│   ├── Fluid.h
├── src/
│   ├── main.cpp
│   ├── Window.cpp
│   ├── Shader.cpp
│   ├── Camera.cpp
│   ├── Fluid.cpp        (working on currently)
│   ├── Input.cpp
│   ├── Mesh.cpp
│   └── Renderer.cpp
├── output/                         (build output, gitignored)
└── .vscode/                        (build tasks, gitignored)
└── CMakeLists.txt
```

## Building
CMake

## Progress / roadmap
- [x] Window creation, RAII wrapped
- [x] Shader class (compile/link, error checking)
- [x] Orbit Camera (spherical coordinates, mouse look + scroll zoom)
- [x] Quad rendering
- [x] Cube rendering (EBO, depth test, GLM transforms)
- [x] Fluid solver: advection
- [x] Fluid solver: diffusion
- [x] Fluid solver: pressure projection
- [ ] 3D density texture + ray marching
- [ ] Joystick input → force injection
- [ ] ESP32 + MPU6050 IMU input (hardware track, separate)

## References
- `https://learnopengl.com/`
This project is following established techniques from graphics research. Implementation is original, or I hope it is, and underlying methods are:
- Stam, Jos. "`Stable Fluids.`" 1999.
- Stam, Jos. "`Real-time Fluid Dynamic for Games`"
- Harhris, Mark "`Fast Fluid Dynamics Simulation on the GPU.`" *GPU Gems*, Chapter 38

## Learning Approach

This project is built to understand concepts, not just for producing/refining code. Implementation is hand-written, AI is used for concept explanation, debugging guidance, and code review, no code generation or copy + paste.

