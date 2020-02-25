/*==============================================================================
  Copyright (c) 2012-2014,2017 Qualcomm Technologies, Inc.
  All rights reserved. Qualcomm Proprietary and Confidential.
==============================================================================*/

#include "dsp.h"
#include "dsp_test.h"
#include "rpcmem.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int local_dsp_sum(const int* vec, int vecLen, int64* res)
{
  printf("- Run code in CPU locally\n");
  return 0;
}

int dsp_test(int runLocal, int num)
{
  int nErr = 0;
  int* test = 0;
  int len = 0;
  int ii;
  int64 result = 0;

  rpcmem_init();

  len = sizeof(*test) * num;
  printf("- allocate %d bytes from ION heap\n", len);
  if (0 == (test = (int*)rpcmem_alloc(0, RPCMEM_HEAP_DEFAULT, len))) {
    printf("Error: alloc failed\n");
    nErr = 1;
    goto bail;
  }

  printf("- creating sequence of numbers from 0 to %d\n", num - 1);
  for (ii = 0; ii < num; ++ii) {
    test[ii] = ii;
  }

  if (runLocal) {
    printf("- compute sum locally\n");
    if (0 != local_dsp_sum(test, num, &result)) {
      printf("Error: local compute failed\n");
      nErr = 1;
      goto bail;
    }
  } else {
    printf("- compute FFT on the DSP\n");
    if (0 != dsp_fft()) {
      printf("Error: compute FFT on DSP failed\n");
      nErr = 1;
      goto bail;
    }
  }

bail:
  if (test) {
    rpcmem_free(test);
  }
  rpcmem_deinit();
  return nErr;
}
