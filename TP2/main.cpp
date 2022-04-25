#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>

float angle = 0;
float transX = 0;
float transY = 0;
float transZ = 0;
float scaleX = 1, scaleY = 1, scaleZ = 1;

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


void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(5.0,5.0,5.0, 
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);


    glRotatef(angle, 0, 1, 0);
    glTranslatef(transX, transY, transZ);
    glScalef(scaleX, scaleY, scaleZ);

    // Drawing our axis
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

    glBegin(GL_TRIANGLES);
    //Base
    glColor3f(0.75f, 0.5f, 0.25f);
    glVertex3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, 0.0f, 0.0f);

    glColor3f(0.25f, 0.75f, 0.75f);
    glVertex3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, -1.0f);
    glVertex3f(1.0f, 0.0f, 0.0f);

    // Side 1
    glColor3f(0.5f, 0.25f, 0.25f);
    glVertex3f(1.0f, 0.0f, -1.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, 0.0f, 1.0f);

    // Side 2
    glColor3f(0.25f, 0.5f, 0.25f);
    glVertex3f(1.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, 0.0f, 1.0f);

    // Side 3
    glColor3f(0.25f, 0.25f, 0.5f);
    glVertex3f(-1.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, 0.0f, -1.0f);

    // Side 4
    glColor3f(0.25f, 0.25f, 0.25f);
    glVertex3f(-1.0f, 0.0f, -1.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, 0.0f, -1.0f);

    glEnd();

    // put the geometric transformations here

	// End of frame
	glutSwapBuffers();
}



// write function to process keyboard events


void normalKeys(unsigned char key, int x, int y){
    switch(key){
        case 'd':
            transX += 0.5;
            break;
        case 'a':
            transX -= 0.5;
            break;
        case 'w':
            transY += 0.5;
            break;
        case 's':
            transY -= 0.5;
            break;
        case 'q':
            transZ += 0.5;
            break;
        case 'e':
            transZ -= 0.5;
            break;
        case 'z':
            scaleX += 0.1;
            break;
        case 'x':
            scaleX -= 0.1;
            break;
        case 'c':
            scaleY += 0.1;
            break;
        case 'v':
            scaleY -= 0.1;
            break;
        case 'b':
            scaleZ += 0.1;
            break;
        case 'n':
            scaleZ -= 0.1;
            break;
        case 'u':
            angle += 15;
            break;
        case 'i':
            angle -= 15;
            break;
        case '+':
            scaleX += 0.1;
            scaleY += 0.1;
            scaleZ += 0.1;
            break;
        case '-':
            scaleX -= 0.1;
            scaleY -= 0.1;
            scaleZ -= 0.1;
            break;
    }
    glutPostRedisplay();
}



int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("TP2");
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);


// put here the registration of the keyboard callbacks
    glutKeyboardFunc(normalKeys);


//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
