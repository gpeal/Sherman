#include <plib.h>
#include <math.h>
#include <dsplib_dsp.h>
#include "fftc.h"


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
 */
void NU32_FFT_1024(double *dout, double *din, double multiplier)
{
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


/* This function is for performing a finite-impulse response filter of
 * a signal that is 1024 long.  It takes the following args:
 *	1) dout - pointer to result array
 *	2) din - pointer to input array
 *	3) coeffs - pointer to coefficient array
 *	4) mult_coeff - for scaling the coefficients
 *	5) mult_sig - for scaling the input signal
 *	6) num - number of filter coeffs
 *
 */
void NU32_FIR_1024(double *dout, double *din, double *coeffs,
		   double mult_coeff, double mult_sig, int num)
{
  double scale_c, scale_s, scale;
  int16 fir_coeffs[MAX_ORD], fir_coeffs2x[2*MAX_ORD];
  int16 delay[MAX_ORD];
  int16 din16[LEN], dout16[LEN];
  int i=0;
  unsigned int qform = 1<<QFORMAT;
  memset(delay, 0, sizeof(delay));

  /* get the final scaling factors: */
  scale_c = 0;
  for (i=0; i<num; i++)
    scale_c = max(scale_c, fabs(coeffs[i]));
  scale_c *= mult_coeff;
  scale_s = 0;
  for (i=0; i<LEN; i++)
    scale_s = max(scale_s, fabs(din[i]));
  scale_s *= mult_sig;
    
  /* now we can convert the FIR coefficients to Q15 */
  scale = qform/scale_c;
  for (i=0; i<num; i++)
    fir_coeffs[i] = (int) (coeffs[i]*scale);

  /* now we can convert the input signal to Q15 */
  scale = qform/scale_s;
  for (i=0; i<LEN; i++)
    din16[i] = (int) (din[i]*scale);

  /* setup the filter: */
  mips_fir16_setup(fir_coeffs2x, fir_coeffs, num);
  /* run the filter */
  mips_fir16(dout16, din16, fir_coeffs2x, delay, LEN, num, 0);

  /* convert back to original scaling */
  scale = scale_c*scale_s/qform;
  for (i=0; i<LEN; i++)
    dout[i] = dout16[i]*scale;    
}
