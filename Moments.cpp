#include "Moments.h"
#include<stdio.h>

Moments::Moments(int* array, int width, int height)
{
    this->width = width; this->height = height;
    object = new int[width * height];
    normalizeObject(array);
}

Moments::~Moments()
{
    delete[] object;
}

float Moments::calcMoment(int p, int q)
{
    float sum = 0;
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            sum += object[x + y * width] * pow(x, p) * pow(y, q);
        }
    }
    return sum;
}

void Moments::calcOrgins()
{
    orginOfX = calcMoment(1, 0) / calcMoment(0, 0);
    orginOfY = calcMoment(0, 1) / calcMoment(0, 0);
}

float Moments::calcOrginMoment(int p, int q)
{
    float sum = 0;
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            sum += object[x + y * width] * pow((x - orginOfX), p) * pow((y - orginOfY), q);
        }
    }
    return sum;
}

float Moments::calcNormalizedMoment(int p, int q)
{
    float temp = ((p + q) / 2) + 1;
    return calcOrginMoment(p, q) / pow(calcMoment(0, 0), temp);
}

void Moments::calcInvariantMoments()
{
    invariantMoments[0] = calcNormalizedMoment(2, 0) + calcNormalizedMoment(0, 2);
    invariantMoments[1] = pow((calcNormalizedMoment(2, 0) - calcNormalizedMoment(0, 2)), 2) + 4 * (pow(calcNormalizedMoment(1, 1), 2));
    invariantMoments[2] = pow((calcNormalizedMoment(3, 0) - 3 * calcNormalizedMoment(1, 2)), 2) + pow((3 * calcNormalizedMoment(2, 1) - calcNormalizedMoment(0, 3)), 2);
    invariantMoments[3] = pow((calcNormalizedMoment(3, 0) + calcNormalizedMoment(1, 2)), 2) + pow((calcNormalizedMoment(2, 1) + calcNormalizedMoment(0, 3)), 2);
    invariantMoments[4] = (calcNormalizedMoment(3, 0) - 3 * calcNormalizedMoment(1, 2)) * (calcNormalizedMoment(3, 0) + calcNormalizedMoment(1, 2)) * (pow(calcNormalizedMoment(3, 0) + calcNormalizedMoment(1, 2), 2) - 3 * pow(calcNormalizedMoment(2, 1) + calcNormalizedMoment(0, 3), 2)) + (3 * calcNormalizedMoment(2, 1) - calcNormalizedMoment(0, 3)) * (calcNormalizedMoment(2, 1) + calcNormalizedMoment(0, 3)) * (pow(3 * (calcNormalizedMoment(3, 0) + calcNormalizedMoment(1, 2)), 2) - pow(calcNormalizedMoment(2, 1) + calcNormalizedMoment(0, 3), 2));
    invariantMoments[5] = (calcNormalizedMoment(2, 0) - calcNormalizedMoment(0, 2)) * (pow(calcNormalizedMoment(3, 0) + calcNormalizedMoment(1, 2), 2) - pow(calcNormalizedMoment(2, 1) + calcNormalizedMoment(0, 3), 2)) + (4 * calcNormalizedMoment(1, 1) * (calcNormalizedMoment(3, 0) + calcNormalizedMoment(1, 2)) * calcNormalizedMoment(2, 1) + calcNormalizedMoment(0, 3));
    invariantMoments[6] = (3 * calcNormalizedMoment(2, 1) - calcNormalizedMoment(0, 3)) * (calcNormalizedMoment(3, 0) + calcNormalizedMoment(1, 2)) * (pow(calcNormalizedMoment(3, 0) + calcNormalizedMoment(1, 2), 2) - 3 * pow(calcNormalizedMoment(2, 1) + calcNormalizedMoment(0, 3), 2)) - (calcNormalizedMoment(3, 0) - 3 * calcNormalizedMoment(1, 2)) * (calcNormalizedMoment(2, 1) + calcNormalizedMoment(0, 3)) * (3 * pow(calcNormalizedMoment(3, 0) + calcNormalizedMoment(1, 2), 2) - pow(calcNormalizedMoment(2, 1) + calcNormalizedMoment(0, 3), 2));
}

float* Moments::getInvariantMoments()
{
    return invariantMoments;
}

void Moments::getMoments()
{

    for (int i = 0; i < 7; i++)
    {
        printf("\n%i: %f\n", i,invariantMoments[i]);
    }
    printf("........");

    
}

void Moments::normalizeObject(int* arr)
{
    for (int i = 0; i < width * height; i++)
    {
        if (arr[i] > 4)
            object[i] = 1;
        else
            object[i] = 0;
    }
}