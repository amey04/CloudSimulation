#include <stdio.h>
#include <math.h>

#include "perlinNoise.h"


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
				printf("%d,%d \n", i, j);
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