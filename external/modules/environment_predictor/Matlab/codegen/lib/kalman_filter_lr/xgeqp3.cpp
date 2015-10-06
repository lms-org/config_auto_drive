//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: xgeqp3.cpp
//
// MATLAB Coder version            : 3.0
// C/C++ source code generated on  : 06-Oct-2015 19:14:10
//

// Include Files
#include "rt_nonfinite.h"
#include "kalman_filter_lr.h"
#include "xgeqp3.h"
#include "xnrm2.h"
#include "xscal.h"
#include "kalman_filter_lr_emxutil.h"
#include "colon.h"

// Function Declarations
static double rt_hypotd_snf(double u0, double u1);

// Function Definitions

//
// Arguments    : double u0
//                double u1
// Return Type  : double
//
static double rt_hypotd_snf(double u0, double u1)
{
  double y;
  double a;
  double b;
  a = fabs(u0);
  b = fabs(u1);
  if (a < b) {
    a /= b;
    y = b * sqrt(a * a + 1.0);
  } else if (a > b) {
    b /= a;
    y = a * sqrt(b * b + 1.0);
  } else if (rtIsNaN(b)) {
    y = b;
  } else {
    y = a * 1.4142135623730951;
  }

  return y;
}

//
// Arguments    : emxArray_real_T *A
//                emxArray_real_T *tau
//                emxArray_int32_T *jpvt
// Return Type  : void
//
void xgeqp3(emxArray_real_T *A, emxArray_real_T *tau, emxArray_int32_T *jpvt)
{
  int m;
  int n;
  int mn;
  int i10;
  emxArray_real_T *work;
  int itemp;
  emxArray_real_T *vn1;
  emxArray_real_T *vn2;
  int k;
  int nmi;
  int i;
  int i_i;
  int mmi;
  int ix;
  double smax;
  double s;
  int pvt;
  int iy;
  double absxk;
  int i_ip1;
  int lastv;
  int lastc;
  boolean_T exitg2;
  int exitg1;
  double t;
  m = A->size[0];
  n = A->size[1];
  if (A->size[0] <= A->size[1]) {
    mn = A->size[0];
  } else {
    mn = A->size[1];
  }

  i10 = tau->size[0];
  tau->size[0] = mn;
  emxEnsureCapacity((emxArray__common *)tau, i10, (int)sizeof(double));
  eml_signed_integer_colon(A->size[1], jpvt);
  if ((A->size[0] == 0) || (A->size[1] == 0)) {
  } else {
    emxInit_real_T1(&work, 1);
    itemp = A->size[1];
    i10 = work->size[0];
    work->size[0] = itemp;
    emxEnsureCapacity((emxArray__common *)work, i10, (int)sizeof(double));
    for (i10 = 0; i10 < itemp; i10++) {
      work->data[i10] = 0.0;
    }

    emxInit_real_T1(&vn1, 1);
    emxInit_real_T1(&vn2, 1);
    itemp = A->size[1];
    i10 = vn1->size[0];
    vn1->size[0] = itemp;
    emxEnsureCapacity((emxArray__common *)vn1, i10, (int)sizeof(double));
    i10 = vn2->size[0];
    vn2->size[0] = vn1->size[0];
    emxEnsureCapacity((emxArray__common *)vn2, i10, (int)sizeof(double));
    k = 1;
    for (nmi = 0; nmi + 1 <= n; nmi++) {
      vn1->data[nmi] = xnrm2(m, A, k);
      vn2->data[nmi] = vn1->data[nmi];
      k += m;
    }

    for (i = 0; i + 1 <= mn; i++) {
      i_i = i + i * m;
      nmi = n - i;
      mmi = (m - i) - 1;
      if (nmi < 1) {
        itemp = -1;
      } else {
        itemp = 0;
        if (nmi > 1) {
          ix = i;
          smax = fabs(vn1->data[i]);
          for (k = 0; k + 2 <= nmi; k++) {
            ix++;
            s = fabs(vn1->data[ix]);
            if (s > smax) {
              itemp = k + 1;
              smax = s;
            }
          }
        }
      }

      pvt = i + itemp;
      if (pvt + 1 != i + 1) {
        ix = m * pvt;
        iy = m * i;
        for (k = 1; k <= m; k++) {
          smax = A->data[ix];
          A->data[ix] = A->data[iy];
          A->data[iy] = smax;
          ix++;
          iy++;
        }

        itemp = jpvt->data[pvt];
        jpvt->data[pvt] = jpvt->data[i];
        jpvt->data[i] = itemp;
        vn1->data[pvt] = vn1->data[i];
        vn2->data[pvt] = vn2->data[i];
      }

      if (i + 1 < m) {
        absxk = A->data[i_i];
        s = 0.0;
        if (1 + mmi <= 0) {
        } else {
          smax = b_xnrm2(mmi, A, i_i + 2);
          if (smax != 0.0) {
            smax = rt_hypotd_snf(A->data[i_i], smax);
            if (A->data[i_i] >= 0.0) {
              smax = -smax;
            }

            if (fabs(smax) < 1.0020841800044864E-292) {
              pvt = 0;
              do {
                pvt++;
                xscal(mmi, 9.9792015476736E+291, A, i_i + 2);
                smax *= 9.9792015476736E+291;
                absxk *= 9.9792015476736E+291;
              } while (!(fabs(smax) >= 1.0020841800044864E-292));

              smax = b_xnrm2(mmi, A, i_i + 2);
              smax = rt_hypotd_snf(absxk, smax);
              if (absxk >= 0.0) {
                smax = -smax;
              }

              s = (smax - absxk) / smax;
              xscal(mmi, 1.0 / (absxk - smax), A, i_i + 2);
              for (k = 1; k <= pvt; k++) {
                smax *= 1.0020841800044864E-292;
              }

              absxk = smax;
            } else {
              s = (smax - A->data[i_i]) / smax;
              xscal(mmi, 1.0 / (A->data[i_i] - smax), A, i_i + 2);
              absxk = smax;
            }
          }
        }

        tau->data[i] = s;
        A->data[i_i] = absxk;
      } else {
        tau->data[i] = 0.0;
      }

      if (i + 1 < n) {
        absxk = A->data[i_i];
        A->data[i_i] = 1.0;
        i_ip1 = (i + (i + 1) * m) + 1;
        if (tau->data[i] != 0.0) {
          lastv = mmi;
          itemp = i_i + mmi;
          while ((lastv + 1 > 0) && (A->data[itemp] == 0.0)) {
            lastv--;
            itemp--;
          }

          lastc = nmi - 1;
          exitg2 = false;
          while ((!exitg2) && (lastc > 0)) {
            itemp = i_ip1 + (lastc - 1) * m;
            k = itemp;
            do {
              exitg1 = 0;
              if (k <= itemp + lastv) {
                if (A->data[k - 1] != 0.0) {
                  exitg1 = 1;
                } else {
                  k++;
                }
              } else {
                lastc--;
                exitg1 = 2;
              }
            } while (exitg1 == 0);

            if (exitg1 == 1) {
              exitg2 = true;
            }
          }
        } else {
          lastv = -1;
          lastc = 0;
        }

        if (lastv + 1 > 0) {
          if (lastc == 0) {
          } else {
            for (iy = 1; iy <= lastc; iy++) {
              work->data[iy - 1] = 0.0;
            }

            iy = 0;
            i10 = i_ip1 + m * (lastc - 1);
            itemp = i_ip1;
            while ((m > 0) && (itemp <= i10)) {
              ix = i_i;
              smax = 0.0;
              pvt = itemp + lastv;
              for (k = itemp; k <= pvt; k++) {
                smax += A->data[k - 1] * A->data[ix];
                ix++;
              }

              work->data[iy] += smax;
              iy++;
              itemp += m;
            }
          }

          if (-tau->data[i] == 0.0) {
          } else {
            itemp = 0;
            for (nmi = 1; nmi <= lastc; nmi++) {
              if (work->data[itemp] != 0.0) {
                smax = work->data[itemp] * -tau->data[i];
                ix = i_i;
                i10 = lastv + i_ip1;
                for (pvt = i_ip1; pvt <= i10; pvt++) {
                  A->data[pvt - 1] += A->data[ix] * smax;
                  ix++;
                }
              }

              itemp++;
              i_ip1 += m;
            }
          }
        }

        A->data[i_i] = absxk;
      }

      for (nmi = i + 1; nmi + 1 <= n; nmi++) {
        itemp = (i + m * nmi) + 1;
        if (vn1->data[nmi] != 0.0) {
          smax = fabs(A->data[i + A->size[0] * nmi]) / vn1->data[nmi];
          smax = 1.0 - smax * smax;
          if (smax < 0.0) {
            smax = 0.0;
          }

          s = vn1->data[nmi] / vn2->data[nmi];
          s = smax * (s * s);
          if (s <= 1.4901161193847656E-8) {
            if (i + 1 < m) {
              smax = 0.0;
              if (mmi < 1) {
              } else if (mmi == 1) {
                smax = fabs(A->data[itemp]);
              } else {
                s = 2.2250738585072014E-308;
                pvt = itemp + mmi;
                while (itemp + 1 <= pvt) {
                  absxk = fabs(A->data[itemp]);
                  if (absxk > s) {
                    t = s / absxk;
                    smax = 1.0 + smax * t * t;
                    s = absxk;
                  } else {
                    t = absxk / s;
                    smax += t * t;
                  }

                  itemp++;
                }

                smax = s * sqrt(smax);
              }

              vn1->data[nmi] = smax;
              vn2->data[nmi] = vn1->data[nmi];
            } else {
              vn1->data[nmi] = 0.0;
              vn2->data[nmi] = 0.0;
            }
          } else {
            vn1->data[nmi] *= sqrt(smax);
          }
        }
      }
    }

    emxFree_real_T(&vn2);
    emxFree_real_T(&vn1);
    emxFree_real_T(&work);
  }
}

//
// File trailer for xgeqp3.cpp
//
// [EOF]
//
