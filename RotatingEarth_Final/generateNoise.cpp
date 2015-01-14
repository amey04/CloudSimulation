#include <math.h>
#include <cstdlib>

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

int generateTexture2D(char *framebuffer, int width, int height, char* imagePx) {

	GzColor textureColor = { 0, 0, 0 };
	int finalColor[] = { 0, 0, 0 };

	char *copybuffer = framebuffer;

	Perlin *per = new Perlin(4, 6.5, 0.5, 10);

	for (float x = 0; x < 256; x += 1) {
		for (float y = 0; y < 256; y += 1) {

			float noise = per->Get(x, y);
			//float noise = perlin_type1_2D(x, y);
			
			textureColor[RED] = finddensity(y, x, 256, 256, noise, 145);
			textureColor[GREEN] = finddensity(y, x, 256, 256, noise, 145);
			
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

int generateNoiseType2(char *framebuffer, int width, int height, char *imagePx) {
	GzColor textureColor = { 0, 0, 0 };
	int finalColor[] = { 0, 0, 0 };

	int seed = 63;
	int wide =12413;

	float  disp1, disp2, disp3, disp4, disp5, disp6;

	for (float y = 0; y < height; y += 1) {
		for (float x = 0; x < width; x += 1) {

			disp1 = PerlinNoise(x, y, wide, 2, seed, 100);
			disp2 = PerlinNoise(x, y, wide, 2, seed, 25);
			disp3 = PerlinNoise(x, y, wide, 2, seed, 12.5);
			disp4 = PerlinNoise(x, y, wide, 2, seed, 6.25);
			disp5 = PerlinNoise(x, y, wide, 2, seed, 3.125);
			disp6 = PerlinNoise(x, y, wide, 2, seed, 1.56);

			float noise = disp1 + (disp2*.25) + (disp3*.125) + (disp4*.0625) + (disp5*.03125) + (disp6*.0156);

			//float noise = disp1 + (disp2*.25) + (disp3*.125) + (disp4*.0625) + (disp5*.03125) + (disp6*.0156);
			//float noise = disp6;

			textureColor[RED] = noise;
			textureColor[GREEN] = noise;
			textureColor[BLUE] = 255;

			if (noise > 255)
				noise = 255;
			if (noise < 0)
				noise = 0;

			float alpha = (noise / 255.0f) - 0.29;
			if (alpha > 255)
				alpha = 255;
			if (alpha < 0)
				alpha = 0;

			float r = (imagePx[3 * ((int)y * width + (int)(x))] < -1) ? imagePx[3 * ((int)y * width + (int)(x))] + 256 : imagePx[3 * ((int)y * width + (int)(x))];
			float g = (imagePx[3 * ((int)y * width + (int)(x)) + 1] < -1) ? imagePx[3 * ((int)y * width + (int)(x)) + 1] + 256 : imagePx[3 * ((int)y * width + (int)(x)) + 1];
			float b = (imagePx[3 * ((int)y * width + (int)(x)) + 2] < -1) ? imagePx[3 * ((int)y * width + (int)(x)) + 2] + 256 : imagePx[3 * ((int)y * width + (int)(x)) + 2];

			finalColor[RED] = (int)((textureColor[RED])*(alpha)+(1 - alpha)*r);
			finalColor[GREEN] = (int)((textureColor[GREEN])*(alpha)+(1 - alpha)*g);
			finalColor[BLUE] = (int)((textureColor[BLUE])*(alpha)+(1 - alpha)*b);

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