#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <gl\glut.h>

#include "imageloader.h"
#include "generateNoise.h"
#include "generateNoise3D.h"

using namespace std;

GLuint texture;
GLuint texture1;
GLfloat angle = 0.0;
GLUquadric *quad;
bool flag = true;

GLuint LoadTexture3(const char * filename, int width, int height){

	unsigned char * data;
	FILE * file;

	file = fopen(filename, "rb");
	if (file == NULL) return 0;
	data = (unsigned char *)malloc(width * height * 3);
	fread(data, width * height * 3, 1, file);
	fclose(file);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_NEAREST);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height,
		GL_BGR_EXT, GL_UNSIGNED_BYTE, data);

	free(data);

	return texture;

}

GLuint loadTexture() {
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	int width = 256;
	int height = 256;

	char *framebuffer = (char *)malloc((3 * sizeof(char) * width * height));

	generateTexture3D(framebuffer, width, height);

	glTexImage2D(GL_TEXTURE_2D,
				 0,
				 GL_RGB,
				 width, height,
				 0, 
				 GL_RGB,
				 GL_UNSIGNED_BYTE,                  
				 framebuffer);
	return textureId;
}


GLuint _textureId, textureId2; 
float translate_1, translate_2, translate_3, translate_4, translate_5, translate_6;


void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	quad = gluNewQuadric();

	if (flag){
	textureId2 = loadTexture();
	_textureId = LoadTexture3("Sky-and-trees.bmp", 800, 800);
}
}

void handleResize(int w, int h) {
	if (flag){
		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0, (float)w / (float)h, 1.0, 200.0);
	}
}

void rect() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId);

	glPushMatrix();
	glRotatef(angle, 0.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS);

	glTexCoord2d(0.0, 0.0); glVertex2d(-7.0, -8.5);
	glTexCoord2d(1.0, 0.0); glVertex2d(+7.0, -8.5);
	glTexCoord2d(1.0, 1.0); glVertex2d(+7.0, +8.5);
	glTexCoord2d(0.0, 1.0); glVertex2d(-7.0, +8.5);
	glEnd();

	glPopMatrix();
	glutSwapBuffers();
}

void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	glTranslatef(0.0f, 1.0f, -16.0f);
	rect();

	glTranslatef(3.0f, 2.0f, 0.0f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureId2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glRotatef(270,1.0f,0.0f,0.0f);;

	glScalef(1.7, 0.9, 1);
	gluQuadricTexture(quad,1);

	glTranslatef(-3.5f, 0.0f, 1.5f);
	gluSphere(quad, 2.5, 20, 20);

	glTranslatef(4.0f, 0.0f, -0.4f);
	gluSphere(quad, 2.2, 20, 20);

	glTranslatef(-4.0f, 0.0f, -2.5f);
	gluSphere(quad, 1.6, 20, 20);

	glTranslatef(2.5f, 0.0f, 0.0f);
	gluSphere(quad, 1.2, 20, 20);

	glTranslatef(2.0f, 0.0f, 0.0f);
	gluSphere(quad, 1.5, 20, 20);

	glTranslatef(-2.0f, 0.0f, -2.0f);
	gluSphere(quad, 1, 20, 20);

	glTranslatef(2.0f, 0.0f, 0.0f);
	gluSphere(quad, 0.3, 20, 20);

	glTranslatef(0.0f, 0.0f, 0.2f);
	gluSphere(quad, 0.3, 20, 20);

	glTranslatef(0.2f, 0.0f, 0.0f);
	gluSphere(quad, 0.3, 10, 10);

	glTranslatef(0.0f, 0.0f, -0.2f);
	gluSphere(quad, 0.3, 20, 20);

	glTranslatef(-0.1f, 0.0f, -0.2f);
	gluSphere(quad, 0.2, 20, 20);

	/*glTranslatef(-2.0f, 0.0f, -0.4f);
	gluSphere(quad, 2.2, 10, 10);*/

	/*glTranslatef(-4.0f, 0.0f, 0.7f);
	gluSphere(quad, 1.5, 10, 10);

	glTranslatef(0.0f, 0.0f, -0.7f);
	gluSphere(quad, 1, 10, 10);

	glTranslatef(-4.0f, 0.0f, 0.7f);
	gluSphere(quad, 1.5, 10, 10);

	glTranslatef(0.0f - translate_1 + translate_3, 0.0f, 2.5f);
	gluSphere(quad, 2, 10, 10);*/

	glutSwapBuffers();
}
void update(int value)
{
	flag = false;
	translate_1 += 0.010f;
	translate_2 += 0.002f;
	translate_3 += 0.011f;
	if (translate_1>5.1)
    {
		translate_1 = -9.0f;
    }
	if (translate_2>11.1)
	{
		translate_2 = -9.0f;
	}
	if (translate_3>8.1)
	{
		translate_3 = -13.0f;
	}
    glutPostRedisplay();
    glutTimerFunc(25,update,0);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);

	glutCreateWindow("Cloud Rendering");
	initRendering();

	//glutTimerFunc(25,update,0);

	glutDisplayFunc(drawScene);
	glutReshapeFunc(handleResize);

	glutMainLoop();
	return 0;
}