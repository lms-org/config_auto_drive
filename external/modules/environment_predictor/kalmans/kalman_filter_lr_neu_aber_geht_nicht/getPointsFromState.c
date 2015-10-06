/*
 * File: getPointsFromState.c
 *
 * MATLAB Coder version            : 2.7
 * C/C++ source code generated on  : 26-Jun-2015 19:42:29
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "kalman_filter_lr.h"
#include "getPointsFromState.h"
#include "kalman_filter_lr_emxutil.h"

/* Function Definitions */

/*
 * Arguments    : const emxArray_real_T *r
 *                double delta
 *                emxArray_real_T *X
 *                emxArray_real_T *Y
 *                emxArray_real_T *PHI
 * Return Type  : void
 */
void getPointsFromState(const emxArray_real_T *r, double delta, emxArray_real_T *
  X, emxArray_real_T *Y, emxArray_real_T *PHI)
{
  emxArray_real_T *P;
  int r_idx_0;
  int i0;
  double phi;
  b_emxInit_real_T(&P, 2);
  r_idx_0 = r->size[0];
  i0 = P->size[0] * P->size[1];
  P->size[0] = r_idx_0;
  P->size[1] = 3;
  emxEnsureCapacity((emxArray__common *)P, i0, (int)sizeof(double));
  r_idx_0 = r->size[0] * 3;
  for (i0 = 0; i0 < r_idx_0; i0++) {
    P->data[i0] = 0.0;
  }

  /* x, y, phi */
  phi = r->data[1];

  /* erster Punkt  */
  P->data[0] = 0.0;
  P->data[P->size[0]] = r->data[0];
  P->data[P->size[0] << 1] = r->data[1];

  /* zweiter Punkt */
  P->data[1] = P->data[0] + delta * cos(r->data[1]);
  P->data[1 + P->size[0]] = P->data[P->size[0]] + delta * sin(r->data[1]);
  P->data[1 + (P->size[0] << 1)] = r->data[1];
  for (r_idx_0 = 2; r_idx_0 - 2 <= r->size[0] - 3; r_idx_0++) {
    /*  "-" wegen VZ-Definition der Kruemmung */
    phi = (phi + 2.0 * acos(-delta * r->data[r_idx_0] / 2.0)) -
      3.1415926535897931;
    P->data[r_idx_0] = P->data[r_idx_0 - 1] + delta * cos(phi);
    P->data[r_idx_0 + P->size[0]] = P->data[(r_idx_0 + P->size[0]) - 1] + delta *
      sin(phi);
    P->data[r_idx_0 + (P->size[0] << 1)] = phi;
  }

  r_idx_0 = P->size[0];
  i0 = X->size[0];
  X->size[0] = r_idx_0;
  emxEnsureCapacity((emxArray__common *)X, i0, (int)sizeof(double));
  for (i0 = 0; i0 < r_idx_0; i0++) {
    X->data[i0] = P->data[i0];
  }

  r_idx_0 = P->size[0];
  i0 = Y->size[0];
  Y->size[0] = r_idx_0;
  emxEnsureCapacity((emxArray__common *)Y, i0, (int)sizeof(double));
  for (i0 = 0; i0 < r_idx_0; i0++) {
    Y->data[i0] = P->data[i0 + P->size[0]];
  }

  r_idx_0 = P->size[0];
  i0 = PHI->size[0];
  PHI->size[0] = r_idx_0;
  emxEnsureCapacity((emxArray__common *)PHI, i0, (int)sizeof(double));
  for (i0 = 0; i0 < r_idx_0; i0++) {
    PHI->data[i0] = P->data[i0 + (P->size[0] << 1)];
  }

  emxFree_real_T(&P);
}

/*
 * File trailer for getPointsFromState.c
 *
 * [EOF]
 */
