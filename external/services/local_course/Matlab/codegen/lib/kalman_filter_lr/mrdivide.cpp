//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: mrdivide.cpp
//
// MATLAB Coder version            : 3.0
// C/C++ source code generated on  : 29-Nov-2015 11:30:09
//

// Include Files
#include "rt_nonfinite.h"
#include "kalman_filter_lr.h"
#include "mrdivide.h"
#include "kalman_filter_lr_emxutil.h"
#include "xzgetrf.h"
#include "xgeqp3.h"

// Function Definitions

//
// Arguments    : emxArray_real_T *A
//                const emxArray_real_T *B
// Return Type  : void
//
void mrdivide(emxArray_real_T *A, const emxArray_real_T *B)
{
  emxArray_real_T *Y;
  emxArray_real_T *b_B;
  emxArray_real_T *b_A;
  emxArray_real_T *tau;
  emxArray_int32_T *jpvt;
  emxArray_real_T *c_B;
  int minmn;
  int maxmn;
  int j;
  int mn;
  int rankR;
  int nb;
  int k;
  int i;
  double tol;
  emxInit_real_T(&Y, 2);
  emxInit_real_T(&b_B, 2);
  emxInit_real_T(&b_A, 2);
  emxInit_real_T1(&tau, 1);
  emxInit_int32_T(&jpvt, 2);
  emxInit_real_T(&c_B, 2);
  if ((A->size[0] == 0) || (A->size[1] == 0) || ((B->size[0] == 0) || (B->size[1]
        == 0))) {
    minmn = A->size[0];
    maxmn = B->size[0];
    j = A->size[0] * A->size[1];
    A->size[0] = minmn;
    A->size[1] = maxmn;
    emxEnsureCapacity((emxArray__common *)A, j, (int)sizeof(double));
    for (j = 0; j < maxmn; j++) {
      for (mn = 0; mn < minmn; mn++) {
        A->data[mn + A->size[0] * j] = 0.0;
      }
    }
  } else if (B->size[0] == B->size[1]) {
    rankR = B->size[1];
    j = b_A->size[0] * b_A->size[1];
    b_A->size[0] = B->size[0];
    b_A->size[1] = B->size[1];
    emxEnsureCapacity((emxArray__common *)b_A, j, (int)sizeof(double));
    maxmn = B->size[0] * B->size[1];
    for (j = 0; j < maxmn; j++) {
      b_A->data[j] = B->data[j];
    }

    xzgetrf(B->size[1], B->size[1], b_A, B->size[1], jpvt, &minmn);
    nb = A->size[0];
    if ((A->size[0] == 0) || (A->size[1] == 0)) {
    } else {
      for (j = 0; j + 1 <= rankR; j++) {
        minmn = nb * j - 1;
        maxmn = rankR * j;
        for (k = 1; k <= j; k++) {
          mn = nb * (k - 1);
          if (b_A->data[(k + maxmn) - 1] != 0.0) {
            for (i = 1; i <= nb; i++) {
              A->data[i + minmn] -= b_A->data[(k + maxmn) - 1] * A->data[(i + mn)
                - 1];
            }
          }
        }

        tol = 1.0 / b_A->data[j + maxmn];
        for (i = 1; i <= nb; i++) {
          A->data[i + minmn] *= tol;
        }
      }
    }

    if ((A->size[0] == 0) || (A->size[1] == 0)) {
    } else {
      for (j = B->size[1]; j > 0; j--) {
        minmn = nb * (j - 1) - 1;
        maxmn = rankR * (j - 1) - 1;
        for (k = j + 1; k <= rankR; k++) {
          mn = nb * (k - 1);
          if (b_A->data[k + maxmn] != 0.0) {
            for (i = 1; i <= nb; i++) {
              A->data[i + minmn] -= b_A->data[k + maxmn] * A->data[(i + mn) - 1];
            }
          }
        }
      }
    }

    for (minmn = B->size[1] - 2; minmn + 1 > 0; minmn--) {
      if (jpvt->data[minmn] != minmn + 1) {
        maxmn = jpvt->data[minmn] - 1;
        for (mn = 0; mn + 1 <= nb; mn++) {
          tol = A->data[mn + A->size[0] * minmn];
          A->data[mn + A->size[0] * minmn] = A->data[mn + A->size[0] * maxmn];
          A->data[mn + A->size[0] * maxmn] = tol;
        }
      }
    }
  } else {
    j = b_B->size[0] * b_B->size[1];
    b_B->size[0] = A->size[1];
    b_B->size[1] = A->size[0];
    emxEnsureCapacity((emxArray__common *)b_B, j, (int)sizeof(double));
    maxmn = A->size[0];
    for (j = 0; j < maxmn; j++) {
      minmn = A->size[1];
      for (mn = 0; mn < minmn; mn++) {
        b_B->data[mn + b_B->size[0] * j] = A->data[j + A->size[0] * mn];
      }
    }

    j = b_A->size[0] * b_A->size[1];
    b_A->size[0] = B->size[1];
    b_A->size[1] = B->size[0];
    emxEnsureCapacity((emxArray__common *)b_A, j, (int)sizeof(double));
    maxmn = B->size[0];
    for (j = 0; j < maxmn; j++) {
      minmn = B->size[1];
      for (mn = 0; mn < minmn; mn++) {
        b_A->data[mn + b_A->size[0] * j] = B->data[j + B->size[0] * mn];
      }
    }

    xgeqp3(b_A, tau, jpvt);
    rankR = 0;
    if (b_A->size[0] < b_A->size[1]) {
      minmn = b_A->size[0];
      maxmn = b_A->size[1];
    } else {
      minmn = b_A->size[1];
      maxmn = b_A->size[0];
    }

    if (minmn > 0) {
      tol = (double)maxmn * fabs(b_A->data[0]) * 2.2204460492503131E-16;
      while ((rankR < minmn) && (fabs(b_A->data[rankR + b_A->size[0] * rankR]) >=
              tol)) {
        rankR++;
      }
    }

    minmn = b_A->size[1];
    maxmn = b_B->size[1];
    j = Y->size[0] * Y->size[1];
    Y->size[0] = minmn;
    Y->size[1] = maxmn;
    emxEnsureCapacity((emxArray__common *)Y, j, (int)sizeof(double));
    maxmn *= minmn;
    for (j = 0; j < maxmn; j++) {
      Y->data[j] = 0.0;
    }

    j = c_B->size[0] * c_B->size[1];
    c_B->size[0] = b_B->size[0];
    c_B->size[1] = b_B->size[1];
    emxEnsureCapacity((emxArray__common *)c_B, j, (int)sizeof(double));
    maxmn = b_B->size[0] * b_B->size[1];
    for (j = 0; j < maxmn; j++) {
      c_B->data[j] = b_B->data[j];
    }

    maxmn = b_A->size[0];
    minmn = b_A->size[0];
    mn = b_A->size[1];
    if (minmn <= mn) {
      mn = minmn;
    }

    for (j = 0; j + 1 <= mn; j++) {
      if (tau->data[j] != 0.0) {
        for (k = 0; k + 1 <= b_B->size[1]; k++) {
          tol = c_B->data[j + c_B->size[0] * k];
          for (i = j + 1; i + 1 <= maxmn; i++) {
            tol += b_A->data[i + b_A->size[0] * j] * c_B->data[i + c_B->size[0] *
              k];
          }

          tol *= tau->data[j];
          if (tol != 0.0) {
            c_B->data[j + c_B->size[0] * k] -= tol;
            for (i = j + 1; i + 1 <= maxmn; i++) {
              c_B->data[i + c_B->size[0] * k] -= b_A->data[i + b_A->size[0] * j]
                * tol;
            }
          }
        }
      }
    }

    for (k = 0; k + 1 <= b_B->size[1]; k++) {
      for (i = 0; i + 1 <= rankR; i++) {
        Y->data[(jpvt->data[i] + Y->size[0] * k) - 1] = c_B->data[i + c_B->size
          [0] * k];
      }

      for (j = rankR - 1; j + 1 > 0; j--) {
        Y->data[(jpvt->data[j] + Y->size[0] * k) - 1] /= b_A->data[j + b_A->
          size[0] * j];
        for (i = 0; i + 1 <= j; i++) {
          Y->data[(jpvt->data[i] + Y->size[0] * k) - 1] -= Y->data[(jpvt->data[j]
            + Y->size[0] * k) - 1] * b_A->data[i + b_A->size[0] * j];
        }
      }
    }

    j = A->size[0] * A->size[1];
    A->size[0] = Y->size[1];
    A->size[1] = Y->size[0];
    emxEnsureCapacity((emxArray__common *)A, j, (int)sizeof(double));
    maxmn = Y->size[0];
    for (j = 0; j < maxmn; j++) {
      minmn = Y->size[1];
      for (mn = 0; mn < minmn; mn++) {
        A->data[mn + A->size[0] * j] = Y->data[j + Y->size[0] * mn];
      }
    }
  }

  emxFree_real_T(&c_B);
  emxFree_int32_T(&jpvt);
  emxFree_real_T(&tau);
  emxFree_real_T(&b_A);
  emxFree_real_T(&b_B);
  emxFree_real_T(&Y);
}

//
// File trailer for mrdivide.cpp
//
// [EOF]
//
