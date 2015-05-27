/*
 * File: messmatrix_new.h
 *
 * MATLAB Coder version            : 2.7
 * C/C++ source code generated on  : 27-May-2015 19:40:39
 */

#ifndef __MESSMATRIX_NEW_H__
#define __MESSMATRIX_NEW_H__

/* Include Files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "kalman_filter_lr_types.h"

/* Function Declarations */
extern void messmatrix_new(const double P[30], const double r[10], double delta,
  const double ind[10], const double lambda[10], double H[200]);

#endif

/*
 * File trailer for messmatrix_new.h
 *
 * [EOF]
 */
