# Final Project

## 1. Introduction

You've made it to the end of another semester! To strut your newly-acquired
experience, you'll be tasked with developing a showcase application
incorporating:

* 3D model transformations (translation, rotation and scaling)
* 3D viewing pipeline:
  - Model-to-world transformation
  - World-to-camera transformation
  - Camera-to-**perspective** projection
* Texture mapping, either of the following:
  - Spherical mapping (see our slides)
  - Custom affine mapping (i.e. lab 6)
* Phong-based shading

As in all the assignments this term, this framework
uses OpenGL, GLFW, and GLEW; if you have completed those assignments
successfully on your own system,
you have working versions of these libraries
and you should be ready to go on this assignment.

In this project, you are free to design and model your own scene! You will
want to shape the scope of your work around the above features, and by
grading matrix below (see _6. Grading_).

You have been provided a boilerplate GLEW application; the program accepts keyboard input as follows:

<blockquote>
<table border="1" cellpadding="2">
<tr>
  <td align="center">a</td>
  <td>Start animation</td>
</tr>
<tr>
  <td align="center">s</td>
  <td>Stop animation</td>
</tr>
<tr>
  <td align="center">q, ESC</td>
  <td>Exit from the program</td>
</tr>
</table>
</blockquote>

You are not required to implement animation; however the application state
will expose functionality allowing you to do so. Therefor, without any
further development, starting and stopping animation will simply update
the `animating` global variable, located in `finalMain.cpp` or
`finalMain.c`. This state is available for an extra credit opportunity, listed below.

**Before moving forward, it is recommended that you create another Git branch for your work.** To create a new Git branch for this repository, run `git branch <your-new-branch-name-here>` followed by `git checkout <your-new-branch-name-here>`. We will be triggering "submissions" via **pull request** into the master branch.

## 2. Programming Environment

As with previous assignments, a programming environment is supplied for you. This environment uses OpenGL, GLFW, and GLEW, and is supplied in C and C++ forms. You are free to use either of these implementations.

See the _Supplied Files_ section (below) for details on how to download
the framework.

## 3. Routines to Implement

Unlike previous projects or labs, you have some creative license as to file/code layout. You may benefit from a review of
patterns employed in previous labs, however, as the boilerplate for this final is laid out quite similarly. For example,
`BufferSet` is still an option for storing and passing buffered data to the shaders, code is still available for the loading
of shader files, the main display/render loop should look familiar, etc.

**Important note:**
You are not allowed to "hard-code" any parameters into your GLSL shader files; all of these parameters must be sent to the shaders from your main
application. You **are** permitted to define constants in the main
application (stored as variables - no "magic numers" should be found
interspersed within your application's control flow).

## 4. Supplied Files

The programming framework for this assignment is available in the `./final` subfolder beneath this page.

Under the `./final` folder, are subfolders `c` and `c++`, which contain the obvious things. In the C and C++ folders you'll find a file named `header.mak`, for use on the CS systems to help you generate a `Makefile` that will compile and link your program with the libraries used by the framework. See the contents of `header.mak` for details on how to do this. There is also a subfolder named `misc` which contains a shellscript named `compmac` for use on Mac systems.

You are free to incorporate additional resources into your project
(textures, 3D model files, surface normal data, etc.). **Please keep total
project size under 10MB.**

## 5. What to Submit

Please include all source files and scripts required to build and run
your application. If you include any texture data, model files, etc., you
will need to include those as well! If you are using the matrix libraries
referred to in our previous projects, you will **not** need to include
binaries in your repos - our systems already have these libraries
available. If, however, you choose to use another library, please include
said binaries **as well as** links to where the binaries/sources were originally found (GitHub, Sourceforge, etc.) (or email them to me).

The minimum acceptance test is that your code must be complete - that is, it must compile and link cleanly when submitted. Submissions **will not be accepted** if they fail to compile and link cleanly. *(Warning messages from the compiler are acceptable, but not fatal errors.)*

To submit your work, *commit* your changes to your CSCI 510 Git repository and *push*
it to your remote Github repository, using something like the following:

```sh
git stage .
git commit -m "My commit message"
git push
```

After you have committed your work and pushed to `remote`, create a pull request into the base `final` branch (a *PR*) on Github and assign/invite your instructur to the PR (in this case `jake-brandt`). If you decide that you would like to submit further changes after already kicking off your PR, just tag your instructor (something along the lines of "`@jake-brandt I have additional changes`") in a comment on the PR thread and let your instructor know that there are new changes which you would like to include in your graded assignment.


## 6. Grading

This project is worth **50 points**, distributed as follows:

* **5 points** for submitting code that compiles, runs, and produces a
  window.
* **10 points** for model transformations
    * The model vertices _need to_ be defined in model space; and then
      transformed to the desired world space. A world transform **must**
      include all three transforms: translation, rotation and scaling.
      Translation and rotation cannot be (0, 0, 0); and scaling cannot be
      (1, 1, 1).
* **10 points** for camera transformations
    * Your camera location must be **non-standard**, such that the
      position incorporates non-default settings for lookat, up, and
      position.
* **10 points** for texture mapping
    * You implement either spherical OR predefined (U, V) affine mapping;
      employing a texture image file or a procedurally-generated texture (such as some trig operation for a given U,V pair).
* **10 points** for Phong-based shading
    * You will employ a light located at a non-standard location. The
    the light is a point source (i.e. a lightbulb) and does not need to
    emulate directional or spotlight behavior. You will employ a
    non-white light to a non-white surface (the surface must be a
    different color than the light source).
* **5 points** for style and documentation

> **You also have the opportunity to earn extra-credit points (a total of 5 for any of the options) on this assignment. You will only be graded for one of the following; but will be graded for the highest-scoring opportunity:**
>
> * Adding an extra key input, triggering the export of the rendered
>   image to the file system.
> * Employing Gouraud shading for at least one shape; and Phong shading
>   for at least one shape.
> * Tracking the existing animation state (on/off) made available to you,
>   and animating the camera location.

## 7. Notes

The OpenGL, GLSL, shading, and texture mapping lecture notes presented in class should help you to get started on this assignment, as should the files provided with earlier OpenGL assignments.

Remember that Phong shading interpolates the normal vector across the polygon (done for you by the rasterizer in the OpenGL pipeline) and calculates color based on that.

Refer back to the "Hello, OpenGL!" programming assignment for information about obtaining and installing GLFW and/or GLEW libraries.

Remember that in addition to transforming the vertex and light locations into clip (eye) space, the vertex shader must also transform the surface normal into clip space. Many vertex shaders use an OpenGL built-in variable named `gl_NormalMatrix` to do this; however, because we are using shader-based OpenGL, you cannot use that variable. Here are some web pages which explain how to calculate it:

* [The Normal Matrix](http://www.lighthouse3d.com/tutorials/glsl-tutorial/the-normal-matrix/)
at `lighthouse3d.com`
* [OpenGL Normal Vector Transformation](http://www.songho.ca/opengl/gl_normaltransform.html) at `songho.ca` _(note: fixed-pipeline aka old OpenGL)_
* [gl_NormalMatrix in GLSL how to calculate?](https://www.gamedev.net/topic/598985-gl-normalmatrix-in-glsl-how-to-calculate/) at `gamedev.net`

Unfortunately, GLSL 1.30 does not have an `inverse()` function for matrices. To help you deal with this, the vertex shader files(`phong.vert` and `texture.vert`) contain implementations of `inverse()` for `float`, `mat2`, `mat3`, and `mat4` data types; these functions were written by Mikola Lysenko, and are available from his GitHub repository (URL in the shader files) under the terms of the MIT License.

You may use convenience libraries for loading your texture images. We recommend the use of SOIL, found online at [http://www.lonesock.net/soil.html](http://www.lonesock.net/soil.html), and already installed on the CS Ubuntu systems. All framework files in which SOIL might be used include the necessary header as `<SOIL.h>` - if you need to include it in other files, use the same path, and (if necessary) tell your compiler where to find the header; the `header.mak` file is set up to properly compile and link against SOIL on the CS Ubuntu systems.

If you are using SOIL on a Mac, you must remember to also use the CoreFoundations framework, as the Mac implementation of SOIL makes use of some of the functions in this framework. The `compmac` script (in the `misc` subfolder of the framework ZIP file) has comments in it pointing this out, as well as indicating changes that must be made to the `libs` and (possibly) `incs` assignments in the script.

SOIL is possible to compile for Windows with Visual Studio (tested on 2017 Community Edition). You may receive errors compiling `test_SOIL.cpp`; if this occurs, you will likely need to include `<ctime>` in this file.

**Ubuntu&reg; is a registered trademark of Canonical Ltd.**