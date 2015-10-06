//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: getPointsFromState.cpp
//
// MATLAB Coder version            : 3.0
// C/C++ source code generated on  : 06-Oct-2015 19:14:10
//

// Include Files
#include "rt_nonfinite.h"
#include "kalman_filter_lr.h"
#include "getPointsFromState.h"
#include "kalman_filter_lr_emxutil.h"

// Function Definitions

//
// Arguments    : const emxArray_real_T *r
//                double delta
//                emxArray_real_T *X
//                emxArray_real_T *Y
//                emxArray_real_T *PHI
// Return Type  : void
//
void getPointsFromState(const emxArray_real_T *r, double delta, emxArray_real_T *
  X, emxArray_real_T *Y, emxArray_real_T *PHI)
{
  emxArray_real_T *P;
  int i2;
  int loop_ub;
  double phi;
  int s;
  emxInit_real_T(&P, 2);
  i2 = P->size[0] * P->size[1];
  P->size[0] = r->size[0];
  P->size[1] = 3;
  emxEnsureCapacity((emxArray__common *)P, i2, (int)sizeof(double));
  loop_ub = r->size[0] * 3;
  for (i2 = 0; i2 < loop_ub; i2++) {
    P->data[i2] = 0.0;
  }

  // x, y, phi
  phi = r->data[1];

  // erster Punkt
  P->data[0] = 0.0;
  P->data[P->size[0]] = r->data[0];
  P->data[P->size[0] << 1] = r->data[1];

  // zweiter Punkt
  P->data[1] = P->data[0] + delta * cos(r->data[1]);
  P->data[1 + P->size[0]] = P->data[P->size[0]] + delta * sin(r->data[1]);
  P->data[1 + (P->size[0] << 1)] = r->data[1];
  for (s = 2; s - 2 <= r->size[0] - 3; s++) {
    //  "-" wegen VZ-Definition der Kruemmung
    phi = (phi - 2.0 * acos(-delta * r->data[s] / 2.0)) - 3.1415926535897931;
    P->data[s] = P->data[s - 1] + delta * cos(phi);
    P->data[s + P->size[0]] = P->data[(s + P->size[0]) - 1] + delta * sin(phi);
    P->data[s + (P->size[0] << 1)] = phi;
  }

  loop_ub = P->size[0];
  i2 = X->size[0];
  X->size[0] = loop_ub;
  emxEnsureCapacity((emxArray__common *)X, i2, (int)sizeof(double));
  for (i2 = 0; i2 < loop_ub; i2++) {
    X->data[i2] = P->data[i2];
  }

  loop_ub = P->size[0];
  i2 = Y->size[0];
  Y->size[0] = loop_ub;
  emxEnsureCapacity((emxArray__common *)Y, i2, (int)sizeof(double));
  for (i2 = 0; i2 < loop_ub; i2++) {
    Y->data[i2] = P->data[i2 + P->size[0]];
  }

  loop_ub = P->size[0];
  i2 = PHI->size[0];
  PHI->size[0] = loop_ub;
  emxEnsureCapacity((emxArray__common *)PHI, i2, (int)sizeof(double));
  for (i2 = 0; i2 < loop_ub; i2++) {
    PHI->data[i2] = P->data[i2 + (P->size[0] << 1)];
  }

  emxFree_real_T(&P);
}

//
// File trailer for getPointsFromState.cpp
//
// [EOF]
//
