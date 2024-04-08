// main.cpp
// Par Gabriel Le Breton aka GabLeRoux
// 3 Octobre 2011

#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <fstream>
#include <string>
#include <cmath>
#include <iostream>

#if defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
// Windows users, you should try this:
// #include "glut/glut.h"

/* Global Variables (Configs) */
// Init options

// GLfloat Na2CO3_coords[7][3] = {
//     {0, 0, 0},
//     {1, 1, 1},
//     {1, -1, -1},
//     {-1, -1, 1},
//     {-1, 1, -1},
//     {2, 1, 1},
//     {2, -1, -1}};

GLfloat Na2CO3_coords[6][3] = {
    {0, 0, 0},  // Carbon (C)
    {-1, 1, 0},  // Oxygen (O) near na
    {1, 1, 0}, // Oxygen (O) near H
    {0, -1, 0},  // Oxygen (O) double bond
    {-2, 0, 0},  // left Sodium (Na)
    {2, 0, 0}   // right Sodium (Na)
};

// GLfloat Na2CO3_coords[7][3] = {
//     {5.2, 0, 0},
//     {6.2, 1, 1},
//     {6.2, -1, -1},
//     {4.2, -1, 1},
//     {4.2, 1, -1},
//     {7.2, 1, 1},
//     {7.2, -1, -1}};

int selectedObject = 3;
bool drawThatAxis = 0;
bool lightEffect = 1;
GLdouble sphereRadius = 0.4;
GLdouble cylinderRadius = 0.2;
GLint resolution = 100;
GLint slices = resolution, stacks = resolution;

// Viewer options (GluLookAt)
float fovy = 60.0, aspect = 1.0, zNear = 1.0, zFar = 100.0;

// Mouse modifiers
float depth = 8;
float phi = 0, theta = 0;
float downX, downY;
bool leftButton = false, middleButton = false;

// colors
GLfloat oxygen[3] = {1.0, 0.5, 0.0};    // (O - Golden)
GLfloat nitrogen[3] = {0.0, 0.0, 1.0};  // (N - Blue)
GLfloat phosphate[3] = {1.0, 0.5, 0.0}; // (P - Orange)
GLfloat carbon[3] = {0.5, 0.5, 0.5};    // (C - Grey)
GLfloat sulphur[3] = {1, 0, 0};         //  (S - Red)
GLfloat hydrogen[3] = {0.0, 0.0, 1.0};  // (H - Blue)
GLfloat white[3] = {1.0, 1.0, 1.0};
GLfloat black[3] = {1.0, 1.0, 1.0};
GLfloat sodium[3] = {0.67, 0.34, 0.93}; // RGB color for Sodium

/* Prototypes */
void liaison(GLfloat color[3], GLfloat height);
void atome(GLfloat color[3]);
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

void draw_Na2CO3(GLfloat center[3])
{
    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            Na2CO3_coords[i][j] = center[j] + Na2CO3_coords[i][j];
        }
    }

    for (int i = 0; i < 6; i++)
    {
        glPushMatrix();
        glTranslatef(Na2CO3_coords[i][0], Na2CO3_coords[i][1], Na2CO3_coords[i][2]);
        if (i == 0)
            atome(carbon);
        else if (i < 4)
            atome(oxygen);
        else 
            atome(sodium);
        glPopMatrix();
    }

    GLUquadric *myQuad;
    myQuad = gluNewQuadric();


        glColor3fv(white);
        setLightColor(white);

        
    renderCylinder(Na2CO3_coords[0][0], Na2CO3_coords[0][1], Na2CO3_coords[0][2],
                    Na2CO3_coords[2][0], Na2CO3_coords[2][1], Na2CO3_coords[2][2],
                    cylinderRadius, myQuad);
    
    renderCylinder(Na2CO3_coords[1][0], Na2CO3_coords[1][1], Na2CO3_coords[1][2],
                    Na2CO3_coords[0][0], Na2CO3_coords[0][1], Na2CO3_coords[0][2],
                    cylinderRadius, myQuad);

    renderCylinder(Na2CO3_coords[0][0], Na2CO3_coords[0][1], Na2CO3_coords[0][2],
                    Na2CO3_coords[3][0], Na2CO3_coords[3][1], Na2CO3_coords[3][2],
                    cylinderRadius, myQuad);
    
}

void translate_Na2CO3(char key)
{
    int n = 7;
    GLfloat delta = 0.25;
    if (key == 'w')
    {
        for (int i = 0; i < n; i++)
        {
            Na2CO3_coords[i][1] += delta;
        }
    }
    else if (key == 'a')
    {
        for (int i = 0; i < n; i++)
        {
            Na2CO3_coords[i][0] -= delta;
        }
    }
    else if (key == 's')
    {
        for (int i = 0; i < n; i++)
        {
            Na2CO3_coords[i][1] -= delta;
        }
    }
    else if (key == 'd')
    {
        for (int i = 0; i < n; i++)
        {
            Na2CO3_coords[i][0] += delta;
        }
    }
    else if (key == '8')
    {
        for (int i = 0; i < n; i++)
        {
            Na2CO3_coords[i][2] += delta;
        }
    }
    else if (key == '5')
    {
        for (int i = 0; i < n; i++)
        {
            Na2CO3_coords[i][2] -= delta;
        }
    }
}

GLdouble width, height; /* window width and height */
int wd;                 /* GLUT window handle */

/* Program initialization NOT OpenGL/GLUT dependent,
 as we haven't created a GLUT window yet */
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

int main(int argc, char *argv[])
{
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

    /* create the window and store the handle to it */
    wd = glutCreateWindow("H2SO4" /* title */);

    /* --- register callbacks with GLUT --- */

    /* register function to handle window resizes */
    glutReshapeFunc(reshape);

    setLightColor(white);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glClearColor(0.0, 0.0, 0.0, 0.0);

    // buildDisplayList();

    /* register function that draws in the window */
    glutDisplayFunc(displayCallback);

    // motion
    glutMouseFunc(mouseCallback);
    glutMotionFunc(motionCallback);

    // exit with [esc] keyboard button
    glutKeyboardFunc(keyboardCallback);

    /* start the GLUT main loop */
    glutMainLoop();

    return 0;
}

void liaison(GLfloat color[3], GLfloat height)
{
    // sans lumière:
    glColor3fv(color);
    // avec lumière
    setLightColor(color);

    GLUquadric *myQuad;
    myQuad = gluNewQuadric();

    // Création du cylindre
    gluCylinder(myQuad, cylinderRadius, cylinderRadius, height, slices, stacks);
}

// Sphere
void atome(GLfloat color[3])
{
    // sans lumière:
    glColor3fv(color);
    // avec lumière
    setLightColor(color);

    GLUquadric *myQuad;
    myQuad = gluNewQuadric();

    // Création de la sphere
    gluSphere(myQuad, sphereRadius, slices, stacks);
}

void setLightColor(GLfloat light_color[3])
{
    // Light Options
    GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat shine[] = {100.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_color);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
    glMaterialfv(GL_FRONT, GL_SHININESS, shine);
}

// SRC: http://lifeofaprogrammergeek.blogspot.com/2008/07/rendering-cylinder-between-two-points.html
// will make a cylender between 2 pts :D
void renderCylinder(float x1, float y1, float z1, float x2, float y2, float z2, float radius, GLUquadricObj *quadric)
{
    float vx = x2 - x1;
    float vy = y2 - y1;
    float vz = z2 - z1;
    float ax, rx, ry, rz;
    float len = sqrt(vx * vx + vy * vy + vz * vz);

    glPushMatrix();
    glTranslatef(x1, y1, z1);
    if (fabs(vz) < 0.0001)
    {
        glRotatef(90, 0, 1, 0);
        ax = 57.2957795 * -atan(vy / vx);
        if (vx < 0)
        {
        }
        rx = 1;
        ry = 0;
        rz = 0;
    }
    else
    {
        ax = 57.2957795 * acos(vz / len);
        if (vz < 0.0)
            ax = -ax;
        rx = -vy * vz;
        ry = vx * vz;
        rz = 0;
    }
    glRotatef(ax, rx, ry, rz);
    gluQuadricOrientation(quadric, GLU_OUTSIDE);
    gluCylinder(quadric, radius, radius, len, slices, stacks);
    glPopMatrix();
}

void drawAxis()
{

    float originAxis[3] = {0, 0, 0}; // Origine
    float xAxis[3] = {1, 0, 0};      // L'axe des x
    float yAxis[3] = {0, 1, 0};      // L'axe des y
    float zAxis[3] = {0, 0, 1};      // L'axe des z

    // Temp: Désactivation de la lumière
    glDisable(GL_LIGHTING);
    glPushMatrix();
    glLineWidth(10.0);

    // x = rouge, y = vert, z = bleu

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
    // Réactivation de la lumière
    glEnable(GL_LIGHTING);
}

/* Callbacks */
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

void keyboardCallback(unsigned char key, int x, int y)
{
    if (key == 'a' || key == 'A')
    {
        // cout << "Key pressed: A\n";
        translate_Na2CO3('a');
    }
    if (key == 'w' || key == 'W')
    {
        // cout << "Key pressed: W\n";
        translate_Na2CO3('w');
    }
    if (key == 's' || key == 'S')
    {
        // cout << "Key pressed: S\n";
        translate_Na2CO3('s');
    }
    if (key == 'd' || key == 'D')
    {
        // cout << "Key pressed: D\n";
        translate_Na2CO3('d');
    }
    if (key == '8')
    {
        // cout << "Key pressed: D\n";
        translate_Na2CO3('8');
    }
    if (key == '5')
    {
        // cout << "Key pressed: D\n";
        translate_Na2CO3('5');
    }
    glutPostRedisplay();
}

void displayCallback(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovy, aspect, zNear, zFar);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // gluLookAt(eyex, eyey, eyez, atx, aty, atz, upx, upy, upz);
    gluLookAt(0, 0, 2, 0, 0, 0, 0, 1, 0);

    // Motion Options
    glTranslatef(0.0, 0.0, -depth);
    glRotatef(-theta, 1.0, 0.0, 0.0);
    glRotatef(phi, 0.0, 1.0, 0.0);

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

    switch (selectedObject)
    {
    // Molecule 1
    case (1):
        glCallList(1);
        break;
    // Molecule 2
    case (2):
        glCallList(2);
        break;
    // Molecule 3
    case (3):
        glCallList(3);
    default:
        break;
    }

    GLfloat cntr[3] = {0, 0, 0};
    draw_Na2CO3(cntr);
    glFlush();
}
