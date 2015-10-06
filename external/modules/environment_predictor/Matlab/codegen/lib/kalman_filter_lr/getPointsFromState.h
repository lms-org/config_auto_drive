//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: getPointsFromState.h
//
// MATLAB Coder version            : 3.0
// C/C++ source code generated on  : 06-Oct-2015 19:14:10
//
#ifndef __GETPOINTSFROMSTATE_H__
#define __GETPOINTSFROMSTATE_H__

// Include Files
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "kalman_filter_lr_types.h"

// Function Declarations
extern void getPointsFromState(const emxArray_real_T *r, double delta,
  emxArray_real_T *X, emxArray_real_T *Y, emxArray_real_T *PHI);

#endif

//
// File trailer for getPointsFromState.h
//
// [EOF]
//
