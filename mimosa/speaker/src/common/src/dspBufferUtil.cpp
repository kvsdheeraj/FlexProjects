#include "dspBufferUtil.h"

void dspClearFloatBuffer( 
    float *inplace, 
    size_t size )
{
    for( unsigned int i = 0; i < size; i++ )
    {
        inplace[i] = 0.0f;
    }
}

void dspClearComplexBuffer( 
    cfloat *inplace, 
    size_t size )
{
    for( unsigned int i = 0; i < size; i++ )
    {
        inplace[i] = 0.0f;
        inplace[i] = 0.0f;
    }
}

void dspCopyFloatBuffer( 
    float *output, 
    const float *input, 
    size_t size )
{
    for( unsigned int i = 0; i < size; i++ )
    {
        output[i] = input[i];
    }
}

void dspCopyComplexBuffer( 
    cfloat *output, 
    const cfloat *input, 
    size_t size )
{
    for( unsigned int i = 0; i < size; i++ )
    {
        output[i] = input[i];
        output[i] = input[i];
    }
}

