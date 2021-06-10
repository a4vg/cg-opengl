#include <iostream>

#include <cmath>
#include <iostream>

#ifdef __APPLE__
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>
#pragma comment(lib, "glew32.lib") // Compiler directive to include the GLEW library.
#endif

#define PI 3.14159265

using namespace std;

#include "glut_ply.h"

static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate hemisphere.
Model_PLY model;
char *archivo = "../models/cow.ply";


// Initialization routine.
void setup(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);

    glEnableClientState(GL_VERTEX_ARRAY); // Enable vertex array.
}

// Drawing routine.
void drawScene(void) {

    glClear (GL_COLOR_BUFFER_BIT);

    glLoadIdentity();
    glVertexPointer(3, GL_FLOAT, 0, model.Vertices);

    // Command to push the hemisphere, which is drawn centered at the origin,
    // into the viewing frustum.
    glTranslatef(0.0, 0.0, -7.0);

    // Commands to turn the hemisphere.
    glRotatef(Zangle, 0.0, 0.0, 1.0);
    glRotatef(Yangle, 0.0, 1.0, 0.0);
    glRotatef(Xangle, 1.0, 0.0, 0.0);

    // Hemisphere properties.
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glColor3f(0.0, 0.0, 0.0);

    // Multidraw command equivalent to the drawing loop for(j = 0; j < q; j++){...} in hemisphere.cpp.
    glDrawElements(GL_TRIANGLES, model.cantIndices, GL_UNSIGNED_INT, (const void *)model.Indices);

    glFlush();
}

// OpenGL window reshape routine.
void resize(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y) {
    switch(key)
    {
        case 27:
            exit(0);
            break;
        case 'x':
            Xangle += 5.0;
            if (Xangle > 360.0) Xangle -= 360.0;
            glutPostRedisplay();
            break;
        case 'X':
            Xangle -= 5.0;
            if (Xangle < 0.0) Xangle += 360.0;
            glutPostRedisplay();
            break;
        case 'y':
            Yangle += 5.0;
            if (Yangle > 360.0) Yangle -= 360.0;
            glutPostRedisplay();
            break;
        case 'Y':
            Yangle -= 5.0;
            if (Yangle < 0.0) Yangle += 360.0;
            glutPostRedisplay();
            break;
        case 'z':
            Zangle += 5.0;
            if (Zangle > 360.0) Zangle -= 360.0;
            glutPostRedisplay();
            break;
        case 'Z':
            Zangle -= 5.0;
            if (Zangle < 0.0) Zangle += 360.0;
            glutPostRedisplay();
            break;
        default:
            break;
    }
}




void printInteraction(void) {
    cout << "Interaction:" << endl;
    cout << "Press P/p to increase/decrease the number of longitudinal slices." << endl
         << "Press Q/q to increase/decrease the number of latitudinal slices." << endl
         << "Press x, X, y, Y, z, Z to turn the hemisphere." << endl;
}

// Main routine.
int main(int argc, char **argv) {
    model.Load(archivo);

    printInteraction();
    glutInit(&argc, argv);

    //glutInitContextVersion(4, 3);
    glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Visualizando modelo.cpp");
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);

    glewExperimental = GL_TRUE;
    glewInit();

    setup();

    glutMainLoop();
}
