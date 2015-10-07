//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: projectPoints.h
//
// MATLAB Coder version            : 3.0
// C/C++ source code generated on  : 07-Oct-2015 12:34:33
//
#ifndef __PROJECTPOINTS_H__
#define __PROJECTPOINTS_H__

// Include Files
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "kalman_filter_lr_types.h"

// Function Declarations
extern void projectPoints(const emxArray_real_T *r, double delta, double dist,
  emxArray_real_T *xp, emxArray_real_T *yp, emxArray_real_T *phi);

#endif

//
// File trailer for projectPoints.h
//
// [EOF]
//
