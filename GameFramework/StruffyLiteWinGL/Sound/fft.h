
#ifndef FFT_H
#define FFT_H

/*
 * Allocates the temporary buffers that are needed for the FFT. Length is the
 * number of frequency bins. It should be equal to the number of samples in an
 * audio buffer, and a power of 2.
 */
extern void init_fft(int length, int sampleRate);

/*
 * Deallocates the temporary buffers that were used by the FFT.
 */
extern void done_fft();

/*
 * Finds the loudest frequency in the recorded buffer. Input format must be
 * 16-bit little endian.
 */
extern float find_pitch(unsigned char* buffer, float minFreq, float maxFreq);

#endif  // FFT_H
