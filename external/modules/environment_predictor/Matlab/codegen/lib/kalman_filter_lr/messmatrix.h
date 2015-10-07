//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: messmatrix.h
//
// MATLAB Coder version            : 3.0
// C/C++ source code generated on  : 07-Oct-2015 12:34:33
//
#ifndef __MESSMATRIX_H__
#define __MESSMATRIX_H__

// Include Files
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "kalman_filter_lr_types.h"

// Function Declarations
extern void messmatrix(const emxArray_real_T *P, const emxArray_real_T *r,
  double delta, const emxArray_real_T *ind, const emxArray_real_T *lambda,
  emxArray_real_T *H);

#endif

//
// File trailer for messmatrix.h
//
// [EOF]
//
