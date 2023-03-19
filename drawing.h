#ifndef DRAWING_H
#define DRAWING_H

#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include <libetc.h>
#include "constants.h"

typedef struct {
    RECT prect;
    RECT crect;
    int mode;
    int uoffset;
    int voffset;
} Texture;

typedef struct {
    int vertex, uv, normal;
} Index;

typedef struct {
    SVECTOR *mesh;
    SVECTOR *normals;
    CVECTOR *colors;
    u_char (*uv)[2];
    Index *index;
    int num_vertices;
    int drawing_mode; // 0: textured, 1: flat-shaded, 2: untextured
    Texture *texture;
} Model;

void setModel(Model *model);
void drawFunc(VECTOR *position, SVECTOR *rotate, VECTOR *scale);
void drawModel(VECTOR *position, SVECTOR *rotate, VECTOR *scale);
void drawModelFlatShaded(VECTOR *position, SVECTOR *rotate, VECTOR *scale);
void drawModelUntextured(VECTOR *position, SVECTOR *rotate, VECTOR *scale);
void loadTexture(u_long *tim, Texture *texture);
void initDrawing(u_long ot[][OTLEN], char primbuff[][PRIMBUFFLEN], DISPENV *disp, DRAWENV *draw, short *db, char **nextpri);
void display(DISPENV *disp, DRAWENV *draw);

#endif // DRAWING_H