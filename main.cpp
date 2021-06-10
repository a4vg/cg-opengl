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

#include "matriz.h"
#include "glut_ply.h"


static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate hemisphere.
static float Scale = 1.0;
Model_PLY model;
char *archivo = "../models/cow.ply";

GLuint p_id;
GLint mat_mvp_id, mat_mv_id, mat_normal_id, luz_pos_id, color_difuso_id;
GLint vertex_id = 0, normal_id = 1;


// NEW: Function to read a shader file.
char* readShader(char* aShaderFile)
{
    FILE* filePointer = fopen(aShaderFile, "rb");
    char* content = NULL;
    long numVal = 0;

    fseek(filePointer, 0L, SEEK_END);
    numVal = ftell(filePointer);
    fseek(filePointer, 0L, SEEK_SET);
    content = (char*) malloc((numVal+1) * sizeof(char));
    fread(content, 1, numVal, filePointer);
    content[numVal] = '\0';
    fclose(filePointer);
    return content;
}

static void Error(char *message) {
    printf(message);
}

/* Compila shader */
static void CompileShader (GLuint id) {
    GLint status;
    glCompileShader(id);
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);
    if (!status) {
        GLint len;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
        char* message = (char*) malloc(len*sizeof(char));
        glGetShaderInfoLog(id, len, 0, message);
        Error(message);
        free(message);
    }
}

/* Link−edita shader */
static void LinkProgram (GLuint id) {
    GLint status;
    glLinkProgram(id);
    glGetProgramiv(id, GL_LINK_STATUS, &status);
    if (!status) {
        GLint len;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &len);
        char* message = (char*) malloc(len*sizeof(char));
        glGetProgramInfoLog(id, len, 0, message);
        Error(message);
        free(message);
    }
}


/* Cria programa de shader */
static void CreateShaderProgram (char* vertexShaderFile, char* fragmentShaderFile) {
    char*	vertexShader   = readShader(vertexShaderFile);
    char*	fragmentShader = readShader(fragmentShaderFile);

    /* vertex shader */
    GLuint v_id = glCreateShader(GL_VERTEX_SHADER);
    if (v_id == 0)
        Error("Could not create vertex shader object");

    glShaderSource(v_id, 1, (const char**) &vertexShader, 0);
    CompileShader(v_id);

    /* fragment shader */
    GLuint f_id = glCreateShader(GL_FRAGMENT_SHADER);
    if (f_id == 0)
        Error("Could not create fragment shader object");

    glShaderSource(f_id, 1, (const char**) &fragmentShader, 0);
    CompileShader(f_id);

    /* program */
    p_id = glCreateProgram();
    if (p_id == 0)
        Error("Could not create program object");
    glBindAttribLocation(p_id, vertex_id, "myVertex");
    glBindAttribLocation(p_id, normal_id, "myNormal");
    glAttachShader(p_id, v_id);
    glAttachShader(p_id, f_id);
    LinkProgram(p_id);

    mat_mvp_id		= glGetUniformLocation(p_id, "mvpMatrix");
    mat_mv_id		= glGetUniformLocation(p_id, "mvMatrix");
    mat_normal_id	= glGetUniformLocation(p_id, "normalMatrix");
    luz_pos_id		= glGetUniformLocation(p_id, "vLightPosition");
    color_difuso_id	= glGetUniformLocation(p_id, "diffuseColor");
}



// Initialization routine.
void setup(void) {
    glClearColor(1.0, 1.0, 1.0, 0.0);

    glEnableClientState(GL_VERTEX_ARRAY); // Enable vertex array.
    glEnable(GL_DEPTH_TEST);
    CreateShaderProgram("../triangulo2.vs","../triangulo2.fs");
}

// Drawing routine.
void drawScene(void) {
    int vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(p_id);
    glVertexAttribPointer(vertex_id, 3, GL_FLOAT, GL_FALSE, 0, model.Vertices);
    glVertexAttribPointer(normal_id, 3, GL_FLOAT, GL_FALSE, 0, model.Normales);
    glEnableVertexAttribArray(vertex_id);
    glEnableVertexAttribArray(normal_id);

    //glDrawArrays(GL_TRIANGLES, 0, model.cantVertices);
    glDrawElements(GL_TRIANGLES, model.cantIndices, GL_UNSIGNED_INT, (const void *)model.Indices);

    static float mvpMatrix[] = {
            0.5, 0.0, 0.0, 0,
            0.0, 0.5, 0.0, 0,
            0.0, 0.0, 0.5, 0,
            0.0, 0.0, 0.0, 1.0
    };
    static float normalMatrix[] = {
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f
    };

    Matriz mv, ms, mc, mp;
    mv.setRotationXMatrix(Xangle);
    ms.scale(Scale, Scale, Scale);
    mv.Multiplicacion(ms);
    //mp.Multiplicacion(mc);

    mc.SetCamera(10, 10, 10, 0, 0, 0, 0, 1, 0);
    mp.ProjectionMatrix(50.0, 1, 10, 1000);

    mp.Multiplicacion(mv);

    static float aLuzPos[] = {-1.0f, 1.0f, 1.0f};
    static float aColorDifuso[] = {0.0f, 0.0f, 1.0f, 1.0f};
    GLboolean transpose = GL_TRUE;
    glUniformMatrix4fv(mat_mvp_id, 1, transpose, mp.mat);
    glUniformMatrix4fv(mat_mv_id, 1, transpose, mv.mat);
    glUniformMatrix3fv(mat_normal_id, 1, transpose, normalMatrix);
    glUniform3fv(luz_pos_id, 1, aLuzPos);
    glUniform4fv(color_difuso_id, 1, aColorDifuso);


/*
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
*/
    glutSwapBuffers();
}

// OpenGL window reshape routine.
void resize(int w, int h) {
    /*glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);
    glMatrixMode(GL_MODELVIEW);*/
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y) {
    switch(key)
    {
        case 27:
            exit(0);
            break;
        case 'x':
            Xangle += 1.0;
            if (Xangle > 360.0) Xangle -= 360.0;
            glutPostRedisplay();
            break;
        case 'X':
            Xangle -= 1.0;
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
        case 's': Scale += 0.1;
            glutPostRedisplay();
            cout << Scale << " ";
            break;
        case  'S': Scale -= 0.1;
            if (Scale == 0.0) Scale = 0.1;
            cout << Scale << " ";
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

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Visualizando modelo");
    glutDisplayFunc(drawScene);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);

    glewExperimental = GL_TRUE;
    glewInit();

    setup();

    glutMainLoop();
    return 0;
}
