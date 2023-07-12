#include "Clustering.h"
#include <cmath>

int* Histogram(image im)
{
	int* hist;
	if (im.c == 1)
	{
		hist = new int[256];
		for (int i = 0; i < 256; i++)
			hist[i] = 0;
		for (int i = 0; i < im.h * im.w; i++)
			hist[im.data[i]]++;
		return hist;
	}
	else hist = NULL;
}

/*BYTE* k_means(BYTE* Buffer, unsigned int Width, unsigned int Height)
{
	float t1 = 0, t2 = 255, t11 = -1, t22 = -1;
	double sum1 = 0, sum2 = 0;
	double division1 = 0, division2 = 0;

	int* histogram_array = new int[256];
	image im;
	im.w = Width;
	im.h = Height;
	im.data = Buffer;
	histogram_array = Histogram(im);

	while (true)
	{
		for (int i = 0; i < 256; i++)
		{
			if (fabs(i - t1) < fabs(i - t2))
			{
				sum1 += (i * histogram_array[i]);
				division1 += histogram_array[i];
			}
			else
			{
				sum2 += (i * histogram_array[i]);
				division2 += histogram_array[i];
			}
		}

		t11 = sum1 / division1;
		t22 = sum2 / division2;

		if (t1 == t11 && t2 == t22)
			break;

		t1 = t11;
		t2 = t22;
	}

	BYTE* k_means_buffer = new BYTE[Width * Height];
	k_means_buffer = Buffer;
	for (int i = 0; i < Width * Height; i++)
	{
		if (fabs(k_means_buffer[i] - t11) < fabs(k_means_buffer[i] - t22))
			k_means_buffer[i] = 0;
		else
			k_means_buffer[i] = 255;
	}

	return k_means_buffer;
}
*/