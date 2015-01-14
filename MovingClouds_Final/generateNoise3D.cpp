#include <math.h>
#include "generateNoise3D.h"

float noise3d[256][256][256];

float finddensity3D(float x, float y, float height, float width, float noise, float radius) {

	float center_x = (width / 2.0);
	float center_y = (height / 2.0);

	float distance = sqrt(pow((x - center_x), 2) + pow((y - center_y), 2));

	if (noise >= 1)
		noise = 0.5;
	noise = noise + (1.0 - (distance / radius));

	if (noise  < 0) {
		noise = 0.0;
	}

	return noise;
}

int generateTexture3D(char *framebuffer, int width, int height) {

	GzColor textureColor = { 0, 0, 0 };
	int finalColor[] = { 0, 0, 0 };

	char *copybuffer = framebuffer;

	Perlin *per = new Perlin(4, 0.5, 2.0, 10);

	for (float x = 0; x < 256; x += 1) {
		for (float y = 0; y < 256; y += 1) {
			
			float noise = 0.0f;
			
			for (float z = 0; z < 256; z += 10){
				 noise += noise3d[(int)x][(int)y][(int)z] = per->Get(x, y, z);
			}
			noise /= 25;

			float noise_value = finddensity3D(y, x, 256, 256, noise, 45);
			float alpha = noise_value / 1.0f;
			
			textureColor[RED] = noise;
			textureColor[GREEN] = noise;
			textureColor[BLUE] = noise;

			finalColor[RED] = (int)(textureColor[RED] * 255) + 220;
			finalColor[GREEN] = (int)(textureColor[GREEN] * 255) + 220;
			finalColor[BLUE] = (int)(textureColor[BLUE] * 255) + 220;
			

			/*finalColor[RED] = (int)((finalColor[RED])*alpha + (1 - alpha) * 162);
			finalColor[GREEN] = (int)((finalColor[GREEN])*alpha + (1 - alpha) * 201);
			finalColor[BLUE] = (int)((finalColor[BLUE])*alpha + (1 - alpha) * 234);*/

			finalColor[RED] = (int)((finalColor[RED])*alpha + (1 - alpha) * 69);
			finalColor[GREEN] = (int)((finalColor[GREEN])*alpha + (1 - alpha) * 137);
			finalColor[BLUE] = (int)((finalColor[BLUE])*alpha + (1 - alpha) * 198);

			if (finalColor[RED] >= 255)
				finalColor[RED] = 234;
			if (finalColor[RED] < 0)
				finalColor[RED] = 0;
			if (finalColor[GREEN] >= 255)
				finalColor[GREEN] = 234;
			if (finalColor[GREEN] < 0)
				finalColor[GREEN] = 0;
			if (finalColor[BLUE] >= 255)
				finalColor[BLUE] = 234;
			if (finalColor[BLUE] < 0)
				finalColor[BLUE] = 0;

			*(framebuffer++) = (char)(finalColor[RED]);
			*(framebuffer++) = (char)(finalColor[GREEN]);
			*(framebuffer++) = (char)(finalColor[BLUE]);

		}
	}
	return 0;
}