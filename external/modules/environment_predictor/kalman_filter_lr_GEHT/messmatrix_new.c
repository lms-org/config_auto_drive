/*
 * File: messmatrix_new.c
 *
 * MATLAB Coder version            : 2.7
 * C/C++ source code generated on  : 28-May-2015 15:37:30
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "kalman_filter_lr.h"
#include "messmatrix_new.h"
#include "kalman_filter_lr_emxutil.h"

/* Function Definitions */

/*
 * Arguments    : const double P[30]
 *                const double r[10]
 *                double delta
 *                const emxArray_real_T *ind
 *                const emxArray_real_T *lambda
 *                emxArray_real_T *H
 * Return Type  : void
 */
void messmatrix_new(const double P[30], const double r[10], double delta, const
                    emxArray_real_T *ind, const emxArray_real_T *lambda,
                    emxArray_real_T *H)
{
  double d_phi0[20];
  int p;
  double d_c_x[100];
  double d_c_y[100];
  int c;
  unsigned int count;

  /* Ableitungen nach phi0 */
  memset(&d_phi0[0], 0, 20U * sizeof(double));
  for (p = 0; p < 9; p++) {
    d_phi0[p + 1] = d_phi0[p] - delta * sin(P[p + 21]);

    /* x-Komponenten */
    d_phi0[p + 11] = d_phi0[10 + p] + delta * cos(P[p + 21]);

    /* y-Komponenten */
  }

  /* Ableitungen nach den Krümmungen c_i */
  for (c = 0; c < 100; c++) {
    d_c_x[c] = 0.0;

    /* x-Komponenten */
    d_c_y[c] = 0.0;
  }

  /* y-Komponenten */
  for (p = 0; p < 8; p++) {
    for (c = 0; c < 8; c++) {
      if (3 + p > 2 + c) {
        d_c_x[(p + 10 * (c + 1)) + 2] = d_c_x[(p + 10 * (c + 1)) + 1] - delta *
          delta * sin(P[p + 22]) / sqrt(1.0 - delta * delta * (r[2 + c] * r[2 +
          c]) / 4.0);
        d_c_y[(p + 10 * (c + 1)) + 2] = d_c_y[(p + 10 * (c + 1)) + 1] + delta *
          delta * cos(P[p + 22]) / sqrt(1.0 - delta * delta * (r[2 + c] * r[2 +
          c]) / 4.0);
      }
    }
  }

  /* Messmatrix zusammensetzen */
  p = (int)(2.0 * (double)lambda->size[0]);
  c = H->size[0] * H->size[1];
  H->size[0] = p;
  H->size[1] = 10;
  emxEnsureCapacity((emxArray__common *)H, c, (int)sizeof(double));
  p = (int)(2.0 * (double)lambda->size[0]) * 10;
  for (c = 0; c < p; c++) {
    H->data[c] = 0.0;
  }

  count = 2U;
  for (p = 0; p < ind->size[0]; p++) {
    /* alle Messpunkte iterieren */
    H->data[(int)count - 2] = 0.0;

    /* Ableitung nach y0 */
    H->data[(int)count - 1] = 1.0;
    H->data[((int)count + H->size[0]) - 2] = d_phi0[(int)ind->data[p] - 1];

    /* Ableitung nach phi0 */
    H->data[((int)count + H->size[0]) - 1] = d_phi0[(int)ind->data[p] + 9];
    for (c = 0; c < (int)(ind->data[p] + -2.0); c++) {
      /* Ableitung nach Krümmungen c */
      if (3.0 + (double)c == ind->data[p]) {
        H->data[((int)count + H->size[0] * (c + 2)) - 2] = d_c_x[((int)
          (ind->data[p] - 1.0) + 10 * ((int)((3.0 + (double)c) - 1.0) - 1)) - 1]
          - lambda->data[p] * (delta * delta) * sin(P[(int)ind->data[p] + 19]) /
          sqrt(1.0 - delta * delta * (r[(int)((3.0 + (double)c) - 1.0) - 1] * r
                [(int)((3.0 + (double)c) - 1.0) - 1]) / 4.0);
        H->data[((int)count + H->size[0] * (c + 2)) - 1] = d_c_y[((int)
          (ind->data[p] - 1.0) + 10 * ((int)((3.0 + (double)c) - 1.0) - 1)) - 1]
          + lambda->data[p] * (delta * delta) * cos(P[(int)ind->data[p] + 19]) /
          sqrt(1.0 - delta * delta * (r[(int)((3.0 + (double)c) - 1.0) - 1] * r
                [(int)((3.0 + (double)c) - 1.0) - 1]) / 4.0);
      } else {
        H->data[((int)count + H->size[0] * (c + 2)) - 2] = d_c_x[((int)ind->
          data[p] + 10 * ((int)((3.0 + (double)c) - 1.0) - 1)) - 1];
        H->data[((int)count + H->size[0] * (c + 2)) - 1] = d_c_y[((int)ind->
          data[p] + 10 * ((int)((3.0 + (double)c) - 1.0) - 1)) - 1];
      }
    }

    count += 2U;
  }
}

/*
 * File trailer for messmatrix_new.c
 *
 * [EOF]
 */
