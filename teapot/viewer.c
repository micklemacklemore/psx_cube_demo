#include <stddef.h>

#include "constants.h"
#include "drawing.h"
#include "teapot.c"

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
    SVECTOR Rotate;                    
    VECTOR  Scale;
    unsigned int time = 0;

    initDrawing(ot, primbuff, disp, draw, &db, &nextpri);

    Model cube = {
        .mesh = model_mesh,
        .normals = NULL,
        .colors = NULL,
        .uv = NULL,
        .index = model_index,
        .num_vertices = 27012,
        .drawing_mode = FLAT,    
        .texture = NULL,
    };

    VECTOR Position = {0, 2000, 0 + CENTERX * 30, 0};

    Texture cubeTexture;
    loadMyImage(&cubeTexture);
    cube.texture = &cubeTexture;
    setModel(&cube);

    while (1) {
        Rotate = (SVECTOR){ 2100, 0, 0, 0 };                        
        Scale = (VECTOR){ ONE/8, ONE/8, ONE/8, 0 };  // ONE == 4096     
        Rotate.vy += time;  // Pan
        time += 32;

        // Clear the current OT and initialize a new one
        ClearOTagR(ot[db], OTLEN);

        drawFunc(&Position, &Rotate, &Scale);

        displayDrawing();
    }

    return 0;
}

