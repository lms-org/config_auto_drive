/*
 * File: mrdivide.c
 *
 * MATLAB Coder version            : 2.7
 * C/C++ source code generated on  : 27-May-2015 19:40:39
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "kalman_filter_lr.h"
#include "mrdivide.h"

/* Function Declarations */
static void b_eml_xtrsm(const double A[1600], double B[400]);
static void eml_xswap(double x[1600], int ix0, int iy0);
static void eml_xtrsm(const double A[1600], double B[400]);

/* Function Definitions */

/*
 * Arguments    : const double A[1600]
 *                double B[400]
 * Return Type  : void
 */
static void b_eml_xtrsm(const double A[1600], double B[400])
{
  int j;
  int jBcol;
  int jAcol;
  int k;
  int kBcol;
  int i;
  for (j = 39; j > -1; j += -1) {
    jBcol = 10 * j;
    jAcol = 40 * j - 1;
    for (k = j + 2; k < 41; k++) {
      kBcol = 10 * (k - 1);
      if (A[k + jAcol] != 0.0) {
        for (i = 0; i < 10; i++) {
          B[i + jBcol] -= A[k + jAcol] * B[i + kBcol];
        }
      }
    }
  }
}

/*
 * Arguments    : double x[1600]
 *                int ix0
 *                int iy0
 * Return Type  : void
 */
static void eml_xswap(double x[1600], int ix0, int iy0)
{
  int ix;
  int iy;
  int k;
  double temp;
  ix = ix0 - 1;
  iy = iy0 - 1;
  for (k = 0; k < 40; k++) {
    temp = x[ix];
    x[ix] = x[iy];
    x[iy] = temp;
    ix += 40;
    iy += 40;
  }
}

/*
 * Arguments    : const double A[1600]
 *                double B[400]
 * Return Type  : void
 */
static void eml_xtrsm(const double A[1600], double B[400])
{
  int j;
  int jBcol;
  int jAcol;
  int k;
  int kBcol;
  int i;
  double temp;
  for (j = 0; j < 40; j++) {
    jBcol = 10 * j;
    jAcol = 40 * j;
    for (k = 0; k + 1 <= j; k++) {
      kBcol = 10 * k;
      if (A[k + jAcol] != 0.0) {
        for (i = 0; i < 10; i++) {
          B[i + jBcol] -= A[k + jAcol] * B[i + kBcol];
        }
      }
    }

    temp = 1.0 / A[j + jAcol];
    for (i = 0; i < 10; i++) {
      B[i + jBcol] *= temp;
    }
  }
}

/*
 * Arguments    : const double A[400]
 *                const double B[1600]
 *                double y[400]
 * Return Type  : void
 */
void mrdivide(const double A[400], const double B[1600], double y[400])
{
  double b_A[1600];
  signed char ipiv[40];
  int i1;
  int j;
  int c;
  int jA;
  int ix;
  double temp;
  int jp;
  double s;
  int xi;
  int ijA;
  memcpy(&b_A[0], &B[0], 1600U * sizeof(double));
  for (i1 = 0; i1 < 40; i1++) {
    ipiv[i1] = (signed char)(1 + i1);
  }

  for (j = 0; j < 39; j++) {
    c = j * 41;
    jA = 1;
    ix = c;
    temp = fabs(b_A[c]);
    for (jp = 2; jp <= 40 - j; jp++) {
      ix++;
      s = fabs(b_A[ix]);
      if (s > temp) {
        jA = jp;
        temp = s;
      }
    }

    if (b_A[(c + jA) - 1] != 0.0) {
      if (jA - 1 != 0) {
        ipiv[j] = (signed char)(j + jA);
        eml_xswap(b_A, j + 1, j + jA);
      }

      i1 = (c - j) + 40;
      for (jA = c + 1; jA + 1 <= i1; jA++) {
        b_A[jA] /= b_A[c];
      }
    }

    jA = c;
    jp = c + 40;
    for (xi = 1; xi <= 39 - j; xi++) {
      temp = b_A[jp];
      if (b_A[jp] != 0.0) {
        ix = c + 1;
        i1 = (jA - j) + 80;
        for (ijA = 41 + jA; ijA + 1 <= i1; ijA++) {
          b_A[ijA] += b_A[ix] * -temp;
          ix++;
        }
      }

      jp += 40;
      jA += 40;
    }
  }

  memcpy(&y[0], &A[0], 400U * sizeof(double));
  eml_xtrsm(b_A, y);
  b_eml_xtrsm(b_A, y);
  for (jA = 38; jA > -1; jA += -1) {
    if (ipiv[jA] != jA + 1) {
      jp = ipiv[jA] - 1;
      for (xi = 0; xi < 10; xi++) {
        temp = y[xi + 10 * jA];
        y[xi + 10 * jA] = y[xi + 10 * jp];
        y[xi + 10 * jp] = temp;
      }
    }
  }
}

/*
 * File trailer for mrdivide.c
 *
 * [EOF]
 */
