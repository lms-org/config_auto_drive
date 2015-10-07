//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: state_transition.cpp
//
// MATLAB Coder version            : 3.0
// C/C++ source code generated on  : 07-Oct-2015 12:34:33
//

// Include Files
#include "rt_nonfinite.h"
#include "kalman_filter_lr.h"
#include "state_transition.h"
#include "kalman_filter_lr_emxutil.h"
#include "eye.h"

// Function Definitions

//
// Arguments    : emxArray_real_T *r
//                double delta
//                double delta_x
//                double delta_y
//                double delta_phi
//                unsigned char interp_mode
//                emxArray_real_T *A
// Return Type  : void
//
void state_transition(emxArray_real_T *r, double delta, double delta_x, double
                      delta_y, double delta_phi, unsigned char interp_mode,
                      emxArray_real_T *A)
{
  double phi0;
  double theta;
  double a;
  double c;
  int i;
  double d;
  emxArray_real_T *b;
  int i7;
  int ia;
  emxArray_real_T *b_A;
  int ar;
  int ib;
  int k;
  unsigned int A_idx_0;
  int m;
  int ic;
  int br;
  double x;
  phi0 = r->data[1];
  theta = (1.5707963267948966 + r->data[1]) - delta_phi;
  a = (r->data[0] - delta_y) + delta_x * tan(r->data[1]);
  c = delta_x / cos(r->data[1]) - a * sin(delta_phi) / sin(theta);
  r->data[0] = a * cos(r->data[1]) / sin(theta);
  r->data[1] -= delta_phi;
  eye((double)r->size[0], A);
  A->data[1 + (A->size[0] << 1)] = c / sqrt(1.0 - delta * delta * (r->data[2] *
    r->data[2]) / 4.0);
  if (interp_mode == 0) {
    for (i = 2; i - 2 < (int)(((double)r->size[0] - 1.0) + -2.0); i++) {
      // lineare Interpolation der Krümmungen
      A->data[i + A->size[0] * i] = 1.0 - c / delta;
      A->data[i + A->size[0] * ((int)((3.0 + (double)(i - 2)) + 1.0) - 1)] = c /
        delta;
    }

    A->data[(r->size[0] + A->size[0] * (r->size[0] - 1)) - 1] = 1.0;
  } else {
    d = c / delta;
    A->data[2 + (A->size[0] << 1)] = (0.5 * (d * d - d) + 1.0) - d * d;

    // Krümmung am ersten Punkt = Krümmung am zweiten Punkt
    A->data[2 + A->size[0] * 3] = 0.5 * (d * d + d);
    for (i = 3; i - 3 < (int)(((double)r->size[0] - 1.0) + -3.0); i++) {
      // quadratische Interpolation der Krümmungen
      A->data[i + A->size[0] * ((int)((4.0 + (double)(i - 3)) - 1.0) - 1)] = 0.5
        * (d * d - d);
      A->data[i + A->size[0] * i] = 1.0 - d * d;
      A->data[i + A->size[0] * ((int)((4.0 + (double)(i - 3)) + 1.0) - 1)] = 0.5
        * (d * d + d);
    }

    A->data[(r->size[0] + A->size[0] * (r->size[0] - 1)) - 1] = 1.0;
  }

  emxInit_real_T1(&b, 1);
  i7 = b->size[0];
  b->size[0] = r->size[0];
  emxEnsureCapacity((emxArray__common *)b, i7, (int)sizeof(double));
  ia = r->size[0];
  for (i7 = 0; i7 < ia; i7++) {
    b->data[i7] = r->data[i7];
  }

  emxInit_real_T1(&b_A, 1);
  if ((A->size[1] == 1) || (r->size[0] == 1)) {
    i7 = b_A->size[0];
    b_A->size[0] = A->size[0];
    emxEnsureCapacity((emxArray__common *)b_A, i7, (int)sizeof(double));
    ia = A->size[0];
    for (i7 = 0; i7 < ia; i7++) {
      b_A->data[i7] = 0.0;
      ar = A->size[1];
      for (ib = 0; ib < ar; ib++) {
        b_A->data[i7] += A->data[i7 + A->size[0] * ib] * r->data[ib];
      }
    }

    i7 = r->size[0];
    r->size[0] = b_A->size[0];
    emxEnsureCapacity((emxArray__common *)r, i7, (int)sizeof(double));
    ia = b_A->size[0];
    for (i7 = 0; i7 < ia; i7++) {
      r->data[i7] = b_A->data[i7];
    }
  } else {
    k = A->size[1];
    A_idx_0 = (unsigned int)A->size[0];
    i7 = r->size[0];
    r->size[0] = (int)A_idx_0;
    emxEnsureCapacity((emxArray__common *)r, i7, (int)sizeof(double));
    m = A->size[0];
    i7 = r->size[0];
    emxEnsureCapacity((emxArray__common *)r, i7, (int)sizeof(double));
    ia = r->size[0];
    for (i7 = 0; i7 < ia; i7++) {
      r->data[i7] = 0.0;
    }

    if (A->size[0] == 0) {
    } else {
      ar = 0;
      while ((m > 0) && (ar <= 0)) {
        for (ic = 1; ic <= m; ic++) {
          r->data[ic - 1] = 0.0;
        }

        ar = m;
      }

      br = 0;
      ar = 0;
      while ((m > 0) && (ar <= 0)) {
        ar = 0;
        i7 = br + k;
        for (ib = br; ib + 1 <= i7; ib++) {
          if (b->data[ib] != 0.0) {
            ia = ar;
            for (ic = 0; ic + 1 <= m; ic++) {
              ia++;
              r->data[ic] += b->data[ib] * A->data[ia - 1];
            }
          }

          ar += m;
        }

        br += k;
        ar = m;
      }
    }
  }

  emxFree_real_T(&b_A);
  emxFree_real_T(&b);
  A->data[0] = cos(phi0) / cos(phi0 - delta_phi);
  x = cos(delta_phi - phi0);
  A->data[A->size[0]] = ((delta_x * cos(delta_phi) + delta_y * sin(delta_phi)) -
    r->data[0] * sin(delta_phi)) / (x * x);
}

//
// File trailer for state_transition.cpp
//
// [EOF]
//
