# Learning Log

Running log of concepts learned and bugs debugged while building FluidSim.

---

## [July 26] — [Orbit Camera Math]

- Implemented Spherical to Cartesian coordinates conversation for an orbit-based camera
- Realization: `y` depends only on `pitch ` (rotation around x-axis). `x`/`z` depend on both `yaw` and `cos(pitch)` (shrinking factor). Less horizontal reach as you tilt towards the vertical.
- Kind of stupid, but Radians vs. degrees: sin/cos defined in terms of radians, degrees gives wrong answer for this program.
- Bug: computed `x/y/z` in `updatePosition()` but never assigned the result to `m_viewMatrix` which meant that it compiled just fine, but rendered nothing at all.
- Bug: `m_up` was never initialized, ruining the math for the matrixes (not sure what exactly yet, haven't learned linear algebra)

## [July 26] — [Structs, header/cpp splitting]

- First real use of a `struct`, I know, crazy huh. Used `Vertex` for `position and color`. Learned that structs are public by default and that they don't have a required behavior.
- File-scope rule: declarations are legal at global scope, bare assignment statements are illegal.

## [July 26] — [Mouse input, GLFW callbacks]
- GLFW callbacks must be static, this is standard.
- Cursor-position callback gives an absolute position, not a detla. You have to track `lastX`/ `lastY` manually and compute the change each call
- Bug: gated the whole callback behind a mouse click, which skipped updating `lastX` / `lastY` while not clicking. Causing a teleport/rubber-band-like snap back on the next click. Fix: update tracking state always.

## [August 26] — [Cube, EBO, depth testing]
- Moved from drawing arrays via `glDrawArray` (quad) to `glDrawElements` + EBO (for cube), index buffer avoids duplicating shared vertices across faces.
- `GL_DEPTH_TEST` was needed when cube rotates, without it, the back faces can draw over front faces adn there's no per-pixel depth comparison.
- Vertex colors are interpolated smoothly across a triangle via the barycentric method in OpenGL. The white streak that I saw on my cube wasn't a bug, just OpenGL interpolation stuff.