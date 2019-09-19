
//#include <GL/glew.h>
//#include <GL/wglew.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

/*
 * Display, idle functions from https://github.com/mikepack/lorenz/blob/master/lorenz.c
 * Special (arrow keys) and key functions based on ex6
 * Lorenz default parameters and equations from lorenz.c integrator on Moodle
 * Key Bindings:
 * [        decrease s
 * ]        increase s
 * {        decrease r
 * }        increase r
 * r        reset the drawing
 * d        set s/r back to default
 * arrows   change view angle
 * ESC      exit
 */
int nIter = 200000;
double th=0;  //  Rotation angle
float pts[200000][3];
static GLfloat view_rotx = 25.0;
static GLfloat view_roty = 50.0;
static GLfloat view_rotz = 35.0;
//static GLfloat view_posz = 60.0;
/*  Lorenz Parameters  */
double s  = 10;
double b  = 2.6666;
double r  = 28;

int iter = 0;
int iterInc = 10;

void display() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();

	glRotatef(view_rotx, 1.0, 0.0, 0.0);
	glRotatef(view_roty, 0.0, 1.0, 0.0);
	glRotatef(view_rotz, 0.0, 0.0, 1.0);

	glBegin(GL_LINE_STRIP);
	int i = 0;
	while( i < iter && i < nIter ) {
		glColor3fv(pts[i]);
		glVertex3fv(pts[i++]);
	}
	glEnd();
	if( iter < nIter ) {
		if( iter + iterInc > nIter ) iter = nIter;
		else iter+=iterInc;
	}

	glFlush();
	glutSwapBuffers();

	glPopMatrix();

}

void reshape(int width, int height) {
  GLfloat h = (GLfloat) height / (GLfloat) width;

  glViewport(0, 0, (GLint) width, (GLint) height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-5.0, 5.0, -h*2, h*2, 1.0, 300.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -60.0);
}

void lorenzGen() {
	int i;
	/*  Coordinates  */
	float x = pts[0][0] = 1;
	float y = pts[0][1] = 1;
	float z = pts[0][2] = 1;
	/*  Time step  */
	float dt = 0.001;

	for (i=0;i<nIter-1;i++)
	{
		float dx = s*(y-x);
		float dy = x*(r-z)-y;
		float dz = x*y - b*z;
		x += dt*dx;
		y += dt*dy;
		z += dt*dz;

		pts[i+1][0] = x;
		pts[i+1][1] = y;
		pts[i+1][2] = z;

	}
}
void special(int key, int x, int y) {
    //  Right arrow key
    if (key == GLUT_KEY_RIGHT){
        view_roty -= 5.0;
    }
    //  Left arrow key
    else if (key == GLUT_KEY_LEFT){
        view_roty += 5.0;
    }
    //  Up arrow key
    else if (key == GLUT_KEY_UP){
        view_rotx += 5.0;
    }
    //  Down arrow key
    else if (key == GLUT_KEY_DOWN){
        view_rotx -= 5.0;
    }
    else{
        return;
    }
  glutPostRedisplay();
}

void key(unsigned char ch, int x, int y) {
    //  Exit on ESC
    if (ch == 27)
      exit(0);
    else if (ch == ']'){
        s+=1.0;
    }
    else if (ch == '['){
        s-=1.0;
    }
    else if (ch == '}'){
        r+=1.0;
    }
    else if (ch == '{'){
        r-=1.0;
    }
    else if (ch == 'r'){
        iter = 0;
    }
    else if (ch == 'd'){
        s  = 10;
        r  = 28;
    }
    else{
        return;
    }
    lorenzGen();
	glutPostRedisplay();
}

void idle() {
	iter+=iterInc;
	glutPostRedisplay();
}


int main(int argc,char* argv[]) {
    lorenzGen();
    glutInit(&argc,argv);

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowPosition(50, 50);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Elizabeth Qiu - HW2");
    //glewInit();
    // My GPU runs uncapped by default, capping it here stabilizes the drawing
    //wglSwapIntervalEXT(1);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    //  Tell GLUT to call "special" when an arrow key is pressed
    glutSpecialFunc(special);
    //  Tell GLUT to call "key" when a key is pressed
    glutKeyboardFunc(key);
    glutIdleFunc(idle);
    glutMainLoop();

    return 0;
}
