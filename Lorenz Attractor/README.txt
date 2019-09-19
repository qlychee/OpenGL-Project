Elizabeth Qiu 
HW2- Lorenz Attractor
CSCI4229 Fall 2018

Three dimensional OpenGL visualization of the Lorenz Attractor

Objective:
Provide a line trace of the sequence of attractor
Allow the trace to be viewed from different positions
Allow one or more of the parameters in the Lorenz equations to be modified interactively

Credits:
Display, idle functions from https://github.com/mikepack/lorenz/blob/master/lorenz.c
Special (arrow keys) and key functions based on ex6.c
Lorenz default parameters and equations from lorenz.c integrator on Moodle

vsync enable in main() for uncapped Windows systems,
the following includes and calls are commented out to compile and run on Linux systems
#include <GL/glew.h>
#include <GL/wglew.h>
glewInit();
wglSwapIntervalEXT(1);
the program is run with glew
(necessary tag included in Windows, where this program was tested)

How to run: type "make" then "lorenz"

Key Bindings:
 [        decrease s
 ]        increase s
 {        decrease r
 }        increase r
 r        reset the drawing
 d        set s/r back to default
 arrows   change view angle
 ESC      exit


time spent: 2.5 hours