/*==============================================================================
  Copyright (c) 2012-2014 Qualcomm Technologies, Inc.
  All rights reserved. Qualcomm Proprietary and Confidential.
==============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include "typedef.h"
#include "fft_util.h"
#include "fft.h"
#include "fftwiddles.h"
#include "q6basic_op.h"
#include "data_macros.h"
#include "flt_bkfir.h"
#include "flt_iir_cas_bq.h"
#include "hexagon_sim_timer.h"
#include "verify.h"
#include "HAP_farf.h"
#include "dsp.h"

#if 1
CWord2x16 Input[256]   __attribute__((aligned(1<<16)));
CWord2x16 Output[256]  __attribute__((aligned(8)));
CWord2x16 Expected_results[256];

#define  N        2048
#define  LOG2N    11
#define  NTEST     1

typedef  __complex__ float    cmplxf_t;
typedef volatile unsigned long u32;
typedef volatile unsigned long long u64;

cfloat fInput[N]   __attribute__((aligned(1<<16)));
cfloat fOutput[N]  __attribute__((aligned(8)));
cfloat fWtwiddles[3*N/4] __attribute__((aligned(8)));
cfloat fref[N]  __attribute__((aligned(8)));

/**************************************************/
/* Defines of IIR and FIR Test case 		  */
/**************************************************/

#define		T_BKFIR              16
#define		N_BKFIR              80

float coeffs[T_BKFIR] __attribute__((aligned(8))) = {
0.025452, -0.977081, -0.828766, 0.619141, -0.668732, -0.279694, 0.375275, 0.213715, 
0.410980, -0.526672, 0.962097, 0.158844, -0.034271, 0.224792, 0.357727, -0.792664 
};

float input[N_BKFIR+T_BKFIR] __attribute__((aligned(8))) = {
-1.899977, -3.680657, 3.296419, -3.510036, 1.486542, -0.897696, 2.541173, -2.555543, 
-0.551665, -2.112112, 0.548700, 1.022696, 1.921077, 2.624202, -2.786952, 3.313298, 
-0.733919, -1.848654, -1.185789, -1.360515, 3.265169, 2.493613, 0.622149, 2.278057, 
-0.255817, -3.658075, -0.154881, -1.616674, -3.469548, 0.397240, -0.671533, -2.709512, 
1.025091, 3.582687, 1.655452, -0.428832, 2.716127, 0.109831, 2.642564, 2.430885, 
0.457801, -0.893362, 2.563983, -0.950274, -3.380702, -0.549612, 2.990192, -1.924840, 
1.663549, -0.940351, -3.637203, 2.655224, -2.580862, -2.345575, 2.146556, 2.384694,
-0.551665, -2.112112, 0.548700, 1.022696, 1.921077, 2.624202, -2.786952, 3.313298, 
-0.733919, -1.848654, -1.185789, -1.360515, 3.265169, 2.493613, 0.622149, 2.278057, 
-0.255817, -3.658075, -0.154881, -1.616674, -3.469548, 0.397240, -0.671533, -2.709512, 
1.025091, 3.582687, 1.655452, -0.428832, 2.716127, 0.109831, 2.642564, 2.430885, 
0.457801, -0.893362, 2.563983, -0.950274, -3.380702, -0.549612, 2.990192, -1.924840 
};

float output[N_BKFIR]  __attribute__((aligned(8)));
float ref_out[N_BKFIR];



#define NS       81

float Coeffs[8] __attribute__((aligned(8))) =
{
// first section
0.067352,       // f0
0.999969,       // b0
-0.194000,      // f1
0.500000,       // b1
// second section
-0.032043,      // f0
0.999969,       // b0
-0.156982,      // f1
0.500000        // b1
};


float InputIIR[] =
{ 471,2159,2695,2607,1319,488,625,1239,-42,-771,
  -92,-566,-1601,-794,-1380,-739,-750,-1513,-1604,-543,
  -153,-158,563,1330,1024,1596,2286,2985,2526,2263,
 2268,1840,1838,954,520,-337,-488,243,-1096,-1872,
  471,2159,2695,2607,1319,488,625,1239,-42,-771,
  -92,-566,-1601,-794,-1380,-739,-750,-1513,-1604,-543,
  -153,-158,563,1330,1024,1596,2286,2985,2526,2263,
 2268,1840,1838,954,520,-337,-488,243,-1096,-1872,
 100
};

float States[4] __attribute__((aligned(8)));
float OutputIIR[NS], ref_outIIR[NS];

extern void flt_IIR_casBiquad_ref(float *in, float *coef, float *states, int nSec, int nsamples, float *out); 

#endif

uint64_t diff_time(struct timeval *start, struct timeval * end) {
   uint64_t s = start->tv_sec * 1000000 + start->tv_usec;
   uint64_t e = end->tv_sec * 1000000 + end->tv_usec;
   return e - s;
}

int dsp_fft(){

	int i, j, Nfft;
	int i_fir;
    	int i_iir;

	struct timeval start, end;

	gettimeofday(&start, 0);

	FARF(ALWAYS, "===============     DSP: FFT --  ===============");
#if 1
	for (j = 0; j < NTEST; j++) 
	{
		for (i=0; i<256; i++)
		{
		   Input[i] = 1567788;//(CWord2x16)mrand48();
		   fInput[i] = 13456.34;
		}

		printf("Attn:  DSP: sfft 128 result\n");
		FARF(RUNTIME_HIGH, "===============     DSP: sfft 128 result  ===============");
		/* --------------------------------------------*/
		/* test precision of 128-point scaled SFFT      */
		/* ------------------------------------------- */
		Nfft = 128;
		fft16x16(Input, Nfft, Wtwiddles, Output);
		PrecisionCheck( Input, Output, Nfft, 1, 1);
		FARF(RUNTIME_HIGH, "===============     DSP: sfft 256 result  ===============");
		printf("Attn:  DSP: sfft 256 result\n");
		/* --------------------------------------------*/
		/* test precision of 256-point scaled SFFT      */
		/* ------------------------------------------- */
		Nfft = 256;
		fft16x16(Input, Nfft, Wtwiddles, Output);
		PrecisionCheck( Input, Output, Nfft, 1, 1);

		FARF(RUNTIME_HIGH, "===============     DSP: fft 128 result  ===============");
		printf("Attn:  DSP: fft 128 result\n");
		/* --------------------------------------------*/
		/* test precision of 128-point scaled FFT      */
		/* ------------------------------------------- */
		Nfft = 128;
		sfft16x16(Input, Nfft, Wtwiddles, Output);
		PrecisionCheck( Input, Output, Nfft, 1, 1);
		FARF(RUNTIME_HIGH, "===============     DSP: fft 256 result  ===============");
		printf("Attn:  DSP: fft 256 result\n");
		/* --------------------------------------------*/
		/* test precision of 256-point scaled FFT      */
		/* ------------------------------------------- */
		Nfft = 256;
		sfft16x16(Input, Nfft, Wtwiddles, Output);
		PrecisionCheck( Input, Output, Nfft, 1, 1);
		FARF(RUNTIME_HIGH, "===============     DSP: ifft 128 result  ===============");
		printf("Attn:  DSP: ifft 128 result\n");
		/* --------------------------------------------*/
		/* test precision of 256-point scaled iFFT      */
		/* ------------------------------------------- */
		Nfft = 128;
		ifft16x16(Input, Nfft, Wtwiddles, Output);
		PrecisionCheck( Input, Output, Nfft, 1, 1);
		FARF(RUNTIME_HIGH, "===============     DSP: ifft 256 result  ===============");
		printf("Attn:  DSP: ifft 256 result\n");
		/* --------------------------------------------*/
		/* test precision of 256-point scaled iFFT      */
		/* ------------------------------------------- */
		Nfft = 256;
		ifft16x16(Input, Nfft, Wtwiddles, Output);
		PrecisionCheck( Input, Output, Nfft, 1, 1);

		FARF(RUNTIME_HIGH, "===============     DSP: Float fft 128 result  ===============");
		printf("Attn:  DSP: float-fft 128 result\n");
		/* --------------------------------------------*/
		/* test precision of 128-point scaled Float FFT      */
		/* ------------------------------------------- */
		Nfft = 128;
		sfpFFT(fInput, Nfft, fWtwiddles, fOutput);
		fPrecisionCheck( fInput, fOutput, Nfft);

		FARF(RUNTIME_HIGH, "===============     DSP: Float fft 256 result  ===============");
		printf("Attn:  DSP: float-fft 256 result\n");
		/* --------------------------------------------*/
		/* test precision of 256-point scaled Float FFT      */
		/* ------------------------------------------- */
		Nfft = 256;
		sfpFFT(fInput, Nfft, fWtwiddles, fOutput);
		fPrecisionCheck( fInput, fOutput, Nfft);
	}
#endif


		/* --------------------------------------------*/
		/* 		              BKFIR   	                 */
		/* ------------------------------------------- */


		// Generate reference outputs
		flt_bkfir_ref(input,coeffs,T_BKFIR,N_BKFIR,ref_out);
		//FARF(RUNTIME_HIGH, "===============     DSP: BKFIR  ===============");
		//printf("Calling FIR Kernel\n");

		flt_bkfir(input,coeffs,T_BKFIR,N_BKFIR,output);

		for (i_fir = 0; i_fir < N_BKFIR; i_fir++) {
			if ( fabs(output[i_fir]-ref_out[i_fir]) > 1.0e-5) {
				//FARF(HIGH, "### FAILED!!!"); 
				return 1;
			}
		}
		//printf("*** PASS!\n");
    //FARF(HIGH, "===============     DSP: BKFIR Test  ===============");


		/* --------------------------------------------*/
		/* 		              IIR   	                 */
		/* ------------------------------------------- */

		// Generate reference outputs
		for (i_iir=0; i_iir<4; i_iir++)
		States[i_iir] = 0;
		flt_IIR_casBiquad_ref(InputIIR,Coeffs,States,2,NS, ref_outIIR);

		//printf("Calling IIR Kernel\n");
	        // FARF(RUNTIME_HIGH, "===============     DSP: IIR  ===============");

		for (i_iir=0; i_iir<4; i_iir++)
		States[i_iir] = 0;

		flt_IIR_casBiquad(InputIIR,Coeffs,States,2,NS, OutputIIR);


		for (i_iir=0;i_iir<NS;i_iir++) {
		if (fabs(OutputIIR[i_iir]- ref_outIIR[i_iir]) > 0.001) {
		    //printf("### FAILED!!!\n");
		    return 1;
		}
		}

		//printf("*** PASS!\n");
    //FARF(HIGH, "===============     DSP: IIR Test  ===============");
		gettimeofday(&end, 0);
		FARF(HIGH, "time: %llu\n", (long long unsigned int)diff_time(&start, &end));

	return 0;

};

