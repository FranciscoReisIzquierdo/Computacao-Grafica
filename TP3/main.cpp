#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>


//Polar camPos = { sqrt(75), M_PI_4, M_PI_4 };


struct Polar{
    double radius; // Distance to the point
    float alpha; // Angle in the ZoX plane
    float beta; // Angle in the Y plane
};

float polarToX(Polar coordinate){ return coordinate.radius * cos(coordinate.beta) * sin(coordinate.alpha); }
float polarToY(Polar coordinate){ return coordinate.radius * sin(coordinate.beta); }
float polarToZ(Polar coordinate){ return coordinate.radius * cos(coordinate.alpha) * cos(coordinate.beta); }

Polar p = {sqrt(75), M_PI_4, M_PI_4 };


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


void drawCylinder(float radius, float height, int slices) {

// put code to draw cylinder in here
    float halfHeight = height/ 2.0;

    float gap = 2* M_PI / slices;

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

    for(int slice = 0; slice < 10; slice++){
        float angle = (float) slice * gap;
        float nextAngle = (float) (slice + 1) * gap;

        // Top base
        glBegin(GL_TRIANGLES);
        glColor3f(1.0f,0.12*slice,0.0f);
        glVertex3f(0.0f, halfHeight, 0.0f);
        glVertex3f(radius * sin(angle), halfHeight, radius * cos(angle));
        glVertex3f(radius * sin(nextAngle), halfHeight, radius * cos(nextAngle));
        glEnd();

        // Lateral
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
        glEnd();

        // Lower Base
        glBegin(GL_TRIANGLES);
        glColor3f(1.0f,0.12*slice,0.0f);
        glVertex3f(0.0f, -halfHeight, 0.0f);
        glVertex3f(radius * sin(nextAngle), -halfHeight, radius * cos(nextAngle));
        glVertex3f(radius * sin(angle), -halfHeight, radius * cos(angle));

        glEnd();

    }
}


void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
    gluLookAt(polarToX(p), polarToY(p), polarToZ(p),
              0.0,0.0,0.0,
              0.0f,p.beta > M_PI /2 ? -1.0f: 1.0f,0.0f);

	drawCylinder(1,2,10);

	// End of frame
	glutSwapBuffers();
}


void processKeys(unsigned char key, int x, int y) {
    switch(key) {
        case 'a':
            p.alpha -= 0.5;
            break;
        case 'd':
            p.alpha += 0.5;
            break;
        case 's':
            p.beta -= 0.5;
            break;
        case 'w':
            p.beta += 0.5;
            break;
    }

    // Beta must be between PI/2 and -PI/2
    if (p.alpha < 0) p.alpha += M_PI * 2;
    else if (p.alpha > M_PI * 2) p.alpha -= M_PI * 2;
    if (p.beta < - M_PI_2) p.beta += M_PI * 2;
    else if (p.beta > (3 * M_PI_2)) p.beta -= M_PI * 2;

    glutPostRedisplay();
}


void processSpecialKeys(int key, int xx, int yy) {

// put code to process special keys in here

}


int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("TP3");
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	
// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
