# psx_cube_demo

I have created a drawing library (see drawing.c / drawing.h) for the Playstation 1 using the original Sony Playstation libraries. 
It allows you to draw any arbitrary 3D object (provided that they're in the correct format) with arbitrary textures. Using this, I
created the demo's below!

###  Cube program

https://user-images.githubusercontent.com/56715549/232962554-7d6fbff6-2940-4c7d-b055-9086281b39af.mp4

### Teapot program

https://user-images.githubusercontent.com/56715549/232962616-d35632f1-8aab-45b2-b159-73d7ed39bfb9.mp4

## Features
* Draw 3D models with different drawing modes:
  * Shaded Textured
  * Flat
  * Quad Flat
* Easy-to-use API for setting up the drawing environment
* Texture loading from TIM files

## `drawing.h`

The drawing.h header file defines the structures and functions used by the library:

`Texture`: Contains information about a texture, including the texture coordinates and mode.

`Index`: Defines the indices for vertices, UVs, and normals.

`DrawingMode`: Enumerates the available drawing modes.

`Model`: Defines a 3D model, including mesh, normals, colors, UVs, and texture.

The following functions are defined:

`setModel(Model *model)`: Sets the current model for drawing.

`drawFunc(VECTOR *position, SVECTOR *rotate, VECTOR *scale)`: Draws the model with the given position, rotation, and scale.

`loadTexture(u_long *tim, Texture *texture)`: Loads a texture from a TIM file.

`initDrawing(u_long ot[][OTLEN], char primbuff[][PRIMBUFFLEN], DISPENV *disp, DRAWENV *draw, short *db, char **nextpri)`: Initializes the drawing environment, buffers, and variables.

`displayDrawing()`: Displays the drawing on the screen.
