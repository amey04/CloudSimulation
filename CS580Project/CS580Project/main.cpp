#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <glut.h>
//#include "stdafx.h"

#include "imageloader.h"
#include "generateNoise.h"
#include "generateNoise3D.h"

using namespace std;

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

	int width = image->width;
	int height = image->height;
	char *framebuffer = (char *)malloc((3 * sizeof(char) * width * height));


	//generateTexture(framebuffer, width, height);
	//generateTexture2D(framebuffer, width, height);
	//generateTexture3D(framebuffer, width, height, image->pixels);
	generateNoiseType2(framebuffer, width, height, image->pixels);

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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glRotatef(270,1.0f,0.0f,0.0f);
	
	glRotatef(rotate_1,0.0f,0.0f,1.0f);
	//glTranslatef(rotate_1, 0.0f, 0.0f);

	//glScalef(1.7, 0.9, 1);
	gluQuadricTexture(quad,1);
    gluSphere(quad,4,20,20);
	//glutSolidCube(12);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId);
	//glTranslatef(1.6f, 0.0f, 0.0f);
	//gluSphere(quad, 1, 20, 20);
	//glTranslatef(.7f, 0.0f, 0.0f);
	//gluSphere(quad, 1, 20, 20);

	glutSwapBuffers();
}
void update(int value)
{
    rotate_1+=2.0f;
    if(rotate_1>360.f)
    {
        rotate_1 -= 360;
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

	glutTimerFunc(25,update,0);

	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);

	glutMainLoop();
	return 0;
}