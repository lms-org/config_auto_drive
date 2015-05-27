/*
 * File: eye.c
 *
 * MATLAB Coder version            : 2.7
 * C/C++ source code generated on  : 27-May-2015 19:40:39
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "kalman_filter_lr.h"
#include "eye.h"

/* Function Definitions */

/*
 * Arguments    : double I[100]
 * Return Type  : void
 */
void b_eye(double I[100])
{
  int k;
  memset(&I[0], 0, 100U * sizeof(double));
  for (k = 0; k < 10; k++) {
    I[k + 10 * k] = 1.0;
  }
}

/*
 * Arguments    : double I[1600]
 * Return Type  : void
 */
void eye(double I[1600])
{
  int k;
  memset(&I[0], 0, 1600U * sizeof(double));
  for (k = 0; k < 40; k++) {
    I[k + 40 * k] = 1.0;
  }
}

/*
 * File trailer for eye.c
 *
 * [EOF]
 */
