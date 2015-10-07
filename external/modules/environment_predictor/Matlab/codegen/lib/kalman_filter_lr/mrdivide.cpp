//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: mrdivide.cpp
//
// MATLAB Coder version            : 3.0
// C/C++ source code generated on  : 07-Oct-2015 12:34:33
//

// Include Files
#include "rt_nonfinite.h"
#include "kalman_filter_lr.h"
#include "mrdivide.h"
#include "kalman_filter_lr_emxutil.h"
#include "lusolve.h"
#include "xgeqp3.h"

// Function Definitions

//
// Arguments    : const emxArray_real_T *A
//                const emxArray_real_T *B
//                emxArray_real_T *y
// Return Type  : void
//
void mrdivide(const emxArray_real_T *A, const emxArray_real_T *B,
              emxArray_real_T *y)
{
  emxArray_real_T *Y;
  emxArray_real_T *b_B;
  emxArray_real_T *b_A;
  emxArray_real_T *tau;
  emxArray_int32_T *jpvt;
  emxArray_real_T *c_B;
  unsigned int unnamed_idx_0;
  unsigned int unnamed_idx_1;
  int m;
  int i;
  int minmn;
  int maxmn;
  int rankR;
  double tol;
  int mn;
  emxInit_real_T(&Y, 2);
  emxInit_real_T(&b_B, 2);
  emxInit_real_T(&b_A, 2);
  emxInit_real_T1(&tau, 1);
  emxInit_int32_T(&jpvt, 2);
  emxInit_real_T(&c_B, 2);
  if ((A->size[0] == 0) || (A->size[1] == 0) || ((B->size[0] == 0) || (B->size[1]
        == 0))) {
    unnamed_idx_0 = (unsigned int)A->size[0];
    unnamed_idx_1 = (unsigned int)B->size[0];
    m = y->size[0] * y->size[1];
    y->size[0] = (int)unnamed_idx_0;
    y->size[1] = (int)unnamed_idx_1;
    emxEnsureCapacity((emxArray__common *)y, m, (int)sizeof(double));
    i = (int)unnamed_idx_0 * (int)unnamed_idx_1;
    for (m = 0; m < i; m++) {
      y->data[m] = 0.0;
    }
  } else if (B->size[0] == B->size[1]) {
    m = y->size[0] * y->size[1];
    y->size[0] = A->size[0];
    y->size[1] = A->size[1];
    emxEnsureCapacity((emxArray__common *)y, m, (int)sizeof(double));
    i = A->size[0] * A->size[1];
    for (m = 0; m < i; m++) {
      y->data[m] = A->data[m];
    }

    lusolve(B, y);
  } else {
    m = b_B->size[0] * b_B->size[1];
    b_B->size[0] = A->size[1];
    b_B->size[1] = A->size[0];
    emxEnsureCapacity((emxArray__common *)b_B, m, (int)sizeof(double));
    i = A->size[0];
    for (m = 0; m < i; m++) {
      minmn = A->size[1];
      for (maxmn = 0; maxmn < minmn; maxmn++) {
        b_B->data[maxmn + b_B->size[0] * m] = A->data[m + A->size[0] * maxmn];
      }
    }

    m = b_A->size[0] * b_A->size[1];
    b_A->size[0] = B->size[1];
    b_A->size[1] = B->size[0];
    emxEnsureCapacity((emxArray__common *)b_A, m, (int)sizeof(double));
    i = B->size[0];
    for (m = 0; m < i; m++) {
      minmn = B->size[1];
      for (maxmn = 0; maxmn < minmn; maxmn++) {
        b_A->data[maxmn + b_A->size[0] * m] = B->data[m + B->size[0] * maxmn];
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
    m = Y->size[0] * Y->size[1];
    Y->size[0] = minmn;
    Y->size[1] = maxmn;
    emxEnsureCapacity((emxArray__common *)Y, m, (int)sizeof(double));
    i = minmn * maxmn;
    for (m = 0; m < i; m++) {
      Y->data[m] = 0.0;
    }

    m = c_B->size[0] * c_B->size[1];
    c_B->size[0] = b_B->size[0];
    c_B->size[1] = b_B->size[1];
    emxEnsureCapacity((emxArray__common *)c_B, m, (int)sizeof(double));
    i = b_B->size[0] * b_B->size[1];
    for (m = 0; m < i; m++) {
      c_B->data[m] = b_B->data[m];
    }

    m = b_A->size[0];
    minmn = b_A->size[0];
    mn = b_A->size[1];
    if (minmn <= mn) {
      mn = minmn;
    }

    for (minmn = 0; minmn + 1 <= mn; minmn++) {
      if (tau->data[minmn] != 0.0) {
        for (maxmn = 0; maxmn + 1 <= b_B->size[1]; maxmn++) {
          tol = c_B->data[minmn + c_B->size[0] * maxmn];
          for (i = minmn + 1; i + 1 <= m; i++) {
            tol += b_A->data[i + b_A->size[0] * minmn] * c_B->data[i + c_B->
              size[0] * maxmn];
          }

          tol *= tau->data[minmn];
          if (tol != 0.0) {
            c_B->data[minmn + c_B->size[0] * maxmn] -= tol;
            for (i = minmn + 1; i + 1 <= m; i++) {
              c_B->data[i + c_B->size[0] * maxmn] -= b_A->data[i + b_A->size[0] *
                minmn] * tol;
            }
          }
        }
      }
    }

    for (maxmn = 0; maxmn + 1 <= b_B->size[1]; maxmn++) {
      for (i = 0; i + 1 <= rankR; i++) {
        Y->data[(jpvt->data[i] + Y->size[0] * maxmn) - 1] = c_B->data[i +
          c_B->size[0] * maxmn];
      }

      for (minmn = rankR - 1; minmn + 1 > 0; minmn--) {
        Y->data[(jpvt->data[minmn] + Y->size[0] * maxmn) - 1] /= b_A->data[minmn
          + b_A->size[0] * minmn];
        for (i = 0; i + 1 <= minmn; i++) {
          Y->data[(jpvt->data[i] + Y->size[0] * maxmn) - 1] -= Y->data
            [(jpvt->data[minmn] + Y->size[0] * maxmn) - 1] * b_A->data[i +
            b_A->size[0] * minmn];
        }
      }
    }

    m = y->size[0] * y->size[1];
    y->size[0] = Y->size[1];
    y->size[1] = Y->size[0];
    emxEnsureCapacity((emxArray__common *)y, m, (int)sizeof(double));
    i = Y->size[0];
    for (m = 0; m < i; m++) {
      minmn = Y->size[1];
      for (maxmn = 0; maxmn < minmn; maxmn++) {
        y->data[maxmn + y->size[0] * m] = Y->data[m + Y->size[0] * maxmn];
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
