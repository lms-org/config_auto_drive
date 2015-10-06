/*
 * File: kalman_filter_lr.c
 *
 * MATLAB Coder version            : 2.7
 * C/C++ source code generated on  : 15-Jun-2015 13:18:28
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "kalman_filter_lr.h"
#include "d_line_point.h"
#include "kalman_filter_lr_emxutil.h"
#include "eye.h"
#include "mrdivide.h"
#include "messmatrix.h"
#include "getPointsFromState.h"
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
 *   * Q: Kovarianz des Zustandsübergangs (Prozessrauschen) dim[n x n] (symmetrische Matrix, mit weg von der
 *    Diagonalen abnehmenden Eintraegen)
 *   * R_fakt: Unsicherheit der Messwerte (Messrauschen)
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
 *                double delta_x
 *                double delta_y
 *                double delta_phi
 *                emxArray_real_T *Pk
 *                const emxArray_real_T *Q
 *                double R_fakt
 *                double delta
 *                emxArray_real_T *xl
 *                emxArray_real_T *yl
 *                emxArray_real_T *xr
 *                emxArray_real_T *yr
 *                emxArray_real_T *xm
 *                emxArray_real_T *ym
 * Return Type  : void
 */
void kalman_filter_lr(emxArray_real_T *r, double delta_x, double delta_y, double
                      delta_phi, emxArray_real_T *Pk, const emxArray_real_T *Q,
                      double R_fakt, double delta, emxArray_real_T *xl,
                      emxArray_real_T *yl, emxArray_real_T *xr, emxArray_real_T *
                      yr, emxArray_real_T *xm, emxArray_real_T *ym)
{
  emxArray_real_T *P;
  emxArray_real_T *z_m;
  emxArray_real_T *zmm;
  emxArray_real_T *xp;
  int xp_idx_0;
  int br;
  int ar;
  int i5;
  emxArray_real_T *D;
  int loop_ub;
  int m;
  double dist_point;
  double a;
  double b_P[2];
  double c_P[2];
  double b_xl[2];
  double unusedU2[2];
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
  double f_P[2];
  double g_P[2];
  double b_xm[2];
  emxArray_real_T *f_D;
  emxArray_real_T *g_D;
  emxArray_real_T *Hm;
  emxArray_real_T *H;
  int i6;
  emxArray_real_T *b_Hl;
  int k;
  int ic;
  int ia;
  int c;
  emxArray_real_T *b;
  emxArray_real_T *b_zl;
  emxArray_real_T *b_zml;
  emxArray_real_T *y_tilde;
  emxArray_real_T *C;
  emxArray_real_T *b_C;
  emxArray_real_T *b_Hr;
  b_emxInit_real_T(&P, 2);
  emxInit_real_T(&z_m, 1);
  emxInit_real_T(&zmm, 1);
  emxInit_real_T(&xp, 1);

  /* % linke Seitenlinie */
  projectPoints(r, delta, xp, z_m, zmm);

  /* Punkte von Mittellinie nach links projizieren */
  xp_idx_0 = xp->size[0];
  br = z_m->size[0];
  ar = zmm->size[0];
  i5 = P->size[0] * P->size[1];
  P->size[0] = xp_idx_0;
  P->size[1] = 3;
  emxEnsureCapacity((emxArray__common *)P, i5, (int)sizeof(double));
  for (i5 = 0; i5 < xp_idx_0; i5++) {
    P->data[i5] = xp->data[i5];
  }

  for (i5 = 0; i5 < br; i5++) {
    P->data[i5 + P->size[0]] = z_m->data[i5];
  }

  for (i5 = 0; i5 < ar; i5++) {
    P->data[i5 + (P->size[0] << 1)] = zmm->data[i5];
  }

  b_emxInit_real_T(&D, 2);
  xp_idx_0 = xl->size[0];
  i5 = D->size[0] * D->size[1];
  D->size[0] = xp_idx_0;
  D->size[1] = 3;
  emxEnsureCapacity((emxArray__common *)D, i5, (int)sizeof(double));
  loop_ub = xl->size[0] * 3;
  for (i5 = 0; i5 < loop_ub; i5++) {
    D->data[i5] = 10000.0;
  }

  /*  Fuer jeden Messpunkt den nähesten Punkt der aktuellen Praediktion finden  */
  for (xp_idx_0 = 0; xp_idx_0 < r->size[0]; xp_idx_0++) {
    for (m = 0; m < xl->size[0]; m++) {
      dist_point = P->data[xp_idx_0] - xl->data[m];
      a = P->data[xp_idx_0 + P->size[0]] - yl->data[m];
      dist_point = sqrt(dist_point * dist_point + a * a);
      if (dist_point < D->data[m + (D->size[0] << 1)]) {
        D->data[m] = 1.0 + (double)xp_idx_0;
        D->data[m + D->size[0]] = 0.0;
        D->data[m + (D->size[0] << 1)] = dist_point;
      }

      if (1.0 + (double)xp_idx_0 > 1.0) {
        b_P[0] = P->data[(int)((1.0 + (double)xp_idx_0) - 1.0) - 1];
        b_P[1] = P->data[((int)((1.0 + (double)xp_idx_0) - 1.0) + P->size[0]) -
          1];
        c_P[0] = P->data[xp_idx_0];
        c_P[1] = P->data[xp_idx_0 + P->size[0]];
        b_xl[0] = xl->data[m];
        b_xl[1] = yl->data[m];
        d_line_point(b_P, c_P, b_xl, &dist_point, &a, unusedU2);
        if ((dist_point < D->data[m + (D->size[0] << 1)]) && (a > 0.0) && (a <
             1.0)) {
          D->data[m] = (1.0 + (double)xp_idx_0) - 1.0;
          D->data[m + D->size[0]] = a;
          D->data[m + (D->size[0] << 1)] = dist_point;
        }
      }
    }
  }

  emxInit_boolean_T(&ind, 1);

  /*  Messpunkte ausfiltern, die vor dem letzten prädizierten Punkt oder zu weit entfernt liegen */
  loop_ub = D->size[0];
  xp_idx_0 = r->size[0];
  i5 = ind->size[0];
  ind->size[0] = loop_ub;
  emxEnsureCapacity((emxArray__common *)ind, i5, (int)sizeof(boolean_T));
  for (i5 = 0; i5 < loop_ub; i5++) {
    ind->data[i5] = ((D->data[i5] == xp_idx_0) || (D->data[i5 + (D->size[0] << 1)]
      > 0.5));
  }

  emxInit_real_T(&b_D, 1);
  eml_null_assignment(xl, ind);
  eml_null_assignment(yl, ind);
  b_eml_null_assignment(D, ind);

  /*  Messmatrix, Mess- und Erwartungsvektor bauen */
  loop_ub = D->size[0];
  i5 = b_D->size[0];
  b_D->size[0] = loop_ub;
  emxEnsureCapacity((emxArray__common *)b_D, i5, (int)sizeof(double));
  for (i5 = 0; i5 < loop_ub; i5++) {
    b_D->data[i5] = D->data[i5];
  }

  emxInit_real_T(&c_D, 1);
  loop_ub = D->size[0];
  i5 = c_D->size[0];
  c_D->size[0] = loop_ub;
  emxEnsureCapacity((emxArray__common *)c_D, i5, (int)sizeof(double));
  for (i5 = 0; i5 < loop_ub; i5++) {
    c_D->data[i5] = D->data[i5 + D->size[0]];
  }

  b_emxInit_real_T(&Hl, 2);
  emxInit_real_T(&zl, 1);
  messmatrix(P, r, delta, b_D, c_D, Hl);
  i5 = zl->size[0];
  zl->size[0] = (int)(2.0 * (double)xl->size[0]);
  emxEnsureCapacity((emxArray__common *)zl, i5, (int)sizeof(double));
  loop_ub = (int)(2.0 * (double)xl->size[0]);
  emxFree_real_T(&c_D);
  emxFree_real_T(&b_D);
  for (i5 = 0; i5 < loop_ub; i5++) {
    zl->data[i5] = 0.0;
  }

  emxInit_real_T(&zml, 1);
  i5 = zml->size[0];
  zml->size[0] = (int)(2.0 * (double)xl->size[0]);
  emxEnsureCapacity((emxArray__common *)zml, i5, (int)sizeof(double));
  loop_ub = (int)(2.0 * (double)xl->size[0]);
  for (i5 = 0; i5 < loop_ub; i5++) {
    zml->data[i5] = 0.0;
  }

  count = 2U;
  for (m = 0; m < xl->size[0]; m++) {
    zl->data[(int)count - 2] = xl->data[m];
    zl->data[(int)count - 1] = yl->data[m];
    zml->data[(int)count - 2] = P->data[(int)D->data[m] - 1] + D->data[m +
      D->size[0]] * (P->data[(int)(D->data[m] + 1.0) - 1] - P->data[(int)D->
                     data[m] - 1]);
    zml->data[(int)count - 1] = P->data[((int)D->data[m] + P->size[0]) - 1] +
      D->data[m + D->size[0]] * (P->data[((int)(D->data[m] + 1.0) + P->size[0])
      - 1] - P->data[((int)D->data[m] + P->size[0]) - 1]);
    count += 2U;
  }

  /* % rechte Seitenlinie */
  b_projectPoints(r, delta, xp, z_m, zmm);

  /* Punkte von Mittellinie nach rechts projizieren */
  xp_idx_0 = xp->size[0];
  br = z_m->size[0];
  ar = zmm->size[0];
  i5 = P->size[0] * P->size[1];
  P->size[0] = xp_idx_0;
  P->size[1] = 3;
  emxEnsureCapacity((emxArray__common *)P, i5, (int)sizeof(double));
  for (i5 = 0; i5 < xp_idx_0; i5++) {
    P->data[i5] = xp->data[i5];
  }

  for (i5 = 0; i5 < br; i5++) {
    P->data[i5 + P->size[0]] = z_m->data[i5];
  }

  for (i5 = 0; i5 < ar; i5++) {
    P->data[i5 + (P->size[0] << 1)] = zmm->data[i5];
  }

  xp_idx_0 = xr->size[0];
  i5 = D->size[0] * D->size[1];
  D->size[0] = xp_idx_0;
  D->size[1] = 3;
  emxEnsureCapacity((emxArray__common *)D, i5, (int)sizeof(double));
  loop_ub = xr->size[0] * 3;
  for (i5 = 0; i5 < loop_ub; i5++) {
    D->data[i5] = 10000.0;
  }

  /*  Fuer jeden Messpunkt den nähesten Punkt der aktuellen Praediktion finden */
  for (xp_idx_0 = 0; xp_idx_0 < r->size[0]; xp_idx_0++) {
    for (m = 0; m < xr->size[0]; m++) {
      dist_point = P->data[xp_idx_0] - xr->data[m];
      a = P->data[xp_idx_0 + P->size[0]] - yr->data[m];
      dist_point = sqrt(dist_point * dist_point + a * a);
      if (dist_point < D->data[m + (D->size[0] << 1)]) {
        D->data[m] = 1.0 + (double)xp_idx_0;
        D->data[m + D->size[0]] = 0.0;
        D->data[m + (D->size[0] << 1)] = dist_point;
      }

      if (1.0 + (double)xp_idx_0 > 1.0) {
        d_P[0] = P->data[(int)((1.0 + (double)xp_idx_0) - 1.0) - 1];
        d_P[1] = P->data[((int)((1.0 + (double)xp_idx_0) - 1.0) + P->size[0]) -
          1];
        e_P[0] = P->data[xp_idx_0];
        e_P[1] = P->data[xp_idx_0 + P->size[0]];
        b_xr[0] = xr->data[m];
        b_xr[1] = yr->data[m];
        d_line_point(d_P, e_P, b_xr, &dist_point, &a, unusedU2);
        if ((dist_point < D->data[m + (D->size[0] << 1)]) && (a > 0.0) && (a <
             1.0)) {
          D->data[m] = (1.0 + (double)xp_idx_0) - 1.0;
          D->data[m + D->size[0]] = a;
          D->data[m + (D->size[0] << 1)] = dist_point;
        }
      }
    }
  }

  /*  Messpunkte ausfiltern, die vor dem letzten prädizierten Punkt oder zu weit entfernt liegen */
  /*  ind = ((D(:, 1) == numel(r) & D(:, 2) == 0) | D(:, 3) > 0.5 ); */
  loop_ub = D->size[0];
  xp_idx_0 = r->size[0];
  i5 = ind->size[0];
  ind->size[0] = loop_ub;
  emxEnsureCapacity((emxArray__common *)ind, i5, (int)sizeof(boolean_T));
  for (i5 = 0; i5 < loop_ub; i5++) {
    ind->data[i5] = ((D->data[i5] == xp_idx_0) || (D->data[i5 + (D->size[0] << 1)]
      > 0.5));
  }

  emxInit_real_T(&d_D, 1);
  eml_null_assignment(xr, ind);
  eml_null_assignment(yr, ind);
  b_eml_null_assignment(D, ind);

  /*  Messmatrix, Mess- und Erwartungsvektor bauen */
  loop_ub = D->size[0];
  i5 = d_D->size[0];
  d_D->size[0] = loop_ub;
  emxEnsureCapacity((emxArray__common *)d_D, i5, (int)sizeof(double));
  for (i5 = 0; i5 < loop_ub; i5++) {
    d_D->data[i5] = D->data[i5];
  }

  emxInit_real_T(&e_D, 1);
  loop_ub = D->size[0];
  i5 = e_D->size[0];
  e_D->size[0] = loop_ub;
  emxEnsureCapacity((emxArray__common *)e_D, i5, (int)sizeof(double));
  for (i5 = 0; i5 < loop_ub; i5++) {
    e_D->data[i5] = D->data[i5 + D->size[0]];
  }

  b_emxInit_real_T(&Hr, 2);
  emxInit_real_T(&zr, 1);
  messmatrix(P, r, delta, d_D, e_D, Hr);
  i5 = zr->size[0];
  zr->size[0] = (int)(2.0 * (double)xr->size[0]);
  emxEnsureCapacity((emxArray__common *)zr, i5, (int)sizeof(double));
  loop_ub = (int)(2.0 * (double)xr->size[0]);
  emxFree_real_T(&e_D);
  emxFree_real_T(&d_D);
  for (i5 = 0; i5 < loop_ub; i5++) {
    zr->data[i5] = 0.0;
  }

  emxInit_real_T(&zmr, 1);
  i5 = zmr->size[0];
  zmr->size[0] = (int)(2.0 * (double)xr->size[0]);
  emxEnsureCapacity((emxArray__common *)zmr, i5, (int)sizeof(double));
  loop_ub = (int)(2.0 * (double)xr->size[0]);
  for (i5 = 0; i5 < loop_ub; i5++) {
    zmr->data[i5] = 0.0;
  }

  count = 2U;
  for (m = 0; m < xr->size[0]; m++) {
    zr->data[(int)count - 2] = xr->data[m];
    zr->data[(int)count - 1] = yr->data[m];
    zmr->data[(int)count - 2] = P->data[(int)D->data[m] - 1] + D->data[m +
      D->size[0]] * (P->data[(int)(D->data[m] + 1.0) - 1] - P->data[(int)D->
                     data[m] - 1]);
    zmr->data[(int)count - 1] = P->data[((int)D->data[m] + P->size[0]) - 1] +
      D->data[m + D->size[0]] * (P->data[((int)(D->data[m] + 1.0) + P->size[0])
      - 1] - P->data[((int)D->data[m] + P->size[0]) - 1]);
    count += 2U;
  }

  /* % Mittellinie */
  getPointsFromState(r, delta, xp, z_m, zmm);
  xp_idx_0 = xp->size[0];
  br = z_m->size[0];
  ar = zmm->size[0];
  i5 = P->size[0] * P->size[1];
  P->size[0] = xp_idx_0;
  P->size[1] = 3;
  emxEnsureCapacity((emxArray__common *)P, i5, (int)sizeof(double));
  for (i5 = 0; i5 < xp_idx_0; i5++) {
    P->data[i5] = xp->data[i5];
  }

  for (i5 = 0; i5 < br; i5++) {
    P->data[i5 + P->size[0]] = z_m->data[i5];
  }

  for (i5 = 0; i5 < ar; i5++) {
    P->data[i5 + (P->size[0] << 1)] = zmm->data[i5];
  }

  xp_idx_0 = xm->size[0];
  i5 = D->size[0] * D->size[1];
  D->size[0] = xp_idx_0;
  D->size[1] = 3;
  emxEnsureCapacity((emxArray__common *)D, i5, (int)sizeof(double));
  loop_ub = xm->size[0] * 3;
  for (i5 = 0; i5 < loop_ub; i5++) {
    D->data[i5] = 10000.0;
  }

  /*  Fuer jeden Messpunkt den nähesten Punkt der aktuellen Praediktion finden  */
  for (xp_idx_0 = 0; xp_idx_0 < r->size[0]; xp_idx_0++) {
    for (m = 0; m < xm->size[0]; m++) {
      dist_point = P->data[xp_idx_0] - xm->data[m];
      a = P->data[xp_idx_0 + P->size[0]] - ym->data[m];
      dist_point = sqrt(dist_point * dist_point + a * a);
      if (dist_point < D->data[m + (D->size[0] << 1)]) {
        D->data[m] = 1.0 + (double)xp_idx_0;
        D->data[m + D->size[0]] = 0.0;
        D->data[m + (D->size[0] << 1)] = dist_point;
      }

      if (1.0 + (double)xp_idx_0 > 1.0) {
        f_P[0] = P->data[(int)((1.0 + (double)xp_idx_0) - 1.0) - 1];
        f_P[1] = P->data[((int)((1.0 + (double)xp_idx_0) - 1.0) + P->size[0]) -
          1];
        g_P[0] = P->data[xp_idx_0];
        g_P[1] = P->data[xp_idx_0 + P->size[0]];
        b_xm[0] = xm->data[m];
        b_xm[1] = ym->data[m];
        d_line_point(f_P, g_P, b_xm, &dist_point, &a, unusedU2);
        if ((dist_point < D->data[m + (D->size[0] << 1)]) && (a > 0.0) && (a <
             1.0)) {
          D->data[m] = (1.0 + (double)xp_idx_0) - 1.0;
          D->data[m + D->size[0]] = a;
          D->data[m + (D->size[0] << 1)] = dist_point;
        }
      }
    }
  }

  /*  Messpunkte ausfiltern, die vor dem letzten prädizierten Punkt oder zu weit entfernt liegen */
  loop_ub = D->size[0];
  xp_idx_0 = r->size[0];
  i5 = ind->size[0];
  ind->size[0] = loop_ub;
  emxEnsureCapacity((emxArray__common *)ind, i5, (int)sizeof(boolean_T));
  for (i5 = 0; i5 < loop_ub; i5++) {
    ind->data[i5] = ((D->data[i5] == xp_idx_0) || (D->data[i5 + (D->size[0] << 1)]
      > 0.5));
  }

  emxInit_real_T(&f_D, 1);
  eml_null_assignment(xm, ind);
  eml_null_assignment(ym, ind);
  b_eml_null_assignment(D, ind);

  /*  Messmatrix, Mess- und Erwartungsvektor bauen */
  loop_ub = D->size[0];
  i5 = f_D->size[0];
  f_D->size[0] = loop_ub;
  emxEnsureCapacity((emxArray__common *)f_D, i5, (int)sizeof(double));
  emxFree_boolean_T(&ind);
  for (i5 = 0; i5 < loop_ub; i5++) {
    f_D->data[i5] = D->data[i5];
  }

  emxInit_real_T(&g_D, 1);
  loop_ub = D->size[0];
  i5 = g_D->size[0];
  g_D->size[0] = loop_ub;
  emxEnsureCapacity((emxArray__common *)g_D, i5, (int)sizeof(double));
  for (i5 = 0; i5 < loop_ub; i5++) {
    g_D->data[i5] = D->data[i5 + D->size[0]];
  }

  b_emxInit_real_T(&Hm, 2);
  messmatrix(P, r, delta, f_D, g_D, Hm);
  i5 = z_m->size[0];
  z_m->size[0] = (int)(2.0 * (double)xm->size[0]);
  emxEnsureCapacity((emxArray__common *)z_m, i5, (int)sizeof(double));
  loop_ub = (int)(2.0 * (double)xm->size[0]);
  emxFree_real_T(&g_D);
  emxFree_real_T(&f_D);
  for (i5 = 0; i5 < loop_ub; i5++) {
    z_m->data[i5] = 0.0;
  }

  i5 = zmm->size[0];
  zmm->size[0] = (int)(2.0 * (double)xm->size[0]);
  emxEnsureCapacity((emxArray__common *)zmm, i5, (int)sizeof(double));
  loop_ub = (int)(2.0 * (double)xm->size[0]);
  for (i5 = 0; i5 < loop_ub; i5++) {
    zmm->data[i5] = 0.0;
  }

  count = 2U;
  for (m = 0; m < xm->size[0]; m++) {
    z_m->data[(int)count - 2] = xm->data[m];
    z_m->data[(int)count - 1] = ym->data[m];
    zmm->data[(int)count - 2] = P->data[(int)D->data[m] - 1] + D->data[m +
      D->size[0]] * (P->data[(int)(D->data[m] + 1.0) - 1] - P->data[(int)D->
                     data[m] - 1]);
    zmm->data[(int)count - 1] = P->data[((int)D->data[m] + P->size[0]) - 1] +
      D->data[m + D->size[0]] * (P->data[((int)(D->data[m] + 1.0) + P->size[0])
      - 1] - P->data[((int)D->data[m] + P->size[0]) - 1]);
    count += 2U;
  }

  emxFree_real_T(&D);
  emxFree_real_T(&P);
  b_emxInit_real_T(&H, 2);

  /* % Linke und rechte Linie kombinieren */
  i5 = H->size[0] * H->size[1];
  H->size[0] = (Hl->size[0] + Hr->size[0]) + Hm->size[0];
  H->size[1] = Hl->size[1];
  emxEnsureCapacity((emxArray__common *)H, i5, (int)sizeof(double));
  loop_ub = Hl->size[1];
  for (i5 = 0; i5 < loop_ub; i5++) {
    ar = Hl->size[0];
    for (i6 = 0; i6 < ar; i6++) {
      H->data[i6 + H->size[0] * i5] = Hl->data[i6 + Hl->size[0] * i5];
    }
  }

  loop_ub = Hr->size[1];
  for (i5 = 0; i5 < loop_ub; i5++) {
    ar = Hr->size[0];
    for (i6 = 0; i6 < ar; i6++) {
      H->data[(i6 + Hl->size[0]) + H->size[0] * i5] = Hr->data[i6 + Hr->size[0] *
        i5];
    }
  }

  loop_ub = Hm->size[1];
  for (i5 = 0; i5 < loop_ub; i5++) {
    ar = Hm->size[0];
    for (i6 = 0; i6 < ar; i6++) {
      H->data[((i6 + Hl->size[0]) + Hr->size[0]) + H->size[0] * i5] = Hm->
        data[i6 + Hm->size[0] * i5];
    }
  }

  /* Messmatrix */
  /* Messwerte */
  /* erwartete Werte */
  /* % KALMAN FILTER     */
  /* state transition (experimentell und vereinfacht) */
  xp_idx_0 = r->size[0];
  i5 = Hl->size[0] * Hl->size[1];
  Hl->size[0] = xp_idx_0;
  emxEnsureCapacity((emxArray__common *)Hl, i5, (int)sizeof(double));
  xp_idx_0 = r->size[0];
  i5 = Hl->size[0] * Hl->size[1];
  Hl->size[1] = xp_idx_0;
  emxEnsureCapacity((emxArray__common *)Hl, i5, (int)sizeof(double));
  loop_ub = r->size[0] * r->size[0];
  for (i5 = 0; i5 < loop_ub; i5++) {
    Hl->data[i5] = 0.0;
  }

  Hl->data[0] = 1.0;
  Hl->data[1 + Hl->size[0]] = 1.0;
  Hl->data[1 + (Hl->size[0] << 1)] = delta_x / sqrt(1.0 - delta * delta *
    (r->data[2] * r->data[2]) / 4.0);
  for (xp_idx_0 = 0; xp_idx_0 < (int)(((double)r->size[0] - 1.0) + -2.0);
       xp_idx_0++) {
    Hl->data[(xp_idx_0 + Hl->size[0] * (xp_idx_0 + 2)) + 2] = 1.0 - delta_x /
      delta;
    Hl->data[(xp_idx_0 + Hl->size[0] * ((int)((3.0 + (double)xp_idx_0) + 1.0) -
               1)) + 2] = delta_x / delta;
  }

  Hl->data[(r->size[0] + Hl->size[0] * (r->size[0] - 1)) - 1] = 1.0;
  i5 = xp->size[0];
  xp->size[0] = r->size[0];
  emxEnsureCapacity((emxArray__common *)xp, i5, (int)sizeof(double));
  loop_ub = r->size[0];
  for (i5 = 0; i5 < loop_ub; i5++) {
    xp->data[i5] = r->data[i5];
  }

  emxInit_real_T(&b_Hl, 1);
  if ((Hl->size[1] == 1) || (r->size[0] == 1)) {
    i5 = b_Hl->size[0];
    b_Hl->size[0] = Hl->size[0];
    emxEnsureCapacity((emxArray__common *)b_Hl, i5, (int)sizeof(double));
    loop_ub = Hl->size[0];
    for (i5 = 0; i5 < loop_ub; i5++) {
      b_Hl->data[i5] = 0.0;
      ar = Hl->size[1];
      for (i6 = 0; i6 < ar; i6++) {
        b_Hl->data[i5] += Hl->data[i5 + Hl->size[0] * i6] * r->data[i6];
      }
    }

    i5 = r->size[0];
    r->size[0] = b_Hl->size[0];
    emxEnsureCapacity((emxArray__common *)r, i5, (int)sizeof(double));
    loop_ub = b_Hl->size[0];
    for (i5 = 0; i5 < loop_ub; i5++) {
      r->data[i5] = b_Hl->data[i5];
    }
  } else {
    k = Hl->size[1];
    unusedU2[0] = Hl->size[0];
    m = Hl->size[0];
    i5 = r->size[0];
    r->size[0] = (int)unusedU2[0];
    emxEnsureCapacity((emxArray__common *)r, i5, (int)sizeof(double));
    loop_ub = (int)unusedU2[0];
    for (i5 = 0; i5 < loop_ub; i5++) {
      r->data[i5] = 0.0;
    }

    if (Hl->size[0] == 0) {
    } else {
      xp_idx_0 = 0;
      while ((m > 0) && (xp_idx_0 <= 0)) {
        for (ic = 0; ic + 1 <= m; ic++) {
          r->data[ic] = 0.0;
        }

        xp_idx_0 = m;
      }

      br = 0;
      xp_idx_0 = 0;
      while ((m > 0) && (xp_idx_0 <= 0)) {
        ar = 0;
        i5 = br + k;
        for (loop_ub = br; loop_ub + 1 <= i5; loop_ub++) {
          if (xp->data[loop_ub] != 0.0) {
            ia = ar;
            for (ic = 0; ic + 1 <= m; ic++) {
              ia++;
              r->data[ic] += xp->data[loop_ub] * Hl->data[ia - 1];
            }
          }

          ar += m;
        }

        br += k;
        xp_idx_0 = m;
      }
    }
  }

  emxFree_real_T(&b_Hl);
  r->data[0] -= delta_y;
  r->data[1] -= delta_phi;
  eye(2.0 * ((double)((unsigned int)xl->size[0] + xr->size[0]) + (double)
             xm->size[0]), Hr);
  i5 = Hr->size[0] * Hr->size[1];
  emxEnsureCapacity((emxArray__common *)Hr, i5, (int)sizeof(double));
  xp_idx_0 = Hr->size[0];
  br = Hr->size[1];
  loop_ub = xp_idx_0 * br;
  for (i5 = 0; i5 < loop_ub; i5++) {
    Hr->data[i5] *= R_fakt;
  }

  if ((Hl->size[1] == 1) || (Pk->size[0] == 1)) {
    i5 = Hm->size[0] * Hm->size[1];
    Hm->size[0] = Hl->size[0];
    Hm->size[1] = Pk->size[1];
    emxEnsureCapacity((emxArray__common *)Hm, i5, (int)sizeof(double));
    loop_ub = Hl->size[0];
    for (i5 = 0; i5 < loop_ub; i5++) {
      ar = Pk->size[1];
      for (i6 = 0; i6 < ar; i6++) {
        Hm->data[i5 + Hm->size[0] * i6] = 0.0;
        xp_idx_0 = Hl->size[1];
        for (br = 0; br < xp_idx_0; br++) {
          Hm->data[i5 + Hm->size[0] * i6] += Hl->data[i5 + Hl->size[0] * br] *
            Pk->data[br + Pk->size[0] * i6];
        }
      }
    }
  } else {
    k = Hl->size[1];
    unusedU2[0] = (unsigned int)Hl->size[0];
    unusedU2[1] = (unsigned int)Pk->size[1];
    m = Hl->size[0];
    i5 = Hm->size[0] * Hm->size[1];
    Hm->size[0] = (int)unusedU2[0];
    emxEnsureCapacity((emxArray__common *)Hm, i5, (int)sizeof(double));
    i5 = Hm->size[0] * Hm->size[1];
    Hm->size[1] = (int)unusedU2[1];
    emxEnsureCapacity((emxArray__common *)Hm, i5, (int)sizeof(double));
    loop_ub = (int)unusedU2[0] * (int)unusedU2[1];
    for (i5 = 0; i5 < loop_ub; i5++) {
      Hm->data[i5] = 0.0;
    }

    if ((Hl->size[0] == 0) || (Pk->size[1] == 0)) {
    } else {
      c = Hl->size[0] * (Pk->size[1] - 1);
      xp_idx_0 = 0;
      while ((m > 0) && (xp_idx_0 <= c)) {
        i5 = xp_idx_0 + m;
        for (ic = xp_idx_0; ic + 1 <= i5; ic++) {
          Hm->data[ic] = 0.0;
        }

        xp_idx_0 += m;
      }

      br = 0;
      xp_idx_0 = 0;
      while ((m > 0) && (xp_idx_0 <= c)) {
        ar = 0;
        i5 = br + k;
        for (loop_ub = br; loop_ub + 1 <= i5; loop_ub++) {
          if (Pk->data[loop_ub] != 0.0) {
            ia = ar;
            i6 = xp_idx_0 + m;
            for (ic = xp_idx_0; ic + 1 <= i6; ic++) {
              ia++;
              Hm->data[ic] += Pk->data[loop_ub] * Hl->data[ia - 1];
            }
          }

          ar += m;
        }

        br += k;
        xp_idx_0 += m;
      }
    }
  }

  b_emxInit_real_T(&b, 2);
  i5 = b->size[0] * b->size[1];
  b->size[0] = Hl->size[1];
  b->size[1] = Hl->size[0];
  emxEnsureCapacity((emxArray__common *)b, i5, (int)sizeof(double));
  loop_ub = Hl->size[0];
  for (i5 = 0; i5 < loop_ub; i5++) {
    ar = Hl->size[1];
    for (i6 = 0; i6 < ar; i6++) {
      b->data[i6 + b->size[0] * i5] = Hl->data[i5 + Hl->size[0] * i6];
    }
  }

  if ((Hm->size[1] == 1) || (b->size[0] == 1)) {
    i5 = Pk->size[0] * Pk->size[1];
    Pk->size[0] = Hm->size[0];
    Pk->size[1] = b->size[1];
    emxEnsureCapacity((emxArray__common *)Pk, i5, (int)sizeof(double));
    loop_ub = Hm->size[0];
    for (i5 = 0; i5 < loop_ub; i5++) {
      ar = b->size[1];
      for (i6 = 0; i6 < ar; i6++) {
        Pk->data[i5 + Pk->size[0] * i6] = 0.0;
        xp_idx_0 = Hm->size[1];
        for (br = 0; br < xp_idx_0; br++) {
          Pk->data[i5 + Pk->size[0] * i6] += Hm->data[i5 + Hm->size[0] * br] *
            b->data[br + b->size[0] * i6];
        }
      }
    }
  } else {
    k = Hm->size[1];
    unusedU2[0] = (unsigned int)Hm->size[0];
    unusedU2[1] = (unsigned int)b->size[1];
    m = Hm->size[0];
    i5 = Pk->size[0] * Pk->size[1];
    Pk->size[0] = (int)unusedU2[0];
    Pk->size[1] = (int)unusedU2[1];
    emxEnsureCapacity((emxArray__common *)Pk, i5, (int)sizeof(double));
    loop_ub = (int)unusedU2[1];
    for (i5 = 0; i5 < loop_ub; i5++) {
      ar = (int)unusedU2[0];
      for (i6 = 0; i6 < ar; i6++) {
        Pk->data[i6 + Pk->size[0] * i5] = 0.0;
      }
    }

    if ((Hm->size[0] == 0) || (b->size[1] == 0)) {
    } else {
      c = Hm->size[0] * (b->size[1] - 1);
      xp_idx_0 = 0;
      while ((m > 0) && (xp_idx_0 <= c)) {
        i5 = xp_idx_0 + m;
        for (ic = xp_idx_0; ic + 1 <= i5; ic++) {
          Pk->data[ic] = 0.0;
        }

        xp_idx_0 += m;
      }

      br = 0;
      xp_idx_0 = 0;
      while ((m > 0) && (xp_idx_0 <= c)) {
        ar = 0;
        i5 = br + k;
        for (loop_ub = br; loop_ub + 1 <= i5; loop_ub++) {
          if (b->data[loop_ub] != 0.0) {
            ia = ar;
            i6 = xp_idx_0 + m;
            for (ic = xp_idx_0; ic + 1 <= i6; ic++) {
              ia++;
              Pk->data[ic] += b->data[loop_ub] * Hm->data[ia - 1];
            }
          }

          ar += m;
        }

        br += k;
        xp_idx_0 += m;
      }
    }
  }

  i5 = Pk->size[0] * Pk->size[1];
  emxEnsureCapacity((emxArray__common *)Pk, i5, (int)sizeof(double));
  loop_ub = Pk->size[1];
  for (i5 = 0; i5 < loop_ub; i5++) {
    ar = Pk->size[0];
    for (i6 = 0; i6 < ar; i6++) {
      Pk->data[i6 + Pk->size[0] * i5] += Q->data[i6 + Q->size[0] * i5];
    }
  }

  emxInit_real_T(&b_zl, 1);
  emxInit_real_T(&b_zml, 1);
  i5 = b_zl->size[0];
  b_zl->size[0] = (zl->size[0] + zr->size[0]) + z_m->size[0];
  emxEnsureCapacity((emxArray__common *)b_zl, i5, (int)sizeof(double));
  loop_ub = zl->size[0];
  for (i5 = 0; i5 < loop_ub; i5++) {
    b_zl->data[i5] = zl->data[i5];
  }

  loop_ub = zr->size[0];
  for (i5 = 0; i5 < loop_ub; i5++) {
    b_zl->data[i5 + zl->size[0]] = zr->data[i5];
  }

  loop_ub = z_m->size[0];
  for (i5 = 0; i5 < loop_ub; i5++) {
    b_zl->data[(i5 + zl->size[0]) + zr->size[0]] = z_m->data[i5];
  }

  emxFree_real_T(&z_m);
  emxFree_real_T(&zr);
  emxFree_real_T(&zl);
  i5 = b_zml->size[0];
  b_zml->size[0] = (zml->size[0] + zmr->size[0]) + zmm->size[0];
  emxEnsureCapacity((emxArray__common *)b_zml, i5, (int)sizeof(double));
  loop_ub = zml->size[0];
  for (i5 = 0; i5 < loop_ub; i5++) {
    b_zml->data[i5] = zml->data[i5];
  }

  loop_ub = zmr->size[0];
  for (i5 = 0; i5 < loop_ub; i5++) {
    b_zml->data[i5 + zml->size[0]] = zmr->data[i5];
  }

  loop_ub = zmm->size[0];
  for (i5 = 0; i5 < loop_ub; i5++) {
    b_zml->data[(i5 + zml->size[0]) + zmr->size[0]] = zmm->data[i5];
  }

  emxFree_real_T(&zmm);
  emxFree_real_T(&zmr);
  emxFree_real_T(&zml);
  emxInit_real_T(&y_tilde, 1);
  i5 = y_tilde->size[0];
  y_tilde->size[0] = b_zl->size[0];
  emxEnsureCapacity((emxArray__common *)y_tilde, i5, (int)sizeof(double));
  loop_ub = b_zl->size[0];
  for (i5 = 0; i5 < loop_ub; i5++) {
    y_tilde->data[i5] = b_zl->data[i5] - b_zml->data[i5];
  }

  emxFree_real_T(&b_zml);
  emxFree_real_T(&b_zl);
  if ((H->size[1] == 1) || (Pk->size[0] == 1)) {
    i5 = Hm->size[0] * Hm->size[1];
    Hm->size[0] = H->size[0];
    Hm->size[1] = Pk->size[1];
    emxEnsureCapacity((emxArray__common *)Hm, i5, (int)sizeof(double));
    loop_ub = H->size[0];
    for (i5 = 0; i5 < loop_ub; i5++) {
      ar = Pk->size[1];
      for (i6 = 0; i6 < ar; i6++) {
        Hm->data[i5 + Hm->size[0] * i6] = 0.0;
        xp_idx_0 = H->size[1];
        for (br = 0; br < xp_idx_0; br++) {
          Hm->data[i5 + Hm->size[0] * i6] += H->data[i5 + H->size[0] * br] *
            Pk->data[br + Pk->size[0] * i6];
        }
      }
    }
  } else {
    k = H->size[1];
    unusedU2[0] = (unsigned int)H->size[0];
    unusedU2[1] = (unsigned int)Pk->size[1];
    m = H->size[0];
    i5 = Hm->size[0] * Hm->size[1];
    Hm->size[0] = (int)unusedU2[0];
    emxEnsureCapacity((emxArray__common *)Hm, i5, (int)sizeof(double));
    i5 = Hm->size[0] * Hm->size[1];
    Hm->size[1] = (int)unusedU2[1];
    emxEnsureCapacity((emxArray__common *)Hm, i5, (int)sizeof(double));
    loop_ub = (int)unusedU2[0] * (int)unusedU2[1];
    for (i5 = 0; i5 < loop_ub; i5++) {
      Hm->data[i5] = 0.0;
    }

    if ((H->size[0] == 0) || (Pk->size[1] == 0)) {
    } else {
      c = H->size[0] * (Pk->size[1] - 1);
      xp_idx_0 = 0;
      while ((m > 0) && (xp_idx_0 <= c)) {
        i5 = xp_idx_0 + m;
        for (ic = xp_idx_0; ic + 1 <= i5; ic++) {
          Hm->data[ic] = 0.0;
        }

        xp_idx_0 += m;
      }

      br = 0;
      xp_idx_0 = 0;
      while ((m > 0) && (xp_idx_0 <= c)) {
        ar = 0;
        i5 = br + k;
        for (loop_ub = br; loop_ub + 1 <= i5; loop_ub++) {
          if (Pk->data[loop_ub] != 0.0) {
            ia = ar;
            i6 = xp_idx_0 + m;
            for (ic = xp_idx_0; ic + 1 <= i6; ic++) {
              ia++;
              Hm->data[ic] += Pk->data[loop_ub] * H->data[ia - 1];
            }
          }

          ar += m;
        }

        br += k;
        xp_idx_0 += m;
      }
    }
  }

  i5 = b->size[0] * b->size[1];
  b->size[0] = H->size[1];
  b->size[1] = H->size[0];
  emxEnsureCapacity((emxArray__common *)b, i5, (int)sizeof(double));
  loop_ub = H->size[0];
  for (i5 = 0; i5 < loop_ub; i5++) {
    ar = H->size[1];
    for (i6 = 0; i6 < ar; i6++) {
      b->data[i6 + b->size[0] * i5] = H->data[i5 + H->size[0] * i6];
    }
  }

  b_emxInit_real_T(&C, 2);
  if ((Hm->size[1] == 1) || (b->size[0] == 1)) {
    i5 = C->size[0] * C->size[1];
    C->size[0] = Hm->size[0];
    C->size[1] = b->size[1];
    emxEnsureCapacity((emxArray__common *)C, i5, (int)sizeof(double));
    loop_ub = Hm->size[0];
    for (i5 = 0; i5 < loop_ub; i5++) {
      ar = b->size[1];
      for (i6 = 0; i6 < ar; i6++) {
        C->data[i5 + C->size[0] * i6] = 0.0;
        xp_idx_0 = Hm->size[1];
        for (br = 0; br < xp_idx_0; br++) {
          C->data[i5 + C->size[0] * i6] += Hm->data[i5 + Hm->size[0] * br] *
            b->data[br + b->size[0] * i6];
        }
      }
    }
  } else {
    k = Hm->size[1];
    unusedU2[0] = (unsigned int)Hm->size[0];
    unusedU2[1] = (unsigned int)b->size[1];
    m = Hm->size[0];
    i5 = C->size[0] * C->size[1];
    C->size[0] = (int)unusedU2[0];
    emxEnsureCapacity((emxArray__common *)C, i5, (int)sizeof(double));
    i5 = C->size[0] * C->size[1];
    C->size[1] = (int)unusedU2[1];
    emxEnsureCapacity((emxArray__common *)C, i5, (int)sizeof(double));
    loop_ub = (int)unusedU2[0] * (int)unusedU2[1];
    for (i5 = 0; i5 < loop_ub; i5++) {
      C->data[i5] = 0.0;
    }

    if ((Hm->size[0] == 0) || (b->size[1] == 0)) {
    } else {
      c = Hm->size[0] * (b->size[1] - 1);
      xp_idx_0 = 0;
      while ((m > 0) && (xp_idx_0 <= c)) {
        i5 = xp_idx_0 + m;
        for (ic = xp_idx_0; ic + 1 <= i5; ic++) {
          C->data[ic] = 0.0;
        }

        xp_idx_0 += m;
      }

      br = 0;
      xp_idx_0 = 0;
      while ((m > 0) && (xp_idx_0 <= c)) {
        ar = 0;
        i5 = br + k;
        for (loop_ub = br; loop_ub + 1 <= i5; loop_ub++) {
          if (b->data[loop_ub] != 0.0) {
            ia = ar;
            i6 = xp_idx_0 + m;
            for (ic = xp_idx_0; ic + 1 <= i6; ic++) {
              ia++;
              C->data[ic] += b->data[loop_ub] * Hm->data[ia - 1];
            }
          }

          ar += m;
        }

        br += k;
        xp_idx_0 += m;
      }
    }
  }

  i5 = b->size[0] * b->size[1];
  b->size[0] = H->size[1];
  b->size[1] = H->size[0];
  emxEnsureCapacity((emxArray__common *)b, i5, (int)sizeof(double));
  loop_ub = H->size[0];
  for (i5 = 0; i5 < loop_ub; i5++) {
    ar = H->size[1];
    for (i6 = 0; i6 < ar; i6++) {
      b->data[i6 + b->size[0] * i5] = H->data[i5 + H->size[0] * i6];
    }
  }

  if ((Pk->size[1] == 1) || (b->size[0] == 1)) {
    i5 = Hm->size[0] * Hm->size[1];
    Hm->size[0] = Pk->size[0];
    Hm->size[1] = b->size[1];
    emxEnsureCapacity((emxArray__common *)Hm, i5, (int)sizeof(double));
    loop_ub = Pk->size[0];
    for (i5 = 0; i5 < loop_ub; i5++) {
      ar = b->size[1];
      for (i6 = 0; i6 < ar; i6++) {
        Hm->data[i5 + Hm->size[0] * i6] = 0.0;
        xp_idx_0 = Pk->size[1];
        for (br = 0; br < xp_idx_0; br++) {
          Hm->data[i5 + Hm->size[0] * i6] += Pk->data[i5 + Pk->size[0] * br] *
            b->data[br + b->size[0] * i6];
        }
      }
    }
  } else {
    k = Pk->size[1];
    unusedU2[0] = (unsigned int)Pk->size[0];
    unusedU2[1] = (unsigned int)b->size[1];
    m = Pk->size[0];
    i5 = Hm->size[0] * Hm->size[1];
    Hm->size[0] = (int)unusedU2[0];
    emxEnsureCapacity((emxArray__common *)Hm, i5, (int)sizeof(double));
    i5 = Hm->size[0] * Hm->size[1];
    Hm->size[1] = (int)unusedU2[1];
    emxEnsureCapacity((emxArray__common *)Hm, i5, (int)sizeof(double));
    loop_ub = (int)unusedU2[0] * (int)unusedU2[1];
    for (i5 = 0; i5 < loop_ub; i5++) {
      Hm->data[i5] = 0.0;
    }

    if ((Pk->size[0] == 0) || (b->size[1] == 0)) {
    } else {
      c = Pk->size[0] * (b->size[1] - 1);
      xp_idx_0 = 0;
      while (((m > 0) && (xp_idx_0 <= c)) || ((m < 0) && (xp_idx_0 >= c))) {
        i5 = xp_idx_0 + m;
        for (ic = xp_idx_0; ic + 1 <= i5; ic++) {
          Hm->data[ic] = 0.0;
        }

        xp_idx_0 += m;
      }

      br = 0;
      xp_idx_0 = 0;
      while (((m > 0) && (xp_idx_0 <= c)) || ((m < 0) && (xp_idx_0 >= c))) {
        ar = 0;
        i5 = br + k;
        for (loop_ub = br; loop_ub + 1 <= i5; loop_ub++) {
          if (b->data[loop_ub] != 0.0) {
            ia = ar;
            i6 = xp_idx_0 + m;
            for (ic = xp_idx_0; ic + 1 <= i6; ic++) {
              ia++;
              Hm->data[ic] += b->data[loop_ub] * Pk->data[ia - 1];
            }
          }

          ar += m;
        }

        br += k;
        xp_idx_0 += m;
      }
    }
  }

  b_emxInit_real_T(&b_C, 2);
  i5 = b_C->size[0] * b_C->size[1];
  b_C->size[0] = C->size[0];
  b_C->size[1] = C->size[1];
  emxEnsureCapacity((emxArray__common *)b_C, i5, (int)sizeof(double));
  loop_ub = C->size[0] * C->size[1];
  for (i5 = 0; i5 < loop_ub; i5++) {
    b_C->data[i5] = C->data[i5] + Hr->data[i5];
  }

  mrdivide(Hm, b_C, Hl);
  emxFree_real_T(&b_C);
  emxFree_real_T(&Hm);
  if ((Hl->size[1] == 1) || (y_tilde->size[0] == 1)) {
    i5 = xp->size[0];
    xp->size[0] = Hl->size[0];
    emxEnsureCapacity((emxArray__common *)xp, i5, (int)sizeof(double));
    loop_ub = Hl->size[0];
    for (i5 = 0; i5 < loop_ub; i5++) {
      xp->data[i5] = 0.0;
      ar = Hl->size[1];
      for (i6 = 0; i6 < ar; i6++) {
        xp->data[i5] += Hl->data[i5 + Hl->size[0] * i6] * y_tilde->data[i6];
      }
    }
  } else {
    k = Hl->size[1];
    unusedU2[0] = Hl->size[0];
    m = Hl->size[0];
    i5 = xp->size[0];
    xp->size[0] = (int)unusedU2[0];
    emxEnsureCapacity((emxArray__common *)xp, i5, (int)sizeof(double));
    loop_ub = (int)unusedU2[0];
    for (i5 = 0; i5 < loop_ub; i5++) {
      xp->data[i5] = 0.0;
    }

    if (Hl->size[0] == 0) {
    } else {
      xp_idx_0 = 0;
      while ((m > 0) && (xp_idx_0 <= 0)) {
        for (ic = 0; ic + 1 <= m; ic++) {
          xp->data[ic] = 0.0;
        }

        xp_idx_0 = m;
      }

      br = 0;
      xp_idx_0 = 0;
      while ((m > 0) && (xp_idx_0 <= 0)) {
        ar = 0;
        i5 = br + k;
        for (loop_ub = br; loop_ub + 1 <= i5; loop_ub++) {
          if (y_tilde->data[loop_ub] != 0.0) {
            ia = ar;
            for (ic = 0; ic + 1 <= m; ic++) {
              ia++;
              xp->data[ic] += y_tilde->data[loop_ub] * Hl->data[ia - 1];
            }
          }

          ar += m;
        }

        br += k;
        xp_idx_0 = m;
      }
    }
  }

  emxFree_real_T(&y_tilde);
  i5 = r->size[0];
  emxEnsureCapacity((emxArray__common *)r, i5, (int)sizeof(double));
  loop_ub = r->size[0];
  for (i5 = 0; i5 < loop_ub; i5++) {
    r->data[i5] += xp->data[i5];
  }

  emxFree_real_T(&xp);
  eye(r->size[0], Hr);
  if ((Hl->size[1] == 1) || (H->size[0] == 1)) {
    i5 = C->size[0] * C->size[1];
    C->size[0] = Hl->size[0];
    C->size[1] = H->size[1];
    emxEnsureCapacity((emxArray__common *)C, i5, (int)sizeof(double));
    loop_ub = Hl->size[0];
    for (i5 = 0; i5 < loop_ub; i5++) {
      ar = H->size[1];
      for (i6 = 0; i6 < ar; i6++) {
        C->data[i5 + C->size[0] * i6] = 0.0;
        xp_idx_0 = Hl->size[1];
        for (br = 0; br < xp_idx_0; br++) {
          C->data[i5 + C->size[0] * i6] += Hl->data[i5 + Hl->size[0] * br] *
            H->data[br + H->size[0] * i6];
        }
      }
    }
  } else {
    k = Hl->size[1];
    unusedU2[0] = (unsigned int)Hl->size[0];
    unusedU2[1] = (unsigned int)H->size[1];
    m = Hl->size[0];
    i5 = C->size[0] * C->size[1];
    C->size[0] = (int)unusedU2[0];
    emxEnsureCapacity((emxArray__common *)C, i5, (int)sizeof(double));
    i5 = C->size[0] * C->size[1];
    C->size[1] = (int)unusedU2[1];
    emxEnsureCapacity((emxArray__common *)C, i5, (int)sizeof(double));
    loop_ub = (int)unusedU2[0] * (int)unusedU2[1];
    for (i5 = 0; i5 < loop_ub; i5++) {
      C->data[i5] = 0.0;
    }

    if ((Hl->size[0] == 0) || (H->size[1] == 0)) {
    } else {
      c = Hl->size[0] * (H->size[1] - 1);
      xp_idx_0 = 0;
      while ((m > 0) && (xp_idx_0 <= c)) {
        i5 = xp_idx_0 + m;
        for (ic = xp_idx_0; ic + 1 <= i5; ic++) {
          C->data[ic] = 0.0;
        }

        xp_idx_0 += m;
      }

      br = 0;
      xp_idx_0 = 0;
      while ((m > 0) && (xp_idx_0 <= c)) {
        ar = 0;
        i5 = br + k;
        for (loop_ub = br; loop_ub + 1 <= i5; loop_ub++) {
          if (H->data[loop_ub] != 0.0) {
            ia = ar;
            i6 = xp_idx_0 + m;
            for (ic = xp_idx_0; ic + 1 <= i6; ic++) {
              ia++;
              C->data[ic] += H->data[loop_ub] * Hl->data[ia - 1];
            }
          }

          ar += m;
        }

        br += k;
        xp_idx_0 += m;
      }
    }
  }

  emxFree_real_T(&H);
  emxFree_real_T(&Hl);
  i5 = Hr->size[0] * Hr->size[1];
  emxEnsureCapacity((emxArray__common *)Hr, i5, (int)sizeof(double));
  xp_idx_0 = Hr->size[0];
  br = Hr->size[1];
  loop_ub = xp_idx_0 * br;
  for (i5 = 0; i5 < loop_ub; i5++) {
    Hr->data[i5] -= C->data[i5];
  }

  emxFree_real_T(&C);
  i5 = b->size[0] * b->size[1];
  b->size[0] = Pk->size[0];
  b->size[1] = Pk->size[1];
  emxEnsureCapacity((emxArray__common *)b, i5, (int)sizeof(double));
  loop_ub = Pk->size[0] * Pk->size[1];
  for (i5 = 0; i5 < loop_ub; i5++) {
    b->data[i5] = Pk->data[i5];
  }

  b_emxInit_real_T(&b_Hr, 2);
  if ((Hr->size[1] == 1) || (Pk->size[0] == 1)) {
    i5 = b_Hr->size[0] * b_Hr->size[1];
    b_Hr->size[0] = Hr->size[0];
    b_Hr->size[1] = Pk->size[1];
    emxEnsureCapacity((emxArray__common *)b_Hr, i5, (int)sizeof(double));
    loop_ub = Hr->size[0];
    for (i5 = 0; i5 < loop_ub; i5++) {
      ar = Pk->size[1];
      for (i6 = 0; i6 < ar; i6++) {
        b_Hr->data[i5 + b_Hr->size[0] * i6] = 0.0;
        xp_idx_0 = Hr->size[1];
        for (br = 0; br < xp_idx_0; br++) {
          b_Hr->data[i5 + b_Hr->size[0] * i6] += Hr->data[i5 + Hr->size[0] * br]
            * Pk->data[br + Pk->size[0] * i6];
        }
      }
    }

    i5 = Pk->size[0] * Pk->size[1];
    Pk->size[0] = b_Hr->size[0];
    Pk->size[1] = b_Hr->size[1];
    emxEnsureCapacity((emxArray__common *)Pk, i5, (int)sizeof(double));
    loop_ub = b_Hr->size[1];
    for (i5 = 0; i5 < loop_ub; i5++) {
      ar = b_Hr->size[0];
      for (i6 = 0; i6 < ar; i6++) {
        Pk->data[i6 + Pk->size[0] * i5] = b_Hr->data[i6 + b_Hr->size[0] * i5];
      }
    }
  } else {
    k = Hr->size[1];
    unusedU2[0] = (unsigned int)Hr->size[0];
    unusedU2[1] = (unsigned int)Pk->size[1];
    m = Hr->size[0];
    i5 = Pk->size[0] * Pk->size[1];
    Pk->size[0] = (int)unusedU2[0];
    Pk->size[1] = (int)unusedU2[1];
    emxEnsureCapacity((emxArray__common *)Pk, i5, (int)sizeof(double));
    loop_ub = (int)unusedU2[1];
    for (i5 = 0; i5 < loop_ub; i5++) {
      ar = (int)unusedU2[0];
      for (i6 = 0; i6 < ar; i6++) {
        Pk->data[i6 + Pk->size[0] * i5] = 0.0;
      }
    }

    if ((Hr->size[0] == 0) || (b->size[1] == 0)) {
    } else {
      c = Hr->size[0] * (b->size[1] - 1);
      xp_idx_0 = 0;
      while ((m > 0) && (xp_idx_0 <= c)) {
        i5 = xp_idx_0 + m;
        for (ic = xp_idx_0; ic + 1 <= i5; ic++) {
          Pk->data[ic] = 0.0;
        }

        xp_idx_0 += m;
      }

      br = 0;
      xp_idx_0 = 0;
      while ((m > 0) && (xp_idx_0 <= c)) {
        ar = 0;
        i5 = br + k;
        for (loop_ub = br; loop_ub + 1 <= i5; loop_ub++) {
          if (b->data[loop_ub] != 0.0) {
            ia = ar;
            i6 = xp_idx_0 + m;
            for (ic = xp_idx_0; ic + 1 <= i6; ic++) {
              ia++;
              Pk->data[ic] += b->data[loop_ub] * Hr->data[ia - 1];
            }
          }

          ar += m;
        }

        br += k;
        xp_idx_0 += m;
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
  for (xp_idx_0 = 2; xp_idx_0 - 2 <= i5 - 3; xp_idx_0++) {
    if ((-0.75 >= r->data[xp_idx_0]) || rtIsNaN(r->data[xp_idx_0])) {
      dist_point = -0.75;
    } else {
      dist_point = r->data[xp_idx_0];
    }

    r->data[xp_idx_0] = dist_point;
    if ((0.75 <= r->data[xp_idx_0]) || rtIsNaN(r->data[xp_idx_0])) {
      dist_point = 0.75;
    } else {
      dist_point = r->data[xp_idx_0];
    }

    r->data[xp_idx_0] = dist_point;
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
