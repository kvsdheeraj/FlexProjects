/***************************************************************************
* Copyright (c) Date: Mon Nov 24 16:26:00 CST 2008 QUALCOMM INCORPORATED 
* All Rights Reserved 
* Modified by QUALCOMM INCORPORATED on Mon Nov 24 16:26:00 CST 2008 
****************************************************************************/ 


Word32 bitrev( Word32 x, Word32 BITS );

void Radix2BTFLY( CWord2x16 *x );
void Radix4BTFLYqv3( CWord2x16 *x );
void Radix4BTFLY( CWord2x16 *x );

// scaled version 
void sRadix2BTFLY( CWord2x16 *x );
void sRadix4BTFLY( CWord2x16 *x );
void sRadix4BTFLYqv3( CWord2x16 *x );

void ifftRadix2BTFLY( CWord2x16 *x );
void ifftRadix4BTFLY( CWord2x16 *x );
void ifftRadix4BTFLYqv3( CWord2x16 *x );

void PrecisionCheck( CWord2x16 *Input, CWord2x16 *FFTOut, 
                     int npoint, int fftmode, int scale  );
void fPrecisionCheck( cfloat *Input, cfloat *FFTOut, int npoint);
