#include <bits/stdc++.h>
#include <GL/glut.h>
using namespace std;

void draw(){
    
}

void Initial()
{
    glClearColor(1.0, 1.0, 1.0, 0);
    glColor3f(1, 0, 1);
    glPointSize(5.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 100, 0, 100);
}

int main(int argc, char *argv[])
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(0, 0);

    glutCreateWindow("Chem Expo");
    Initial();
    glutDisplayFunc(draw);
    glutMainLoop();
    return 0;
}