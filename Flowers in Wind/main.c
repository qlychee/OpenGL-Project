/*
 * Elizabeth Qiu
 * Final Project
 * 
 * Credit:
 * Adapted from Wind Turbine/Particle Sim by Thomas Spooner
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "CSCIx229.h"
#include "FrameTimer.h"

#define cylAngle atan(radius2/height);
#define PI 3.14159265358979324

int obj;

unsigned int texture[3];
unsigned int sky[6];
unsigned int testSky;

int NUM_FLOWERS  = 49;
#define MAX_FLOWERS 50
int wind = 0;

int thetaWind = 0;
double speedWind = 0;
int windChange = 0;

int th=0;         //  Azimuth of view angle
int ph=15;
double zh = 0;
int axes = 0;
int moveLight = 1;
int texmode = 0;
int mode = 1;
int fov = 55;//50
int box = 1;
double asp = 1;
double dim = 6;    //Size of world
double zoom = .8;
double rot[10];

//Light Values
int move = 1;         //Move light
int light = 1;        //Lighting
int emission = 0;     //Emission Intensity
int ambient = 30;     //Ambient Intensity
int diffuse = 100;    //Diffuse Intensity
int specular = 0;     //Specular Intensity
int shininess = 2;    //Shininess (Power of two)
float shinyvec[1];    //Shininess (Value)
int zht   = -40;        //Light azimuth
float ylight = 7;     //Elevation of light

int one  = 1;         //Unit value
int distance = 10;     //Light Distance
int inc = 10;         //Ball Increment
int smooth = 1;       //Smooth/Flat Shading
int local   = 0;      //Local viewer model

double spin = 0.0;
double speed = 0.0;

#define Dfloor 7.5
#define Yfloor 0.0001
float N[] = {0, -1, 0};
float E[] = {0, Yfloor, 0};

//Fog

/// the colour of the fog and the clear colour.
float  fog_colour[] = {0.6f,0.58f,0.79f,0.0f};

/// the density of the fog
float  fog_density  = 0.02f;

/// The fog mode, either GL_LINEAR, GL_EXP or GL_EXP2
GLenum fog_mode     = GL_EXP2;

/// the far distance of the fog & camera
float far_distance  = 10.0f;

/// the near distance of the fog & camera
float near_distance = 0.05f;

//Camera
float zoom2 = 15.0f;
float rotx = 15.0;
float roty = 180.0;
float tx = 0;
float ty = 0;
int lastx = 0;
int lasty = 0;
unsigned char Buttons[3] = {0};

//Flower
double flow = 0.0;
float angle = 0;
int petalNum = 30;
int sway = 0;
int swaySwitch = 0;
struct flowers {
   //Placement in xyz space
   double x;
   double y;
   double z;
   int type;
   
};
struct flowers flower[MAX_FLOWERS];
//colors
struct colors {
	double r;
	double g;
	double b;

};
struct colors color[4];
int colorInd = 0;
// a structure to hold a particle
struct Particle {
   // the current particle position
   float position[3];
   // the direction of the particle
   float direction[3];
   float color[3];
   // the lifespan
   float life;
   // pointer to the next particle
   struct Particle* next;
};

typedef struct Particle Object;

Object* Particle_new();

Object* Particle_new() {
   Object* p = malloc(sizeof(Object));
   p->position[0] = (float) (rand() % 16);
   p->position[2] = 0;
   p->position[1] = 3.0f;

   p->direction[0] = (10000 - rand()%20000)/10000.0f;
   p->direction[1] = (10000 - rand()%20000)/10000.0f;
   p->direction[2] = (10000 - rand()%20000)/10000.0f; 
   
   int value = rand()%2;
   if (value == 0){
	   p->color[0] = color[colorInd].r;
	   p->color[1] = color[colorInd].g;//rand() % 15000/20000.0f;
	   p->color[2] = color[colorInd].b;
	}else{
		p->color[0] = 1;
	   p->color[1] = 1;
	   p->color[2] = 1;
	}
   

   p->life         = rand()%10000/1000.0f;   
   return p;
}

void ShadowProjection(float L[4], float E[4], float N[4])
{
   float mat[16];
   float e = E[0]*N[0] + E[1]*N[1] + E[2]*N[2];
   float l = L[0]*N[0] + L[1]*N[1] + L[2]*N[2];
   float c = e - l;
   //  Create the matrix.
   mat[0] = N[0]*L[0]+c; mat[4] = N[1]*L[0];   mat[8]  = N[2]*L[0];   mat[12] = -e*L[0];
   mat[1] = N[0]*L[1];   mat[5] = N[1]*L[1]+c; mat[9]  = N[2]*L[1];   mat[13] = -e*L[1];
   mat[2] = N[0]*L[2];   mat[6] = N[1]*L[2];   mat[10] = N[2]*L[2]+c; mat[14] = -e*L[2];
   mat[3] = N[0];        mat[7] = N[1];        mat[11] = N[2];        mat[15] = -l;
   //  Multiply modelview matrix
   glMultMatrixf(mat);
}

/// the first particle in the linked list
struct Particle* pList;

void NewParticle() {

   // create new particle and add as first in list
   Object* p = Particle_new();

   p->next = pList;
   pList = p;
}

void UpdateParticles(float dt) {

   // traverse all particles and update
   Object* p = pList;
   while(p) {
      // decrease lifespan
      p->life -= dt;

      // apply gravity
      p->direction[1] -= 9.81f*dt;

      // modify position
      p->position[0] += dt * p->direction[0];
      p->position[1] += 0.5 * dt * p->direction[1];
      p->position[2] += 0.5 * dt * p->direction[2];

      // goto next particle
      p=p->next;
   }
}

void RemoveDeadParticles() {

   // iterate over particles
   Object* curr = pList;
   Object* prev = 0;

   while (curr) {

      // if dead
      if (curr->life<0) {

         // update the previous pointer to skip over the curr 
         // particle, or just remove the particle if it's the 
         // first in the list that we need to remove.
         //
         if (prev) {
            prev->next = curr->next;
         }
         else {
            pList = curr->next;
         }

         // take temporary reference
         Object* temp = curr;

         // skip over particle in list
         curr = curr->next;

         // delete particle
         free(temp);
      }
      else {
         // move to next if not removing
         prev = curr;
         curr = curr->next;
      }
   }
}

void DrawParticles() {
   glPushMatrix();
   // iterate over all particles and draw a point
   Object* curr = pList;
   glTranslatef(-8, 0, 18);
   glRotatef(90, 1, 0, 0);
   glPointSize(3);
   glBegin(GL_POINTS);
   //glColor3f(1, 0, 0);
   while (curr) {
      glColor3fv(curr->color);
      glVertex3fv(curr->position);
      curr = curr->next;
   }
   glEnd();
   glPopMatrix();
}

/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  //  Maximum length of text string
void Print(const char* format , ...)
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

/*
 *  Draw vertex in polar coordinates
 */
static void Vertex(double th,double ph)
{
   double x = Sin(th)*Cos(ph);
   double y = Cos(th)*Cos(ph);
   double z =         Sin(ph);

   glNormal3d(x,y,z);
   glTexCoord2f(th/360, ph/180 +.5);
   glVertex3d(x,y,z);
}

static void ball(double x, double y, double z, double r)
{
   int th, ph;
   float yellow[] = {1.0, 1.0, 0.0, 1.0};
   float Emission[] = {0.0, 0.0, 0.01 * emission, 1.0};
   //Save trans
   glPushMatrix();

   glTranslated(x,y,z);
   glScaled(r, r, r);

   //White ball
   glColor3f(1, 1, 1);
   glMaterialfv(GL_FRONT, GL_SHININESS, shinyvec);
   glMaterialfv(GL_FRONT, GL_SPECULAR, yellow);
   glMaterialfv(GL_FRONT, GL_EMISSION, Emission);
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,texmode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[1]);   
   // Bands of latitude
   for (ph= -90; ph < 90; ph +=inc)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=2*inc)
      {
         Vertex(th,ph);
         Vertex(th,ph+inc);
      }
      glEnd();
   }
   glPopMatrix();
}


// Routine to draw hemisphere.
void drawHemisphere(float radius, int longSlices, int latSlices)
{
   int  i, j;
   for(j = 0; j < latSlices; j++)
   {
      // One latitudinal triangle strip.
      glBegin(GL_TRIANGLE_STRIP);
      for(i = 0; i <= longSlices; i++)
      {
         glVertex3f( radius * cos( (float)(j+1)/latSlices * PI/2.0 ) * cos( 2.0 * (float)i/longSlices * PI ),
                     radius * sin( (float)(j+1)/latSlices * PI/2.0 ),
					 radius * cos( (float)(j+1)/latSlices * PI/2.0 ) * sin( 2.0 * (float)i/longSlices * PI ) );
         glVertex3f( radius * cos( (float)j/latSlices * PI/2.0 ) * cos( 2.0 * (float)i/longSlices * PI ),
                     radius * sin( (float)j/latSlices * PI/2.0 ),
					 radius * cos( (float)j/latSlices * PI/2.0 ) * sin( 2.0 * (float)i/longSlices * PI ) );         
	  }
      glEnd();
   }
}
// Draw lengthy box that will serve as ground for objects
static void drawGround(double x, double y, double z, unsigned int texnum)
{
   glPushMatrix();

   glTranslated(0, -0.1, 0);   
   glScaled(x, y, z);

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, texmode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D, texture[texnum]);

   glBegin(GL_QUADS);
   glColor3f(1, 1, 1);

   //Front
   glNormal3f(0,0,+1);   
   glVertex3f(-1,-1, 1);
   glVertex3f(+1,-1, 1);
   glVertex3f(+1,+1, 1);
   glVertex3f(-1,+1, 1);
   //  Back
   glNormal3f(0,0,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,+1,-1);
   glVertex3f(+1,+1,-1);
   //  Right
   glNormal3f(+1, 0, 0);
   glVertex3f(+1,-1,+1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,+1,-1);
   glVertex3f(+1,+1,+1);
   //  Left
   glNormal3f(-1, 0, 0);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,-1,+1);
   glVertex3f(-1,+1,+1);
   glVertex3f(-1,+1,-1);
   //  Top
   glNormal3f(0,+1,0);
   glTexCoord2f(0,0); glVertex3f(-1,+1,+1);
   glTexCoord2f(1,0); glVertex3f(+0,+1,+1);
   glTexCoord2f(1,1); glVertex3f(+0,+1,0);
   glTexCoord2f(0,1); glVertex3f(-1,+1,0);

   glNormal3f(0,+1,0);
   glTexCoord2f(0,0); glVertex3f(0,+1,0);
   glTexCoord2f(1,0); glVertex3f(+0,+1,+1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,+1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,0);

   glNormal3f(0,+1,0);
   glTexCoord2f(0,0); glVertex3f(0,+1,0);
   glTexCoord2f(1,0); glVertex3f(+1,+1,0);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(0,+1,-1);

   glNormal3f(0,+1,0);
   glTexCoord2f(0,0); glVertex3f(0,1,0);
   glTexCoord2f(1,0); glVertex3f(0,+1,-1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,0);         
   //  Bottom
   glNormal3f(0, -1, 0);
   glVertex3f(-1,-1,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,-1,+1);
   glVertex3f(-1,-1,+1); 
   glEnd();  

   glPopMatrix();
   glDisable(GL_TEXTURE_2D);
}

static void drawPetals(double x, double y, double z) {

   double h, phi = 0;
	for (int k = 1; k <= petalNum; ++k)
	{
		glPushMatrix();
		glColor3f(1,1,1);
		h = -1.0 + 0.5*k / petalNum;
		//h = -0.8;
		phi = phi + 3.6 / sqrt((double)petalNum)*(1.0 / sqrt((double)1 - pow(h, 2.0)));
		phi = fmod((double)phi, (double)(2 * PI));
		glTranslated(x, y, z);
		glRotatef(270, 1, 0, 0);
		glRotatef(-90, 0, 1, 0);
		glRotatef(phi / (2.0*PI)*360.0f, 1, 0, 0);
		glRotatef(acos(h) / (2.0*PI)*360.0f, 0, 1, 0);
		glTranslatef(0, 0, 1.5);
		glScalef(1, 2.5, 7);
		glutSolidSphere(1.5, 8, 8);
		glPopMatrix();
	}

}
static void drawPetals2(double x, double y, double z) {
	
   double h, phi = 0;
	for (int k = 1; k <= petalNum; ++k)
	{
		glPushMatrix();
		glColor3f(color[colorInd].r,color[colorInd].g,color[colorInd].b);
		h = -1.0 + 0.5*k / petalNum;
		//h = -0.8;
		phi = phi + 3.6 / sqrt((double)petalNum)*(1.0 / sqrt((double)1 - pow(h, 2.0)));
		phi = fmod((double)phi, (double)(2 * PI));
		glTranslated(x, y, z);
		glRotatef(270, 1, 0, 0);
		glRotatef(-90, 0, 1, 0);
		glRotatef(phi / (2.0*PI)*360.0f, 1, 0, 0);
		glRotatef(acos(h) / (2.0*PI)*360.0f, 0, 1, 0);
		glTranslatef(0, 0, 1.5);
		glScalef(1, 2.5, 7);
		glutSolidSphere(1.5, 8, 8);
		glPopMatrix();
	}
	
}

static void drawCenter(double x, double y, double z)
{
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslatef(x, y+3, z);
   glColor3f(0.36,0.25, 0.20);
	glutSolidSphere(5, 8, 8);
	
   glPopMatrix();
}
static void drawStem(double radius, double height, double x, double y, double z)
{
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shinyvec);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
   glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);
   glColor3f(0.184314,0.309804, 0.184314);
	glPushMatrix();
	glTranslatef(x, y, z);	
	glRotatef(-90, 1, 0, 0);
	glColor3f(0.184314,0.309804, 0.184314);
	/*
	glutSolidCone(radius, height, 5, 5);
	*/
	GLUquadricObj *quadratic=gluNewQuadric();
    gluQuadricNormals(quadratic, GLU_SMOOTH);
    gluQuadricTexture(quadratic, GL_TRUE);

    gluCylinder(quadratic,radius,radius/2,height,8,8);
	glPopMatrix();
}
static void drawFlower(struct flowers f)
{
   glPushMatrix();
   glTranslated(f.x, f.y, f.z);
   glScaled(0.016, 0.016, 0.016);
   glRotatef(angle,Cos(thetaWind),0,-Sin(thetaWind));
	double h = 40;
   drawStem(2, h, 0, 0, 0);
   drawCenter(0,h,0);
   drawPetals(0,h,0);
   glPopMatrix();
}
static void drawFlower2(struct flowers f)
{
   glPushMatrix();
   glTranslated(f.x, f.y, f.z);
   glScaled(0.016, 0.016, 0.016);
   glRotatef(angle,Cos(thetaWind),0,-Sin(thetaWind));
	double h = 40;
   drawStem(2, h, 0, 0, 0);
   drawCenter(0,h,0);
   drawPetals2(0,h,0);
   glPopMatrix();
}

static void Sky(double D)
{
   glPushMatrix();
   glColor3f(1,1,1);
   glEnable(GL_TEXTURE_2D);
   //glTranslatef(0, 0.5*D, 0);
   //  Sides
   glBindTexture(GL_TEXTURE_2D,testSky);
   glBegin(GL_QUADS);
   glTexCoord2f(0.0,0.34); glVertex3f(-D,-D,-D);
   glTexCoord2f(0.25,0.34); glVertex3f(+D,-D,-D);
   glTexCoord2f(0.25,0.66); glVertex3f(+D,+D,-D);
   glTexCoord2f(0.0,0.66); glVertex3f(-D,+D,-D);
   glEnd();

   //glBindTexture(GL_TEXTURE_2D,sky[3]);
   glBegin(GL_QUADS);
   glTexCoord2f(0.25,0.34); glVertex3f(+D,-D,-D);
   glTexCoord2f(0.5,0.34); glVertex3f(+D,-D,+D);
   glTexCoord2f(0.5,0.66); glVertex3f(+D,+D,+D);
   glTexCoord2f(0.25,0.66); glVertex3f(+D,+D,-D);
   glEnd();

   //glBindTexture(GL_TEXTURE_2D,sky[2]);
   glBegin(GL_QUADS);
   glTexCoord2f(0.5,0.34); glVertex3f(+D,-D,+D);
   glTexCoord2f(0.75,0.34); glVertex3f(-D,-D,+D);
   glTexCoord2f(0.75,0.66); glVertex3f(-D,+D,+D);
   glTexCoord2f(0.5,0.66); glVertex3f(+D,+D,+D);
   glEnd();

   //glBindTexture(GL_TEXTURE_2D,sky[4]);
   glBegin(GL_QUADS);
   glTexCoord2f(0.75,0.34); glVertex3f(-D,-D,+D);
   glTexCoord2f(1   ,0.34); glVertex3f(-D,-D,-D);
   glTexCoord2f(1   ,0.66); glVertex3f(-D,+D,-D);
   glTexCoord2f(0.75,0.66); glVertex3f(-D,+D,+D);
   glEnd();
   //  Top and bottom
   //glBindTexture(GL_TEXTURE_2D,sky[0]);
   glBegin(GL_QUADS);
   glTexCoord2f(0.25,0.67); glVertex3f(+D,+D,-D);
   glTexCoord2f(0.50,0.67); glVertex3f(+D,+D,+D);
   glTexCoord2f(0.50,1); glVertex3f(-D,+D,+D);
   glTexCoord2f(0.25,1); glVertex3f(-D,+D,-D);
   glEnd();
   glPopMatrix();

//   glBindTexture(GL_TEXTURE_2D,sky[5]);
   glBegin(GL_QUADS);
   glTexCoord2f(0.25,0); glVertex3f(-D,-D,+D);
   glTexCoord2f(0.5,0); glVertex3f(+D,-D,+D);
   glTexCoord2f(0.5,0.34); glVertex3f(+D,-D,-D);
   glTexCoord2f(0.25,0.34); glVertex3f(-D,-D,-D);
   glEnd();

   glDisable(GL_TEXTURE_2D);
}
/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   const double len=4;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
   // Set background color
   glClearColor(fog_colour[0], fog_colour[1], fog_colour[2], fog_colour[3]);
   // Set the fog attribs
   glFogf(GL_FOG_START, near_distance);
   glFogf(GL_FOG_END, far_distance);
   glFogfv(GL_FOG_COLOR, fog_colour);
   glFogi(GL_FOG_MODE, fog_mode);
   glFogf(GL_FOG_DENSITY, fog_density);
   glEnable(GL_FOG);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();
   if (mode)
   {
      double Ex = -2*dim*Sin(roty)*Cos(rotx);
      double Ey = +2*dim *Sin(rotx);
      double Ez = +2*dim*Cos(roty)*Cos(rotx);
      gluLookAt(Ex,Ey,Ez, 0,0,0, 0,Cos(rotx),0);      
   }
   else 
   {
      //  Set view angle
      glRotatef(ph,1,0,0);
      glRotatef(th,0,1,0);
   }

   if(box)
      Sky(3*dim);

   glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);

  float Ambient[]   = {0.01 * ambient, 0.01 * ambient, 0.01 * ambient, 1.0};
  float Diffuse[]   = {0.01 * diffuse, 0.01 * diffuse, 0.01 * diffuse, 1.0};
  float Specular[]  = {0.01* specular, 0.01* specular, 0.01* specular, 1.0};
  //Light Position
  float Position[]  = {distance * Cos(zht), ylight, distance * Sin(zht), 1.0};
  glColor3f(1,1,1);
  ball(Position[0], Position[1] + 3, Position[2], 0.1);
  glEnable(GL_NORMALIZE);
  glEnable(GL_LIGHTING);
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_AMBIENT, Ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, Diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, Specular);
  glLightfv(GL_LIGHT0, GL_POSITION, Position);
   drawGround(7.5, 0.1, 7.5, 0);

   for (int i = 0; i < NUM_FLOWERS; i++) {
	  if(flower[i].type==1){
		  drawFlower(flower[i]);
	  }else{
		  drawFlower2(flower[i]);
	  } 
   } 


   if(wind) {
      glPushMatrix();
      glRotatef(thetaWind, 0, 1, 0);
      DrawParticles();
      glPopMatrix();
   }

   glPushAttrib(GL_ENABLE_BIT);

   glDisable(GL_LIGHTING);

   glEnable(GL_STENCIL_TEST);

   glStencilFunc(GL_ALWAYS,1,0xFFFFFFFF);

   glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);

   glDepthMask(0);
   glColorMask(0,0,0,0);

   glPushMatrix();
    ShadowProjection(Position, E, N);

   glPopMatrix();

   glDepthMask(1);
   glColorMask(1,1,1,1);

   glStencilFunc(GL_LESS,0,0xFFFFFFFF);

   glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
   glColor4f(0,0,0,0.5);

   glBegin(GL_QUADS);
      glVertex3f(-Dfloor,Yfloor,-Dfloor);
      glVertex3f(+Dfloor,Yfloor,-Dfloor);
      glVertex3f(+Dfloor,Yfloor,+Dfloor);
      glVertex3f(-Dfloor,Yfloor,+Dfloor);
   glEnd();

   glPopAttrib();

   //  White
   glColor3f(1,1,1);
   //  Draw axes
   if (axes) {
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

   glWindowPos2i(5,5);
   Print("Angle=%0.f,%.0f  Dim=%.1f FOV=%d Projection=%s Light=%s",
     rotx,roty,dim,fov,mode?"Perpective":"Orthogonal",light?"On":"Off");
   ErrCheck("display");

   //  Render the scene
   //glPopMatrix();
   glFlush();
   //  Make the rendered scene visible
   glutSwapBuffers();
} 

void idle()
{

   double t = glutGet(GLUT_ELAPSED_TIME)/5000.0;
   sway++; //trying to control sway animation
   if(sway == 300){//reset
	   sway = 0;
   }
   swaySwitch = 1-swaySwitch;
   zh = fmod(180*t,360);
   int i;
   // update the frame time
   SortFrameTimer();
   int val = 10;
   // create a new particle every frame
   if(speedWind > 0)
      val = rand()%(5 * (int) speedWind);
   for(i = 0; i < val; i++)
      NewParticle();
   // update the particle simulation
   UpdateParticles(FrameTime());

   // remove any dead particles
   RemoveDeadParticles();   
   
   if(wind && speedWind <40){
	   if(sway%3==0){
		   if(swaySwitch)
		       angle +=2;
		   else
			   angle -=2;
		}
	}
	if(speedWind >= 40){
		if(swaySwitch)
		   angle +=2;
	   else
		   angle -=2;
	}
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
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
   else if (key == GLUT_KEY_DOWN && ph > 10)
      ph -= 5;


   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //thetaWind %= 360;
   //  Tell GLUT it is necessary to redisplay the scene
   Project(mode?fov:0,asp,dim);
   glutPostRedisplay();
}

void key(unsigned char ch, int x, int y)
{
   if (ch == 27)
      exit(0);
   else if (ch == '0')
      th = ph = 0;
   else if (ch == 'm' || ch == 'M')
      mode = 1 - mode;
   else if (ch == 'x' || ch == 'X')
      axes = 1 - axes;
   else if (ch == 'l' || ch == 'L')
      light = 1 - light;
   else if (ch == 'c' || ch == 'C'){
	   colorInd ++;
	   if(colorInd == 4){
		   colorInd =0; //reset
       }
   }
   else if (ch == '-' || ch == '_')
      fov --;
   else if (ch == '+' || ch == '=')
      fov ++;
   else if (ch == '[' && NUM_FLOWERS >=0)
      NUM_FLOWERS--;
   else if (ch == ']' && NUM_FLOWERS <=49)
      NUM_FLOWERS++;
   else if (ch == '>') {
      thetaWind += 5;
      thetaWind = thetaWind % 360;
      //flow -= 0.05;
      if(wind)
         windChange = 1;
   }
   else if (ch == '<') {
      thetaWind -= 5;
      thetaWind = thetaWind % 360;
      //flow += 0.05;
      if(wind)
         windChange = 1;      
   }
   else if (ch == ')' && speedWind < 80) {
      speedWind += 2;
      angle = -speedWind;
      wind = 1;
      windChange = 1;
      fog_density += 0.001;
      if(petalNum >0)
		petalNum--;
   }
   else if (ch == '(' && speedWind > 0) {
      speedWind -= 2;
      angle = -speedWind;
      windChange = 1;
      if(speedWind == 0)
         wind = 0;

      fog_density -= 0.001;
      if(petalNum <30)
		petalNum++;

   }
   //Ambient Light
   else if (ch == 'a' && ambient > 0)
      ambient -= 5;
   else if (ch == 'A' && ambient < 100)
      ambient += 5;
   //  Diffuse level
   else if (ch=='d' && diffuse>0)
      diffuse -= 5;
   else if (ch=='D' && diffuse<100)
      diffuse += 5;
   //  Specular level
   else if (ch=='s' && specular>0)
      specular -= 5;
   else if (ch=='S' && specular<100)
      specular += 5;
   //  Emission level
   else if (ch=='e' && emission>0)
      emission -= 5;
   else if (ch=='E' && emission<100)
      emission += 5;
   //  Shininess level
   else if (ch=='n' && shininess>-1)
      shininess -= 1;
   else if (ch=='N' && shininess<7)
      shininess += 1;   
   else if (ch == 'q' || 'Q')
      moveLight = 1 - moveLight;
   else if (ch == 'x' || 'X')
      axes = 1 - axes;

   // Translate shinieness power to value (-1 => 0)
   shinyvec[0] = shininess<0 ? 0 : pow(2.0, shininess);
   // Reproject
   Project(mode?fov:0,asp,dim);
   //Tell GLUT it is necessary to redisplay the scene
   glutIdleFunc(move?idle:NULL);

   glutPostRedisplay();
}

void Motion(int x,int y)
{
   int diffx=x-lastx;
   int diffy=y-lasty;
   lastx=x;
   lasty=y;

   if( Buttons[0] && Buttons[1] )
   {
      zoom2 -= (float) 0.05f * diffx;
   }
   else
      if( Buttons[0])
      {

         if (rotx > 5 && rotx < 175)
            rotx += (float) 0.5f * diffy;
         else if ((rotx <= 5 && diffy > 0) || (rotx >= 175 && diffy < 0))
            rotx += (float) 0.5f * diffy;

         roty += (float) 0.5f * diffx;    
      }
      else
         if( Buttons[1] )
         {
            tx += (float) 0.05f * diffx;
            ty -= (float) 0.05f * diffy;
         }
         glutPostRedisplay();
}
void Mouse(int b, int s, int x, int y)
{
   lastx = x;
   lasty = y;

   switch(b)
   {
      case GLUT_LEFT_BUTTON:
         Buttons[0] = ((GLUT_DOWN==s)?1:0);
         break;
      case GLUT_RIGHT_BUTTON:
         Buttons[1] = ((GLUT_DOWN==s)?1:0);
      default:
         break;
   }
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   asp = (height > 0) ? (double)width/height : 1;
   glViewport(0, 0, width, height);
   Project(mode?fov:0,asp,dim);
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
	//initilize colors
	color[0].r=0.8;color[0].g=0.7;color[0].b=0.2;
	color[1].r=0;color[1].g=0.0;color[1].b=1; //blue
	color[2].r=1;color[2].g=0;color[2].b=1;//magenta
	color[3].r=1;color[3].g=0;color[3].b=0; //red
	//initialize flowers
	int fx,fz;
	fx = -6;
	fz = -6;
  for (int i = 0; i < 13; i++) {
	  flower[i].x = fx; flower[i].y = 0; flower[i].z = fz;
	  fx++;
	  fz++;
	  if(i%2 == 0){
		  flower[i].type = 1;
	  }else{
		  flower[i].type = 2;
	  }
   }
   fx = -6;
   fz = 6;
   for (int i = 13; i < 19; i++) {
	  flower[i].x = fx; flower[i].y = 0; flower[i].z = fz;
	  fx++;
	  fz--;
	  if(i%2 == 1){
		  flower[i].type = 1;
	  }else{
		  flower[i].type = 2;
	  }
   }
   fx = 1;
   fz = -1;
   for (int i = 19; i < 25; i++) {
	  flower[i].x = fx; flower[i].y = 0; flower[i].z = fz;
	  fx++;
	  fz--;
	  if(i%2 == 0){
		  flower[i].type = 1;
	  }else{
		  flower[i].type = 2;
	  } 
   }       
   fz = -1;
   for (int i = 25; i < 31; i++) {
	  flower[i].x = 0; flower[i].y = 0; flower[i].z = fz;
	  fz--;
	  if(i%2 == 0){
		  flower[i].type = 1;
	  }else{
		  flower[i].type = 2;
	  }
   }    
   fz = 1;
   for (int i = 31; i < 37; i++) {
	  flower[i].x = 0; flower[i].y = 0; flower[i].z = fz;
	  fz++;
	  if(i%2 == 0){
		  flower[i].type = 1;
	  }else{
		  flower[i].type = 2;
	  } 
   }    
   fx = -1;
   for (int i = 37; i < 43; i++) {
	  flower[i].x = fx; flower[i].y = 0; flower[i].z = 0;
	  fx--;
	  if(i%2 == 0){
		  flower[i].type = 1;
	  }else{
		  flower[i].type = 2;
	  }
   }   
   fx = 1;
   for (int i = 43; i < 49; i++) {
	  flower[i].x = fx; flower[i].y = 0; flower[i].z = 0;
	  fx++;
	  if(i%2 == 0){
		  flower[i].type = 1;
	  }else{
		  flower[i].type = 2;
	  }
   } 
   //  Initialize GLUT and process user parameters
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitWindowSize(1024,600);
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   //  Create the window
   glutCreateWindow("Elizabeth Qiu - Final Project");
   //  Tell GLUT to call "idle" when there is nothing else to do
   glutIdleFunc(idle);
   //  Tell GLUT to call "display" when the scene should be drawn
   glutDisplayFunc(display);
   //  Tell GLUT to call "reshape" when the window is resized
   glutReshapeFunc(reshape);
   //  Tell GLUT to call "special" when an arrow key is pressed
   glutSpecialFunc(special);
   glutMouseFunc(Mouse);
   glutMotionFunc(Motion);
   glutKeyboardFunc(key);
   //  Tell GLUT to call "key" when a key is pressed
   //  Pass control to GLUT so it can interact with the user
   //glutTimerFunc(5, animate, 1);
   texture[0] = LoadTexBMP("Textures/grass.bmp");
   testSky = LoadTexBMP("Textures/skybox_texture.bmp");
   obj        = LoadOBJ("wing.obj");
   glutMainLoop();
   return 0;
}
