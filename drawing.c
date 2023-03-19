#include "drawing.h"
#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include <libetc.h>
#include <stddef.h>

// static global variables
static Model *currentModel = NULL;
static void (*drawModelFunc)(VECTOR *, SVECTOR *, VECTOR *) = NULL;
static unsigned long (*_ot)[OTLEN] = NULL;
static char (*_primbuff)[PRIMBUFFLEN] = NULL;
static char *_nextpri = NULL;
static short *_db = NULL;

// drawing functions
void drawModelTextured(VECTOR *position, SVECTOR *rotate, VECTOR *scale);
void drawModelFlatShaded(VECTOR *position, SVECTOR *rotate, VECTOR *scale);
void drawModelUntextured(VECTOR *position, SVECTOR *rotate, VECTOR *scale);

void setModel(Model *model) {
    if (model == NULL) {
        currentModel = NULL;
        drawModelFunc = NULL;
    }

    currentModel = model;

    switch (currentModel->drawing_mode) {
        case 0:
            drawModelFunc = drawModelTextured;
            break;
        case 1:
            drawModelFunc = drawModelFlatShaded;
            break;
        case 2:
            drawModelFunc = drawModelUntextured;
            break;
        default:
            break;
    }
}

void drawFunc(VECTOR *position, SVECTOR *rotate, VECTOR *scale) {
    if (drawModelFunc == NULL) {
        return;
    }
    drawModelFunc(position, rotate, scale);
}

void drawModelTextured(VECTOR *position, SVECTOR *rotate, VECTOR *scale) {
    long t, p, OTz, Flag;
    POLY_GT3 *poly;
    MATRIX  Matrix = { 0 };  // Matrix data for the GTE

    if (currentModel == NULL) {
        return;
    }

    Index *index = currentModel->index;
    Texture *texture = currentModel->texture;

    RotMatrix(rotate, &Matrix);
    TransMatrix(&Matrix, position);
    ScaleMatrix(&Matrix, scale);
    SetRotMatrix(&Matrix);
    SetTransMatrix(&Matrix);

    for (t = 0; t < currentModel->num_vertices; t += 3) {
        poly = (POLY_GT3 *)_nextpri;

        SetPolyGT3(poly);
        setRGB0(poly, 128, 128, 128);
        setRGB1(poly, 128, 128, 128);
        setRGB2(poly, 128, 128, 128);
        setTPage(poly, texture->mode & 0x3, 0, texture->prect.x, texture->prect.y);
        setUV3(
            poly,
            texture->uoffset + currentModel->uv[index[t].uv][0],
            texture->voffset + currentModel->uv[index[t].uv][1],
            texture->uoffset + currentModel->uv[index[t + 1].uv][0],
            texture->voffset + currentModel->uv[index[t + 1].uv][1],
            texture->uoffset + currentModel->uv[index[t + 2].uv][0],
            texture->voffset + currentModel->uv[index[t + 2].uv][1]
        );

        OTz = RotTransPers(&currentModel->mesh[index[t].vertex], (long *)&poly->x0, &p, &Flag);
        OTz += RotTransPers(&currentModel->mesh[index[t + 1].vertex], (long *)&poly->x1, &p, &Flag);
        OTz += RotTransPers(&currentModel->mesh[index[t + 2].vertex], (long *)&poly->x2, &p, &Flag);

        OTz /= 3;
        if ((OTz > 0) && (OTz < OTLEN)) {
            AddPrim(&_ot[*_db][OTz - 2], poly);
        }
        _nextpri += sizeof(POLY_GT3);
    }
}

void drawModelFlatShaded(VECTOR *position, SVECTOR *rotate, VECTOR *scale) {
    // Implement the flat-shaded drawing code here (with POLY_F3 or POLY_F4)
}

void drawModelUntextured(VECTOR *position, SVECTOR *rotate, VECTOR *scale) {
    // Implement the untextured drawing code here (with POLY_G3 or POLY_G4)
}

void loadTexture(u_long *tim, Texture *texture) {
    TIM_IMAGE tim_image;

    // Read TIM parameters
    OpenTIM(tim);
    ReadTIM(&tim_image);

    // Upload pixel data to framebuffer
    LoadImage(tim_image.prect, (u_long *) tim_image.paddr);
    DrawSync(0);

    // Upload CLUT to framebuffer if present
    if (tim_image.mode & 0x8) {
        LoadImage(tim_image.crect, (u_long *) tim_image.caddr);
        DrawSync(0);
    }

    // Store texture information in Texture struct
    texture->prect = *tim_image.prect;
    texture->crect = *tim_image.crect;
    texture->mode = tim_image.mode;
    texture->uoffset = (tim_image.prect->x % 64) << (2 - (tim_image.mode & 0x3));
    texture->voffset = (tim_image.prect->y & 0xff);
}

void initDrawing(u_long ot[][OTLEN], char primbuff[][PRIMBUFFLEN], DISPENV *disp, DRAWENV *draw, short *db, char **nextpri) {
    // Reset the GPU before doing anything and the controller
    PadInit(0);
    ResetGraph(0);

    // Set globals
    _ot = ot;
    _primbuff = primbuff;
    _nextpri = *nextpri;
    _db = db;

    // Initialize and setup the GTE
    InitGeom();
    SetGeomOffset(CENTERX, CENTERY);        // x, y offset
    SetGeomScreen(CENTERX);                 // Distance between eye and screen  

    // Set the display and draw environments
    SetDefDispEnv(&disp[0], 0, 0         , SCREENXRES, SCREENYRES);
    SetDefDispEnv(&disp[1], 0, SCREENYRES, SCREENXRES, SCREENYRES);
    SetDefDrawEnv(&draw[0], 0, SCREENYRES, SCREENXRES, SCREENYRES);
    SetDefDrawEnv(&draw[1], 0, 0, SCREENXRES, SCREENYRES);

    if (VMODE)
    {
        SetVideoMode(MODE_PAL);
        disp[0].screen.y += 8;
        disp[1].screen.y += 8;
    }

    SetDispMask(1); // Display on screen  

    // TODO: set background color
    setRGB0(&draw[0], 51, 76, 76);
    setRGB0(&draw[1], 51, 76, 76);

    draw[0].isbg = 1;
    draw[1].isbg = 1;

    PutDispEnv(&disp[*_db]);
    PutDrawEnv(&draw[*_db]);
}

void displayDrawing(DISPENV *disp, DRAWENV *draw) {
    DrawSync(0);
    VSync(0);
    PutDispEnv(&disp[*_db]);
    PutDrawEnv(&draw[*_db]);
    DrawOTag(&_ot[*_db][OTLEN - 1]);
    *_db = !(*_db);
    _nextpri = _primbuff[*_db];
}
