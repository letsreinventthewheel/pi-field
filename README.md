# Pi Field

An animated shader-based spike field built in **C** with **Raylib**.



https://github.com/user-attachments/assets/517c6219-6a56-4234-8e68-0ade0fa8b172



The scene renders a glowing π symbol by drawing a single tiny spike mesh many times with GPU instancing. The mesh itself is simple: two vertical quads intersecting in the middle. The visual complexity comes from the shaders, which animate spike heights, reveal the π shape from a mask texture, and color the field with a height-based gradient.

## Demo concept

**One mesh. 65,536 instances. Mostly shader magic.**

The project demonstrates how a minimal piece of geometry can become a dense animated visual effect when combined with instanced rendering and shader-driven animation.

## Features

- Written in C with raylib
- Single spike mesh built from two intersecting vertical quads
- 256 × 256 instanced spike grid
- 65,536 spike instances rendered with `DrawMeshInstanced`
- Shader-driven spike height animation
- π symbol generated from a mask texture
- Height-based color gradient in the fragment shader
- Animated procedural background shader
- Pause/resume animation with the Space key

## How it works

The scene is made from one reusable spike mesh. Each spike is placed into a regular grid using an instance transform matrix.

At render time, the vertex shader uses `gl_InstanceID` to determine the current spike position in the grid. It samples a mask texture to decide whether that spike belongs to the π symbol or the surrounding field.

Spikes inside the π mask are raised higher than the rest of the field. Both the base field and the π symbol receive their own animated wobble, giving the scene a pulsing motion.

The fragment shader receives the final spike height and maps it to a color gradient, producing the purple, magenta, orange, and yellow look.

## Requirements

- C compiler with C23 support
- Raylib
- OpenGL 3.3 compatible environment

## Main implementation details

### Spike mesh

The spike mesh contains four triangles: two triangles for one vertical quad and two triangles for another vertical quad rotated 90 degrees around the Y axis.

This creates a lightweight cross-shaped spike that reads well from multiple camera angles while keeping the geometry very small.

### Instanced grid

The field uses a 256 × 256 grid. Each instance receives a transform that scales the spike to fit one grid cell and translates it into position.

### Mask-driven π symbol

The vertex shader samples `assets/mask.png`. The alpha value of the mask determines whether a given spike should behave like part of the π symbol or part of the lower background field.

### Shader animation

The vertex shader computes animated height values using deterministic hash-based randomness and sine waves. This gives each spike slightly different motion while keeping the whole field coherent.

### Height-based coloring

The fragment shader colors each spike based on its final height. Low spikes stay deep purple, while taller spikes shift through magenta and orange into pale yellow.

## Video

Full development process captured on video: [This Entire Effect Is One Instanced Mesh](https://www.youtube.com/watch?v=nFsrrqEYtPk)
