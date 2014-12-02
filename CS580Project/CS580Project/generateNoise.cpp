#include <math.h>
#include "generateNoise.h"

float finddensity(float x, float y, float height, float width, float noise, float radius) {

	float center_x = (width / 2.0);
	float center_y = (height / 2.0);

	float distance = sqrt(pow((x - center_x), 2) + pow((y - center_y), 2));

	noise = noise + (1.0 - (distance / radius));

	if (noise  < 0) {
		noise = 0.0;
	}

	return noise;
}

int generateTexture2D(char *framebuffer, int width, int height) {

	GzColor textureColor = { 0, 0, 0 };
	int finalColor[] = { 0, 0, 0 };

	char *copybuffer = framebuffer;

	Perlin *per = new Perlin(4, 0.5, 2.0, 10);

	for (float x = 0; x < 256; x += 1) {
		for (float y = 0; y < 256; y += 1) {

			//float noise = per->Get(x, y);

			//textureColor[RED] = finddensity(y, x, 256, 256, noise, 160);
			//textureColor[GREEN] = finddensity(y, x, 256, 256, noise, 160);

			float noise = perlin_type1_2D(x, y);

			textureColor[RED] = noise; // finddensity(y, x, 256, 256, noise, 160);
			textureColor[GREEN] = noise;
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