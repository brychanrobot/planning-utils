# Planning Utilies
This repo includes a collection of libraries and functions that are useful for path planning. In order to compile it, you will need to install several libraries. On Ubuntu or one of its variants you can run.

`sudo apt install libboost-all-dev libglfw3-dev mesa-common-dev freeglut3-dev`

I recommend adding this library to your project as a submodule. I've made the folder structure relatively easy in order to accomodate that.

# Geom
This folder provides 2D Coord and Rect classes which handle the basic needs of those geometries.

# utils.hpp
Provides map generation and obstacle collision checking

# display.hpp
Provides various drawing methods. To use this file first call `initWindow`, and then call `displayLoop` with the `display` and `remainder` callbacks. The display function should hold all of your code for displaying whatever it is that you want to display. It is called at the frame rate specified. The remainder function is called every time the display loop executes unless the display was redrawn. You can use this function to do any other logic you might have. Sampling from RRT* for example.

# Compiling
You'll need to compile this with C++14 and link to the following libraries.

* m
* GL
* GLU
* glfw
* glut
* X11
* Xrandr
* Xinerama
* Xi
* Xxf86vm
* Xcursor
* pthread
* dl