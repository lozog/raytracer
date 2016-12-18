# raytracer
A basic ray tracer with Phong Illumination.
I originally made this for CS488, Intro to Computer Graphics, at the University of Waterloo.

![Alt Text](screenshot.png)

---

## Build

```
$ cd raytracer
raytracer$ premake4 gmake
raytracer$ make
raytracer$ cd src
raytracer/src$ premake4 gmake
raytracer/src$ make
```

---

## Dependencies
* OpenGL 3.2+
* GLFW
    * http://www.glfw.org/
* Lua
    * http://www.lua.org/
* Premake4
    * https://github.com/premake/premake-4.x/wiki
    * http://premake.github.io/download.html
* GLM
    * http://glm.g-truc.net/0.9.7/index.html
* ImGui
    * https://github.com/ocornut/imgui

This build is only tested on Linux, but should theoretically also work for OSX.