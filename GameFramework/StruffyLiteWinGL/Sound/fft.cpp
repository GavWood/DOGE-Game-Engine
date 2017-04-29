/*
Copyright(c) 2009 - 2010 Matthijs Hollemans

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "fft.h"

/*
 * Conversion between frequency bins and actual frequencies:
 *     frequency = frequency_bin * sample_rate / length_of_spectrum
 *     frequency_bin = frequency * length_of_spectrum / sample_rate
 */

typedef struct
{
	float real;
	float imag;
}
complex_t;

const float PI = 3.14159265358979323846264338f;

static int sampleRate = 0;
static int length = 0;          // number of samples / frequency bins
static int numBits = 0;         // how many bits we need for "length"
static float* samples = NULL;   // floating point samples / frequency spectrum
static float* real = NULL;      // temporary buffer (real part)
static float* imag = NULL;      // temporary buffer (imaginary part)
static float* sinTable = NULL;  // sin() look-up table
static float* cosTable = NULL;  // cos() look-up table
static float* window = NULL;    // Hamming window

// Calculates log2 of number.  
double Log2( double n )  
{  
	// log(n)/log(2) is log2.  
	return log( n ) / log( 2.0 );  
}

static int reverse_bits(int n, int numBits)
{
	int k = 0;
	for (int i = 0; i < numBits; i++)
	{
		int j = n >> 1;
		k = ((k << 1) + n) - (j << 1);
		n = j;
	}
	return k;
}

static void fft()
{
	for (int i = 0; i < length; i++)
	{
		int j = reverse_bits(i, numBits);
		real[j] = samples[i];
		imag[j] = 0.0f;
	}

	int x = 0;
	for (int i = 0; i < numBits; i++)  // Cooley-Tukey 
	{
		int m = 1 << i;
		int n = m << 1;

		for (int k = 0; k < m; k++)
		{
			float c = cosTable[x];
			float s = sinTable[x];
			x++;

			float tr, ti;
			for (int j = k; j < length; j += n)
			{
				tr = c * real[j + m] - s * imag[j + m];
				ti = s * real[j + m] + c * imag[j + m];

				real[j + m] = real[j] - tr;
				imag[j + m] = imag[j] - ti;

				real[j] += tr;
				imag[j] += ti;
			}
		}
	}
	
	for (int i = 0; i < length; i++)  // power spectrum
	{
		samples[i] = real[i]*real[i] + imag[i]*imag[i];
	}
}

void init_fft(int length_, int sampleRate_)
{
	sampleRate = sampleRate_;
	length = length_;
	numBits = (int) Log2(length);

	samples = (float*) malloc(length*sizeof(float));
	window = (float*) malloc(length*sizeof(float));
	real = (float*) malloc(length*sizeof(float));
	imag = (float*) malloc(length*sizeof(float));

	cosTable = (float*) malloc(length*sizeof(float));
	sinTable = (float*) malloc(length*sizeof(float));

	int x = 0;
	for (int i = 0; i < numBits; i++)  // pre-calculate sin and cos
	{
		int m = 1 << i;
		int n = m << 1;
		for (int k = 0; k < m; k++)
		{
			float arg = -2.0f * PI * k / n;
			cosTable[x] = cosf(arg);
			sinTable[x] = sinf(arg);
			x++;
		}
	}

	for (int t = 0; t < length; t++)  // Hamming window
		window[t] = 0.53836f - 0.46164f * cosf(2*PI*t/(length - 1));
}

void done_fft()
{
	free(samples);
	free(real);
	free(imag);
	free(sinTable);
	free(cosTable);
	free(window);
}

float find_pitch(unsigned char* buffer, float minFreq, float maxFreq)
{
	for (int t = 0; t < length; t++)
	{
		unsigned char byte1 = buffer[t*2];
		unsigned char byte2 = buffer[t*2 + 1];
		short sample = byte1 | (byte2 << 8);   // little endian
		samples[t] = window[t] * sample / 32768.0f;
	}

	fft();

	int bin = 0;
	float max = samples[0];

	int startBin = (int) (minFreq * length / sampleRate) - 1;
	if (startBin < 1)
		startBin = 1;  // we've already looked at bin 0

	int endBin = (int) (maxFreq * length / sampleRate) + 1;
	if (endBin > length)
		endBin = length;

	for (int i = startBin; i < endBin; i++)
	{
		if (samples[i] > max)
		{
			max = samples[i];
			bin = i;
		}
	}

	// On the iPhone itself (not in the simulator) there seems to be an issue
	// with the microphone picking up some noise. Therefore, we ignore the freq
	// if the max amplitude is below a threshold (chosen by trial-and-error). 
	if (max < 50) bin = 0;

	float freq = (float) bin * sampleRate / length;
	if (freq < minFreq)
		freq = 0;

	return freq;
}
