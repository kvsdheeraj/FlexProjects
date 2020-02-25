#pragma once

//#include "NE10.h"
#include <stdio.h>

//Replacing ne10_float32_t to float and ne10_fft_cpx_float32_t to cfloat

typedef __complex float  cfloat;

void dspClearFloatBuffer( 
    float *inplace, 
    size_t size );
    
void dspClearComplexBuffer( 
    cfloat *inplace, 
    size_t size );
    
void dspCopyFloatBuffer( 
    float *output, 
    const float *input, 
    size_t size );
    
void dspCopyComplexBuffer( 
    cfloat *output, 
    const cfloat *input, 
    size_t size );
    
    
