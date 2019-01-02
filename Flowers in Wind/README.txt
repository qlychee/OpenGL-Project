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

Credit:
Adapted from Wind Turbine/Particle Sim by Thomas Spooner
