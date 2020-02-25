/*==============================================================================
  Copyright (c) 2015 Qualcomm Technologies, Inc.
  All rights reserved. Qualcomm Proprietary and Confidential.
==============================================================================*/

#ifndef CONTROL_DSP
#define CONTROL_DSP

class Calculator{

private:
    std::vector<int> input_vec;

public:
    Calculator(const int* seq, int vecLen);
    Calculator(const char* filename);
    int64 sum();
};

#endif
