/*
 * File: getPointsFromState.h
 *
 * MATLAB Coder version            : 2.7
 * C/C++ source code generated on  : 10-Jun-2015 13:38:07
 */

#ifndef __GETPOINTSFROMSTATE_H__
#define __GETPOINTSFROMSTATE_H__

/* Include Files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "kalman_filter_lr_types.h"

/* Function Declarations */
extern void getPointsFromState(const emxArray_real_T *r, double delta,
  emxArray_real_T *X, emxArray_real_T *Y, emxArray_real_T *PHI);

#endif

/*
 * File trailer for getPointsFromState.h
 *
 * [EOF]
 */
