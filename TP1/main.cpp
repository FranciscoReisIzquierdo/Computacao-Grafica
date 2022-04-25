#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>

float angle = 10;
float size = 1;

void changeSize(int w, int h){
	// prevent a divide by zero, when window is too short
	// (you can not make a window with zero width).
	if (h == 0)
		h = 1;
	// compute window's aspect ratio
	float ratio = w * 1.0f / h;

	// set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load the identity matrix
	glLoadIdentity();
	// set the perspective
	gluPerspective(45.0f, ratio, 1.0f, 1000.0f);
	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);

	// et the viewport to be the entire window
	glViewport(0, 0, w, h);
}


void renderScene(void){
	// clear buffers (Frames that could be in the buffer must be cleared)
    // GL_COLOR_BUFFER_BIT -> CLEAR THE BUFFER TO THE DEFAULT COLOR (BLACK)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// set camera
    // Reloads the metrics, like a reset
	glLoadIdentity();
	gluLookAt(0.0f, 0.0f, 5.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 1.0f, 0.0f);
		
	// put drawing instructions here
    glEnable(GL_DEPTH_TEST);
    glPushMatrix();
    glRotatef(angle, 1, 1, 0);
    glutWireTeapot(0.75 + 0.25 * sin(glutGet(GLUT_ELAPSED_TIME) / 1000.0));

    glPopMatrix();
    //To display the frame drawn
    glFlush();
	
	// End of frame
	glutSwapBuffers();
}

void timer(int t){
    // The current window needs to be redisplay
    glutPostRedisplay();
    angle += 10;
    glutTimerFunc(50, timer, t+ 1);
}


void changeColor(int c) {
    switch(c) {
        case 0:
            glColor3ub(75,0,130);
            break;
        case 1:
            glColor3ub(0,0,255);
            break;
        case 2:
            glColor3ub(0,128,0);
            break;
        case 3:
            glColor3ub(255,255,0);
            break;
        case 4:
            glColor3ub(255,165,0);
            break;
        case 5:
            glColor3ub(255,0,0);
            break;
    }
    glutTimerFunc(200, changeColor, (c + 1) % 6);
}

void printInfo() {

	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));
}


int main(int argc, char** argv){
	// put GLUT init here
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);

    // Function for the client's window
    glutInitWindowPosition(100, 100); //Initialize the window's position
    glutInitWindowSize(800, 800); //Initialize the window's size
    glutCreateWindow("Hello World"); //Create window

    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);

    glutTimerFunc(1000, timer, 0);
    glutTimerFunc(1000, changeColor, 0);


    // some OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Background Color

    // enter GLUTs main cycle
    glutMainLoop();
    // put callback registry here

	return 1;
}