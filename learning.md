# Learning Log

Running log of concepts learned and bugs debugged while building FluidSim.

---

## [July 2026] — [Orbit Camera Math]

- Implemented Spherical to Cartesian coordinates conversation for an orbit-based camera
- Realization: `y` depends only on `pitch ` (rotation around x-axis). `x`/`z` depend on both `yaw` and `cos(pitch)` (shrinking factor). Less horizontal reach as you tilt towards the vertical.
- Kind of stupid, but Radians vs. degrees: sin/cos defined in terms of radians, degrees gives wrong answer for this program.
- Bug: computed `x/y/z` in `updatePosition()` but never assigned the result to `m_viewMatrix` which meant that it compiled just fine, but rendered nothing at all.
- Bug: `m_up` was never initialized, ruining the math for the matrixes (not sure what exactly yet, haven't learned linear algebra)

## [July 2026] — [Structs, header/cpp splitting]

- My first real use of a `struct`, I know, crazy huh. Used `Vertex` for `position and color`. Learned that structs are public by default and that they don't have a required behavior.
- File-scope rule: declarations are legal at global scope, bare assignment statements are illegal.
- Learned how to organize code using `.h` and `.cpp` files.
- `#pragma once` is a `preprocesser directive`, meaning that C++ keeps an eye out for this specific file and marks it to only be included once during `compile time`.

## [July 2026] — [Mouse input, GLFW callbacks]
- GLFW callbacks must be static, this is standard.
- Cursor-position callback gives an absolute position, not a detla. You have to track `lastX`/ `lastY` manually and compute the change each call
- Bug: gated the whole callback behind a mouse click, which skipped updating `lastX` / `lastY` while not clicking. Causing a teleport/rubber-band-like snap back on the next click. Fix: update tracking state always.

## [August 2026] — [Cube, EBO, depth testing]
- Moved from drawing arrays via `glDrawArray` (quad) to `glDrawElements` + EBO (for cube), index buffer avoids duplicating shared vertices across faces.
- `GL_DEPTH_TEST` was needed when cube rotates, without it, the back faces can draw over front faces and there's no per-pixel depth comparison.
- Vertex colors are interpolated smoothly across a triangle via the barycentric method in OpenGL. The white streak that I saw on my cube wasn't a bug, just OpenGL interpolation stuff.

## [August 2026] - [Refactor Renderer -> free function]
- Moved the `rendering` logic to a file to organize. Instead of making it an object like the other c++ files, I just opted for `free functions`. My reasoning behind this is because the renderer does not need to persist state between frames. Honestly, I didn't know the term for functions not related to classes before this somehow...
- Realization: Instead of defaulting to classes for organization, now, I know understand that sometimes you don't need it.

## [August 2026] - [Git]
- Learned about `git add/commit/push`. `git add .` tells `Git` what you updated. `git commit` saves a snapshot of your project to the hidden local `Git` repository. `git push` uploads that `local` repository into a `remote` repository.
- `Local`-only vs. `remote`.

## [August 2026] - [Jos Stam's, "Stable fluids", "Real-time Fluid Dynamic for Games"]
- Note before learning: This portion of the code will be most difficult becuase I've had no prior experience with fluid dynamics. I will however be excited to learn.
- In compact `Navier-stokes equation` to my understanding: `v` is the `kinematic viscosity` that the fluid has. `Rho` is the `density`. `f` is the `external force`, most likely going to be the gravity and perhaps my input paddle in the future. `Nambla symbol` is the `del`, which is built from partial derivatives. `Del squared` is the `diffusion` of momentum/viscocity, matched with `v`.
- equation 2 is partial derivative of `u` w.s.t `t` = `-(u dot del)times u - 1/rho times del p + v times del^2 times u + f`
- Need add external force, diffuse, project, advect, project -> add forces, diffuse velocity, project, advect velocity, project (to keep stable), inject density, diffuse density, advect density.
- `Nambla dot u = 0` is similar to all the other conservation of energy in other fields, but for fluid dynamics it's the conservation of volume for incompressible fluids.
- Boundary conditions: fluid is defined on an n-dimension torus, fluids wraps around. The normal components of the velocity field is zero at boundary, meaning it doesn't move through the boundary.
- There is a `velocity` and `pressure` field.
- Jos Stam follows the `Helmholtz-Hodge Decomposition` mathematic result, "any vector field `w` can uniquely be decomposed into form: `w = u + delq` ... `u` has zero divergence ... `q` is a scalar field". Del of a vector field is gradient of that field for future me to remember. Vector field is sum of mass conserving field & gradient field. Operator `P` projects any vector field `w` onto div free part "`u = Pw`" Implicitly defined equation (`Poisson`) = `del dot w = del^2 times q`, `Neumann` boundary condition is partial derivative of `q w.s.t n = 0 on partial derivative D` (n is wall coordinate). The solution to the `Poisson` equation is then "used to compute the project `u`: `u = Pw = w - delq`.
- The projection operator we found in the period bullet point is then used on both sides of Eq. 2 to "obtain a single equation for the velocity: `partial der u w.s.t t = P times (-(u dot del) times u + v times del^2 times u + f)`
- note for the velocity equation above: `Pu = u`, `Pdelp = 0`
- The `Poisson` equation here is used to incompressibility constraint & calculate pressure.

## [August 2026] - [Stack vs. heap]
- `Stack and heap` is both memory related. Grid array will be heap-allocated.
- Function call stack is a memory structure used by the program to keep track of ongoing function calls and to make sure that they go in the order that they're assigned, `LIFO`.
- How the `Stack allocation` works: `temporary` memory is allocated for the function being called, in which the memory is in `contiguous blocks` which is the `call stack`. Size of memory is `known` before the execution, function called -> local variables allocated on stack. Function finishs execution -> memory `deallocated`.
- Features of Stack allocation: memory is only available when the function is being called. Automatic dealloc. Stack memory full? `Segmentation fault`. Data can only be accessed by `owner thread`, making this `safer` than heap memory.
- Faster than heap allocation because of the automatic memory management.
- `Heap allocation` is the large pool of memory available for `dynamic alloc`. 
- Create object -> stored in `heap memory`. References to the object is stored in `stack memory`.
- Heap is accesible by `multiple` threads.
- Heap does not have automatic dealloc.
- Heap memory `persists`.

## [August 2026] - [Staggered Grid aka Arakawa C-grid]
- Why pick `Arakawa C-grid` over `Collocated` grids? Because it allows oscillating pressure fields to zero out, `P_i+1 - P_i`.
- Divides a physical space into fixed grids of cells rather than tracking every individual particle like sph approach.
- The physical variables are all 'staggered' across the box at different locations.
- `Tracers` and `scalars` are placed at the cell's center (p).
- `Zonal` velocity (u, v) is placed east and west cell edge.
- `Meridional` Velocity at north `(v[j+1, i])` and south `(v[j, i])` cell edges.
- `N_x` cross `N_y` tracer cells: Tracer array is `(N_y, N_x)`.
- `u` velocity array: `(N_y, N_x +1)` becuse of one extra edge on outer boundary.
- `v` velocity array: `(N_y +1, N_x)` likewise

## [August 2026] - [Flattening 2D arrays]
- `i, j` are grid coordinates, differ depending on what we're talking about in this program.
- `width` depends on how much elements in one row of specific array.
- General row-major flattening is taking a 2D array -> 1D array. `(i, j)` -> `single` int output
- Find where `(i, j)` lives first, then attempt to reduce to `1d` by skipping `j` rows, `(j * row width)`, then move `i` over.
- Recap: `index` = `(j * width) + i`,
- Application for project: density flattening formula (cell center) = (j * nx) + i, xVelocity Flattening (cell face) = j * (nx + 1) + i, yVelocity Flattening (cell face) = (j * width) + i