#include <stdio.h>
#include <math.h>

#include "perlinNoise.h"

//-------------------------- Type 1 -----------------------------------------
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

//-------------------------------- End of Type One ---------------------------------------

//-------------------------------- Type Two ----------------------------------------------

double interpolate(double t, double a, double b) {
	return a*(1 - t) + t *b;
}

double gradient(int hash, double x, double y) {

	int h = hash & 15;
	double u, v;
	if (h > 8){
		u = y;
	}
	else {
		u = x;
	}
	if (h < 4) {
		v = y;
	}
	else {
		v = x;
	}

	if ((h & 1) != 0)
		u = -u;
	if ((h & 2) != 0)
		v = -v;

	return u + v;
}

int p[512];

int permutation[] = { 151, 160, 137, 91, 90, 15,
131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
};

int permute() {
	for (int i = 0; i < 256; i++){
		p[256 + i] = p[i] = permutation[i];
	}
	return 0;
}

float generatenoise(float u, float v) {

	int X1 = (int)floor(u) & 255;
	int	Y1 = (int)floor(v) & 255;

	u -= floor(u);
	v -= floor(v);

	double u1 = pow(u, 3) * (u * (u * 6 - 15) + 10);
	double v1 = pow(v, 3) * (v * (v * 6 - 15) + 10);

	int A = p[X1] + Y1;
	int	AA = p[A];
	int AB = p[A + 1];

	int	B = p[X1 + 1] + Y1;
	int BA = p[B];
	int BB = p[B + 1];

	return interpolate(v1,
		interpolate(u1,
		gradient(p[AA], u, v),
		gradient(p[BA], u - 1, v)
		),
		interpolate(u1,
		gradient(p[AB], u, v - 1),
		gradient(p[BB], u - 1, v - 1)
		)
		);

}

//---------------------------- End of Type Two ------------------------------------------

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
		//noise_value += noise((u * f) / 75, v / (75 * f)) * a;
		noise_value += generatenoise((u * f) / 75, v / (75 * f)) * a;
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

int generateTexture(char *framebuffer, int width, int height) {

	GzColor textureColor = { 0, 0, 0 };
	GzIntensity finalColor[] = { 0, 0, 0 };

	char *copybuffer = framebuffer;

	for (int i = 255; i >= 0; i--){
		for (int j = 0; j < 256; j++){

			textureColor[RED] = 0;
			textureColor[GREEN] = 0;
			textureColor[BLUE] = 0;

			ptex_fun(i, j, textureColor);

			finalColor[RED] = ctoi(textureColor[RED]);
			finalColor[GREEN] = ctoi(textureColor[GREEN]);
			finalColor[BLUE] = ctoi(textureColor[BLUE]);

			*(framebuffer++) = (char)(finalColor[RED]);
			*(framebuffer++) = (char)(finalColor[GREEN]);
			*(framebuffer++) = (char)(finalColor[BLUE]);
		}
	}

	GzFlushCloudTexture2File(copybuffer, width, height);

	return 0;
}
int GzFlushCloudTexture2File(char *framebuffer, int width, int height)
{
	
	FILE *outfile;
	if ((fopen_s(&outfile, OUTFILE1, "w")) !=0)
	{
		printf("The output file was not opened\n");
		return 1;
	}

	/* write pixels to ppm file based on display class -- "P6 %d %d 255\r" */
	int i, j;
	if (outfile != NULL && framebuffer != NULL) {
		fprintf(outfile, "P6 %d %d 255\r", width, height);

		for (i = width-1; i >= 0; i--) {
			for (j = 0; j < height; j++) {
				fprintf(outfile, "%c%c%c", (*framebuffer++), (*framebuffer++), (*framebuffer++));
			}
		}

		if (fclose(outfile))
			printf("The output file was not closed\n");
		return 0;
	}
	else {
		return 1;
	}

}