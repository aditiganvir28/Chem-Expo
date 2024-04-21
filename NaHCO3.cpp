#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>
#include <map>
#include <iostream>
#include <string>
#include "SOIL.h"

using namespace std;
#include <GL/glut.h>
bool assigned = false;
GLuint texture[7];

int LoadGLTextures()
{
     // texture[2] = SOIL_load_OGL_texture("img/H-final.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    // texture[3] = SOIL_load_OGL_texture("img/O-final.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    // texture[4] = SOIL_load_OGL_texture("img/S-final.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    // texture[5] = SOIL_load_OGL_texture("img/C-final.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    // texture[6] = SOIL_load_OGL_texture("img/B-final.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    texture[1] = SOIL_load_OGL_texture("img/Na-final.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    texture[2] = SOIL_load_OGL_texture("img/C-final.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    texture[3] = SOIL_load_OGL_texture("img/H-final.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    texture[4] = SOIL_load_OGL_texture("img/O-final.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    texture[5] = SOIL_load_OGL_texture("img/C-final.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    texture[6] = SOIL_load_OGL_texture("img/O-final.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

    if (texture[0] == 0)
    {
        return false;
    }
    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glEnable(GL_TEXTURE_2D);                           // Enable Texture Mapping ( NEW )
    glShadeModel(GL_SMOOTH);                           // Enable Smooth Shading
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);              // Black Background
    glClearDepth(1.0f);                                // Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);                           // Enables Depth Testing
    glDepthFunc(GL_LEQUAL);                            // The Type Of Depth Testing To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Really Nice Perspective Calculations
    return true;                                       // Return Success
}

// Global coordinates for cyclopentane
GLfloat NaHCO3_coords[6][3] = {
    {-8.25, 1.299038106, 0},   // Na on left O
    {-7.25, 1.299038106, 0},   // left O
    {-6.25, 0, 0},             // C
    {-5.25, 1.299038106, 0},   // right O
    {-4.25, 1.299038106, 0},   // H on right O
    {-6.25, -1.299038106, 0}}; // down O

GLfloat pa_coords[12][3] = {

    // horizontal line for second plus
    {0.5, 0, 0},
    {1.5, 0, 0},
    // vertical line for second plus
    {1, -0.5, 0},
    {1, 0.5, 0},


    // horizontal line for 1st plus
    {4.5, 0, 0},
    {5.5, 0, 0},
    // vertical line for 1st plus
    {5, -0.5, 0},
    {5, 0.5, 0},
    
    // arrow
    {-4, 0, 0},
    {-3, 0, 0}, //middle
    {-3.3, 0.353553391, 0},
    {-3.3, -0.353553391, 0}};

// Global coordinates for HBr
GLfloat Na2CO3_coords[6][3] = {
    {0, 0, 0},     // left Na
    {2, 0, 0},     // C
    {1, 1.29, 0},  // left up O
    {3, 1.29, 0},  // right up O
    {2, -1.29, 0}, // down O
    {4, 0, 0}      // right Na

};

// Global coordinates for bromocyclopentane
GLfloat CO2_coords[3][3] = {
    {7, 1.29, 0}, // up O
    {7, 0, 0}, // C
    {7, -1.29, 0}  // down O
};

GLfloat H2O_coords[3][3] = {
    {10, -1.29, 0}, // left H
    {11, 0, 0},     // O
    {12, -1.29, 0}  // right H
};

GLfloat product_Na2CO3_coords[6][3] = {
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}
    };

GLfloat product_CO2_coords[3][3] = {
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}};

GLfloat product_H2O_coords[6][3] = {
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}};

GLfloat deltas_NaHCO3[6][3] = {
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}};

GLfloat deltas_CO2[3][3] = {
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}};

GLfloat deltas_H2O[3][3] = {
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}};

GLfloat NaHCO3_orig_coords[6][3] = {
     {-8.25, 1.299038106, 0},   // Na on left O
    {-7.25, 1.299038106, 0},   // left O
    {-6.25, 0, 0},             // C
    {-5.25, 1.299038106, 0},   // right O
    {-4.25, 1.299038106, 0},   // H on right O
    {-6.25, -1.299038106, 0}}; // down O

int selectedObject = 3;
bool drawThatAxis = 0;
bool lightEffect = 1;
int lightOption = 1;
int projection = 1;
int drawLegend = 1;
GLdouble sphereRadius = 0.4;
GLdouble cylinderRadius = 0.2;
GLint resolution = 100;
GLint slices = resolution, stacks = resolution;

// defining time counters for the animation.
int time_cnt = 0;
int t1 = 125, t2 = 155, t3 = 500;

// Animation variables
int camera = 1;
int animation = 0, merging = 0, merge_cnt = 0, merge_pause = 20, to_print = 1, calc = 1, is_rotate = 0;
GLdouble rot_angle = 0;
GLdouble CBrcylinderRadius = 0;
GLdouble HBrcylinderRadius = 0.2;
GLdouble delta_HBr = cylinderRadius / (t2 - t1);
GLdouble delta_CBr = delta_HBr;

// Viewer options (GluLookAt)
float fovy = 60.0, aspect = 1.0, zNear = 1.0, zFar = 100.0;

// Mouse modifiers
float depth = 8;
float phi = 0, theta = 0;
float downX, downY;
bool leftButton = false, middleButton = false;

// colors
GLfloat oxygen[3] = {1.0, 0.5, 0.0};   // (O - Golden)
GLfloat sodium[3] = {0, 0, 0.8};   // (N - Blue)
GLfloat carbon[3] = {0.7, 0.5, 0.7};   // (C - Grey)
GLfloat hydrogen[3] = {0.0, 1.0, 0.0}; // (H - Green)
GLfloat white[3] = {1.0, 1.0, 1.0};
GLfloat black[3] = {0.0, 0.0, 0.0};
GLfloat color_t1[3] = {0.0, 0.0, 0.0};
GLfloat color_t2[3] = {1.0, 1.0, 1.0};
GLfloat delta_color = 1.0 / (t2 - t1);

/* Functions used */
void draw_atom(GLfloat color[3]);
void setLightColor(GLfloat light_color[3]);
void renderCylinder(float x1, float y1, float z1, float x2, float y2, float z2, float radius, GLUquadricObj *quadrilc);
void drawAxis();
void reshape(int w, int h);
void mouseCallback(int button, int state, int x, int y);
void motionCallback(int x, int y);
void keyboardCallback(unsigned char ch, int x, int y);
void displayCallback(void);
void buildDisplayList();
void options_menu(int input);
void initMenu();

// Adding legends on the screen
void displayLegend()
{

    // displaying the menu on screen
    // glClearColor(0, 1, 0, 1);
    // glColor3f(1.0, 1.0, 1.0);

    glPushMatrix();
    glRotatef(40, 0, 0, 1);
    // glRotatef(20, 0, 1, 0);
    // glRotatef(20, 1, 0, 0);
    setLightColor(white);
    string legend_disp_1 = "Use W/A (along X), S/D(along Y), 5/8(along Z) to Translate";
    string legend_disp_2 = "Use Mouse: Hold Scroll and Drag to Zoom In/Out";
    string legend_disp_3 = "Use Mouse: Left Click and Drag to Rotate Axis";
    string legend_disp_4 = "Press Enter to Start/Pause animation";
    string legend_disp_5 = "Press Space to Start/Pause rotation";
    string legend_disp_6 = "Press R to Reset animation";
    string legend_disp_7 = "Press + to Add/Remove Axis";
    // string legend_disp_8 = "Press - to Change Lighting effect";
    string legend_disp_9 = "Press Esc to Close Window";
    string legend_disp_10 = "Press P to Change Projection";
    string legend_disp_11 = "Press C to Change Camera";
    string legend_disp_12 = "Press L to Change Light Effects";
    string legend_disp_13 = "Press M to Toggle Legend";

    glRasterPos3f(4, 4.1, 0);
    for (int i = 0; i < legend_disp_1.size(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, legend_disp_1[i]);
    }
    glRasterPos3f(4, 3.8, 0);
    for (int i = 0; i < legend_disp_2.size(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, legend_disp_2[i]);
    }
    glRasterPos3f(4, 3.5, 0);
    for (int i = 0; i < legend_disp_3.size(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, legend_disp_3[i]);
    }
    glRasterPos3f(4, 3.2, 0);
    for (int i = 0; i < legend_disp_4.size(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, legend_disp_4[i]);
    }
    glRasterPos3f(4, 2.9, 0);
    for (int i = 0; i < legend_disp_5.size(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, legend_disp_5[i]);
    }
    glRasterPos3f(4, 2.6, 0);
    for (int i = 0; i < legend_disp_6.size(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, legend_disp_6[i]);
    }
    glRasterPos3f(4, 2.3, 0);
    for (int i = 0; i < legend_disp_7.size(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, legend_disp_7[i]);
    }
    glRasterPos3f(4, 2, 0);
    for (int i = 0; i < legend_disp_9.size(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, legend_disp_9[i]);
    }
    glRasterPos3f(4, 1.7, 0);
    for (int i = 0; i < legend_disp_10.size(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, legend_disp_10[i]);
    }
    glRasterPos3f(4, 1.4, 0);
    for (int i = 0; i < legend_disp_11.size(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, legend_disp_11[i]);
    }
    glRasterPos3f(4, 1.1, 0);
    for (int i = 0; i < legend_disp_12.size(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, legend_disp_12[i]);
    }
    glRasterPos3f(4, 0.8, 0);
    for (int i = 0; i < legend_disp_11.size(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, legend_disp_13[i]);
    }

    string info_1 = "Reaction: Thermal Decomposition of Sodium Bicarbonate";
    string info_2 = "NaHCO3 -> Na2CO3 + H2O + CO2";
    string info_3 = "Sodium bicarbonate's decomposition, releasing CO2 and its alkalinity,";
    string info_4 = "serves in food, cleaning, firefighting, and medicine.";

    // glScalef(2, 2, 2);
    glRasterPos3f(-3.5, -2, 0);
    for (int i = 0; i < info_1.size(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, info_1[i]);
    }
    glRasterPos3f(-3.5, -2.5, 0);
    for (int i = 0; i < info_2.size(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, info_2[i]);
    }
    glRasterPos3f(-3.5, -3, 0);
    for (int i = 0; i < info_3.size(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, info_3[i]);
    }
    glRasterPos3f(-3.5, -3.5, 0);
    for (int i = 0; i < info_4.size(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, info_4[i]);
    }

    string atom_1 = "Symbol     Name      Number        Mass";
    string atom_2 = "  H       Hydrogen      1           1.008";
    string atom_3 = "  O       Oxygen        8           15.999";
    string atom_4 = "  C       Carbon        6           12.011";
    string atom_5 = "  Na       Na            11          22.990";

    // glScalef(2, 2, 2);
    glRasterPos3f(-1.5, -2, 0);
    for (int i = 0; i < atom_1.size(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, atom_1[i]);
    }
    glRasterPos3f(-1.5, -2.5, 0);
    for (int i = 0; i < atom_2.size(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, atom_2[i]);
    }
    glRasterPos3f(-1.5, -3, 0);
    for (int i = 0; i < atom_3.size(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, atom_3[i]);
    }
    glRasterPos3f(-1.5, -3.5, 0);
    for (int i = 0; i < atom_4.size(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, atom_4[i]);
    }
    glRasterPos3f(-1.5, -4, 0);
    for (int i = 0; i < atom_5.size(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, atom_5[i]);
    }

    glPopMatrix();
    // glFlush();
}

// Function for displaying projection
void display_projection()
{
    string projection_info[] = {"Perspective Projection", "Parallel Projection: Isometric",
                                "Parallel Projection: Cavalier", "Parallel Projection: Cabinet"};

    string pr = projection_info[projection - 1];
    setLightColor(white);
    glPushMatrix();
    glRotatef(90, 0, 1, 0);
    glRotatef(40, 0, 0, 1);
    glRasterPos3f(-3, -2, -5);
    for (int i = 0; i < pr.size(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, pr[i]);
    }

    glPopMatrix();
}

// Function for drawing plus and arrow
void draw_plus_arrow()
{
    // plus
    GLUquadric *myQuad;
    myQuad = gluNewQuadric();

    // // first plus
    setLightColor(oxygen);
    renderCylinder(pa_coords[0][0], pa_coords[0][1], pa_coords[0][2], pa_coords[1][0], pa_coords[1][1], pa_coords[1][2], 0.1, myQuad);
    renderCylinder(pa_coords[2][0], pa_coords[2][1], pa_coords[2][2], pa_coords[3][0], pa_coords[3][1], pa_coords[3][2], 0.1, myQuad);

    // // second plus
    setLightColor(oxygen);
    renderCylinder(pa_coords[4][0], pa_coords[4][1], pa_coords[4][2], pa_coords[5][0], pa_coords[5][1], pa_coords[5][2], 0.1, myQuad);
    renderCylinder(pa_coords[6][0], pa_coords[6][1], pa_coords[6][2], pa_coords[7][0], pa_coords[7][1], pa_coords[7][2], 0.1, myQuad);

    // arrow
    myQuad = gluNewQuadric();

    setLightColor(oxygen);
    renderCylinder(pa_coords[8][0], pa_coords[8][1], pa_coords[8][2], pa_coords[9][0], pa_coords[9][1], pa_coords[9][2], 0.1, myQuad);
    renderCylinder(pa_coords[11][0], pa_coords[11][1], pa_coords[11][2], pa_coords[9][0], pa_coords[9][1], pa_coords[9][2], 0.1, myQuad);
    renderCylinder(pa_coords[10][0], pa_coords[10][1], pa_coords[10][2], pa_coords[9][0], pa_coords[9][1], pa_coords[9][2], 0.1, myQuad);
}

void draw_arrow()
{
    // plus
    GLUquadric *myQuad;
    // myQuad = gluNewQuadric();

    // // first plus
    // setLightColor(oxygen);
    // renderCylinder(pa_coords[0][0], pa_coords[0][1], pa_coords[0][2], pa_coords[1][0], pa_coords[1][1], pa_coords[1][2], 0.1, myQuad);
    // renderCylinder(pa_coords[2][0], pa_coords[2][1], pa_coords[2][2], pa_coords[3][0], pa_coords[3][1], pa_coords[3][2], 0.1, myQuad);

    // // second plus
    // setLightColor(oxygen);
    // renderCylinder(pa_coords[4][0], pa_coords[4][1], pa_coords[4][2], pa_coords[5][0], pa_coords[5][1], pa_coords[5][2], 0.1, myQuad);
    // renderCylinder(pa_coords[6][0], pa_coords[6][1], pa_coords[6][2], pa_coords[7][0], pa_coords[7][1], pa_coords[7][2], 0.1, myQuad);

    // arrow
    myQuad = gluNewQuadric();

    setLightColor(oxygen);
    renderCylinder(pa_coords[8][0], pa_coords[8][1], pa_coords[8][2], pa_coords[9][0], pa_coords[9][1], pa_coords[9][2], 0.1, myQuad);
    renderCylinder(pa_coords[11][0], pa_coords[11][1], pa_coords[11][2], pa_coords[9][0], pa_coords[9][1], pa_coords[9][2], 0.1, myQuad);
    renderCylinder(pa_coords[10][0], pa_coords[10][1], pa_coords[10][2], pa_coords[9][0], pa_coords[9][1], pa_coords[9][2], 0.1, myQuad);
}

void draw_plus()
{
    // plus
    GLUquadric *myQuad;
    myQuad = gluNewQuadric();

    // // first plus
    setLightColor(oxygen);
    renderCylinder(pa_coords[0][0], pa_coords[0][1], pa_coords[0][2], pa_coords[1][0], pa_coords[1][1], pa_coords[1][2], 0.1, myQuad);
    renderCylinder(pa_coords[2][0], pa_coords[2][1], pa_coords[2][2], pa_coords[3][0], pa_coords[3][1], pa_coords[3][2], 0.1, myQuad);

    // // second plus
    setLightColor(oxygen);
    renderCylinder(pa_coords[4][0], pa_coords[4][1], pa_coords[4][2], pa_coords[5][0], pa_coords[5][1], pa_coords[5][2], 0.1, myQuad);
    renderCylinder(pa_coords[6][0], pa_coords[6][1], pa_coords[6][2], pa_coords[7][0], pa_coords[7][1], pa_coords[7][2], 0.1, myQuad);

    // arrow
    // myQuad = gluNewQuadric();

    // setLightColor(oxygen);
    // renderCylinder(pa_coords[8][0], pa_coords[8][1], pa_coords[8][2], pa_coords[9][0], pa_coords[9][1], pa_coords[9][2], 0.1, myQuad);
    // renderCylinder(pa_coords[11][0], pa_coords[11][1], pa_coords[11][2], pa_coords[9][0], pa_coords[9][1], pa_coords[9][2], 0.1, myQuad);
    // renderCylinder(pa_coords[10][0], pa_coords[10][1], pa_coords[10][2], pa_coords[9][0], pa_coords[9][1], pa_coords[9][2], 0.1, myQuad);
}
// Function for drawing cyclopentane
void draw_NaHCO3(GLfloat center[3])
{
    for (int i = 0; i < 6; i++)
    {
        glPushMatrix();
        glTranslatef(NaHCO3_coords[i][0], NaHCO3_coords[i][1], NaHCO3_coords[i][2]);
        if (i == 0)
        {
            draw_atom(sodium);
        }
        else if (i == 2)
        {
            draw_atom(carbon);
        }
        else if (i == 4)
        {
            draw_atom(hydrogen);
        }
        else
        {
            draw_atom(oxygen);
        }
        glPopMatrix();
    }

    GLUquadric *myQuad;
    myQuad = gluNewQuadric();
    setLightColor(white);
    renderCylinder(NaHCO3_coords[0][0], NaHCO3_coords[0][1], NaHCO3_coords[0][2],
                   NaHCO3_coords[1][0], NaHCO3_coords[1][1], NaHCO3_coords[1][2],
                   cylinderRadius, myQuad);
    renderCylinder(NaHCO3_coords[1][0], NaHCO3_coords[1][1], NaHCO3_coords[1][2],
                   NaHCO3_coords[2][0], NaHCO3_coords[2][1], NaHCO3_coords[2][2],
                   cylinderRadius, myQuad);
    renderCylinder(NaHCO3_coords[2][0], NaHCO3_coords[2][1], NaHCO3_coords[2][2],
                   NaHCO3_coords[3][0], NaHCO3_coords[3][1], NaHCO3_coords[3][2],
                   cylinderRadius, myQuad);
    renderCylinder(NaHCO3_coords[3][0], NaHCO3_coords[3][1], NaHCO3_coords[3][2],
                   NaHCO3_coords[4][0], NaHCO3_coords[4][1], NaHCO3_coords[4][2],
                   cylinderRadius, myQuad);
    renderCylinder(NaHCO3_coords[2][0], NaHCO3_coords[2][1], NaHCO3_coords[2][2],
                   NaHCO3_coords[5][0], NaHCO3_coords[5][1], NaHCO3_coords[3][2],
                   cylinderRadius, myQuad);
}

void draw_Na2CO3(GLfloat center[3])
{
    for (int i = 0; i < 6; i++)
    {
        glPushMatrix();
        glTranslatef(Na2CO3_coords[i][0], Na2CO3_coords[i][1], Na2CO3_coords[i][2]);
        if (i == 0 || i == 5)
        {
            draw_atom(sodium);
        }
        else if (i == 2)
        {
            draw_atom(carbon);
        }
        else
        {
            draw_atom(oxygen);
        }
        glPopMatrix();
    }

    GLUquadric *myQuad;
    myQuad = gluNewQuadric();



    renderCylinder(Na2CO3_coords[1][0], Na2CO3_coords[1][1], Na2CO3_coords[1][2],
                   Na2CO3_coords[2][0], Na2CO3_coords[2][1], Na2CO3_coords[2][2],
                   cylinderRadius, myQuad);
    renderCylinder(Na2CO3_coords[2][0], Na2CO3_coords[2][1], Na2CO3_coords[2][2],
                   Na2CO3_coords[3][0], Na2CO3_coords[3][1], Na2CO3_coords[3][2],
                   cylinderRadius, myQuad);
    renderCylinder(Na2CO3_coords[2][0], Na2CO3_coords[2][1], Na2CO3_coords[2][2],
                   Na2CO3_coords[4][0], Na2CO3_coords[4][1], Na2CO3_coords[4][2],
                   cylinderRadius, myQuad);
}

// Function for drawing HBr
void draw_CO2(GLfloat center[3])
{
    for (int i = 0; i < 3; i++)
    {
        glPushMatrix();
        glTranslatef(CO2_coords[i][0], CO2_coords[i][1], CO2_coords[i][2]);
        if (i == 1)
        {
            draw_atom(carbon);
        }
        else
        {
            draw_atom(oxygen);
        }
        glPopMatrix();
    }

    GLUquadric *myQuad;
    myQuad = gluNewQuadric();

    setLightColor(white);
    renderCylinder(CO2_coords[0][0], CO2_coords[0][1], CO2_coords[0][2],
                   CO2_coords[1][0], CO2_coords[1][1], CO2_coords[1][2],
                   cylinderRadius, myQuad);
    renderCylinder(CO2_coords[1][0], CO2_coords[1][1], CO2_coords[1][2],
                   CO2_coords[2][0], CO2_coords[2][1], CO2_coords[2][2],
                   cylinderRadius, myQuad);
}

void draw_H2O(GLfloat center[3])
{
    for (int i = 0; i < 3; i++)
    {
        glPushMatrix();
        glTranslatef(H2O_coords[i][0], H2O_coords[i][1], H2O_coords[i][2]);
        if (i == 1)
        {
            draw_atom(hydrogen);
        }
        else
        {
            draw_atom(oxygen);
        }
        glPopMatrix();
    }

    GLUquadric *myQuad;
    myQuad = gluNewQuadric();

    setLightColor(white);
    renderCylinder(H2O_coords[0][0], H2O_coords[0][1], H2O_coords[0][2],
                   H2O_coords[1][0], H2O_coords[1][1], H2O_coords[1][2],
                   cylinderRadius, myQuad);
    renderCylinder(H2O_coords[1][0], H2O_coords[1][1], H2O_coords[1][2],
                   H2O_coords[2][0], H2O_coords[2][1], H2O_coords[2][2],
                   cylinderRadius, myQuad);
}

void draw_product_Na2CO3()
{

    for (int i = 0; i < 6; i++)
    {
        glPushMatrix();
        glTranslatef(product_Na2CO3_coords[i][0], product_Na2CO3_coords[i][1], product_Na2CO3_coords[i][2]);
        if (i == 0 || i == 5)
        {
            draw_atom(sodium);
        }
        else if (i == 1)
        {
            draw_atom(carbon);
        }
        else
        {
            draw_atom(oxygen);
        }
        glPopMatrix();
    }

    GLUquadric *myQuad;
    myQuad = gluNewQuadric();

    setLightColor(white);

    renderCylinder(product_Na2CO3_coords[2][0], product_Na2CO3_coords[2][1], product_Na2CO3_coords[1][2],
                   product_Na2CO3_coords[1][0], product_Na2CO3_coords[1][1], product_Na2CO3_coords[2][2],
                   cylinderRadius, myQuad);
    renderCylinder(product_Na2CO3_coords[1][0], product_Na2CO3_coords[1][1], product_Na2CO3_coords[2][2],
                   product_Na2CO3_coords[3][0], product_Na2CO3_coords[3][1], product_Na2CO3_coords[3][2],
                   cylinderRadius, myQuad);
    renderCylinder(product_Na2CO3_coords[1][0], product_Na2CO3_coords[1][1], product_Na2CO3_coords[2][2],
                   product_Na2CO3_coords[4][0], product_Na2CO3_coords[4][1], product_Na2CO3_coords[4][2],
                   cylinderRadius, myQuad);
}

void draw_product_CO2(){
     for (int i = 0; i < 3; i++)
    {
        glPushMatrix();
        glTranslatef(product_CO2_coords[i][0], product_CO2_coords[i][1], product_CO2_coords[i][2]);
        if (i == 1)
        {
            draw_atom(carbon);
        }
        else
        {
            draw_atom(oxygen);
        }
        glPopMatrix();
    }

    GLUquadric *myQuad;
    myQuad = gluNewQuadric();

    setLightColor(white);
    renderCylinder(product_CO2_coords[0][0], product_CO2_coords[0][1], product_CO2_coords[0][2],
                   product_CO2_coords[1][0], product_CO2_coords[1][1], product_CO2_coords[1][2],
                   cylinderRadius, myQuad);
    renderCylinder(product_CO2_coords[1][0], product_CO2_coords[1][1], product_CO2_coords[1][2],
                   product_CO2_coords[2][0], product_CO2_coords[2][1], product_CO2_coords[2][2],
                   cylinderRadius, myQuad);
}

void draw_product_H2O(){
    for (int i = 0; i < 3; i++)
    {
        glPushMatrix();
        glTranslatef(product_H2O_coords[i][0], product_H2O_coords[i][1], product_H2O_coords[i][2]);
        if (i == 1)
        {
            draw_atom(oxygen);
        }
        else
        {
            draw_atom(hydrogen);
        }
        glPopMatrix();
    }

    GLUquadric *myQuad;
    myQuad = gluNewQuadric();

    setLightColor(white);
    renderCylinder(product_H2O_coords[0][0], product_H2O_coords[0][1], product_H2O_coords[0][2],
                   product_H2O_coords[1][0], product_H2O_coords[1][1], product_H2O_coords[1][2],
                   cylinderRadius, myQuad);
    renderCylinder(product_H2O_coords[1][0], product_H2O_coords[1][1], product_H2O_coords[1][2],
                   product_H2O_coords[2][0], product_H2O_coords[2][1], product_H2O_coords[2][2],
                   cylinderRadius, myQuad);
}

// Function for translating molecules

void translate_xyz(std::string s, GLfloat dx, GLfloat dy, GLfloat dz)
{

    std::map<std::string, int> molecules = {{"NaHCO3", 0},
                                            {"Na2CO3", 1},
                                            {"CO2", 2},
                                            {"H2O", 3},
                                            {"pa", 4},
                                            {"Product_Na2CO3", 5},
                                            {"Product_CO2", 6},
                                            {"Product_H2O", 7},
                                            {"NaHCO3_orig", 8}};
    int ind = molecules[s];

    switch (ind)
    {
    case 0:
        for (int i = 0; i < 6; i++)
        {
            NaHCO3_coords[i][0] += dx;
            NaHCO3_coords[i][1] += dy;
            NaHCO3_coords[i][2] += dz;
        }
        break;
    case 1:
        for (int i = 0; i < 6; i++)
        {
            Na2CO3_coords[i][0] += dx;
            Na2CO3_coords[i][1] += dy;
            Na2CO3_coords[i][2] += dz;
        }
        break;
    case 2:
        for (int i = 0; i < 3; i++)
        {
            CO2_coords[i][0] += dx;
            CO2_coords[i][1] += dy;
            CO2_coords[i][2] += dz;
        }
        break;
    case 3:
        for (int i = 0; i < 3; i++)
        {
            H2O_coords[i][0] += dx;
            H2O_coords[i][1] += dy;
            H2O_coords[i][2] += dz;
        }
        break;
    case 4:
        for (int i = 0; i < 12; i++)
        {
            pa_coords[i][0] += dx;
            pa_coords[i][1] += dy;
            pa_coords[i][2] += dz;
        }
        break;
    case 5:
        for (int i = 0; i < 6; i++)
        {
            product_Na2CO3_coords[i][0] += dx;
            product_Na2CO3_coords[i][1] += dy;
            product_Na2CO3_coords[i][2] += dz;
        }
        break;
    case 6:
        for (int i = 0; i < 3; i++)
        {
            product_CO2_coords[i][0] += dx;
            product_CO2_coords[i][1] += dy;
            product_CO2_coords[i][2] += dz;
        }
        break;
     case 7:
        for (int i = 0; i < 3; i++)
        {
            product_H2O_coords[i][0] += dx;
            product_H2O_coords[i][1] += dy;
            product_H2O_coords[i][2] += dz;
        }
        break;
    case 8:
        for (int i = 0; i < 6; i++)
        {
            NaHCO3_orig_coords[i][0] += dx;
            NaHCO3_orig_coords[i][1] += dy;
            NaHCO3_orig_coords[i][2] += dz;
        }
        break;
    }
}

GLdouble width, height; /* window width and height */
int wd;                 /* GLUT window handle */

void init(void)
{
    width = 1280.0; /* initial window width and height, */
    height = 800.0; /* within which we draw. */
}

// Called when window is resized,
// also when window is first created,
// before the first call to display().
void reshape(int w, int h)
{
    /* save new screen dimensions */
    width = (GLdouble)w;
    height = (GLdouble)h;

    /* tell OpenGL to use the whole window for drawing */
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    /* do an orthographic parallel projection with the coordinate
     system set to first quadrant, limited by screen/window size */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, width*2, 0.0, height*2);

    aspect = width / height;
    glutPostRedisplay();
}

// Function for displaying animation of a reaction
void timer(int)
{
    glutPostRedisplay();
    glutTimerFunc(1000 / 120, timer, 0);

    if (is_rotate)
    {
        rot_angle += 2;
        if (rot_angle > 360)
            rot_angle -= 360;
    }

    if (animation)
    {
        if (time_cnt < t1)
        {
            translate_xyz("NaHCO3", 0.025, 0, 0);

            NaHCO3_coords[1][0] += 0.01; //left o x-axis
            NaHCO3_coords[1][1] -= 0.01; //left 0 y-axis
            NaHCO3_coords[3][0] -= 0.01; //right o x-axis
            NaHCO3_coords[3][1] -= 0.01; //right o y-xis
            NaHCO3_coords[5][1] += 0.01; //down o y-axis
            NaHCO3_coords[0][0] += 0.015; //left Na x-axis
            NaHCO3_coords[0][1] -= 0.01; //left Na y-axis
            NaHCO3_coords[4][0] -= 0.015; //right H x-axis
            NaHCO3_coords[4][1] -= 0.01; //right H y-axis

            // translate_xyz("HBr", -0.01, 0, 0);
            time_cnt++;
            // std::cout << time_cnt << "\n";

            // if (time_cnt > t1)
            // {
            //     for (int i = 0; i < 3; ++i)
            //     {
            //         color_t1[i] += delta_color;
            //         color_t2[i] -= delta_color;
            //     }
            //     HBrcylinderRadius -= delta_HBr;
            //     CBrcylinderRadius += delta_CBr;
            // }
        }

        if(assigned){
        if(time_cnt >= t1 && time_cnt<320){
            time_cnt++;
            translate_xyz("Product_Na2CO3", 0.01, 0, 0);

            cout<<product_Na2CO3_coords[0][0]<<" "<<NaHCO3_coords[0][0]<<endl;

            if(Na2CO3_coords[0][0] >  product_Na2CO3_coords[0][0]){
                product_Na2CO3_coords[0][0] -= 0.005;
            }

            if(Na2CO3_coords[2][1] > product_Na2CO3_coords[2][1]){
                product_Na2CO3_coords[2][1] += 0.01;
                product_Na2CO3_coords[2][0] -= 0.01;
            }

            if(Na2CO3_coords[3][1] > product_Na2CO3_coords[3][1]){
                product_Na2CO3_coords[3][1] += 0.01;
                product_Na2CO3_coords[3][0] += 0.01;
            }

            if(Na2CO3_coords[5][0] > product_Na2CO3_coords[5][0]){
                product_Na2CO3_coords[5][0] += 0.005;
            }

            if(Na2CO3_coords[4][1] < product_Na2CO3_coords[4][1]){
                product_Na2CO3_coords[4][1] -= 0.01;
            }

            translate_xyz("Product_CO2", 0.05, 0, 0);

            if(product_CO2_coords[0][1] < CO2_coords[0][1]){
                product_CO2_coords[0][1] += 0.025;
            }

            if(product_CO2_coords[2][1] > CO2_coords[2][1]){
                product_CO2_coords[2][1] -= 0.025;
            }

            translate_xyz("Product_H2O", 0.03, 0, 0);

            if(product_H2O_coords[0][0] < H2O_coords[0][0]){
                product_H2O_coords[0][0] -= 0.0070;
                product_H2O_coords[0][1] -= 0.0060;
            }

            if(product_H2O_coords[2][0] < H2O_coords[2][0]){
                product_H2O_coords[2][0] += 0.004;
                product_H2O_coords[2][1] -= 0.0045;
            }
        }
        }

        // if (merging)
        // {
        //     if (time_cnt < t3)
        //     {
        //         time_cnt++;
        //         // std::cout << time_cnt << "\n";
        //         for (int i = 0; i < 7; ++i)
        //         {
        //             for (int j = 0; j < 3; ++j)
        //             {
        //                 product_coords[i][j] = product_coords[i][j] += deltas[i][j];
        //             }
        //         }
        //     }
        //     else if (time_cnt == t3 && calc)
        //     {
        //         calc = 0;
        //         for (int i = 0; i < 6; ++i)
        //         {
        //             for (int j = 0; j < 3; ++j)
        //             {
        //                 cyclopentane_coords[i][j] = cyclopentane_orig_coords[i][j];
        //             }
        //         }

        //         for (int i = 0; i < 2; ++i)
        //         {
        //             for (int j = 0; j < 3; ++j)
        //             {
        //                 HBr_coords[i][j] = HBr_orig_coords[i][j];
        //             }
        //         }
        //     }
        // }
    }
}


int main(int argc, char *argv[])
{
    std::cout << "Menu: \n";
    std::cout << "1. Enter : Start/Pause animation \n";
    std::cout << "2. Space : Start/Pause rotation \n";
    std::cout << "3. R     : Reset animation \n";
    std::cout << "4. Esc   : Close Window \n";
    std::cout << "5. A/D   : Translate along X-axis \n";
    std::cout << "6. S/W   : Translate along Y-axis \n";
    std::cout << "7. 5/8   : Translate along Z-axis \n";
    std::cout << "8. +     : Add/Remove Axis \n";
    std::cout << "9. P     : Change Projection\n";
    std::cout << "10. C    : Change Camera \n";
    std::cout << "11. L    : Change Light Effects \n";
    std::cout << "12. M    : Toggle Legend \n";
    std::cout << "13. Mouse: Left Click and Drag : Rotate Axis \n";
    std::cout << "14. Mouse: Hold Scroll and Drag: Zoom In/Out \n";

    /* perform initialization NOT OpenGL/GLUT dependent,
     as we haven't created a GLUT window yet */
    init();

    /* initialize GLUT, let it extract command-line
     GLUT options that you may provide
     - NOTE THE '&' BEFORE argc */
    glutInit(&argc, argv);

    /* specify the display to be single
     buffered and color as RGBA values */
    glutInitDisplayMode(GLUT_DEPTH);

    /* set the initial window size */
    glutInitWindowSize((int)width, (int)height);
    wd = glutCreateWindow("NaHCO3 -> Na2CO3 + H2O + CO2" /* title */);
    LoadGLTextures();

    /* --- register callbacks with GLUT --- */

    /* register function to handle window resizes */
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);
    setLightColor(white);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glClearColor(0.0, 0.0, 0.0, 0.0);

    /* register function that draws in the window */
    glutDisplayFunc(displayCallback);
    // motion
    glutMouseFunc(mouseCallback);
    glutMotionFunc(motionCallback);
    // keyboard functions
    glutKeyboardFunc(keyboardCallback);
    /* start the GLUT main loop */
    glutMainLoop();
    return 0;
}

// Function for drawing sphere
void draw_atom(GLfloat color[3])
{

    glColor3fv(color);

    setLightColor(color);

    GLUquadric *myQuad;
    myQuad = gluNewQuadric();

    int textureIndex = 0;

    if (color == hydrogen)
    {
        textureIndex = 3;
    }
    else if (color == oxygen)
    {
        textureIndex = 4;
    }
    else if (color == sodium)
    {
        textureIndex = 1;
    }
    else if (color == carbon)
    {
        textureIndex = 2;
    }

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[textureIndex]);
    gluQuadricTexture(myQuad, GL_TRUE);

    // creating the sphere
    glPushMatrix();
    glRotatef(90, -1, 0, 0);
    gluSphere(myQuad, sphereRadius, slices, stacks);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}

// Function for displaying different illuminations
void setLightColor(GLfloat light_color[3])
{
    // Light Options

    // default
    if (lightOption == 1)
    {
        GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
        GLfloat shine[] = {100.0};

        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_color);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
        glMaterialfv(GL_FRONT, GL_SHININESS, shine);
    }
    // from +z axiz and its own light
    else if (lightOption == 2)
    {
        GLfloat light_ambient[] = {0.0, 0.0, 0.0, 1.0};
        GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
        GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
        GLfloat light_position[] = {1.0, 0.0, 1.0, 0.0};

        glLightfv(GL_LIGHT0, GL_AMBIENT, light_color);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_color);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    }
    // from +z axis
    else if (lightOption == 3)
    {
        GLfloat light_ambient[] = {0.0, 0.0, 0.0, 1.0};
        GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
        GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
        GLfloat light_position[] = {0.0, 0.0, 1.0, 0.0};

        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_color);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    }
    // from +x axis
    else if (lightOption == 4)
    {
        GLfloat light_ambient[] = {0.0, 0.0, 0.0, 1.0};
        GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
        GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
        GLfloat light_position[] = {1.0, 0.0, 0.0, 0.0};

        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_color);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    }
    // from +y axis
    else if (lightOption == 5)
    {
        GLfloat light_ambient[] = {0.0, 0.0, 0.0, 1.0};
        GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
        GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
        GLfloat light_position[] = {0.0, 1.0, 0.0, 0.0};

        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_color);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    }
    // random position
    else
    {
        GLfloat light_ambient[] = {0.0, 0.0, 0.0, 1.0};
        GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
        GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
        GLfloat light_position[] = {1.0, 5.0, -6.0, 0.0};

        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_color);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    }
}

// Function for drawing cylinder(bond)
void renderCylinder(float x1, float y1, float z1, float x2, float y2, float z2, float radius, GLUquadricObj *quadric)
{
    if (z2 < z1)
    {
        std::swap(x1, x2);
        std::swap(y1, y2);
        std::swap(z1, z2);
    }

    float vx = x2 - x1;
    float vy = y2 - y1;
    float vz = z2 - z1;
    float v = sqrt(vx * vx + vy * vy + vz * vz);
    float ax;

    if (fabs(vz) < 1.0e-3)
    {
        ax = 57.2957795 * acos(vx / v); // rotation angle in x-y plane
        if (vy <= 0.0)
            ax = -ax;
    }
    else
    {
        ax = 57.2957795 * acos(vz / v); // rotation angle
        if (vz <= 0.0)
            ax = -ax;
    }

    float rx = -vy * vz;
    float ry = vx * vz;

    glPushMatrix();
    // draw the cylinder body
    glTranslatef(x1, y1, z1);
    if (fabs(vz) < 1.0e-3)
    {
        if (vx < 0.0)
        {
            glRotated(90.0, 0, 1, 0.0);
            glRotated(ax, -1.0, 0.0, 0.0);
        }
        else
        {
            glRotated(90.0, 0, 1, 0.0);    // Rotate & align with x axis
            glRotated(ax, -1.0, 0.0, 0.0); // Rotate to point 2 in x-y plane
        }
    }
    else
    {
        glRotated(ax, rx, ry, 0.0); // Rotate about rotation vector
    }
    gluQuadricOrientation(quadric, GLU_OUTSIDE);
    gluCylinder(quadric, radius, radius, v, slices, stacks);
    glPopMatrix();
}

// Function for drawing axis
void drawAxis()
{

    float originAxis[3] = {-4, -4, -4};
    float xAxis[3] = {-2, -4, -4};
    float yAxis[3] = {-4, -2, -4};
    float zAxis[3] = {-4, -4, -2};

    glDisable(GL_LIGHTING);
    glPushMatrix();
    glLineWidth(10.0);

    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3fv(originAxis);
    glVertex3fv(xAxis);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3fv(originAxis);
    glVertex3fv(yAxis);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3fv(originAxis);
    glVertex3fv(zAxis);
    glEnd();

    glPopMatrix();
    glEnable(GL_LIGHTING);
}

/* Function for mouse callback functions */
void mouseCallback(int button, int state, int x, int y)
{
    downX = x;
    downY = y;
    leftButton = ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN));
    middleButton = ((button == GLUT_MIDDLE_BUTTON) && (state == GLUT_DOWN));
}

void motionCallback(int x, int y)
{
    if (leftButton) // Rotate
    {
        phi += (x - downX) / 4.0;
        theta += (downY - y) / 4.0;
    }
    if (middleButton) // Scale
    {
        if (depth + (downY - y) / 10.0 < zFar - 10 && depth + (downY - y) / 10.0 > zNear + 3)
            depth += (downY - y) / 10.0;
    }
    downX = x;
    downY = y;

    glutPostRedisplay();
}

// Function for keyboard callback functions
void keyboardCallback(unsigned char key, int x, int y)
{
    GLfloat delta = 0.25;

    if (key == 'a' || key == 'A')
    {
        translate_xyz("cyclopentane", -delta, 0, 0);
        translate_xyz("HBr", -delta, 0, 0);
        translate_xyz("bromocyclopentane", -delta, 0, 0);
        translate_xyz("PA", -delta, 0, 0);
        translate_xyz("Product", -delta, 0, 0);
        translate_xyz("cyclopentane_orig", -delta, 0, 0);
        translate_xyz("HBr_orig", -delta, 0, 0);
    }
    if (key == 'w' || key == 'W')
    {
        translate_xyz("cyclopentane", 0, delta, 0);
        translate_xyz("HBr", 0, delta, 0);
        translate_xyz("bromocyclopentane", 0, delta, 0);
        translate_xyz("PA", 0, delta, 0);
        translate_xyz("Product", 0, delta, 0);
        translate_xyz("cyclopentane_orig", 0, delta, 0);
        translate_xyz("HBr_orig", 0, delta, 0);
    }
    if (key == 's' || key == 'S')
    {
        translate_xyz("cyclopentane", 0, -delta, 0);
        translate_xyz("HBr", 0, -delta, 0);
        translate_xyz("bromocyclopentane", 0, -delta, 0);
        translate_xyz("PA", 0, -delta, 0);
        translate_xyz("Product", 0, -delta, 0);
        translate_xyz("cyclopentane_orig", 0, -delta, 0);
        translate_xyz("HBr_orig", 0, -delta, 0);
    }
    if (key == 'd' || key == 'D')
    {
        translate_xyz("cyclopentane", delta, 0, 0);
        translate_xyz("HBr", delta, 0, 0);
        translate_xyz("bromocyclopentane", delta, 0, 0);
        translate_xyz("PA", delta, 0, 0);
        translate_xyz("Product", delta, 0, 0);
        translate_xyz("cyclopentane_orig", delta, 0, 0);
        translate_xyz("HBr_orig", delta, 0, 0);
    }
    if (key == '8')
    {
        translate_xyz("cyclopentane", 0, 0, delta);
        translate_xyz("HBr", 0, 0, delta);
        translate_xyz("bromocyclopentane", 0, 0, delta);
        translate_xyz("PA", 0, 0, delta);
        translate_xyz("Product", 0, 0, delta);
        translate_xyz("cyclopentane_orig", 0, 0, delta);
        translate_xyz("HBr_orig", 0, 0, delta);
    }
    if (key == '5')
    {
        translate_xyz("cyclopentane", 0, 0, -delta);
        translate_xyz("HBr", 0, 0, -delta);
        translate_xyz("bromocyclopentane", 0, 0, -delta);
        translate_xyz("PA", 0, 0, -delta);
        translate_xyz("Product", 0, 0, -delta);
        translate_xyz("cyclopentane_orig", 0, 0, -delta);
        translate_xyz("HBr_orig", 0, 0, -delta);
    }

    if (key == 'c' || key == 'C')
    {
        camera = 1 + camera;
        if (camera == 6)
            camera = 1;
    }
    if (key == 'M' || key == 'm')
    {
        drawLegend = 1 - drawLegend;
    }
    if (key == '+')
    {
        drawThatAxis = 1 - drawThatAxis;
    }

    if (key == '-')
    {
        lightEffect = 1 - lightEffect;
    }

    if (key == 'R' || key == 'r')
    {
        time_cnt = 0;
        animation = 0;
        merging = 0;
        merge_cnt = 0;
        to_print = 1;
        calc = 1;
        CBrcylinderRadius = 0;
        HBrcylinderRadius = 0.2;

        for (int i = 0; i < 3; ++i)
        {
            color_t1[i] = 0;
            color_t2[i] = 1;
        }

        for (int i = 0; i < 6; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                NaHCO3_coords[i][j] = NaHCO3_orig_coords[i][j];
            }
        }

        for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                // HBr_coords[i][j] = HBr_orig_coords[i][j];
            }
        }
    }

    if (key == 'l' || key == 'L')
    {

        if (lightOption == 6)
            lightOption = 1;
        else
            lightOption++;
    }

    if (key == 'p' || key == 'P')
    {
        projection++;
        if (projection == 5)
            projection = 1;
    }

    if (key == 13)
    {
        animation = 1 - animation;
        std::cout << "Timer : " << time_cnt << std::endl;
    }

    if (key == 32)
    {
        is_rotate = 1 - is_rotate;
    }
    if (key == 27)
    {
        exit(0);
    }

    glutPostRedisplay();
}

// Main displaying function
void displayCallback(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Perspective projection
    if (projection == 1)
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(fovy, aspect, zNear, zFar);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Point source of perspective projection
        if (camera == 1)
        {
            gluLookAt(0, 0, 2, 0, 0, 0, 0, 1, 0);
        }
        else if (camera == 2)
        {
            gluLookAt(0, 0.8, 2, 0, 0, 0, 0, 1, 0);
        }
        else if (camera == 3)
        {
            gluLookAt(0, -0.8, 2, 0, 0, 0, 0, 1, 0);
        }
        else if (camera == 4)
        {
            gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);
        }
        else if (camera == 5)
        {
        }
        else
        {
            exit(0);
        }

        // Motion Options
        glTranslatef(0.0, 0.0, -depth);
        glRotatef(-theta, 1.0, 0.0, 0.0);
        glRotatef(phi, 0.0, 1.0, 0.0);
    }

    else if (projection == 2) // Isometric
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        double ratio = (double)width / (double)height;
        int w = 7;

        glOrtho(-w * ratio, w * ratio, -w, w, -10, 20);
        // glOrtho(-7.0f, 7.0f, -7.0f, 7.0f, -7.0f, 15.0f);

        glTranslatef(0.0, 0.0, -depth);
        glRotatef(-theta, 1.0, 0.0, 0.0);
        glRotatef(phi, 0.0, 1.0, 0.0);

        glRotatef(35.264f, 1.0f, 0.0f, 0.0f);
        glRotatef(-45.0f, 0.0f, 1.0f, 0.0f);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

    else if (projection == 3) // Cavalier
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        double alpha_d = 45;
        double alpha = alpha * 3.14159 / 180;
        double cavalier[] = {
            1, 0, -1 * cos(alpha), 0,
            0, 1, -1 * sin(alpha), 0,
            0, 0, 1, 0,
            0, 0, 0, 1};

        double ratio = (double)width / (double)height;
        int w = 7;

        glOrtho(-w * ratio, w * ratio, -w, w, -10, 20);

        glTranslatef(0.0, 0.0, -depth);
        glRotatef(-theta, 1.0, 0.0, 0.0);
        glRotatef(phi, 0.0, 1.0, 0.0);

        glMultTransposeMatrixd(cavalier);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

    else if (projection == 4) // Cabinet
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        double alpha_d = 63.4;
        double alpha = alpha * 3.14159 / 180;
        double cabinet[] = {
            1, 0, 0.5 * cos(alpha), 0,
            0, 1, 0.5 * sin(alpha), 0,
            0, 0, 1, 0,
            0, 0, 0, 1};

        double ratio = (double)width / (double)height;
        int w = 7;

        glOrtho(-w * ratio, w * ratio, -w, w, -10, 20);

        glTranslatef(0.0, 0.0, -depth);
        glRotatef(-theta, 1.0, 0.0, 0.0);
        glRotatef(phi, 0.0, 1.0, 0.0);

        glMultTransposeMatrixd(cabinet);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

    display_projection();
    // Axis x, y and z Toggle
    if (drawThatAxis)
    {
        drawAxis();
    }

    // Light Effect Toggle :)
    if (lightEffect)
    {
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
    }
    else
    {
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
    }
    if (drawLegend)
    {
        glPushMatrix();
        glRotatef(90, 0, 1, 0);
        displayLegend();
        glPopMatrix();
    }

    GLfloat all_center[3] = {0, 0, 0};

    if (time_cnt == 0)
    {

        glPushMatrix();
        glTranslatef((NaHCO3_coords[0][0] + NaHCO3_coords[2][0]) / 2, (NaHCO3_coords[0][1] + NaHCO3_coords[2][1]) / 2, (NaHCO3_coords[0][2] + NaHCO3_coords[2][2]) / 2);
        glRotatef(rot_angle, -1, -1, -1);
        glTranslatef(-(NaHCO3_coords[0][0] + NaHCO3_coords[2][0]) / 2, -(NaHCO3_coords[0][1] + NaHCO3_coords[2][1]) / 2,
                     -(NaHCO3_coords[0][2] + NaHCO3_coords[2][2]) / 2);
        draw_NaHCO3(all_center);
        glPopMatrix();

        draw_arrow();
        glPopMatrix();
    }

    if (time_cnt < t1)
    {
        // cout<<time_cnt<<endl;
        // displayLegend();
        glPushMatrix();
        glTranslatef((NaHCO3_coords[0][0] + NaHCO3_coords[2][0]) / 2, (NaHCO3_coords[0][1] + NaHCO3_coords[2][1]) / 2, (NaHCO3_coords[0][2] + NaHCO3_coords[2][2]) / 2);
        glRotatef(rot_angle, -1, -1, -1);
        glTranslatef(-(NaHCO3_coords[0][0] + NaHCO3_coords[2][0]) / 2, -(NaHCO3_coords[0][1] + NaHCO3_coords[2][1]) / 2,
                     -(NaHCO3_coords[0][2] + NaHCO3_coords[2][2]) / 2);
        draw_NaHCO3(all_center);
        glPopMatrix();

    }

    if(time_cnt == t1){
        
        product_Na2CO3_coords[0][0] = NaHCO3_coords[0][0]; 
        product_Na2CO3_coords[0][1] = NaHCO3_coords[0][1]; 

        product_Na2CO3_coords[1][0] = NaHCO3_coords[2][0]; 
        product_Na2CO3_coords[1][1] = NaHCO3_coords[2][1]; 

        product_Na2CO3_coords[2][0] = NaHCO3_coords[1][0]; 
        product_Na2CO3_coords[2][1] = NaHCO3_coords[1][1]; 

        product_Na2CO3_coords[3][0] = NaHCO3_coords[3][0]; 
        product_Na2CO3_coords[3][1] = NaHCO3_coords[3][1];

        product_Na2CO3_coords[4][0] = NaHCO3_coords[5][0]; 
        product_Na2CO3_coords[4][1] = NaHCO3_coords[5][1];

        product_Na2CO3_coords[5][0] = NaHCO3_coords[4][0]; 
        product_Na2CO3_coords[5][1] = NaHCO3_coords[4][1];

        //co2
        product_CO2_coords[0][0] = NaHCO3_coords[5][0];
        product_CO2_coords[0][1] = NaHCO3_coords[5][1];

        product_CO2_coords[1][0] = NaHCO3_coords[2][0];
        product_CO2_coords[1][1] = NaHCO3_coords[2][1];

        product_CO2_coords[2][0] = NaHCO3_coords[5][0];
        product_CO2_coords[2][1] = NaHCO3_coords[5][1];

        //h20
        product_H2O_coords[0][0] = NaHCO3_coords[4][0];
        product_H2O_coords[0][1] = NaHCO3_coords[4][1];

        product_H2O_coords[1][0] = NaHCO3_coords[3][0];
        product_H2O_coords[1][1] = NaHCO3_coords[3][1];

        product_H2O_coords[2][0] = NaHCO3_coords[4][0];
        product_H2O_coords[2][1] = NaHCO3_coords[4][1];

        assigned = true;
    }

    if (time_cnt > t1)
    {

        glPushMatrix();
        glTranslatef((product_Na2CO3_coords[0][0] + product_Na2CO3_coords[2][0]) / 2, (product_Na2CO3_coords[0][1] + product_Na2CO3_coords[2][1]) / 2, (product_Na2CO3_coords[0][2] + product_Na2CO3_coords[2][2]) / 2);
        glRotatef(rot_angle, -1, -1, -1);
        glTranslatef(-(product_Na2CO3_coords[0][0] + product_Na2CO3_coords[2][0]) / 2, -(product_Na2CO3_coords[0][1] + product_Na2CO3_coords[2][1]) / 2,
                     -(product_Na2CO3_coords[0][2] + product_Na2CO3_coords[2][2]) / 2);
        draw_product_Na2CO3();
        glPopMatrix();

        glPushMatrix();
        glTranslatef((product_CO2_coords[0][0] + product_CO2_coords[2][0]) / 2, (product_CO2_coords[0][1] + product_CO2_coords[2][1]) / 2, (product_CO2_coords[0][2] + product_CO2_coords[2][2]) / 2);
        glRotatef(rot_angle, -1, -1, -1);
        glTranslatef(-(product_CO2_coords[0][0] + product_CO2_coords[2][0]) / 2, -(product_CO2_coords[0][1] + product_CO2_coords[2][1]) / 2,
                     -(product_CO2_coords[0][2] + product_CO2_coords[2][2]) / 2);
        draw_product_CO2();
        glPopMatrix();

        glPushMatrix();
        glTranslatef((product_H2O_coords[0][0] + product_H2O_coords[2][0]) / 2, (product_H2O_coords[0][1] + product_H2O_coords[2][1]) / 2, (product_H2O_coords[0][2] + product_H2O_coords[2][2]) / 2);
        glRotatef(rot_angle, -1, -1, -1);
        glTranslatef(-(product_H2O_coords[0][0] + product_H2O_coords[2][0]) / 2, -(product_H2O_coords[0][1] + product_H2O_coords[2][1]) / 2,
                     -(product_H2O_coords[0][2] + product_H2O_coords[2][2]) / 2);
        draw_product_H2O();
        glPopMatrix();

       for (int i = 0; i < 6; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                NaHCO3_coords[i][j] = NaHCO3_orig_coords[i][j];
            }
        }

        // draw_plus();
    }

    // // int stop_plus = 120;
    if (time_cnt < 40)
    {
        // displayLegend();
        draw_arrow();
    }

    
    if (time_cnt >= 320)
    {
        // displayLegend();

        glPushMatrix();
        glTranslatef((product_Na2CO3_coords[0][0] + product_Na2CO3_coords[2][0]) / 2, (product_Na2CO3_coords[0][1] + product_Na2CO3_coords[2][1]) / 2, (product_Na2CO3_coords[0][2] + product_Na2CO3_coords[2][2]) / 2);
        glRotatef(rot_angle, -1, -1, -1);
        glTranslatef(-(product_Na2CO3_coords[0][0] + product_Na2CO3_coords[2][0]) / 2, -(product_Na2CO3_coords[0][1] + product_Na2CO3_coords[2][1]) / 2,
                     -(product_Na2CO3_coords[0][2] + product_Na2CO3_coords[2][2]) / 2);
        draw_product_Na2CO3();
        glPopMatrix();

        glPushMatrix();
        glTranslatef((product_CO2_coords[0][0] + product_CO2_coords[2][0]) / 2, (product_CO2_coords[0][1] + product_CO2_coords[2][1]) / 2, (product_CO2_coords[0][2] + product_CO2_coords[2][2]) / 2);
        glRotatef(rot_angle, -1, -1, -1);
        glTranslatef(-(product_CO2_coords[0][0] + product_CO2_coords[2][0]) / 2, -(product_CO2_coords[0][1] + product_CO2_coords[2][1]) / 2,
                     -(product_CO2_coords[0][2] + product_CO2_coords[2][2]) / 2);
        draw_product_CO2();
        glPopMatrix();

        glPushMatrix();
        glTranslatef((product_H2O_coords[0][0] + product_H2O_coords[2][0]) / 2, (product_H2O_coords[0][1] + product_H2O_coords[2][1]) / 2, (product_H2O_coords[0][2] + product_H2O_coords[2][2]) / 2);
        glRotatef(rot_angle, -1, -1, -1);
        glTranslatef(-(product_H2O_coords[0][0] + product_H2O_coords[2][0]) / 2, -(product_H2O_coords[0][1] + product_H2O_coords[2][1]) / 2,
                     -(product_H2O_coords[0][2] + product_H2O_coords[2][2]) / 2);
        draw_product_H2O();
        glPopMatrix();

        draw_plus_arrow();
        glPopMatrix();

        glPushMatrix();
        glTranslatef((NaHCO3_coords[0][0] + NaHCO3_coords[2][0]) / 2, (NaHCO3_coords[0][1] + NaHCO3_coords[2][1]) / 2, (NaHCO3_coords[0][2] + NaHCO3_coords[2][2]) / 2);
        glRotatef(rot_angle, -1, -1, -1);
        glTranslatef(-(NaHCO3_coords[0][0] + NaHCO3_coords[2][0]) / 2, -(NaHCO3_coords[0][1] + NaHCO3_coords[2][1]) / 2,
                     -(NaHCO3_coords[0][2] + NaHCO3_coords[2][2]) / 2);
        draw_NaHCO3(all_center);
        glPopMatrix();

    }

    

    glFlush();
}