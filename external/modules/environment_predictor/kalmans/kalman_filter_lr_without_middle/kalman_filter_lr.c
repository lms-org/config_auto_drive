/*
 * File: kalman_filter_lr.c
 *
 * MATLAB Coder version            : 2.7
 * C/C++ source code generated on  : 28-May-2015 16:02:10
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
  int i7;
  int j;
  emxArray_real_T *b_x;
  nrows = 0;
  for (k = 1; k <= idx->size[0]; k++) {
    nrows += idx->data[k - 1];
  }

  nrows = x->size[0] - nrows;
  i = 0;
  i7 = x->size[0];
  for (k = 1; k <= i7; k++) {
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
  i7 = b_x->size[0] * b_x->size[1];
  b_x->size[0] = nrows;
  b_x->size[1] = 3;
  emxEnsureCapacity((emxArray__common *)b_x, i7, (int)sizeof(double));
  for (i7 = 0; i7 < 3; i7++) {
    for (i = 0; i < nrows; i++) {
      b_x->data[i + b_x->size[0] * i7] = x->data[i + x->size[0] * i7];
    }
  }

  i7 = x->size[0] * x->size[1];
  x->size[0] = b_x->size[0];
  x->size[1] = 3;
  emxEnsureCapacity((emxArray__common *)x, i7, (int)sizeof(double));
  for (i7 = 0; i7 < 3; i7++) {
    nrows = b_x->size[0];
    for (i = 0; i < nrows; i++) {
      x->data[i + x->size[0] * i7] = b_x->data[i + b_x->size[0] * i7];
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
 * Arguments    : emxArray_real_T *r
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
void kalman_filter_lr(emxArray_real_T *r, const emxArray_real_T *A,
                      emxArray_real_T *Pk, const emxArray_real_T *Q, double
                      R_fakt, double delta, emxArray_real_T *xl, emxArray_real_T
                      *yl, emxArray_real_T *xr, emxArray_real_T *yr)
{
  emxArray_real_T *P;
  emxArray_real_T *zr;
  emxArray_real_T *zmr;
  emxArray_real_T *y_tilde;
  int zr_idx_0;
  int zmr_idx_0;
  int br;
  int i5;
  emxArray_real_T *D;
  int ar;
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
  emxArray_real_T *H;
  int i6;
  int k;
  int ic;
  int ib;
  int ia;
  emxArray_real_T *b;
  emxArray_real_T *b_zl;
  emxArray_real_T *b_zml;
  emxArray_real_T *C;
  emxArray_real_T *b_C;
  emxArray_real_T *b_Hr;
  b_emxInit_real_T(&P, 2);
  emxInit_real_T(&zr, 1);
  emxInit_real_T(&zmr, 1);
  emxInit_real_T(&y_tilde, 1);

  /* % linke Seitenlinie */
  projectPoints(r, delta, zr, zmr, y_tilde);

  /* Punkte von Mittellinie nach links projizieren */
  zr_idx_0 = zr->size[0];
  zmr_idx_0 = zmr->size[0];
  br = y_tilde->size[0];
  i5 = P->size[0] * P->size[1];
  P->size[0] = zr_idx_0;
  P->size[1] = 3;
  emxEnsureCapacity((emxArray__common *)P, i5, (int)sizeof(double));
  for (i5 = 0; i5 < zr_idx_0; i5++) {
    P->data[i5] = zr->data[i5];
  }

  for (i5 = 0; i5 < zmr_idx_0; i5++) {
    P->data[i5 + P->size[0]] = zmr->data[i5];
  }

  for (i5 = 0; i5 < br; i5++) {
    P->data[i5 + (P->size[0] << 1)] = y_tilde->data[i5];
  }

  b_emxInit_real_T(&D, 2);
  zr_idx_0 = xl->size[0];
  i5 = D->size[0] * D->size[1];
  D->size[0] = zr_idx_0;
  D->size[1] = 3;
  emxEnsureCapacity((emxArray__common *)D, i5, (int)sizeof(double));
  ar = xl->size[0] * 3;
  for (i5 = 0; i5 < ar; i5++) {
    D->data[i5] = 10000.0;
  }

  /*  Fuer jeden Messpunkt den nähesten Punkt der aktuellen Praediktion finden  */
  for (zr_idx_0 = 0; zr_idx_0 < r->size[0]; zr_idx_0++) {
    for (m = 0; m < xl->size[0]; m++) {
      dist_point = P->data[zr_idx_0] - xl->data[m];
      a = P->data[zr_idx_0 + P->size[0]] - yl->data[m];
      dist_point = sqrt(dist_point * dist_point + a * a);
      if (dist_point < D->data[m + (D->size[0] << 1)]) {
        D->data[m] = 1.0 + (double)zr_idx_0;
        D->data[m + D->size[0]] = 0.0;
        D->data[m + (D->size[0] << 1)] = dist_point;
      }

      if (1.0 + (double)zr_idx_0 > 1.0) {
        b_P[0] = P->data[(int)((1.0 + (double)zr_idx_0) - 1.0) - 1];
        b_P[1] = P->data[((int)((1.0 + (double)zr_idx_0) - 1.0) + P->size[0]) -
          1];
        c_P[0] = P->data[zr_idx_0];
        c_P[1] = P->data[zr_idx_0 + P->size[0]];
        b_xl[0] = xl->data[m];
        b_xl[1] = yl->data[m];
        d_line_point(b_P, c_P, b_xl, &dist_point, &a, unusedU1);
        if ((dist_point < D->data[m + (D->size[0] << 1)]) && (a > 0.0) && (a <
             1.0)) {
          D->data[m] = 1.0 + (double)zr_idx_0;
          D->data[m + D->size[0]] = a;
          D->data[m + (D->size[0] << 1)] = dist_point;
        }
      }
    }
  }

  emxInit_boolean_T(&ind, 1);

  /*  Messpunkte ausfiltern, die vor dem letzten prädizierten Punkt liegen */
  ar = D->size[0];
  zr_idx_0 = r->size[0];
  i5 = ind->size[0];
  ind->size[0] = ar;
  emxEnsureCapacity((emxArray__common *)ind, i5, (int)sizeof(boolean_T));
  for (i5 = 0; i5 < ar; i5++) {
    ind->data[i5] = (((D->data[i5] == zr_idx_0) && (D->data[i5 + D->size[0]] ==
      0.0)) || (D->data[i5 + (D->size[0] << 1)] > 0.5));
  }

  emxInit_real_T(&b_D, 1);
  eml_null_assignment(xl, ind);
  eml_null_assignment(yl, ind);
  b_eml_null_assignment(D, ind);

  /*  Messmatrix, Mess- und Erwartungsvektor bauen */
  ar = D->size[0];
  i5 = b_D->size[0];
  b_D->size[0] = ar;
  emxEnsureCapacity((emxArray__common *)b_D, i5, (int)sizeof(double));
  for (i5 = 0; i5 < ar; i5++) {
    b_D->data[i5] = D->data[i5];
  }

  emxInit_real_T(&c_D, 1);
  ar = D->size[0];
  i5 = c_D->size[0];
  c_D->size[0] = ar;
  emxEnsureCapacity((emxArray__common *)c_D, i5, (int)sizeof(double));
  for (i5 = 0; i5 < ar; i5++) {
    c_D->data[i5] = D->data[i5 + D->size[0]];
  }

  b_emxInit_real_T(&Hl, 2);
  emxInit_real_T(&zl, 1);
  messmatrix_new(P, r, delta, b_D, c_D, Hl);
  i5 = zl->size[0];
  zl->size[0] = (int)(2.0 * (double)xl->size[0]);
  emxEnsureCapacity((emxArray__common *)zl, i5, (int)sizeof(double));
  ar = (int)(2.0 * (double)xl->size[0]);
  emxFree_real_T(&c_D);
  emxFree_real_T(&b_D);
  for (i5 = 0; i5 < ar; i5++) {
    zl->data[i5] = 0.0;
  }

  emxInit_real_T(&zml, 1);
  i5 = zml->size[0];
  zml->size[0] = (int)(2.0 * (double)xl->size[0]);
  emxEnsureCapacity((emxArray__common *)zml, i5, (int)sizeof(double));
  ar = (int)(2.0 * (double)xl->size[0]);
  for (i5 = 0; i5 < ar; i5++) {
    zml->data[i5] = 0.0;
  }

  count = 2U;
  for (m = 0; m < xl->size[0]; m++) {
    zl->data[(int)count - 2] = xl->data[m];
    zl->data[(int)count - 1] = yl->data[m];
    if (D->data[m + D->size[0]] > 0.0) {
      zml->data[(int)count - 2] = P->data[(int)(D->data[m] - 1.0) - 1] + D->
        data[m + D->size[0]] * (P->data[(int)D->data[m] - 1] - P->data[(int)
        (D->data[m] - 1.0) - 1]);
      zml->data[(int)count - 1] = P->data[((int)(D->data[m] - 1.0) + P->size[0])
        - 1] + D->data[m + D->size[0]] * (P->data[((int)D->data[m] + P->size[0])
        - 1] - P->data[((int)(D->data[m] - 1.0) + P->size[0]) - 1]);
    } else {
      zml->data[(int)count - 2] = P->data[(int)D->data[m] - 1];
      zml->data[(int)count - 1] = P->data[((int)D->data[m] + P->size[0]) - 1];
    }

    count += 2U;
  }

  /* % rechte Seitenlinie */
  b_projectPoints(r, delta, zr, zmr, y_tilde);

  /* Punkte von Mittellinie nach rechts projizieren */
  zr_idx_0 = zr->size[0];
  zmr_idx_0 = zmr->size[0];
  br = y_tilde->size[0];
  i5 = P->size[0] * P->size[1];
  P->size[0] = zr_idx_0;
  P->size[1] = 3;
  emxEnsureCapacity((emxArray__common *)P, i5, (int)sizeof(double));
  for (i5 = 0; i5 < zr_idx_0; i5++) {
    P->data[i5] = zr->data[i5];
  }

  for (i5 = 0; i5 < zmr_idx_0; i5++) {
    P->data[i5 + P->size[0]] = zmr->data[i5];
  }

  for (i5 = 0; i5 < br; i5++) {
    P->data[i5 + (P->size[0] << 1)] = y_tilde->data[i5];
  }

  zr_idx_0 = xr->size[0];
  i5 = D->size[0] * D->size[1];
  D->size[0] = zr_idx_0;
  D->size[1] = 3;
  emxEnsureCapacity((emxArray__common *)D, i5, (int)sizeof(double));
  ar = xr->size[0] * 3;
  for (i5 = 0; i5 < ar; i5++) {
    D->data[i5] = 10000.0;
  }

  /*  Fuer jeden Messpunkt den nähesten Punkt der aktuellen Praediktion finden */
  for (zr_idx_0 = 0; zr_idx_0 < r->size[0]; zr_idx_0++) {
    for (m = 0; m < xr->size[0]; m++) {
      dist_point = P->data[zr_idx_0] - xr->data[m];
      a = P->data[zr_idx_0 + P->size[0]] - yr->data[m];
      dist_point = sqrt(dist_point * dist_point + a * a);
      if (dist_point < D->data[m + (D->size[0] << 1)]) {
        D->data[m] = 1.0 + (double)zr_idx_0;
        D->data[m + D->size[0]] = 0.0;
        D->data[m + (D->size[0] << 1)] = dist_point;
      }

      if (1.0 + (double)zr_idx_0 > 1.0) {
        d_P[0] = P->data[(int)((1.0 + (double)zr_idx_0) - 1.0) - 1];
        d_P[1] = P->data[((int)((1.0 + (double)zr_idx_0) - 1.0) + P->size[0]) -
          1];
        e_P[0] = P->data[zr_idx_0];
        e_P[1] = P->data[zr_idx_0 + P->size[0]];
        b_xr[0] = xr->data[m];
        b_xr[1] = yr->data[m];
        d_line_point(d_P, e_P, b_xr, &dist_point, &a, unusedU1);
        if ((dist_point < D->data[m + (D->size[0] << 1)]) && (a > 0.0) && (a <
             1.0)) {
          D->data[m] = 1.0 + (double)zr_idx_0;
          D->data[m + D->size[0]] = a;
          D->data[m + (D->size[0] << 1)] = dist_point;
        }
      }
    }
  }

  /*  Messpunkte ausfiltern, die vor dem letzten prädizierten Punkt liegen */
  ar = D->size[0];
  zr_idx_0 = r->size[0];
  i5 = ind->size[0];
  ind->size[0] = ar;
  emxEnsureCapacity((emxArray__common *)ind, i5, (int)sizeof(boolean_T));
  for (i5 = 0; i5 < ar; i5++) {
    ind->data[i5] = (((D->data[i5] == zr_idx_0) && (D->data[i5 + D->size[0]] ==
      0.0)) || (D->data[i5 + (D->size[0] << 1)] > 0.5));
  }

  emxInit_real_T(&d_D, 1);
  eml_null_assignment(xr, ind);
  eml_null_assignment(yr, ind);
  b_eml_null_assignment(D, ind);

  /*  Messmatrix, Mess- und Erwartungsvektor bauen */
  ar = D->size[0];
  i5 = d_D->size[0];
  d_D->size[0] = ar;
  emxEnsureCapacity((emxArray__common *)d_D, i5, (int)sizeof(double));
  emxFree_boolean_T(&ind);
  for (i5 = 0; i5 < ar; i5++) {
    d_D->data[i5] = D->data[i5];
  }

  emxInit_real_T(&e_D, 1);
  ar = D->size[0];
  i5 = e_D->size[0];
  e_D->size[0] = ar;
  emxEnsureCapacity((emxArray__common *)e_D, i5, (int)sizeof(double));
  for (i5 = 0; i5 < ar; i5++) {
    e_D->data[i5] = D->data[i5 + D->size[0]];
  }

  b_emxInit_real_T(&Hr, 2);
  messmatrix_new(P, r, delta, d_D, e_D, Hr);
  i5 = zr->size[0];
  zr->size[0] = (int)(2.0 * (double)xr->size[0]);
  emxEnsureCapacity((emxArray__common *)zr, i5, (int)sizeof(double));
  ar = (int)(2.0 * (double)xr->size[0]);
  emxFree_real_T(&e_D);
  emxFree_real_T(&d_D);
  for (i5 = 0; i5 < ar; i5++) {
    zr->data[i5] = 0.0;
  }

  i5 = zmr->size[0];
  zmr->size[0] = (int)(2.0 * (double)xr->size[0]);
  emxEnsureCapacity((emxArray__common *)zmr, i5, (int)sizeof(double));
  ar = (int)(2.0 * (double)xr->size[0]);
  for (i5 = 0; i5 < ar; i5++) {
    zmr->data[i5] = 0.0;
  }

  count = 2U;
  for (m = 0; m < xr->size[0]; m++) {
    zr->data[(int)count - 2] = xr->data[m];
    zr->data[(int)count - 1] = yr->data[m];
    if (D->data[m + D->size[0]] > 0.0) {
      zmr->data[(int)count - 2] = P->data[(int)(D->data[m] - 1.0) - 1] + D->
        data[m + D->size[0]] * (P->data[(int)D->data[m] - 1] - P->data[(int)
        (D->data[m] - 1.0) - 1]);
      zmr->data[(int)count - 1] = P->data[((int)(D->data[m] - 1.0) + P->size[0])
        - 1] + D->data[m + D->size[0]] * (P->data[((int)D->data[m] + P->size[0])
        - 1] - P->data[((int)(D->data[m] - 1.0) + P->size[0]) - 1]);
    } else {
      zmr->data[(int)count - 2] = P->data[(int)D->data[m] - 1];
      zmr->data[(int)count - 1] = P->data[((int)D->data[m] + P->size[0]) - 1];
    }

    count += 2U;
  }

  emxFree_real_T(&D);
  emxFree_real_T(&P);
  b_emxInit_real_T(&H, 2);

  /* % Linke und rechte Linie kombinieren */
  i5 = H->size[0] * H->size[1];
  H->size[0] = Hl->size[0] + Hr->size[0];
  H->size[1] = Hl->size[1];
  emxEnsureCapacity((emxArray__common *)H, i5, (int)sizeof(double));
  ar = Hl->size[1];
  for (i5 = 0; i5 < ar; i5++) {
    br = Hl->size[0];
    for (i6 = 0; i6 < br; i6++) {
      H->data[i6 + H->size[0] * i5] = Hl->data[i6 + Hl->size[0] * i5];
    }
  }

  ar = Hr->size[1];
  for (i5 = 0; i5 < ar; i5++) {
    br = Hr->size[0];
    for (i6 = 0; i6 < br; i6++) {
      H->data[(i6 + Hl->size[0]) + H->size[0] * i5] = Hr->data[i6 + Hr->size[0] *
        i5];
    }
  }

  /* % KALMAN FILTER     */
  /*  r = A*r + B*u */
  eye(2.0 * (double)((unsigned int)xl->size[0] + xr->size[0]), Hl);
  i5 = Hl->size[0] * Hl->size[1];
  emxEnsureCapacity((emxArray__common *)Hl, i5, (int)sizeof(double));
  zr_idx_0 = Hl->size[0];
  zmr_idx_0 = Hl->size[1];
  ar = zr_idx_0 * zmr_idx_0;
  for (i5 = 0; i5 < ar; i5++) {
    Hl->data[i5] *= R_fakt;
  }

  if ((A->size[1] == 1) || (Pk->size[0] == 1)) {
    i5 = Hr->size[0] * Hr->size[1];
    Hr->size[0] = A->size[0];
    Hr->size[1] = Pk->size[1];
    emxEnsureCapacity((emxArray__common *)Hr, i5, (int)sizeof(double));
    ar = A->size[0];
    for (i5 = 0; i5 < ar; i5++) {
      br = Pk->size[1];
      for (i6 = 0; i6 < br; i6++) {
        Hr->data[i5 + Hr->size[0] * i6] = 0.0;
        zr_idx_0 = A->size[1];
        for (zmr_idx_0 = 0; zmr_idx_0 < zr_idx_0; zmr_idx_0++) {
          Hr->data[i5 + Hr->size[0] * i6] += A->data[i5 + A->size[0] * zmr_idx_0]
            * Pk->data[zmr_idx_0 + Pk->size[0] * i6];
        }
      }
    }
  } else {
    k = A->size[1];
    unusedU1[0] = (unsigned int)A->size[0];
    unusedU1[1] = (unsigned int)Pk->size[1];
    m = A->size[0];
    i5 = Hr->size[0] * Hr->size[1];
    Hr->size[0] = (int)unusedU1[0];
    emxEnsureCapacity((emxArray__common *)Hr, i5, (int)sizeof(double));
    i5 = Hr->size[0] * Hr->size[1];
    Hr->size[1] = (int)unusedU1[1];
    emxEnsureCapacity((emxArray__common *)Hr, i5, (int)sizeof(double));
    ar = (int)unusedU1[0] * (int)unusedU1[1];
    for (i5 = 0; i5 < ar; i5++) {
      Hr->data[i5] = 0.0;
    }

    if ((A->size[0] == 0) || (Pk->size[1] == 0)) {
    } else {
      zr_idx_0 = A->size[0] * (Pk->size[1] - 1);
      zmr_idx_0 = 0;
      while ((m > 0) && (zmr_idx_0 <= zr_idx_0)) {
        i5 = zmr_idx_0 + m;
        for (ic = zmr_idx_0; ic + 1 <= i5; ic++) {
          Hr->data[ic] = 0.0;
        }

        zmr_idx_0 += m;
      }

      br = 0;
      zmr_idx_0 = 0;
      while ((m > 0) && (zmr_idx_0 <= zr_idx_0)) {
        ar = -1;
        i5 = br + k;
        for (ib = br; ib + 1 <= i5; ib++) {
          if (Pk->data[ib] != 0.0) {
            ia = ar;
            i6 = zmr_idx_0 + m;
            for (ic = zmr_idx_0; ic + 1 <= i6; ic++) {
              ia++;
              Hr->data[ic] += Pk->data[ib] * A->data[ia];
            }
          }

          ar += m;
        }

        br += k;
        zmr_idx_0 += m;
      }
    }
  }

  b_emxInit_real_T(&b, 2);
  i5 = b->size[0] * b->size[1];
  b->size[0] = A->size[1];
  b->size[1] = A->size[0];
  emxEnsureCapacity((emxArray__common *)b, i5, (int)sizeof(double));
  ar = A->size[0];
  for (i5 = 0; i5 < ar; i5++) {
    br = A->size[1];
    for (i6 = 0; i6 < br; i6++) {
      b->data[i6 + b->size[0] * i5] = A->data[i5 + A->size[0] * i6];
    }
  }

  if ((Hr->size[1] == 1) || (b->size[0] == 1)) {
    i5 = Pk->size[0] * Pk->size[1];
    Pk->size[0] = Hr->size[0];
    Pk->size[1] = b->size[1];
    emxEnsureCapacity((emxArray__common *)Pk, i5, (int)sizeof(double));
    ar = Hr->size[0];
    for (i5 = 0; i5 < ar; i5++) {
      br = b->size[1];
      for (i6 = 0; i6 < br; i6++) {
        Pk->data[i5 + Pk->size[0] * i6] = 0.0;
        zr_idx_0 = Hr->size[1];
        for (zmr_idx_0 = 0; zmr_idx_0 < zr_idx_0; zmr_idx_0++) {
          Pk->data[i5 + Pk->size[0] * i6] += Hr->data[i5 + Hr->size[0] *
            zmr_idx_0] * b->data[zmr_idx_0 + b->size[0] * i6];
        }
      }
    }
  } else {
    k = Hr->size[1];
    unusedU1[0] = (unsigned int)Hr->size[0];
    unusedU1[1] = (unsigned int)b->size[1];
    m = Hr->size[0];
    i5 = Pk->size[0] * Pk->size[1];
    Pk->size[0] = (int)unusedU1[0];
    Pk->size[1] = (int)unusedU1[1];
    emxEnsureCapacity((emxArray__common *)Pk, i5, (int)sizeof(double));
    ar = (int)unusedU1[1];
    for (i5 = 0; i5 < ar; i5++) {
      br = (int)unusedU1[0];
      for (i6 = 0; i6 < br; i6++) {
        Pk->data[i6 + Pk->size[0] * i5] = 0.0;
      }
    }

    if ((Hr->size[0] == 0) || (b->size[1] == 0)) {
    } else {
      zr_idx_0 = Hr->size[0] * (b->size[1] - 1);
      zmr_idx_0 = 0;
      while ((m > 0) && (zmr_idx_0 <= zr_idx_0)) {
        i5 = zmr_idx_0 + m;
        for (ic = zmr_idx_0; ic + 1 <= i5; ic++) {
          Pk->data[ic] = 0.0;
        }

        zmr_idx_0 += m;
      }

      br = 0;
      zmr_idx_0 = 0;
      while ((m > 0) && (zmr_idx_0 <= zr_idx_0)) {
        ar = -1;
        i5 = br + k;
        for (ib = br; ib + 1 <= i5; ib++) {
          if (b->data[ib] != 0.0) {
            ia = ar;
            i6 = zmr_idx_0 + m;
            for (ic = zmr_idx_0; ic + 1 <= i6; ic++) {
              ia++;
              Pk->data[ic] += b->data[ib] * Hr->data[ia];
            }
          }

          ar += m;
        }

        br += k;
        zmr_idx_0 += m;
      }
    }
  }

  i5 = Pk->size[0] * Pk->size[1];
  emxEnsureCapacity((emxArray__common *)Pk, i5, (int)sizeof(double));
  ar = Pk->size[1];
  for (i5 = 0; i5 < ar; i5++) {
    br = Pk->size[0];
    for (i6 = 0; i6 < br; i6++) {
      Pk->data[i6 + Pk->size[0] * i5] += Q->data[i6 + Q->size[0] * i5];
    }
  }

  emxInit_real_T(&b_zl, 1);
  emxInit_real_T(&b_zml, 1);
  i5 = b_zl->size[0];
  b_zl->size[0] = zl->size[0] + zr->size[0];
  emxEnsureCapacity((emxArray__common *)b_zl, i5, (int)sizeof(double));
  ar = zl->size[0];
  for (i5 = 0; i5 < ar; i5++) {
    b_zl->data[i5] = zl->data[i5];
  }

  ar = zr->size[0];
  for (i5 = 0; i5 < ar; i5++) {
    b_zl->data[i5 + zl->size[0]] = zr->data[i5];
  }

  emxFree_real_T(&zl);
  i5 = b_zml->size[0];
  b_zml->size[0] = zml->size[0] + zmr->size[0];
  emxEnsureCapacity((emxArray__common *)b_zml, i5, (int)sizeof(double));
  ar = zml->size[0];
  for (i5 = 0; i5 < ar; i5++) {
    b_zml->data[i5] = zml->data[i5];
  }

  ar = zmr->size[0];
  for (i5 = 0; i5 < ar; i5++) {
    b_zml->data[i5 + zml->size[0]] = zmr->data[i5];
  }

  emxFree_real_T(&zmr);
  emxFree_real_T(&zml);
  i5 = y_tilde->size[0];
  y_tilde->size[0] = b_zl->size[0];
  emxEnsureCapacity((emxArray__common *)y_tilde, i5, (int)sizeof(double));
  ar = b_zl->size[0];
  for (i5 = 0; i5 < ar; i5++) {
    y_tilde->data[i5] = b_zl->data[i5] - b_zml->data[i5];
  }

  emxFree_real_T(&b_zml);
  emxFree_real_T(&b_zl);
  if ((H->size[1] == 1) || (Pk->size[0] == 1)) {
    i5 = Hr->size[0] * Hr->size[1];
    Hr->size[0] = H->size[0];
    Hr->size[1] = Pk->size[1];
    emxEnsureCapacity((emxArray__common *)Hr, i5, (int)sizeof(double));
    ar = H->size[0];
    for (i5 = 0; i5 < ar; i5++) {
      br = Pk->size[1];
      for (i6 = 0; i6 < br; i6++) {
        Hr->data[i5 + Hr->size[0] * i6] = 0.0;
        zr_idx_0 = H->size[1];
        for (zmr_idx_0 = 0; zmr_idx_0 < zr_idx_0; zmr_idx_0++) {
          Hr->data[i5 + Hr->size[0] * i6] += H->data[i5 + H->size[0] * zmr_idx_0]
            * Pk->data[zmr_idx_0 + Pk->size[0] * i6];
        }
      }
    }
  } else {
    k = H->size[1];
    unusedU1[0] = (unsigned int)H->size[0];
    unusedU1[1] = (unsigned int)Pk->size[1];
    m = H->size[0];
    i5 = Hr->size[0] * Hr->size[1];
    Hr->size[0] = (int)unusedU1[0];
    emxEnsureCapacity((emxArray__common *)Hr, i5, (int)sizeof(double));
    i5 = Hr->size[0] * Hr->size[1];
    Hr->size[1] = (int)unusedU1[1];
    emxEnsureCapacity((emxArray__common *)Hr, i5, (int)sizeof(double));
    ar = (int)unusedU1[0] * (int)unusedU1[1];
    for (i5 = 0; i5 < ar; i5++) {
      Hr->data[i5] = 0.0;
    }

    if ((H->size[0] == 0) || (Pk->size[1] == 0)) {
    } else {
      zr_idx_0 = H->size[0] * (Pk->size[1] - 1);
      zmr_idx_0 = 0;
      while ((m > 0) && (zmr_idx_0 <= zr_idx_0)) {
        i5 = zmr_idx_0 + m;
        for (ic = zmr_idx_0; ic + 1 <= i5; ic++) {
          Hr->data[ic] = 0.0;
        }

        zmr_idx_0 += m;
      }

      br = 0;
      zmr_idx_0 = 0;
      while ((m > 0) && (zmr_idx_0 <= zr_idx_0)) {
        ar = -1;
        i5 = br + k;
        for (ib = br; ib + 1 <= i5; ib++) {
          if (Pk->data[ib] != 0.0) {
            ia = ar;
            i6 = zmr_idx_0 + m;
            for (ic = zmr_idx_0; ic + 1 <= i6; ic++) {
              ia++;
              Hr->data[ic] += Pk->data[ib] * H->data[ia];
            }
          }

          ar += m;
        }

        br += k;
        zmr_idx_0 += m;
      }
    }
  }

  i5 = b->size[0] * b->size[1];
  b->size[0] = H->size[1];
  b->size[1] = H->size[0];
  emxEnsureCapacity((emxArray__common *)b, i5, (int)sizeof(double));
  ar = H->size[0];
  for (i5 = 0; i5 < ar; i5++) {
    br = H->size[1];
    for (i6 = 0; i6 < br; i6++) {
      b->data[i6 + b->size[0] * i5] = H->data[i5 + H->size[0] * i6];
    }
  }

  b_emxInit_real_T(&C, 2);
  if ((Hr->size[1] == 1) || (b->size[0] == 1)) {
    i5 = C->size[0] * C->size[1];
    C->size[0] = Hr->size[0];
    C->size[1] = b->size[1];
    emxEnsureCapacity((emxArray__common *)C, i5, (int)sizeof(double));
    ar = Hr->size[0];
    for (i5 = 0; i5 < ar; i5++) {
      br = b->size[1];
      for (i6 = 0; i6 < br; i6++) {
        C->data[i5 + C->size[0] * i6] = 0.0;
        zr_idx_0 = Hr->size[1];
        for (zmr_idx_0 = 0; zmr_idx_0 < zr_idx_0; zmr_idx_0++) {
          C->data[i5 + C->size[0] * i6] += Hr->data[i5 + Hr->size[0] * zmr_idx_0]
            * b->data[zmr_idx_0 + b->size[0] * i6];
        }
      }
    }
  } else {
    k = Hr->size[1];
    unusedU1[0] = (unsigned int)Hr->size[0];
    unusedU1[1] = (unsigned int)b->size[1];
    m = Hr->size[0];
    i5 = C->size[0] * C->size[1];
    C->size[0] = (int)unusedU1[0];
    emxEnsureCapacity((emxArray__common *)C, i5, (int)sizeof(double));
    i5 = C->size[0] * C->size[1];
    C->size[1] = (int)unusedU1[1];
    emxEnsureCapacity((emxArray__common *)C, i5, (int)sizeof(double));
    ar = (int)unusedU1[0] * (int)unusedU1[1];
    for (i5 = 0; i5 < ar; i5++) {
      C->data[i5] = 0.0;
    }

    if ((Hr->size[0] == 0) || (b->size[1] == 0)) {
    } else {
      zr_idx_0 = Hr->size[0] * (b->size[1] - 1);
      zmr_idx_0 = 0;
      while ((m > 0) && (zmr_idx_0 <= zr_idx_0)) {
        i5 = zmr_idx_0 + m;
        for (ic = zmr_idx_0; ic + 1 <= i5; ic++) {
          C->data[ic] = 0.0;
        }

        zmr_idx_0 += m;
      }

      br = 0;
      zmr_idx_0 = 0;
      while ((m > 0) && (zmr_idx_0 <= zr_idx_0)) {
        ar = -1;
        i5 = br + k;
        for (ib = br; ib + 1 <= i5; ib++) {
          if (b->data[ib] != 0.0) {
            ia = ar;
            i6 = zmr_idx_0 + m;
            for (ic = zmr_idx_0; ic + 1 <= i6; ic++) {
              ia++;
              C->data[ic] += b->data[ib] * Hr->data[ia];
            }
          }

          ar += m;
        }

        br += k;
        zmr_idx_0 += m;
      }
    }
  }

  i5 = b->size[0] * b->size[1];
  b->size[0] = H->size[1];
  b->size[1] = H->size[0];
  emxEnsureCapacity((emxArray__common *)b, i5, (int)sizeof(double));
  ar = H->size[0];
  for (i5 = 0; i5 < ar; i5++) {
    br = H->size[1];
    for (i6 = 0; i6 < br; i6++) {
      b->data[i6 + b->size[0] * i5] = H->data[i5 + H->size[0] * i6];
    }
  }

  if ((Pk->size[1] == 1) || (b->size[0] == 1)) {
    i5 = Hr->size[0] * Hr->size[1];
    Hr->size[0] = Pk->size[0];
    Hr->size[1] = b->size[1];
    emxEnsureCapacity((emxArray__common *)Hr, i5, (int)sizeof(double));
    ar = Pk->size[0];
    for (i5 = 0; i5 < ar; i5++) {
      br = b->size[1];
      for (i6 = 0; i6 < br; i6++) {
        Hr->data[i5 + Hr->size[0] * i6] = 0.0;
        zr_idx_0 = Pk->size[1];
        for (zmr_idx_0 = 0; zmr_idx_0 < zr_idx_0; zmr_idx_0++) {
          Hr->data[i5 + Hr->size[0] * i6] += Pk->data[i5 + Pk->size[0] *
            zmr_idx_0] * b->data[zmr_idx_0 + b->size[0] * i6];
        }
      }
    }
  } else {
    k = Pk->size[1];
    unusedU1[0] = (unsigned int)Pk->size[0];
    unusedU1[1] = (unsigned int)b->size[1];
    m = Pk->size[0];
    i5 = Hr->size[0] * Hr->size[1];
    Hr->size[0] = (int)unusedU1[0];
    emxEnsureCapacity((emxArray__common *)Hr, i5, (int)sizeof(double));
    i5 = Hr->size[0] * Hr->size[1];
    Hr->size[1] = (int)unusedU1[1];
    emxEnsureCapacity((emxArray__common *)Hr, i5, (int)sizeof(double));
    ar = (int)unusedU1[0] * (int)unusedU1[1];
    for (i5 = 0; i5 < ar; i5++) {
      Hr->data[i5] = 0.0;
    }

    if ((Pk->size[0] == 0) || (b->size[1] == 0)) {
    } else {
      zr_idx_0 = Pk->size[0] * (b->size[1] - 1);
      zmr_idx_0 = 0;
      while (((m > 0) && (zmr_idx_0 <= zr_idx_0)) || ((m < 0) && (zmr_idx_0 >=
               zr_idx_0))) {
        i5 = zmr_idx_0 + m;
        for (ic = zmr_idx_0; ic + 1 <= i5; ic++) {
          Hr->data[ic] = 0.0;
        }

        zmr_idx_0 += m;
      }

      br = 0;
      zmr_idx_0 = 0;
      while (((m > 0) && (zmr_idx_0 <= zr_idx_0)) || ((m < 0) && (zmr_idx_0 >=
               zr_idx_0))) {
        ar = -1;
        i5 = br + k;
        for (ib = br; ib + 1 <= i5; ib++) {
          if (b->data[ib] != 0.0) {
            ia = ar;
            i6 = zmr_idx_0 + m;
            for (ic = zmr_idx_0; ic + 1 <= i6; ic++) {
              ia++;
              Hr->data[ic] += b->data[ib] * Pk->data[ia];
            }
          }

          ar += m;
        }

        br += k;
        zmr_idx_0 += m;
      }
    }
  }

  b_emxInit_real_T(&b_C, 2);
  i5 = b_C->size[0] * b_C->size[1];
  b_C->size[0] = C->size[0];
  b_C->size[1] = C->size[1];
  emxEnsureCapacity((emxArray__common *)b_C, i5, (int)sizeof(double));
  ar = C->size[0] * C->size[1];
  for (i5 = 0; i5 < ar; i5++) {
    b_C->data[i5] = C->data[i5] + Hl->data[i5];
  }

  mrdivide(Hr, b_C, Hl);
  emxFree_real_T(&b_C);
  if ((Hl->size[1] == 1) || (y_tilde->size[0] == 1)) {
    i5 = zr->size[0];
    zr->size[0] = Hl->size[0];
    emxEnsureCapacity((emxArray__common *)zr, i5, (int)sizeof(double));
    ar = Hl->size[0];
    for (i5 = 0; i5 < ar; i5++) {
      zr->data[i5] = 0.0;
      br = Hl->size[1];
      for (i6 = 0; i6 < br; i6++) {
        zr->data[i5] += Hl->data[i5 + Hl->size[0] * i6] * y_tilde->data[i6];
      }
    }
  } else {
    k = Hl->size[1];
    unusedU1[0] = Hl->size[0];
    m = Hl->size[0];
    i5 = zr->size[0];
    zr->size[0] = (int)unusedU1[0];
    emxEnsureCapacity((emxArray__common *)zr, i5, (int)sizeof(double));
    ar = (int)unusedU1[0];
    for (i5 = 0; i5 < ar; i5++) {
      zr->data[i5] = 0.0;
    }

    if (Hl->size[0] == 0) {
    } else {
      zmr_idx_0 = 0;
      while ((m > 0) && (zmr_idx_0 <= 0)) {
        for (ic = 0; ic + 1 <= m; ic++) {
          zr->data[ic] = 0.0;
        }

        zmr_idx_0 = m;
      }

      br = 0;
      zmr_idx_0 = 0;
      while ((m > 0) && (zmr_idx_0 <= 0)) {
        ar = -1;
        i5 = br + k;
        for (ib = br; ib + 1 <= i5; ib++) {
          if (y_tilde->data[ib] != 0.0) {
            ia = ar;
            for (ic = 0; ic + 1 <= m; ic++) {
              ia++;
              zr->data[ic] += y_tilde->data[ib] * Hl->data[ia];
            }
          }

          ar += m;
        }

        br += k;
        zmr_idx_0 = m;
      }
    }
  }

  emxFree_real_T(&y_tilde);
  i5 = r->size[0];
  emxEnsureCapacity((emxArray__common *)r, i5, (int)sizeof(double));
  ar = r->size[0];
  for (i5 = 0; i5 < ar; i5++) {
    r->data[i5] += zr->data[i5];
  }

  emxFree_real_T(&zr);
  eye(r->size[0], Hr);
  if ((Hl->size[1] == 1) || (H->size[0] == 1)) {
    i5 = C->size[0] * C->size[1];
    C->size[0] = Hl->size[0];
    C->size[1] = H->size[1];
    emxEnsureCapacity((emxArray__common *)C, i5, (int)sizeof(double));
    ar = Hl->size[0];
    for (i5 = 0; i5 < ar; i5++) {
      br = H->size[1];
      for (i6 = 0; i6 < br; i6++) {
        C->data[i5 + C->size[0] * i6] = 0.0;
        zr_idx_0 = Hl->size[1];
        for (zmr_idx_0 = 0; zmr_idx_0 < zr_idx_0; zmr_idx_0++) {
          C->data[i5 + C->size[0] * i6] += Hl->data[i5 + Hl->size[0] * zmr_idx_0]
            * H->data[zmr_idx_0 + H->size[0] * i6];
        }
      }
    }
  } else {
    k = Hl->size[1];
    unusedU1[0] = (unsigned int)Hl->size[0];
    unusedU1[1] = (unsigned int)H->size[1];
    m = Hl->size[0];
    i5 = C->size[0] * C->size[1];
    C->size[0] = (int)unusedU1[0];
    emxEnsureCapacity((emxArray__common *)C, i5, (int)sizeof(double));
    i5 = C->size[0] * C->size[1];
    C->size[1] = (int)unusedU1[1];
    emxEnsureCapacity((emxArray__common *)C, i5, (int)sizeof(double));
    ar = (int)unusedU1[0] * (int)unusedU1[1];
    for (i5 = 0; i5 < ar; i5++) {
      C->data[i5] = 0.0;
    }

    if ((Hl->size[0] == 0) || (H->size[1] == 0)) {
    } else {
      zr_idx_0 = Hl->size[0] * (H->size[1] - 1);
      zmr_idx_0 = 0;
      while ((m > 0) && (zmr_idx_0 <= zr_idx_0)) {
        i5 = zmr_idx_0 + m;
        for (ic = zmr_idx_0; ic + 1 <= i5; ic++) {
          C->data[ic] = 0.0;
        }

        zmr_idx_0 += m;
      }

      br = 0;
      zmr_idx_0 = 0;
      while ((m > 0) && (zmr_idx_0 <= zr_idx_0)) {
        ar = -1;
        i5 = br + k;
        for (ib = br; ib + 1 <= i5; ib++) {
          if (H->data[ib] != 0.0) {
            ia = ar;
            i6 = zmr_idx_0 + m;
            for (ic = zmr_idx_0; ic + 1 <= i6; ic++) {
              ia++;
              C->data[ic] += H->data[ib] * Hl->data[ia];
            }
          }

          ar += m;
        }

        br += k;
        zmr_idx_0 += m;
      }
    }
  }

  emxFree_real_T(&H);
  emxFree_real_T(&Hl);
  i5 = Hr->size[0] * Hr->size[1];
  emxEnsureCapacity((emxArray__common *)Hr, i5, (int)sizeof(double));
  zr_idx_0 = Hr->size[0];
  zmr_idx_0 = Hr->size[1];
  ar = zr_idx_0 * zmr_idx_0;
  for (i5 = 0; i5 < ar; i5++) {
    Hr->data[i5] -= C->data[i5];
  }

  emxFree_real_T(&C);
  i5 = b->size[0] * b->size[1];
  b->size[0] = Pk->size[0];
  b->size[1] = Pk->size[1];
  emxEnsureCapacity((emxArray__common *)b, i5, (int)sizeof(double));
  ar = Pk->size[0] * Pk->size[1];
  for (i5 = 0; i5 < ar; i5++) {
    b->data[i5] = Pk->data[i5];
  }

  b_emxInit_real_T(&b_Hr, 2);
  if ((Hr->size[1] == 1) || (Pk->size[0] == 1)) {
    i5 = b_Hr->size[0] * b_Hr->size[1];
    b_Hr->size[0] = Hr->size[0];
    b_Hr->size[1] = Pk->size[1];
    emxEnsureCapacity((emxArray__common *)b_Hr, i5, (int)sizeof(double));
    ar = Hr->size[0];
    for (i5 = 0; i5 < ar; i5++) {
      br = Pk->size[1];
      for (i6 = 0; i6 < br; i6++) {
        b_Hr->data[i5 + b_Hr->size[0] * i6] = 0.0;
        zr_idx_0 = Hr->size[1];
        for (zmr_idx_0 = 0; zmr_idx_0 < zr_idx_0; zmr_idx_0++) {
          b_Hr->data[i5 + b_Hr->size[0] * i6] += Hr->data[i5 + Hr->size[0] *
            zmr_idx_0] * Pk->data[zmr_idx_0 + Pk->size[0] * i6];
        }
      }
    }

    i5 = Pk->size[0] * Pk->size[1];
    Pk->size[0] = b_Hr->size[0];
    Pk->size[1] = b_Hr->size[1];
    emxEnsureCapacity((emxArray__common *)Pk, i5, (int)sizeof(double));
    ar = b_Hr->size[1];
    for (i5 = 0; i5 < ar; i5++) {
      br = b_Hr->size[0];
      for (i6 = 0; i6 < br; i6++) {
        Pk->data[i6 + Pk->size[0] * i5] = b_Hr->data[i6 + b_Hr->size[0] * i5];
      }
    }
  } else {
    k = Hr->size[1];
    unusedU1[0] = (unsigned int)Hr->size[0];
    unusedU1[1] = (unsigned int)Pk->size[1];
    m = Hr->size[0];
    i5 = Pk->size[0] * Pk->size[1];
    Pk->size[0] = (int)unusedU1[0];
    Pk->size[1] = (int)unusedU1[1];
    emxEnsureCapacity((emxArray__common *)Pk, i5, (int)sizeof(double));
    ar = (int)unusedU1[1];
    for (i5 = 0; i5 < ar; i5++) {
      br = (int)unusedU1[0];
      for (i6 = 0; i6 < br; i6++) {
        Pk->data[i6 + Pk->size[0] * i5] = 0.0;
      }
    }

    if ((Hr->size[0] == 0) || (b->size[1] == 0)) {
    } else {
      zr_idx_0 = Hr->size[0] * (b->size[1] - 1);
      zmr_idx_0 = 0;
      while ((m > 0) && (zmr_idx_0 <= zr_idx_0)) {
        i5 = zmr_idx_0 + m;
        for (ic = zmr_idx_0; ic + 1 <= i5; ic++) {
          Pk->data[ic] = 0.0;
        }

        zmr_idx_0 += m;
      }

      br = 0;
      zmr_idx_0 = 0;
      while ((m > 0) && (zmr_idx_0 <= zr_idx_0)) {
        ar = -1;
        i5 = br + k;
        for (ib = br; ib + 1 <= i5; ib++) {
          if (b->data[ib] != 0.0) {
            ia = ar;
            i6 = zmr_idx_0 + m;
            for (ic = zmr_idx_0; ic + 1 <= i6; ic++) {
              ia++;
              Pk->data[ic] += b->data[ib] * Hr->data[ia];
            }
          }

          ar += m;
        }

        br += k;
        zmr_idx_0 += m;
      }
    }
  }

  emxFree_real_T(&b_Hr);
  emxFree_real_T(&b);
  emxFree_real_T(&Hr);

  /* % Zustandsbegrenzungen */
  /*  Krümmung */
  /*  bezogen auf Mittellinie, 0.72 entspricht einem minimalen Innenradius von 1m */
  i5 = r->size[0];
  for (zr_idx_0 = 2; zr_idx_0 - 2 <= i5 - 3; zr_idx_0++) {
    if ((-0.75 >= r->data[zr_idx_0]) || rtIsNaN(r->data[zr_idx_0])) {
      dist_point = -0.75;
    } else {
      dist_point = r->data[zr_idx_0];
    }

    r->data[zr_idx_0] = dist_point;
    if ((0.75 <= r->data[zr_idx_0]) || rtIsNaN(r->data[zr_idx_0])) {
      dist_point = 0.75;
    } else {
      dist_point = r->data[zr_idx_0];
    }

    r->data[zr_idx_0] = dist_point;
  }

  /*  y-Wert des ersten Punktes */
  if ((0.5 <= r->data[0]) || rtIsNaN(r->data[0])) {
    dist_point = 0.5;
  } else {
    dist_point = r->data[0];
  }

  if ((-0.5 >= dist_point) || rtIsNaN(dist_point)) {
    r->data[0] = -0.5;
  } else {
    r->data[0] = dist_point;
  }

  /*  Startwinkel  */
  if ((0.78 <= r->data[1]) || rtIsNaN(r->data[1])) {
    dist_point = 0.78;
  } else {
    dist_point = r->data[1];
  }

  if ((-0.78 >= dist_point) || rtIsNaN(dist_point)) {
    r->data[1] = -0.78;
  } else {
    r->data[1] = dist_point;
  }

  /*  45° */
}

/*
 * File trailer for kalman_filter_lr.c
 *
 * [EOF]
 */
