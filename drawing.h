#ifndef DRAWING_H
#define DRAWING_H

#include <libgte.h>
#include <libgpu.h>
#include <constants.h>

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

typedef enum {
    QUAD_FLAT,
    FLAT, 
    FLAT_TEXTURED, 
    SHADED, 
    SHADED_TEXTURED
} DrawingMode;

typedef struct {
    SVECTOR *mesh;
    SVECTOR *normals;
    CVECTOR *colors;
    u_char (*uv)[2];
    Index *index;
    int num_vertices;
    DrawingMode drawing_mode;
    Texture *texture;
} Model;

void setModel(Model *model);
void drawFunc(VECTOR *position, SVECTOR *rotate, VECTOR *scale);
void loadTexture(u_long *tim, Texture *texture);
void initDrawing(u_long ot[][OTLEN], char primbuff[][PRIMBUFFLEN], DISPENV *disp, DRAWENV *draw, short *db, char **nextpri);
void displayDrawing();

#endif // DRAWING_H