#include "perlin.h"
#include "noiseType1.h"
#include "kenPerlinNoise.h"
#include "noiseType2.h"

typedef short   GzIntensity;
typedef float   GzColor[3];
#define RED     0               // array indicies for color vector
#define GREEN   1
#define BLUE    2

float finddensity(float x, float y, float height, float width, float noise, float radius);
int generateTexture2D(char *framebuffer, int width, int height);
int generateNoiseType2(char *framebuffer, int width, int height);