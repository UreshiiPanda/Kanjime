#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define _USE_MATH_DEFINES
#include <math.h>

#ifndef F_PI
#define F_PI		((float)(M_PI))
#define F_2_PI		((float)(2.f*F_PI))
#define F_PI_2		((float)(F_PI/2.f))
#endif


#define XSIDE	10			// length of the x side of the grid
#define X0      (-XSIDE/2.)		// where one side starts
#define NX	    500			// how many points in x
#define DX	    ( XSIDE/(float)NX )	// change in x between the points

#define YGRID	0.f

#define ZSIDE	10		// length of the z side of the grid
#define Z0      (-ZSIDE/2.)		// where one side starts
#define NZ	    500			// how many points in z
#define DZ	    ( ZSIDE/(float)NZ )	// change in z between the points



#include "glew.h"
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include "glut.h"





// millisecs
const int MSEC = 25000;

// title of these windows:

const char *WINDOWTITLE = "OpenGL / GLUT Sample -- Joseph Houghton";
const char *GLUITITLE   = "User Interface Window";

// what the glui package defines as true and false:

const int GLUITRUE  = true;
const int GLUIFALSE = false;

// the escape key:

const int ESCAPE = 0x1b;

// initial window size:

const int INIT_WINDOW_SIZE = 600;

// size of the 3d box to be drawn:

const float BOXSIZE = 2.f;

// multiplication factors for input interaction:
//  (these are known from previous experience)

const float ANGFACT = 1.f;
const float SCLFACT = 0.005f;

// minimum allowable scale factor:

const float MINSCALE = 0.05f;

// scroll wheel button values:

const int SCROLL_WHEEL_UP   = 3;
const int SCROLL_WHEEL_DOWN = 4;

// equivalent mouse movement when we click the scroll wheel:

const float SCROLL_WHEEL_CLICK_FACTOR = 5.f;

// active mouse buttons (or them together):

const int LEFT   = 4;
const int MIDDLE = 2;
const int RIGHT  = 1;

// which projection:

enum Projections
{
	ORTHO,
	PERSP
};

// which button:

enum ButtonVals
{
	RESET,
	QUIT
};

// window background color (rgba):

const GLfloat BACKCOLOR[ ] = { .8, .8, 1., 1. };

// line width for the axes:

const GLfloat AXES_WIDTH   = 3.;

// the color numbers:
// this order must match the radio button order, which must match the order of the color names,
// 	which must match the order of the color RGB values

enum Colors
{
	RED,
	YELLOW,
	GREEN,
	CYAN,
	BLUE,
	MAGENTA,
    WH
};

char * ColorNames[ ] =
{
	(char *)"Red",
	(char*)"Yellow",
	(char*)"Green",
	(char*)"Cyan",
	(char*)"Blue",
	(char*)"Magenta",
    (char*)"White"
};


enum Lights
{
    POINT,
    SPOT
};

// the color definitions:
// this order must match the menu order

const GLfloat Colors[ ][3] = 
{
	{ 1., 0., 0. },		// red
	{ 1., 1., 0. },		// yellow
	{ 0., 1., 0. },		// green
	{ 0., 1., 1. },		// cyan
	{ 0., 0., 1. },		// blue
	{ 1., 0., 1. },		// magenta
    { 1., 1., 1. },     // white
};

// fog parameters:

const GLfloat FOGCOLOR[4] = { .0f, .0f, .0f, 1.f };
const GLenum  FOGMODE     = GL_LINEAR;
const GLfloat FOGDENSITY  = 0.30f;
const GLfloat FOGSTART    = 1.5f;
const GLfloat FOGEND      = 4.f;

// for lighting:

const float	WHITE[ ] = { 1.,1.,1.,1. };

// for animation:

const int MS_PER_CYCLE = 25000;		// 10000 milliseconds = 10 seconds



// non-constant global variables:

int		ActiveButton;			// current button that is down
GLuint	AxesList;				// list to hold the axes
GLuint  Light1List;
GLuint  Light2List;
GLuint  Light3List;
GLuint  Light4List;
int		AxesOn;					// != 0 means to draw the axes
GLuint	BoxList;				// object display list
GLuint  PikaList;               // Pikachu obj
GLuint  FujiList;               // fuji obj
GLuint  PineList;               // pine obj
GLuint  CubeList;               // cube obj
GLuint  GridDL;                 // grid / ground
GLuint  PenList;                // Penguin obj
int		DebugOn;				// != 0 means to print debugging info
int		DepthCueOn;				// != 0 means to use intensity depth cueing
int		DepthBufferOn;			// != 0 means to use the z-buffer
int		DepthFightingOn;		// != 0 means to force the creation of z-fighting
int		MainWindow;				// window id for main graphics window
int		NowColor;				// index into Colors[ ]
int		NowProjection;		    // ORTHO or PERSP
float	Scale;					// scaling factor
int		ShadowsOn;				// != 0 means to turn shadows on
float	Time;					// used for animation, this has a value between 0. and 1.
int		Xmouse, Ymouse;			// mouse values
float	Xrot, Yrot;				// rotation angles in degrees
bool    Frozen;                 // freeze animation
int LightType;
int LightColor;


unsigned int tex;




// function prototypes:

void	Animate( );
void	Display( );
void	DoAxesMenu( int );
void	DoColorMenu( int );
void	DoDepthBufferMenu( int );
void	DoDepthFightingMenu( int );
void	DoDepthMenu( int );
void	DoDebugMenu( int );
void	DoMainMenu( int );
void	DoProjectMenu( int );
void	DoRasterString( float, float, float, char * );
void	DoStrokeString( float, float, float, float, char * );
float	ElapsedSeconds( );
void	InitGraphics( );
void	InitLists( );
void	InitMenus( );
void	Keyboard( unsigned char, int, int );
void	MouseButton( int, int, int, int );
void	MouseMotion( int, int );
void	Reset( );
void	Resize( int, int );
void	Visibility( int );

void			Axes( float );
void			HsvRgb( float[3], float [3] );
void			Cross(float[3], float[3], float[3]);
float			Dot(float [3], float [3]);
float			Unit(float [3], float [3]);
float			Unit(float [3]);

void    AddTimeValue( float time, float value );
float   GetFirstTime( );
float   GetLastTime( );
int     GetNumKeytimes( );
float   GetValue( float time );
void	Init( );
void    PrintTimeValues( );


// utility to create an array from 3 separate values:

float *
Array3( float a, float b, float c, float d )
{
	static float array[4];

	array[0] = a;
	array[1] = b;
	array[2] = c;
	array[3] = d;
	return array;
}

// utility to create an array from a multiplier and an array:

float *
MulArray3( float factor, float array0[ ] )
{
	static float array[4];

	array[0] = factor * array0[0];
	array[1] = factor * array0[1];
	array[2] = factor * array0[2];
	array[3] = factor * array0[3];
	return array;
}


float *
MulArray3(float factor, float a, float b, float c, float d )
{
	static float array[4];

	float* abc = Array3(a, b, c, d);
	array[0] = factor * abc[0];
	array[1] = factor * abc[1];
	array[2] = factor * abc[2];
	array[3] = factor * abc[3];
	return array;
}


#include "setmaterial.cpp"
#include "setlight.cpp"
#include "bmptotexture.cpp"
#include "loadobjfile.cpp"
#include "keytime.cpp"
//#include "glslprogram.cpp"




// keytimes
Keytimes Xpika;
Keytimes Ypika;
Keytimes Zpika;

Keytimes Xpen;
Keytimes Ypen;
Keytimes Zpen;

Keytimes XposT;
Keytimes sizeT;
Keytimes colorL;

Keytimes eyeX;
Keytimes eyeZ;
Keytimes eyeY;
Keytimes lookX;
Keytimes lookY;
Keytimes lookZ;

Keytimes fadeG;
Keytimes fadeK;

// main program:

int
main( int argc, char *argv[ ] )
{
	// turn on the glut package:
	// (do this before checking argc and argv since glutInit might
	// pull some command line arguments out)

	glutInit( &argc, argv );

	// setup all the graphics stuff:

	InitGraphics( );

	// create the display lists that **will not change**:

	InitLists( );

	// init all the global variables used by Display( ):
	// this will also post a redisplay

	Reset( );

	// setup all the user interface stuff:

	InitMenus( );

	// draw the scene once and wait for some interaction:
	// (this will never return)

	glutSetWindow( MainWindow );
	glutMainLoop( );

	// glutMainLoop( ) never actually returns
	// the following line is here to make the compiler happy:

	return 0;
}

void
Animate( )
{
	// put animation stuff in here -- change some global variables for Display( ) to find:

	int ms = glutGet(GLUT_ELAPSED_TIME);
	ms %= MS_PER_CYCLE;							// makes the value of ms between 0 and MS_PER_CYCLE-1
	Time = (float)ms / (float)MS_PER_CYCLE;		// makes the value of Time between 0. and slightly less than 1.

	// for example, if you wanted to spin an object in Display( ), you might call: glRotatef( 360.f*Time,   0., 1., 0. );

	// force a call to Display( ) next time it is convenient:

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// draw the complete scene:

void
Display( )
{
	if (DebugOn != 0)
		fprintf(stderr, "Starting Display.\n");

	// set which window we want to do the graphics into:
	glutSetWindow( MainWindow );

	// erase the background:
	glDrawBuffer( GL_BACK );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glEnable( GL_DEPTH_TEST );





    // turn # msec into the cycle ( 0 - MSEC-1 ):
	int msec = glutGet( GLUT_ELAPSED_TIME )  %  MSEC;

	// turn that into a time in seconds:
	float nowTime = (float)msec  / 1000.;



	// specify shading to be flat:

	glShadeModel( GL_SMOOTH );

	// set the viewport to be a square centered in the window:

	GLsizei vx = glutGet( GLUT_WINDOW_WIDTH );
	GLsizei vy = glutGet( GLUT_WINDOW_HEIGHT );
	GLsizei v = vx < vy ? vx : vy;			// minimum dimension
	GLint xl = ( vx - v ) / 2;
	GLint yb = ( vy - v ) / 2;
	glViewport( xl, yb,  v, v );


	// set the viewing volume:
	// remember that the Z clipping  values are given as DISTANCES IN FRONT OF THE EYE
	// USE gluOrtho2D( ) IF YOU ARE DOING 2D !

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	if( NowProjection == ORTHO )
		glOrtho( -2.f, 2.f,     -2.f, 2.f,     0.1f, 1000.f );
	else
		gluPerspective( 70.f, 1.f,	0.1f, 1000.f );

	// place the objects into the scene:

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );

	// set the eye position, look-at position, and up-vector:
    //float ang = 2*F_PI*Time;

    gluLookAt( eyeX.GetValue( nowTime ), 90.f, eyeZ.GetValue( nowTime ),   lookX.GetValue( nowTime ), -10., lookZ.GetValue( nowTime ),    0.f, 1.f, 0.f );



	//gluLookAt( -220.f, 50.f, 0.f,     0.f, 0.f, 0.f,     0.f, 1.f, 0.f );


	// rotate the scene:

	glRotatef( (GLfloat)Yrot, 0.f, 1.f, 0.f );
	glRotatef( (GLfloat)Xrot, 1.f, 0.f, 0.f );

	// uniformly scale the scene:

	if( Scale < MINSCALE )
		Scale = MINSCALE;
	glScalef( (GLfloat)Scale, (GLfloat)Scale, (GLfloat)Scale );

	// set the fog parameters:

	if( DepthCueOn != 0 )
	{
		glFogi( GL_FOG_MODE, FOGMODE );
		glFogfv( GL_FOG_COLOR, FOGCOLOR );
		glFogf( GL_FOG_DENSITY, FOGDENSITY );
		glFogf( GL_FOG_START, FOGSTART );
		glFogf( GL_FOG_END, FOGEND );
		glEnable( GL_FOG );
	}
	else
	{
		glDisable( GL_FOG );
	}

	// possibly draw the axes:

	if( AxesOn != 0 )
	{
        glColor3fv( &Colors[NowColor][0] );
        glCallList( AxesList );
	}

	// since we are using glScalef( ), be sure the normals get unitized:

	glEnable( GL_NORMALIZE );


    glEnable( GL_LIGHTING );



    glEnable( GL_BLEND );
    if (nowTime < 14) {
        glDepthMask( GL_TRUE );
    }
    else {
        glDepthMask( GL_FALSE );
    }
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



    glScalef(20., 20., 20.);

	glEnable( GL_TEXTURE_2D );
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);



    glPushMatrix();
        //SetMaterial(.8, .8, .8, 5., 15.);
        SetMaterial(.8, .8, .8, fadeG.GetValue(nowTime), 15.);
		glBindTexture( GL_TEXTURE_2D, tex );

        glCallList(FujiList);
    glPopMatrix();


	glDisable( GL_TEXTURE_2D );



    glPushMatrix();
        //SetMaterial(0., 1., 0., 1., 15.);
        SetMaterial(0., 1., 0., fadeG.GetValue(nowTime), 15.);
        glCallList(PineList);
    glPopMatrix();









    // kanji cubes


    // ground
	glPushMatrix( );
        SetMaterial(0., 0., 0., fadeK.GetValue(nowTime), 15.);
        //SetMaterial(0., 0., 0., 1., 5.);
        glScalef(0.1, 0.1, 4.);
        glTranslatef( 0., 1., 0. - .15 );
		glRotatef( 0.,  1., 0., 0. );
		glRotatef( 0.,  0., 1., 0. );
		glRotatef( 0.,  0., 0., 1. );

	    glCallList( CubeList );

	glPopMatrix( );



    // ten ten
	glPushMatrix( );
        SetMaterial(0., 0., 0., fadeK.GetValue(nowTime), 15.);

        //SetMaterial(0., 0., 0., 1., 5.);
        glRotatef(45., 1., 0., 0.);
        glTranslatef( 0., 0.5, .25  - .3 );
        glScalef(0.1, .7, 0.1);
		glRotatef( 0.,  1., 0., 0. );
		glRotatef( 0.,  0., 1., 0. );
		glRotatef( 0.,  0., 0., 1. );

	    glCallList( CubeList );

	glPopMatrix( );


	glPushMatrix( );
        SetMaterial(0., 0., 0., fadeK.GetValue(nowTime), 15.);

        //SetMaterial(0., 0., 0., 1., 5.);
        glRotatef(-45., 1., 0., 0.);
        glTranslatef( 0., 1.4, -.25  - .6  );
        glScalef(0.1, .7, 0.1);
		glRotatef( 0.,  1., 0., 0. );
		glRotatef( 0.,  0., 1., 0. );
		glRotatef( 0.,  0., 0., 1. );

	    glCallList( CubeList );

	glPopMatrix( );




    // mouth horiz
	glPushMatrix( );
        SetMaterial(0., 0., 0., fadeK.GetValue(nowTime), 15.);

        // SetMaterial(0., 0., 0., 1., 15.);
        glTranslatef( 0., .8, 0.  - .6  );
        glScalef(0.1, 0.1, 1.2);
		glRotatef( 0.,  1., 0., 0. );
		glRotatef( 0.,  0., 1., 0. );
		glRotatef( 0.,  0., 0., 1. );

	    glCallList( CubeList );

	glPopMatrix( );


	glPushMatrix( );
        SetMaterial(0., 0., 0., fadeK.GetValue(nowTime), 15.);

        // SetMaterial(0., 0., 0., 1., 5.);
        glTranslatef( 0., 1.3, 0.   - .6 );
        glScalef(0.1, 0.1, 1.2);
		glRotatef( 0.,  1., 0., 0. );
		glRotatef( 0.,  0., 1., 0. );
		glRotatef( 0.,  0., 0., 1. );

	    glCallList( CubeList );

	glPopMatrix( );



    // mouth vert
    glPushMatrix( );
        SetMaterial(0., 0., 0., fadeK.GetValue(nowTime), 15.);

        // SetMaterial(0., 0., 0., 1., 5.);
        glTranslatef( 0., 1.1, .5  - .6  );
        glScalef(0.1, .5, 0.1);
        glRotatef( 0.,  1., 0., 0. );
        glRotatef( 0.,  0., 1., 0. );
        glRotatef( 0.,  0., 0., 1. );
    
        glCallList( CubeList );
    
    glPopMatrix( );
    
    
    glPushMatrix( );
        SetMaterial(0., 0., 0., fadeK.GetValue(nowTime), 15.);

       //  SetMaterial(0., 0., 0., 1., 5.);
        glTranslatef( 0., 1.1, -.5  - .6  );
        glScalef(0.1, .5, 0.1);
        glRotatef( 0.,  1., 0., 0. );
        glRotatef( 0.,  0., 1., 0. );
        glRotatef( 0.,  0., 0., 1. );
    
        glCallList( CubeList );
    
    glPopMatrix( );



    // top of mouth
	glPushMatrix( );
        SetMaterial(0., 0., 0., fadeK.GetValue(nowTime), 15.);

         //SetMaterial(0., 0., 0., 1., 5.);
        glTranslatef( 0., 1.5, 0.  - .6  );
        glScalef(0.1, 0.1, 1.0);
		glRotatef( 0.,  1., 0., 0. );
		glRotatef( 0.,  0., 1., 0. );
		glRotatef( 0.,  0., 0., 1. );

	    glCallList( CubeList );

	glPopMatrix( );




    // roof
	glPushMatrix( );
        SetMaterial(0., 0., 0., fadeK.GetValue(nowTime), 15.);

        // SetMaterial(0., 0., 0., 1., 5.);
        glRotatef(-30., 1., 0., 0.);
        glTranslatef( 0., 1.95, .5  - .8  );
        glScalef(0.1, .1, 1.8);
		glRotatef( 0.,  1., 0., 0. );
		glRotatef( 0.,  0., 1., 0. );
		glRotatef( 0.,  0., 0., 1. );

	    glCallList( CubeList );

	glPopMatrix( );


	glPushMatrix( );
        SetMaterial(0., 0., 0., fadeK.GetValue(nowTime), 15.);

         //SetMaterial(0., 0., 0., 1., 5.);
        glRotatef(30., 1., 0., 0.);
        glTranslatef( 0., 1.4, -.5  );
        glScalef(0.1, .1, 1.8);
		glRotatef( 0.,  1., 0., 0. );
		glRotatef( 0.,  0., 1., 0. );
		glRotatef( 0.,  0., 0., 1. );

	    glCallList( CubeList );

	glPopMatrix( );



    // tree horiz
	glPushMatrix( );
        SetMaterial(0., 0., 0., fadeK.GetValue(nowTime), 15.);

       //  SetMaterial(0., 0., 0., 1., 5.);
        glTranslatef( 0., 1.9, -.6  - .3  );
        glScalef(0.1, 0.1, .5);
		glRotatef( 0.,  1., 0., 0. );
		glRotatef( 0.,  0., 1., 0. );
		glRotatef( 0.,  0., 0., 1. );

	    glCallList( CubeList );

	glPopMatrix( );


    // tree 45
	glPushMatrix( );
        SetMaterial(0., 0., 0., fadeK.GetValue(nowTime), 15.);

       //  SetMaterial(0., 0., 0., 1., 5.);
        glRotatef(-45., 1., 0., 0.);
        glTranslatef( 0., 2.4, .16  - .1  );
        glScalef(0.1, .7, 0.1);
		glRotatef( 0.,  1., 0., 0. );
		glRotatef( 0.,  0., 1., 0. );
		glRotatef( 0.,  0., 0., 1. );

	    glCallList( CubeList );

	glPopMatrix( );


    // hikers
	glPushMatrix( );
        SetMaterial(0., 0., 0., fadeK.GetValue(nowTime), 15.);

       //  SetMaterial(0., 0., 0., 1., 5.);
        glRotatef(45., 1., 0., 0.);
        glTranslatef( 0., 1.5, -.8  - .4  );
        glScalef(0.1, .7, 0.1);
		glRotatef( 0.,  1., 0., 0. );
		glRotatef( 0.,  0., 1., 0. );
		glRotatef( 0.,  0., 0., 1. );

	    glCallList( CubeList );

	glPopMatrix( );


	glPushMatrix( );
        SetMaterial(0., 0., 0., fadeK.GetValue(nowTime), 15.);

       //  SetMaterial(0., 0., 0., 1., 5.);
        glRotatef(45., 1., 0., 0.);
        glTranslatef( 0., 1.6, -.9  );
        glScalef(0.1, .7, 0.1);
		glRotatef( 0.,  1., 0., 0. );
		glRotatef( 0.,  0., 1., 0. );
		glRotatef( 0.,  0., 0., 1. );

	    glCallList( CubeList );

	glPopMatrix( );







    // pikachu
	glPushMatrix( );
        //SetMaterial(1., 1., 0., 1., 15.);
        SetMaterial(1., 1., 0., fadeG.GetValue(nowTime), 15.);

        glTranslatef( Xpika.GetValue( nowTime ), 0., 0. );
        glTranslatef( 0., Ypika.GetValue( nowTime ), 0. );
		glTranslatef( 0., 0., Zpika.GetValue( nowTime ) );
		glRotatef( 0.,  1., 0., 0. );
		glRotatef( 0.,  0., 1., 0. );
		glRotatef( 0.,  0., 0., 1. );

	    glCallList( PikaList );

	glPopMatrix( );


    // penguin
	glPushMatrix( );
        //SetMaterial(1., 0., 0., 1., 15.);
        SetMaterial(1., 0., 0., fadeG.GetValue(nowTime), 15.);

        glTranslatef( Xpen.GetValue( nowTime ), 0., 0. );
        glTranslatef( 0., Ypen.GetValue( nowTime ), 0. );
		glTranslatef( 0., 0., Zpen.GetValue( nowTime ) );
		glRotatef( 0.,  1., 0., 0. );
		glRotatef( 0.,  0., 0., 1. );
		glRotatef( 0.,  0., 1., 0. );

	    glCallList( PenList );

	glPopMatrix( );



    glDepthMask( GL_TRUE );
    glDisable( GL_BLEND );


    glDisable( GL_LIGHTING );




    // display lights

    if ( LightType == POINT ) {
        SetPointLight(GL_LIGHT0, 0., 5., 0., 0., 0., colorL.GetValue( nowTime ), 1.);
  
        glDisable(GL_LIGHTING);
    }

    else {


    }




	glDisable( GL_DEPTH_TEST );
	glColor3f( 0.f, 1.f, 1.f );
	//DoRasterString( 0.f, 1.f, 0.f, (char *)"Text That Moves" );

	glDisable( GL_DEPTH_TEST );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	gluOrtho2D( 0.f, 100.f,     0.f, 100.f );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );
	glColor3f( 1.f, 1.f, 1.f );
	//DoRasterString( 5.f, 5.f, 0.f, (char *)"Text That Doesn't" );

	// swap the double-buffered framebuffers:

	glutSwapBuffers( );

	// be sure the graphics buffer has been sent:
	// note: be sure to use glFlush( ) here, not glFinish( ) !

	glFlush( );
}


void
DoAxesMenu( int id )
{
	AxesOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoColorMenu( int id )
{
	NowColor = id - RED;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoDebugMenu( int id )
{
	DebugOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoDepthBufferMenu( int id )
{
	DepthBufferOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoDepthFightingMenu( int id )
{
	DepthFightingOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoDepthMenu( int id )
{
	DepthCueOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// main menu callback:

void
DoMainMenu( int id )
{
	switch( id )
	{
		case RESET:
			Reset( );
			break;

		case QUIT:
			// gracefully close out the graphics:
			// gracefully close the graphics window:
			// gracefully exit the program:
			glutSetWindow( MainWindow );
			glFinish( );
			glutDestroyWindow( MainWindow );
			exit( 0 );
			break;

		default:
			fprintf( stderr, "Don't know what to do with Main Menu ID %d\n", id );
	}

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoProjectMenu( int id )
{
	NowProjection = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// use glut to display a string of characters using a raster font:

void
DoRasterString( float x, float y, float z, char *s )
{
	glRasterPos3f( (GLfloat)x, (GLfloat)y, (GLfloat)z );

	char c;			// one character to print
	for( ; ( c = *s ) != '\0'; s++ )
	{
		glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, c );
	}
}


// use glut to display a string of characters using a stroke font:

void
DoStrokeString( float x, float y, float z, float ht, char *s )
{
	glPushMatrix( );
		glTranslatef( (GLfloat)x, (GLfloat)y, (GLfloat)z );
		float sf = ht / ( 119.05f + 33.33f );
		glScalef( (GLfloat)sf, (GLfloat)sf, (GLfloat)sf );
		char c;			// one character to print
		for( ; ( c = *s ) != '\0'; s++ )
		{
			glutStrokeCharacter( GLUT_STROKE_ROMAN, c );
		}
	glPopMatrix( );
}


// return the number of seconds since the start of the program:

float
ElapsedSeconds( )
{
	// get # of milliseconds since the start of the program:

	int ms = glutGet( GLUT_ELAPSED_TIME );

	// convert it to seconds:

	return (float)ms / 1000.f;
}


// initialize the glui window:

void
InitMenus( )
{
	if (DebugOn != 0)
		fprintf(stderr, "Starting InitMenus.\n");

	glutSetWindow( MainWindow );

	int numColors = sizeof( Colors ) / ( 3*sizeof(float) );
	int colormenu = glutCreateMenu( DoColorMenu );
	for( int i = 0; i < numColors; i++ )
	{
		glutAddMenuEntry( ColorNames[i], i );
	}

	int axesmenu = glutCreateMenu( DoAxesMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int depthcuemenu = glutCreateMenu( DoDepthMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int depthbuffermenu = glutCreateMenu( DoDepthBufferMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int depthfightingmenu = glutCreateMenu( DoDepthFightingMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int debugmenu = glutCreateMenu( DoDebugMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int projmenu = glutCreateMenu( DoProjectMenu );
	glutAddMenuEntry( "Orthographic",  ORTHO );
	glutAddMenuEntry( "Perspective",   PERSP );

	int mainmenu = glutCreateMenu( DoMainMenu );
	glutAddSubMenu(   "Axes",          axesmenu);
	glutAddSubMenu(   "Axis Colors",   colormenu);

	glutAddSubMenu(   "Depth Cue",     depthcuemenu);
	glutAddSubMenu(   "Projection",    projmenu );
	glutAddMenuEntry( "Reset",         RESET );
	glutAddSubMenu(   "Debug",         debugmenu);
	glutAddMenuEntry( "Quit",          QUIT );

// attach the pop-up menu to the right mouse button:

	glutAttachMenu( GLUT_RIGHT_BUTTON );
}



// initialize the glut and OpenGL libraries:
//	also setup callback functions

void
InitGraphics( )
{
	if (DebugOn != 0)
		fprintf(stderr, "Starting InitGraphics.\n");



    // Keytimes Pikachu
    // keytime Xpika 
    Xpika.Init( );
	Xpika.AddTimeValue(  0.0,  0. );
	Xpika.AddTimeValue(  2.0,  0. );
	Xpika.AddTimeValue(  4.0,  0. );
	Xpika.AddTimeValue(  6.0,  0. );
	Xpika.AddTimeValue(  8.0,  0. );
	Xpika.AddTimeValue(  10.0,  0. );
	Xpika.AddTimeValue(  12.0,  0. );
	Xpika.AddTimeValue(  14.0,  0. );
	Xpika.AddTimeValue(  16.0,  0. );
	Xpika.AddTimeValue(  18.0,  0. );
	Xpika.AddTimeValue(  20.0,  0. );
	Xpika.AddTimeValue(  22.0,  0. );
	Xpika.AddTimeValue(  24.0,  0. );
	Xpika.AddTimeValue(  25.0,  0. );


    // keytime Ypika
    Ypika.Init( );
	Ypika.AddTimeValue(  0.0,  -.4 );
	Ypika.AddTimeValue(  2.0,  -.4 );
	Ypika.AddTimeValue(  4.0,  -.4 );
	Ypika.AddTimeValue(  6.0,  -.2 );
	Ypika.AddTimeValue(  8.0,  -.2 );
    Ypika.AddTimeValue(  10.0, 0.0 );
	Ypika.AddTimeValue(  12.0, 0.0 );
	Ypika.AddTimeValue(  14.0, 0.2 );
	Ypika.AddTimeValue(  16.0, 0.5 );
	Ypika.AddTimeValue(  18.0, 0.8 );
	Ypika.AddTimeValue(  20.0, 1.0 );
	Ypika.AddTimeValue(  22.0, 1.2 );
	Ypika.AddTimeValue(  24.0, 1.2 );
	Ypika.AddTimeValue(  25.0, 1.2 );


    // keytime Zpika
    Zpika.Init( );
	Zpika.AddTimeValue(  0.0,  5.5 );
	Zpika.AddTimeValue(  2.0,  5.0 );
	Zpika.AddTimeValue(  4.0,  4.5 );
	Zpika.AddTimeValue(  6.0,  4.0 );
	Zpika.AddTimeValue(  8.0,  3.5 );
	Zpika.AddTimeValue( 10.0,  3.0 );
	Zpika.AddTimeValue(  12.0,  2.5 );
	Zpika.AddTimeValue(  14.0,  2.2 );
	Zpika.AddTimeValue(  16.0,  1.7 );
	Zpika.AddTimeValue(  18.0,  1.3 );
	Zpika.AddTimeValue(  20.0,  1. );
	Zpika.AddTimeValue(  22.0,  0.8 );
	Zpika.AddTimeValue(  24.0,  0.5 );
	Zpika.AddTimeValue(  25.0,  0.5 );



    // Keytimes Penguin
    // keytime Xpen 
    Xpen.Init( );
	Xpen.AddTimeValue(  0.0,  0. );
	Xpen.AddTimeValue(  2.0,  0. );
	Xpen.AddTimeValue(  4.0,  0. );
	Xpen.AddTimeValue(  6.0,  0. );
	Xpen.AddTimeValue(  8.0,  0. );
	Xpen.AddTimeValue( 10.0,  0. );
	Xpen.AddTimeValue(  12.0,  0. );
	Xpen.AddTimeValue(  14.0,  0. );
	Xpen.AddTimeValue(  16.0,  0. );
	Xpen.AddTimeValue(  18.0,  0. );
	Xpen.AddTimeValue(  20.0,  0. );
	Xpen.AddTimeValue(  22.0,  0. );
	Xpen.AddTimeValue(  24.0,  0. );
	Xpen.AddTimeValue(  25.0,  0. );


    // keytime Ypen
    Ypen.Init( );
	Ypen.AddTimeValue(  0.0,  0.0 );
	Ypen.AddTimeValue(  2.0,  0.1 );
	Ypen.AddTimeValue(  4.0,  0.2 );
	Ypen.AddTimeValue(  6.0,  0.3 );
	Ypen.AddTimeValue(  8.0,  0.4 );
	Ypen.AddTimeValue( 10.0,  0.5 );
	Ypen.AddTimeValue(  12.0,  0.7 );
	Ypen.AddTimeValue(  14.0,  .9 );
	Ypen.AddTimeValue(  16.0,  1.1 );
	Ypen.AddTimeValue(  18.0,  1.3 );
	Ypen.AddTimeValue(  20.0,  1.5 );
	Ypen.AddTimeValue(  22.0,  1.7 );
	Ypen.AddTimeValue(  24.0,  1.8 );
	Ypen.AddTimeValue(  25.0,  1.9 );


    // keytime Zpen
    Zpen.Init( );
	Zpen.AddTimeValue(  0.0,  4.5 );
	Zpen.AddTimeValue(  2.0,  4.0 );
	Zpen.AddTimeValue(  4.0,  3.5 );
	Zpen.AddTimeValue(  6.0,  3.0 );
	Zpen.AddTimeValue(  8.0,  2.5 );
	Zpen.AddTimeValue(  10.0, 2.2 );
	Zpen.AddTimeValue(  12.0, 1.9 );
	Zpen.AddTimeValue(  14.0, 1.5 );
	Zpen.AddTimeValue(  16.0,  1.0 );
	Zpen.AddTimeValue(  18.0,  0.5 );
	Zpen.AddTimeValue(  20.0,  0.3 );
	Zpen.AddTimeValue(  22.0,  0.1 );
	Zpen.AddTimeValue(  24.0,  0.1 );
	Zpen.AddTimeValue(  25.0,  0.1 );



    // Keytimes Eye/look
    // eyeX
    eyeX.Init( );
	eyeX.AddTimeValue(  0.0,  -200. );
	eyeX.AddTimeValue(  2.0,  -100. );
	eyeX.AddTimeValue(  4.0,  0. );
	eyeX.AddTimeValue(  6.0,  100. );
	eyeX.AddTimeValue(  8.0,  200. );
	eyeX.AddTimeValue( 10.0,  100. );
	eyeX.AddTimeValue(  12.0, 0. );
	eyeX.AddTimeValue(  14.0, -100. );
	eyeX.AddTimeValue(  16.0,  -200. );
	eyeX.AddTimeValue(  18.0,  -200. );
	eyeX.AddTimeValue(  20.0,  -200. );
	eyeX.AddTimeValue(  22.0,  -200. );
	eyeX.AddTimeValue(  24.0,  -200. );
	eyeX.AddTimeValue(  25.0,  -200. );


    // eyeZ 
     eyeZ.Init( );
	 eyeZ.AddTimeValue(  0.0,  0. );
	 eyeZ.AddTimeValue(  2.0,  100. );
	 eyeZ.AddTimeValue(  4.0,  200. );
	 eyeZ.AddTimeValue(  6.0,  100. );
	 eyeZ.AddTimeValue(  8.0,  0. );
	 eyeZ.AddTimeValue( 10.0,  -100. );
	eyeZ.AddTimeValue(  12.0,  -200. );
	eyeZ.AddTimeValue(  14.0,  -100. );
	eyeZ.AddTimeValue(  16.0,  0. );
	eyeZ.AddTimeValue(  18.0,  0. );
	eyeZ.AddTimeValue(  20.0,  0. );
	eyeZ.AddTimeValue(  22.0,  0. );
	eyeZ.AddTimeValue(  24.0,  0. );
	eyeZ.AddTimeValue(  25.0,  0. );


    // eyeY 
     eyeY.Init( );
	 eyeY.AddTimeValue(  0.0,  20. );
	 eyeY.AddTimeValue(  2.0,  20. );
	 eyeY.AddTimeValue(  4.0,  20. );
	 eyeY.AddTimeValue(  6.0,  20. );
	 eyeY.AddTimeValue(  8.0,  20. );
	 eyeY.AddTimeValue( 10.0,  20. );

    
    // lookX
    lookX.Init( );
	lookX.AddTimeValue(  0.0,  0.0 );
	lookX.AddTimeValue(  2.0,  0.0 );
	lookX.AddTimeValue(  4.0,  0.0 );
	lookX.AddTimeValue(  6.0,  0.0 );
	lookX.AddTimeValue(  8.0,  0.0 );
	lookX.AddTimeValue(  10.0,  0.0 );
    

    // lookY
    lookY.Init( );
	lookY.AddTimeValue(  0.0,  0.0 );
	lookY.AddTimeValue(  2.0,  0.0 );
	lookY.AddTimeValue(  4.0,  0.0 );
	lookY.AddTimeValue(  6.0,  0.0 );
	lookY.AddTimeValue(  8.0,  0.0 );
	lookY.AddTimeValue(  10.0,  0.0 );

    
    // lookZ
    lookZ.Init( );
	lookZ.AddTimeValue(  0.0,  0.0 );
	lookZ.AddTimeValue(  2.0,  0.0 );
	lookZ.AddTimeValue(  4.0,  0.0 );
	lookZ.AddTimeValue(  6.0,  0.0 );
	lookZ.AddTimeValue(  8.0,  0.0 );
	lookZ.AddTimeValue(  10.0,  0.0 );
  




    // Keytimes fades
    // fadeG
    fadeG.Init( );
	fadeG.AddTimeValue(  0.0,  1. );
	fadeG.AddTimeValue(  2.0,  1. );
	fadeG.AddTimeValue(  4.0,  1. );
	fadeG.AddTimeValue(  6.0,  1. );
	fadeG.AddTimeValue(  8.0,  1. );
	fadeG.AddTimeValue(  10.0,  1. );
	fadeG.AddTimeValue(  12.0, 1. );
	fadeG.AddTimeValue(  14.0, 1. );
	fadeG.AddTimeValue(  16.0, .8 );
	fadeG.AddTimeValue(  18.0, .6 );
	fadeG.AddTimeValue(  20.0, .4 );
	fadeG.AddTimeValue(  22.0, .2 );
	fadeG.AddTimeValue(  24.0, 0. );
	fadeG.AddTimeValue(  25.0, 0. );


    // fadeK
    fadeK.Init( );
	fadeK.AddTimeValue(  0.0,  0. );
	fadeK.AddTimeValue(  2.0,  0. );
	fadeK.AddTimeValue(  4.0,  0. );
	fadeK.AddTimeValue(  6.0,  0. );
	fadeK.AddTimeValue(  8.0,  0. );
	fadeK.AddTimeValue( 10.0,  0. );
	fadeK.AddTimeValue(  12.0, 0. );
	fadeK.AddTimeValue(  14.0, 0. );
	fadeK.AddTimeValue(  16.0, .2 );
	fadeK.AddTimeValue(  18.0, .4 );
	fadeK.AddTimeValue(  20.0, .6 );
	fadeK.AddTimeValue(  22.0, .8 );
	fadeK.AddTimeValue(  24.0, 1. );
	fadeK.AddTimeValue(  25.0, 1. );





    // request the display modes:
	// ask for red-green-blue-alpha color, double-buffering, and z-buffering:

	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );

	// set the initial window configuration:

	glutInitWindowPosition( 0, 0 );
	glutInitWindowSize( INIT_WINDOW_SIZE, INIT_WINDOW_SIZE );

	// open the window and set its title:

	MainWindow = glutCreateWindow( WINDOWTITLE );
	glutSetWindowTitle( WINDOWTITLE );

	// set the framebuffer clear values:

	glClearColor( BACKCOLOR[0], BACKCOLOR[1], BACKCOLOR[2], BACKCOLOR[3] );

	// setup the callback functions:

	glutSetWindow( MainWindow );
	glutDisplayFunc( Display );
	glutReshapeFunc( Resize );
	glutKeyboardFunc( Keyboard );
	glutMouseFunc( MouseButton );
	glutMotionFunc( MouseMotion );
	glutPassiveMotionFunc(MouseMotion);
	//glutPassiveMotionFunc( NULL );
	glutVisibilityFunc( Visibility );
	glutEntryFunc( NULL );
	glutSpecialFunc( NULL );
	glutSpaceballMotionFunc( NULL );
	glutSpaceballRotateFunc( NULL );
	glutSpaceballButtonFunc( NULL );
	glutButtonBoxFunc( NULL );
	glutDialsFunc( NULL );
	glutTabletMotionFunc( NULL );
	glutTabletButtonFunc( NULL );
	glutMenuStateFunc( NULL );
	glutTimerFunc( -1, NULL, 0 );

	// setup glut to call Animate( ) every time it has
	// 	nothing it needs to respond to (which is most of the time)
	// we don't need to do this for this program, and really should set the argument to NULL
	// but, this sets us up nicely for doing animation

	glutIdleFunc( Animate );

	// init the glew package (a window must be open to do this):


	int width, height;
	char *file = (char *)"snows.bmp";
	unsigned char *texture = BmpToTexture( file, &width, &height );
	if( texture == NULL )
        	fprintf( stderr, "Cannot open texture '%s'\n", file );
	else
        	fprintf( stderr, "Opened '%s': width = %d ; height = %d\n", file, width, height );

	glGenTextures( 1, &tex );
	glBindTexture( GL_TEXTURE_2D, tex);
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D( GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture );





}


// initialize the display lists that will not change:
// (a display list is a way to store opengl commands in
//  memory so that they can be played back efficiently at a later time
//  with a call to glCallList( )

void
InitLists( )
{
	if (DebugOn != 0)
		fprintf(stderr, "Starting InitLists.\n");

	float dx = BOXSIZE / 2.f;
	float dy = BOXSIZE / 2.f;
	float dz = BOXSIZE / 2.f;
	glutSetWindow( MainWindow );

	// create the object:

	BoxList = glGenLists( 1 );
	glNewList( BoxList, GL_COMPILE );

		glBegin( GL_QUADS );

			glColor3f( 1., 0., 0. );

				glNormal3f( 1., 0., 0. );
					glVertex3f(  dx, -dy,  dz );
					glVertex3f(  dx, -dy, -dz );
					glVertex3f(  dx,  dy, -dz );
					glVertex3f(  dx,  dy,  dz );

				glNormal3f(-1., 0., 0.);
					glVertex3f( -dx, -dy,  dz);
					glVertex3f( -dx,  dy,  dz );
					glVertex3f( -dx,  dy, -dz );
					glVertex3f( -dx, -dy, -dz );

			glColor3f( 0., 1., 0. );

				glNormal3f(0., 1., 0.);
					glVertex3f( -dx,  dy,  dz );
					glVertex3f(  dx,  dy,  dz );
					glVertex3f(  dx,  dy, -dz );
					glVertex3f( -dx,  dy, -dz );

				glNormal3f(0., -1., 0.);
					glVertex3f( -dx, -dy,  dz);
					glVertex3f( -dx, -dy, -dz );
					glVertex3f(  dx, -dy, -dz );
					glVertex3f(  dx, -dy,  dz );

			glColor3f(0., 0., 1.);

				glNormal3f(0., 0., 1.);
					glVertex3f(-dx, -dy, dz);
					glVertex3f( dx, -dy, dz);
					glVertex3f( dx,  dy, dz);
					glVertex3f(-dx,  dy, dz);

				glNormal3f(0., 0., -1.);
					glVertex3f(-dx, -dy, -dz);
					glVertex3f(-dx,  dy, -dz);
					glVertex3f( dx,  dy, -dz);
					glVertex3f( dx, -dy, -dz);

		glEnd( );

	glEndList( );



    // pika
    PikaList = glGenLists(1);
    glNewList( PikaList, GL_COMPILE );

        glPushMatrix();
            //SetMaterial(1., 1., 0., 10.);
                glTranslatef( -.6, 0., 0. );
                glScalef(1.0, 1.0, 1.0);
                glRotatef( 180.f, 0.f, 1.f, 0.f );
                    LoadObjFile( (char*) "pika2.obj" );
        glPopMatrix();

    glEndList();


    // penguin
    PenList = glGenLists(1);
    glNewList( PenList, GL_COMPILE );

        glPushMatrix();
            //SetMaterial(1., 0., 0., 10.);
                glTranslatef( -.6, 0., 0. );
                glScalef(0.8, 0.8, 0.8);
                glRotatef( 180.f, 0.f, 1.f, 0.f );
                    LoadObjFile( (char*) "penguin.obj" );
        glPopMatrix();

    glEndList();


    // pine
    PineList = glGenLists(1);
    glNewList( PineList, GL_COMPILE );

        glPushMatrix();
            //SetMaterial(0., 1., 0., 10.);

            glColor4f(0.f, 1.f, 0.f, 1.f);   
                glTranslatef( -.7, 1.5, -1.3 );
                glScalef(.004, .004, .004);
                glRotatef( 270.f, 1.f, 0.f, 0.f );
                    LoadObjFile( (char*) "pine.obj" );
        glPopMatrix();

    glEndList();

    CubeList = glGenLists(1);
    glNewList( CubeList, GL_COMPILE );

        glPushMatrix();
            LoadObjFile( (char*) "cube.obj" );
        glPopMatrix();

    glEndList();


    // fuji
    FujiList = glGenLists( 1 );
    glNewList( FujiList, GL_COMPILE );
            LoadObjFile( (char*) "fuji.obj" );
    glEndList( );



    // create the axes:

	AxesList = glGenLists( 1 );
	glNewList( AxesList, GL_COMPILE );
		glLineWidth( AXES_WIDTH );
			Axes( 40.0 );
		glLineWidth( 1. );
	glEndList( );
}


// the keyboard callback:

void
Keyboard( unsigned char c, int x, int y )
{
	if( DebugOn != 0 )
		fprintf( stderr, "Keyboard: '%c' (0x%0x)\n", c, c );

	switch( c )
	{
		case 'o':
		case 'O':
			NowProjection = ORTHO;
			break;

		case 'p':
		case 'P':
			NowProjection = PERSP;
			break;

        case 'a':
        case 'A':
            LightType = POINT;
            break;

        case 's':
        case 'S':
            LightType = SPOT;
            break;

		case 'q':
		case 'Q':
		case ESCAPE:
			DoMainMenu( QUIT );	// will not return here
			break;				// happy compiler
                                
        case 'f':
	    case 'F':
		    Frozen = ! Frozen;
		    if( Frozen )
		    	glutIdleFunc( NULL );
		    else
		    	glutIdleFunc( Animate );
		    break;


		default:
			fprintf( stderr, "Don't know what to do with keyboard hit: '%c' (0x%0x)\n", c, c );
	}

	// force a call to Display( ):

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// called when the mouse button transitions down or up:

void
MouseButton( int button, int state, int x, int y )
{
	int b = 0;			// LEFT, MIDDLE, or RIGHT

	if( DebugOn != 0 )
		fprintf( stderr, "MouseButton: %d, %d, %d, %d\n", button, state, x, y );

	
	// get the proper button bit mask:

	switch( button )
	{
		case GLUT_LEFT_BUTTON:
			b = LEFT;		break;

		case GLUT_MIDDLE_BUTTON:
			b = MIDDLE;		break;

		case GLUT_RIGHT_BUTTON:
			b = RIGHT;		break;

		case SCROLL_WHEEL_UP:
			Scale += SCLFACT * SCROLL_WHEEL_CLICK_FACTOR;
			// keep object from turning inside-out or disappearing:
			if (Scale < MINSCALE)
				Scale = MINSCALE;
			break;

		case SCROLL_WHEEL_DOWN:
			Scale -= SCLFACT * SCROLL_WHEEL_CLICK_FACTOR;
			// keep object from turning inside-out or disappearing:
			if (Scale < MINSCALE)
				Scale = MINSCALE;
			break;

		default:
			b = 0;
			fprintf( stderr, "Unknown mouse button: %d\n", button );
	}

	// button down sets the bit, up clears the bit:

	if( state == GLUT_DOWN )
	{
		Xmouse = x;
		Ymouse = y;
		ActiveButton |= b;		// set the proper bit
	}
	else
	{
		ActiveButton &= ~b;		// clear the proper bit
	}

	glutSetWindow(MainWindow);
	glutPostRedisplay();

}


// called when the mouse moves while a button is down:

void
MouseMotion( int x, int y )
{
	int dx = x - Xmouse;		// change in mouse coords
	int dy = y - Ymouse;

	if( ( ActiveButton & LEFT ) != 0 )
	{
		Xrot += ( ANGFACT*dy );
		Yrot += ( ANGFACT*dx );
	}

	if( ( ActiveButton & MIDDLE ) != 0 )
	{
		Scale += SCLFACT * (float) ( dx - dy );

		// keep object from turning inside-out or disappearing:

		if( Scale < MINSCALE )
			Scale = MINSCALE;
	}

	Xmouse = x;			// new current position
	Ymouse = y;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// reset the transformations and the colors:
// this only sets the global variables --
// the glut main loop is responsible for redrawing the scene

void
Reset( )
{
	ActiveButton = 0;
	AxesOn = 1;
	DebugOn = 0;
	DepthBufferOn = 1;
	DepthFightingOn = 0;
	DepthCueOn = 0;
	Scale  = 1.0;
	ShadowsOn = 0;
	NowColor = YELLOW;
	NowProjection = PERSP;
	Xrot = Yrot = 0.;
    Frozen = false;
    LightType = POINT;
    LightColor = WH;
}


// called when user resizes the window:

void
Resize( int width, int height )
{
	// don't really need to do anything since window size is
	// checked each time in Display( ):

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// handle a change to the window's visibility:

void
Visibility ( int state )
{
	if( DebugOn != 0 )
		fprintf( stderr, "Visibility: %d\n", state );

	if( state == GLUT_VISIBLE )
	{
		glutSetWindow( MainWindow );
		glutPostRedisplay( );
	}
	else
	{
		// could optimize by keeping track of the fact
		// that the window is not visible and avoid
		// animating or redrawing it ...
	}
}



///////////////////////////////////////   HANDY UTILITIES:  //////////////////////////


// the stroke characters 'X' 'Y' 'Z' :

static float xx[ ] = { 0.f, 1.f, 0.f, 1.f };

static float xy[ ] = { -.5f, .5f, .5f, -.5f };

static int xorder[ ] = { 1, 2, -3, 4 };

static float yx[ ] = { 0.f, 0.f, -.5f, .5f };

static float yy[ ] = { 0.f, .6f, 1.f, 1.f };

static int yorder[ ] = { 1, 2, 3, -2, 4 };

static float zx[ ] = { 1.f, 0.f, 1.f, 0.f, .25f, .75f };

static float zy[ ] = { .5f, .5f, -.5f, -.5f, 0.f, 0.f };

static int zorder[ ] = { 1, 2, 3, 4, -5, 6 };

// fraction of the length to use as height of the characters:
const float LENFRAC = 0.10f;

// fraction of length to use as start location of the characters:
const float BASEFRAC = 1.10f;

//	Draw a set of 3D axes:
//	(length is the axis length in world coordinates)

void
Axes( float length )
{
	glBegin( GL_LINE_STRIP );
		glVertex3f( length, 0., 0. );
		glVertex3f( 0., 0., 0. );
		glVertex3f( 0., length, 0. );
	glEnd( );
	glBegin( GL_LINE_STRIP );
		glVertex3f( 0., 0., 0. );
		glVertex3f( 0., 0., length );
	glEnd( );

	float fact = LENFRAC * length;
	float base = BASEFRAC * length;

	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 4; i++ )
		{
			int j = xorder[i];
			if( j < 0 )
			{
				
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( base + fact*xx[j], fact*xy[j], 0.0 );
		}
	glEnd( );

	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 5; i++ )
		{
			int j = yorder[i];
			if( j < 0 )
			{
				
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( fact*yx[j], base + fact*yy[j], 0.0 );
		}
	glEnd( );

	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 6; i++ )
		{
			int j = zorder[i];
			if( j < 0 )
			{
				
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( 0.0, fact*zy[j], base + fact*zx[j] );
		}
	glEnd( );

}


// function to convert HSV to RGB
// 0.  <=  s, v, r, g, b  <=  1.
// 0.  <= h  <=  360.
// when this returns, call:
//		glColor3fv( rgb );

void
HsvRgb( float hsv[3], float rgb[3] )
{
	// guarantee valid input:

	float h = hsv[0] / 60.f;
	while( h >= 6. )	h -= 6.;
	while( h <  0. ) 	h += 6.;

	float s = hsv[1];
	if( s < 0. )
		s = 0.;
	if( s > 1. )
		s = 1.;

	float v = hsv[2];
	if( v < 0. )
		v = 0.;
	if( v > 1. )
		v = 1.;

	// if sat==0, then is a gray:

	if( s == 0.0 )
	{
		rgb[0] = rgb[1] = rgb[2] = v;
		return;
	}

	// get an rgb from the hue itself:
	
	float i = (float)floor( h );
	float f = h - i;
	float p = v * ( 1.f - s );
	float q = v * ( 1.f - s*f );
	float t = v * ( 1.f - ( s * (1.f-f) ) );

	float r=0., g=0., b=0.;			// red, green, blue
	switch( (int) i )
	{
		case 0:
			r = v;	g = t;	b = p;
			break;
	
		case 1:
			r = q;	g = v;	b = p;
			break;
	
		case 2:
			r = p;	g = v;	b = t;
			break;
	
		case 3:
			r = p;	g = q;	b = v;
			break;
	
		case 4:
			r = t;	g = p;	b = v;
			break;
	
		case 5:
			r = v;	g = p;	b = q;
			break;
	}


	rgb[0] = r;
	rgb[1] = g;
	rgb[2] = b;
}

void
Cross(float v1[3], float v2[3], float vout[3])
{
	float tmp[3];
	tmp[0] = v1[1] * v2[2] - v2[1] * v1[2];
	tmp[1] = v2[0] * v1[2] - v1[0] * v2[2];
	tmp[2] = v1[0] * v2[1] - v2[0] * v1[1];
	vout[0] = tmp[0];
	vout[1] = tmp[1];
	vout[2] = tmp[2];
}

float
Dot(float v1[3], float v2[3])
{
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}


float
Unit(float vin[3], float vout[3])
{
	float dist = vin[0] * vin[0] + vin[1] * vin[1] + vin[2] * vin[2];
	if (dist > 0.0)
	{
		dist = sqrtf(dist);
		vout[0] = vin[0] / dist;
		vout[1] = vin[1] / dist;
		vout[2] = vin[2] / dist;
	}
	else
	{
		vout[0] = vin[0];
		vout[1] = vin[1];
		vout[2] = vin[2];
	}
	return dist;
}


float
Unit( float v[3] )
{
	float dist = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
	if (dist > 0.0)
	{
		dist = sqrtf(dist);
		v[0] /= dist;
		v[1] /= dist;
		v[2] /= dist;
	}
	return dist;
}
