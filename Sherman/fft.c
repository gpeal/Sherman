#include "fft.h"
#include "fftc.h"
#include <plib.h>
#include <math.h>
#include <dsplib_dsp.h>


#define TWIDDLE fft16c1024
#define LOG2N 10
#define LEN (1 << LOG2N)
#define MAX_ORD (128)
#define QFORMAT (15)

/* This function is for performing an FFT on a signal of 1024 length.
 * It takes the following arguments:
 *	1) dout - pointer to array where answer will be stored
 *	2) din - pointer to double array to be analyzed
 *	3) scale - used in converting values to Q15 format.
 *
 *      NOTE: the original function was not written by Gabriel Peal or Kashif Malik
 */
void fft(double *dout, double *din)
{
    double multiplier = 10;
    double scale = 0.0;
    int i = 0;
    int16c twiddle[LEN/2];
    unsigned int qform = 1<<QFORMAT;
    int16c dest[LEN], src[LEN];
    double re, im;
    int16c scratch[LEN];
    double scale2 = 0.0;

    /* The first thing to do is rescale din to fit in Q15 */
    /* To start, we find the max value of the signal: */
    for (i=0; i<LEN; i++)
        scale = max(scale, fabs(din[i]));
    /* set the final scaling value */
    scale *= multiplier;

    scale2 = qform/scale;
    /* convert to Q31: */
    for (i=0; i<LEN; i++) {
        src[i].re = (int) (din[i] * scale2);
        src[i].im = 0;
    }

  memcpy(twiddle, TWIDDLE, sizeof (TWIDDLE));
  /* Now we are actually ready to do the FFT */
  mips_fft16(dest, src, twiddle, scratch, LOG2N);
  scale2 = scale/qform;
  /* now we can convert the results back to doubles */
  for (i=0; i<LEN; i++) {
    re = dest[i].re * scale2;
    im = dest[i].im * scale2;
    dout[i] = sqrt(re*re + im*im);
  }
}
