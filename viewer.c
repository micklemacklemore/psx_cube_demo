#include <stddef.h>

#include "constants.h"
#include "drawing.h"
#include "cube.c"

static DISPENV disp[2];
static DRAWENV draw[2];

static u_long ot[2][OTLEN];
static char primbuff[2][PRIMBUFFLEN];
static char *nextpri = primbuff[0];
static short db = 0;

void loadMyImage(Texture *texture) {
    extern u_long tim_cont[];
    // Load the texture
    loadTexture(tim_cont, texture);
}

int main() {
    int cube_i;           
    SVECTOR Rotate;                    
    VECTOR  Scale;
    unsigned int time = 0;

    initDrawing(ot, primbuff, disp, draw, &db, &nextpri);

    Model cube = {
        .mesh = model_mesh,
        .normals = NULL,
        .colors = NULL,
        .uv = model_UV,
        .index = model_index,
        .num_vertices = 36,
        .drawing_mode = 0,      // 0: textured, 1: flat-shaded, 2: untextured
        .texture = NULL,
    };

    VECTOR cubePositions[] = {
        {0, 0, 0+CENTERX * 2, 0},
        {200,  50, 150+CENTERX * 2, 0},
        {-150, -220, 250+CENTERX * 2, 0},
        {980, -800, 1230+CENTERX * 2, 0},
        {-240, 40, 35+CENTERX * 2, 0},
        {170,  -400, 750+CENTERX * 2, 0}
    };

    Texture cubeTexture;
    loadMyImage(&cubeTexture);
    cube.texture = &cubeTexture;
    setModel(&cube);

    while (1) {
        Rotate = (SVECTOR){ 232, 0, 0, 0 };                        
        Scale = (VECTOR){ ONE, ONE, ONE, 0 };  // ONE == 4096     
        Rotate.vy += time;  // Pan
        time += 8;

        // Clear the current OT
        ClearOTagR(ot[db], OTLEN);

        for (cube_i = 0; cube_i < 6; cube_i++) {
            Rotate.vx += 20 * cube_i; // Tilt
            Rotate.vz += 30 * cube_i; // Roll
            drawFunc(&cubePositions[cube_i], &Rotate, &Scale);
        }

        displayDrawing(disp, draw);
    }

    return 0;
}

