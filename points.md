1. Behind the scenes OpenGL uses the data specified via glViewport to transform the 2D coordinates it processed to coordinates on your screen. 		
For example, a processed point of location (-0.5,0.5) would (as its final transformation) be mapped to (200,450) in screen coordinates. 
Note that processed coordinates in OpenGL are between -1 and 1 so we effectively map from the range (-1 to 1) to (0, 800) and (0, 600).
2. Sprites are the render-able image/texture objects we use in a 2D game
3. projection matrix xonverts all view-space coorfinates to clip-space (and then to normalized device) coordinates
	the game is entirely in 2d so an orthographic projection matrix is used
4. AABB stands for axis-aligned bounding box
5. Particle emitter or particle generator
6. Note that we render the particles before we render the ball. This way, the particles end up rendered in front of all other objects, but behind the ball.
