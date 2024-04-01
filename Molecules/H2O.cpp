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

double H2O_centers[3][2] = {{-150, -150}, {0, 0}, {150, -150}};
color H2O_colors[] = {blue, red, blue};
double H2O_radii[3] = {35, 50, 35};

void draw_circle(double x, double y, double r, color c, string label, double offsetX = 0.0, double offsetY = 0.0)
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

    // Calculate position for label relative to the circle
    double labelX = x + offsetX;
    double labelY = y + offsetY;

    glColor3f(1.0, 1.0, 1.0); // Set color to white for text
    glRasterPos2d(labelX, labelY); // Position the text near the circle
    for (int i = 0; i < label.length(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, label[i]); // Use a font (HELVETICA_12 in this case)
    }
}
void draw_H2O(double centers[3][2], double radii[3], color colors[3])
{
    glBegin(GL_LINES);
    glColor3f(1, 1, 1);
    glVertex2f(centers[0][0], centers[0][1]);
    glVertex2f(centers[1][0], centers[1][1]);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(centers[1][0], centers[1][1]);
    glVertex2f(centers[2][0], centers[2][1]);
    glEnd();

    // Define labels for each circle
    string labels[] = {"H", "O", "H"};

    // Define offsets for positioning the labels relative to the circles
    double offsets[][2] = {{-10, -5}, {-10, 5}, {-2, -2}};

    for (int i = 0; i < 3; i++)
    {
        draw_circle(centers[i][0], centers[i][1], radii[i], colors[i], labels[i], offsets[i][0], offsets[i][1]);
    }
}

void translate_H2O(char key)
{
    int n = 3;
    glClear(GL_COLOR_BUFFER_BIT);
    double delta = 10;

    if (key == 'w')
    {
        for (int i = 0; i < n; i++)
        {
            H2O_centers[i][1] += delta;
        }
    }
    else if (key == 'a')
    {
        for (int i = 0; i < n; i++)
        {
            H2O_centers[i][0] -= delta;
        }
    }
    else if (key == 's')
    {
        for (int i = 0; i < n; i++)
        {
            H2O_centers[i][1] -= delta;
        }
    }
    else
    {
        for (int i = 0; i < n; i++)
        {
            H2O_centers[i][0] += delta;
        }
    }
    draw_H2O(H2O_centers, H2O_radii, H2O_colors);

    glFlush();
}

// Scale Polygon along its center
void scale(char zoom)
{
    double scale;
    glClear(GL_COLOR_BUFFER_BIT);

    if (zoom == 'i')
    {
        scale = 1.05;
    }
    else
        scale = 0.95;

    double d_old_1 = sqrt(pow(H2O_centers[0][0] - H2O_centers[1][0], 2) + pow(H2O_centers[0][1] - H2O_centers[1][1], 2));
    double d_old_2 = sqrt(pow(H2O_centers[1][0] - H2O_centers[2][0], 2) + pow(H2O_centers[1][1] - H2O_centers[2][1], 2));
    double d_new_1 = scale * d_old_1;
    double d_new_2 = scale * d_old_2;

    double m1 = abs((H2O_centers[0][1] - H2O_centers[1][1]) / (H2O_centers[0][0] - H2O_centers[1][0]));
    double m2 = abs((H2O_centers[2][1] - H2O_centers[1][1]) / (H2O_centers[2][0] - H2O_centers[1][0]));

    int flags[4] = {1, 1, 1, 1};
    if (H2O_centers[0][0] < H2O_centers[1][0])
        flags[0] = -1;
    if (H2O_centers[0][1] < H2O_centers[1][1])
        flags[1] = -1;
    if (H2O_centers[2][0] < H2O_centers[1][0])
        flags[2] = -1;
    if (H2O_centers[2][1] < H2O_centers[1][1])
        flags[3] = -1;

    H2O_centers[0][0] = H2O_centers[1][0] + flags[0] * d_new_1 / (sqrt(1 + m1 * m1));
    H2O_centers[0][1] = H2O_centers[1][1] + flags[1] * m1 * d_new_1 / (sqrt(1 + m1 * m1));
    H2O_centers[2][0] = H2O_centers[1][0] + flags[2] * d_new_2 / (sqrt(1 + m2 * m2));
    H2O_centers[2][1] = H2O_centers[1][1] + flags[3] * m2 * d_new_2 / (sqrt(1 + m2 * m2));

    for (int i = 0; i < 3; i++)
    {
        H2O_radii[i] *= scale;
    }
    draw_H2O(H2O_centers, H2O_radii, H2O_colors);
    glFlush();
}

void displayMe(void)
{

    glClear(GL_COLOR_BUFFER_BIT);
    //glClearColor(1.0, 1.0, 1.0, 0.0);

    draw_H2O(H2O_centers, H2O_radii, H2O_colors);

    glFlush();
}

void rotate_H2O(double angle)
{
    glClear(GL_COLOR_BUFFER_BIT);

    double xc = H2O_centers[1][0];
    double yc = H2O_centers[1][1];

    for (int i = 0; i < 3; i++)
    {
        H2O_centers[i][0] -= xc;
        H2O_centers[i][1] -= yc;
    }

    for (int i = 0; i < 3; i++)
    {
        double x = H2O_centers[i][0];
        double y = H2O_centers[i][1];

        H2O_centers[i][0] = x * cos(angle) - y * sin(angle);
        H2O_centers[i][1] = x * sin(angle) + y * cos(angle);
    }

    for (int i = 0; i < 3; i++)
    {
        H2O_centers[i][0] += xc;
        H2O_centers[i][1] += yc;
    }

    draw_H2O(H2O_centers, H2O_radii, H2O_colors);
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

// Mouse Rotation function
void mouse_move(int x, int y)
{
    // Calculate mouse position change
    int delta_x = x - FinalX;

    // Rotate the figure
    double angle = delta_x * 0.05f;
    rotate_H2O(angle);

    // Change Final Mouse Position
    FinalX = x;
    FinalY = y;
}

void keyUser(unsigned char key, int x, int y)
{
    if (key == 'a' || key == 'A')
    {
        cout << "Key pressed: A\n";
        translate_H2O('a');
    }
    if (key == 'w' || key == 'W')
    {
        cout << "Key pressed: W\n";
        translate_H2O('w');
    }
    if (key == 's' || key == 'S')
    {
        cout << "Key pressed: S\n";
        translate_H2O('s');
    }
    if (key == 'd' || key == 'D')
    {
        cout << "Key pressed: D\n";
        translate_H2O('d');
    }
    if (key == 'i' || key == 'I')
    {
        cout << "Key pressed: I\n";
        scale('i');
    }
    if (key == 'o' || key == 'O')
    {
        cout << "Key pressed: O\n";
        scale('o');
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
    glutKeyboardFunc(keyUser);
    glutMotionFunc(mouse_move);
    glutMouseFunc(mouse);
    glutMainLoop();
}