#include "perlin.h"
#include "kenPerlinNoise.h"

typedef short   GzIntensity;
typedef float   GzColor[3];
#define RED     0               // array indicies for color vector
#define GREEN   1
#define BLUE    2

float finddensity3D(float x, float y, float height, float width, float noise, float radius);
int generateTexture3D(char *framebuffer, int width, int height, char *imagePx);