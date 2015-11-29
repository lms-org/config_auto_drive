//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: handle_measurements.cpp
//
// MATLAB Coder version            : 3.0
// C/C++ source code generated on  : 29-Nov-2015 11:30:09
//

// Include Files
#include "rt_nonfinite.h"
#include "kalman_filter_lr.h"
#include "handle_measurements.h"
#include "kalman_filter_lr_emxutil.h"
#include "messmatrix.h"
#include "projectPoints.h"

// Function Declarations
static void b_eml_null_assignment(emxArray_real_T *x, const emxArray_boolean_T
  *idx);
static void eml_null_assignment(emxArray_real_T *x, const emxArray_boolean_T
  *idx);

// Function Definitions

//
// Arguments    : emxArray_real_T *x
//                const emxArray_boolean_T *idx
// Return Type  : void
//
static void b_eml_null_assignment(emxArray_real_T *x, const emxArray_boolean_T
  *idx)
{
  int nrowx;
  int nrows;
  int k;
  int i;
  int j;
  emxArray_real_T *b_x;
  nrowx = x->size[0];
  nrows = 0;
  for (k = 1; k <= idx->size[0]; k++) {
    nrows += idx->data[k - 1];
  }

  nrows = x->size[0] - nrows;
  i = 0;
  for (k = 1; k <= nrowx; k++) {
    if ((k > idx->size[0]) || (!idx->data[k - 1])) {
      for (j = 0; j < 3; j++) {
        x->data[i + x->size[0] * j] = x->data[(k + x->size[0] * j) - 1];
      }

      i++;
    }
  }

  if (1 > nrows) {
    nrows = 0;
  }

  emxInit_real_T(&b_x, 2);
  i = b_x->size[0] * b_x->size[1];
  b_x->size[0] = nrows;
  b_x->size[1] = 3;
  emxEnsureCapacity((emxArray__common *)b_x, i, (int)sizeof(double));
  for (i = 0; i < 3; i++) {
    for (j = 0; j < nrows; j++) {
      b_x->data[j + b_x->size[0] * i] = x->data[j + x->size[0] * i];
    }
  }

  i = x->size[0] * x->size[1];
  x->size[0] = b_x->size[0];
  x->size[1] = 3;
  emxEnsureCapacity((emxArray__common *)x, i, (int)sizeof(double));
  for (i = 0; i < 3; i++) {
    nrows = b_x->size[0];
    for (j = 0; j < nrows; j++) {
      x->data[j + x->size[0] * i] = b_x->data[j + b_x->size[0] * i];
    }
  }

  emxFree_real_T(&b_x);
}

//
// Arguments    : emxArray_real_T *x
//                const emxArray_boolean_T *idx
// Return Type  : void
//
static void eml_null_assignment(emxArray_real_T *x, const emxArray_boolean_T
  *idx)
{
  int nxin;
  int nrowx;
  int nxout;
  int k;
  int k0;
  emxArray_real_T *b_x;
  nxin = x->size[0];
  nrowx = x->size[0];
  nxout = 0;
  for (k = 1; k <= idx->size[0]; k++) {
    nxout += idx->data[k - 1];
  }

  nxout = x->size[0] - nxout;
  k0 = -1;
  for (k = 1; k <= nxin; k++) {
    if ((k > idx->size[0]) || (!idx->data[k - 1])) {
      k0++;
      x->data[k0] = x->data[k - 1];
    }
  }

  if (nrowx != 1) {
    if (1 > nxout) {
      nxout = 0;
    }

    emxInit_real_T1(&b_x, 1);
    k0 = b_x->size[0];
    b_x->size[0] = nxout;
    emxEnsureCapacity((emxArray__common *)b_x, k0, (int)sizeof(double));
    for (k0 = 0; k0 < nxout; k0++) {
      b_x->data[k0] = x->data[k0];
    }

    k0 = x->size[0];
    x->size[0] = b_x->size[0];
    emxEnsureCapacity((emxArray__common *)x, k0, (int)sizeof(double));
    nxout = b_x->size[0];
    for (k0 = 0; k0 < nxout; k0++) {
      x->data[k0] = b_x->data[k0];
    }

    emxFree_real_T(&b_x);
  } else {
    k0 = x->size[0];
    if (1 > nxout) {
      x->size[0] = 0;
    } else {
      x->size[0] = nxout;
    }

    emxEnsureCapacity((emxArray__common *)x, k0, (int)sizeof(double));
  }
}

//
// Arguments    : const emxArray_real_T *r
//                double delta
//                emxArray_real_T *xm
//                emxArray_real_T *ym
//                emxArray_real_T *H
//                emxArray_real_T *z
//                emxArray_real_T *zm
//                double *nValidPoints
// Return Type  : void
//
void b_handle_measurements(const emxArray_real_T *r, double delta,
  emxArray_real_T *xm, emxArray_real_T *ym, emxArray_real_T *H, emxArray_real_T *
  z, emxArray_real_T *zm, double *nValidPoints)
{
  emxArray_real_T *P;
  emxArray_real_T *xp;
  emxArray_real_T *yp;
  emxArray_real_T *phi;
  int xp_idx_0;
  int yp_idx_0;
  int phi_idx_0;
  int i3;
  emxArray_real_T *D;
  int s;
  int m;
  double a;
  double b_a;
  double dist_point;
  double b_P[2];
  double M_idx_1;
  double c_P[2];
  double v[2];
  double lambda;
  double dist_line;
  emxArray_boolean_T *ind;
  emxArray_real_T *b_D;
  emxArray_real_T *c_D;
  unsigned int count;
  emxInit_real_T(&P, 2);
  emxInit_real_T1(&xp, 1);
  emxInit_real_T1(&yp, 1);
  emxInit_real_T1(&phi, 1);
  projectPoints(r, delta, -0.38, xp, yp, phi);

  // Punkte von Mittellinie nach links/rechts projizieren
  xp_idx_0 = xp->size[0];
  yp_idx_0 = yp->size[0];
  phi_idx_0 = phi->size[0];
  i3 = P->size[0] * P->size[1];
  P->size[0] = xp_idx_0;
  P->size[1] = 3;
  emxEnsureCapacity((emxArray__common *)P, i3, (int)sizeof(double));
  for (i3 = 0; i3 < xp_idx_0; i3++) {
    P->data[i3] = xp->data[i3];
  }

  emxFree_real_T(&xp);
  for (i3 = 0; i3 < yp_idx_0; i3++) {
    P->data[i3 + P->size[0]] = yp->data[i3];
  }

  emxFree_real_T(&yp);
  for (i3 = 0; i3 < phi_idx_0; i3++) {
    P->data[i3 + (P->size[0] << 1)] = phi->data[i3];
  }

  emxFree_real_T(&phi);
  emxInit_real_T(&D, 2);
  i3 = D->size[0] * D->size[1];
  D->size[0] = xm->size[0];
  D->size[1] = 3;
  emxEnsureCapacity((emxArray__common *)D, i3, (int)sizeof(double));
  xp_idx_0 = xm->size[0] * 3;
  for (i3 = 0; i3 < xp_idx_0; i3++) {
    D->data[i3] = 10000.0;
  }

  //  Fuer jeden Messpunkt den nächsten Punkt der aktuellen Praediktion finden
  for (s = 0; s < r->size[0]; s++) {
    for (m = 0; m < xm->size[0]; m++) {
      a = P->data[s] - xm->data[m];
      b_a = P->data[s + P->size[0]] - ym->data[m];
      dist_point = sqrt(a * a + b_a * b_a);
      if (dist_point < D->data[m + (D->size[0] << 1)]) {
        D->data[m] = 1.0 + (double)s;
        D->data[m + D->size[0]] = 0.0;
        D->data[m + (D->size[0] << 1)] = dist_point;
      }

      if (1 + s > 1) {
        b_P[0] = P->data[s - 1];
        b_P[1] = P->data[(s + P->size[0]) - 1];
        b_a = xm->data[m];
        M_idx_1 = ym->data[m];

        // Abstand d zwischen der Gerade g (von P in Richtung Q) und dem Punkt M 
        // S ist der Punkt auf g mit kleinstem Abstand zu M
        c_P[0] = P->data[s];
        c_P[1] = P->data[s + P->size[0]];
        for (i3 = 0; i3 < 2; i3++) {
          v[i3] = c_P[i3] - b_P[i3];
        }

        lambda = -(v[0] * (b_P[0] - b_a) + v[1] * (b_P[1] - M_idx_1)) / (v[0] *
          v[0] + v[1] * v[1]);
        for (i3 = 0; i3 < 2; i3++) {
          b_P[i3] += lambda * v[i3];
        }

        a = b_a - b_P[0];
        b_a = M_idx_1 - b_P[1];
        dist_line = sqrt(a * a + b_a * b_a);
        if ((dist_line < D->data[m + (D->size[0] << 1)]) && (lambda > 0.0) &&
            (lambda < 1.0)) {
          D->data[m] = (1.0 + (double)s) - 1.0;
          D->data[m + D->size[0]] = lambda;
          D->data[m + (D->size[0] << 1)] = dist_line;
        }
      }
    }
  }

  emxInit_boolean_T(&ind, 1);

  //  Messpunkte ausfiltern, die vor dem letzten prädizierten Punkt oder zu weit entfernt liegen 
  xp_idx_0 = D->size[0];
  yp_idx_0 = r->size[0];
  i3 = ind->size[0];
  ind->size[0] = xp_idx_0;
  emxEnsureCapacity((emxArray__common *)ind, i3, (int)sizeof(boolean_T));
  for (i3 = 0; i3 < xp_idx_0; i3++) {
    ind->data[i3] = ((D->data[i3] == yp_idx_0) || (D->data[i3 + (D->size[0] << 1)]
      > 1.0));
  }

  emxInit_real_T1(&b_D, 1);
  eml_null_assignment(xm, ind);
  eml_null_assignment(ym, ind);
  b_eml_null_assignment(D, ind);
  *nValidPoints = D->size[0];

  //  Anzahl an letzlich verwerteten Messpunkten
  //  Messmatrix H, Messvektor z und Erwartungsvektor zm berechnen
  xp_idx_0 = D->size[0];
  i3 = b_D->size[0];
  b_D->size[0] = xp_idx_0;
  emxEnsureCapacity((emxArray__common *)b_D, i3, (int)sizeof(double));
  emxFree_boolean_T(&ind);
  for (i3 = 0; i3 < xp_idx_0; i3++) {
    b_D->data[i3] = D->data[i3];
  }

  emxInit_real_T1(&c_D, 1);
  xp_idx_0 = D->size[0];
  i3 = c_D->size[0];
  c_D->size[0] = xp_idx_0;
  emxEnsureCapacity((emxArray__common *)c_D, i3, (int)sizeof(double));
  for (i3 = 0; i3 < xp_idx_0; i3++) {
    c_D->data[i3] = D->data[i3 + D->size[0]];
  }

  messmatrix(P, r, delta, b_D, c_D, H);
  i3 = z->size[0];
  z->size[0] = (int)(2.0 * (double)xm->size[0]);
  emxEnsureCapacity((emxArray__common *)z, i3, (int)sizeof(double));
  xp_idx_0 = (int)(2.0 * (double)xm->size[0]);
  emxFree_real_T(&c_D);
  emxFree_real_T(&b_D);
  for (i3 = 0; i3 < xp_idx_0; i3++) {
    z->data[i3] = 0.0;
  }

  i3 = zm->size[0];
  zm->size[0] = (int)(2.0 * (double)xm->size[0]);
  emxEnsureCapacity((emxArray__common *)zm, i3, (int)sizeof(double));
  xp_idx_0 = (int)(2.0 * (double)xm->size[0]);
  for (i3 = 0; i3 < xp_idx_0; i3++) {
    zm->data[i3] = 0.0;
  }

  count = 2U;
  for (m = 0; m < xm->size[0]; m++) {
    z->data[(int)count - 2] = xm->data[m];
    z->data[(int)count - 1] = ym->data[m];
    zm->data[(int)count - 2] = P->data[(int)D->data[m] - 1] + D->data[m +
      D->size[0]] * (P->data[(int)(D->data[m] + 1.0) - 1] - P->data[(int)D->
                     data[m] - 1]);
    zm->data[(int)count - 1] = P->data[((int)D->data[m] + P->size[0]) - 1] +
      D->data[m + D->size[0]] * (P->data[((int)(D->data[m] + 1.0) + P->size[0])
      - 1] - P->data[((int)D->data[m] + P->size[0]) - 1]);
    count += 2U;
  }

  emxFree_real_T(&D);
  emxFree_real_T(&P);
}

//
// Arguments    : const emxArray_real_T *r
//                double delta
//                emxArray_real_T *xm
//                emxArray_real_T *ym
//                emxArray_real_T *H
//                emxArray_real_T *z
//                emxArray_real_T *zm
//                double *nValidPoints
// Return Type  : void
//
void c_handle_measurements(const emxArray_real_T *r, double delta,
  emxArray_real_T *xm, emxArray_real_T *ym, emxArray_real_T *H, emxArray_real_T *
  z, emxArray_real_T *zm, double *nValidPoints)
{
  emxArray_real_T *P;
  emxArray_real_T *xp;
  emxArray_real_T *yp;
  emxArray_real_T *phi;
  int xp_idx_0;
  int yp_idx_0;
  int phi_idx_0;
  int i4;
  emxArray_real_T *D;
  int s;
  int m;
  double a;
  double b_a;
  double dist_point;
  double b_P[2];
  double M_idx_1;
  double c_P[2];
  double v[2];
  double lambda;
  double dist_line;
  emxArray_boolean_T *ind;
  emxArray_real_T *b_D;
  emxArray_real_T *c_D;
  unsigned int count;
  emxInit_real_T(&P, 2);
  emxInit_real_T1(&xp, 1);
  emxInit_real_T1(&yp, 1);
  emxInit_real_T1(&phi, 1);
  projectPoints(r, delta, 0.0, xp, yp, phi);

  // Punkte von Mittellinie nach links/rechts projizieren
  xp_idx_0 = xp->size[0];
  yp_idx_0 = yp->size[0];
  phi_idx_0 = phi->size[0];
  i4 = P->size[0] * P->size[1];
  P->size[0] = xp_idx_0;
  P->size[1] = 3;
  emxEnsureCapacity((emxArray__common *)P, i4, (int)sizeof(double));
  for (i4 = 0; i4 < xp_idx_0; i4++) {
    P->data[i4] = xp->data[i4];
  }

  emxFree_real_T(&xp);
  for (i4 = 0; i4 < yp_idx_0; i4++) {
    P->data[i4 + P->size[0]] = yp->data[i4];
  }

  emxFree_real_T(&yp);
  for (i4 = 0; i4 < phi_idx_0; i4++) {
    P->data[i4 + (P->size[0] << 1)] = phi->data[i4];
  }

  emxFree_real_T(&phi);
  emxInit_real_T(&D, 2);
  i4 = D->size[0] * D->size[1];
  D->size[0] = xm->size[0];
  D->size[1] = 3;
  emxEnsureCapacity((emxArray__common *)D, i4, (int)sizeof(double));
  xp_idx_0 = xm->size[0] * 3;
  for (i4 = 0; i4 < xp_idx_0; i4++) {
    D->data[i4] = 10000.0;
  }

  //  Fuer jeden Messpunkt den nächsten Punkt der aktuellen Praediktion finden
  for (s = 0; s < r->size[0]; s++) {
    for (m = 0; m < xm->size[0]; m++) {
      a = P->data[s] - xm->data[m];
      b_a = P->data[s + P->size[0]] - ym->data[m];
      dist_point = sqrt(a * a + b_a * b_a);
      if (dist_point < D->data[m + (D->size[0] << 1)]) {
        D->data[m] = 1.0 + (double)s;
        D->data[m + D->size[0]] = 0.0;
        D->data[m + (D->size[0] << 1)] = dist_point;
      }

      if (1 + s > 1) {
        b_P[0] = P->data[s - 1];
        b_P[1] = P->data[(s + P->size[0]) - 1];
        b_a = xm->data[m];
        M_idx_1 = ym->data[m];

        // Abstand d zwischen der Gerade g (von P in Richtung Q) und dem Punkt M 
        // S ist der Punkt auf g mit kleinstem Abstand zu M
        c_P[0] = P->data[s];
        c_P[1] = P->data[s + P->size[0]];
        for (i4 = 0; i4 < 2; i4++) {
          v[i4] = c_P[i4] - b_P[i4];
        }

        lambda = -(v[0] * (b_P[0] - b_a) + v[1] * (b_P[1] - M_idx_1)) / (v[0] *
          v[0] + v[1] * v[1]);
        for (i4 = 0; i4 < 2; i4++) {
          b_P[i4] += lambda * v[i4];
        }

        a = b_a - b_P[0];
        b_a = M_idx_1 - b_P[1];
        dist_line = sqrt(a * a + b_a * b_a);
        if ((dist_line < D->data[m + (D->size[0] << 1)]) && (lambda > 0.0) &&
            (lambda < 1.0)) {
          D->data[m] = (1.0 + (double)s) - 1.0;
          D->data[m + D->size[0]] = lambda;
          D->data[m + (D->size[0] << 1)] = dist_line;
        }
      }
    }
  }

  emxInit_boolean_T(&ind, 1);

  //  Messpunkte ausfiltern, die vor dem letzten prädizierten Punkt oder zu weit entfernt liegen 
  xp_idx_0 = D->size[0];
  yp_idx_0 = r->size[0];
  i4 = ind->size[0];
  ind->size[0] = xp_idx_0;
  emxEnsureCapacity((emxArray__common *)ind, i4, (int)sizeof(boolean_T));
  for (i4 = 0; i4 < xp_idx_0; i4++) {
    ind->data[i4] = ((D->data[i4] == yp_idx_0) || (D->data[i4 + (D->size[0] << 1)]
      > 1.0));
  }

  emxInit_real_T1(&b_D, 1);
  eml_null_assignment(xm, ind);
  eml_null_assignment(ym, ind);
  b_eml_null_assignment(D, ind);
  *nValidPoints = D->size[0];

  //  Anzahl an letzlich verwerteten Messpunkten
  //  Messmatrix H, Messvektor z und Erwartungsvektor zm berechnen
  xp_idx_0 = D->size[0];
  i4 = b_D->size[0];
  b_D->size[0] = xp_idx_0;
  emxEnsureCapacity((emxArray__common *)b_D, i4, (int)sizeof(double));
  emxFree_boolean_T(&ind);
  for (i4 = 0; i4 < xp_idx_0; i4++) {
    b_D->data[i4] = D->data[i4];
  }

  emxInit_real_T1(&c_D, 1);
  xp_idx_0 = D->size[0];
  i4 = c_D->size[0];
  c_D->size[0] = xp_idx_0;
  emxEnsureCapacity((emxArray__common *)c_D, i4, (int)sizeof(double));
  for (i4 = 0; i4 < xp_idx_0; i4++) {
    c_D->data[i4] = D->data[i4 + D->size[0]];
  }

  messmatrix(P, r, delta, b_D, c_D, H);
  i4 = z->size[0];
  z->size[0] = (int)(2.0 * (double)xm->size[0]);
  emxEnsureCapacity((emxArray__common *)z, i4, (int)sizeof(double));
  xp_idx_0 = (int)(2.0 * (double)xm->size[0]);
  emxFree_real_T(&c_D);
  emxFree_real_T(&b_D);
  for (i4 = 0; i4 < xp_idx_0; i4++) {
    z->data[i4] = 0.0;
  }

  i4 = zm->size[0];
  zm->size[0] = (int)(2.0 * (double)xm->size[0]);
  emxEnsureCapacity((emxArray__common *)zm, i4, (int)sizeof(double));
  xp_idx_0 = (int)(2.0 * (double)xm->size[0]);
  for (i4 = 0; i4 < xp_idx_0; i4++) {
    zm->data[i4] = 0.0;
  }

  count = 2U;
  for (m = 0; m < xm->size[0]; m++) {
    z->data[(int)count - 2] = xm->data[m];
    z->data[(int)count - 1] = ym->data[m];
    zm->data[(int)count - 2] = P->data[(int)D->data[m] - 1] + D->data[m +
      D->size[0]] * (P->data[(int)(D->data[m] + 1.0) - 1] - P->data[(int)D->
                     data[m] - 1]);
    zm->data[(int)count - 1] = P->data[((int)D->data[m] + P->size[0]) - 1] +
      D->data[m + D->size[0]] * (P->data[((int)(D->data[m] + 1.0) + P->size[0])
      - 1] - P->data[((int)D->data[m] + P->size[0]) - 1]);
    count += 2U;
  }

  emxFree_real_T(&D);
  emxFree_real_T(&P);
}

//
// Arguments    : const emxArray_real_T *r
//                double delta
//                emxArray_real_T *xm
//                emxArray_real_T *ym
//                emxArray_real_T *H
//                emxArray_real_T *z
//                emxArray_real_T *zm
//                double *nValidPoints
// Return Type  : void
//
void handle_measurements(const emxArray_real_T *r, double delta, emxArray_real_T
  *xm, emxArray_real_T *ym, emxArray_real_T *H, emxArray_real_T *z,
  emxArray_real_T *zm, double *nValidPoints)
{
  emxArray_real_T *P;
  emxArray_real_T *xp;
  emxArray_real_T *yp;
  emxArray_real_T *phi;
  int xp_idx_0;
  int yp_idx_0;
  int phi_idx_0;
  int i0;
  emxArray_real_T *D;
  int s;
  int m;
  double a;
  double b_a;
  double dist_point;
  double b_P[2];
  double M_idx_1;
  double c_P[2];
  double v[2];
  double lambda;
  double dist_line;
  emxArray_boolean_T *ind;
  emxArray_real_T *b_D;
  emxArray_real_T *c_D;
  unsigned int count;
  emxInit_real_T(&P, 2);
  emxInit_real_T1(&xp, 1);
  emxInit_real_T1(&yp, 1);
  emxInit_real_T1(&phi, 1);
  projectPoints(r, delta, 0.38, xp, yp, phi);

  // Punkte von Mittellinie nach links/rechts projizieren
  xp_idx_0 = xp->size[0];
  yp_idx_0 = yp->size[0];
  phi_idx_0 = phi->size[0];
  i0 = P->size[0] * P->size[1];
  P->size[0] = xp_idx_0;
  P->size[1] = 3;
  emxEnsureCapacity((emxArray__common *)P, i0, (int)sizeof(double));
  for (i0 = 0; i0 < xp_idx_0; i0++) {
    P->data[i0] = xp->data[i0];
  }

  emxFree_real_T(&xp);
  for (i0 = 0; i0 < yp_idx_0; i0++) {
    P->data[i0 + P->size[0]] = yp->data[i0];
  }

  emxFree_real_T(&yp);
  for (i0 = 0; i0 < phi_idx_0; i0++) {
    P->data[i0 + (P->size[0] << 1)] = phi->data[i0];
  }

  emxFree_real_T(&phi);
  emxInit_real_T(&D, 2);
  i0 = D->size[0] * D->size[1];
  D->size[0] = xm->size[0];
  D->size[1] = 3;
  emxEnsureCapacity((emxArray__common *)D, i0, (int)sizeof(double));
  xp_idx_0 = xm->size[0] * 3;
  for (i0 = 0; i0 < xp_idx_0; i0++) {
    D->data[i0] = 10000.0;
  }

  //  Fuer jeden Messpunkt den nächsten Punkt der aktuellen Praediktion finden
  for (s = 0; s < r->size[0]; s++) {
    for (m = 0; m < xm->size[0]; m++) {
      a = P->data[s] - xm->data[m];
      b_a = P->data[s + P->size[0]] - ym->data[m];
      dist_point = sqrt(a * a + b_a * b_a);
      if (dist_point < D->data[m + (D->size[0] << 1)]) {
        D->data[m] = 1.0 + (double)s;
        D->data[m + D->size[0]] = 0.0;
        D->data[m + (D->size[0] << 1)] = dist_point;
      }

      if (1 + s > 1) {
        b_P[0] = P->data[s - 1];
        b_P[1] = P->data[(s + P->size[0]) - 1];
        b_a = xm->data[m];
        M_idx_1 = ym->data[m];

        // Abstand d zwischen der Gerade g (von P in Richtung Q) und dem Punkt M 
        // S ist der Punkt auf g mit kleinstem Abstand zu M
        c_P[0] = P->data[s];
        c_P[1] = P->data[s + P->size[0]];
        for (i0 = 0; i0 < 2; i0++) {
          v[i0] = c_P[i0] - b_P[i0];
        }

        lambda = -(v[0] * (b_P[0] - b_a) + v[1] * (b_P[1] - M_idx_1)) / (v[0] *
          v[0] + v[1] * v[1]);
        for (i0 = 0; i0 < 2; i0++) {
          b_P[i0] += lambda * v[i0];
        }

        a = b_a - b_P[0];
        b_a = M_idx_1 - b_P[1];
        dist_line = sqrt(a * a + b_a * b_a);
        if ((dist_line < D->data[m + (D->size[0] << 1)]) && (lambda > 0.0) &&
            (lambda < 1.0)) {
          D->data[m] = (1.0 + (double)s) - 1.0;
          D->data[m + D->size[0]] = lambda;
          D->data[m + (D->size[0] << 1)] = dist_line;
        }
      }
    }
  }

  emxInit_boolean_T(&ind, 1);

  //  Messpunkte ausfiltern, die vor dem letzten prädizierten Punkt oder zu weit entfernt liegen 
  xp_idx_0 = D->size[0];
  yp_idx_0 = r->size[0];
  i0 = ind->size[0];
  ind->size[0] = xp_idx_0;
  emxEnsureCapacity((emxArray__common *)ind, i0, (int)sizeof(boolean_T));
  for (i0 = 0; i0 < xp_idx_0; i0++) {
    ind->data[i0] = ((D->data[i0] == yp_idx_0) || (D->data[i0 + (D->size[0] << 1)]
      > 1.0));
  }

  emxInit_real_T1(&b_D, 1);
  eml_null_assignment(xm, ind);
  eml_null_assignment(ym, ind);
  b_eml_null_assignment(D, ind);
  *nValidPoints = D->size[0];

  //  Anzahl an letzlich verwerteten Messpunkten
  //  Messmatrix H, Messvektor z und Erwartungsvektor zm berechnen
  xp_idx_0 = D->size[0];
  i0 = b_D->size[0];
  b_D->size[0] = xp_idx_0;
  emxEnsureCapacity((emxArray__common *)b_D, i0, (int)sizeof(double));
  emxFree_boolean_T(&ind);
  for (i0 = 0; i0 < xp_idx_0; i0++) {
    b_D->data[i0] = D->data[i0];
  }

  emxInit_real_T1(&c_D, 1);
  xp_idx_0 = D->size[0];
  i0 = c_D->size[0];
  c_D->size[0] = xp_idx_0;
  emxEnsureCapacity((emxArray__common *)c_D, i0, (int)sizeof(double));
  for (i0 = 0; i0 < xp_idx_0; i0++) {
    c_D->data[i0] = D->data[i0 + D->size[0]];
  }

  messmatrix(P, r, delta, b_D, c_D, H);
  i0 = z->size[0];
  z->size[0] = (int)(2.0 * (double)xm->size[0]);
  emxEnsureCapacity((emxArray__common *)z, i0, (int)sizeof(double));
  xp_idx_0 = (int)(2.0 * (double)xm->size[0]);
  emxFree_real_T(&c_D);
  emxFree_real_T(&b_D);
  for (i0 = 0; i0 < xp_idx_0; i0++) {
    z->data[i0] = 0.0;
  }

  i0 = zm->size[0];
  zm->size[0] = (int)(2.0 * (double)xm->size[0]);
  emxEnsureCapacity((emxArray__common *)zm, i0, (int)sizeof(double));
  xp_idx_0 = (int)(2.0 * (double)xm->size[0]);
  for (i0 = 0; i0 < xp_idx_0; i0++) {
    zm->data[i0] = 0.0;
  }

  count = 2U;
  for (m = 0; m < xm->size[0]; m++) {
    z->data[(int)count - 2] = xm->data[m];
    z->data[(int)count - 1] = ym->data[m];
    zm->data[(int)count - 2] = P->data[(int)D->data[m] - 1] + D->data[m +
      D->size[0]] * (P->data[(int)(D->data[m] + 1.0) - 1] - P->data[(int)D->
                     data[m] - 1]);
    zm->data[(int)count - 1] = P->data[((int)D->data[m] + P->size[0]) - 1] +
      D->data[m + D->size[0]] * (P->data[((int)(D->data[m] + 1.0) + P->size[0])
      - 1] - P->data[((int)D->data[m] + P->size[0]) - 1]);
    count += 2U;
  }

  emxFree_real_T(&D);
  emxFree_real_T(&P);
}

//
// File trailer for handle_measurements.cpp
//
// [EOF]
//
