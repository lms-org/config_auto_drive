/*
 * File: projectPoints.h
 *
 * MATLAB Coder version            : 2.7
 * C/C++ source code generated on  : 27-May-2015 22:19:16
 */

#ifndef __PROJECTPOINTS_H__
#define __PROJECTPOINTS_H__

/* Include Files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "kalman_filter_lr_types.h"

/* Function Declarations */
extern void b_projectPoints(const emxArray_real_T *r, double delta,
  emxArray_real_T *xp, emxArray_real_T *yp, emxArray_real_T *phi);
extern void projectPoints(const emxArray_real_T *r, double delta,
  emxArray_real_T *xp, emxArray_real_T *yp, emxArray_real_T *phi);

#endif

/*
 * File trailer for projectPoints.h
 *
 * [EOF]
 */
