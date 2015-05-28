/*
 * File: messmatrix_new.c
 *
 * MATLAB Coder version            : 2.7
 * C/C++ source code generated on  : 27-May-2015 19:40:39
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "kalman_filter_lr.h"
#include "messmatrix_new.h"

/* Function Definitions */

/*
 * Arguments    : const double P[30]
 *                const double r[10]
 *                double delta
 *                const double ind[10]
 *                const double lambda[10]
 *                double H[200]
 * Return Type  : void
 */
void messmatrix_new(const double P[30], const double r[10], double delta, const
                    double ind[10], const double lambda[10], double H[200])
{
  double d_phi0[20];
  int count;
  double d_c_x[100];
  double d_c_y[100];
  int c;
  int i;

  /*  d ist delta, bei regelm‰ﬂigen Abst‰nden */
  /* Ableitungen nach phi0 */
  memset(&d_phi0[0], 0, 20U * sizeof(double));
  for (count = 0; count < 9; count++) {
    /*      d = distance(P(p-1, 1),P(p-1, 2),P(p, 1),P(p, 2)); */
    d_phi0[count + 1] = d_phi0[count] - delta * sin(P[count + 21]);

    /* x-Komponenten */
    d_phi0[count + 11] = d_phi0[10 + count] + delta * cos(P[count + 21]);

    /* y-Komponenten */
  }

  /* Ableitungen nach den Kr¸mmungen c_i */
  for (count = 0; count < 100; count++) {
    d_c_x[count] = 0.0;

    /* x-Komponenten */
    d_c_y[count] = 0.0;
  }

  /* y-Komponenten */
  for (count = 0; count < 8; count++) {
    for (c = 0; c < 8; c++) {
      if (3 + count > 2 + c) {
        /*              d = distance(P(p-1, 1),P(p-1, 2),P(p, 1),P(p, 2)); */
        d_c_x[(count + 10 * (c + 1)) + 2] = d_c_x[(count + 10 * (c + 1)) + 1] -
          delta * delta * sin(P[count + 22]) / sqrt(1.0 - delta * delta * (r[2 +
          c] * r[2 + c]) / 4.0);
        d_c_y[(count + 10 * (c + 1)) + 2] = d_c_y[(count + 10 * (c + 1)) + 1] +
          delta * delta * cos(P[count + 22]) / sqrt(1.0 - delta * delta * (r[2 +
          c] * r[2 + c]) / 4.0);
      }
    }
  }

  /* Messmatrix zusammensetzen */
  memset(&H[0], 0, 200U * sizeof(double));
  count = 0;
  for (i = 0; i < 10; i++) {
    /* alle Messpunkteiterieren */
    H[count] = 0.0;

    /* Ableitung nach y0 */
    H[count + 1] = 1.0;
    H[20 + count] = d_phi0[(int)ind[i] - 1];

    /* Ableitung nach phi0 */
    H[count + 21] = d_phi0[(int)ind[i] + 9];
    for (c = 0; c < (int)(ind[i] + -2.0); c++) {
      /* Ableitung nach Kr¸mmungen c */
      /*          d = distance(P(ind(i)-1, 1),P(ind(i)-1, 2),P(ind(i), 1),P(ind(i), 2)); */
      if (3.0 + (double)c == ind[i]) {
        H[count + 20 * (c + 2)] = d_c_x[((int)(ind[i] - 1.0) + 10 * ((int)((3.0
          + (double)c) - 1.0) - 1)) - 1] - lambda[i] * (delta * delta) * sin(P
          [(int)ind[i] + 19]) / sqrt(1.0 - delta * delta * (r[(int)((3.0 +
          (double)c) - 1.0) - 1] * r[(int)((3.0 + (double)c) - 1.0) - 1]) / 4.0);
        H[(count + 20 * (c + 2)) + 1] = d_c_y[((int)(ind[i] - 1.0) + 10 * ((int)
          ((3.0 + (double)c) - 1.0) - 1)) - 1] + lambda[i] * (delta * delta) *
          cos(P[(int)ind[i] + 19]) / sqrt(1.0 - delta * delta * (r[(int)((3.0 +
          (double)c) - 1.0) - 1] * r[(int)((3.0 + (double)c) - 1.0) - 1]) / 4.0);
      } else {
        H[count + 20 * (c + 2)] = d_c_x[((int)ind[i] + 10 * ((int)((3.0 +
          (double)c) - 1.0) - 1)) - 1];
        H[(count + 20 * (c + 2)) + 1] = d_c_y[((int)ind[i] + 10 * ((int)((3.0 +
          (double)c) - 1.0) - 1)) - 1];
      }
    }

    count += 2;
  }
}

/*
 * File trailer for messmatrix_new.c
 *
 * [EOF]
 */
