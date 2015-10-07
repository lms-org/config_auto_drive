//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: lusolve.cpp
//
// MATLAB Coder version            : 3.0
// C/C++ source code generated on  : 07-Oct-2015 12:34:33
//

// Include Files
#include "rt_nonfinite.h"
#include "kalman_filter_lr.h"
#include "lusolve.h"
#include "kalman_filter_lr_emxutil.h"
#include "colon.h"

// Function Definitions

//
// Arguments    : const emxArray_real_T *A
//                emxArray_real_T *B
// Return Type  : void
//
void lusolve(const emxArray_real_T *A, emxArray_real_T *B)
{
  emxArray_real_T *b_A;
  int n;
  int i8;
  int iy;
  emxArray_int32_T *ipiv;
  int j;
  int mmj;
  int c;
  int ix;
  double smax;
  int k;
  double s;
  int i;
  int jA;
  int jy;
  int nb;
  emxInit_real_T(&b_A, 2);
  n = A->size[1];
  i8 = b_A->size[0] * b_A->size[1];
  b_A->size[0] = A->size[0];
  b_A->size[1] = A->size[1];
  emxEnsureCapacity((emxArray__common *)b_A, i8, (int)sizeof(double));
  iy = A->size[0] * A->size[1];
  for (i8 = 0; i8 < iy; i8++) {
    b_A->data[i8] = A->data[i8];
  }

  emxInit_int32_T(&ipiv, 2);
  iy = A->size[1];
  eml_signed_integer_colon(iy, ipiv);
  if (A->size[1] < 1) {
  } else {
    if (A->size[1] - 1 <= A->size[1]) {
      i8 = A->size[1] - 1;
    } else {
      i8 = A->size[1];
    }

    for (j = 0; j + 1 <= i8; j++) {
      mmj = n - j;
      c = j * (n + 1);
      if (mmj < 1) {
        iy = -1;
      } else {
        iy = 0;
        if (mmj > 1) {
          ix = c;
          smax = fabs(b_A->data[c]);
          for (k = 1; k + 1 <= mmj; k++) {
            ix++;
            s = fabs(b_A->data[ix]);
            if (s > smax) {
              iy = k;
              smax = s;
            }
          }
        }
      }

      if (b_A->data[c + iy] != 0.0) {
        if (iy != 0) {
          ipiv->data[j] = (j + iy) + 1;
          ix = j;
          iy += j;
          for (k = 1; k <= n; k++) {
            smax = b_A->data[ix];
            b_A->data[ix] = b_A->data[iy];
            b_A->data[iy] = smax;
            ix += n;
            iy += n;
          }
        }

        k = c + mmj;
        for (i = c + 1; i + 1 <= k; i++) {
          b_A->data[i] /= b_A->data[c];
        }
      }

      iy = (n - j) - 1;
      jA = c + n;
      jy = c + n;
      for (nb = 1; nb <= iy; nb++) {
        smax = b_A->data[jy];
        if (b_A->data[jy] != 0.0) {
          ix = c + 1;
          k = mmj + jA;
          for (i = 1 + jA; i + 1 <= k; i++) {
            b_A->data[i] += b_A->data[ix] * -smax;
            ix++;
          }
        }

        jy += n;
        jA += n;
      }
    }
  }

  nb = B->size[0];
  if ((A->size[1] == 0) || ((B->size[0] == 0) || (B->size[1] == 0))) {
  } else {
    for (j = 0; j + 1 <= n; j++) {
      jA = nb * j;
      jy = n * j;
      for (k = 0; k + 1 <= j; k++) {
        iy = nb * k;
        if (b_A->data[k + jy] != 0.0) {
          for (i = 0; i + 1 <= nb; i++) {
            B->data[i + jA] -= b_A->data[k + jy] * B->data[i + iy];
          }
        }
      }

      smax = 1.0 / b_A->data[j + jy];
      for (i = 0; i + 1 <= nb; i++) {
        B->data[i + jA] *= smax;
      }
    }
  }

  if ((A->size[1] == 0) || ((B->size[0] == 0) || (B->size[1] == 0))) {
  } else {
    for (j = A->size[1]; j > 0; j--) {
      jA = nb * (j - 1);
      jy = n * (j - 1);
      for (k = j; k + 1 <= n; k++) {
        iy = nb * k;
        if (b_A->data[k + jy] != 0.0) {
          for (i = 0; i + 1 <= nb; i++) {
            B->data[i + jA] -= b_A->data[k + jy] * B->data[i + iy];
          }
        }
      }
    }
  }

  emxFree_real_T(&b_A);
  for (iy = A->size[1] - 2; iy + 1 > 0; iy--) {
    if (ipiv->data[iy] != iy + 1) {
      jA = ipiv->data[iy] - 1;
      for (jy = 0; jy + 1 <= nb; jy++) {
        smax = B->data[jy + B->size[0] * iy];
        B->data[jy + B->size[0] * iy] = B->data[jy + B->size[0] * jA];
        B->data[jy + B->size[0] * jA] = smax;
      }
    }
  }

  emxFree_int32_T(&ipiv);
}

//
// File trailer for lusolve.cpp
//
// [EOF]
//
