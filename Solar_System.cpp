/****************************SolarSystem.cpp**********************************/
//3D Texture Mapped Model of the Solar System
//Modeled to An Approximate Logarithmic Scale
//Particle System For The Sun Implemented


#include <gl/glut.h>
#include <windows.h>
#include <math.h>
#include "RGBpixmap.h" 
#include "SolarSystemGlobals.h"
#include "ParticleSystem.h"


/***************************************** Global Variables *****************************************/

// Viewer positioning angles.
GLfloat viewerAzimuth = INITIAL_VIEWER_AZIMUTH; //SolarSystemGlobals.h
GLfloat viewerZenith = INITIAL_VIEWER_ZENITH;

// ID numbers for all texture maps.
GLuint ParticleTextureName = -1;
GLuint SunTextureName = 0;
GLuint MercuryTextureName = 1;
GLuint VenusTextureName = 2;
GLuint EarthTextureName = 3;
GLuint MoonTextureName = 4;
GLuint MarsTextureName = 5;
GLuint JupiterTextureName = 6;
GLuint SaturnTextureName = 7;
GLuint UranusTextureName = 8;
GLuint NeptuneTextureName = 9;
GLuint RingTextureName = 10;


// Variables to keep track of current day status.
GLfloat CurrentEarthRotation = 0.00;
GLfloat EarthDaysTranspired = 0.00;
GLfloat EarthDayIncrement = 0.01;

// The initial window and viewport sizes (in pixels), set to ensure that the aspect ration for the viewport, 
//will be a constant. If the window is resized, the viewport will be adjusted to preserve the aspect ratio.
GLint currWindowSize[2] = { 750, 750 / ASPECT_RATIO };
GLint currViewportSize[2] = { 750, 750 / ASPECT_RATIO };

GLfloat ViewerDistance = INITIAL_VIEWER_DISTANCE; //SolarSystemGlobals.h

ParticleSystem particles;
bool particleFlag = true;
bool blendFlag = true;

/*******************Introduction*********************/
char line1[] = { "RNS INSTITUTE OF TECHNOLOGY" };
char line2[] = { "Computer Science Department" };
char line3[] = { "Computer Graphics Project" };
char line4[] = { "Topic: Solar System" };
char line5[] = { "NAMRATHA H V" };
char line6[] = { "1RN18CS067" };
char line7[] = { "SAHANA S GOWDA" };
char line8[] = { "1RN18CS089" };

/***************************************** Function prototypes *****************************************/

void KeyboardPress(unsigned char pressedKey, int mouseXPosition, int mouseYPosition);
void NonASCIIKeyboardPress(int pressedKey, int mouseXPosition, int mouseYPosition);
void TimerFunction(int value);
void Display();

void MakeAllImages();
void MakeImage(const char bitmapFilename[], GLuint& textureName, bool hasAlpha);

void SetLights();
void UpdateLight();

void ResizeWindow(GLsizei w, GLsizei h);

void drawEarthAndMoon();
void drawSun();
void drawSaturnRing();
void drawAllPlanets();
void drawGenericPlanet(GLfloat inclination, GLfloat orbitDuration,
	GLfloat orbitRadius, GLfloat rotationDuration, GLuint texturename, GLfloat radius);
void drawParticle(Particle currParticle);
void drawAllParticles();
//void drawStars();

/*******************Introduction*********************/
void IntroductionTimerFunction(int);
void displayText(float, float, int, int, int, const char*);
void myinit();
void Display_Intro();

/****************************************** Function implementations ******************************************/

// The main function sets up the data and the environment to display the textured objects. //

int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	// Set up the display window.
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_STENCIL | GLUT_DEPTH);
	glutInitWindowPosition(INIT_WINDOW_POSITION[0], INIT_WINDOW_POSITION[1]);
	glutInitWindowSize(currWindowSize[0], currWindowSize[1]);
	glutCreateWindow("Solar System");
	glutDisplayFunc(Display_Intro);
	glutTimerFunc(30000, IntroductionTimerFunction, 1); //after 30 seconds solar system is displayed
	// Specify the resizing and refreshing routines.
	//glutReshapeFunc(ResizeWindow);
	glutKeyboardFunc(KeyboardPress);
	glutSpecialFunc(NonASCIIKeyboardPress);
	//glutDisplayFunc(Display);
	glutTimerFunc(20, TimerFunction, 1);
	glViewport(0, 0, currWindowSize[0], currWindowSize[1]);

	// Set up standard lighting, shading, and depth testing.
	glShadeModel(GL_SMOOTH);
	//glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	glEnable(GL_NORMALIZE);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	SetLights();

	// Set up all texture maps and texture-mapped objects.
	MakeAllImages();

	//set up sound effect
	/*
	Project-> properties ->
	->Advanced -> Character Set: Use Multi-Byte Character Set
	->Linker -> Input -> Additional Dependencies: Winmm.lib
	*/
	//bool played = PlaySound("audio/Mars-sound.wav", NULL, SND_LOOP | SND_ASYNC);

	glutMainLoop();
	return 0;
}

/*******************Introduction*********************/
void Display_Intro()
{
	myinit();
	glClear(GL_COLOR_BUFFER_BIT);
	displayText(150, 600, 0, 0, 1, line1);
	displayText(195, 550, 0, 0, 1, line2);
	displayText(215, 500, 0, 0, 1, line3);
	displayText(250, 400, 1, 0, 0, line4);
	displayText(50, 300, 0, 1, 0, line5);
	displayText(50, 260, 0, 1, 0, line6);
	displayText(450, 300, 0, 1, 0, line7);
	displayText(450, 260, 0, 1, 0, line8);
	glutSwapBuffers();
	glFlush();
}

void myinit() {
	glMatrixMode(GL_PROJECTION_MATRIX);
	glLoadIdentity();
	gluOrtho2D(0, 750, 0, 750);
	glMatrixMode(GL_MODELVIEW);

}
void displayText(float x, float y, int r, int g, int b, const char* string) {
	int j = strlen(string);
	glClearColor(0, 0, 0, 0);
	glColor3f(r, g, b);
	glRasterPos2f(x, y);
	for (int i = 0; i < j; i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);

}
void IntroductionTimerFunction(int value)
{
	if (value == 1)
	{
		glutDisplayFunc(Display);
		glutReshapeFunc(ResizeWindow);
		glEnable(GL_DEPTH_TEST);
	}
}


//****************** Function to react to ASCII keyboard keys pressed by the user.   ****************//
// +/- keys are used to accelerate/decelerate the animation, while the z/Z keys are used to zoom in and out of the animation. //
void KeyboardPress(unsigned char pressedKey, int mouseXPosition, int mouseYPosition)
{
	char pressedChar = char(pressedKey);
	switch (pressedKey)
	{
		case '+': {                           //+ key is used to accelerate
			EarthDayIncrement *= 2.0;
			if (EarthDayIncrement > 10.0)
				EarthDayIncrement = 10.0;
			break;
		} 
		case '-': {                          //- keys is used to decelerate
			EarthDayIncrement *= 0.5;
			if (EarthDayIncrement < 0.01)
				EarthDayIncrement = 0.01;
			break;
		}
		case 'z': {                         //z key is used to zoom in 
			ViewerDistance -= VIEWER_DISTANCE_INCREMENT;
			if (ViewerDistance < MINIMUM_VIEWER_DISTANCE)
				ViewerDistance = MINIMUM_VIEWER_DISTANCE;
			break;
		}
		case 'Z': {                        //Z key is used to zoom  out
			ViewerDistance += VIEWER_DISTANCE_INCREMENT;
			if (ViewerDistance > MAXIMUM_VIEWER_DISTANCE)
				ViewerDistance = MAXIMUM_VIEWER_DISTANCE;
			break;
		}
				//enable and disable the particle system
		case 'p': {
			particleFlag = !particleFlag;
			break;
		}
				//enable and disable the blending function
		case 'b': {
			blendFlag = !blendFlag;
			break;
		}
	}
}

//**************** Function to react to non-ASCII keyboard keys pressed by the user. *******************//
// Used to alter spherical coordinates of the viewer's position.(TopView and FrontView)  //
void NonASCIIKeyboardPress(int pressedKey, int mouseXPosition, int mouseYPosition)
{
	glutIgnoreKeyRepeat(false);
	switch (pressedKey)
	{
		case GLUT_KEY_RIGHT: {
			viewerAzimuth += VIEWER_ANGLE_INCREMENT;
			if (viewerAzimuth > 2 * PI)
				viewerAzimuth -= 2 * PI;
			break;
		}
		case GLUT_KEY_LEFT: {
			viewerAzimuth -= VIEWER_ANGLE_INCREMENT;
			if (viewerAzimuth < 0.0)
				viewerAzimuth += 2 * PI;
			break;
		}
		case GLUT_KEY_UP: {
			viewerZenith -= VIEWER_ANGLE_INCREMENT;
			if (viewerZenith < VIEWER_ANGLE_INCREMENT)
				viewerZenith = VIEWER_ANGLE_INCREMENT;
			break;
		}
		case GLUT_KEY_DOWN: {
			viewerZenith += VIEWER_ANGLE_INCREMENT;
			if (viewerZenith > PI - VIEWER_ANGLE_INCREMENT)
				viewerZenith = PI - VIEWER_ANGLE_INCREMENT;
			break;
		}
	}
}

// The EarthDayIncrement represents the fraction of an Earth day being added to the scene in one screen update. //
void TimerFunction(int value)
{
	CurrentEarthRotation += EarthDayIncrement;
	EarthDaysTranspired += EarthDayIncrement;
	if (EarthDaysTranspired == EARTH_ORBIT_DUR)
		EarthDaysTranspired = 0;
	glutPostRedisplay();
	glutTimerFunc(20, TimerFunction, 1);
	// glutTimerFunc-> registers a timer callback to be triggered in a specified number of milliseconds.
	// void glutTimerFunc(unsigned int msecs,void (*func)(int value), value);
}

// Principal display routine: sets up material, lighting, and camera properties, clears the frame buffer, and 
//draws all texture-mapped objects within the window.    //
void Display()
{
	//Basic Light properties
	/*
		Diffuse- the way light "falls off" of an object (final_color(diffuse)=max(n.l,0)*cd*ld;)
		Specular- the "shininess"(s)  of the object (final_color(specular)=(n.h)^s*cs*ls;)
		Ambient- Used to simulate light bouncing around the environment/
				a minimum amount of light used to simulate "global illumination"/
				real world is far too complex for real time,so just add a little light!
				(final_color(ambient)=la+ca;)
		Emit- a "glowing" effect (final_color(emit)=le+ce;)
		The final polygon colour is the sum of all four light components.
	*/
	
	// Initialize lighting.
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LIGHT_MODEL_AMBIENT); 

	//glLightModel — set the lighting model parameters
	//void glLightModelfv(	GLenum pname,const GLfloat* params);
	/*GL_LIGHT_MODEL_AMBIENT -contains four fixed - point or floating - point values that specify the ambient 
	intensity of the entire scene.*/
	//LIGHT_MODEL_AMBIENT- The initial value is (0.2, 0.2, 0.2, 1.0).

	/*
	The lighted color of a vertex is the sum of the material emission intensity, the product of the material 
	ambient reflectance and the lighting model full-scene ambient intensity, and the contribution of each 
	enabled light source. Each light source contributes the sum of three terms: ambient, diffuse, and specular.
	The ambient light source contribution is the product of the material ambient reflectance and the light's 
	ambient intensity. The diffuse light source contribution is the product of the material diffuse reflectance,
	the light's diffuse intensity, and the dot product of the vertex's normal with the normalized vector from 
	the vertex to the light source. The specular light source contribution is the product of the material 
	specular reflectance, the light's specular intensity, and the dot product of the normalized vertex-to-eye 
	and vertex-to-light vectors, raised to the power of the shininess of the material. All three light source 
	contributions are attenuated equally based on the distance from the vertex to the light source and on light
	source direction, spread exponent, and spread cutoff angle. All dot products are replaced with 0 if they 
	evaluate to a negative value.

	The alpha component of the resulting lighted color is set to the alpha value of the material diffuse 
	reflectance.
	*/

	glEnable(GL_LIGHTING);

	// Set up the properties of the viewing camera.
	glMatrixMode(GL_PROJECTION); 
	/*The projection matrix defines the properties of the camera that views the objects in the world coordinate
	frame.Here you typically set the zoom factor, aspect ratio and the nearand far clipping planes. */
	glLoadIdentity();
	gluPerspective(60.0, ASPECT_RATIO, 0.2, 100.0); //Used to give depth to the scene
	//void gluPerspective(	GLdouble fovy,GLdouble aspect,GLdouble zNear,GLdouble zFar);

	/*
	fovy- Specifies the field of view angle, in degrees, in the y direction.
	aspect- Specifies the aspect ratio that determines the field of view in the x direction. The aspect ratio is the ratio of x (width) to y (height).
	zNear- Specifies the distance from the viewer to the near clipping plane (always positive).
	zFar- Specifies the distance from the viewer to the far clipping plane (always positive).
	*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	/*The modelview matrix defines how your objects are transformed(meaning translation, rotation and scaling)
	in your world coordinate frame*/
	glLoadIdentity();

	// Position and orient viewer. 
	gluLookAt(LOOK_AT_POSITION[0] + ViewerDistance * sin(viewerZenith) * sin(viewerAzimuth),
		LOOK_AT_POSITION[1] + ViewerDistance * cos(viewerZenith),
		LOOK_AT_POSITION[2] + ViewerDistance * sin(viewerZenith) * cos(viewerAzimuth),
		LOOK_AT_POSITION[0], LOOK_AT_POSITION[1], LOOK_AT_POSITION[2],
		0.0, 1.0, 0.020);
	//gluLookAt — define a viewing transformation


	// Render scene.
	UpdateLight();
	drawEarthAndMoon();
	drawSun();
	drawAllPlanets();
	drawSaturnRing();
	//drawStars(); //to display stars in the background

	if (blendFlag) { //if the user has selected blending, enable it.
		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);
		glBlendFunc(GL_SRC_COLOR, GL_ONE); //specify pixel arithmetic
	}
	if (particleFlag)
		drawAllParticles();

	glDepthMask(GL_TRUE); //enable or disable writing into the depth buffer
	glDisable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glutSwapBuffers(); //glutSwapBuffers swaps the buffers of the current window if double buffered.
	glFlush();
	/*glFlush empties all of these buffers, causing all issued commands to be executed as quickly as they are
	accepted by the actual rendering engine. Though this execution may not be completed in any particular time
	period, it does complete in finite time.*/
}

// Create the textures associated with all texture-mapped objects being displayed. //
void MakeAllImages()
{
	MakeImage(EARTH_BMP_FILENAME, EarthTextureName, false);
	MakeImage(MOON_BMP_FILENAME, MoonTextureName, false);
	MakeImage(SUN_BMP_FILENAME, SunTextureName, false);
	MakeImage(MERCURY_BMP_FILENAME, MercuryTextureName, false);
	MakeImage(VENUS_BMP_FILENAME, VenusTextureName, false);
	MakeImage(PARTICLE_BMP_FILENAME, ParticleTextureName, false);
	MakeImage(MARS_BMP_FILENAME, MarsTextureName, false);
	MakeImage(JUPITER_BMP_FILENAME, JupiterTextureName, false);
	MakeImage(SATURN_BMP_FILENAME, SaturnTextureName, false);
	MakeImage(URANUS_BMP_FILENAME, UranusTextureName, false);
	MakeImage(NEPTUNE_BMP_FILENAME, NeptuneTextureName, false);
	MakeImage(RING_BMP_FILENAME, RingTextureName, false);
	return;
}

//makes calls to the generic planet drawing function.we took this out of the display function to enhance readability
void drawAllPlanets() {
	drawGenericPlanet(MERCURY_INCLINATION, MERCURY_ORBIT_DUR, MERCURY_ORBIT_RADIUS,
		MERCURY_ROTATION_DUR, MercuryTextureName, MERCURY_RADIUS);
	drawGenericPlanet(VENUS_INCLINATION, VENUS_ORBIT_DUR, VENUS_ORBIT_RADIUS,
		VENUS_ROTATION_DUR, VenusTextureName, VENUS_RADIUS);
	drawGenericPlanet(MARS_INCLINATION, MARS_ORBIT_DUR, MARS_ORBIT_RADIUS,
		MARS_ROTATION_DUR, MarsTextureName, MARS_RADIUS);
	drawGenericPlanet(JUPITER_INCLINATION, JUPITER_ORBIT_DUR, JUPITER_ORBIT_RADIUS,
		JUPITER_ROTATION_DUR, JupiterTextureName, JUPITER_RADIUS);
	drawGenericPlanet(SATURN_INCLINATION, SATURN_ORBIT_DUR, SATURN_ORBIT_RADIUS,
		SATURN_ROTATION_DUR, SaturnTextureName, SATURN_RADIUS);
	drawGenericPlanet(URANUS_INCLINATION, URANUS_ORBIT_DUR, URANUS_ORBIT_RADIUS,
		URANUS_ROTATION_DUR, UranusTextureName, URANUS_RADIUS);
	drawGenericPlanet(NEPTUNE_INCLINATION, NEPTUNE_ORBIT_DUR, NEPTUNE_ORBIT_RADIUS,
		NEPTUNE_ROTATION_DUR, NeptuneTextureName, NEPTUNE_RADIUS);

}

// Convert the bitmap with the parameterized name into an OpenGL texture. //
void MakeImage(const char bitmapFilename[], GLuint& textureName, bool hasAlpha)
{
	RGBpixmap pix;
	pix.readBMPFile(bitmapFilename, hasAlpha);
	pix.setTexture(textureName);
	return;
}


// Set the two lights to illuminate the scene. //
void SetLights()
{
	
	//glLight - sets the values of individual light source parameters.
	//light - Specifies a light.light names the light and is a symbolic name of the form GL_LIGHTi (where i=0 to GL_MAX_LIGHT-1)
	glLightfv(GL_LIGHT0, GL_AMBIENT, LIGHT_AMBIENT);  //GL_AMBIENT-ambient RGBA intensity of the light
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LIGHT_DIFFUSE);  //GL_DIFFUSE-diffuse RGBA intensity of the light
	glLightfv(GL_LIGHT0, GL_SPECULAR, LIGHT_SPECULAR); //GL_SPECULAR-specular RGBA intensity of the light
	glLightfv(GL_LIGHT0, GL_POSITION, LIGHT_0_POSITION); //GL_POSITION-specify the position of the light in homogeneous object coordinates.

	glLightfv(GL_LIGHT1, GL_AMBIENT, LIGHT_AMBIENT);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LIGHT_DIFFUSE);
	glLightfv(GL_LIGHT1, GL_SPECULAR, LIGHT_SPECULAR);
	glLightfv(GL_LIGHT1, GL_POSITION, LIGHT_1_POSITION);

	glLightfv(GL_LIGHT2, GL_AMBIENT, LIGHT_AMBIENT);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, LIGHT_DIFFUSE);
	glLightfv(GL_LIGHT2, GL_SPECULAR, LIGHT_SPECULAR);
	glLightfv(GL_LIGHT2, GL_POSITION, LIGHT_2_POSITION);

	glLightfv(GL_LIGHT3, GL_AMBIENT, LIGHT_AMBIENT);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, LIGHT_DIFFUSE);
	glLightfv(GL_LIGHT3, GL_SPECULAR, LIGHT_SPECULAR);
	glLightfv(GL_LIGHT3, GL_POSITION, LIGHT_3_POSITION);
}


// Enable the scene's lighting. //
void UpdateLight()
{
	glPushMatrix();
	glLightfv(GL_LIGHT0, GL_POSITION, LIGHT_0_POSITION);
	glLightfv(GL_LIGHT1, GL_POSITION, LIGHT_1_POSITION);
	glLightfv(GL_LIGHT2, GL_POSITION, LIGHT_2_POSITION);
	glLightfv(GL_LIGHT3, GL_POSITION, LIGHT_3_POSITION);
	glPopMatrix();

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
}

// Window-reshaping callback, adjusting the viewport to be as large  //
// as possible within the window, without changing its aspect ratio. //
void ResizeWindow(GLsizei w, GLsizei h)
{
	currWindowSize[0] = w;
	currWindowSize[1] = h;
	if (ASPECT_RATIO > w / h)
	{
		currViewportSize[0] = w;
		currViewportSize[1] = w / ASPECT_RATIO;
	}
	else
	{
		currViewportSize[1] = h;
		currViewportSize[0] = h * ASPECT_RATIO;
	}

	glViewport(0.5 * (w - currViewportSize[0]), 0.5 * (h - currViewportSize[1]), currViewportSize[0], currViewportSize[1]);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


//Draws the texture-mapped earth and moon. //
void drawEarthAndMoon()
{
	GLfloat MoonRevolution = EarthDaysTranspired / LUNAR_CYCLE;
	GLUquadricObj* quadro = gluNewQuadric();  //to create Rendering Sphere quadric obj is used //gluNewQuadric()- To create a quadrics object
	gluQuadricNormals(quadro, GLU_SMOOTH);
	//gluQuadricNormals()- is used to specify when to generate normal vectors.
	//GLU_SMOOTH- generates one normal for every vertex of the quadric, which is usually best for lighting with
	//smooth shading.
	gluQuadricTexture(quadro, GL_TRUE);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glPushMatrix();
	glRotatef(EARTH_INCLINATION, 0.0, 0.0, 1.0);
	glRotatef(360.0 * (EarthDaysTranspired / EARTH_ORBIT_DUR), 0.0, 1.0, 0.0);
	glTranslatef(EARTH_ORBIT_RADIUS, 0.0, 0.0);
	glRotatef(360.0 * CurrentEarthRotation, 0.0, 1.0, 0.0);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glBindTexture(GL_TEXTURE_2D, EarthTextureName);
	gluSphere(quadro, EARTH_RADIUS, 48, 48);
	glPopMatrix();
	glRotatef(EARTH_INCLINATION, 0.0, 0.0, 1.0);
	glRotatef(360.0 * (EarthDaysTranspired / EARTH_ORBIT_DUR), 0.0, 1.0, 0.0);
	glTranslatef(EARTH_ORBIT_RADIUS, 0.0, 0.0);
	glRotatef(360.0 * MoonRevolution, 0.0, 1.0, 0.0);
	glTranslatef(MOON_ORBIT_RADIUS, 0.0, 0.0);
	glBindTexture(GL_TEXTURE_2D, MoonTextureName);
	gluSphere(quadro, MOON_RADIUS, 48, 48);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	gluDeleteQuadric(quadro);
}

//Function to draw and texture map the sun at the origin
void drawSun()
{
	GLUquadricObj* quadro = gluNewQuadric();
	gluQuadricNormals(quadro, GLU_SMOOTH);
	gluQuadricTexture(quadro, GL_TRUE);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glBindTexture(GL_TEXTURE_2D, SunTextureName);
	gluSphere(quadro, SUN_RADIUS, 48, 48);
	glPopMatrix();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	gluDeleteQuadric(quadro);
}


//Draw Rings
void drawSaturnRing()
{
	GLUquadricObj* quadro = gluNewQuadric();
	gluQuadricNormals(quadro, GLU_SMOOTH);
	gluQuadricTexture(quadro, GL_TRUE);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glPushMatrix();
	glRotatef(SATURN_INCLINATION, 0.0, 0.0, 1.0);
	glRotatef(360.0 * (EarthDaysTranspired / SATURN_ORBIT_DUR), 0.0, 1.0, 0.0);
	glTranslatef(SATURN_ORBIT_RADIUS, 0.0, 0.0);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glBindTexture(GL_TEXTURE_2D, RingTextureName);
	glScalef(1, 1, .02);
	gluSphere(quadro, SATURN_RADIUS * 2, 48, 48);
	glPopMatrix();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	gluDeleteQuadric(quadro);
}

//Given parameters about the planets dimension, orbit, radius etc, this function
//will draw a texture mapped plant.
//it is used to draw everything except the sun, earth/moon. and saturns rings, as
//they are special cases of this function
void drawGenericPlanet(GLfloat inclination, GLfloat orbitDuration,
	GLfloat orbitRadius, GLfloat rotationDuration, GLuint texturename, GLfloat radius)
{
	GLUquadricObj* quadro = gluNewQuadric();
	gluQuadricNormals(quadro, GLU_SMOOTH);
	gluQuadricTexture(quadro, GL_TRUE);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glPushMatrix();
	glRotatef(inclination, 0.0, 0.0, 1.0);
	glRotatef(360.0 * (EarthDaysTranspired / orbitDuration), 0.0, 1.0, 0.0);
	glTranslatef(orbitRadius, 0.0, 0.0);
	glRotatef(360.0 * (CurrentEarthRotation / rotationDuration), 0.0, 1.0, 0.0);
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glBindTexture(GL_TEXTURE_2D, texturename);
	gluSphere(quadro, radius, 48, 48);
	glPopMatrix();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	gluDeleteQuadric(quadro);
}

//Cycles through each particle in the particle system and passes it to the
//draw function
void drawAllParticles() {
	particles.updateAll();
	for (int i = 0; i < particles.getNumberOfParticles(); i++)
		drawParticle(particles.getNextParticle());

}

//Draws one individual particle, given a particle struct from the particle
//system object
void drawParticle(Particle currParticle)
{
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glRotatef(currParticle.azimuthRotation, 0, 1, 0);
	glRotatef(currParticle.zenithRotation, 0, 0, 1);
	glTranslatef(SUN_RADIUS + currParticle.surfaceTranslationFactor, 0, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(.5, .5, 1.0);
	glBindTexture(GL_TEXTURE_2D, ParticleTextureName);

	// Logo Facing Earth
	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2d(1, 1);
	glVertex3f(0.5f, 0.5f, 0.0f); // Top Right
	glTexCoord2d(0, 1);
	glVertex3f(-0.5f, 0.5f, 0.0f); // Top Left
	glTexCoord2d(1, 0);
	glVertex3f(0.5f, -0.5f, 0.0f); // Bottom Right
	glTexCoord2d(0, 0);
	glVertex3f(-0.5f, -0.5f, 0.0f); // Bottom Left
	glEnd();

	// Logo Facing Away From Earth
	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2d(1, 1);
	glVertex3f(-0.5f, 0.5f, 0.0f); // Top Right
	glTexCoord2d(0, 1);
	glVertex3f(0.5f, 0.5f, 0.0f); // Top Left
	glTexCoord2d(1, 0);
	glVertex3f(-0.5f, -0.5f, 0.0f); // Bottom Right
	glTexCoord2d(0, 0);
	glVertex3f(0.5f, -0.5f, 0.0f); // Bottom Left
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

//to display stars in the background
/*
void drawStars()
{
	//to put the stars as a background
	glPushMatrix();
	glTranslatef(0.0, -2.0, 0.0);
	gluLookAt(0.0, 10.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 3.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glScalef(200.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, 2.0, 0.0);
	gluLookAt(0.0, 10.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 3.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glScalef(200.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, -4.0, 0.0);
	gluLookAt(0.0, 10.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 3.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glScalef(200.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, 4.0, 0.0);
	gluLookAt(0.0, 10.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 3.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glScalef(200.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.1, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, -6.0, 0.0);
	gluLookAt(0.0, 10.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 3.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glScalef(200.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.1, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, 6.0, 0.0);
	gluLookAt(0.0, 10.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 3.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glScalef(200.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.1, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, -8.0, 0.0);
	gluLookAt(0.0, 10.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 3.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glScalef(200.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.1, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, 8.0, 0.0);
	gluLookAt(0.0, 10.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 3.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glScalef(200.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.1, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(8.0, 0.0, 0.0);
	gluLookAt(0.0, 10.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 3.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glScalef(200.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-8.0, -2.0, 0.0);
	gluLookAt(0.0, 10.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 3.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glScalef(200.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(6.0, 4.0, 0.0);
	gluLookAt(0.0, 10.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 3.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glScalef(200.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-6.0, 4.0, 0.0);
	gluLookAt(0.0, 10.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 3.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glScalef(200.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(5.0, -4.0, 0.0);
	gluLookAt(0.0, 10.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 3.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glScalef(200.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-7.0, 3.0, 0.0);
	gluLookAt(0.0, 10.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 3.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glScalef(200.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-7.0, 2.0, 0.0);
	gluLookAt(0.0, 10.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 3.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glScalef(200.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(7.0, -2.0, 0.0);
	gluLookAt(0.0, 10.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 3.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glScalef(200.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(7.0, -3.0, 0.0);
	gluLookAt(0.0, 10.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 3.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glScalef(200.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-7.0, -3.0, 0.0);
	gluLookAt(0.0, 10.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 3.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glScalef(200.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(7.0, 2.0, 0.0);
	gluLookAt(0.0, 10.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 3.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glScalef(200.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(1.0, -7.0, 0.0);
	gluLookAt(0.0, 10.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 3.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glScalef(200.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(2.0, -5.0, 0.0);
	gluLookAt(0.0, 10.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 3.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glScalef(200.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(5.0, -1.0, 0.0);
	gluLookAt(0.0, 10.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.07, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-6.0, 7.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.07, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-0.5, 3.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.07, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-1.5, 4.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.07, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-9.0, 3.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.07, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(9.0, -5.9, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.1, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(6.5, 8.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.1, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(5.0, 7.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.1, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-9.0, 6.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.1, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-10.5, 9.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.07, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-11.0, -7.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.07, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-11.0, 5.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.05, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-7.0, -5.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-10.0, 3.7, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-7.0, -2.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-8.0, 5.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.03, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-8.0, -5.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.05, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-11.0, -4.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(6.0, -5.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-6.9, 7.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(5.0, -4.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.05, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(6.0, 4.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(3.0, -4.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(4.0, -7.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.05, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-4.0, -3.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(4.0, -7.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(11.0, -4.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.05, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(9.0, -9.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(8.0, -4.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.05, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(9.0, 5.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(7.0, 7.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.9, 7.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(1.0, 6.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.8, -5.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(3.0, -7.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.05, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(1.0, 5.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(2.0, 4.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, 7.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.05, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, 5.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-9.0, 0.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.05, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-10.0, 4.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(9.0, 3.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.05, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-10.0, -6.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(10.0, 0.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.05, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-9.0, -7.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-3.0, 4.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.05, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-9.9, -7.5, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(1.0, 5.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.05, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(3.0, 6.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-2.0, -5.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.05, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-3.0, -2.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-4.0, -8.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.05, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(8.3, -7.1, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-10.0, 7.6, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.05, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-3.0, 7.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-1.4, 7.5, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.05, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(3.0, 6.5, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-6.0, 4.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-6.0, -6.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.05, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.7, 4.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(2.0, 2.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glScalef(200.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glScalef(200.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, -1.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glScalef(200.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, 1.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glScalef(200.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, 2.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 0.0, 0.0, 0.0);
	glScalef(200.0, 0.0, 0.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(8.7, 9.0, 0.0);
	gluLookAt(0.0, 10.0, 2.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	glRotatef((GLfloat)b, 1.0, 7.0, 5.0);
	glColor3f(4.3, 3.5, 1.0);
	glutSolidSphere(0.04, 20, 8);
	glPopMatrix();
}
*/