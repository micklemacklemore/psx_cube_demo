/*  primdrawG.c, by Schnappy, 12-2020
    - Draw a gouraud shaded mesh exported as a TMESH by the blender <= 2.79b plugin io_export_psx_tmesh.py
    based on primdraw.c by Lameguy64 (http://www.psxdev.net/forum/viewtopic.php?f=64&t=537)
    2014 Meido-Tek Productions.
    Controls:
        Start                           - Toggle interactive/non-interactive mode.
        Select                          - Reset object's position and angles.
        L1/L2                           - Move object closer/farther.
        L2/R2                           - Rotate object (XY).
        Up/Down/Left/Right              - Rotate object (XZ/YZ).
        Triangle/Cross/Square/Circle    - Move object up/down/left/right.
*/

#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include <libetc.h>
#include <stdio.h>
// Sample vector model

#include "cube.c"
#define VMODE       0
#define SCREENXRES 320
#define SCREENYRES 240
#define CENTERX     SCREENXRES/2
#define CENTERY     SCREENYRES/2
#define OTLEN       2048        // Maximum number of OT entries
#define PRIMBUFFLEN 32768       // Maximum number of POLY_GT3 primitives

// Display and draw environments, double buffered
DISPENV disp[2];
DRAWENV draw[2];

u_long      ot[2][OTLEN];                       // Ordering table (contains addresses to primitives)
char        primbuff[2][PRIMBUFFLEN];           // Primitive list // That's our prim buffer
char * nextpri = primbuff[0];                   // Primitive counter
short           db  = 0;                        // Current buffer counter

// texture globals
int tim_mode;           // TIM image parameters
RECT tim_prect,tim_crect;
int tim_uoffs,tim_voffs;

void init(void);
void display(void);
void loadTexture(u_long *tim, TIM_IMAGE *tparam);
void loadMyImage(void);


void loadTexture(u_long *tim, TIM_IMAGE *tparam){
    // Read TIM parameters (PsyQ)
    OpenTIM(tim);
    ReadTIM(tparam);

    // Read TIM parameters (PSn00bSDK)
    //GetTimInfo(tim, tparam);

    // Upload pixel data to framebuffer
    LoadImage(tparam->prect, (u_long*)tparam->paddr);
    DrawSync(0);

    // Upload CLUT to framebuffer if present
    if( tparam->mode & 0x8 ) {

        LoadImage(tparam->crect, (u_long*)tparam->caddr);
        DrawSync(0);

    }
}

void loadMyImage(void) {
    TIM_IMAGE my_image;

    extern u_long tim_cont[];;

    // load the texture
    loadTexture(tim_cont, &my_image);

    // Copy the TIM coordinates
    tim_prect   = *my_image.prect;
    tim_crect   = *my_image.crect;
    tim_mode    = my_image.mode;

    // Calculate U,V offset for TIMs that are not page aligned
    tim_uoffs = (tim_prect.x%64)<<(2-(tim_mode&0x3));
    tim_voffs = (tim_prect.y&0xff);
}

void init() {
    // Reset the GPU before doing anything and the controller
    PadInit(0);
    ResetGraph(0);

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

    SetDispMask(1);                 // Display on screen  

    setRGB0(&draw[0], 51, 76, 76);
    setRGB0(&draw[1], 51, 76, 76);

    draw[0].isbg = 1;
    draw[1].isbg = 1;

    loadMyImage();

    PutDispEnv(&disp[db]);
    PutDrawEnv(&draw[db]);

    // Init font system
    FntLoad(960, 0);
    FntOpen(16, 16, 196, 64, 0, 256);
}

void display(void) {
    DrawSync(0);
    VSync(0);
    PutDispEnv(&disp[db]);
    PutDrawEnv(&draw[db]);
    DrawOTag(&ot[db][OTLEN - 1]);
    db = !db;
    nextpri = primbuff[db];
}

int main() {
    int     i, cube_i;
    long    t, p, OTz, Flag;                       // t == vertex count, p == depth cueing interpolation value, OTz ==  value to create Z-ordered OT, Flag == see LibOver47.pdf, p.143    
    POLY_GT3 *poly = {0};                           // pointer to a POLY_G4 
    SVECTOR Rotate = { 232, 0, 0, 0 };                        
    VECTOR  Scale = { ONE, ONE, ONE, 0 };            // ONE == 4096
    MATRIX  Matrix={0};                              // Matrix data for the GTE
    unsigned int time = 0;

    VECTOR cubePositions[] = {
        {0, 0, 0+CENTERX * 2, 0},
        {200,  50, 150+CENTERX * 2, 0},
        {-150, -220, 250+CENTERX * 2, 0},
        {980, -800, 1230+CENTERX * 2, 0},
        {-240, 40, 35+CENTERX * 2, 0},
        {170,  -400, 750+CENTERX * 2, 0}
    };

    init();

    // Main loop
    while (1) {
        Rotate = (SVECTOR){ 232, 0, 0, 0 };                        
        Scale = (VECTOR){ ONE, ONE, ONE, 0 };      // ONE == 4096
        Matrix = (MATRIX){ 0 };        
        Rotate.vy += time; // Pan
        time += 8;

        // Clear the current OT
        ClearOTagR(ot[db], OTLEN);

        for (cube_i = 0; cube_i < 6; cube_i++) {
            // Convert and set the matrixes
            //Rotate.vy += 9 * i; // Pan
            Rotate.vx += 2 * i; // Tilt
            Rotate.vz += 3 * i; // Roll
            RotMatrix(&Rotate, &Matrix);
            TransMatrix(&Matrix, &cubePositions[cube_i]);
            ScaleMatrix(&Matrix, &Scale);
            SetRotMatrix(&Matrix);
            SetTransMatrix(&Matrix);

            // modelCube is a TMESH, len member == # vertices, but here it's # of triangle... So, for each tri * 3 vertices ...
            for (i = 0; i < (36); i += 3) {               
                poly = (POLY_GT3 *)nextpri;

                // Initialize the primitive and set its color values
                SetPolyGT3(poly);
                setRGB0(poly, 128, 128, 128);
                setRGB1(poly, 128, 128, 128);
                setRGB2(poly, 128, 128, 128);
                setTPage(poly, tim_mode&0x3, 0, tim_prect.x, tim_prect.y);
                setUV3(
                    poly, 
                    tim_uoffs+model_UV[index[i].uv][0], 
                    tim_voffs+model_UV[index[i].uv][1], 
                    tim_uoffs+model_UV[index[i + 1].uv][0], 
                    tim_voffs+model_UV[index[i + 1].uv][1], 
                    tim_uoffs+model_UV[index[i + 2].uv][0], 
                    tim_voffs+model_UV[index[i + 2].uv][1]
                );

                // Rotate, translate, and project the vectors and output the results into a primitive
                OTz = RotTransPers(&model_mesh[index[i].vertex], (long*)&poly->x0, &p, &Flag);
                OTz += RotTransPers(&model_mesh[index[i + 1].vertex], (long*)&poly->x1, &p, &Flag);
                OTz += RotTransPers(&model_mesh[index[i + 2].vertex], (long*)&poly->x2, &p, &Flag);

                // Sort the primitive into the OT
                OTz /= 3;
                if ((OTz > 0) && (OTz < OTLEN))
                    AddPrim(&ot[db][OTz-2], poly);
                nextpri += sizeof(POLY_GT3);
            }
        }

        FntPrint("");
        FntFlush(-1);
        display();
    }
    return 0;
}
