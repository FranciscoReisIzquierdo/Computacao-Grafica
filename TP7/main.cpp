

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
int ra = 50, ri= 35, rc= 15;

int alpha = 0, betat = 45, rar = 50;

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

double radianToDegree(double radians){ return  radians * (180.0/M_PI); }

float h(int x, int z){
    return imageData[x * largura + z]/ 2;
}


float hf(int x, int z){
    int x1 = floor(x);
    int z1 = floor(z);
    int x2 = x1 + 1;
    int z2 = z1 + 1;

    float fz = z - z1;
    float fx = x - x1;

    float h_x1_z = h(x1,z1) * (1-fz) + h(x1,z2) * fz;
    float h_x2_z = h(x2,z1) * (1-fz) + h(x2,z2) * fz;

    return h_x1_z * (1 - fx) + h_x2_z * fx;

}

void drawTerrain() {

    // colocar aqui o código de desnho do terreno usando VBOs com TRIANGLE_STRIPS
    for (int i = 0; i < altura - 1 ; i++) {
        glBindBuffer(GL_ARRAY_BUFFER, pontos[i]);
        glVertexPointer(3, GL_DOUBLE, 0, 0);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, (largura) * 2);
    }

    glColor3ub(255, 69, 0);
    glutSolidTorus(2, 7.5, 10, 10);

    int numberOfTeaPots = 16;
    double gap = 2* M_PI / numberOfTeaPots;

    double rotation = glutGet(GLUT_ELAPSED_TIME)/ 1000.f;

    for(int nextStep= 0; nextStep< numberOfTeaPots; nextStep++) {

        glPushMatrix();
        glTranslatef(ri * sin(gap* nextStep + rotation), 2.0f, ri * cos(gap * nextStep + rotation));
        glRotatef(15 + radianToDegree(gap* nextStep + rotation), 0, 1, 0);

        glColor3ub(178, 34, 34);
        glutSolidTeapot(2);
        glPopMatrix();
    }

    numberOfTeaPots = 8;
    gap = 2* M_PI / numberOfTeaPots;
    for(int nextStep= 0; nextStep< numberOfTeaPots; nextStep++) {

        glPushMatrix();
        glTranslatef(rc * sin(gap* nextStep - rotation), 2.0f, rc * cos(gap * nextStep - rotation));
        glRotatef(-90 + radianToDegree( + gap* nextStep - rotation), 0, 1, 0);

        glColor3ub(0, 0, 205);
        glutSolidTeapot(2);
        glPopMatrix();
    }

    srand(1);
    for(int i= 0; i< 750; i++) {
        double randGreen = rand() % (225 - 75 + 1) + 75;
        double randomX = 0;
        double randomZ = 0;
        double randomHeight = rand() % (15 - 5 + 1) + 5;
        while (pow(randomX, 2) + pow(randomZ, 2) < pow(ra, 2)) {
            randomX = (rand() % (ra * 4)) - (ra * 2);
            randomZ = (rand() % (ra * 4)) - (ra * 2);
        }

        glPushMatrix();
        glColor3ub(102, 62, 36);
        glTranslatef(randomX, hf(randomX + largura / 2, randomZ + altura / 2), randomZ);
        glRotatef(-90.0f, 1, 0, 0);
        glutSolidCone(0.5f, 2, 10, 10);
        glPopMatrix();

        glPushMatrix();
        glColor3ub(0, randGreen, 0);
        glTranslatef(randomX, hf(randomX + largura / 2, randomZ + altura / 2) + 2.0f, randomZ);
        glRotatef(-90.0f, 1, 0, 0);
        glutSolidCone(2, randomHeight, 10, 10);
        glPopMatrix();
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

            rar -= yy - startY;
            if (rar < 3)
                rar = 3.0;
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

        rAux = rar;
    }
    else if (tracking == 2) {

        alphaAux = alpha;
        betaAux = beta;
        rAux = rar - deltaY;
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
