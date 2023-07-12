#include "image.h"
#include "Clustering.h"
#include "Moments.h"
#include <cmath>
#include <stdlib.h>
#include <malloc.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/include/stb_image.h"


image load_image(const char* filename)
{
	int w, h, c;
	int channel = 3;
	//w = width, h = height, c = # 8 - bit components per pixel ...
	unsigned char* data = stbi_load(filename, &w, &h, &c, channel);    // without OpenCV
	
	if (!data) {
		exit(EXIT_FAILURE);
	}
	
	image out;
	out.data = data;
	out.h = h;
	out.w = w;
	out.c = c;
	return out;
}//load_image

image RGBtoIntensity(image m)
{
	image raw;
	raw.data = new (std::nothrow) unsigned char[m.h * m.w];
	raw.w = m.w;
	raw.h = m.h;
	raw.c = 1;
	long bufpos = 0;
	long newpos = 0;
	for (int row = 0; row < m.h; row++)
		for (int column= 0; column < m.w; column++)
		{
			newpos = row * m.w + column;
			bufpos = row * m.w * m.c + column * m.c;
			//printf("%i,%i\n", newpos, bufpos);
			raw.data[newpos] = unsigned char(0.3 * m.data[bufpos] + 0.59 * m.data[bufpos + 1] + 0.11 * m.data[bufpos + 2]);
		}
	return raw;
	delete [] raw.data;
}

image ConvertIntensityToRgb(image m, long* newsize)
{
	// first make sure the parameters are valid


	// now we have to find with how many bytes
	// we have to pad for the next DWORD boundary	

	int padding = 0;
	int scanlinebytes = m.w * 3;
	while ((scanlinebytes + padding) % 4 != 0)     // DWORD = 4 bytes
		padding++;
	// get the padded scanline width
	int psw = scanlinebytes + padding;
	// we can already store the size of the new padded buffer
	*newsize = m.h * psw;

	// and create new buffer
	image raw; 
	raw.w = m.w;
	raw.h = m.h;
    raw.data = new unsigned char[*newsize];

	// fill the buffer with zero bytes then we dont have to add
	// extra padding zero bytes later on
	memset(raw.data, 0, *newsize);

	// now we loop trough all bytes of the original buffer, 
	// swap the R and B bytes and the scanlines
	long bufpos = 0;
	long newpos = 0;
	for (int row = 0; row < m.h; row++)
		for (int column = 0; column < m.w; column++) {
			bufpos = row * m.w + column;     // position in original buffer
			newpos = (m.h - row - 1) * psw + column * 3;           // position in padded buffer
			//newpos = (row)* psw + column * 3;
			raw.data[newpos] = m.data[bufpos];       //  blue
			raw.data[newpos + 1] = m.data[bufpos];   //  green
			raw.data[newpos + 2] = m.data[bufpos];   //  red
		}

	return raw;
}

image IntensityToBinary(image m) //Öklid ile Hesaplama Methodu
{
	/*for (int row = 0; row < m.h; row++)
		for (int column = 0; column < m.h; column++)
		{
			long i = row * m.w * m.c + column * m.c;
			printf("%i\n", m.data[i]);
		}
	printf("..................................................................................");*/
	float t1 = 0, t2 = 255, t11 = -1, t22 = -1;
	double sum1 = 0, sum2 = 0;
	double division1 = 0, division2 = 0;

	image raw;
	//printf("h:%i w:%i h*w:%i\n",m.h,m.w,m.h*m.w);
	unsigned char* data = (unsigned char*)malloc(sizeof(char) * (m.h * m.w));
	raw.data = data;
	raw.w = m.w;
	raw.h = m.h;
	raw.c =1;
	long bufpos = 0;
	long newpos = 0;
	int* histogram_array = new int[256];
	histogram_array = Histogram(m);
	

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

	for (int i = 0; i < m.w * m.h; i++)
	{
		if (fabs(m.data[i] - t11) < fabs(m.data[i] - t22))
			m.data[i] = 0;
		else
			m.data[i] = 255;
	}
	m.k = t1;
	m.k2 = t2;
	return m;
	delete[] raw.data;
	
}

image Dilation(image m)
{
	image raw;
	unsigned char* data = (unsigned char*)malloc(sizeof(char) * (m.h * m.w));
	raw.data = data;
	raw.w = m.w;
	raw.h = m.h;
	raw.c = 1;


	int D;
	bool result = 0;
	int heigth = m.h;
	int width = m.w;

	for (int i = 1; i < heigth-1; i++)
	{
		for (int j = 1; j < width-1; j++)
		{
			D = (i * width + j);
			
			result = (m.data[(D - width)] || m.data[(D - 1)] || m.data[D] || m.data[(D + 1)] || m.data[(D + width)]);
			//result = (m.data[(D - 1)] || m.data[D] || m.data[(D + 1)]);
			//result = (Buffer[(C - Width)] || Buffer[(C - 1)] || Buffer[C] || Buffer[(C + 1)] || Buffer[(C + Width)]);

			if (result == true)
				raw.data[D] = 255;
			else
				raw.data[D] = 0;
		}
		
	}
	return raw;
	delete[] raw.data;

}

image Erosion(image m)
{
	image raw;
	unsigned char* data = (unsigned char*)malloc(sizeof(char) * (m.h * m.w));
	raw.data = data;
	raw.w = m.w;
	raw.h = m.h;
	raw.c = 1;


	int D;
	bool result = 0;
	int heigth = raw.h;
	int width = raw.w;

	for (int i = 1; i < heigth-1; i++)
	{
		for (int j = 1; j < width-1; j++)
		{
			D = (i * width + j);

			result = (m.data[(D - width - 1)] && m.data[(D - width)] && m.data[D - width +1] && m.data[(D -1)] && m.data[D] && m.data[(D + 1)] && m.data[(D + width - 1)]) && m.data[(D + width)] && m.data[(D + width + 1)];
			//result = (Buffer[(C - Width - 1)] && Buffer[(C - Width)] && Buffer[(C - Width + 1)] && Buffer[(C - 1)] && Buffer[C] && Buffer[(C + 1)] && Buffer[(C + Width - 1)] && Buffer[(C + Width)] && Buffer[(C + Width + 1)]);

			if (result == true)
				raw.data[D] = 255;
			else
				raw.data[D] = 0;
		}

	}
	return raw;
	delete[] raw.data;

}

image Labeling(image m)
{
	/*for (int row = 0; row < m.h; row++)
	for (int column = 0; column < m.h; column++)
	{
		long i = row * m.w * m.c + column * m.c;
		printf("%i: %i\n", i,m.data[i]);
	}
    printf("..................................................................................");*/
	image raw;
	unsigned char* data = (unsigned char*)malloc(sizeof(char) * (m.h * m.w));
	raw.data = data;
	raw.w = m.w;
	raw.h = m.h;
	raw.c = 1;
	for (int row = 0; row < m.h; row++)
		for (int column = 0; column < m.w; column++)
		{
			long i = row * m.w * m.c + column * m.c;
			raw.data[i] = m.data[i];
		}

	int D;
	int A = 0;
	for (int i = 1; i < m.h - 1; i++)
	{
		for (int j = 1; j < m.w - 1; j++)
		{
			D = (i * m.w + j);
			int temp = 0;//siyah
			//printf("%i: %i\n", D, raw.data[D]);
			if (raw.data[D] == 0)//piksel siyah
			{
				int array[9] = { raw.data[(D - m.w - 1)] ,raw.data[(D - m.w)] , raw.data[(D - m.w + 1)] , raw.data[(D - 1)] , raw.data[D] , raw.data[(D + 1)] , raw.data[(D + m.w - 1)] , raw.data[(D + m.w)] ,raw.data[(D + m.w + 1)] };
				//printf("[%i,%i,%i,%i,%i,%i,%i,%i,%i,temp:%i]\n",array[0], array[1], array[2], array[3], array[4], array[5], array[6], array[7], array[8],temp );
				for (int i = 0; i < 9; i++)
				{
					//printf("%i: %i\n", i, array[i]);

					if (array[i] != 0 && array[i] != 255)
					{
						if (array[i] > temp)
							temp = array[i];

					}
				}

				if (temp != 0)
					raw.data[D] = temp;

				else
				{
					raw.data[D] = 250 - A;
					A += 2;
				}
			}
		}
	}
	for (int i = m.h - 1; i > 1; i--)
	{
		for (int j = m.w - 1; j > 1; j--)
		{
			D = (i * m.w + j);
			int temp = 0;

			if (raw.data[D] < 255)
			{
				int b[9] = { raw.data[(D - m.w - 1)] ,raw.data[(D - m.w)] , raw.data[(D - m.w + 1)] , raw.data[(D - 1)] , raw.data[D] , raw.data[(D + 1)] , raw.data[(D + m.w - 1)] , raw.data[(D + m.w)] ,raw.data[(D + m.w + 1)] };

				for (int i = 0; i < 9; i++)
				{

					if (b[i] != 0 && b[i] != 255)
					{
						if (b[i] > temp)
							temp = b[i];

					}
				}

				if (temp != 0)
					raw.data[D] = temp;

			}
		}
	}
	return raw;

}

float oklid_Distance(float* arr1, float* arr2, int size)
{
	float temp = 0;
	for (int i = 0; i < size; i++)
	{
		temp += pow((arr1[i] - arr2[i]), 2);
	}
	return sqrt(temp);
}

void draw(image m, int padding, int x1, int x2, int y1, int y2, int object)
{


	int r, g, b;
	y1 = m.h - y1 - 1;
	y2 = m.h - y2 - 1;
	x1 = 3 * x1;
	x2 = 3 * x2;
	y1 = 3 * y1;
	y2 = 3 * y2;
	if (object == 0) {
		r = 255; g = 0; b = 255;
	}
	else if (object == 1)
	{
		r = 0; g = 255; b = 0;
	}
	else if (object == 2)
	{
		r = 0; g = 255; b = 0;
	}
	else if (object == 3)
	{
		r = 255; g = 0; b = 0;
	}
	else if (object == 4)
	{
		r = 255; g = 150; b = 0;
	}
	else if (object == 5)
	{
		r = 0; g = 150; b = 255;
	}
	else if (object == 6)
	{
		r = 0; g = 255; b = 255;
	}
	else if (object == 7)
	{
		r = 150; g = 150; b = 150;
	}



	for (int i = x1; i <= x2; i += 3)
	{
		m.data[y1 * m.w + (y1 / 3 * padding) + i] = b;	//BMP 'den gelen paddingleri goz ard? edemeyiz.
		m.data[y1 * m.w + (y1 / 3 * padding) + i + 1] = g;
		m.data[y1 * m.w + (y1 / 3 * padding) + i + 2] = r;
		m.data[y2 * m.w + (y2 / 3 * padding) + i] = b;
		m.data[y2 * m.w + (y2 / 3 * padding) + i + 1] = g;
		m.data[y2 * m.w + (y2 / 3 * padding) + i + 2] = r;
	}

	for (int i = y2; i <= y1; i += 3)
	{
		m.data[i * m.w + i / 3 * padding + x1] = b;
		m.data[i * m.w + i / 3 * padding + x1 + 1] = g;
		m.data[i * m.w + i / 3 * padding + x1 + 2] = r;
		m.data[i * m.w + i / 3 * padding + x2] = b;
		m.data[i * m.w + i / 3 * padding + x2 + 1] = g;
		m.data[i * m.w + i / 3 * padding + x2 + 2] = r;
	}


}
image ObjectDetection(image m)
{
	int labelArray[50];
	int D;
	int value = 0;
	int index = 0;
	for (int i = 1; i < m.h - 1; i++)
	{
		for (int j = 1; j < m.w - 1; j++)
		{
			D = (i * m.w + j);

			if (m.data[D] < 255)					//goruntude geziyoruz ve 255'den buyuk pixel'e rastlarsak yani bu piksel bir objenin parçasý
			{
				for (int k = 0; k < 50; k++)
				{
					if (m.data[D] == labelArray[k])		// eger etiketlenmis nesne zaten dizide varsa deger 1'e setlenir
						value = 1;
				}
				if (value != 1)								//deger 1'den farklýysa yani bu etikete sahip nesne bu dizide yoksa diziye eklenir 
				{
					labelArray[index] = m.data[D];
					index++;
					labelArray[index] = -1;				//nesnenin sonunu belirlemek için en son etiket degerinden sonra -1 atan?yor
				}
				value = 0;									//degeri tekrar 0'a setliyerek donguye devam ederiz
			}

		}
	}

	float momentvalue[8];
	//float mercimek[7] = { 0.165087,0.00170739,0.00708642,0.000142945,2.59475e-07,-0.00802825,1.58015e-07 };
	float mercimek[7] = { 0.167595,0.000255,0.000504,0.014343,-0.000019, 0.086350,0.000000 };
	//float mercimek[7] = { 0.175317,0.002455,0.002391,0.002125,0.000001,0.020076, 0.000004 };
	float cekirdek[7] = { 0.234149,0.0277601,1.0764,0.237477,0.105781,-0.196802,-0.0101849 };
	float eriste[7] = { 1.00171,0.997429,22.9338,19.5061,761.183,19.5376,-11.6029 };
	float fasulye[7] = { 0.190439,0.0107762,0.0305301,0.00227828,3.78958e-05,0.00298993,2.54525e-05 };
	float misir[7] = { 0.164747,0.000834593,0.0931484,0.00139435,2.86284e-06,-0.0279052,5.64801e-06 };
	float nohut[7] = { 0.166182,0.00138244,0.156274,0.0031489,0.000338772,-0.0143759,-0.000127367 };
	float para[7] = { 0.175317,0.002455,0.002391,0.002125,0.000001,0.020076, 0.000004};
	float fistik[7] = { 0.181707, 0.005192, 0.002951 ,0.004917,-0.000017,0.040059,0.000007};

	long new_size;
	image tempImage = ConvertIntensityToRgb(m,&new_size);
	int padding = new_size / m.h - 3 * m.w;

	int x1, x2, y1, y2;
	int a = 0, b = m.w, c = 0, d = m.h;

	for (int k = 0; labelArray[k] != -1; k++)	//etiketlerin tutuldugu dizinin sonuna gelene kadar dongude kaliriz
	{
		for (int i = 0; i < m.h; i++)
		{
			for (int j = 0; j < m.w; j++)
			{
				D = (i * m.w + j);

				if (m.data[D] == labelArray[k])		//goruntumuzde gezeriz ve etiketlenmis pixelleri buluruz
				{
					if (j > a)							//nesnelerin uc s?n?rlar?n? belirledik
						a = j;
					if (j < b)
						b = j;

					if (i > c)
						c = i;
					if (i < d)
						d = i;

					x1 = b;								//belirledigimiz s?n?rlad? x1,x2,y1,y2 degiskenlerine atadik
					x2 = a;

					y1 = d;
					y2 = c;


				}

			}
		}
		int* arrayOf_object = new int[(x2 - x1 + 1) * (y2 - y1 + 1)];
		int h = 0;
		for (int i = y1; i <= y2; i++)
			for (int j = x1; j <= x2; j++)
			{
				arrayOf_object[h] = m.data[i * m.w + j]; //Tespit edilen nesneyi bir obje dizisi içerisine alarak tutuyoruz.
				h++;

			}
		Moments obje(arrayOf_object, (x2 - x1 + 1), (y2 - y1 + 1));
		obje.calcOrgins();
		obje.calcInvariantMoments();
		float* moments = obje.getInvariantMoments();	//nesnenin hesaplanm?s 7 momentini bir diziye att?k

		//veritabanýndaki momentler ile nesne momentleri kýyaslanýr öklid ile
		momentvalue[0] = oklid_Distance(moments, mercimek, 8);
		momentvalue[1] = oklid_Distance(moments, fistik, 8);
		momentvalue[2] = oklid_Distance(moments, nohut, 8);
		momentvalue[3] = oklid_Distance(moments, fasulye, 8);
		momentvalue[4] = oklid_Distance(moments, cekirdek, 8);
		momentvalue[5] = oklid_Distance(moments, misir, 8);
		momentvalue[6] = oklid_Distance(moments, para, 8);
		momentvalue[7] = oklid_Distance(moments, fistik, 8);
		/*for (int i = 0; i < 8; i++)
		{
			printf("%i %f\n", i,momentvalue[i]);
		}
		printf("***************************************");*/

		//Nesnenin moment yakýnlýðýna göre sýralanmasý için
		float byComparison = 10000000;
		int object_identification = 0;
		for (int i = 0; i < 8; i++)
		{
			if (momentvalue[i] < byComparison)
			{
				byComparison = momentvalue[i];
				object_identification = i;		//8 nesneden hangisine daha yakinsa ona setlenir
			}
		}
		//delete[] arrayOf_object;

		for (int i = x1; i <= x2; i++)
		{
			m.data[y1 * m.w + i] = 255;
			m.data[y2 * m.w + i] = 255;
		}

		for (int i = y1; i <= y2; i++)
		{
			m.data[i * m.w + x1] = 255;
			m.data[i * m.w + x2] = 255;
		}
		a = 0, b = m.w, c = 0, d = m.h;		//her nesne icin hesap yapabilmek icin sinirlari eski haline setleriz

		draw(tempImage, padding, x1, x2, y1, y2, object_identification);

	}
	return tempImage;
}

void ObjectMomentHesapla(image m)
{
	int labelArray[50];
	int D;
	int value = 0;
	int index = 0;
	for (int i = 1; i < m.h - 1; i++)
	{
		for (int j = 1; j < m.w - 1; j++)
		{
			D = (i * m.w + j);

			if (m.data[D] < 255)					//goruntude geziyoruz ve 255'den buyuk pixel'e rastlarsak yani bu piksel bir objenin parçasý
			{
				for (int k = 0; k < 50; k++)
				{
					if (m.data[D] == labelArray[k])		// eger etiketlenmis nesne zaten dizide varsa deger 1'e setlenir
						value = 1;
				}
				if (value != 1)								//deger 1'den farklýysa yani bu etikete sahip nesne bu dizide yoksa diziye eklenir 
				{
					labelArray[index] = m.data[D];
					index++;
					labelArray[index] = -1;				//nesnenin sonunu belirlemek için en son etiket degerinden sonra -1 atan?yor
				}
				value = 0;									//degeri tekrar 0'a setliyerek donguye devam ederiz
			}

		}
	}
	long new_size;
	image tempImage = ConvertIntensityToRgb(m, &new_size);
	int padding = new_size / m.h - 3 * m.w;

	int x1, x2, y1, y2;
	int a = 0, b = m.w, c = 0, d = m.h;

	for (int k = 0; labelArray[k] != -1; k++)	//etiketlerin tutuldugu dizinin sonuna gelene kadar dongude kaliriz
	{
		for (int i = 0; i < m.h; i++)
		{
			for (int j = 0; j < m.w; j++)
			{
				D = (i * m.w + j);

				if (m.data[D] == labelArray[k])		//goruntumuzde gezeriz ve etiketlenmis pixelleri buluruz
				{
					if (j > a)							//nesnelerin uc s?n?rlar?n? belirledik
						a = j;
					if (j < b)
						b = j;

					if (i > c)
						c = i;
					if (i < d)
						d = i;

					x1 = b;								//belirledigimiz s?n?rlad? x1,x2,y1,y2 degiskenlerine atadik
					x2 = a;

					y1 = d;
					y2 = c;


				}

			}
		}
		int* arrayOf_object = new int[(x2 - x1 + 1) * (y2 - y1 + 1)];
		int h = 0;
		for (int i = y1; i <= y2; i++)
			for (int j = x1; j <= x2; j++)
			{
				arrayOf_object[h] = m.data[i * m.w + j]; //Tespit edilen nesneyi bir obje dizisi içerisine alarak tutuyoruz.
				h++;

			}
		Moments obje(arrayOf_object, (x2 - x1 + 1), (y2 - y1 + 1));
		obje.calcOrgins();
		obje.calcInvariantMoments();
		//float* moments = obje.getInvariantMoments();
		obje.getMoments();



	}
}

image edgeImage(image m)
{
	image raw;
	unsigned char* data = (unsigned char*)malloc(sizeof(char) * (m.h * m.w));
	raw.data = data;
	raw.w = m.w;
	raw.h = m.h;
	raw.c = 1;
	//raw = RGBtoIntensity(m);
	for (int row = 0; row < m.h; row++)
		for (int column = 0; column < m.w; column++)
		{
			long i = row * m.w * m.c + column * m.c;
			raw.data[i] = m.data[i];
		}


	int C, index = 0;	//Center
	int result;
	int* vertical_derivative = new int[(m.w - 2) * (m.h - 2)];
	for (int i = 1; i < m.h - 1; i++)
	{
		for (int j = 1; j < m.w - 1; j++)
		{
			C = (i * raw.w + j);
			// 1 2 1 - 0 0 0 - -1 -2 -1  maskesini gezidiriyoruz	//kenar yönü yatayda
			result = (1 * raw.data[(C - m.w - 1)] + 2 * raw.data[(C - m.w)] + 1 * raw.data[(C - m.w + 1)] + 0 * raw.data[(C - 1)] + 0 * raw.data[C] + 0 * raw.data[(C + 1)] + (-1) * raw.data[(C + m.w - 1)] + (-2) * raw.data[(C + m.w)] + (-1) * raw.data[(C +m.w + 1)]);
			vertical_derivative[index] = result;
			index++;
		}
	}

	/*image verticalImage;
	unsigned char* data = (unsigned char*)malloc(sizeof(char) * ( (m.h-2) * (m.w-2) ));
	verticalImage.data = data;
	for (int i = 0; i < raw.h - 2; i++)
	{
		for (int j = 0; j < raw.w - 2; j++)
		{
			C = i * (raw.w - 2) + j;
			verticalImage.data[C] = round(vertical_derivative[C] / 4);
		}
	}
	verticalImage.h = raw.h - 2;
	verticalImage.w = raw.w - 2;

	return verticalImage;*/

	index = 0;
	int* horizontal_derivative = new int[(m.w - 2) * (m.h - 2)];
	for (int i = 1; i < m.h - 1; i++)
	{
		for (int j = 1; j < m.w - 1; j++)
		{
			C = (i * m.w + j);
			// 1 0 -1 -  2 0 -2 -  1 0 -1  maskesini gezidiriyoruz		//kenar yönü dikeyde
			result = (1 * raw.data[(C - m.w - 1)] + 0 * raw.data[(C - m.w)] + (-1) * raw.data[(C - m.w + 1)] + 2 * raw.data[(C - 1)] + 0 * raw.data[C] + (-2) * raw.data[(C + 1)] + 1 * raw.data[(C + m.w - 1)] + 0 * raw.data[(C + m.w)] + (-1) * raw.data[(C + m.w + 1)]);
			horizontal_derivative[index] = result;
			index++;
		}
	}

	int* edge_image = new int[(m.w - 2) * (m.h - 2)];
	int* edge_image1 = new int[(m.w - 2) * (m.h - 2)];
	int sonuc2;
	for (int i = 0; i < m.h - 2; i++)
	{
		for (int j = 0; j < m.w - 2; j++)
		{
			C = (i * (m.w - 2) + j);
			// 2 maske sonucunu topluyoruz  not: 255den daha buyuk degeler olab?l?r max 1020 olab?l?r
			result = abs(vertical_derivative[C]) + abs(horizontal_derivative[C]);
			sonuc2 = vertical_derivative[C] + horizontal_derivative[C];
			edge_image[C] = result;
			edge_image1[C] = sonuc2;
		}
	}

	image edgeImage;
	unsigned char* data1 = (unsigned char*)malloc(sizeof(char) * ((m.h - 2) * (m.w - 2)));
	edgeImage.data = data1;
	edgeImage.h = raw.h - 2;
	edgeImage.w = raw.w - 2;
	edgeImage.c = 3;

	for (int i = 0; i < raw.h - 2; i++)
	{
		for (int j = 0; j < raw.w - 2; j++)
		{
			C = i * (raw.w - 2) + j;
			edgeImage.data[C] = round(edge_image[C] / 4);
		}
	}
	/*printf("start here");
	for (int i = 0; i < raw.h - 2; i++)
	{
		for (int j = 0; j < raw.w - 2; j++)
		{
			C = i * (raw.w - 2) + j;
			printf("%i: %i\n", C,edgeImage.data[C]);
		}
	}*/
	long new_size;
	ConvertIntensityToRgb(edgeImage, &new_size);

	return edgeImage;

}

image nonMaxSuppression(image m)
{
	image raw;
	unsigned char* data = (unsigned char*)malloc(sizeof(char) * (m.h * m.w));
	raw.data = data;
	raw.w = m.w;
	raw.h = m.h;
	raw.c = 1;
	//raw = RGBtoIntensity(m);
	for (int row = 0; row < m.h; row++)
		for (int column = 0; column < m.w; column++)
		{
			long i = row * m.w * m.c + column * m.c;
			raw.data[i] = m.data[i];
		}


	int C, index = 0;	//Center
	int result;
	int* vertical_derivative = new int[(m.w - 2) * (m.h - 2)];
	for (int i = 1; i < m.h - 1; i++)
	{
		for (int j = 1; j < m.w - 1; j++)
		{
			C = (i * raw.w + j);
			// 1 2 1 - 0 0 0 - -1 -2 -1  maskesini gezidiriyoruz	//kenar yönü yatayda
			result = (1 * raw.data[(C - m.w - 1)] + 2 * raw.data[(C - m.w)] + 1 * raw.data[(C - m.w + 1)] + 0 * raw.data[(C - 1)] + 0 * raw.data[C] + 0 * raw.data[(C + 1)] + (-1) * raw.data[(C + m.w - 1)] + (-2) * raw.data[(C + m.w)] + (-1) * raw.data[(C + m.w + 1)]);
			vertical_derivative[index] = result;
			index++;
		}
	}


	index = 0;
	int* horizontal_derivative = new int[(m.w - 2) * (m.h - 2)];
	for (int i = 1; i < m.h - 1; i++)
	{
		for (int j = 1; j < m.w - 1; j++)
		{
			C = (i * m.w + j);
			// 1 0 -1 -  2 0 -2 -  1 0 -1  maskesini gezidiriyoruz		//kenar yönü dikeyde
			result = (1 * raw.data[(C - m.w - 1)] + 0 * raw.data[(C - m.w)] + (-1) * raw.data[(C - m.w + 1)] + 2 * raw.data[(C - 1)] + 0 * raw.data[C] + (-2) * raw.data[(C + 1)] + 1 * raw.data[(C + m.w - 1)] + 0 * raw.data[(C + m.w)] + (-1) * raw.data[(C + m.w + 1)]);
			horizontal_derivative[index] = result;
			index++;
		}
	}

	int* edge_image = new int[(m.w - 2) * (m.h - 2)];
	int* edge_image1 = new int[(m.w - 2) * (m.h - 2)];
	int sonuc2;
	for (int i = 0; i < m.h - 2; i++)
	{
		for (int j = 0; j < m.w - 2; j++)
		{
			C = (i * (m.w - 2) + j);
			// 2 maske sonucunu topluyoruz  not: 255den daha buyuk degeler olab?l?r max 1020 olab?l?r
			result = abs(vertical_derivative[C]) + abs(horizontal_derivative[C]);
			sonuc2 = vertical_derivative[C] + horizontal_derivative[C];
			edge_image[C] = result;
			edge_image1[C] = sonuc2;
		}
	}

	image edgeImage;
	unsigned char* data1 = (unsigned char*)malloc(sizeof(char) * ((m.h - 2) * (m.w - 2)));
	edgeImage.data = data1;
	edgeImage.h = raw.h - 2;
	edgeImage.w = raw.w - 2;
	edgeImage.c = 3;

	for (int i = 0; i < raw.h - 2; i++)
	{
		for (int j = 0; j < raw.w - 2; j++)
		{
			C = i * (raw.w - 2) + j;
			edgeImage.data[C] = round(edge_image[C] / 4);
		}
	}
	//long new_size;
	//ConvertIntensityToRgb(edgeImage, &new_size);
	float Q; //Angle

	for (int i = 0; i < m.h - 2; i++)
	{
		for (int j = 0; j < m.w - 2; j++)
		{
			C = (i * (m.w - 2) + j);

			Q = atan2(vertical_derivative[C], horizontal_derivative[C]) * 180 / System::Math::PI;		//q=tan^-1((dI/dy)/(dI/dx))		gradient drection

			// buldugumuz aciya gore buyukluk k?yaslamas? yapacag?z ve eger buyukse aynen kalacak degilse 0 atanacak boylece non-maximum suppresion matrisimizi elde edecegiz.
			if ((0 <= Q && Q < 22.5) || (337.5 <= Q && Q <= 360) || (157.5 <= Q && Q < 202.5) || (0 > Q && Q > -22.5) || (-157.5 >= Q && Q > -202.5) || (-337.5 >= Q && Q >= -360))	// ac? bu araliktaysa yatayda
			{
				if (j == 0)
				{
					if (edge_image[C] > edge_image[C + 1])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}
				else if (j == (m.w - 2) - 1)
				{
					if (edge_image[C] > edge_image[C - 1])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}
				else
				{
					if (edge_image[C] > edge_image[C - 1] && edge_image[C] > edge_image[C + 1])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}

			}

			else if ((22.5 <= Q && Q < 67.5) || (202.5 <= Q && Q < 247.5) || (-112.5 >= Q && Q > -157.5) || (-292.5 >= Q && Q > -337.5))		// ac? bu araliktaysa caprazda(45 derece)
			{

				if ((j == 0 && i == 0) || (i == (m.h - 2 - 1) && j == (m.w - 2) - 1))
				{
					edge_image[C] = edge_image[C];
				}

				else if ((i == 0) || (j == (m.w - 2) - 1))
				{
					if (edge_image[C] > edge_image[C + (m.w - 2) - 1])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}

				else if ((j == 0) || (i == (m.h - 2) - 1))
				{
					if (edge_image[C] > edge_image[(C - (m.w - 2) + 1)])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}
				else
				{
					if (edge_image[C] > edge_image[(C - (m.w - 2) + 1)] && edge_image[C] > edge_image[C + (m.w - 2) - 1])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}

			}

			else if ((67.5 <= Q && Q < 112.5) || (247.5 <= Q && Q < 292.5) || (-67.5 >= Q && Q > -112.5) || (-247.5 >= Q && Q > -292.5))	// ac? bu aralikta ise dikeyde (90 derece)
			{
				if (i == 0)
				{
					if (edge_image[C] > edge_image[C + (m.w - 2)])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}
				else if (i == (m.h - 2 - 1))
				{
					if (edge_image[C] > edge_image[(C - (m.w - 2))])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}
				else
				{
					if (edge_image[C] > edge_image[C - (m.w - 2)] && edge_image[C] > edge_image[C + (m.w - 2)])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}
			}

			else if ((112.5 <= Q && Q < 157.5) || (292.5 <= Q && Q < 337.5) || (-22.5 >= Q && Q > -67.5) || (-202.5 >= Q && Q > -247.5))	// ac? bu aralikta ise caprazda (135 derece)
			{
				if ((j == 0 && i == (m.h - 2) - 1) || (i == 0 && j == (m.w - 2) - 1))
				{
					edge_image[C] = edge_image[C];
				}

				else if (j == 0 || i == 0)
				{
					if (edge_image[C] > edge_image[C + (m.w - 2) + 1])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}

				else if (i == (m.h - 2 - 1) || j == (m.w - 2 - 1))
				{
					if (edge_image[C] > edge_image[(C - (m.w - 2) - 1)])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}

				else
				{
					if (edge_image[C] > edge_image[(C - (m.w - 2) - 1)] && edge_image[C] > edge_image[C + (m.w - 2) + 1])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}

			}

		}
	}
	image nonMaxImage;
	unsigned char* data2 = (unsigned char*)malloc(sizeof(char) * ((m.h - 2) * (m.w - 2)));
	nonMaxImage.data = data2;
	nonMaxImage.h = raw.h - 2;
	nonMaxImage.w = raw.w - 2;
	nonMaxImage.c = 1;

	for (int i = 0; i < raw.h - 2; i++)
	{
		for (int j = 0; j < raw.w - 2; j++)
		{
			C = i * (raw.w - 2) + j;
			nonMaxImage.data[C] = round(edge_image[C] / 4);
		}
	}
	/*printf("\n nonmaxImage start here");
	for (int i = 0; i < raw.h - 2; i++)
	{
		for (int j = 0; j < raw.w - 2; j++)
		{
			C = i * (raw.w - 2) + j;
			printf("%i: %i\n", C, nonMaxImage.data[C]);
		}
	}*/
	long new_size;
	ConvertIntensityToRgb(nonMaxImage, &new_size);

	return nonMaxImage;
}

image hystericThreshold(image m)
{
	image raw;
	unsigned char* data = (unsigned char*)malloc(sizeof(char) * (m.h * m.w));
	raw.data = data;
	raw.w = m.w;
	raw.h = m.h;
	raw.c = 1;
	//raw = RGBtoIntensity(m);
	for (int row = 0; row < m.h; row++)
		for (int column = 0; column < m.w; column++)
		{
			long i = row * m.w * m.c + column * m.c;
			raw.data[i] = m.data[i];
		}


	int C, index = 0;	//Center
	int result;
	int* vertical_derivative = new int[(m.w - 2) * (m.h - 2)];
	for (int i = 1; i < m.h - 1; i++)
	{
		for (int j = 1; j < m.w - 1; j++)
		{
			C = (i * raw.w + j);
			// 1 2 1 - 0 0 0 - -1 -2 -1  maskesini gezidiriyoruz	//kenar yönü yatayda
			result = (1 * raw.data[(C - m.w - 1)] + 2 * raw.data[(C - m.w)] + 1 * raw.data[(C - m.w + 1)] + 0 * raw.data[(C - 1)] + 0 * raw.data[C] + 0 * raw.data[(C + 1)] + (-1) * raw.data[(C + m.w - 1)] + (-2) * raw.data[(C + m.w)] + (-1) * raw.data[(C + m.w + 1)]);
			vertical_derivative[index] = result;
			index++;
		}
	}


	index = 0;
	int* horizontal_derivative = new int[(m.w - 2) * (m.h - 2)];
	for (int i = 1; i < m.h - 1; i++)
	{
		for (int j = 1; j < m.w - 1; j++)
		{
			C = (i * m.w + j);
			// 1 0 -1 -  2 0 -2 -  1 0 -1  maskesini gezidiriyoruz		//kenar yönü dikeyde
			result = (1 * raw.data[(C - m.w - 1)] + 0 * raw.data[(C - m.w)] + (-1) * raw.data[(C - m.w + 1)] + 2 * raw.data[(C - 1)] + 0 * raw.data[C] + (-2) * raw.data[(C + 1)] + 1 * raw.data[(C + m.w - 1)] + 0 * raw.data[(C + m.w)] + (-1) * raw.data[(C + m.w + 1)]);
			horizontal_derivative[index] = result;
			index++;
		}
	}

	int* edge_image = new int[(m.w - 2) * (m.h - 2)];
	int* edge_image1 = new int[(m.w - 2) * (m.h - 2)];
	int sonuc2;
	for (int i = 0; i < m.h - 2; i++)
	{
		for (int j = 0; j < m.w - 2; j++)
		{
			C = (i * (m.w - 2) + j);
			// 2 maske sonucunu topluyoruz  not: 255den daha buyuk degeler olab?l?r max 1020 olab?l?r
			result = abs(vertical_derivative[C]) + abs(horizontal_derivative[C]);
			sonuc2 = vertical_derivative[C] + horizontal_derivative[C];
			edge_image[C] = result;
			edge_image1[C] = sonuc2;
		}
	}

	image edgeImage;
	unsigned char* data1 = (unsigned char*)malloc(sizeof(char) * ((m.h - 2) * (m.w - 2)));
	edgeImage.data = data1;
	edgeImage.h = raw.h - 2;
	edgeImage.w = raw.w - 2;
	edgeImage.c = 3;

	for (int i = 0; i < raw.h - 2; i++)
	{
		for (int j = 0; j < raw.w - 2; j++)
		{
			C = i * (raw.w - 2) + j;
			edgeImage.data[C] = round(edge_image[C] / 4);
		}
	}
	//long new_size;
	//ConvertIntensityToRgb(edgeImage, &new_size);
	float Q; //Angle

	for (int i = 0; i < m.h - 2; i++)
	{
		for (int j = 0; j < m.w - 2; j++)
		{
			C = (i * (m.w - 2) + j);

			Q = atan2(vertical_derivative[C], horizontal_derivative[C]) * 180 / System::Math::PI;		//q=tan^-1((dI/dy)/(dI/dx))		gradient drection

			// buldugumuz aciya gore buyukluk k?yaslamas? yapacag?z ve eger buyukse aynen kalacak degilse 0 atanacak boylece non-maximum suppresion matrisimizi elde edecegiz.
			if ((0 <= Q && Q < 22.5) || (337.5 <= Q && Q <= 360) || (157.5 <= Q && Q < 202.5) || (0 > Q && Q > -22.5) || (-157.5 >= Q && Q > -202.5) || (-337.5 >= Q && Q >= -360))	// ac? bu araliktaysa yatayda
			{
				if (j == 0)
				{
					if (edge_image[C] > edge_image[C + 1])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}
				else if (j == (m.w - 2) - 1)
				{
					if (edge_image[C] > edge_image[C - 1])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}
				else
				{
					if (edge_image[C] > edge_image[C - 1] && edge_image[C] > edge_image[C + 1])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}

			}

			else if ((22.5 <= Q && Q < 67.5) || (202.5 <= Q && Q < 247.5) || (-112.5 >= Q && Q > -157.5) || (-292.5 >= Q && Q > -337.5))		// ac? bu araliktaysa caprazda(45 derece)
			{

				if ((j == 0 && i == 0) || (i == (m.h - 2 - 1) && j == (m.w - 2) - 1))
				{
					edge_image[C] = edge_image[C];
				}

				else if ((i == 0) || (j == (m.w - 2) - 1))
				{
					if (edge_image[C] > edge_image[C + (m.w - 2) - 1])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}

				else if ((j == 0) || (i == (m.h - 2) - 1))
				{
					if (edge_image[C] > edge_image[(C - (m.w - 2) + 1)])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}
				else
				{
					if (edge_image[C] > edge_image[(C - (m.w - 2) + 1)] && edge_image[C] > edge_image[C + (m.w - 2) - 1])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}

			}

			else if ((67.5 <= Q && Q < 112.5) || (247.5 <= Q && Q < 292.5) || (-67.5 >= Q && Q > -112.5) || (-247.5 >= Q && Q > -292.5))	// ac? bu aralikta ise dikeyde (90 derece)
			{
				if (i == 0)
				{
					if (edge_image[C] > edge_image[C + (m.w - 2)])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}
				else if (i == (m.h - 2 - 1))
				{
					if (edge_image[C] > edge_image[(C - (m.w - 2))])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}
				else
				{
					if (edge_image[C] > edge_image[C - (m.w - 2)] && edge_image[C] > edge_image[C + (m.w - 2)])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}
			}

			else if ((112.5 <= Q && Q < 157.5) || (292.5 <= Q && Q < 337.5) || (-22.5 >= Q && Q > -67.5) || (-202.5 >= Q && Q > -247.5))	// ac? bu aralikta ise caprazda (135 derece)
			{
				if ((j == 0 && i == (m.h - 2) - 1) || (i == 0 && j == (m.w - 2) - 1))
				{
					edge_image[C] = edge_image[C];
				}

				else if (j == 0 || i == 0)
				{
					if (edge_image[C] > edge_image[C + (m.w - 2) + 1])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}

				else if (i == (m.h - 2 - 1) || j == (m.w - 2 - 1))
				{
					if (edge_image[C] > edge_image[(C - (m.w - 2) - 1)])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}

				else
				{
					if (edge_image[C] > edge_image[(C - (m.w - 2) - 1)] && edge_image[C] > edge_image[C + (m.w - 2) + 1])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}

			}

		}
	}

	int* nonmaximum_suppression = new int[(m.w - 2) * (m.h - 2)];
	nonmaximum_suppression = edge_image;

	//goruntudeki en buyuk degeri bulduk
	int max_pixel = 0;
	for (int i = 0; i < m.h - 2; i++)
		for (int j = 0; j < m.w - 2; j++)
		{
			C = i * (m.w- 2) + j;
			if (nonmaximum_suppression[C] > max_pixel)
				max_pixel = nonmaximum_suppression[C];

		}

	//Tlow ve Th degerlerimizi belirledik
	for (int i = 0; i < m.h - 2; i++)
	{
		for (int j = 0; j < m.w - 2; j++)
		{
			C = i * (m.w - 2) + j;
			if (nonmaximum_suppression[C] <= 40)
			{
				nonmaximum_suppression[C] = 0;
			}
			else if (nonmaximum_suppression[C] >= max_pixel - 50)
			{
				nonmaximum_suppression[C] = 1;
			}

		}
	}
	image hystericThresholdImage;
	unsigned char* data3 = (unsigned char*)malloc(sizeof(char) * ((m.h - 2) * (m.w - 2)));
	hystericThresholdImage.data = data3;
	hystericThresholdImage.h = raw.h - 2;
	hystericThresholdImage.w = raw.w - 2;
	hystericThresholdImage.c = 1;

	for (int i = 0; i < raw.h - 2; i++)
	{
		for (int j = 0; j < raw.w - 2; j++)
		{
			C = i * (raw.w - 2) + j;
			hystericThresholdImage.data[C] = round(edge_image[C] / 4);
		}
	}
	/*printf("\n hysteric start here");
for (int i = 0; i < raw.h - 2; i++)
{
	for (int j = 0; j < raw.w - 2; j++)
	{
		C = i * (raw.w - 2) + j;
		printf("%i: %i\n", C, hystericThresholdImage.data[C]);
	}
}*/
	long new_size;
	ConvertIntensityToRgb(hystericThresholdImage, &new_size);

	return hystericThresholdImage;
}

image binaryEdgeImage(image m)
{
	image raw;
	unsigned char* data = (unsigned char*)malloc(sizeof(char) * (m.h * m.w));
	raw.data = data;
	raw.w = m.w;
	raw.h = m.h;
	raw.c = 1;
	//raw = RGBtoIntensity(m);
	for (int row = 0; row < m.h; row++)
		for (int column = 0; column < m.w; column++)
		{
			long i = row * m.w * m.c + column * m.c;
			raw.data[i] = m.data[i];
		}


	int C, index = 0;	//Center
	int result;
	int* vertical_derivative = new int[(m.w - 2) * (m.h - 2)];
	for (int i = 1; i < m.h - 1; i++)
	{
		for (int j = 1; j < m.w - 1; j++)
		{
			C = (i * raw.w + j);
			// 1 2 1 - 0 0 0 - -1 -2 -1  maskesini gezidiriyoruz	//kenar yönü yatayda
			result = (1 * raw.data[(C - m.w - 1)] + 2 * raw.data[(C - m.w)] + 1 * raw.data[(C - m.w + 1)] + 0 * raw.data[(C - 1)] + 0 * raw.data[C] + 0 * raw.data[(C + 1)] + (-1) * raw.data[(C + m.w - 1)] + (-2) * raw.data[(C + m.w)] + (-1) * raw.data[(C + m.w + 1)]);
			vertical_derivative[index] = result;
			index++;
		}
	}


	index = 0;
	int* horizontal_derivative = new int[(m.w - 2) * (m.h - 2)];
	for (int i = 1; i < m.h - 1; i++)
	{
		for (int j = 1; j < m.w - 1; j++)
		{
			C = (i * m.w + j);
			// 1 0 -1 -  2 0 -2 -  1 0 -1  maskesini gezidiriyoruz		//kenar yönü dikeyde
			result = (1 * raw.data[(C - m.w - 1)] + 0 * raw.data[(C - m.w)] + (-1) * raw.data[(C - m.w + 1)] + 2 * raw.data[(C - 1)] + 0 * raw.data[C] + (-2) * raw.data[(C + 1)] + 1 * raw.data[(C + m.w - 1)] + 0 * raw.data[(C + m.w)] + (-1) * raw.data[(C + m.w + 1)]);
			horizontal_derivative[index] = result;
			index++;
		}
	}

	int* edge_image = new int[(m.w - 2) * (m.h - 2)];
	int* edge_image1 = new int[(m.w - 2) * (m.h - 2)];
	int sonuc2;
	for (int i = 0; i < m.h - 2; i++)
	{
		for (int j = 0; j < m.w - 2; j++)
		{
			C = (i * (m.w - 2) + j);
			// 2 maske sonucunu topluyoruz  not: 255den daha buyuk degeler olab?l?r max 1020 olab?l?r
			result = abs(vertical_derivative[C]) + abs(horizontal_derivative[C]);
			sonuc2 = vertical_derivative[C] + horizontal_derivative[C];
			edge_image[C] = result;
			edge_image1[C] = sonuc2;
		}
	}

	image edgeImage;
	unsigned char* data1 = (unsigned char*)malloc(sizeof(char) * ((m.h - 2) * (m.w - 2)));
	edgeImage.data = data1;
	edgeImage.h = raw.h - 2;
	edgeImage.w = raw.w - 2;
	edgeImage.c = 3;

	for (int i = 0; i < raw.h - 2; i++)
	{
		for (int j = 0; j < raw.w - 2; j++)
		{
			C = i * (raw.w - 2) + j;
			edgeImage.data[C] = round(edge_image1[C] / 4);
		}
	}
	//long new_size;
	//ConvertIntensityToRgb(edgeImage, &new_size);
	float Q; //Angle

	for (int i = 0; i < m.h - 2; i++)
	{
		for (int j = 0; j < m.w - 2; j++)
		{
			C = (i * (m.w - 2) + j);

			Q = atan2(vertical_derivative[C], horizontal_derivative[C]) * 180 / System::Math::PI;		//q=tan^-1((dI/dy)/(dI/dx))		gradient drection

			// buldugumuz aciya gore buyukluk k?yaslamas? yapacag?z ve eger buyukse aynen kalacak degilse 0 atanacak boylece non-maximum suppresion matrisimizi elde edecegiz.
			if ((0 <= Q && Q < 22.5) || (337.5 <= Q && Q <= 360) || (157.5 <= Q && Q < 202.5) || (0 > Q && Q > -22.5) || (-157.5 >= Q && Q > -202.5) || (-337.5 >= Q && Q >= -360))	// ac? bu araliktaysa yatayda
			{
				if (j == 0)
				{
					if (edge_image[C] > edge_image[C + 1])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}
				else if (j == (m.w - 2) - 1)
				{
					if (edge_image[C] > edge_image[C - 1])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}
				else
				{
					if (edge_image[C] > edge_image[C - 1] && edge_image[C] > edge_image[C + 1])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}

			}

			else if ((22.5 <= Q && Q < 67.5) || (202.5 <= Q && Q < 247.5) || (-112.5 >= Q && Q > -157.5) || (-292.5 >= Q && Q > -337.5))		// ac? bu araliktaysa caprazda(45 derece)
			{

				if ((j == 0 && i == 0) || (i == (m.h - 2 - 1) && j == (m.w - 2) - 1))
				{
					edge_image[C] = edge_image[C];
				}

				else if ((i == 0) || (j == (m.w - 2) - 1))
				{
					if (edge_image[C] > edge_image[C + (m.w - 2) - 1])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}

				else if ((j == 0) || (i == (m.h - 2) - 1))
				{
					if (edge_image[C] > edge_image[(C - (m.w - 2) + 1)])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}
				else
				{
					if (edge_image[C] > edge_image[(C - (m.w - 2) + 1)] && edge_image[C] > edge_image[C + (m.w - 2) - 1])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}

			}

			else if ((67.5 <= Q && Q < 112.5) || (247.5 <= Q && Q < 292.5) || (-67.5 >= Q && Q > -112.5) || (-247.5 >= Q && Q > -292.5))	// ac? bu aralikta ise dikeyde (90 derece)
			{
				if (i == 0)
				{
					if (edge_image[C] > edge_image[C + (m.w - 2)])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}
				else if (i == (m.h - 2 - 1))
				{
					if (edge_image[C] > edge_image[(C - (m.w - 2))])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}
				else
				{
					if (edge_image[C] > edge_image[C - (m.w - 2)] && edge_image[C] > edge_image[C + (m.w - 2)])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}
			}

			else if ((112.5 <= Q && Q < 157.5) || (292.5 <= Q && Q < 337.5) || (-22.5 >= Q && Q > -67.5) || (-202.5 >= Q && Q > -247.5))	// ac? bu aralikta ise caprazda (135 derece)
			{
				if ((j == 0 && i == (m.h - 2) - 1) || (i == 0 && j == (m.w - 2) - 1))
				{
					edge_image[C] = edge_image[C];
				}

				else if (j == 0 || i == 0)
				{
					if (edge_image[C] > edge_image[C + (m.w - 2) + 1])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}

				else if (i == (m.h - 2 - 1) || j == (m.w - 2 - 1))
				{
					if (edge_image[C] > edge_image[(C - (m.w - 2) - 1)])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}

				else
				{
					if (edge_image[C] > edge_image[(C - (m.w - 2) - 1)] && edge_image[C] > edge_image[C + (m.w - 2) + 1])
						edge_image[C] = edge_image[C];
					else
						edge_image[C] = 0;
				}

			}

		}
	}

	int* nonmaximum_suppression = new int[(m.w - 2) * (m.h - 2)];
	nonmaximum_suppression = edge_image;

	//goruntudeki en buyuk degeri bulduk
	int max_pixel = 0;
	for (int i = 0; i < m.h - 2; i++)
		for (int j = 0; j < m.w - 2; j++)
		{
			C = i * (m.w - 2) + j;
			if (nonmaximum_suppression[C] > max_pixel)
				max_pixel = nonmaximum_suppression[C];

		}

	//Tlow ve Th degerlerimizi belirledik
	for (int i = 0; i < m.h - 2; i++)
	{
		for (int j = 0; j < m.w - 2; j++)
		{
			C = i * (m.w - 2) + j;
			if (nonmaximum_suppression[C] <= 40)
			{
				nonmaximum_suppression[C] = 0;
			}
			else if (nonmaximum_suppression[C] >= max_pixel - 50)
			{
				nonmaximum_suppression[C] = 1;
			}

		}
	}

	for (int i = 0; i < m.h - 2; i++)
	{
		for (int j = 0; j < m.w- 2; j++)
		{
			C = i * (m.w - 2) + j;
			if (nonmaximum_suppression[C] != 0 && nonmaximum_suppression[C] != 1)
			{

				Q = atan2(vertical_derivative[C], horizontal_derivative[C]) * 180 / System::Math::PI;		//q=tan^-1((dI/dy)/(dI/dx))		gradient drection


			   //buldugumuz ac?n?n bu sefer tersine gore yani kenar dogrultusu boyunca komsular?na bakicaz eger 1 ise 1, 0 ise 0 vericez ikisi varsa 0'? tercih edecegiz.
				if ((0 <= Q && Q < 22.5) || (337.5 <= Q && Q <= 360) || (157.5 <= Q && Q < 202.5) || (0 > Q && Q > -22.5) || (-157.5 >= Q && Q > -202.5) || (-337.5 >= Q && Q >= -360))	// ac? bu araliktaysa dikeyde kenar dogrultusunda komsuluklar?na bak?l?r
				{
					if (i == 0)
					{
						if (nonmaximum_suppression[C + (m.w - 2)] == 1)
							nonmaximum_suppression[C] = 1;
						else if (nonmaximum_suppression[C + (m.w - 2)] == 0)
							nonmaximum_suppression[C] = 0;


					}
					else if (i == (m.h - 2) - 1)
					{
						if (nonmaximum_suppression[(C - (m.w- 2))] == 1)
							nonmaximum_suppression[C] = 1;
						else if (nonmaximum_suppression[(C - (m.w- 2))] == 0)
							nonmaximum_suppression[C] = 0;
					}
					else
					{
						if ((nonmaximum_suppression[(C - (m.w - 2))] == 1 && nonmaximum_suppression[C + (m.w - 2)] == 1))
							nonmaximum_suppression[C] = 1;

						else if ((nonmaximum_suppression[(C - (m.w - 2))] == 0 && nonmaximum_suppression[C + (m.w - 2)] == 0) || (nonmaximum_suppression[(C - (m.w- 2))] == 0 && nonmaximum_suppression[C + (m.w - 2)] == 1) || (nonmaximum_suppression[(C - (m.w - 2))] == 1 && nonmaximum_suppression[C + (m.w - 2)] == 0))
							nonmaximum_suppression[C] = 0;
					}

				}

				else if ((22.5 <= Q && Q < 67.5) || (202.5 <= Q && Q < 247.5) || (-112.5 >= Q && Q > -157.5) || (-292.5 >= Q && Q > -337.5))		// ac? bu araliktaysa caprazda(135 derece) kenar dogrultusunda komsuluklara bak?l?r
				{

					if ((j == 0 && i == (m.h - 2) - 1) || (i == 0 && j == (m.w - 2) - 1))
					{
						nonmaximum_suppression[C] = 0;
					}

					else if (j == 0 || i == 0)
					{
						if (nonmaximum_suppression[C + (m.w - 2) + 1] == 1)
							nonmaximum_suppression[C] = 1;
						else if (nonmaximum_suppression[C + (m.w- 2) + 1] == 0)
							nonmaximum_suppression[C] = 0;
					}

					else if (i == (m.h - 2 - 1) || j == (m.w - 2 - 1))
					{
						if (nonmaximum_suppression[C - (m.w - 2) - 1] == 1)
							nonmaximum_suppression[C] = 1;
						else if (nonmaximum_suppression[C - (m.w - 2) - 1] == 0)
							nonmaximum_suppression[C] = 0;
					}

					else
					{
						if ((nonmaximum_suppression[(C - (m.w - 2) - 1)] == 1 && nonmaximum_suppression[C + (m.w - 2) + 1] == 1))
							nonmaximum_suppression[C] = 1;

						else if ((nonmaximum_suppression[(C - (m.w - 2) - 1)] == 0 && nonmaximum_suppression[C + (m.w - 2) + 1] == 0) || (nonmaximum_suppression[(C - (m.w - 2) - 1)] == 0 && nonmaximum_suppression[C + (m.w- 2) + 1] == 1) || (nonmaximum_suppression[(C - (m.w - 2) - 1)] == 1 && nonmaximum_suppression[C + (m.w - 2) + 1] == 0))
							nonmaximum_suppression[C] = 0;
					}

				}

				else if ((67.5 <= Q && Q < 112.5) || (247.5 <= Q && Q < 292.5) || (-67.5 >= Q && Q > -112.5) || (-247.5 >= Q && Q > -292.5))	// ac? bu aralikta ise yatayda kenar dogrultusunda komsulklara bak?l?r (0 derece)
				{

					if (j == 0)
					{
						if (nonmaximum_suppression[C + 1] == 1)
							nonmaximum_suppression[C] = 1;
						else if (nonmaximum_suppression[C + 1] == 0)
							nonmaximum_suppression[C] = 0;
					}
					else if (j == (m.w - 2) - 1)
					{
						if (nonmaximum_suppression[C - 1] == 1)
							nonmaximum_suppression[C] = nonmaximum_suppression[C - 1];
						else if (nonmaximum_suppression[C - 1] == 0)
							nonmaximum_suppression[C] = 0;
					}
					else
					{
						if ((nonmaximum_suppression[C - 1] == 1 && nonmaximum_suppression[C + 1] == 1))
							nonmaximum_suppression[C] = 1;

						else if ((nonmaximum_suppression[C - 1] == 0 && nonmaximum_suppression[C + 1] == 0) || (nonmaximum_suppression[C - 1] == 0 && nonmaximum_suppression[C + 1] == 1) || (nonmaximum_suppression[C - 1] == 1 && nonmaximum_suppression[C + 1] == 0))
							nonmaximum_suppression[C] = 0;
					}

				}

				else if ((112.5 <= Q && Q < 157.5) || (292.5 <= Q && Q < 337.5) || (-22.5 >= Q && Q > -67.5) || (-202.5 >= Q && Q > -247.5))	// ac? bu aralikta ise caprazda (45 derece) kenar dogrultusunda komsuluklara bak?l?r
				{

					if ((j == 0 && i == 0) || (i == (m.h - 2 - 1) && j == (m.w - 2 - 1)))
					{
						nonmaximum_suppression[C] = 0;
					}

					else if ((i == 0) || (j == (m.w- 2) - 1))
					{
						if (nonmaximum_suppression[C + (m.w - 2) - 1] == 1)
							nonmaximum_suppression[C] = 1;
						else if (nonmaximum_suppression[C + (m.w - 2) - 1] == 0)
							nonmaximum_suppression[C] == 0;

					}

					else if ((j == 0) || (i == (m.h - 2) - 1))
					{
						if (nonmaximum_suppression[C + (m.w - 2) + 1] == 1)
							nonmaximum_suppression[C] = 1;
						else if (nonmaximum_suppression[C + (m.w- 2) + 1] == 0)
							nonmaximum_suppression[C] == 0;
					}
					else
					{
						if ((nonmaximum_suppression[(C - (m.w - 2) + 1)] == 1 && nonmaximum_suppression[C + (m.w - 2) - 1] == 1))
							nonmaximum_suppression[C] = 1;

						else if ((nonmaximum_suppression[(C - (m.w- 2) + 1)] == 0 && nonmaximum_suppression[C + (m.w - 2) - 1] == 0) || (nonmaximum_suppression[(C - (m.w - 2) + 1)] == 0 && nonmaximum_suppression[C + (m.w - 2) - 1] == 1) || (nonmaximum_suppression[(C - (m.w - 2) + 1)] == 1 && nonmaximum_suppression[C + (m.w- 2) - 1] == 0))
							nonmaximum_suppression[C] = 0;
					}
				}
			}

			/*else
				binary_image[M] = nonmaximum_suppression[M];*/
		}
	}


	int* binary_edge_image = new int[(m.w - 2) * (m.h - 2)];
	for (int i = 0; i < m.h - 2; i++)
	{
		for (int j = 0; j < m.w - 2; j++)
		{
			C = i * (m.w - 2) + j;
			if (nonmaximum_suppression[C] == 0)
			{
				binary_edge_image[C] = 0;
			}
			else
			{
				binary_edge_image[C] = 1;
			}
		}
	}

	image binaryEdgeImage;
	unsigned char* data4 = (unsigned char*)malloc(sizeof(char) * ((m.h - 2) * (m.w - 2)));
	binaryEdgeImage.data = data4;
	binaryEdgeImage.h = raw.h - 2;
	binaryEdgeImage.w = raw.w - 2;
	binaryEdgeImage.c = 1;

	for (int i = 0; i < raw.h - 2; i++)
	{
		for (int j = 0; j < raw.w - 2; j++)
		{
			C = i * (m.w - 2) + j;
			if (binary_edge_image[C] == 0)
			{
				binaryEdgeImage.data[C] = 0;
			}
			else
			{
				binaryEdgeImage.data[C] = 255;
			}
		}
	}

	/*printf("\n binary edge start here");
for (int i = 0; i < raw.h - 2; i++)
{
	for (int j = 0; j < raw.w - 2; j++)
	{
		C = i * (raw.w - 2) + j;
		printf("%i: %i\n", C, binaryEdgeImage.data[C]);
	}
}*/

	long new_size;
	ConvertIntensityToRgb(binaryEdgeImage, &new_size);

	return binaryEdgeImage;
}