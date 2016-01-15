//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: handle_measurements.h
//
// MATLAB Coder version            : 3.0
// C/C++ source code generated on  : 29-Nov-2015 11:30:09
//
#ifndef __HANDLE_MEASUREMENTS_H__
#define __HANDLE_MEASUREMENTS_H__

// Include Files
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "kalman_filter_lr_types.h"

// Function Declarations
extern void b_handle_measurements(const emxArray_real_T *r, double delta,
  emxArray_real_T *xm, emxArray_real_T *ym, emxArray_real_T *H, emxArray_real_T *
  z, emxArray_real_T *zm, double *nValidPoints);
extern void c_handle_measurements(const emxArray_real_T *r, double delta,
  emxArray_real_T *xm, emxArray_real_T *ym, emxArray_real_T *H, emxArray_real_T *
  z, emxArray_real_T *zm, double *nValidPoints);
extern void handle_measurements(const emxArray_real_T *r, double delta,
  emxArray_real_T *xm, emxArray_real_T *ym, emxArray_real_T *H, emxArray_real_T *
  z, emxArray_real_T *zm, double *nValidPoints);

#endif

//
// File trailer for handle_measurements.h
//
// [EOF]
//
