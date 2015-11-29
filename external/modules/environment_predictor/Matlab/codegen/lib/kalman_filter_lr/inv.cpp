//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: inv.cpp
//
// MATLAB Coder version            : 3.0
// C/C++ source code generated on  : 29-Nov-2015 11:30:09
//

// Include Files
#include "rt_nonfinite.h"
#include "kalman_filter_lr.h"
#include "inv.h"
#include "kalman_filter_lr_emxutil.h"
#include "colon.h"
#include "xzgetrf.h"

// Function Definitions

//
// Arguments    : const emxArray_real_T *x
//                emxArray_real_T *y
// Return Type  : void
//
void inv(const emxArray_real_T *x, emxArray_real_T *y)
{
  int jBcol;
  int kAcol;
  int n;
  emxArray_real_T *b_x;
  emxArray_int32_T *p;
  emxArray_int32_T *ipiv;
  int k;
  int j;
  int i;
  double c_x;
  double b_y;
  if ((x->size[0] == 0) || (x->size[1] == 0)) {
    jBcol = y->size[0] * y->size[1];
    y->size[0] = x->size[0];
    y->size[1] = x->size[1];
    emxEnsureCapacity((emxArray__common *)y, jBcol, (int)sizeof(double));
    kAcol = x->size[0] * x->size[1];
    for (jBcol = 0; jBcol < kAcol; jBcol++) {
      y->data[jBcol] = x->data[jBcol];
    }
  } else {
    n = x->size[0];
    jBcol = y->size[0] * y->size[1];
    y->size[0] = x->size[0];
    y->size[1] = x->size[1];
    emxEnsureCapacity((emxArray__common *)y, jBcol, (int)sizeof(double));
    kAcol = x->size[0] * x->size[1];
    for (jBcol = 0; jBcol < kAcol; jBcol++) {
      y->data[jBcol] = 0.0;
    }

    emxInit_real_T(&b_x, 2);
    jBcol = b_x->size[0] * b_x->size[1];
    b_x->size[0] = x->size[0];
    b_x->size[1] = x->size[1];
    emxEnsureCapacity((emxArray__common *)b_x, jBcol, (int)sizeof(double));
    kAcol = x->size[0] * x->size[1];
    for (jBcol = 0; jBcol < kAcol; jBcol++) {
      b_x->data[jBcol] = x->data[jBcol];
    }

    emxInit_int32_T(&p, 2);
    emxInit_int32_T(&ipiv, 2);
    xzgetrf(x->size[0], x->size[0], b_x, x->size[0], ipiv, &jBcol);
    eml_signed_integer_colon(x->size[0], p);
    for (k = 0; k < ipiv->size[1]; k++) {
      if (ipiv->data[k] > 1 + k) {
        jBcol = p->data[ipiv->data[k] - 1];
        p->data[ipiv->data[k] - 1] = p->data[k];
        p->data[k] = jBcol;
      }
    }

    emxFree_int32_T(&ipiv);
    for (k = 0; k + 1 <= n; k++) {
      kAcol = p->data[k] - 1;
      y->data[k + y->size[0] * (p->data[k] - 1)] = 1.0;
      for (j = k; j + 1 <= n; j++) {
        if (y->data[j + y->size[0] * kAcol] != 0.0) {
          for (i = j + 1; i + 1 <= n; i++) {
            y->data[i + y->size[0] * kAcol] -= y->data[j + y->size[0] * kAcol] *
              b_x->data[i + b_x->size[0] * j];
          }
        }
      }
    }

    emxFree_int32_T(&p);
    for (j = 1; j <= n; j++) {
      jBcol = n * (j - 1);
      for (k = n - 1; k + 1 > 0; k--) {
        kAcol = n * k;
        if (y->data[k + jBcol] != 0.0) {
          c_x = y->data[k + jBcol];
          b_y = b_x->data[k + kAcol];
          y->data[k + jBcol] = c_x / b_y;
          for (i = 0; i + 1 <= k; i++) {
            y->data[i + jBcol] -= y->data[k + jBcol] * b_x->data[i + kAcol];
          }
        }
      }
    }

    emxFree_real_T(&b_x);
  }
}

//
// File trailer for inv.cpp
//
// [EOF]
//
