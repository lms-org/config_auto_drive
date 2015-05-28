/*
 * File: kalman_filter_lr.h
 *
 * MATLAB Coder version            : 2.7
 * C/C++ source code generated on  : 27-May-2015 22:19:16
 */

#ifndef __KALMAN_FILTER_LR_H__
#define __KALMAN_FILTER_LR_H__

/* Include Files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "kalman_filter_lr_types.h"

/* Function Declarations */
extern void kalman_filter_lr(emxArray_real_T *r, const emxArray_real_T *A,
  emxArray_real_T *Pk, const emxArray_real_T *Q, double R_fakt, double delta,
  emxArray_real_T *xl, emxArray_real_T *yl, emxArray_real_T *xr, emxArray_real_T
  *yr);

#endif

/*
 * File trailer for kalman_filter_lr.h
 *
 * [EOF]
 */
