typedef short   GzIntensity;
typedef float   GzColor[3];
#define RED     0               // array indicies for color vector
#define GREEN   1
#define BLUE    2

float InterLinear(float a, float b, float c);
float PerlinNoise(float x, float y, int width, int octaves, int seed, double periode);