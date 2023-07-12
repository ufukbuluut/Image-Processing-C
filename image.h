#pragma once

#include <string.h>

// image.h
typedef struct image {
	int w;
	int h;
	int c;
	unsigned char* data;
	int k;
	int k2;
} image;

image load_image(const char* filename);
image make_image(int w, int h, int c);
image make_empty_image(int w, int h, int c);
image RGBtoIntensity(image m);
image IntensityToBinary(image m);
image Dilation(image m);
image Erosion(image m);
image Labeling(image m);
float oklid_Distance(float* arr1, float* arr2, int size);
void draw(image m, int padding, int x1, int x2, int y1, int y2, int object);
image ObjectDetection(image m);
image ConvertIntensityToRgb(image m, long* newsize);
void ObjectMomentHesapla(image m);


//Homework2
image edgeImage(image m);
image nonMaxSuppression(image m);
image hystericThreshold(image m);
image binaryEdgeImage(image m);

