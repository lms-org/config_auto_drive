/*
 * File: messmatrix.h
 *
 * MATLAB Coder version            : 2.7
 * C/C++ source code generated on  : 15-Jun-2015 13:18:28
 */

#ifndef __MESSMATRIX_H__
#define __MESSMATRIX_H__

/* Include Files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "kalman_filter_lr_types.h"

/* Function Declarations */
extern void messmatrix(const emxArray_real_T *P, const emxArray_real_T *r,
  double delta, const emxArray_real_T *ind, const emxArray_real_T *lambda,
  emxArray_real_T *H);

#endif

/*
 * File trailer for messmatrix.h
 *
 * [EOF]
 */
