#include <math.h>
#include "generateNoise3D.h"

float noise3d[512][256][256];

float finddensity3D(float x, float y, float height, float width, float noise, float radius) {

	float center_x = (width / 2.0);
	float center_y = (height / 2.0);

	float distance = sqrt(pow((x - center_x), 2) + pow((y - center_y), 2));

	noise = noise + (1.0 - (distance / radius));

	if (noise  < 0) {
		noise = 0.0;
	}

	return noise;
}

int generateTexture3D(char *framebuffer, int width, int height, char *imagePx) {

	GzColor textureColor = { 0, 0, 0 };
	int finalColor[] = { 0, 0, 0 };

	char *copybuffer = framebuffer;
	
	Perlin *per = new Perlin(4, 0.5, 2.0, 10);

	for (float x = 0; x < width; x += 1) {
		for (float y = 0; y < height; y += 1) {
			
			float noise = 0.0f;
			
			for (float z = 0; z < 256; z += 10){
				 noise += noise3d[(int)x][(int)y][(int)z] = per->Get(x, y, z);
				 //noise += noise3d[(int)x][(int)y][(int)z] = ken_perlin_3D(x, y, z);
			}
			noise /= 25;

			textureColor[RED] = finddensity3D(y, x, 256, 256, noise, 45);
			textureColor[GREEN] = finddensity3D(y, x, 256, 256, noise, 45);

			//textureColor[RED] = noise; 
			//textureColor[GREEN] = noise;
			textureColor[BLUE] = 0.8;

			finalColor[RED] = (int)(textureColor[RED] * 255);
			finalColor[GREEN] = (int)(textureColor[GREEN] * 255);
			finalColor[BLUE] = (int)(textureColor[BLUE] * 255);

			if (finalColor[RED] > 255)
				finalColor[RED] = 255;
			if (finalColor[RED] < 0)
				finalColor[RED] = 0;
			if (finalColor[GREEN] > 255)
				finalColor[GREEN] = 255;
			if (finalColor[GREEN] < 0)
				finalColor[GREEN] = 0;
			if (finalColor[BLUE] > 255)
				finalColor[BLUE] = 255;
			if (finalColor[BLUE] < 0)
				finalColor[BLUE] = 0;

			*(framebuffer++) = (char)(finalColor[RED]);
			*(framebuffer++) = (char)(finalColor[GREEN]);
			*(framebuffer++) = (char)(finalColor[BLUE]);

		}
	}
	return 0;
}