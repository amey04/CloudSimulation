#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <gl\glut.h>
//#include "stdafx.h"

#include "imageloader.h"
#include "generateNoise.h"
#include "generateNoise3D.h"

using namespace std;

GLuint texture; //the array for our texture
GLuint texture1;
GLfloat angle = 0.0;
GLUquadric *quad;

void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
		case 27: //Escape key
			exit(0);
	}
}

GLuint loadTexture2(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
		0,                            //0 for now
		GL_RGB,                       //Format OpenGL uses for image
		//image->width, image->height,  //Width and height
		0, 0, 
		0,                            //The border of the image
		GL_RGB, //GL_RGB, because pixels are stored in RGB format
		GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
		//as unsigned numbers
		image->pixels);               //The actual pixel data
	return textureId; //Returns the id of the texture
}

GLuint LoadTexture3(const char * filename, int width, int height){

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

//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit

	int width = 256;
	int height = 256;

	char *framebuffer = (char *)malloc((3 * sizeof(char) * width * height));

	//generateTexture(framebuffer, width, height);
	//generateTexture2D(framebuffer, width, height);
	generateTexture3D(framebuffer, width, height);

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


GLuint _textureId, textureId2; //The id of the textur
//GLUquadric *quad;
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
	textureId2 = loadTexture(image);
	_textureId = LoadTexture3("Sky-and-trees.bmp", 1024, 1024);
	delete image;
}

void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)w / (float)h, 1.0, 200.0);
}

void rect() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId); //bind the texture

	glPushMatrix();
	glRotatef(angle, 0.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS);

	glTexCoord2d(0.0, 0.0); glVertex2d(-7.0, -8.5);
	glTexCoord2d(1.0, 0.0); glVertex2d(+7.0, -8.5);
	glTexCoord2d(1.0, 1.0); glVertex2d(+7.0, +8.5);
	glTexCoord2d(0.0, 1.0); glVertex2d(-7.0, +8.5);
	glEnd();

	/*glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture1); //bind the texture
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0); glVertex2d(-.5, -.5);
	glTexCoord2d(1.0, 0.0); glVertex2d(+.5, -.5);
	glTexCoord2d(1.0, 1.0); glVertex2d(+.5, +.5);
	glTexCoord2d(0.0, 1.0); glVertex2d(-.5, +.5);
	glEnd();*/
	glPopMatrix();
	glutSwapBuffers();
	//glutSolidCube(2);
}

void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	glTranslatef(0.0f, 1.0f, -16.0f);
	rect();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureId2);

	//Bottom
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glRotatef(270,1.0f,0.0f,0.0f);
	
	glRotatef(rotate_1,0.0f,0.0f,1.0f);
	//glTranslatef(rotate_1, 0.0f, 0.0f);

	glScalef(1.7, 0.9, 1);
	gluQuadricTexture(quad,1);
    gluSphere(quad,1,20,20);
	//glRectf(-5.0f, 5.0f, 5.0f, -5.0f);*/

	/*glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureId2);
	gluQuadricTexture(quad, 1);
	glVertex2d(-2.5, -2.5);
	glVertex2d(+2.5, -2.5);
	glVertex2d(+2.5, +2.5);
	glVertex2d(-2.5, +2.5);*/
	
	//glTranslatef(1.6f, 0.0f, 0.0f);
	//gluSphere(quad, 1, 20, 20);
	//glTranslatef(.7f, 0.0f, 0.0f);
	//gluSphere(quad, 1, 20, 20);

	glutSwapBuffers();
}
void update(int value)
{
    rotate_1+=0.020f;
    if(rotate_1>11.1)
    {
        rotate_1 = 0.0f;
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