#ifndef MOMENTS_H
#define MOMENTS_H

#include <math.h>

class Moments
{
public:
    Moments(int* array, int width, int height);
    ~Moments();
    float calcMoment(int p, int q);
    void calcOrgins();
    float calcOrginMoment(int p, int q);
    float calcNormalizedMoment(int p, int q);
    void calcInvariantMoments();
    float* getInvariantMoments();
    void normalizeObject(int* arr);
    void getMoments();

private:
    int width, height;
    float orginOfX, orginOfY;
    int* object;
    float invariantMoments[7];
};

#endif // MOMENTS_H