/***************************************************************************
* Copyright (c) Date: Mon Nov 24 16:26:03 CST 2008 QUALCOMM INCORPORATED 
* All Rights Reserved 
* Modified by QUALCOMM INCORPORATED on Mon Nov 24 16:26:03 CST 2008 
****************************************************************************/ 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "typedef.h"
#include "q6basic_op.h"
#include "fft_util.h"

typedef  __complex__ double   cmplxf_t;
typedef  __complex__ float    fcmplxf_t;


void PrecisionCheck(CWord2x16 *Input, CWord2x16 *FFTOut, int npoint, int fftmode, int scale)
{
   Word16 xr, xi;
   double er, ei, err, sig, ampMax;
   double arg;
   double pi = acos (-1.0);
   cmplxf_t s, wk, xk, refout;
   int j;

   int i, k; 

   sig = 0.0;
   err = 0.0;
   ampMax = 0.0;

   /* --------------------------------------------------------- */
   /*    Direct implementation in floating point                */
   /* --------------------------------------------------------- */
   for (i = 0; i < npoint; i++)
   {
     s = 0.0;

     for (j = 0; j < npoint; j++) {
         k = (i*j) %npoint;
         arg = k*2*pi/npoint;
         if (fftmode)      
            arg = - arg;     // FFT
         wk  = cos (arg) + 1i*sin (arg);
         xk  = real(Input[j]) + 1i*imag(Input[j]);
         s   = s + xk * wk;
     }

     if (fftmode)      
     {   // FFT
         if (scale)
           refout = s/npoint;
         else 
           refout = s;
     }
     else
     {   // IFFT
         if (scale)
           refout = s;
         else 
           refout = s/npoint;
     }

     xr = real(FFTOut[i]);        xi = imag(FFTOut[i]);
     er = __real__(refout);       ei = __imag__(refout);

     sig += er * er + ei * ei;

     if (ampMax < fabs(er) ) ampMax = fabs(er);
     if (ampMax < fabs(ei) ) ampMax = fabs(ei);

     er -= xr;  ei -= xi;

     printf("Point #%d: err %d %d\n", i, (int)er, (int)ei);

     err += er * er + ei * ei;

   }
   printf("Precision bits: %2.2f bits of %2.2f(max)\n\n", 0.5*log(sig/err)/log(2.0), 1.0+log(ampMax)/log(2.0));
}


void fPrecisionCheck( cfloat *Input, cfloat *FFTOut, int npoint)
{
   double xr, xi;
   double er, ei, err, sig, ampMax;
   double arg;
   double pi = acos (-1.0);
   fcmplxf_t s, wk, xk;

   int i, j, k;

   sig = 0.0;
   err = 0.0;
   ampMax = 0.0;

   /* --------------------------------------------------------- */
   /*    Direct implementation in floating point                */
   /* --------------------------------------------------------- */
   for (i = 0; i < npoint; i++)
   {
     s = 0.0;

     for (j = 0; j < npoint; j++) {
         k = (i*j) %npoint;
         arg = - k*2*pi/npoint;
         wk  = cos (arg) + 1i*sin (arg);
         xk  = Input[j];
         s   = s + xk * wk;
     }

     xr = __real__(FFTOut[i]);   xi = __imag__(FFTOut[i]);
     er = __real__(s);           ei = __imag__(s);

//     printf("(%f, %f) (%f, %f)", xr, xi, er, ei);

     sig += er * er + ei * ei;

     if (ampMax < fabs(er) ) ampMax = fabs(er);
     if (ampMax < fabs(ei) ) ampMax = fabs(ei);

     er -= xr;  ei -= xi;

//     printf("Point #%d: err %f %f\n", i, er, ei);

     err += er * er + ei * ei;

   }
   printf("SNR= %.2f  of %.2f(max)\n", 10*log10(sig/err), 20*log10(ampMax));
}


