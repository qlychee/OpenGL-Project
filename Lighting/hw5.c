/*
 * Elizabeth Qiu
 * HW5 Lighting
 * ex13.c and hw3/4
 */
#include "CSCIx229.h"
//not in CSCIx229.h so defined here
#define PI 3.1416
int th=0;         //  Azimuth of view angle
int ph=-5;         //  Elevation of view angle
int axes=0;       //  Display axes
int mode=0;       //  What to display
int FPS=60;
int fp=0;
int fov=85;       //  Field of view (for perspective)
int move=1;       //  Move light
int light=1;        //Lighting
double asp=1;     //  Aspect ratio
double dim=6.0;   //  Size of world
int rot = 0.0; //rotation var for first person

/* Eye coordinates */
double Ex = 0;
double Ey = 0;
double Ez = 5;
/* Camera coordinates*/
double Cx = 0;
double Cz = 0;

//Light values
int one       =   1;  // Unit value
int distance  =   9;  // Light distance
int inc       =  10;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shinyvec[1];    // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   0;  // Elevation of light


/*
 *  Draw vertex in polar coordinates with normal
 */
static void Vertex(double th,double ph)
{
   double x = Sin(th)*Cos(ph);
   double y = Cos(th)*Cos(ph);
   double z =         Sin(ph);
   //  For a sphere at the origin, the position
   //  and normal vectors are the same
   glNormal3d(x,y,z);
   glVertex3d(x,y,z);
}
/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius (r)
 */
void ball(double x,double y,double z,double r)
{
   int th,ph;
   float yellow[] = {1.0,1.0,0.0,1.0};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(1,1,1);
   glMaterialfv(GL_FRONT,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
   glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
   //  Bands of latitude
   for (ph=-90;ph<90;ph+=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*inc)
      {
         Vertex(th,ph);
         Vertex(th,ph+inc);
      }
      glEnd();
   }
   //  Undo transformations
   glPopMatrix();
}


void drawCylinder(double x, double y, double z, double scaleX, double scaleY, double scaleZ, double rotate) {
	double radius = 0.3;
	double height = 0.2;

	glPushMatrix();

	glTranslated(x,y,z);
	glScaled(scaleX, scaleY, scaleZ);
	glRotated(rotate,0,0,1);

	/* Side of cylinder */
	glColor3f(0.0, 0.0, 1.0);

	glBegin(GL_QUAD_STRIP);
    for(double i =0.0; i <=360; i+=.125){
        const float tc = (i / (float) 360);
        glNormal3f(cos(i), 0, sin(i));
        glTexCoord2f(-tc, 0.0);glVertex3d(radius * cos(i), height, radius * sin(i));
        glTexCoord2f(-tc, 2.0);glVertex3d(radius * cos(i), -height, radius * sin(i));

    }
	glEnd();

    /* Top and Bottom of Cylinder */
    glColor3f(0.498039,0.137255,1.0);
    glNormal3f( 0,-1, 0);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(0.0, height, 0.0);

	for(double i = 0.0; i < 2*PI*radius*4; i+=.1) {
		glVertex3d(radius * cos(i), height, radius * sin(i));
	}
	glEnd();
	glBegin(GL_TRIANGLE_FAN);
	glVertex3d(0.0, -height, 0.0);

	for(double i = 0.0; i < 2*PI*radius*4; i+=.1) {
		glVertex3d(radius * cos(i), -height, radius * sin(i));
	}
	glEnd();

	glPopMatrix();
}

static void drawRoof(double x, double y, double z, double dx, double dy, double dz,double th){

  const double rooftop = 2;
  const double roofoff = 1.5;
  const double roofwid = 1.25;
  const double roofbot = .75;
  // Set specular color to white
  float white[] = {1,1,1,1};
  float black[] = {0,0,0,1};
  glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
  glPushMatrix();
  // Translations
  glTranslated(x, y, z);
  glRotated(th, 0, 1, 0);
  glScaled(dx, dy, dz);
  // Roof
  glBegin(GL_TRIANGLES);
  // Front
  glColor3ub(30,30,30);
  glNormal3f(0,0,+1);
  glVertex3f(+roofoff,+roofbot,+roofwid);
  glVertex3f(-roofoff,+roofbot,+roofwid);
  glVertex3f(0,+rooftop,+roofwid);
  // Back
  glNormal3f(0,0,-1);
  glVertex3f(+roofoff,+roofbot,-roofwid);
  glVertex3f(-roofoff,+roofbot,-roofwid);
  glVertex3f(0,+rooftop,-roofwid);
  glEnd();
  // Sides of Roof
  glBegin(GL_QUADS);
  // Right side
  glColor3ub(50,50,50);
  glNormal3f(+1,+1,0);
  glVertex3f(+roofoff,+roofbot,-roofwid);
  glVertex3f(+roofoff,+roofbot,+roofwid);
  glVertex3f(0,+rooftop,+roofwid);
  glVertex3f(0,+rooftop,-roofwid);
  // Left Side
  glColor3ub(90,90,90);
  glNormal3f(-1,-1,0);
  glVertex3f(-roofoff,+roofbot,-roofwid);
  glVertex3f(-roofoff,+roofbot,+roofwid);
  glVertex3f(0,+rooftop,+roofwid);
  glVertex3f(0,+rooftop,-roofwid);
  // Bottom
  glColor3ub(153,51,0);
  glNormal3f(0,+1,0);
  glVertex3f(+roofoff,+roofbot,-roofwid);
  glVertex3f(+roofoff,+roofbot,+roofwid);
  glVertex3f(-roofoff,+roofbot,+roofwid);
  glVertex3f(-roofoff,+roofbot,-roofwid);
  glEnd();

   // Draw the chimney
  glBegin(GL_QUADS);
  // Left
  glColor3ub(60,0,0);
  glNormal3f(-1,0,0);
  glVertex3f(+.5,+1,+.25);
  glVertex3f(+.5,+1,-.25);
  glVertex3f(+.5,+2,-.25);
  glVertex3f(+.5,+2,+.25);
  // Right
  glColor3ub(60,0,0);
  glNormal3f(+1,0,0);
  glVertex3f(+1,+1,+.25);
  glVertex3f(+1,+1,-.25);
  glVertex3f(+1,+2,-.25);
  glVertex3f(+1,+2,+.25);
  // Back
  glColor3ub(100,0,0);
  glNormal3f(0,0,-1);
  glVertex3f(+.5,+1,-.25);
  glVertex3f(+.5,+2,-.25);
  glVertex3f(+1,+2,-.25);
  glVertex3f(+1,+1,-.25);
  // Front
  glColor3ub(100,0,0);
  glNormal3f(0,0,+1);
  glVertex3f(+.5,+1,+.25);
  glVertex3f(+.5,+2,+.25);
  glVertex3f(+1,+2,+.25);
  glVertex3f(+1,+1,+.25);
  // Top
  glColor3ub(10,10,10);
  glVertex3f(+.5,+2,+.25);
  glVertex3f(+.5,+2,-.25);
  glVertex3f(+1,+2,-.25);
  glVertex3f(+1,+2,+.25);
  glEnd();
  glPopMatrix();
}


static void drawHouse(double x, double y, double z, double dx, double dy, double dz,double th){

  const double rooftop = 2;
  const double roofoff = 1.5;
  const double roofwid = 1.25;
  const double roofbot = .75;
  // Set specular color to white
  float white[] = {1,1,1,1};
  float black[] = {0,0,0,1};
  glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
  glPushMatrix();

  // Translations
  glTranslated(x, y, z);
  glRotated(th, 0, 1, 0);
  glScaled(dx, dy, dz);

  // Body of house
  glBegin(GL_QUADS);
  //Back
  glColor3f(0.419608,0.137255,0.556863);
  glNormal3f( 0, 0, +1);
  glVertex3f(-1,-1, 1);
  glVertex3f(+1,-1, 1);
  glVertex3f(+1,+1, 1);
  glVertex3f(-1,+1, 1);
  // Front
  glColor3f(0.419608,0.137255,0.556863);
  glNormal3f( 0, 0, -1);
  glVertex3f(+1,-1,-1);
  glVertex3f(-1,-1,-1);
  glVertex3f(-1,+1,-1);
  glVertex3f(+1,+1,-1);
  //  Right
  glColor3f(0.498039,0.137255,1.0);
  glNormal3f( +1, 0, 0);
  glVertex3f(+1,-1,+1);
  glVertex3f(+1,-1,-1);
  glVertex3f(+1,+1,-1);
  glVertex3f(+1,+1,+1);
  //  Left
  glColor3f(0.498039,0.137255,1.0);
  glNormal3f( -1, 0, 0);
  glVertex3f(-1,-1,-1);
  glVertex3f(-1,-1,+1);
  glVertex3f(-1,+1,+1);
  glVertex3f(-1,+1,-1);
  //  Top
  glColor3f(0.498039,0.137255,1.0);
  glNormal3f( 0,+1, 0);
  glVertex3f(-1,+1,+1);
  glVertex3f(+1,+1,+1);
  glVertex3f(+1,+1,-1);
  glVertex3f(-1,+1,-1);
  //  Bottom
  glColor3f(0.419608,0.137255,0.7);
  glNormal3f( 0,-1, 0);
  glVertex3f(-1,-1,-1);
  glVertex3f(+1,-1,-1);
  glVertex3f(+1,-1,+1);
  glVertex3f(-1,-1,+1);
  glEnd();

  // Roof
  glBegin(GL_TRIANGLES);
  // Front
  glColor3ub(30,30,30);
  glNormal3f(0,0,+1);
  glVertex3f(+roofoff,+roofbot,+roofwid);
  glVertex3f(-roofoff,+roofbot,+roofwid);
  glVertex3f(0,+rooftop,+roofwid);
  // Back
  glNormal3f(0,0,-1);
  glVertex3f(+roofoff,+roofbot,-roofwid);
  glVertex3f(-roofoff,+roofbot,-roofwid);
  glVertex3f(0,+rooftop,-roofwid);
  glEnd();
  // Sides of Roof
  glBegin(GL_QUADS);
  // Right side
  glColor3ub(50,50,50);
  glNormal3f(+1,+1,0);
  glVertex3f(+roofoff,+roofbot,-roofwid);
  glVertex3f(+roofoff,+roofbot,+roofwid);
  glVertex3f(0,+rooftop,+roofwid);
  glVertex3f(0,+rooftop,-roofwid);
  // Left Side
  glColor3ub(90,90,90);
  glNormal3f(-1,-1,0);
  glVertex3f(-roofoff,+roofbot,-roofwid);
  glVertex3f(-roofoff,+roofbot,+roofwid);
  glVertex3f(0,+rooftop,+roofwid);
  glVertex3f(0,+rooftop,-roofwid);
  // Bottom
  glColor3ub(153,51,0);
  glNormal3f(0,+1,0);
  glVertex3f(+roofoff,+roofbot,-roofwid);
  glVertex3f(+roofoff,+roofbot,+roofwid);
  glVertex3f(-roofoff,+roofbot,+roofwid);
  glVertex3f(-roofoff,+roofbot,-roofwid);
  glEnd();

  // Draw the chimney
  glBegin(GL_QUADS);
  // Left
  glColor3ub(60,0,0);
  glNormal3f(-1,0,0);
  glVertex3f(+.5,+1,+.25);
  glVertex3f(+.5,+1,-.25);
  glVertex3f(+.5,+2,-.25);
  glVertex3f(+.5,+2,+.25);
  // Right
  glColor3ub(60,0,0);
  glNormal3f(+1,0,0);
  glVertex3f(+1,+1,+.25);
  glVertex3f(+1,+1,-.25);
  glVertex3f(+1,+2,-.25);
  glVertex3f(+1,+2,+.25);
  // Back
  glColor3ub(100,0,0);
  glNormal3f(0,0,-1);
  glVertex3f(+.5,+1,-.25);
  glVertex3f(+.5,+2,-.25);
  glVertex3f(+1,+2,-.25);
  glVertex3f(+1,+1,-.25);
  // Front
  glColor3ub(100,0,0);
  glNormal3f(0,0,+1);
  glVertex3f(+.5,+1,+.25);
  glVertex3f(+.5,+2,+.25);
  glVertex3f(+1,+2,+.25);
  glVertex3f(+1,+1,+.25);
  // Top
  glColor3ub(10,10,10);
  glVertex3f(+.5,+2,+.25);
  glVertex3f(+.5,+2,-.25);
  glVertex3f(+1,+2,-.25);
  glVertex3f(+1,+2,+.25);
  glEnd();


  // Draw a Door
  glBegin(GL_QUADS);
  glColor3ub(100,50,0);
  // Front
  glNormal3f(  0, 0, +1);
  glVertex3f(+.2,-1,1.1);
  glVertex3f(-.2,-1,1.1);
  glVertex3f(-.2,0,1.1);
  glVertex3f(+.2,0,1.1);
  // Top
  glNormal3f(  0, +1,0);
  glVertex3f(+.2,0,1.1);
  glVertex3f(+.2,0,1.0);
  glVertex3f(-.2,0,1.0);
  glVertex3f(-.2,0,1.1);
  // Right
  glNormal3f( +1, 0, 0);
  glVertex3f(+.2,0,1.1);
  glVertex3f(+.2,-1,1.1);
  glVertex3f(+.2,-1,1.0);
  glVertex3f(+.2,0,1.0);
  // Left
  glNormal3f( -1,0,  0);
  glVertex3f(-.2,0,1.1);
  glVertex3f(-.2,-1,1.1);
  glVertex3f(-.2,-1,1.0);
  glVertex3f(-.2,0,1.0);
  glEnd();

  glPopMatrix();
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   const double len=1.5;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();
   if (fp) {
        //Adjust the camera vector based on rot
      Cx = +2*dim*Sin(rot);
      Cz = -2*dim*Cos(rot);

      gluLookAt(Ex,Ey,Ez, Cx+Ex,Ey,Cz+Ez, 0,1,0);

   }
   else {
      //  Perspective - set eye position
      if (mode)
      {
         double px = -2*dim*Sin(th)*Cos(ph);
         double py = +2*dim        *Sin(ph);
         double pz = -2*dim*Cos(th)*Cos(ph);
         gluLookAt(px,py,pz , 0,0,0 , 0,Cos(ph),0);
      }
      //  Orthogonal - set world orientation
      else
      {
          glOrtho(-2.5, 2.5, -2.5, 2.5, -10, 10);
         glRotatef(ph,1,0,0);
         glRotatef(th,0,1,0);
      }
   }
    //  Flat or smooth shading
   glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);

   //  Light switch
   if (light)
   {
        //  Translate intensity to color vectors
        float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
        float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
        float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
        //  Light position
        float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
        //  Draw light position as ball (still no lighting here)
        glColor3f(1,1,1);
        ball(Position[0],Position[1],Position[2] , 0.1);
        //  OpenGL should normalize normal vectors
        glEnable(GL_NORMALIZE);
        //  Enable lighting
        glEnable(GL_LIGHTING);
        //  Location of viewer for specular calculations
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
        //  glColor sets ambient and diffuse color materials
        glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
        //  Enable light 0
        glEnable(GL_LIGHT0);
        //  Set ambient, diffuse, specular components and position of light 0
        glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
        glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
        glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
        glLightfv(GL_LIGHT0,GL_POSITION,Position);
   }
   else{
        glDisable(GL_LIGHTING);
   }
   drawHouse(-3,0,2 , 1,1,1 , 0);
   drawHouse(6.5,0,1 , 1.5,1,1 , 30);
   drawHouse(-6.5,0,1 , 1.5,1,1 , -30);
   drawCylinder(0,0,2.5, 4, 7, 4, 0);
   drawRoof(0,0.60,2.5,1,1,1,0);
   drawCylinder(3,0,2,4,5,4,0);
   drawRoof(3,0.20,2,1,1,1,0);

    /*
    not enough time to fix the lighting on these
   drawCylinder(0,5,-2, 4, 5, 4, 0);
   drawCylinder(5,5,-2, 4, 9, 3, 20);
   drawCylinder(-5,5,-2, 4, 9, 3, -20);
   */
   //  no lighting from here on
   glDisable(GL_LIGHTING);
   glColor3f(1,1,1);
   //  Draw axes
   if (axes)
   {
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }
   //  Five pixels from the lower left corner of the window
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s Light=%s",
     th,ph,dim,fov,mode?"Perspective":"Orthogonal",light?"On":"Off");
   if (light)
   {
      glWindowPos2i(5,45);
      Print("Model=%s LocalViewer=%s Distance=%d Elevation=%.1f",smooth?"Smooth":"Flat",local?"On":"Off",distance,ylight);
      glWindowPos2i(5,25);
      Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shinyvec[0]);
   }

   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}
/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
    if(!fp) {
       //  Right arrow key - increase angle by 5 degrees
       if (key == GLUT_KEY_RIGHT)
          th += 5;
       //  Left arrow key - decrease angle by 5 degrees
       else if (key == GLUT_KEY_LEFT)
          th -= 5;
       //  Up arrow key - increase elevation by 5 degrees
       else if (key == GLUT_KEY_UP)
          ph += 5;
       //  Down arrow key - decrease elevation by 5 degrees
       else if (key == GLUT_KEY_DOWN)
          ph -= 5;
      //  PageUp key - increase dim
        else if (key == GLUT_KEY_PAGE_UP && dim>1)
         dim -= 0.1;
        //  PageDown key - decrease dim
        else if (key == GLUT_KEY_PAGE_DOWN)
         dim += 0.1;
         /*unused in this implementation
        //  Smooth color model
       else if (key == GLUT_KEY_F1)
          smooth = 1-smooth;
       //  Local Viewer
       else if (key == GLUT_KEY_F2)
          local = 1-local;
       else if (key == GLUT_KEY_F3)
          distance = (distance==1) ? 5 : 1;
       //  Toggle ball increment
       else if (key == GLUT_KEY_F8)
          inc = (inc==10)?3:10;
       //  Flip sign
       else if (key == GLUT_KEY_F9)
          one = -one;
        */
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project(mode?fov:0,asp,dim,fp);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
    }
    else{
     //control movement in first person
      double dt = 0.05;
      if (key == GLUT_KEY_UP){
         Ex += Cx*dt; //Update the eye vector based on the camera vector
         Ez += Cz*dt;
      }
      else if (key == GLUT_KEY_LEFT){
         rot -= 3;
      }
      else if (key == GLUT_KEY_DOWN){
         Ex -= Cx*dt;
         Ez -= Cz*dt;
      }
      else if (key == GLUT_KEY_RIGHT){
         rot += 3;
      }

      //  Keep angles to +/-360 degrees
      rot %= 360;
   }
    //  Update projection
   Project(mode?fov:0,asp,dim,fp);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Toggle axes
   else if (ch == 'a' || ch == 'A'){
      axes = 1-axes;
   }/*
    else if (ch == 'n' || ch == 'N'){
      fp = 1-fp;
   }*/
   else if (ch == 'l' || ch == 'L'){
      light = 1-light;
   }
   //  Toggle light movement
   else if (ch == 'k' || ch == 'K')
      move = 1-move;
   //  Move light
   else if (ch == '<')
      zh += 1;
   else if (ch == '>')
      zh -= 1;
      //  Light elevation
   else if (ch==')')
      ylight += 0.1;
   else if (ch=='(')
      ylight -= 0.1;
   if(!fp) {
      //  Reset view angle
      if (ch == '0'){
         th = 0;
         ph = -5;
      }
      //  Switch display mode
      else if (ch == 'm' || ch == 'M')
         mode = 1-mode;
      //  Change field of view angle
      else if (ch == '[')
         fov--;
      else if (ch == ']')
         fov++;
   }
    //  Translate shininess power to value (-1 => 0)
   shinyvec[0] = shininess<0 ? 0 : pow(2.0,shininess);
   //  Reproject
   Project(mode?fov:0,asp,dim,fp);
   //  Animate if requested
   glutIdleFunc(move?idle:NULL);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project(mode?fov:0,asp,dim,fp);
}


/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT and process user parameters
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitWindowSize(600,600);
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   //  Create the window
   glutCreateWindow("Elizabeth Qiu- HW5");
   //  Tell GLUT to call "idle" when there is nothing else to do
   glutIdleFunc(idle);
   //  Tell GLUT to call "display" when the scene should be drawn
   glutDisplayFunc(display);
   //  Tell GLUT to call "reshape" when the window is resized
   glutReshapeFunc(reshape);
   //  Tell GLUT to call "special" when an arrow key is pressed
   glutSpecialFunc(special);
   //  Tell GLUT to call "key" when a key is pressed
   glutKeyboardFunc(key);
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   return 0;
}
