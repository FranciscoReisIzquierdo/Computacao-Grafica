#include <stdio.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

float alfa = 0.0f, beta = 0.5f, radius = 100.0f;
float camX, camY, camZ;
float angle = glutGet(GLUT_ELAPSED_TIME)/ 1000.f;



int r = 50, ri= 35, rc= 15;


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

double radianToDegree(double radians){ return  radians * (180.0/M_PI); }

void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(camX, camY, camZ,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);

	glColor3f(0.2f, 0.8f, 0.2f);
	glBegin(GL_TRIANGLES);
		glVertex3f(100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, 100.0f);

		glVertex3f(100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, 100.0f);
		glVertex3f(100.0f, 0, 100.0f);
	glEnd();

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
        while (pow(randomX, 2) + pow(randomZ, 2) < pow(r, 2)) {
            randomX = (rand() % (r * 4)) - (r * 2);
            randomZ = (rand() % (r * 4)) - (r * 2);
        }
        glPushMatrix();
        glColor3ub(102, 62, 36);
        glTranslatef(randomX, 0.0f, randomZ);
        glRotatef(-90.0f, 1, 0, 0);
        glutSolidCone(0.5f, 2, 10, 10);
        glPopMatrix();

        glPushMatrix();
        glColor3ub(0, randGreen, 0);
        glTranslatef(randomX, 2.0f, randomZ);
        glRotatef(-90.0f, 1, 0, 0);
        glutSolidCone(2, randomHeight, 10, 10);
        glPopMatrix();
    }
	// End of frame
	
	
	// put code to draw scene in here
	
	
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

	case GLUT_KEY_PAGE_DOWN: radius -= 1.0f;
		if (radius < 1.0f)
			radius = 1.0f;
		break;

	case GLUT_KEY_PAGE_UP: radius += 1.0f; break;
	}
	spherical2Cartesian();
	glutPostRedisplay();

}


void printInfo() {

	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));

	printf("\nUse Arrows to move the camera up/down and left/right\n");
	printf("Home and End control the distance from the camera to the origin");
}


int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("TP5");
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);
	
// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	spherical2Cartesian();

	printInfo();

// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
