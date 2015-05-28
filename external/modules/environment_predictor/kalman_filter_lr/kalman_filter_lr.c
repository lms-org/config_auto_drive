/*
 * File: kalman_filter_lr.c
 *
 * MATLAB Coder version            : 2.7
 * C/C++ source code generated on  : 28-May-2015 15:37:30
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "kalman_filter_lr.h"
#include "d_line_point.h"
#include "kalman_filter_lr_emxutil.h"
#include "eye.h"
#include "mrdivide.h"
#include "messmatrix_new.h"
#include "projectPoints.h"

/* Function Declarations */
static void b_eml_null_assignment(emxArray_real_T *x, const emxArray_boolean_T
  *idx);
static void eml_null_assignment(emxArray_real_T *x, const emxArray_boolean_T
  *idx);

/* Function Definitions */

/*
 * Arguments    : emxArray_real_T *x
 *                const emxArray_boolean_T *idx
 * Return Type  : void
 */
static void b_eml_null_assignment(emxArray_real_T *x, const emxArray_boolean_T
  *idx)
{
  int nrows;
  int k;
  int i;
  int i6;
  int j;
  emxArray_real_T *b_x;
  nrows = 0;
  for (k = 1; k <= idx->size[0]; k++) {
    nrows += idx->data[k - 1];
  }

  nrows = x->size[0] - nrows;
  i = 0;
  i6 = x->size[0];
  for (k = 1; k <= i6; k++) {
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

  b_emxInit_real_T(&b_x, 2);
  i6 = b_x->size[0] * b_x->size[1];
  b_x->size[0] = nrows;
  b_x->size[1] = 3;
  emxEnsureCapacity((emxArray__common *)b_x, i6, (int)sizeof(double));
  for (i6 = 0; i6 < 3; i6++) {
    for (i = 0; i < nrows; i++) {
      b_x->data[i + b_x->size[0] * i6] = x->data[i + x->size[0] * i6];
    }
  }

  i6 = x->size[0] * x->size[1];
  x->size[0] = b_x->size[0];
  x->size[1] = 3;
  emxEnsureCapacity((emxArray__common *)x, i6, (int)sizeof(double));
  for (i6 = 0; i6 < 3; i6++) {
    nrows = b_x->size[0];
    for (i = 0; i < nrows; i++) {
      x->data[i + x->size[0] * i6] = b_x->data[i + b_x->size[0] * i6];
    }
  }

  emxFree_real_T(&b_x);
}

/*
 * Arguments    : emxArray_real_T *x
 *                const emxArray_boolean_T *idx
 * Return Type  : void
 */
static void eml_null_assignment(emxArray_real_T *x, const emxArray_boolean_T
  *idx)
{
  int nxin;
  int k0;
  int k;
  int nxout;
  emxArray_real_T *b_x;
  nxin = x->size[0];
  k0 = 0;
  for (k = 1; k <= idx->size[0]; k++) {
    k0 += idx->data[k - 1];
  }

  nxout = x->size[0] - k0;
  k0 = -1;
  for (k = 1; k <= nxin; k++) {
    if ((k > idx->size[0]) || (!idx->data[k - 1])) {
      k0++;
      x->data[k0] = x->data[k - 1];
    }
  }

  if (x->size[0] != 1) {
    if (1 > nxout) {
      k0 = 0;
    } else {
      k0 = nxout;
    }

    emxInit_real_T(&b_x, 1);
    nxout = b_x->size[0];
    b_x->size[0] = k0;
    emxEnsureCapacity((emxArray__common *)b_x, nxout, (int)sizeof(double));
    for (nxout = 0; nxout < k0; nxout++) {
      b_x->data[nxout] = x->data[nxout];
    }

    nxout = x->size[0];
    x->size[0] = b_x->size[0];
    emxEnsureCapacity((emxArray__common *)x, nxout, (int)sizeof(double));
    k0 = b_x->size[0];
    for (nxout = 0; nxout < k0; nxout++) {
      x->data[nxout] = b_x->data[nxout];
    }

    emxFree_real_T(&b_x);
  } else {
    if (1 > nxout) {
      k0 = 0;
    } else {
      k0 = nxout;
    }

    emxInit_real_T(&b_x, 1);
    nxout = b_x->size[0];
    b_x->size[0] = k0;
    emxEnsureCapacity((emxArray__common *)b_x, nxout, (int)sizeof(double));
    for (nxout = 0; nxout < k0; nxout++) {
      b_x->data[nxout] = x->data[nxout];
    }

    nxout = x->size[0];
    x->size[0] = b_x->size[0];
    emxEnsureCapacity((emxArray__common *)x, nxout, (int)sizeof(double));
    k0 = b_x->size[0];
    for (nxout = 0; nxout < k0; nxout++) {
      x->data[nxout] = b_x->data[nxout];
    }

    emxFree_real_T(&b_x);
  }
}

/*
 * "kalman_filter_lr"
 *
 *  ----Nomenklatur----
 *   * r: Zustandsvektor = [y0, phi0, kappa_1, kappa_2, ... , kappa_(n-1)]
 *   --> y0: y-Wert des ersten Punktes (x-Wert ist immer 0)
 *   --> phi0: Anfangssteigung zwischem ersten und zweiten Punkt
 *   --> kappa_i: Krümmungen an den entsprechenden Punkten (Krümmungsdefinition: 1/R,
 *   wobei R der Radius des Kreises ist, auf dem der betrachtete Punkt, sein Vorgänger
 *   und sein Nachfolger liegen)
 *   * Pk: Kovarianzmatrix des Zustands dim[n x n], mit n = Dimension des Zustandsvektors
 *   * A: State-Transition-Matrix dim[n x n] (ist zurzeit eine Einheitsmatrix, da noch keine
 *    Messwerte fuer die Eigenbewegung verfügbar sind)
 *   * Q: Kovarianz des Zustandsübergangs dim[n x n] (symmetrische Matrix, mit weg von der
 *    Diagonalen abnehmenden Eintraegen)
 *   * R_fakt: Unsicherheit der Messwerte, sollte in der Groessenordnung der Diagonale von Q liegen
 *   * delta: Abstand der Punkte (delta*n ergibt die Länge des praedizierten Fahrstreifens)
 *   * xl, yl: Vektoren mit den Messwerten für die linke Spur
 *   * xr, yr: Vektoren mit den Messwerten für die rechte Spur
 *
 *   ----Grober Ablauf des Algorithmus----
 *   1. Projektion der Punkte von der Mittellinie nach links bzw. rechts
 *   2. Fuer jeden Messpunkt: Berechnung des kleinsten Abstands zum aktuell praedizierten Strassenverlauf
 *   3. Assemblierung der Jakobimatrix fuer die Projektion aus dem Zustandsraum r auf x-y-Koordinaten
 *   4. Kalman-Filter: Praediktion -> Messwerte einbeziehen -> Update
 *   5. Zustandsbegrenzungen einbringen
 * Arguments    : double r[10]
 *                const emxArray_real_T *A
 *                emxArray_real_T *Pk
 *                const emxArray_real_T *Q
 *                double R_fakt
 *                double delta
 *                emxArray_real_T *xl
 *                emxArray_real_T *yl
 *                emxArray_real_T *xr
 *                emxArray_real_T *yr
 * Return Type  : void
 */
void kalman_filter_lr(double r[10], const emxArray_real_T *A, emxArray_real_T
                      *Pk, const emxArray_real_T *Q, double R_fakt, double delta,
                      emxArray_real_T *xl, emxArray_real_T *yl, emxArray_real_T *
                      xr, emxArray_real_T *yr)
{
  double phi[10];
  double yp[10];
  double xp[10];
  double P[30];
  int i4;
  emxArray_real_T *D;
  int xl_idx_0;
  int br;
  int m;
  double dist_point;
  double a;
  double b_P[2];
  double c_P[2];
  double b_xl[2];
  double unusedU1[2];
  emxArray_boolean_T *ind;
  emxArray_real_T *b_D;
  emxArray_real_T *c_D;
  emxArray_real_T *Hl;
  emxArray_real_T *zl;
  emxArray_real_T *zml;
  unsigned int count;
  double d_P[2];
  double e_P[2];
  double b_xr[2];
  emxArray_real_T *d_D;
  emxArray_real_T *e_D;
  emxArray_real_T *Hr;
  emxArray_real_T *zr;
  emxArray_real_T *zmr;
  emxArray_real_T *H;
  int i5;
  emxArray_real_T *R;
  int b_R;
  emxArray_real_T *y;
  int ar;
  int k;
  int ic;
  int ib;
  int ia;
  emxArray_real_T *K;
  emxArray_real_T *b_zl;
  emxArray_real_T *b_zml;
  emxArray_real_T *y_tilde;
  emxArray_real_T *b;
  emxArray_real_T *b_K;
  double b_a[100];

  /* % linke Seitenlinie */
  projectPoints(r, delta, 0.38, xp, yp, phi);

  /* Punkte von Mittellinie nach links projizieren */
  for (i4 = 0; i4 < 10; i4++) {
    P[i4] = xp[i4];
    P[10 + i4] = yp[i4];
    P[20 + i4] = phi[i4];
  }

  b_emxInit_real_T(&D, 2);
  xl_idx_0 = xl->size[0];
  i4 = D->size[0] * D->size[1];
  D->size[0] = xl_idx_0;
  D->size[1] = 3;
  emxEnsureCapacity((emxArray__common *)D, i4, (int)sizeof(double));
  br = xl->size[0] * 3;
  for (i4 = 0; i4 < br; i4++) {
    D->data[i4] = 10000.0;
  }

  /*  Fuer jeden Messpunkt den nähesten Punkt der aktuellen Praediktion finden  */
  for (xl_idx_0 = 0; xl_idx_0 < 10; xl_idx_0++) {
    for (m = 0; m < xl->size[0]; m++) {
      dist_point = P[xl_idx_0] - xl->data[m];
      a = P[10 + xl_idx_0] - yl->data[m];
      dist_point = sqrt(dist_point * dist_point + a * a);
      if (dist_point < D->data[m + (D->size[0] << 1)]) {
        D->data[m] = 1.0 + (double)xl_idx_0;
        D->data[m + D->size[0]] = 0.0;
        D->data[m + (D->size[0] << 1)] = dist_point;
      }

      if (1 + xl_idx_0 > 1) {
        b_P[0] = P[xl_idx_0 - 1];
        b_P[1] = P[xl_idx_0 + 9];
        c_P[0] = P[xl_idx_0];
        c_P[1] = P[10 + xl_idx_0];
        b_xl[0] = xl->data[m];
        b_xl[1] = yl->data[m];
        d_line_point(b_P, c_P, b_xl, &dist_point, &a, unusedU1);
        if ((dist_point < D->data[m + (D->size[0] << 1)]) && (a > 0.0) && (a <
             1.0)) {
          D->data[m] = 1.0 + (double)xl_idx_0;
          D->data[m + D->size[0]] = a;
          D->data[m + (D->size[0] << 1)] = dist_point;
        }
      }
    }
  }

  emxInit_boolean_T(&ind, 1);

  /*  Messpunkte ausfiltern, die vor dem letzten prädizierten Punkt liegen */
  br = D->size[0];
  i4 = ind->size[0];
  ind->size[0] = br;
  emxEnsureCapacity((emxArray__common *)ind, i4, (int)sizeof(boolean_T));
  for (i4 = 0; i4 < br; i4++) {
    ind->data[i4] = (((D->data[i4] == 10.0) && (D->data[i4 + D->size[0]] == 0.0))
                     || (D->data[i4 + (D->size[0] << 1)] > 0.5));
  }

  emxInit_real_T(&b_D, 1);
  eml_null_assignment(xl, ind);
  eml_null_assignment(yl, ind);
  b_eml_null_assignment(D, ind);

  /*  Messmatrix, Mess- und Erwartungsvektor bauen */
  br = D->size[0];
  i4 = b_D->size[0];
  b_D->size[0] = br;
  emxEnsureCapacity((emxArray__common *)b_D, i4, (int)sizeof(double));
  for (i4 = 0; i4 < br; i4++) {
    b_D->data[i4] = D->data[i4];
  }

  emxInit_real_T(&c_D, 1);
  br = D->size[0];
  i4 = c_D->size[0];
  c_D->size[0] = br;
  emxEnsureCapacity((emxArray__common *)c_D, i4, (int)sizeof(double));
  for (i4 = 0; i4 < br; i4++) {
    c_D->data[i4] = D->data[i4 + D->size[0]];
  }

  b_emxInit_real_T(&Hl, 2);
  emxInit_real_T(&zl, 1);
  messmatrix_new(P, r, delta, b_D, c_D, Hl);
  i4 = zl->size[0];
  zl->size[0] = (int)(2.0 * (double)xl->size[0]);
  emxEnsureCapacity((emxArray__common *)zl, i4, (int)sizeof(double));
  br = (int)(2.0 * (double)xl->size[0]);
  emxFree_real_T(&c_D);
  emxFree_real_T(&b_D);
  for (i4 = 0; i4 < br; i4++) {
    zl->data[i4] = 0.0;
  }

  emxInit_real_T(&zml, 1);
  i4 = zml->size[0];
  zml->size[0] = (int)(2.0 * (double)xl->size[0]);
  emxEnsureCapacity((emxArray__common *)zml, i4, (int)sizeof(double));
  br = (int)(2.0 * (double)xl->size[0]);
  for (i4 = 0; i4 < br; i4++) {
    zml->data[i4] = 0.0;
  }

  count = 2U;
  for (m = 0; m < xl->size[0]; m++) {
    zl->data[(int)count - 2] = xl->data[m];
    zl->data[(int)count - 1] = yl->data[m];
    if (D->data[m + D->size[0]] > 0.0) {
      zml->data[(int)count - 2] = P[(int)(D->data[m] - 1.0) - 1] + D->data[m +
        D->size[0]] * (P[(int)D->data[m] - 1] - P[(int)(D->data[m] - 1.0) - 1]);
      zml->data[(int)count - 1] = P[(int)(D->data[m] - 1.0) + 9] + D->data[m +
        D->size[0]] * (P[(int)D->data[m] + 9] - P[(int)(D->data[m] - 1.0) + 9]);
    } else {
      zml->data[(int)count - 2] = P[(int)D->data[m] - 1];
      zml->data[(int)count - 1] = P[(int)D->data[m] + 9];
    }

    count += 2U;
  }

  /* % rechte Seitenlinie */
  projectPoints(r, delta, -0.38, xp, yp, phi);

  /* Punkte von Mittellinie nach rechts projizieren */
  for (i4 = 0; i4 < 10; i4++) {
    P[i4] = xp[i4];
    P[10 + i4] = yp[i4];
    P[20 + i4] = phi[i4];
  }

  xl_idx_0 = xr->size[0];
  i4 = D->size[0] * D->size[1];
  D->size[0] = xl_idx_0;
  D->size[1] = 3;
  emxEnsureCapacity((emxArray__common *)D, i4, (int)sizeof(double));
  br = xr->size[0] * 3;
  for (i4 = 0; i4 < br; i4++) {
    D->data[i4] = 10000.0;
  }

  /*  Fuer jeden Messpunkt den nähesten Punkt der aktuellen Praediktion finden */
  for (xl_idx_0 = 0; xl_idx_0 < 10; xl_idx_0++) {
    for (m = 0; m < xr->size[0]; m++) {
      dist_point = P[xl_idx_0] - xr->data[m];
      a = P[10 + xl_idx_0] - yr->data[m];
      dist_point = sqrt(dist_point * dist_point + a * a);
      if (dist_point < D->data[m + (D->size[0] << 1)]) {
        D->data[m] = 1.0 + (double)xl_idx_0;
        D->data[m + D->size[0]] = 0.0;
        D->data[m + (D->size[0] << 1)] = dist_point;
      }

      if (1 + xl_idx_0 > 1) {
        d_P[0] = P[xl_idx_0 - 1];
        d_P[1] = P[xl_idx_0 + 9];
        e_P[0] = P[xl_idx_0];
        e_P[1] = P[10 + xl_idx_0];
        b_xr[0] = xr->data[m];
        b_xr[1] = yr->data[m];
        d_line_point(d_P, e_P, b_xr, &dist_point, &a, unusedU1);
        if ((dist_point < D->data[m + (D->size[0] << 1)]) && (a > 0.0) && (a <
             1.0)) {
          D->data[m] = 1.0 + (double)xl_idx_0;
          D->data[m + D->size[0]] = a;
          D->data[m + (D->size[0] << 1)] = dist_point;
        }
      }
    }
  }

  /*  Messpunkte ausfiltern, die vor dem letzten prädizierten Punkt liegen */
  br = D->size[0];
  i4 = ind->size[0];
  ind->size[0] = br;
  emxEnsureCapacity((emxArray__common *)ind, i4, (int)sizeof(boolean_T));
  for (i4 = 0; i4 < br; i4++) {
    ind->data[i4] = (((D->data[i4] == 10.0) && (D->data[i4 + D->size[0]] == 0.0))
                     || (D->data[i4 + (D->size[0] << 1)] > 0.5));
  }

  emxInit_real_T(&d_D, 1);
  eml_null_assignment(xr, ind);
  eml_null_assignment(yr, ind);
  b_eml_null_assignment(D, ind);

  /*  Messmatrix, Mess- und Erwartungsvektor bauen */
  br = D->size[0];
  i4 = d_D->size[0];
  d_D->size[0] = br;
  emxEnsureCapacity((emxArray__common *)d_D, i4, (int)sizeof(double));
  emxFree_boolean_T(&ind);
  for (i4 = 0; i4 < br; i4++) {
    d_D->data[i4] = D->data[i4];
  }

  emxInit_real_T(&e_D, 1);
  br = D->size[0];
  i4 = e_D->size[0];
  e_D->size[0] = br;
  emxEnsureCapacity((emxArray__common *)e_D, i4, (int)sizeof(double));
  for (i4 = 0; i4 < br; i4++) {
    e_D->data[i4] = D->data[i4 + D->size[0]];
  }

  b_emxInit_real_T(&Hr, 2);
  emxInit_real_T(&zr, 1);
  messmatrix_new(P, r, delta, d_D, e_D, Hr);
  i4 = zr->size[0];
  zr->size[0] = (int)(2.0 * (double)xr->size[0]);
  emxEnsureCapacity((emxArray__common *)zr, i4, (int)sizeof(double));
  br = (int)(2.0 * (double)xr->size[0]);
  emxFree_real_T(&e_D);
  emxFree_real_T(&d_D);
  for (i4 = 0; i4 < br; i4++) {
    zr->data[i4] = 0.0;
  }

  emxInit_real_T(&zmr, 1);
  i4 = zmr->size[0];
  zmr->size[0] = (int)(2.0 * (double)xr->size[0]);
  emxEnsureCapacity((emxArray__common *)zmr, i4, (int)sizeof(double));
  br = (int)(2.0 * (double)xr->size[0]);
  for (i4 = 0; i4 < br; i4++) {
    zmr->data[i4] = 0.0;
  }

  count = 2U;
  for (m = 0; m < xr->size[0]; m++) {
    zr->data[(int)count - 2] = xr->data[m];
    zr->data[(int)count - 1] = yr->data[m];
    if (D->data[m + D->size[0]] > 0.0) {
      zmr->data[(int)count - 2] = P[(int)(D->data[m] - 1.0) - 1] + D->data[m +
        D->size[0]] * (P[(int)D->data[m] - 1] - P[(int)(D->data[m] - 1.0) - 1]);
      zmr->data[(int)count - 1] = P[(int)(D->data[m] - 1.0) + 9] + D->data[m +
        D->size[0]] * (P[(int)D->data[m] + 9] - P[(int)(D->data[m] - 1.0) + 9]);
    } else {
      zmr->data[(int)count - 2] = P[(int)D->data[m] - 1];
      zmr->data[(int)count - 1] = P[(int)D->data[m] + 9];
    }

    count += 2U;
  }

  emxFree_real_T(&D);
  b_emxInit_real_T(&H, 2);

  /* % Linke und rechte Linie kombinieren */
  i4 = H->size[0] * H->size[1];
  H->size[0] = Hl->size[0] + Hr->size[0];
  H->size[1] = 10;
  emxEnsureCapacity((emxArray__common *)H, i4, (int)sizeof(double));
  for (i4 = 0; i4 < 10; i4++) {
    br = Hl->size[0];
    for (i5 = 0; i5 < br; i5++) {
      H->data[i5 + H->size[0] * i4] = Hl->data[i5 + Hl->size[0] * i4];
    }
  }

  for (i4 = 0; i4 < 10; i4++) {
    br = Hr->size[0];
    for (i5 = 0; i5 < br; i5++) {
      H->data[(i5 + Hl->size[0]) + H->size[0] * i4] = Hr->data[i5 + Hr->size[0] *
        i4];
    }
  }

  emxFree_real_T(&Hr);
  b_emxInit_real_T(&R, 2);

  /* % KALMAN FILTER     */
  /*  r = A*r + B*u */
  eye(2.0 * (double)((unsigned int)xl->size[0] + xr->size[0]), R);
  i4 = R->size[0] * R->size[1];
  emxEnsureCapacity((emxArray__common *)R, i4, (int)sizeof(double));
  xl_idx_0 = R->size[0];
  b_R = R->size[1];
  br = xl_idx_0 * b_R;
  for (i4 = 0; i4 < br; i4++) {
    R->data[i4] *= R_fakt;
  }

  b_emxInit_real_T(&y, 2);
  if ((A->size[1] == 1) || (Pk->size[0] == 1)) {
    i4 = y->size[0] * y->size[1];
    y->size[0] = A->size[0];
    y->size[1] = Pk->size[1];
    emxEnsureCapacity((emxArray__common *)y, i4, (int)sizeof(double));
    br = A->size[0];
    for (i4 = 0; i4 < br; i4++) {
      ar = Pk->size[1];
      for (i5 = 0; i5 < ar; i5++) {
        y->data[i4 + y->size[0] * i5] = 0.0;
        b_R = A->size[1];
        for (xl_idx_0 = 0; xl_idx_0 < b_R; xl_idx_0++) {
          y->data[i4 + y->size[0] * i5] += A->data[i4 + A->size[0] * xl_idx_0] *
            Pk->data[xl_idx_0 + Pk->size[0] * i5];
        }
      }
    }
  } else {
    k = A->size[1];
    unusedU1[0] = (unsigned int)A->size[0];
    unusedU1[1] = (unsigned int)Pk->size[1];
    m = A->size[0];
    i4 = y->size[0] * y->size[1];
    y->size[0] = (int)unusedU1[0];
    emxEnsureCapacity((emxArray__common *)y, i4, (int)sizeof(double));
    i4 = y->size[0] * y->size[1];
    y->size[1] = (int)unusedU1[1];
    emxEnsureCapacity((emxArray__common *)y, i4, (int)sizeof(double));
    br = (int)unusedU1[0] * (int)unusedU1[1];
    for (i4 = 0; i4 < br; i4++) {
      y->data[i4] = 0.0;
    }

    if ((A->size[0] == 0) || (Pk->size[1] == 0)) {
    } else {
      xl_idx_0 = A->size[0] * (Pk->size[1] - 1);
      b_R = 0;
      while ((m > 0) && (b_R <= xl_idx_0)) {
        i4 = b_R + m;
        for (ic = b_R; ic + 1 <= i4; ic++) {
          y->data[ic] = 0.0;
        }

        b_R += m;
      }

      br = 0;
      b_R = 0;
      while ((m > 0) && (b_R <= xl_idx_0)) {
        ar = -1;
        i4 = br + k;
        for (ib = br; ib + 1 <= i4; ib++) {
          if (Pk->data[ib] != 0.0) {
            ia = ar;
            i5 = b_R + m;
            for (ic = b_R; ic + 1 <= i5; ic++) {
              ia++;
              y->data[ic] += Pk->data[ib] * A->data[ia];
            }
          }

          ar += m;
        }

        br += k;
        b_R += m;
      }
    }
  }

  b_emxInit_real_T(&K, 2);
  i4 = K->size[0] * K->size[1];
  K->size[0] = A->size[1];
  K->size[1] = A->size[0];
  emxEnsureCapacity((emxArray__common *)K, i4, (int)sizeof(double));
  br = A->size[0];
  for (i4 = 0; i4 < br; i4++) {
    ar = A->size[1];
    for (i5 = 0; i5 < ar; i5++) {
      K->data[i5 + K->size[0] * i4] = A->data[i4 + A->size[0] * i5];
    }
  }

  if ((y->size[1] == 1) || (K->size[0] == 1)) {
    i4 = Pk->size[0] * Pk->size[1];
    Pk->size[0] = y->size[0];
    Pk->size[1] = K->size[1];
    emxEnsureCapacity((emxArray__common *)Pk, i4, (int)sizeof(double));
    br = y->size[0];
    for (i4 = 0; i4 < br; i4++) {
      ar = K->size[1];
      for (i5 = 0; i5 < ar; i5++) {
        Pk->data[i4 + Pk->size[0] * i5] = 0.0;
        b_R = y->size[1];
        for (xl_idx_0 = 0; xl_idx_0 < b_R; xl_idx_0++) {
          Pk->data[i4 + Pk->size[0] * i5] += y->data[i4 + y->size[0] * xl_idx_0]
            * K->data[xl_idx_0 + K->size[0] * i5];
        }
      }
    }
  } else {
    k = y->size[1];
    unusedU1[0] = (unsigned int)y->size[0];
    unusedU1[1] = (unsigned int)K->size[1];
    m = y->size[0];
    i4 = Pk->size[0] * Pk->size[1];
    Pk->size[0] = (int)unusedU1[0];
    Pk->size[1] = (int)unusedU1[1];
    emxEnsureCapacity((emxArray__common *)Pk, i4, (int)sizeof(double));
    br = (int)unusedU1[1];
    for (i4 = 0; i4 < br; i4++) {
      ar = (int)unusedU1[0];
      for (i5 = 0; i5 < ar; i5++) {
        Pk->data[i5 + Pk->size[0] * i4] = 0.0;
      }
    }

    if ((y->size[0] == 0) || (K->size[1] == 0)) {
    } else {
      xl_idx_0 = y->size[0] * (K->size[1] - 1);
      b_R = 0;
      while ((m > 0) && (b_R <= xl_idx_0)) {
        i4 = b_R + m;
        for (ic = b_R; ic + 1 <= i4; ic++) {
          Pk->data[ic] = 0.0;
        }

        b_R += m;
      }

      br = 0;
      b_R = 0;
      while ((m > 0) && (b_R <= xl_idx_0)) {
        ar = -1;
        i4 = br + k;
        for (ib = br; ib + 1 <= i4; ib++) {
          if (K->data[ib] != 0.0) {
            ia = ar;
            i5 = b_R + m;
            for (ic = b_R; ic + 1 <= i5; ic++) {
              ia++;
              Pk->data[ic] += K->data[ib] * y->data[ia];
            }
          }

          ar += m;
        }

        br += k;
        b_R += m;
      }
    }
  }

  i4 = Pk->size[0] * Pk->size[1];
  emxEnsureCapacity((emxArray__common *)Pk, i4, (int)sizeof(double));
  br = Pk->size[1];
  for (i4 = 0; i4 < br; i4++) {
    ar = Pk->size[0];
    for (i5 = 0; i5 < ar; i5++) {
      Pk->data[i5 + Pk->size[0] * i4] += Q->data[i5 + Q->size[0] * i4];
    }
  }

  emxInit_real_T(&b_zl, 1);
  emxInit_real_T(&b_zml, 1);
  i4 = b_zl->size[0];
  b_zl->size[0] = zl->size[0] + zr->size[0];
  emxEnsureCapacity((emxArray__common *)b_zl, i4, (int)sizeof(double));
  br = zl->size[0];
  for (i4 = 0; i4 < br; i4++) {
    b_zl->data[i4] = zl->data[i4];
  }

  br = zr->size[0];
  for (i4 = 0; i4 < br; i4++) {
    b_zl->data[i4 + zl->size[0]] = zr->data[i4];
  }

  emxFree_real_T(&zr);
  i4 = b_zml->size[0];
  b_zml->size[0] = zml->size[0] + zmr->size[0];
  emxEnsureCapacity((emxArray__common *)b_zml, i4, (int)sizeof(double));
  br = zml->size[0];
  for (i4 = 0; i4 < br; i4++) {
    b_zml->data[i4] = zml->data[i4];
  }

  br = zmr->size[0];
  for (i4 = 0; i4 < br; i4++) {
    b_zml->data[i4 + zml->size[0]] = zmr->data[i4];
  }

  emxFree_real_T(&zmr);
  emxFree_real_T(&zml);
  emxInit_real_T(&y_tilde, 1);
  i4 = y_tilde->size[0];
  y_tilde->size[0] = b_zl->size[0];
  emxEnsureCapacity((emxArray__common *)y_tilde, i4, (int)sizeof(double));
  br = b_zl->size[0];
  for (i4 = 0; i4 < br; i4++) {
    y_tilde->data[i4] = b_zl->data[i4] - b_zml->data[i4];
  }

  emxFree_real_T(&b_zml);
  emxFree_real_T(&b_zl);
  if (Pk->size[0] == 1) {
    i4 = y->size[0] * y->size[1];
    y->size[0] = H->size[0];
    y->size[1] = Pk->size[1];
    emxEnsureCapacity((emxArray__common *)y, i4, (int)sizeof(double));
    br = H->size[0];
    for (i4 = 0; i4 < br; i4++) {
      ar = Pk->size[1];
      for (i5 = 0; i5 < ar; i5++) {
        y->data[i4 + y->size[0] * i5] = 0.0;
        for (xl_idx_0 = 0; xl_idx_0 < 10; xl_idx_0++) {
          y->data[i4 + y->size[0] * i5] += H->data[i4 + H->size[0] * xl_idx_0] *
            Pk->data[xl_idx_0 + Pk->size[0] * i5];
        }
      }
    }
  } else {
    unusedU1[0] = (unsigned int)H->size[0];
    unusedU1[1] = (unsigned int)Pk->size[1];
    m = H->size[0];
    i4 = y->size[0] * y->size[1];
    y->size[0] = (int)unusedU1[0];
    emxEnsureCapacity((emxArray__common *)y, i4, (int)sizeof(double));
    i4 = y->size[0] * y->size[1];
    y->size[1] = (int)unusedU1[1];
    emxEnsureCapacity((emxArray__common *)y, i4, (int)sizeof(double));
    br = (int)unusedU1[0] * (int)unusedU1[1];
    for (i4 = 0; i4 < br; i4++) {
      y->data[i4] = 0.0;
    }

    if ((H->size[0] == 0) || (Pk->size[1] == 0)) {
    } else {
      xl_idx_0 = H->size[0] * (Pk->size[1] - 1);
      b_R = 0;
      while ((m > 0) && (b_R <= xl_idx_0)) {
        i4 = b_R + m;
        for (ic = b_R; ic + 1 <= i4; ic++) {
          y->data[ic] = 0.0;
        }

        b_R += m;
      }

      br = 0;
      b_R = 0;
      while ((m > 0) && (b_R <= xl_idx_0)) {
        ar = -1;
        for (ib = br; ib + 1 <= br + 10; ib++) {
          if (Pk->data[ib] != 0.0) {
            ia = ar;
            i4 = b_R + m;
            for (ic = b_R; ic + 1 <= i4; ic++) {
              ia++;
              y->data[ic] += Pk->data[ib] * H->data[ia];
            }
          }

          ar += m;
        }

        br += 10;
        b_R += m;
      }
    }
  }

  b_emxInit_real_T(&b, 2);
  i4 = b->size[0] * b->size[1];
  b->size[0] = 10;
  b->size[1] = H->size[0];
  emxEnsureCapacity((emxArray__common *)b, i4, (int)sizeof(double));
  br = H->size[0];
  for (i4 = 0; i4 < br; i4++) {
    for (i5 = 0; i5 < 10; i5++) {
      b->data[i5 + b->size[0] * i4] = H->data[i4 + H->size[0] * i5];
    }
  }

  if (y->size[1] == 1) {
    i4 = K->size[0] * K->size[1];
    K->size[0] = y->size[0];
    K->size[1] = b->size[1];
    emxEnsureCapacity((emxArray__common *)K, i4, (int)sizeof(double));
    br = y->size[0];
    for (i4 = 0; i4 < br; i4++) {
      ar = b->size[1];
      for (i5 = 0; i5 < ar; i5++) {
        K->data[i4 + K->size[0] * i5] = 0.0;
        b_R = y->size[1];
        for (xl_idx_0 = 0; xl_idx_0 < b_R; xl_idx_0++) {
          K->data[i4 + K->size[0] * i5] += y->data[i4 + y->size[0] * xl_idx_0] *
            b->data[xl_idx_0 + b->size[0] * i5];
        }
      }
    }
  } else {
    k = y->size[1];
    unusedU1[0] = (unsigned int)y->size[0];
    unusedU1[1] = (unsigned int)b->size[1];
    m = y->size[0];
    i4 = K->size[0] * K->size[1];
    K->size[0] = (int)unusedU1[0];
    emxEnsureCapacity((emxArray__common *)K, i4, (int)sizeof(double));
    i4 = K->size[0] * K->size[1];
    K->size[1] = (int)unusedU1[1];
    emxEnsureCapacity((emxArray__common *)K, i4, (int)sizeof(double));
    br = (int)unusedU1[0] * (int)unusedU1[1];
    for (i4 = 0; i4 < br; i4++) {
      K->data[i4] = 0.0;
    }

    if ((y->size[0] == 0) || (b->size[1] == 0)) {
    } else {
      xl_idx_0 = y->size[0] * (b->size[1] - 1);
      b_R = 0;
      while ((m > 0) && (b_R <= xl_idx_0)) {
        i4 = b_R + m;
        for (ic = b_R; ic + 1 <= i4; ic++) {
          K->data[ic] = 0.0;
        }

        b_R += m;
      }

      br = 0;
      b_R = 0;
      while ((m > 0) && (b_R <= xl_idx_0)) {
        ar = -1;
        i4 = br + k;
        for (ib = br; ib + 1 <= i4; ib++) {
          if (b->data[ib] != 0.0) {
            ia = ar;
            i5 = b_R + m;
            for (ic = b_R; ic + 1 <= i5; ic++) {
              ia++;
              K->data[ic] += b->data[ib] * y->data[ia];
            }
          }

          ar += m;
        }

        br += k;
        b_R += m;
      }
    }
  }

  i4 = b->size[0] * b->size[1];
  b->size[0] = 10;
  b->size[1] = H->size[0];
  emxEnsureCapacity((emxArray__common *)b, i4, (int)sizeof(double));
  br = H->size[0];
  for (i4 = 0; i4 < br; i4++) {
    for (i5 = 0; i5 < 10; i5++) {
      b->data[i5 + b->size[0] * i4] = H->data[i4 + H->size[0] * i5];
    }
  }

  if (Pk->size[1] == 1) {
    i4 = y->size[0] * y->size[1];
    y->size[0] = Pk->size[0];
    y->size[1] = b->size[1];
    emxEnsureCapacity((emxArray__common *)y, i4, (int)sizeof(double));
    br = Pk->size[0];
    for (i4 = 0; i4 < br; i4++) {
      ar = b->size[1];
      for (i5 = 0; i5 < ar; i5++) {
        y->data[i4 + y->size[0] * i5] = 0.0;
        b_R = Pk->size[1];
        for (xl_idx_0 = 0; xl_idx_0 < b_R; xl_idx_0++) {
          y->data[i4 + y->size[0] * i5] += Pk->data[i4 + Pk->size[0] * xl_idx_0]
            * b->data[xl_idx_0 + b->size[0] * i5];
        }
      }
    }
  } else {
    k = Pk->size[1];
    unusedU1[0] = (unsigned int)Pk->size[0];
    unusedU1[1] = (unsigned int)b->size[1];
    m = Pk->size[0];
    i4 = y->size[0] * y->size[1];
    y->size[0] = (int)unusedU1[0];
    emxEnsureCapacity((emxArray__common *)y, i4, (int)sizeof(double));
    i4 = y->size[0] * y->size[1];
    y->size[1] = (int)unusedU1[1];
    emxEnsureCapacity((emxArray__common *)y, i4, (int)sizeof(double));
    br = (int)unusedU1[0] * (int)unusedU1[1];
    for (i4 = 0; i4 < br; i4++) {
      y->data[i4] = 0.0;
    }

    if ((Pk->size[0] == 0) || (b->size[1] == 0)) {
    } else {
      xl_idx_0 = Pk->size[0] * (b->size[1] - 1);
      b_R = 0;
      while (((m > 0) && (b_R <= xl_idx_0)) || ((m < 0) && (b_R >= xl_idx_0))) {
        i4 = b_R + m;
        for (ic = b_R; ic + 1 <= i4; ic++) {
          y->data[ic] = 0.0;
        }

        b_R += m;
      }

      br = 0;
      b_R = 0;
      while (((m > 0) && (b_R <= xl_idx_0)) || ((m < 0) && (b_R >= xl_idx_0))) {
        ar = -1;
        i4 = br + k;
        for (ib = br; ib + 1 <= i4; ib++) {
          if (b->data[ib] != 0.0) {
            ia = ar;
            i5 = b_R + m;
            for (ic = b_R; ic + 1 <= i5; ic++) {
              ia++;
              y->data[ic] += b->data[ib] * Pk->data[ia];
            }
          }

          ar += m;
        }

        br += k;
        b_R += m;
      }
    }
  }

  b_emxInit_real_T(&b_K, 2);
  i4 = b_K->size[0] * b_K->size[1];
  b_K->size[0] = K->size[0];
  b_K->size[1] = K->size[1];
  emxEnsureCapacity((emxArray__common *)b_K, i4, (int)sizeof(double));
  br = K->size[0] * K->size[1];
  for (i4 = 0; i4 < br; i4++) {
    b_K->data[i4] = K->data[i4] + R->data[i4];
  }

  emxFree_real_T(&R);
  mrdivide(y, b_K, K);
  emxFree_real_T(&b_K);
  emxFree_real_T(&y);
  if ((K->size[1] == 1) || (y_tilde->size[0] == 1)) {
    i4 = zl->size[0];
    zl->size[0] = K->size[0];
    emxEnsureCapacity((emxArray__common *)zl, i4, (int)sizeof(double));
    br = K->size[0];
    for (i4 = 0; i4 < br; i4++) {
      zl->data[i4] = 0.0;
      ar = K->size[1];
      for (i5 = 0; i5 < ar; i5++) {
        zl->data[i4] += K->data[i4 + K->size[0] * i5] * y_tilde->data[i5];
      }
    }
  } else {
    k = K->size[1];
    unusedU1[0] = K->size[0];
    m = K->size[0];
    i4 = zl->size[0];
    zl->size[0] = (int)unusedU1[0];
    emxEnsureCapacity((emxArray__common *)zl, i4, (int)sizeof(double));
    br = (int)unusedU1[0];
    for (i4 = 0; i4 < br; i4++) {
      zl->data[i4] = 0.0;
    }

    if (K->size[0] == 0) {
    } else {
      b_R = 0;
      while ((m > 0) && (b_R <= 0)) {
        for (ic = 0; ic + 1 <= m; ic++) {
          zl->data[ic] = 0.0;
        }

        b_R = m;
      }

      br = 0;
      b_R = 0;
      while ((m > 0) && (b_R <= 0)) {
        ar = -1;
        i4 = br + k;
        for (ib = br; ib + 1 <= i4; ib++) {
          if (y_tilde->data[ib] != 0.0) {
            ia = ar;
            for (ic = 0; ic + 1 <= m; ic++) {
              ia++;
              zl->data[ic] += y_tilde->data[ib] * K->data[ia];
            }
          }

          ar += m;
        }

        br += k;
        b_R = m;
      }
    }
  }

  emxFree_real_T(&y_tilde);
  for (i4 = 0; i4 < 10; i4++) {
    r[i4] += zl->data[i4];
  }

  emxFree_real_T(&zl);
  if ((K->size[1] == 1) || (H->size[0] == 1)) {
    i4 = Hl->size[0] * Hl->size[1];
    Hl->size[0] = K->size[0];
    Hl->size[1] = 10;
    emxEnsureCapacity((emxArray__common *)Hl, i4, (int)sizeof(double));
    br = K->size[0];
    for (i4 = 0; i4 < br; i4++) {
      for (i5 = 0; i5 < 10; i5++) {
        Hl->data[i4 + Hl->size[0] * i5] = 0.0;
        ar = K->size[1];
        for (xl_idx_0 = 0; xl_idx_0 < ar; xl_idx_0++) {
          Hl->data[i4 + Hl->size[0] * i5] += K->data[i4 + K->size[0] * xl_idx_0]
            * H->data[xl_idx_0 + H->size[0] * i5];
        }
      }
    }
  } else {
    k = K->size[1];
    unusedU1[0] = K->size[0];
    m = K->size[0];
    i4 = Hl->size[0] * Hl->size[1];
    Hl->size[0] = (int)unusedU1[0];
    Hl->size[1] = 10;
    emxEnsureCapacity((emxArray__common *)Hl, i4, (int)sizeof(double));
    br = (int)unusedU1[0] * 10;
    for (i4 = 0; i4 < br; i4++) {
      Hl->data[i4] = 0.0;
    }

    if (K->size[0] == 0) {
    } else {
      xl_idx_0 = K->size[0] * 9;
      b_R = 0;
      while ((m > 0) && (b_R <= xl_idx_0)) {
        i4 = b_R + m;
        for (ic = b_R; ic + 1 <= i4; ic++) {
          Hl->data[ic] = 0.0;
        }

        b_R += m;
      }

      br = 0;
      b_R = 0;
      while ((m > 0) && (b_R <= xl_idx_0)) {
        ar = -1;
        i4 = br + k;
        for (ib = br; ib + 1 <= i4; ib++) {
          if (H->data[ib] != 0.0) {
            ia = ar;
            i5 = b_R + m;
            for (ic = b_R; ic + 1 <= i5; ic++) {
              ia++;
              Hl->data[ic] += H->data[ib] * K->data[ia];
            }
          }

          ar += m;
        }

        br += k;
        b_R += m;
      }
    }
  }

  emxFree_real_T(&K);
  emxFree_real_T(&H);
  b_eye(b_a);
  for (i4 = 0; i4 < 100; i4++) {
    dist_point = b_a[i4] - Hl->data[i4];
    b_a[i4] = dist_point;
  }

  emxFree_real_T(&Hl);
  if (Pk->size[0] == 1) {
    i4 = b->size[0] * b->size[1];
    b->size[0] = 10;
    b->size[1] = Pk->size[1];
    emxEnsureCapacity((emxArray__common *)b, i4, (int)sizeof(double));
    for (i4 = 0; i4 < 10; i4++) {
      br = Pk->size[1];
      for (i5 = 0; i5 < br; i5++) {
        b->data[i4 + b->size[0] * i5] = 0.0;
        for (xl_idx_0 = 0; xl_idx_0 < 10; xl_idx_0++) {
          b->data[i4 + b->size[0] * i5] += b_a[i4 + 10 * xl_idx_0] * Pk->
            data[xl_idx_0 + Pk->size[0] * i5];
        }
      }
    }
  } else {
    unusedU1[1] = Pk->size[1];
    i4 = b->size[0] * b->size[1];
    b->size[0] = 10;
    emxEnsureCapacity((emxArray__common *)b, i4, (int)sizeof(double));
    i4 = b->size[0] * b->size[1];
    b->size[1] = (int)unusedU1[1];
    emxEnsureCapacity((emxArray__common *)b, i4, (int)sizeof(double));
    br = 10 * (int)unusedU1[1];
    for (i4 = 0; i4 < br; i4++) {
      b->data[i4] = 0.0;
    }

    if (Pk->size[1] == 0) {
    } else {
      xl_idx_0 = 10 * (Pk->size[1] - 1);
      for (b_R = 0; b_R <= xl_idx_0; b_R += 10) {
        for (ic = b_R + 1; ic <= b_R + 10; ic++) {
          b->data[ic - 1] = 0.0;
        }
      }

      br = 0;
      for (b_R = 0; b_R <= xl_idx_0; b_R += 10) {
        ar = -1;
        for (ib = br; ib + 1 <= br + 10; ib++) {
          if (Pk->data[ib] != 0.0) {
            ia = ar;
            for (ic = b_R; ic + 1 <= b_R + 10; ic++) {
              ia++;
              b->data[ic] += Pk->data[ib] * b_a[ia];
            }
          }

          ar += 10;
        }

        br += 10;
      }
    }
  }

  i4 = Pk->size[0] * Pk->size[1];
  Pk->size[0] = 10;
  Pk->size[1] = b->size[1];
  emxEnsureCapacity((emxArray__common *)Pk, i4, (int)sizeof(double));
  br = b->size[1];
  for (i4 = 0; i4 < br; i4++) {
    for (i5 = 0; i5 < 10; i5++) {
      Pk->data[i5 + Pk->size[0] * i4] = b->data[i5 + b->size[0] * i4];
    }
  }

  emxFree_real_T(&b);

  /* % Zustandsbegrenzungen */
  /*  Krümmung */
  /*  bezogen auf Mittellinie, 0.72 entspricht einem minimalen Innenradius von 1m */
  for (xl_idx_0 = 0; xl_idx_0 < 8; xl_idx_0++) {
    if ((-0.75 >= r[xl_idx_0 + 2]) || rtIsNaN(r[xl_idx_0 + 2])) {
      r[xl_idx_0 + 2] = -0.75;
    }

    if ((0.75 <= r[xl_idx_0 + 2]) || rtIsNaN(r[xl_idx_0 + 2])) {
      r[xl_idx_0 + 2] = 0.75;
    }
  }

  /*  y-Wert des ersten Punktes */
  if ((0.5 <= r[0]) || rtIsNaN(r[0])) {
    dist_point = 0.5;
  } else {
    dist_point = r[0];
  }

  if ((-0.5 >= dist_point) || rtIsNaN(dist_point)) {
    r[0] = -0.5;
  } else {
    r[0] = dist_point;
  }

  /*  Startwinkel  */
  if ((0.78 <= r[1]) || rtIsNaN(r[1])) {
    dist_point = 0.78;
  } else {
    dist_point = r[1];
  }

  if ((-0.78 >= dist_point) || rtIsNaN(dist_point)) {
    r[1] = -0.78;
  } else {
    r[1] = dist_point;
  }

  /*  45° */
}

/*
 * File trailer for kalman_filter_lr.c
 *
 * [EOF]
 */
