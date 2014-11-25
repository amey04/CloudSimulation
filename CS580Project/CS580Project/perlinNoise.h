typedef short   GzIntensity;
typedef float   GzColor[3];
#define RED     0               /* array indicies for color vector */
#define GREEN   1
#define BLUE    2


#define OUTFILE1 "cloud.ppm"

double noise(double x, double y);
short	ctoi(float color);
int ptex_fun(float u, float v, GzColor color);
int generateTexture(char *framebuffer, int width, int height);
int GzFlushCloudTexture2File(char *framebuffer, int width, int height);
int generateTexture3D(char *framebuffer, int width, int height);