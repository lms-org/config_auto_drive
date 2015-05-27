/*
 * File: kalman_filter_lr.h
 *
 * MATLAB Coder version            : 2.7
 * C/C++ source code generated on  : 27-May-2015 19:40:39
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
extern void kalman_filter_lr(double r[10], const double A[100], double Pk[100],
  const double Q[100], double R_fakt, double delta, const double xl[10], const
  double yl[10], const double xr[10], const double yr[10]);

#endif

/*
 * File trailer for kalman_filter_lr.h
 *
 * [EOF]
 */
