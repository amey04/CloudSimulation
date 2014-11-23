#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <glut.h>
//#include "stdafx.h"

#include "imageloader.h"

typedef short   GzIntensity;
typedef float   GzColor[3];
#define RED     0               /* array indicies for color vector */
#define GREEN   1
#define BLUE    2

using namespace std;

inline double interpolate1(double a, double b, double x)
{
	double ft = x * 3.1415927;
	double f = (1.0 - cos(ft))* 0.5;
	return a*(1.0 - f) + b*f;
}

inline double findnoise2(double x, double y)
{
	int n = (int)x + (int)y * 57;
	n = (n << 13) ^ n;
	int nn = (n*(n*n * 60493 + 19990303) + 1376312589) & 0x7fffffff;
	return 1.0 - ((double)nn / 1073741824.0);
}

double noise(double x, double y)
{
	double floorx = (double)((int)x);//This is kinda a cheap way to floor a double integer.
	double floory = (double)((int)y);

	double s, t, u, v;//Integer declaration
	s = findnoise2(floorx, floory);
	t = findnoise2(floorx + 1, floory);
	u = findnoise2(floorx, floory + 1);//Get the surrounding pixels to calculate the transition.
	v = findnoise2(floorx + 1, floory + 1);

	double int1 = interpolate1(s, t, x - floorx);//Interpolate between the values.
	double int2 = interpolate1(u, v, x - floorx);//Here we use x-floorx, to get 1st dimension. Don't mind the x-floorx thingie, it's part of the cosine formula.

	return interpolate1(int1, int2, y - floory);//Here we use y-floory, to get the 2nd dimension.

}

short	ctoi(float color)		/* convert float color to GzIntensity short */
{
	return(short)((int)(color * ((1 << 12) - 1)));
}

int ptex_fun(float u, float v, GzColor color)
{
	int octave = 2;
	double noise_value = 0.0;

	//permute();

	for (int a = 0; a < octave; a++) {
		double f = pow(2.0, a);
		double amplitude = pow(0.5, a);
		noise_value += noise((u * f) / 75, v / (75 * f)) * a;
		//noise_value += generatenoise((u * f) / 75, v / (75 * f)) * a;
	}

	float noisevalue = noise_value;


	noisevalue = (noisevalue + 1) / 2.0;

	color[RED] = noisevalue;
	color[GREEN] = (noisevalue);
	//color[BLUE] = noisevalue;
	color[BLUE] = 1;

	if (color[RED] > 1)
		color[RED] = 1;
	if (color[GREEN] > 1)
		color[GREEN] = 1;
	if (color[BLUE] > 1)
		color[BLUE] = 1;

	return 0;
}

void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
		case 27: //Escape key
			exit(0);
	}
}

//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit

	int width = 256;
	int height = 256;

	char *framebuffer = (char *)malloc((3 * sizeof(char) * width * height));

	GzColor textureColor = { 0, 0, 0 };
	GzIntensity finalColor[] = { 0, 0, 0 };
	
	for (int i = 0; i < 256; i++){
		for (int j = 0; j < 256; j++){
			
			textureColor[RED] = 0;
			textureColor[GREEN] = 0;
			textureColor[BLUE] = 0;

			ptex_fun(i, j, textureColor);

			finalColor[RED] = ctoi(textureColor[RED]);
			finalColor[GREEN] = ctoi(textureColor[RED]);
			finalColor[BLUE] = ctoi(textureColor[RED]);
			
			*(framebuffer++) = (char)(finalColor[RED] >> 4);
			*(framebuffer++) = (char)(finalColor[GREEN] >> 4);
			*(framebuffer++) = (char)(finalColor[BLUE] >> 4);
		}
	}


	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
				 0,                            //0 for now
				 GL_RGB,                       //Format OpenGL uses for image
				 width, height,  //Width and height
				 0,                            //The border of the image
				 GL_RGB, //GL_RGB, because pixels are stored in RGB format
				 GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
				                   //as unsigned numbers
				 framebuffer);               //The actual pixel data
	return textureId; //Returns the id of the texture
}

GLuint _textureId; //The id of the textur
GLUquadric *quad;
float rotate_1;

//GLUquadricObj quad;

void initRendering() {
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	quad = gluNewQuadric();

	Image* image = loadBMP("earth.bmp");
	_textureId = loadTexture(image);
	delete image;
}

void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)w / (float)h, 1.0, 200.0);
}

void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	glTranslatef(0.0f, 1.0f, -16.0f);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId);

	//Bottom
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glRotatef(90,1.0f,0.0f,0.0f);
	glRotatef(rotate_1,0.0f,0.0f,1.0f);
	
	gluQuadricTexture(quad,1);
    gluSphere(quad,2,20,20);

	glutSwapBuffers();
}
void update(int value)
{
    rotate_1+=2.0f;
    if(rotate_1>360.f)
    {
        rotate_1-=360;
    }
    glutPostRedisplay();
    glutTimerFunc(25,update,0);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);

	glutCreateWindow("Textures - videotutorialsrock.com");
	initRendering();

	//glutTimerFunc(25,update,0);

	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);

	glutMainLoop();
	return 0;
}