/*
 * File: mrdivide.c
 *
 * MATLAB Coder version            : 2.7
 * C/C++ source code generated on  : 26-Jun-2015 19:42:29
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "kalman_filter_lr.h"
#include "mrdivide.h"
#include "kalman_filter_lr_emxutil.h"
#include "colon.h"

/* Function Declarations */
static double b_eml_xnrm2(int n, const emxArray_real_T *x, int ix0);
static void eml_lusolve(const emxArray_real_T *A, emxArray_real_T *B);
static double eml_matlab_zlarfg(int n, double *alpha1, emxArray_real_T *x, int
  ix0);
static void eml_qrsolve(const emxArray_real_T *A, emxArray_real_T *B,
  emxArray_real_T *Y);
static double eml_xnrm2(int n, const emxArray_real_T *x, int ix0);
static void eml_xscal(int n, double a, emxArray_real_T *x, int ix0);
static double rt_hypotd_snf(double u0, double u1);

/* Function Definitions */

/*
 * Arguments    : int n
 *                const emxArray_real_T *x
 *                int ix0
 * Return Type  : double
 */
static double b_eml_xnrm2(int n, const emxArray_real_T *x, int ix0)
{
  double y;
  double scale;
  int kend;
  int k;
  double absxk;
  double t;
  y = 0.0;
  if (n < 1) {
  } else if (n == 1) {
    y = fabs(x->data[ix0 - 1]);
  } else {
    scale = 2.2250738585072014E-308;
    kend = (ix0 + n) - 1;
    for (k = ix0; k <= kend; k++) {
      absxk = fabs(x->data[k - 1]);
      if (absxk > scale) {
        t = scale / absxk;
        y = 1.0 + y * t * t;
        scale = absxk;
      } else {
        t = absxk / scale;
        y += t * t;
      }
    }

    y = scale * sqrt(y);
  }

  return y;
}

/*
 * Arguments    : const emxArray_real_T *A
 *                emxArray_real_T *B
 * Return Type  : void
 */
static void eml_lusolve(const emxArray_real_T *A, emxArray_real_T *B)
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
  b_emxInit_real_T(&b_A, 2);
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

/*
 * Arguments    : int n
 *                double *alpha1
 *                emxArray_real_T *x
 *                int ix0
 * Return Type  : double
 */
static double eml_matlab_zlarfg(int n, double *alpha1, emxArray_real_T *x, int
  ix0)
{
  double tau;
  double xnorm;
  int knt;
  int k;
  tau = 0.0;
  if (n <= 0) {
  } else {
    xnorm = b_eml_xnrm2(n - 1, x, ix0);
    if (xnorm != 0.0) {
      xnorm = rt_hypotd_snf(*alpha1, xnorm);
      if (*alpha1 >= 0.0) {
        xnorm = -xnorm;
      }

      if (fabs(xnorm) < 1.0020841800044864E-292) {
        knt = 0;
        do {
          knt++;
          eml_xscal(n - 1, 9.9792015476736E+291, x, ix0);
          xnorm *= 9.9792015476736E+291;
          *alpha1 *= 9.9792015476736E+291;
        } while (!(fabs(xnorm) >= 1.0020841800044864E-292));

        xnorm = b_eml_xnrm2(n - 1, x, ix0);
        xnorm = rt_hypotd_snf(*alpha1, xnorm);
        if (*alpha1 >= 0.0) {
          xnorm = -xnorm;
        }

        tau = (xnorm - *alpha1) / xnorm;
        eml_xscal(n - 1, 1.0 / (*alpha1 - xnorm), x, ix0);
        for (k = 1; k <= knt; k++) {
          xnorm *= 1.0020841800044864E-292;
        }

        *alpha1 = xnorm;
      } else {
        tau = (xnorm - *alpha1) / xnorm;
        eml_xscal(n - 1, 1.0 / (*alpha1 - xnorm), x, ix0);
        *alpha1 = xnorm;
      }
    }
  }

  return tau;
}

/*
 * Arguments    : const emxArray_real_T *A
 *                emxArray_real_T *B
 *                emxArray_real_T *Y
 * Return Type  : void
 */
static void eml_qrsolve(const emxArray_real_T *A, emxArray_real_T *B,
  emxArray_real_T *Y)
{
  int m;
  int nb;
  int mn;
  emxArray_real_T *b_A;
  int i4;
  int k;
  int b_m;
  int n;
  int b_mn;
  emxArray_real_T *tau;
  emxArray_int32_T *jpvt;
  emxArray_real_T *work;
  emxArray_real_T *vn1;
  emxArray_real_T *vn2;
  int nmi;
  int i;
  int i_i;
  int mmi;
  int pvt;
  int ix;
  double smax;
  double s;
  int iy;
  int i_ip1;
  int lastv;
  int lastc;
  boolean_T exitg2;
  int32_T exitg1;
  double absxk;
  double t;
  unsigned int unnamed_idx_0;
  unsigned int unnamed_idx_1;
  m = A->size[0] - 1;
  nb = B->size[1] - 1;
  if (A->size[0] <= A->size[1]) {
    mn = A->size[0];
  } else {
    mn = A->size[1];
  }

  b_emxInit_real_T(&b_A, 2);
  i4 = b_A->size[0] * b_A->size[1];
  b_A->size[0] = A->size[0];
  b_A->size[1] = A->size[1];
  emxEnsureCapacity((emxArray__common *)b_A, i4, (int)sizeof(double));
  k = A->size[0] * A->size[1];
  for (i4 = 0; i4 < k; i4++) {
    b_A->data[i4] = A->data[i4];
  }

  b_m = A->size[0];
  n = A->size[1];
  if (A->size[0] <= A->size[1]) {
    b_mn = A->size[0];
  } else {
    b_mn = A->size[1];
  }

  emxInit_real_T(&tau, 1);
  emxInit_int32_T(&jpvt, 2);
  i4 = tau->size[0];
  tau->size[0] = b_mn;
  emxEnsureCapacity((emxArray__common *)tau, i4, (int)sizeof(double));
  eml_signed_integer_colon(A->size[1], jpvt);
  if ((A->size[0] == 0) || (A->size[1] == 0)) {
  } else {
    emxInit_real_T(&work, 1);
    k = A->size[1];
    i4 = work->size[0];
    work->size[0] = k;
    emxEnsureCapacity((emxArray__common *)work, i4, (int)sizeof(double));
    for (i4 = 0; i4 < k; i4++) {
      work->data[i4] = 0.0;
    }

    emxInit_real_T(&vn1, 1);
    emxInit_real_T(&vn2, 1);
    k = A->size[1];
    i4 = vn1->size[0];
    vn1->size[0] = k;
    emxEnsureCapacity((emxArray__common *)vn1, i4, (int)sizeof(double));
    i4 = vn2->size[0];
    vn2->size[0] = k;
    emxEnsureCapacity((emxArray__common *)vn2, i4, (int)sizeof(double));
    k = 1;
    for (nmi = 0; nmi + 1 <= n; nmi++) {
      vn1->data[nmi] = eml_xnrm2(b_m, A, k);
      vn2->data[nmi] = vn1->data[nmi];
      k += b_m;
    }

    for (i = 0; i + 1 <= b_mn; i++) {
      i_i = i + i * b_m;
      nmi = n - i;
      mmi = (b_m - i) - 1;
      if (nmi < 1) {
        pvt = 0;
      } else {
        pvt = 1;
        if (nmi > 1) {
          ix = i;
          smax = fabs(vn1->data[i]);
          for (k = 2; k <= nmi; k++) {
            ix++;
            s = fabs(vn1->data[ix]);
            if (s > smax) {
              pvt = k;
              smax = s;
            }
          }
        }
      }

      pvt = (i + pvt) - 1;
      if (pvt + 1 != i + 1) {
        ix = b_m * pvt;
        iy = b_m * i;
        for (k = 1; k <= b_m; k++) {
          smax = b_A->data[ix];
          b_A->data[ix] = b_A->data[iy];
          b_A->data[iy] = smax;
          ix++;
          iy++;
        }

        k = jpvt->data[pvt];
        jpvt->data[pvt] = jpvt->data[i];
        jpvt->data[i] = k;
        vn1->data[pvt] = vn1->data[i];
        vn2->data[pvt] = vn2->data[i];
      }

      if (i + 1 < b_m) {
        s = b_A->data[i_i];
        tau->data[i] = eml_matlab_zlarfg(1 + mmi, &s, b_A, i_i + 2);
      } else {
        smax = b_A->data[i_i];
        s = b_A->data[i_i];
        b_A->data[i_i] = smax;
        tau->data[i] = 0.0;
      }

      b_A->data[i_i] = s;
      if (i + 1 < n) {
        s = b_A->data[i_i];
        b_A->data[i_i] = 1.0;
        i_ip1 = (i + (i + 1) * b_m) + 1;
        if (tau->data[i] != 0.0) {
          lastv = mmi;
          pvt = i_i + mmi;
          while ((lastv + 1 > 0) && (b_A->data[pvt] == 0.0)) {
            lastv--;
            pvt--;
          }

          lastc = nmi - 1;
          exitg2 = false;
          while ((!exitg2) && (lastc > 0)) {
            k = i_ip1 + (lastc - 1) * b_m;
            nmi = k;
            do {
              exitg1 = 0;
              if (nmi <= k + lastv) {
                if (b_A->data[nmi - 1] != 0.0) {
                  exitg1 = 1;
                } else {
                  nmi++;
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
            i4 = i_ip1 + b_m * (lastc - 1);
            pvt = i_ip1;
            while (((b_m > 0) && (pvt <= i4)) || ((b_m < 0) && (pvt >= i4))) {
              ix = i_i;
              smax = 0.0;
              k = pvt + lastv;
              for (nmi = pvt; nmi <= k; nmi++) {
                smax += b_A->data[nmi - 1] * b_A->data[ix];
                ix++;
              }

              work->data[iy] += smax;
              iy++;
              pvt += b_m;
            }
          }

          if (-tau->data[i] == 0.0) {
          } else {
            pvt = 0;
            for (nmi = 1; nmi <= lastc; nmi++) {
              if (work->data[pvt] != 0.0) {
                smax = work->data[pvt] * -tau->data[i];
                ix = i_i;
                i4 = lastv + i_ip1;
                for (k = i_ip1; k <= i4; k++) {
                  b_A->data[k - 1] += b_A->data[ix] * smax;
                  ix++;
                }
              }

              pvt++;
              i_ip1 += b_m;
            }
          }
        }

        b_A->data[i_i] = s;
      }

      for (nmi = i + 1; nmi + 1 <= n; nmi++) {
        k = (i + b_m * nmi) + 1;
        if (vn1->data[nmi] != 0.0) {
          smax = fabs(b_A->data[i + b_A->size[0] * nmi]) / vn1->data[nmi];
          smax = 1.0 - smax * smax;
          if (smax < 0.0) {
            smax = 0.0;
          }

          s = vn1->data[nmi] / vn2->data[nmi];
          s = smax * (s * s);
          if (s <= 1.4901161193847656E-8) {
            if (i + 1 < b_m) {
              smax = 0.0;
              if (mmi < 1) {
              } else if (mmi == 1) {
                smax = fabs(b_A->data[k]);
              } else {
                s = 2.2250738585072014E-308;
                pvt = k + mmi;
                while (k + 1 <= pvt) {
                  absxk = fabs(b_A->data[k]);
                  if (absxk > s) {
                    t = s / absxk;
                    smax = 1.0 + smax * t * t;
                    s = absxk;
                  } else {
                    t = absxk / s;
                    smax += t * t;
                  }

                  k++;
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

  s = 0.0;
  if (mn > 0) {
    if (A->size[0] >= A->size[1]) {
      k = A->size[0];
    } else {
      k = A->size[1];
    }

    smax = (double)k * fabs(b_A->data[0]) * 2.2204460492503131E-16;
    k = 0;
    while ((k <= mn - 1) && (!(fabs(b_A->data[k + b_A->size[0] * k]) <= smax)))
    {
      s++;
      k++;
    }
  }

  unnamed_idx_0 = (unsigned int)A->size[1];
  unnamed_idx_1 = (unsigned int)B->size[1];
  i4 = Y->size[0] * Y->size[1];
  Y->size[0] = (int)unnamed_idx_0;
  emxEnsureCapacity((emxArray__common *)Y, i4, (int)sizeof(double));
  i4 = Y->size[0] * Y->size[1];
  Y->size[1] = (int)unnamed_idx_1;
  emxEnsureCapacity((emxArray__common *)Y, i4, (int)sizeof(double));
  k = (int)unnamed_idx_0 * (int)unnamed_idx_1;
  for (i4 = 0; i4 < k; i4++) {
    Y->data[i4] = 0.0;
  }

  for (nmi = 0; nmi < mn; nmi++) {
    if (tau->data[nmi] != 0.0) {
      for (k = 0; k <= nb; k++) {
        smax = B->data[nmi + B->size[0] * k];
        i4 = m + (int)(1.0 - ((1.0 + (double)nmi) + 1.0));
        for (i = 0; i <= i4; i++) {
          unnamed_idx_0 = ((unsigned int)nmi + i) + 2U;
          smax += b_A->data[((int)unnamed_idx_0 + b_A->size[0] * nmi) - 1] *
            B->data[((int)unnamed_idx_0 + B->size[0] * k) - 1];
        }

        smax *= tau->data[nmi];
        if (smax != 0.0) {
          B->data[nmi + B->size[0] * k] -= smax;
          i4 = m + (int)(1.0 - ((1.0 + (double)nmi) + 1.0));
          for (i = 0; i <= i4; i++) {
            unnamed_idx_0 = ((unsigned int)nmi + i) + 2U;
            B->data[((int)unnamed_idx_0 + B->size[0] * k) - 1] -= b_A->data
              [((int)unnamed_idx_0 + b_A->size[0] * nmi) - 1] * smax;
          }
        }
      }
    }
  }

  emxFree_real_T(&tau);
  for (k = 0; k <= nb; k++) {
    for (i = 0; i < (int)s; i++) {
      Y->data[(jpvt->data[i] + Y->size[0] * k) - 1] = B->data[i + B->size[0] * k];
    }

    for (nmi = 0; nmi < (int)-(1.0 + (-1.0 - s)); nmi++) {
      smax = s + -(double)nmi;
      Y->data[(jpvt->data[(int)smax - 1] + Y->size[0] * k) - 1] /= b_A->data
        [((int)smax + b_A->size[0] * ((int)smax - 1)) - 1];
      for (i = 0; i <= (int)smax - 2; i++) {
        Y->data[(jpvt->data[i] + Y->size[0] * k) - 1] -= Y->data[(jpvt->data
          [(int)smax - 1] + Y->size[0] * k) - 1] * b_A->data[i + b_A->size[0] *
          ((int)smax - 1)];
      }
    }
  }

  emxFree_int32_T(&jpvt);
  emxFree_real_T(&b_A);
}

/*
 * Arguments    : int n
 *                const emxArray_real_T *x
 *                int ix0
 * Return Type  : double
 */
static double eml_xnrm2(int n, const emxArray_real_T *x, int ix0)
{
  double y;
  double scale;
  int kend;
  int k;
  double absxk;
  double t;
  y = 0.0;
  if (n < 1) {
  } else if (n == 1) {
    y = fabs(x->data[ix0 - 1]);
  } else {
    scale = 2.2250738585072014E-308;
    kend = (ix0 + n) - 1;
    for (k = ix0; k <= kend; k++) {
      absxk = fabs(x->data[k - 1]);
      if (absxk > scale) {
        t = scale / absxk;
        y = 1.0 + y * t * t;
        scale = absxk;
      } else {
        t = absxk / scale;
        y += t * t;
      }
    }

    y = scale * sqrt(y);
  }

  return y;
}

/*
 * Arguments    : int n
 *                double a
 *                emxArray_real_T *x
 *                int ix0
 * Return Type  : void
 */
static void eml_xscal(int n, double a, emxArray_real_T *x, int ix0)
{
  int i9;
  int k;
  i9 = (ix0 + n) - 1;
  for (k = ix0; k <= i9; k++) {
    x->data[k - 1] *= a;
  }
}

/*
 * Arguments    : double u0
 *                double u1
 * Return Type  : double
 */
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

/*
 * Arguments    : const emxArray_real_T *A
 *                const emxArray_real_T *B
 *                emxArray_real_T *y
 * Return Type  : void
 */
void mrdivide(const emxArray_real_T *A, const emxArray_real_T *B,
              emxArray_real_T *y)
{
  emxArray_real_T *r0;
  emxArray_real_T *b_B;
  emxArray_real_T *b_A;
  unsigned int unnamed_idx_0;
  unsigned int unnamed_idx_1;
  int i2;
  int loop_ub;
  int b_loop_ub;
  int i3;
  b_emxInit_real_T(&r0, 2);
  b_emxInit_real_T(&b_B, 2);
  b_emxInit_real_T(&b_A, 2);
  if ((A->size[0] == 0) || (A->size[1] == 0) || ((B->size[0] == 0) || (B->size[1]
        == 0))) {
    unnamed_idx_0 = (unsigned int)A->size[0];
    unnamed_idx_1 = (unsigned int)B->size[0];
    i2 = y->size[0] * y->size[1];
    y->size[0] = (int)unnamed_idx_0;
    emxEnsureCapacity((emxArray__common *)y, i2, (int)sizeof(double));
    i2 = y->size[0] * y->size[1];
    y->size[1] = (int)unnamed_idx_1;
    emxEnsureCapacity((emxArray__common *)y, i2, (int)sizeof(double));
    loop_ub = (int)unnamed_idx_0 * (int)unnamed_idx_1;
    for (i2 = 0; i2 < loop_ub; i2++) {
      y->data[i2] = 0.0;
    }
  } else if (B->size[0] == B->size[1]) {
    i2 = y->size[0] * y->size[1];
    y->size[0] = A->size[0];
    y->size[1] = A->size[1];
    emxEnsureCapacity((emxArray__common *)y, i2, (int)sizeof(double));
    loop_ub = A->size[0] * A->size[1];
    for (i2 = 0; i2 < loop_ub; i2++) {
      y->data[i2] = A->data[i2];
    }

    eml_lusolve(B, y);
  } else {
    i2 = b_B->size[0] * b_B->size[1];
    b_B->size[0] = B->size[1];
    b_B->size[1] = B->size[0];
    emxEnsureCapacity((emxArray__common *)b_B, i2, (int)sizeof(double));
    loop_ub = B->size[0];
    for (i2 = 0; i2 < loop_ub; i2++) {
      b_loop_ub = B->size[1];
      for (i3 = 0; i3 < b_loop_ub; i3++) {
        b_B->data[i3 + b_B->size[0] * i2] = B->data[i2 + B->size[0] * i3];
      }
    }

    i2 = b_A->size[0] * b_A->size[1];
    b_A->size[0] = A->size[1];
    b_A->size[1] = A->size[0];
    emxEnsureCapacity((emxArray__common *)b_A, i2, (int)sizeof(double));
    loop_ub = A->size[0];
    for (i2 = 0; i2 < loop_ub; i2++) {
      b_loop_ub = A->size[1];
      for (i3 = 0; i3 < b_loop_ub; i3++) {
        b_A->data[i3 + b_A->size[0] * i2] = A->data[i2 + A->size[0] * i3];
      }
    }

    eml_qrsolve(b_B, b_A, r0);
    i2 = y->size[0] * y->size[1];
    y->size[0] = r0->size[1];
    y->size[1] = r0->size[0];
    emxEnsureCapacity((emxArray__common *)y, i2, (int)sizeof(double));
    loop_ub = r0->size[0];
    for (i2 = 0; i2 < loop_ub; i2++) {
      b_loop_ub = r0->size[1];
      for (i3 = 0; i3 < b_loop_ub; i3++) {
        y->data[i3 + y->size[0] * i2] = r0->data[i2 + r0->size[0] * i3];
      }
    }
  }

  emxFree_real_T(&b_A);
  emxFree_real_T(&b_B);
  emxFree_real_T(&r0);
}

/*
 * File trailer for mrdivide.c
 *
 * [EOF]
 */
