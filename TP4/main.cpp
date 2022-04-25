#include <stdio.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

float alfa = 0.0f, beta = 0.0f, radius = 5.0f;
float camX, camY, camZ;

int timebase;
double frames;
const int n= 1;
int numberOfVertices;
GLuint buffers[n];


void spherical2Cartesian() {

	camX = radius * cos(beta) * sin(alfa);
	camY = radius * sin(beta);
	camZ = radius * cos(beta) * cos(alfa);
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


/*----------------------------------------------------------------------------------- 
	Draw Cylinder with strips and fans

	  parameters: radius, height, sides

-----------------------------------------------------------------------------------*/


void cylinder0(float radius, float height, int sides) {

	int i;
	float step;

	step = 360.0/sides;

	glColor3f(1,0,0);
	glBegin(GL_TRIANGLE_FAN);

		glVertex3f(0,height*0.5,0);
		for (i=0; i <= sides; i++) {
			glVertex3f(cos(i * step * M_PI/180.0)*radius,height*0.5,-sin(i * step *M_PI/180.0)*radius);
		}
	glEnd();

	glColor3f(0,1,0);
	glBegin(GL_TRIANGLE_FAN);

		glVertex3f(0,-height*0.5,0);
		for (i=0; i <= sides; i++) {
			glVertex3f(cos(i * step * M_PI/180.0)*radius,-height*0.5,sin(i * step *M_PI/180.0)*radius);
		}
	glEnd();

	glColor3f(0,0,1);
	glBegin(GL_TRIANGLE_STRIP);

		for (i=0; i <= sides; i++) {
			glVertex3f(cos(i * step * M_PI/180.0)*radius, height*0.5,-sin(i * step *M_PI/180.0)*radius);
			glVertex3f(cos(i * step * M_PI/180.0)*radius,-height*0.5,-sin(i * step *M_PI/180.0)*radius);
		}
	glEnd();
}


/*-----------------------------------------------------------------------------------
	Draw Cylinder

		parameters: radius, height, sides

-----------------------------------------------------------------------------------*/



void cylinder(float radius, float height, int sides) {

    // criar um vector com as coordenadas dos pontos
    std::vector<float> vetor;

    // put code to draw cylinder in here
    float halfHeight = height/ 2.0;

    float gap = 2* M_PI / sides;

    for(int slice = 0; slice < 10; slice++){
        float angle = (float) slice * gap;
        float nextAngle = (float) (slice + 1) * gap;

        // Top base ( 3 * sides vertices)
        // Vertex 1
        vetor.push_back(0.0f);
        vetor.push_back(halfHeight);
        vetor.push_back(0.0f);
        // Vertex 2
        vetor.push_back(radius * sin(angle));
        vetor.push_back(halfHeight);
        vetor.push_back(radius * cos(angle));
        // Vertex 3
        vetor.push_back(radius * sin(nextAngle));
        vetor.push_back(halfHeight);
        vetor.push_back(radius * cos(nextAngle));

        /*
        glBegin(GL_TRIANGLES);
        glColor3f(1.0f,0.12*slice,0.0f);
        glVertex3f(0.0f, halfHeight, 0.0f);
        glVertex3f(radius * sin(angle), halfHeight, radius * cos(angle));
        glVertex3f(radius * sin(nextAngle), halfHeight, radius * cos(nextAngle));
        glEnd();*/

        // Lateral ( 6 * sides)
        //Vertex 1
        vetor.push_back(radius * sin(angle));
        vetor.push_back(halfHeight);
        vetor.push_back(radius * cos(angle));
        //Vertex 2
        vetor.push_back(radius * sin(angle));
        vetor.push_back(-halfHeight);
        vetor.push_back(radius * cos(angle));
        //Vertex 3
        vetor.push_back(radius * sin(nextAngle));
        vetor.push_back(halfHeight);
        vetor.push_back(radius * cos(nextAngle));
        //Vertex 4
        vetor.push_back(radius * sin(nextAngle));
        vetor.push_back(halfHeight);
        vetor.push_back(radius * cos(nextAngle));
        //Vertex 5
        vetor.push_back(radius * sin(angle));
        vetor.push_back(-halfHeight);
        vetor.push_back(radius * cos(angle));
        //Vertex 6
        vetor.push_back(radius * sin(nextAngle));
        vetor.push_back(-halfHeight);
        vetor.push_back(radius * cos(nextAngle));
        /*
        glBegin(GL_TRIANGLES);
        glColor3f(1.0f,0.12*slice,0.0f);
        glVertex3f(radius * sin(angle), halfHeight, radius * cos(angle));
        glVertex3f(radius * sin(angle), -halfHeight, radius * cos(angle));
        glVertex3f(radius * sin(nextAngle), halfHeight, radius * cos(nextAngle));
        glEnd();

        glBegin(GL_TRIANGLES);
        glColor3f(1.0f,0.12*slice,0.0f);
        glVertex3f(radius * sin(nextAngle), halfHeight, radius * cos(nextAngle));
        glVertex3f(radius * sin(angle), -halfHeight, radius * cos(angle));
        glVertex3f(radius * sin(nextAngle), -halfHeight, radius * cos(nextAngle));
        glEnd();*/

        // Lower Base
        //Vertex 1
        vetor.push_back(0.0f);
        vetor.push_back(-halfHeight);
        vetor.push_back(0.0f);
        //Vertex 2
        vetor.push_back(radius * sin(nextAngle));
        vetor.push_back(-halfHeight);
        vetor.push_back(radius * cos(nextAngle));
        //Vertex 3
        vetor.push_back(radius * sin(angle));
        vetor.push_back(-halfHeight);
        vetor.push_back(radius * cos(angle));
        /*glBegin(GL_TRIANGLES);
        glColor3f(1.0f,0.12*slice,0.0f);
        glVertex3f(0.0f, -halfHeight, 0.0f);
        glVertex3f(radius * sin(nextAngle), -halfHeight, radius * cos(nextAngle));
        glVertex3f(radius * sin(angle), -halfHeight, radius * cos(angle));

        glEnd();*/
    }
    numberOfVertices = vetor.size()/ 3;

    // criar o VBO
    glGenBuffers(n, buffers);

    // copiar o vector para a memória gráfica
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof (float) * vetor.size(), vetor.data(), GL_STATIC_DRAW);
}


void framerate() {
    char title[50];
    frames++;
    double time = glutGet(GLUT_ELAPSED_TIME);

    if (time - timebase> 1000) {
        double fps = frames * 1000.0 / (time - timebase);
        timebase = time;
        frames = 0;
        sprintf(title, "TP4 | %lf FPS", fps);
        glutSetWindowTitle(title);
    }
}

void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(camX, camY, camZ,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);


    glBegin(GL_LINES);
    // X axis -> Red
    glColor3f(1.0f, 0.0, 0.0f);
    glVertex3f(100.0f, 0.0f, 0.0f);
    glVertex3f(-100.0f, 0.0f, 0.0f);
    // Y axis -> Green
    glColor3f(0.0f, 1.0, 0.0f);
    glVertex3f(0.0f, 100.0f, 0.0f);
    glVertex3f(0.0f, -100.0f, 0.0f);
    // Z axis -> Blue
    glColor3f(0.0f, 0.0, 1.0f);
    glVertex3f(0.0f, 0.0f, 100.0f);
    glVertex3f(0.0f, 0.0f, -100.0f);

    glEnd();

    //

    // Desenhar VBOs
    glColor3ub(255,0,255);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, numberOfVertices);

    // See performance
    framerate();

	// End of frame
	glutSwapBuffers();
}


void processKeys(unsigned char c, int xx, int yy) {

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

	case GLUT_KEY_PAGE_DOWN: radius -= 0.1f;
		if (radius < 0.1f)
			radius = 0.1f;
		break;

	case GLUT_KEY_PAGE_UP: radius += 0.1f; break;
	}
	spherical2Cartesian();
	glutPostRedisplay();

}


void printInfo() {

	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));

	printf("\nUse Arrows to move the camera up/down and left/right\n");
	printf("Page Up and Page Down control the distance from the camera to the origin");
}




int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("TP4");


// Required callback registry
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);

	// init GLEW
#ifndef __APPLE__
	glewInit();
#endif

    glEnableClientState(GL_VERTEX_ARRAY);
    cylinder(1,2,10);


//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT, GL_LINE);

	spherical2Cartesian();

	printInfo();

    timebase = glutGet(GLUT_ELAPSED_TIME);

// enter GLUT's main cycle
	glutMainLoop();

	return 1;
}

