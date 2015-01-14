#if 0
#include <stdio.h>
#include <math.h>

#include "perlinNoise.h"

int p[512];

//-------------------------------Find Density-----------------------------------------------

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



//------------------------------ Type 3 -- 3-D Perlin Noise ------------------------------

inline double cosineinterpolate(double a, double b, double x)
{
	double pi_x = x * 3.1415927;
	double f = (1.0 - cos(pi_x));
	return (a*(1.0 - f)) + (0.5 *b*f);
}

inline double findnoise3D(double x, double y, double z)
{
	int m = (int)x + (int)y * 57 + (int)z * (57 * 57);
	long temp = m << 13;
	long n = temp ^ m;
	//int n = (m << 13) ^ m;
	long temp_value = ((((15731 * (n * n)) + 789221) * n) + 1376312589);
	long temp_value2 = (temp_value) & 0x7ffffff;

	return 1.0 - ((float)temp_value2 / 1073741823.0);
}

double calc_f(double x, double y, double z) {
	
	int minX = x - 1;
	int minY = y - 1;
	int minZ = z - 1;

	//Corners of a cube
	double noiseA = findnoise3D(x - 1, y - 1, z + 1);
	double noiseB = findnoise3D(x + 1, y - 1, z + 1);
	double noiseC = findnoise3D(x + 1, y + 1, z + 1);
	double noiseD = findnoise3D(x - 1, y + 1, z + 1);
	double noiseE = findnoise3D(x - 1, y - 1, z - 1);
	double noiseF = findnoise3D(x + 1, y - 1, z - 1);
	double noiseG = findnoise3D(x + 1, y + 1, z - 1);
	double noiseH = findnoise3D(x - 1, y + 1, z - 1);

	//Sides of a Cube
	double sidenoiseA = findnoise3D(x, y, z + 1);
	double sidenoiseB = findnoise3D(x + 1, y, z);
	double sidenoiseC = findnoise3D(x, y, z - 1);
	double sidenoiseD = findnoise3D(x - 1, y, z);
	double sidenoiseE = findnoise3D(x, y + 1, z);
	double sidenoiseF = findnoise3D(x, y - 1, z);

	//Middle of Edges
	double midedgenoiseA = findnoise3D(x - 1, y, z - 1);
	double midedgenoiseB = findnoise3D(x + 1, y, z - 1);
	double midedgenoiseC = findnoise3D(x, y + 1, z - 1);
	double midedgenoiseD = findnoise3D(x, y - 1, z - 1);

	double midedgenoiseE = findnoise3D(x - 1, y, z + 1);
	double midedgenoiseF = findnoise3D(x + 1, y, z + 1);
	double midedgenoiseG = findnoise3D(x, y + 1, z + 1);
	double midedgenoiseH = findnoise3D(x, y - 1, z + 1);

	double midedgenoiseI = findnoise3D(x - 1, y - 1, z);
	double midedgenoiseJ = findnoise3D(x + 1, y - 1, z);
	double midedgenoiseK = findnoise3D(x - 1, y + 1, z);
	double midedgenoiseL = findnoise3D(x + 1, y + 1, z);

	double alpha = 9.0 / 18.0;
	double beta = 2.0 / (8.0 * 18.0);
	double gamma = 4.0 / (6.0 * 18.0);
	double delta = 3.0 / (12.0 * 18.0);

	double totalCornerNoise = noiseA + noiseB + noiseC + noiseD + noiseE + noiseF + noiseG + noiseH;
	double totalSideNoise = sidenoiseA + sidenoiseB + sidenoiseC + sidenoiseD + sidenoiseE + sidenoiseF;
	double totalMidEdgeNoise = midedgenoiseA + midedgenoiseB + midedgenoiseC + midedgenoiseD
		+ midedgenoiseE + midedgenoiseF + midedgenoiseG + midedgenoiseH
		+ midedgenoiseI + midedgenoiseJ + midedgenoiseK + midedgenoiseL;

	double f = (alpha * findnoise3D(x, y, z)) +
		(beta * totalCornerNoise) +
		(gamma * totalSideNoise) +
		(delta * totalMidEdgeNoise);
	
	return f;
}
double noise3D(double x, double y, double z) {

	double x_i = floor(x);
	double y_i = floor(y);
	double z_i = floor(z);

	double x_f = x - x_i;
	double y_f = y - y_i;
	double z_f = z - z_i;

	double XYZ = calc_f(x_i + 1, y_i + 1, z_i + 1);
	double xYZ = calc_f(x_i, y_i + 1, z_i + 1);
	double XyZ = calc_f(x_i + 1, y_i, z_i + 1);
	double xyZ = calc_f(x_i, y_i, z_i + 1);
	double XYz = calc_f(x_i + 1, y_i + 1, z_i);
	double xYz = calc_f(x_i, y_i + 1, z_i);
	double Xyz = calc_f(x_i + 1, y_i, z_i);
	double xyz = calc_f(x_i, y_i, z_i);

	double iYZ = cosineinterpolate(xYZ, XYZ, x_f);
	double iyZ = cosineinterpolate(xyZ, XyZ, x_f);
	double iYz = cosineinterpolate(xYz, XYz, x_f);
	double iyz = cosineinterpolate(xyz, Xyz, x_f);

	double iZ = cosineinterpolate(iyZ, iYZ, y_f);
	double iz = cosineinterpolate(iyz, iYz, y_f);

	double h = cosineinterpolate(iz, iZ, z_f);
	//printf("h = %f  \n", h);
	return h;
}

int getPerlinNoise(double x, double y, double z, GzColor color){

	int Z = 2;
	double perlin_noise_value = 0.0;
	double s = 3.5;
	permute1();
	for (int i = 0; i < Z; i++) {
		double omega = pow(2.0, i);
		double phi = pow(s, i);

		double noise_value_h = noise3D2((x * omega) / 35, y / (35 * omega),  (omega * z)/10);
		//double noise_value_h = noise3D2((omega*x), (omega*y), (omega *z));
		
		perlin_noise_value += (phi * noise_value_h);
		//printf("Omega = %f phi = %f perlin_noise_value = %f \n", omega, phi, perlin_noise_value);
	}

	float p_noise = abs(perlin_noise_value); 

	color[RED] = p_noise;// finddensity(x, y, 256, 256, p_noise, 60);
	color[GREEN] = color[RED];
	//color[BLUE] = p_noise;
	color[BLUE] = 1;

	if (color[RED] > 1)
		color[RED] = 1;
	if (color[GREEN] > 1)
		color[GREEN] = 1;
	if (color[BLUE] > 1)
		color[BLUE] = 1;

	return 0;
}

int generateTexture3D(char *framebuffer, int width, int height, char *image) {

	GzColor textureColor = { 0, 0, 0 };
	int finalColor[] = { 0, 0, 0 };

	char *copybuffer = framebuffer;

	for (int i = 0; i < width; i++){

	
		for (int j = 0; j < height; j++){

			finalColor[RED] = 0;
			finalColor[GREEN] = 0;
			finalColor[BLUE] = 0;

			float ci, cj, ck;
			ci = i;
			cj = j;
			ck = 1;

			for (int l = 0; l < 10; l++) {

				textureColor[RED] = 0;
				textureColor[GREEN] = 0;
				textureColor[BLUE] = 0;

				//double noise = finddensity(i, j, 256, 256, get(ci, cj), 60);
				getPerlinNoise(ci, cj, ck, textureColor);
				ci += 0.1;
				cj += 0.1;
				ck += 0.1;

				
				textureColor[RED] = noise;
				textureColor[GREEN] = noise;
				textureColor[BLUE] = 1;

				//printf("%f \n", textureColor[RED]);
				finalColor[RED] += (int)(textureColor[RED] * 128) + 128;
				finalColor[GREEN] += (int)(textureColor[GREEN] * 128) + 128;
				finalColor[BLUE] += (int)(textureColor[BLUE] * 128) + 128;
			}

			finalColor[RED] /= 10;
			finalColor[GREEN] /= 10;
			finalColor[BLUE] /= 10;

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

	GzFlushCloudTexture2File(copybuffer, width, height);

	return 0;
}

//----------------------------------------------------------------------------------------


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

	/*int	B = p[X1 + 1] + Y1;
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
		);*/
	return 0;

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
		noise_value += noise((u * f) / 35, v / (35 * f)) * a;
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

int generateTexture(char *framebuffer, int width, int height) {

	GzColor textureColor = { 0, 0, 0 };
	GzIntensity finalColor[] = { 0, 0, 0 };

	char *copybuffer = framebuffer;

	for (int i = 0; i < width; i++){
		for (int j = 0; j < height; j++){

			textureColor[RED] = 0;
			textureColor[GREEN] = 0;
			textureColor[BLUE] = 0;

			ptex_fun(i, j, textureColor);

			finalColor[RED] = (int)(textureColor[RED] * 128) + 128;
			finalColor[GREEN] = (int)(textureColor[GREEN] * 128) + 128;
			finalColor[BLUE] = (int)(textureColor[BLUE] * 128) + 128;

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

	GzFlushCloudTexture2File(copybuffer, width, height);

	return 0;
}
int GzFlushCloudTexture2File(char *framebuffer, int width, int height)
{
	char finalColor[3];
	FILE *outfile;
	if ((fopen_s(&outfile, OUTFILE1, "w")) !=0)
	{
		printf("The output file was not opened\n");
		return 1;
	}

	/* write pixels to ppm file based on display class -- "P6 %d %d 255\r" */
	int i, j, k;
	if (outfile != NULL && framebuffer != NULL) {
		fprintf(outfile, "P6 %d %d 255\r", width, height);

		for (i = 0; i < width; i++) {

			for (j = 0; j < height; j++) {

				//for (k = 0; k < width; k++) {
					finalColor[RED] = *framebuffer++;
					finalColor[GREEN] = *framebuffer++;
					finalColor[BLUE] = *framebuffer++;
					fprintf(outfile, "%c%c%c", finalColor[BLUE], finalColor[RED], finalColor[GREEN]);
				//}
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
#endif