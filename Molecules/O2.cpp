#include <bits/stdc++.h> 
#include <GL/glut.h> 
using namespace std; 
 
double FinalX = 0.0; 
double FinalY = 0.0; 
 
struct color 
{ 
    double r, g, b; 
}; 
 
color red = {1, 0, 0}; 
color green = {0, 1, 0}; 
color blue = {0, 0, 1}; 
color yellow = {1, 1, 0}; 
color cyan = {0, 1, 1}; 
color magenta = {1, 0, 1}; 
color white = {1, 1, 1}; 
color black = {0, 0, 0}; 
 
double O2_centers[2][2] = {{-150, 0}, {150, 0}}; 
color O2_colors[] = {red, red}; 
double O2_radii[2] = {50, 50}; 
 
void draw_circle(double x, double y, double r, color c, string label) 
{ 
    glColor3f(c.r, c.g, c.b); 
    glBegin(GL_POLYGON); 
    for (int i = 0; i < 360; i++) 
    { 
        double angle = 2 * 3.1416 * i / 360; 
        double x1 = x + r * cos(angle); 
        double y1 = y + r * sin(angle); 
        glVertex2f(x1, y1); 
    } 
    glEnd(); 
 
    // Position the label near the circle 
    glColor3f(1.0, 1.0, 1.0);  
    glRasterPos2d(x - (r / 4), y);  
    for (int i = 0; i < label.length(); i++) 
    { 
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, label[i]); 
    } 
} 
 
void draw_O2(double centers[2][2], double radii[2], color colors[2]) 
{ 
    glBegin(GL_LINES); 
    glColor3f(1, 1, 1); 
    glVertex2f(centers[0][0] + 1, centers[0][1] + 5); 
    glVertex2f(centers[1][0] + 1, centers[1][1] + 5); 
    glEnd(); 
    glBegin(GL_LINES); 
    glColor3f(1, 1, 1); 
    glVertex2f(centers[0][0] - 1, centers[0][1] - 5); 
    glVertex2f(centers[1][0] - 1, centers[1][1] - 5); 
    glEnd(); 
 
    // Define labels for each circle 
    string labels[] = {"O", "O"}; 
     
    for (int i = 0; i < 2; i++) 
    { 
        draw_circle(centers[i][0], centers[i][1], radii[i], colors[i], labels[i]); 
    } 
} 
 
void displayMe(void) 
{ 
    glClear(GL_COLOR_BUFFER_BIT); 
    draw_O2(O2_centers, O2_radii, O2_colors); 
    glFlush(); 
} 
 
// Get Co-Ordinates after Mouse Click 
void mouse(int button, int state, int x, int y) 
{ 
    if (state == GLUT_DOWN) 
    { 
        FinalX = x; 
        FinalY = y; 
    } 
} 
 
signed main(int argc, char **argv) 
{ 
    glutInit(&argc, argv); 
    glutInitDisplayMode(GLUT_SINGLE); 
    glutInitWindowSize(800, 800); 
    glutInitWindowPosition(100, 100); 
    glutCreateWindow("Window"); 
    glClearColor(0, 0, 0, 0); 
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity(); 
    gluOrtho2D(-500, 500, -500, 500); 
    glutDisplayFunc(displayMe); 
    glutMainLoop(); 
}