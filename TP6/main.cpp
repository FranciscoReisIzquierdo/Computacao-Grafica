

#include<stdio.h>
#include<stdlib.h>
#include <cstring>

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

#include <IL/il.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif


float camX = 00, camY = 30, camZ = 40;
int startX, startY, tracking = 0;

float alfa = 0.0f, beta = 0.5f, radius = 100.0f;

int alpha = 0, betat = 45, r = 50;

int altura, largura;
unsigned char *imageData;
GLuint *pontos;


void spherical2Cartesian() {

    camX = radius * cos(betat) * sin(alfa);
    camY = radius * sin(betat);
    camZ = radius * cos(betat) * cos(alfa);
}

void init() {

    unsigned int t;
    ilGenImages(1,&t);
    ilBindImage(t);

    ilLoadImage((ILstring)"terreno.jpg");
    ilConvertImage(IL_LUMINANCE, IL_UNSIGNED_BYTE);

    largura = ilGetInteger(IL_IMAGE_WIDTH);
    altura = ilGetInteger(IL_IMAGE_HEIGHT);

    imageData = ilGetData();

    pontos = (GLuint *)calloc(altura - 1, sizeof(GLuint));
    glGenBuffers(altura - 1, pontos);


    double startingX = altura / 2 ;
    double startingZ = largura / 2;

    //Creating and storing all the points
    for(int i= 0; i< altura - 1; i++){
        std::vector<double> tmp;
        tmp.clear();

        for(int j = 0; j< largura ; j++){

            tmp.push_back((double) i - startingX);
            tmp.push_back((double) imageData[i * largura + j]/ 2);
            tmp.push_back((double) j - startingZ);

            tmp.push_back((double) i + 1 - startingX);
            tmp.push_back((double) imageData[i * largura + j]/ 2);
            tmp.push_back((double) j + 1 - startingZ);
        }
        glBindBuffer(GL_ARRAY_BUFFER, pontos[i]);
        glBufferData(GL_ARRAY_BUFFER, tmp.size() * sizeof(double), tmp.data(), GL_STATIC_DRAW);
    }

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
}


void changeSize(int w, int h) {

    // Prevent a divide by zero, when window is too short
    // (you cant make a window with zero width).
    if(h == 0)
        h = 1;

    // compute window's aspect ratio
    float ratio = w * 1.0 / h;

    // Set the projection matrix as current
    glMatrixMode(GL_PROJECTION);
    // Load Identity Matrix
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);

    // Set perspective
    gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

    // return to the model view matrix mode
    glMatrixMode(GL_MODELVIEW);
}



void drawTerrain() {

    // colocar aqui o código de desnho do terreno usando VBOs com TRIANGLE_STRIPS
    for (int i = 0; i < altura - 1 ; i++) {
        glBindBuffer(GL_ARRAY_BUFFER, pontos[i]);
        glVertexPointer(3, GL_DOUBLE, 0, 0);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, (largura) * 2);
    }
}



void renderScene(void) {

    float pos[4] = {-1.0, 1.0, 1.0, 0.0};

    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(camX, camY, camZ,
              0.0,0.0,0.0,
              0.0f,1.0f,0.0f);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    drawTerrain();

    // just so that it renders something before the terrain is built
    // to erase when the terrain is ready


// End of frame
    glutSwapBuffers();
}



void processKeys(unsigned char key, int xx, int yy) {

// put code to process regular keys in here
}

void processSpecialKeys(int key, int xx, int yy) {

    switch (key) {

        case GLUT_KEY_RIGHT:
            alfa -= 0.1; break;

        case GLUT_KEY_LEFT:
            alfa += 0.1; break;

        case GLUT_KEY_UP:
            beta += 0.1f;
            if (beta > 1.5f)
                beta = 1.5f;
            break;

        case GLUT_KEY_DOWN:
            beta -= 0.1f;
            if (beta < -1.5f)
                beta = -1.5f;
            break;

        case GLUT_KEY_PAGE_DOWN: radius -= 1.0f;
            if (radius < 1.0f)
                radius = 1.0f;
            break;

        case GLUT_KEY_PAGE_UP: radius += 1.0f; break;
    }
    spherical2Cartesian();
    glutPostRedisplay();

}

void processMouseButtons(int button, int state, int xx, int yy) {

    if (state == GLUT_DOWN)  {
        startX = xx;
        startY = yy;
        if (button == GLUT_LEFT_BUTTON)
            tracking = 1;
        else if (button == GLUT_RIGHT_BUTTON)
            tracking = 2;
        else
            tracking = 0;
    }
    else if (state == GLUT_UP) {
        if (tracking == 1) {
            alpha += (xx - startX);
            beta += (yy - startY);
        }
        else if (tracking == 2) {

            r -= yy - startY;
            if (r < 3)
                r = 3.0;
        }
        tracking = 0;
    }
}


void processMouseMotion(int xx, int yy) {

    int deltaX, deltaY;
    int alphaAux, betaAux;
    int rAux;

    if (!tracking)
        return;

    deltaX = xx - startX;
    deltaY = yy - startY;

    if (tracking == 1) {


        alphaAux = alpha + deltaX;
        betaAux = beta + deltaY;

        if (betaAux > 85.0)
            betaAux = 85.0;
        else if (betaAux < -85.0)
            betaAux = -85.0;

        rAux = r;
    }
    else if (tracking == 2) {

        alphaAux = alpha;
        betaAux = beta;
        rAux = r - deltaY;
        if (rAux < 3)
            rAux = 3;
    }
    camX = rAux * sin(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
    camZ = rAux * cos(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
    camY = rAux * 							     sin(betaAux * 3.14 / 180.0);
}




int main(int argc, char **argv) {

// init GLUT and the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(320,320);
    glutCreateWindow("TP6");


// Required callback registry
    glutDisplayFunc(renderScene);
    glutIdleFunc(renderScene);
    glutReshapeFunc(changeSize);

// Callback registration for keyboard processing
    glutKeyboardFunc(processKeys);
    glutMouseFunc(processMouseButtons);
    glutMotionFunc(processMouseMotion);
    glutSpecialFunc(processSpecialKeys);

    glewInit();
    glEnableClientState(GL_VERTEX_ARRAY);

    ilInit();

    init();

// enter GLUT's main cycle
    glutMainLoop();

    return 0;
}
