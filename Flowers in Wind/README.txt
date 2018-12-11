Elizabeth Qiu 
Final Project
CSCI4229 Fall 2018

Description: This is a 3D render of a field of flowers with animations to simulate flowers in wind.
When the wind is activated, particles/"petals" representing the speed and movement of the wind appear and the flowers begin a slight back and forth motion. As the wind speed increases, the flowers will tilt further in the direction of the wind, petals disappear from the flowers, and fog density increases. The wind direction can be changed, as will the direction of the flower tilt. Once the wind speed reaches above a certain amount, the back and forth motion frequency is also increased to imitate the motion of being blown by strong wind. Changing the color of the petals will also change the color of the petals, though it takes a moment for it to update. The white flowers/petals stay constant. The lighting source is in a fixed position that is approximately where the sun appears in the skybox texture. 


Build: make
Run: ./main

Key Bindings:
 mouse		change view angle
 [		decrease number of flowers (min 0)
 ]		increase number of flowers (max 49)
 (		end/decrease wind speed and fog
 )		start/increase wind speed and fog
 </>		change wind direction
 c		toggle through flower colors
 -/_		decrease fov (zoom in)
 +/=		increase fov (zoom out)
 x		show axes
 ESC		exit



Credit:
Adapted from Wind Turbine/Particle Sim by Thomas Spooner