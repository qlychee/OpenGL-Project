Elizabeth Qiu 
Final Project
CSCI4229 Fall 2018

Description: This is a rendering of a field of flowers with animations to simulate flowers in wind.
When the wind is activated, particles/"petals" representing the speed and movement of the wind appear and the flowers begin a slight back and forth motion. As the wind speed increases, the flowers will tilt further in the direction of the wind, petals disappear from the flowers, and fog density increases. The wind direction can be changed, as will the direction of the flower tilt. Once the wind speed reaches above a certain amounts, the back and forth motion frequency is also increased to imitate the motion of being blown by strong wind. Changing the color of the flowers will also change the color of the blowing petals, though it takes a moment for the newly colored petals to be visible. The white flowers/petals stay constant. The lighting source is in a fixed position that is approximately where the sun appears in the skybox texture.


Build: make
Run: ./main

Key Bindings:
 mouse		change view angle
 [		decrease number of flowers (min 0)
 ]		increase number of flowers (max 49)
 {		decrease flower height (min 0)
 }		increase flower height (max 150)
 (		end/decrease wind speed and fog
 )		start/increase wind speed and fog
 </>		change wind direction
 c		toggle through flower colors*
 t		change flower type
 -/_		decrease fov (zoom in)
 +/=		increase fov (zoom out)
 x		show axes
 ESC		exit

*There are 5 color options, and the 5th color is random. The 5th color will be different every time it is toggled in the cycle.

Project Notes: I believe that I met my original proposal of animating a simulation of flowers blowing in the wind in a 3D environment. There is user interaction for changing between two designs of flowers, different colors, decreasing or increasing the height, as well as the number of flowers on the field. I do wish that I had pushed for more realism beyond imitating the push and pull of the wind and the stem, and having the petals disappear as the wind becomes stronger and the flowers are blown closer to the ground, as well as more realism in the look of the flowers through textures and maybe adding more details like leaves.

Credit:
Adapted from Wind Turbine/Particle Sim by Thomas Spooner