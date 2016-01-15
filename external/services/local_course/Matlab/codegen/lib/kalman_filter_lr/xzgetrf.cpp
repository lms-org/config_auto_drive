//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: xzgetrf.cpp
//
// MATLAB Coder version            : 3.0
// C/C++ source code generated on  : 29-Nov-2015 11:30:09
//

// Include Files
#include "rt_nonfinite.h"
#include "kalman_filter_lr.h"
#include "xzgetrf.h"
#include "colon.h"

// Function Definitions

//
// Arguments    : int m
//                int n
//                emxArray_real_T *A
//                int lda
//                emxArray_int32_T *ipiv
//                int *info
// Return Type  : void
//
void xzgetrf(int m, int n, emxArray_real_T *A, int lda, emxArray_int32_T *ipiv,
             int *info)
{
  int b_m;
  int i8;
  int j;
  int mmj;
  int c;
  int iy;
  int ix;
  double smax;
  int jA;
  double s;
  int i9;
  int jy;
  int b_j;
  int ijA;
  if (m <= n) {
    b_m = m;
  } else {
    b_m = n;
  }

  eml_signed_integer_colon(b_m, ipiv);
  *info = 0;
  if ((m < 1) || (n < 1)) {
  } else {
    if (m - 1 <= n) {
      i8 = m - 1;
    } else {
      i8 = n;
    }

    for (j = 0; j + 1 <= i8; j++) {
      mmj = m - j;
      c = j * (lda + 1);
      if (mmj < 1) {
        iy = -1;
      } else {
        iy = 0;
        if (mmj > 1) {
          ix = c;
          smax = fabs(A->data[c]);
          for (jA = 1; jA + 1 <= mmj; jA++) {
            ix++;
            s = fabs(A->data[ix]);
            if (s > smax) {
              iy = jA;
              smax = s;
            }
          }
        }
      }

      if (A->data[c + iy] != 0.0) {
        if (iy != 0) {
          ipiv->data[j] = (j + iy) + 1;
          ix = j;
          iy += j;
          for (jA = 1; jA <= n; jA++) {
            smax = A->data[ix];
            A->data[ix] = A->data[iy];
            A->data[iy] = smax;
            ix += lda;
            iy += lda;
          }
        }

        i9 = c + mmj;
        for (iy = c + 1; iy + 1 <= i9; iy++) {
          A->data[iy] /= A->data[c];
        }
      } else {
        *info = j + 1;
      }

      iy = (n - j) - 1;
      jA = c + lda;
      jy = c + lda;
      for (b_j = 1; b_j <= iy; b_j++) {
        smax = A->data[jy];
        if (A->data[jy] != 0.0) {
          ix = c + 1;
          i9 = mmj + jA;
          for (ijA = 1 + jA; ijA + 1 <= i9; ijA++) {
            A->data[ijA] += A->data[ix] * -smax;
            ix++;
          }
        }

        jy += lda;
        jA += lda;
      }
    }

    if ((*info == 0) && (m <= n) && (!(A->data[(m + A->size[0] * (m - 1)) - 1]
          != 0.0))) {
      *info = m;
    }
  }
}

//
// File trailer for xzgetrf.cpp
//
// [EOF]
//
