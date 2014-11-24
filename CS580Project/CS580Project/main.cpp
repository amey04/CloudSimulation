
#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <GL/glut.h>
//#include "stdafx.h"

#include "imageloader.h"

using namespace std;

GLuint texture; //the array for our texture
GLuint texture1;
GLfloat angle = 0.0;

GLuint LoadTexture(const char * filename, int width, int height){

	//    GLuint texture;
	unsigned char * data;
	FILE * file;

	//The following code will read in our RAW file
	file = fopen(filename, "rb");
	if (file == NULL) return 0;
	data = (unsigned char *)malloc(width * height * 3);
	fread(data, width * height * 3, 1, file);
	fclose(file);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//    // when texture area is small, bilinear filter the closest mipmap
	//    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
	//                    GL_LINEAR_MIPMAP_NEAREST );
	//    // when texture area is large, bilinear filter the original
	//    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	//    
	//    // the texture wraps over at the edges (repeat)
	//    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	//    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	//    
	//    //Generate the texture
	//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0,GL_RGB, GL_UNSIGNED_BYTE, data);



	// select modulate to mix texture with color for shading
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// when texture area is small, bilinear filter the closest mipmap
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_NEAREST);
	// when texture area is large, bilinear filter the first mipmap
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//    // the texture wraps over at the edges (repeat)
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// build our texture mipmaps
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height,
		GL_BGR_EXT, GL_UNSIGNED_BYTE, data);

	free(data);

	return texture; //return whether it was successful

}

void FreeTexture(GLuint texture){
	glDeleteTextures(1, &texture);
}

void cube() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture); //bind the texture

	glPushMatrix();
	glRotatef(angle, 0.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0); glVertex2d(-2.5, -2.5);
	glTexCoord2d(1.0, 0.0); glVertex2d(+2.5, -2.5);
	glTexCoord2d(1.0, 1.0); glVertex2d(+2.5, +2.5);
	glTexCoord2d(0.0, 1.0); glVertex2d(-2.5, +2.5);
	glEnd();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture1); //bind the texture
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0); glVertex2d(-.5, -.5);
	glTexCoord2d(1.0, 0.0); glVertex2d(+.5, -.5);
	glTexCoord2d(1.0, 1.0); glVertex2d(+.5, +.5);
	glTexCoord2d(0.0, 1.0); glVertex2d(-.5, +.5);
	glEnd();
	glPopMatrix();
	glutSwapBuffers();
	//glutSolidCube(2);
}

void display() {
	//glClearColor(0.0, 0.0, 0.0, 1.0);
	//glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	texture = LoadTexture("Sky-and-trees.bmp", 2560, 1920); //load the texture
	texture1 = LoadTexture("earth.bmp", 1024, 512); //load the texture
	glEnable(GL_TEXTURE_2D); //enable 2D texturing
	//    glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
	//    glEnable(GL_TEXTURE_GEN_T);
	cube();
	FreeTexture(texture);
	//glutSwapBuffers();
	//angle ++;
}

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	//glLoadIdentity ();
	gluPerspective(50, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(800, 800);
	//glutInitWindowPosition(100, 100);
	glutCreateWindow("A basic OpenGL Window");
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}