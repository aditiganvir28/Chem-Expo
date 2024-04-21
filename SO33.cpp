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

GLuint texture[7];
int LoadGLTextures()
{
    texture[2] = SOIL_load_OGL_texture("img/H-final.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    texture[3] = SOIL_load_OGL_texture("img/O-final.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    texture[4] = SOIL_load_OGL_texture("img/S-final.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

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

// Global coordinates for SO2
GLfloat SO2_coords[3][3] = {
    {-6.25, 0, 0},             // sulfur
    {-7.25, -1.299038106, 0},  // left oxygen
    {-5.25, -1.299038106, 0}}; // right oxygen

// Global coordinates for O2
GLfloat O2_coords[2][3] = {
    {-1, 1.299038106, 0},  // left oxygen
    {0, 0, 0}, // right oxygen
};

// Global coordinates for SO3
GLfloat SO3_coords[4][3] = {
    {4.75, 0, 0},            // sulfur
    {3.75, -1.299038106, 0}, // left down oxygen
    {5.75, -1.299038106, 0}, // right down oxygen
    {4.75, +1.299038106, 0}  // up oxygen
};

GLfloat product_coords[4][3] = {
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}};

GLfloat deltas[4][3] = {
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}};

GLfloat pa_coords[8][3] = {
    {-3, 0, 0},
    {-2, 0, 0},
    {-2.5, -0.5, 0},
    {-2.5, 0.5, 0},
    {1.5, 0, 0},
    {2.5, 0, 0},
    {2.146446609, 0.353553391, 0},
    {2.146446609, -0.353553391, 0}};

GLfloat SO2_orig_coords[6][3] = {
    {-6.25, 0, 0},             // sulfur
    {-7.25, -1.299038106, 0},  // left oxygen
    {-5.25, -1.299038106, 0}}; // right oxygen

GLfloat O2_orig_coords[2][3] = {
    {-1, 1.299038106, 0},  // left oxygen
    {0, 0, 0}, // right oxygen
};

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
int t1 = 140, t2 = 197, t3 = 600;

// Animation variables
int camera = 1;
int animation = 0, merging = 0, merge_cnt = 0, merge_pause = 20, to_print = 1, calc = 1, is_rotate = 0;
GLdouble rot_angle = 0;
GLdouble SOcylinderRadius = 0;
GLdouble O2cylinderRadius = 0.2;
GLdouble delta_O2 = cylinderRadius / (t2 - t1);
GLdouble delta_CBr = delta_O2;

// Viewer options (GluLookAt)
float fovy = 60.0, aspect = 1.0, zNear = 1.0, zFar = 100.0;

// Mouse modifiers
float depth = 8;
float phi = 0, theta = 0;
float downX, downY;
bool leftButton = false, middleButton = false;

// colors
GLfloat oxygen[3] = {1.0, 0.0, 0.0}; // (O - Red)
GLfloat sulphur[3] = {1, 1, 0};      //  (S - Yellow)
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

    string info_1 = "Reaction: Synthesis reaction";
    string info_2 = "Sulfur Dioxide + Oxygen -> Sulfur Trioxide";
    string info_3 = "This reaction plays a vital role in the production of sulfuric acid";

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

    string atom_1 = "Symbol     Name      Number        Mass";
    string atom_2 = "  S       Sulfur        16          32.065";
    string atom_3 = "  O       Oxygen        8           15.999";
   
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

    setLightColor(oxygen);
    renderCylinder(pa_coords[0][0], pa_coords[0][1], pa_coords[0][2], pa_coords[1][0], pa_coords[1][1], pa_coords[1][2], 0.1, myQuad);
    renderCylinder(pa_coords[2][0], pa_coords[2][1], pa_coords[2][2], pa_coords[3][0], pa_coords[3][1], pa_coords[3][2], 0.1, myQuad);

    // arrow
    myQuad = gluNewQuadric();

    setLightColor(oxygen);
    renderCylinder(pa_coords[4][0], pa_coords[4][1], pa_coords[4][2], pa_coords[5][0], pa_coords[5][1], pa_coords[5][2], 0.1, myQuad);
    renderCylinder(pa_coords[6][0], pa_coords[6][1], pa_coords[6][2], pa_coords[5][0], pa_coords[5][1], pa_coords[5][2], 0.1, myQuad);
    renderCylinder(pa_coords[7][0], pa_coords[7][1], pa_coords[7][2], pa_coords[5][0], pa_coords[5][1], pa_coords[5][2], 0.1, myQuad);
}

// Function for drawing SO2
void draw_SO2(GLfloat center[3])
{
    for (int i = 0; i < 3; i++)
    {
        glPushMatrix();
        glTranslatef(SO2_coords[i][0], SO2_coords[i][1], SO2_coords[i][2]);
        if (i == 0)
        {
            draw_atom(sulphur);
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

    renderCylinder(SO2_coords[1][0], SO2_coords[1][1], SO2_coords[1][2],
                   SO2_coords[0][0], SO2_coords[0][1], SO2_coords[0][2],
                   cylinderRadius, myQuad);

    renderCylinder(SO2_coords[0][0], SO2_coords[0][1], SO2_coords[0][2],
                   SO2_coords[2][0], SO2_coords[2][1], SO2_coords[2][2],
                   cylinderRadius, myQuad);
}

// Function for drawing O2
void draw_O2(GLfloat center[3])
{
    for (int i = 0; i < 2; i++)
    {
        glPushMatrix();
        glTranslatef(O2_coords[i][0], O2_coords[i][1], O2_coords[i][2]);
        draw_atom(oxygen);
        glPopMatrix();
    }

    GLUquadric *myQuad;
    myQuad = gluNewQuadric();

    setLightColor(white);

    renderCylinder(O2_coords[0][0], O2_coords[0][1], O2_coords[0][2],
                       O2_coords[1][0], O2_coords[1][1], O2_coords[1][2],
                       O2cylinderRadius, myQuad);
    renderCylinder(O2_coords[0][0], O2_coords[0][1], O2_coords[0][2],
                   SO2_coords[0][0], SO2_coords[0][1], SO2_coords[0][2],
                   SOcylinderRadius, myQuad);
}

// Function for drawing SO3
void draw_SO3(GLfloat center[3])
{
    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
        glTranslatef(SO3_coords[i][0], SO3_coords[i][1], SO3_coords[i][2]);
        if(i==0){
            draw_atom(sulphur);
        }
        else{
            draw_atom(oxygen);
        }
        glPopMatrix();
    }

    GLUquadric *myQuad;
    myQuad = gluNewQuadric();

    setLightColor(white);

    renderCylinder(SO3_coords[1][0], SO3_coords[1][1], SO3_coords[1][2],
                   SO3_coords[0][0], SO3_coords[0][1], SO3_coords[0][2],
                   cylinderRadius, myQuad);
    renderCylinder(SO3_coords[0][0], SO3_coords[0][1], SO3_coords[0][2],
                   SO3_coords[2][0], SO3_coords[2][1], SO3_coords[2][2],
                   cylinderRadius, myQuad);
    renderCylinder(SO3_coords[3][0], SO3_coords[3][1], SO3_coords[3][2],
                   SO3_coords[3][0], SO3_coords[0][1], SO3_coords[0][2],
                   cylinderRadius, myQuad);
}

void draw_product()
{

    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
        glTranslatef(product_coords[i][0], product_coords[i][1], product_coords[i][2]);
        if(i==0){
            draw_atom(sulphur);
        }
        else{
            draw_atom(oxygen);
        }
        glPopMatrix();
    }

    GLUquadric *myQuad;
    myQuad = gluNewQuadric();

    setLightColor(white);

    renderCylinder(product_coords[1][0], product_coords[1][1], product_coords[1][2],
                   product_coords[0][0], product_coords[0][1], product_coords[0][2],
                   cylinderRadius, myQuad);
    renderCylinder(product_coords[0][0], product_coords[0][1], product_coords[0][2],
                   product_coords[2][0], product_coords[2][1], product_coords[2][2],
                   cylinderRadius, myQuad);
    renderCylinder(product_coords[3][0], product_coords[3][1], product_coords[3][2],
                   product_coords[0][0], product_coords[0][1], product_coords[0][2],
                   cylinderRadius, myQuad);
}

// Function for translating molecules
void translate_xyz(std::string s, GLfloat dx, GLfloat dy, GLfloat dz)
{

    std::map<std::string, int> molecules = {{"SO2", 0},
                                            {"O2", 1},
                                            {"SO3", 2},
                                            {"PA", 3},
                                            {"Product", 4},
                                            {"SO2_orig", 5},
                                            {"O2_orig", 6}};

    int ind = molecules[s];

    switch (ind)
    {
    case 0:
        for (int i = 0; i < 3; i++)
        {
            SO2_coords[i][0] += dx;
            SO2_coords[i][1] += dy;
            SO2_coords[i][2] += dz;
        }
        break;
    case 1:
        for (int i = 0; i < 2; i++)
        {
            O2_coords[i][0] += dx;
            O2_coords[i][1] += dy;
            O2_coords[i][2] += dz;
        }
        break;
    case 2:
        for (int i = 0; i < 4; i++)
        {
            SO3_coords[i][0] += dx;
            SO3_coords[i][1] += dy;
            SO3_coords[i][2] += dz;
        }
        break;
    case 3:
        for (int i = 0; i < 8; i++)
        {
            pa_coords[i][0] += dx;
            pa_coords[i][1] += dy;
            pa_coords[i][2] += dz;
        }
        break;
    case 4:
        for (int i = 0; i < 4; i++)
        {
            product_coords[i][0] += dx;
            product_coords[i][1] += dy;
            product_coords[i][2] += dz;
        }
        break;
    case 5:
        for (int i = 0; i < 3; i++)
        {
            SO2_orig_coords[i][0] += dx;
            SO2_orig_coords[i][1] += dy;
            SO2_orig_coords[i][2] += dz;
        }
        break;
    case 6:
        for (int i = 0; i < 2; i++)
        {
            O2_orig_coords[i][0] += dx;
            O2_orig_coords[i][1] += dy;
            O2_orig_coords[i][2] += dz;
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
    gluOrtho2D(0.0, width, 0.0, height);

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
        if (time_cnt < t2)
        {
            translate_xyz("SO2", 0.01, 0, 0);
            translate_xyz("O2", -0.01, 0, 0);
            time_cnt++;
            // std::cout << time_cnt << "\n";

            if (time_cnt > t1)
            {
                for (int i = 0; i < 3; ++i)
                {
                    color_t1[i] += delta_color;
                    color_t2[i] -= delta_color;
                }
                O2cylinderRadius -= delta_O2;
                SOcylinderRadius += delta_CBr;
            }
        }

        if (merging)
        {
            if (time_cnt < t3)
            {
                time_cnt++;
                // std::cout << time_cnt << "\n";
                for (int i = 0; i < 4; ++i)
                {
                    for (int j = 0; j < 3; ++j)
                    {
                        product_coords[i][j] = product_coords[i][j] += deltas[i][j];
                    }
                }
            }
            else if (time_cnt == t3 && calc)
            {
                SOcylinderRadius = 0;
                O2cylinderRadius = 0.2;
                calc = 0;
                for (int i = 0; i < 3; ++i)
                {
                    for (int j = 0; j < 3; ++j)
                    {
                        SO2_coords[i][j] = SO2_orig_coords[i][j];
                    }
                }

                for (int i = 0; i < 2; ++i)
                {
                    for (int j = 0; j < 3; ++j)
                    {
                        SO2_coords[i][j] = SO2_orig_coords[i][j];
                    }
                }
            }
        }
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
    wd = glutCreateWindow("SO2 + O2 -> SO3" /* title */);
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

    if (color == oxygen)
    {
        textureIndex = 3;
    }
    else if (color == sulphur)
    {
        textureIndex = 4;
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
        GLfloat light_position[] = {0.0, 0.0, 1.0, 0.0};

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
        translate_xyz("SO2", -delta, 0, 0);
        translate_xyz("O2", -delta, 0, 0);
        translate_xyz("SO3", -delta, 0, 0);
        translate_xyz("PA", -delta, 0, 0);
        translate_xyz("Product", -delta, 0, 0);
        translate_xyz("SO2_orig", -delta, 0, 0);
        translate_xyz("O2_orig", -delta, 0, 0);
    }
    if (key == 'w' || key == 'W')
    {
        translate_xyz("SO2", 0, delta, 0);
        translate_xyz("O2", 0, delta, 0);
        translate_xyz("SO3", 0, delta, 0);
        translate_xyz("PA", 0, delta, 0);
        translate_xyz("Product", 0, delta, 0);
        translate_xyz("SO2_orig", 0, delta, 0);
        translate_xyz("O2_orig", 0, delta, 0);
    }
    if (key == 's' || key == 'S')
    {
        translate_xyz("SO2", 0, -delta, 0);
        translate_xyz("O2", 0, -delta, 0);
        translate_xyz("SO3", 0, -delta, 0);
        translate_xyz("PA", 0, -delta, 0);
        translate_xyz("Product", 0, -delta, 0);
        translate_xyz("SO2_orig", 0, -delta, 0);
        translate_xyz("O2_orig", 0, -delta, 0);
    }
    if (key == 'd' || key == 'D')
    {
        translate_xyz("SO2", delta, 0, 0);
        translate_xyz("O2", delta, 0, 0);
        translate_xyz("SO3", delta, 0, 0);
        translate_xyz("PA", delta, 0, 0);
        translate_xyz("Product", delta, 0, 0);
        translate_xyz("SO2_orig", delta, 0, 0);
        translate_xyz("O2_orig", delta, 0, 0);
    }
    if (key == '8')
    {
        translate_xyz("SO2", 0, 0, delta);
        translate_xyz("O2", 0, 0, delta);
        translate_xyz("SO3", 0, 0, delta);
        translate_xyz("PA", 0, 0, delta);
        translate_xyz("Product", 0, 0, delta);
        translate_xyz("SO2_orig", 0, 0, delta);
        translate_xyz("O2_orig", 0, 0, delta);
    }
    if (key == '5')
    {
        translate_xyz("SO2", 0, 0, -delta);
        translate_xyz("O2", 0, 0, -delta);
        translate_xyz("SO3", 0, 0, -delta);
        translate_xyz("PA", 0, 0, -delta);
        translate_xyz("Product", 0, 0, -delta);
        translate_xyz("SO2_orig", 0, 0, -delta);
        translate_xyz("O2_orig", 0, 0, -delta);
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
        SOcylinderRadius = 0;
        O2cylinderRadius = 0.2;

        for (int i = 0; i < 3; ++i)
        {
            color_t1[i] = 0;
            color_t2[i] = 1;
        }

        for (int i = 0; i < 6; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                SO2_coords[i][j] = SO2_orig_coords[i][j];
            }
        }

        for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                O2_coords[i][j] = O2_orig_coords[i][j];
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
        glTranslatef((SO2_coords[0][0] + SO2_coords[2][0]) / 2, (SO2_coords[0][1] + SO2_coords[2][1]) / 2, (SO2_coords[0][2] + SO2_coords[2][2]) / 2);
        glRotatef(rot_angle, -1, -1, -1);
        glTranslatef(-(SO2_coords[0][0] + SO2_coords[2][0]) / 2, -(SO2_coords[0][1] + SO2_coords[2][1]) / 2,
                     -(SO2_coords[0][2] + SO2_coords[2][2]) / 2);
        draw_SO2(all_center);
        glPopMatrix();

        glPushMatrix();
        glTranslatef((O2_coords[0][0] + O2_coords[1][0]) / 2, (O2_coords[0][1] + O2_coords[1][1]) / 2, (O2_coords[0][2] + O2_coords[1][2]) / 2);
        glRotatef(rot_angle, -1, -1, -1);
        glTranslatef(-(O2_coords[0][0] + O2_coords[1][0]) / 2, -(O2_coords[0][1] + O2_coords[1][1]) / 2, -(O2_coords[0][2] + O2_coords[1][2]) / 2);
        draw_O2(all_center);
        glPopMatrix();
    }
    if (time_cnt > 0 && time_cnt <= t1)
    {
        // displayLegend();
        glPushMatrix();
        glTranslatef((SO2_coords[0][0] + SO2_coords[2][0]) / 2, (SO2_coords[0][1] + SO2_coords[2][1]) / 2, (SO2_coords[0][2] + SO2_coords[2][2]) / 2);
        glRotatef(rot_angle, -1, -1, -1);
        glTranslatef(-(SO2_coords[0][0] + SO2_coords[2][0]) / 2, -(SO2_coords[0][1] + SO2_coords[2][1]) / 2,
                     -(SO2_coords[0][2] + SO2_coords[2][2]) / 2);
        draw_SO2(all_center);
        glPopMatrix();

        if(O2_coords[0][1] > O2_coords[1][1]){
            O2_coords[1][1] += 0.01;
        }
        
        if(O2_coords[0][0] < O2_coords[1][0]){
            O2_coords[1][0] -= 0.01;
        }

        glPushMatrix();
        glTranslatef((O2_coords[0][0] + O2_coords[1][0]) / 2, (O2_coords[0][1] + O2_coords[1][1]) / 2, (O2_coords[0][2] + O2_coords[1][2]) / 2);
        glRotatef(rot_angle, -1, -1, -1);
        glTranslatef(-(O2_coords[0][0] + O2_coords[1][0]) / 2, -(O2_coords[0][1] + O2_coords[1][1]) / 2, -(O2_coords[0][2] + O2_coords[1][2]) / 2);
        draw_O2(all_center);
        glPopMatrix();
    }

    if (time_cnt > t1 && time_cnt <= t2)
    {
        double drx = (O2_coords[0][0] + SO2_coords[0][0]) / 2;
        double dry = (O2_coords[0][1] + SO2_coords[0][1]) / 2;
        double drz = (O2_coords[0][2] + SO2_coords[0][2]) / 2;

        glPushMatrix();
        glTranslatef(drx, dry, drz);
        glRotatef(rot_angle, -1, -1, -1);
        glTranslatef(-drx, -dry, -drz);
        draw_O2(all_center);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(drx, dry, drz);
        glRotatef(rot_angle, -1, -1, -1);
        glTranslatef(-drx, -dry, -drz);
        draw_SO2(all_center);
        glPopMatrix();
    }


    // int stop_plus = 120;
    if (time_cnt < 40)
    {
        // displayLegend();
        draw_plus_arrow();
    }

    if (time_cnt == t2)
    {
        // displayLegend();
        merge_cnt++;

        if (merge_cnt >= merge_pause)
        {
            merging = 1;
        }

        if (to_print)
        {
            int ind[4] = {0, 1, 2, 1};
            char type[4] = {'c', 'c', 'c', 'h'};
            // Type 'o': atom from O2, Type 'o': atom from SO2

            for (int i = 0; i < 4; ++i)
            {
                for (int j = 0; j < 3; ++j)
                {
                    if (type[i] == 'h')
                        product_coords[i][j] = O2_coords[ind[i]][j];
                    else
                        product_coords[i][j] = SO2_coords[ind[i]][j];
                }
            }

            for (int i = 0; i < 4; ++i)
            {
                for (int j = 0; j < 3; ++j)
                {
                    deltas[i][j] = (SO3_coords[i][j] - product_coords[i][j]) / (t3 - t2);
                }
            }
            to_print = 0;
        }
    }

    if (time_cnt > t2 && time_cnt < t3)
    {
        // displayLegend();
        glPushMatrix();
        glTranslatef((product_coords[0][0] + product_coords[2][0]) / 2, (product_coords[0][1] + product_coords[2][1]) / 2, (product_coords[0][2] + product_coords[2][2]) / 2);
        glRotatef(rot_angle, -1, -1, -1);
        glTranslatef(-(product_coords[0][0] + product_coords[2][0]) / 2, -(product_coords[0][1] + product_coords[2][1]) / 2, -(product_coords[0][2] + product_coords[2][2]) / 2);
        draw_product();
        glPopMatrix();
    }

    if (time_cnt == t3)
    {
        // displayLegend();
        glPushMatrix();
        glTranslatef((SO2_coords[0][0] + SO2_coords[2][0]) / 2, (SO2_coords[0][1] + SO2_coords[2][1]) / 2, (SO2_coords[0][2] + SO2_coords[2][2]) / 2);
        glRotatef(rot_angle, -1, -1, -1);
        glTranslatef(-(SO2_coords[0][0] + SO2_coords[2][0]) / 2, -(SO2_coords[0][1] + SO2_coords[2][1]) / 2,
                     -(SO2_coords[0][2] + SO2_coords[2][2]) / 2);
        draw_SO2(all_center);
        glPopMatrix();

        for(int i=0; i<2; i++){
            for(int j=0; j<3; j++){
                O2_coords[i][j] = O2_orig_coords[i][j];
            }
        }

        glPushMatrix();
        glTranslatef((O2_coords[0][0] + O2_coords[1][0]) / 2, (O2_coords[0][1] + O2_coords[1][1]) / 2, (O2_coords[0][2] + O2_coords[1][2]) / 2);
        glRotatef(rot_angle, -1, -1, -1);
        glTranslatef(-(O2_coords[0][0] + O2_coords[1][0]) / 2, -(O2_coords[0][1] + O2_coords[1][1]) / 2, -(O2_coords[0][2] + O2_coords[1][2]) / 2);
        draw_O2(all_center);
        glPopMatrix();

        glPushMatrix();
        glTranslatef((product_coords[0][0] + product_coords[2][0]) / 2, (product_coords[0][1] + product_coords[2][1]) / 2, (SO3_coords[0][2] + product_coords[2][2]) / 2);
        glRotatef(rot_angle, -1, -1, -1);
        glTranslatef(-(product_coords[0][0] + SO3_coords[2][0]) / 2, -(product_coords[0][1] + product_coords[2][1]) / 2, -(product_coords[0][2] + product_coords[2][2]) / 2);
        draw_product();
        glPopMatrix();

        draw_plus_arrow();
    }

    glFlush();
}