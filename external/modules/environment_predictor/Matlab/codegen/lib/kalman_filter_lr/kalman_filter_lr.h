//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: kalman_filter_lr.h
//
// MATLAB Coder version            : 3.0
// C/C++ source code generated on  : 07-Oct-2015 12:34:33
//
#ifndef __KALMAN_FILTER_LR_H__
#define __KALMAN_FILTER_LR_H__

// Include Files
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "kalman_filter_lr_types.h"

// Function Declarations
extern void kalman_filter_lr(emxArray_real_T *r, double delta_x, double delta_y,
  double delta_phi, emxArray_real_T *Pk, const emxArray_real_T *Q, double R_fakt,
  double delta, const emxArray_real_T *xl, const emxArray_real_T *yl, const
  emxArray_real_T *xr, const emxArray_real_T *yr, const emxArray_real_T *xm,
  const emxArray_real_T *ym, unsigned char interp_mode);

#endif

//
// File trailer for kalman_filter_lr.h
//
// [EOF]
//
