/*
 * File: d_line_point.h
 *
 * MATLAB Coder version            : 2.7
 * C/C++ source code generated on  : 26-Jun-2015 19:42:29
 */

#ifndef __D_LINE_POINT_H__
#define __D_LINE_POINT_H__

/* Include Files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "kalman_filter_lr_types.h"

/* Function Declarations */
extern void d_line_point(const double P[2], const double Q[2], const double M[2],
  double *d, double *lambda, double S[2]);

#endif

/*
 * File trailer for d_line_point.h
 *
 * [EOF]
 */
