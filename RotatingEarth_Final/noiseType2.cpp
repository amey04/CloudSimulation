#include "noiseType2.h"

float InterPolation(float a, float b, float c) {
	return a + (b - a)*c*c*(3 - 2 * c);
}

float InterLinear(float a, float b, float c) {
	return a*(1 - c) + b*c;
}

float Noise(int x) {
	x = (x << 13) ^ x;
	return (((x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff) / 2147483648.0);
}

float PerlinNoise(float x, float y, int width, int octaves, int seed, double periode) {
	double a, b, value = 0, freq, zone_x, zone_y;
	int s, box, num, step_x, step_y;
	int amplitude = 120;
	int noisedata;

	freq = 1 / (float)(periode);

	for (s = 0; s<octaves; s++) {
		num = (int)(width*freq);
		step_x = (int)(x*freq);
		step_y = (int)(y*freq);

		zone_x = x*freq - step_x;
		zone_y = y*freq - step_y;

		box = step_x + step_y*num;
		noisedata = (box + seed);

		a = InterPolation(Noise(noisedata), Noise(noisedata + 1), zone_x);
		b = InterPolation(Noise(noisedata + num), Noise(noisedata + 1 + num), zone_x);

		value += InterPolation(a, b, zone_y)*amplitude;
	}
	return value;
}
