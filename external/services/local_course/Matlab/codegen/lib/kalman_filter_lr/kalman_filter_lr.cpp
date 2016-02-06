//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: kalman_filter_lr.cpp
//
// MATLAB Coder version            : 3.0
// C/C++ source code generated on  : 29-Nov-2015 11:30:09
//

// Include Files
#include "rt_nonfinite.h"
#include "kalman_filter_lr.h"
#include "kalman_filter_lr_emxutil.h"
#include "inv.h"
#include "eye.h"
#include "mrdivide.h"
#include "state_transition.h"
#include "handle_measurements.h"

// Function Definitions

//
// "kalman_filter_lr"
//
//  ----Nomenklatur----
//   * r: Zustandsvektor = [y0, phi0, kappa_1, kappa_2, ... , kappa_(n-1)]
//   --> y0: y-Wert des ersten Punktes (x-Wert ist immer 0)
//   --> phi0: Anfangssteigung zwischen erstem und zweitem Punkt
//   --> kappa_i: Krümmungen an den entsprechenden Punkten (Krümmungsdefinition: 1/R,
//       wobei R der Radius des Kreises ist, auf dem der betrachtete Punkt, sein Vorgänger
//       und sein Nachfolger liegen)
//   * delta_x, delta_y, delta_phi: skalare Werte für die Eigenbewegung des Fahrzeugs seit dem letzten Aufruf des Filters (im KOSY des letzten Aufrufs);
//     ist delta_x = delta_y = delta_phi = 0, wird die Berechnung einer state-transition Matrix deaktiviert
//   * Pk: Kovarianzmatrix des Zustands dim[n x n], mit n = Dimension des Zustandsvektors
//   * Q: Kovarianzmatrix des Zustandsübergangs (Prozessrauschen) dim[n x n] (symmetrische Matrix, mit weg von der
//       Diagonalen abnehmenden Eintraegen -> wie stark hängen die Krümmungen an den verschiedenen Punkten zusammen?)
//   * R_fakt: Varianz der Messwerte (Messrauschen)
//   * delta: Abstand zwischen aufeinanderfolgenden Punkten (delta*n ergibt die Länge des praedizierten Fahrstreifens)
//   * xl, yl: Vektoren mit den Messwerten für die linke Spur
//   * xr, yr: Vektoren mit den Messwerten für die rechte Spur
//   * xm, ym: Vektoren mit den Messwerten für die Mittellinie
//   * interp_mode: 0 für lineare Interpolation der Krümmungen bei state transition, 1 für quadratische Interpolation
//
//   ----Grober Ablauf des Algorithmus----
//   1. Projektion der Punkte von der Mittellinie nach links bzw. rechts
//   2. Fuer jeden Messpunkt: Berechnung des kleinsten Abstands zum aktuell praedizierten Strassenverlauf
//   3. Assemblierung der Jakobimatrix fuer die Projektion aus dem Zustandsraum von r auf x-y-Koordinaten
//   4. Kalman-Filter anwenden
//   5. Zustandsbegrenzungen einbringen (mathematisch unschön aber praktisch funktionierts)
// Arguments    : emxArray_real_T *r
//                double delta_x
//                double delta_y
//                double delta_phi
//                emxArray_real_T *Pk
//                const emxArray_real_T *Q
//                double R_fakt
//                double delta
//                const emxArray_real_T *xl
//                const emxArray_real_T *yl
//                const emxArray_real_T *xr
//                const emxArray_real_T *yr
//                const emxArray_real_T *xm
//                const emxArray_real_T *ym
//                unsigned char interp_mode
//                double prior_fact
// Return Type  : void
//
void kalman_filter_lr(emxArray_real_T *r, double delta_x, double delta_y, double
                      delta_phi, emxArray_real_T *Pk, const emxArray_real_T *Q,
                      double R_fakt, double delta, const emxArray_real_T *xl,
                      const emxArray_real_T *yl, const emxArray_real_T *xr,
                      const emxArray_real_T *yr, const emxArray_real_T *xm,
                      const emxArray_real_T *ym, unsigned char interp_mode,
                      double prior_fact)
{
  emxArray_real_T *b_xl;
  int i5;
  int br;
  emxArray_real_T *b_yl;
  emxArray_real_T *A;
  emxArray_real_T *zl;
  emxArray_real_T *zml;
  emxArray_real_T *b_xr;
  double nPointsL;
  emxArray_real_T *b_yr;
  emxArray_real_T *R;
  emxArray_real_T *zr;
  emxArray_real_T *zmr;
  emxArray_real_T *b_xm;
  double nPointsR;
  emxArray_real_T *b_ym;
  emxArray_real_T *H;
  emxArray_real_T *Hm;
  emxArray_real_T *zm;
  emxArray_real_T *zmm;
  double nPointsM;
  int ar;
  int i6;
  int b_R;
  int cr;
  emxArray_real_T *y;
  int k;
  int m;
  int ic;
  int ib;
  int ia;
  emxArray_real_T *b_zl;
  emxArray_real_T *b_zml;
  emxArray_real_T *y_tilde;
  emxArray_real_T *b_y;
  emxArray_real_T *C;
  emxArray_real_T *K;
  emxArray_real_T *b_C;
  emxArray_real_T *c_C;
  unsigned int K_idx_0;
  emxArray_real_T *d_C;
  emxArray_real_T *b_A;
  emxArray_real_T *b_Pk;
  emxArray_real_T *c_y;
  emxArray_real_T *d_y;
  emxArray_real_T *e_y;
  int i;
  double minval;
  emxInit_real_T1(&b_xl, 1);

  // % Messpunkte verarbeiten
  //  linke Seitenlinie
  i5 = b_xl->size[0];
  b_xl->size[0] = xl->size[0];
  emxEnsureCapacity((emxArray__common *)b_xl, i5, (int)sizeof(double));
  br = xl->size[0];
  for (i5 = 0; i5 < br; i5++) {
    b_xl->data[i5] = xl->data[i5];
  }

  emxInit_real_T1(&b_yl, 1);
  i5 = b_yl->size[0];
  b_yl->size[0] = yl->size[0];
  emxEnsureCapacity((emxArray__common *)b_yl, i5, (int)sizeof(double));
  br = yl->size[0];
  for (i5 = 0; i5 < br; i5++) {
    b_yl->data[i5] = yl->data[i5];
  }

  emxInit_real_T(&A, 2);
  emxInit_real_T1(&zl, 1);
  emxInit_real_T1(&zml, 1);
  emxInit_real_T1(&b_xr, 1);
  handle_measurements(r, delta, b_xl, b_yl, A, zl, zml, &nPointsL);

  //  rechte Seitenlinie
  i5 = b_xr->size[0];
  b_xr->size[0] = xr->size[0];
  emxEnsureCapacity((emxArray__common *)b_xr, i5, (int)sizeof(double));
  br = xr->size[0];
  emxFree_real_T(&b_yl);
  emxFree_real_T(&b_xl);
  for (i5 = 0; i5 < br; i5++) {
    b_xr->data[i5] = xr->data[i5];
  }

  emxInit_real_T1(&b_yr, 1);
  i5 = b_yr->size[0];
  b_yr->size[0] = yr->size[0];
  emxEnsureCapacity((emxArray__common *)b_yr, i5, (int)sizeof(double));
  br = yr->size[0];
  for (i5 = 0; i5 < br; i5++) {
    b_yr->data[i5] = yr->data[i5];
  }

  emxInit_real_T(&R, 2);
  emxInit_real_T1(&zr, 1);
  emxInit_real_T1(&zmr, 1);
  emxInit_real_T1(&b_xm, 1);
  b_handle_measurements(r, delta, b_xr, b_yr, R, zr, zmr, &nPointsR);

  //  Mittellinie
  i5 = b_xm->size[0];
  b_xm->size[0] = xm->size[0];
  emxEnsureCapacity((emxArray__common *)b_xm, i5, (int)sizeof(double));
  br = xm->size[0];
  emxFree_real_T(&b_yr);
  emxFree_real_T(&b_xr);
  for (i5 = 0; i5 < br; i5++) {
    b_xm->data[i5] = xm->data[i5];
  }

  emxInit_real_T1(&b_ym, 1);
  i5 = b_ym->size[0];
  b_ym->size[0] = ym->size[0];
  emxEnsureCapacity((emxArray__common *)b_ym, i5, (int)sizeof(double));
  br = ym->size[0];
  for (i5 = 0; i5 < br; i5++) {
    b_ym->data[i5] = ym->data[i5];
  }

  emxInit_real_T(&H, 2);
  emxInit_real_T(&Hm, 2);
  emxInit_real_T1(&zm, 1);
  emxInit_real_T1(&zmm, 1);
  c_handle_measurements(r, delta, b_xm, b_ym, Hm, zm, zmm, &nPointsM);

  //  Linien in eine Matrix kombinieren
  i5 = H->size[0] * H->size[1];
  H->size[0] = (A->size[0] + R->size[0]) + Hm->size[0];
  H->size[1] = A->size[1];
  emxEnsureCapacity((emxArray__common *)H, i5, (int)sizeof(double));
  br = A->size[1];
  emxFree_real_T(&b_ym);
  emxFree_real_T(&b_xm);
  for (i5 = 0; i5 < br; i5++) {
    ar = A->size[0];
    for (i6 = 0; i6 < ar; i6++) {
      H->data[i6 + H->size[0] * i5] = A->data[i6 + A->size[0] * i5];
    }
  }

  br = R->size[1];
  for (i5 = 0; i5 < br; i5++) {
    ar = R->size[0];
    for (i6 = 0; i6 < ar; i6++) {
      H->data[(i6 + A->size[0]) + H->size[0] * i5] = R->data[i6 + R->size[0] *
        i5];
    }
  }

  br = Hm->size[1];
  for (i5 = 0; i5 < br; i5++) {
    ar = Hm->size[0];
    for (i6 = 0; i6 < ar; i6++) {
      H->data[((i6 + A->size[0]) + R->size[0]) + H->size[0] * i5] = Hm->data[i6
        + Hm->size[0] * i5];
    }
  }

  // Messmatrix
  // Messwerte
  // erwartete Werte
  // % KALMAN FILTER
  //  state transition
  if ((delta_x == 0.0) && (delta_y == 0.0) && (delta_phi == 0.0)) {
    eye((double)r->size[0], A);
  } else {
    state_transition(r, delta, delta_x, delta_y, delta_phi, interp_mode, A);
  }

  eye(2.0 * ((nPointsL + nPointsR) + nPointsM), R);
  i5 = R->size[0] * R->size[1];
  emxEnsureCapacity((emxArray__common *)R, i5, (int)sizeof(double));
  b_R = R->size[0];
  cr = R->size[1];
  br = b_R * cr;
  for (i5 = 0; i5 < br; i5++) {
    R->data[i5] *= R_fakt;
  }

  //  Filtergleichungen EKF
  emxInit_real_T(&y, 2);
  if ((A->size[1] == 1) || (Pk->size[0] == 1)) {
    i5 = y->size[0] * y->size[1];
    y->size[0] = A->size[0];
    y->size[1] = Pk->size[1];
    emxEnsureCapacity((emxArray__common *)y, i5, (int)sizeof(double));
    br = A->size[0];
    for (i5 = 0; i5 < br; i5++) {
      ar = Pk->size[1];
      for (i6 = 0; i6 < ar; i6++) {
        y->data[i5 + y->size[0] * i6] = 0.0;
        b_R = A->size[1];
        for (cr = 0; cr < b_R; cr++) {
          y->data[i5 + y->size[0] * i6] += A->data[i5 + A->size[0] * cr] *
            Pk->data[cr + Pk->size[0] * i6];
        }
      }
    }
  } else {
    k = A->size[1];
    b_R = A->size[0];
    cr = Pk->size[1];
    i5 = y->size[0] * y->size[1];
    y->size[0] = b_R;
    y->size[1] = cr;
    emxEnsureCapacity((emxArray__common *)y, i5, (int)sizeof(double));
    m = A->size[0];
    i5 = y->size[0] * y->size[1];
    emxEnsureCapacity((emxArray__common *)y, i5, (int)sizeof(double));
    br = y->size[1];
    for (i5 = 0; i5 < br; i5++) {
      ar = y->size[0];
      for (i6 = 0; i6 < ar; i6++) {
        y->data[i6 + y->size[0] * i5] = 0.0;
      }
    }

    if ((A->size[0] == 0) || (Pk->size[1] == 0)) {
    } else {
      b_R = A->size[0] * (Pk->size[1] - 1);
      cr = 0;
      while ((m > 0) && (cr <= b_R)) {
        i5 = cr + m;
        for (ic = cr; ic + 1 <= i5; ic++) {
          y->data[ic] = 0.0;
        }

        cr += m;
      }

      br = 0;
      cr = 0;
      while ((m > 0) && (cr <= b_R)) {
        ar = -1;
        i5 = br + k;
        for (ib = br; ib + 1 <= i5; ib++) {
          if (Pk->data[ib] != 0.0) {
            ia = ar;
            i6 = cr + m;
            for (ic = cr; ic + 1 <= i6; ic++) {
              ia++;
              y->data[ic] += Pk->data[ib] * A->data[ia];
            }
          }

          ar += m;
        }

        br += k;
        cr += m;
      }
    }
  }

  i5 = Hm->size[0] * Hm->size[1];
  Hm->size[0] = A->size[1];
  Hm->size[1] = A->size[0];
  emxEnsureCapacity((emxArray__common *)Hm, i5, (int)sizeof(double));
  br = A->size[0];
  for (i5 = 0; i5 < br; i5++) {
    ar = A->size[1];
    for (i6 = 0; i6 < ar; i6++) {
      Hm->data[i6 + Hm->size[0] * i5] = A->data[i5 + A->size[0] * i6];
    }
  }

  if ((y->size[1] == 1) || (Hm->size[0] == 1)) {
    i5 = Pk->size[0] * Pk->size[1];
    Pk->size[0] = y->size[0];
    Pk->size[1] = Hm->size[1];
    emxEnsureCapacity((emxArray__common *)Pk, i5, (int)sizeof(double));
    br = y->size[0];
    for (i5 = 0; i5 < br; i5++) {
      ar = Hm->size[1];
      for (i6 = 0; i6 < ar; i6++) {
        Pk->data[i5 + Pk->size[0] * i6] = 0.0;
        b_R = y->size[1];
        for (cr = 0; cr < b_R; cr++) {
          Pk->data[i5 + Pk->size[0] * i6] += y->data[i5 + y->size[0] * cr] *
            Hm->data[cr + Hm->size[0] * i6];
        }
      }
    }
  } else {
    k = y->size[1];
    b_R = y->size[0];
    cr = Hm->size[1];
    i5 = Pk->size[0] * Pk->size[1];
    Pk->size[0] = b_R;
    Pk->size[1] = cr;
    emxEnsureCapacity((emxArray__common *)Pk, i5, (int)sizeof(double));
    m = y->size[0];
    i5 = Pk->size[0] * Pk->size[1];
    emxEnsureCapacity((emxArray__common *)Pk, i5, (int)sizeof(double));
    br = Pk->size[1];
    for (i5 = 0; i5 < br; i5++) {
      ar = Pk->size[0];
      for (i6 = 0; i6 < ar; i6++) {
        Pk->data[i6 + Pk->size[0] * i5] = 0.0;
      }
    }

    if ((y->size[0] == 0) || (Hm->size[1] == 0)) {
    } else {
      b_R = y->size[0] * (Hm->size[1] - 1);
      cr = 0;
      while ((m > 0) && (cr <= b_R)) {
        i5 = cr + m;
        for (ic = cr; ic + 1 <= i5; ic++) {
          Pk->data[ic] = 0.0;
        }

        cr += m;
      }

      br = 0;
      cr = 0;
      while ((m > 0) && (cr <= b_R)) {
        ar = -1;
        i5 = br + k;
        for (ib = br; ib + 1 <= i5; ib++) {
          if (Hm->data[ib] != 0.0) {
            ia = ar;
            i6 = cr + m;
            for (ic = cr; ic + 1 <= i6; ic++) {
              ia++;
              Pk->data[ic] += Hm->data[ib] * y->data[ia];
            }
          }

          ar += m;
        }

        br += k;
        cr += m;
      }
    }
  }

  i5 = Pk->size[0] * Pk->size[1];
  emxEnsureCapacity((emxArray__common *)Pk, i5, (int)sizeof(double));
  br = Pk->size[1];
  for (i5 = 0; i5 < br; i5++) {
    ar = Pk->size[0];
    for (i6 = 0; i6 < ar; i6++) {
      Pk->data[i6 + Pk->size[0] * i5] += Q->data[i6 + Q->size[0] * i5];
    }
  }

  emxInit_real_T1(&b_zl, 1);
  emxInit_real_T1(&b_zml, 1);
  i5 = b_zl->size[0];
  b_zl->size[0] = (zl->size[0] + zr->size[0]) + zm->size[0];
  emxEnsureCapacity((emxArray__common *)b_zl, i5, (int)sizeof(double));
  br = zl->size[0];
  for (i5 = 0; i5 < br; i5++) {
    b_zl->data[i5] = zl->data[i5];
  }

  br = zr->size[0];
  for (i5 = 0; i5 < br; i5++) {
    b_zl->data[i5 + zl->size[0]] = zr->data[i5];
  }

  br = zm->size[0];
  for (i5 = 0; i5 < br; i5++) {
    b_zl->data[(i5 + zl->size[0]) + zr->size[0]] = zm->data[i5];
  }

  emxFree_real_T(&zm);
  emxFree_real_T(&zr);
  i5 = b_zml->size[0];
  b_zml->size[0] = (zml->size[0] + zmr->size[0]) + zmm->size[0];
  emxEnsureCapacity((emxArray__common *)b_zml, i5, (int)sizeof(double));
  br = zml->size[0];
  for (i5 = 0; i5 < br; i5++) {
    b_zml->data[i5] = zml->data[i5];
  }

  br = zmr->size[0];
  for (i5 = 0; i5 < br; i5++) {
    b_zml->data[i5 + zml->size[0]] = zmr->data[i5];
  }

  br = zmm->size[0];
  for (i5 = 0; i5 < br; i5++) {
    b_zml->data[(i5 + zml->size[0]) + zmr->size[0]] = zmm->data[i5];
  }

  emxFree_real_T(&zmm);
  emxFree_real_T(&zmr);
  emxFree_real_T(&zml);
  emxInit_real_T1(&y_tilde, 1);
  i5 = y_tilde->size[0];
  y_tilde->size[0] = b_zl->size[0];
  emxEnsureCapacity((emxArray__common *)y_tilde, i5, (int)sizeof(double));
  br = b_zl->size[0];
  for (i5 = 0; i5 < br; i5++) {
    y_tilde->data[i5] = b_zl->data[i5] - b_zml->data[i5];
  }

  emxFree_real_T(&b_zml);
  emxFree_real_T(&b_zl);
  emxInit_real_T(&b_y, 2);
  if ((H->size[1] == 1) || (Pk->size[0] == 1)) {
    i5 = b_y->size[0] * b_y->size[1];
    b_y->size[0] = H->size[0];
    b_y->size[1] = Pk->size[1];
    emxEnsureCapacity((emxArray__common *)b_y, i5, (int)sizeof(double));
    br = H->size[0];
    for (i5 = 0; i5 < br; i5++) {
      ar = Pk->size[1];
      for (i6 = 0; i6 < ar; i6++) {
        b_y->data[i5 + b_y->size[0] * i6] = 0.0;
        b_R = H->size[1];
        for (cr = 0; cr < b_R; cr++) {
          b_y->data[i5 + b_y->size[0] * i6] += H->data[i5 + H->size[0] * cr] *
            Pk->data[cr + Pk->size[0] * i6];
        }
      }
    }
  } else {
    k = H->size[1];
    b_R = H->size[0];
    cr = Pk->size[1];
    i5 = b_y->size[0] * b_y->size[1];
    b_y->size[0] = b_R;
    b_y->size[1] = cr;
    emxEnsureCapacity((emxArray__common *)b_y, i5, (int)sizeof(double));
    m = H->size[0];
    i5 = b_y->size[0] * b_y->size[1];
    emxEnsureCapacity((emxArray__common *)b_y, i5, (int)sizeof(double));
    br = b_y->size[1];
    for (i5 = 0; i5 < br; i5++) {
      ar = b_y->size[0];
      for (i6 = 0; i6 < ar; i6++) {
        b_y->data[i6 + b_y->size[0] * i5] = 0.0;
      }
    }

    if ((H->size[0] == 0) || (Pk->size[1] == 0)) {
    } else {
      b_R = H->size[0] * (Pk->size[1] - 1);
      cr = 0;
      while ((m > 0) && (cr <= b_R)) {
        i5 = cr + m;
        for (ic = cr; ic + 1 <= i5; ic++) {
          b_y->data[ic] = 0.0;
        }

        cr += m;
      }

      br = 0;
      cr = 0;
      while ((m > 0) && (cr <= b_R)) {
        ar = -1;
        i5 = br + k;
        for (ib = br; ib + 1 <= i5; ib++) {
          if (Pk->data[ib] != 0.0) {
            ia = ar;
            i6 = cr + m;
            for (ic = cr; ic + 1 <= i6; ic++) {
              ia++;
              b_y->data[ic] += Pk->data[ib] * H->data[ia];
            }
          }

          ar += m;
        }

        br += k;
        cr += m;
      }
    }
  }

  i5 = Hm->size[0] * Hm->size[1];
  Hm->size[0] = H->size[1];
  Hm->size[1] = H->size[0];
  emxEnsureCapacity((emxArray__common *)Hm, i5, (int)sizeof(double));
  br = H->size[0];
  for (i5 = 0; i5 < br; i5++) {
    ar = H->size[1];
    for (i6 = 0; i6 < ar; i6++) {
      Hm->data[i6 + Hm->size[0] * i5] = H->data[i5 + H->size[0] * i6];
    }
  }

  emxInit_real_T(&C, 2);
  if ((b_y->size[1] == 1) || (Hm->size[0] == 1)) {
    i5 = C->size[0] * C->size[1];
    C->size[0] = b_y->size[0];
    C->size[1] = Hm->size[1];
    emxEnsureCapacity((emxArray__common *)C, i5, (int)sizeof(double));
    br = b_y->size[0];
    for (i5 = 0; i5 < br; i5++) {
      ar = Hm->size[1];
      for (i6 = 0; i6 < ar; i6++) {
        C->data[i5 + C->size[0] * i6] = 0.0;
        b_R = b_y->size[1];
        for (cr = 0; cr < b_R; cr++) {
          C->data[i5 + C->size[0] * i6] += b_y->data[i5 + b_y->size[0] * cr] *
            Hm->data[cr + Hm->size[0] * i6];
        }
      }
    }
  } else {
    k = b_y->size[1];
    b_R = b_y->size[0];
    cr = Hm->size[1];
    i5 = C->size[0] * C->size[1];
    C->size[0] = b_R;
    C->size[1] = cr;
    emxEnsureCapacity((emxArray__common *)C, i5, (int)sizeof(double));
    m = b_y->size[0];
    i5 = C->size[0] * C->size[1];
    emxEnsureCapacity((emxArray__common *)C, i5, (int)sizeof(double));
    br = C->size[1];
    for (i5 = 0; i5 < br; i5++) {
      ar = C->size[0];
      for (i6 = 0; i6 < ar; i6++) {
        C->data[i6 + C->size[0] * i5] = 0.0;
      }
    }

    if ((b_y->size[0] == 0) || (Hm->size[1] == 0)) {
    } else {
      b_R = b_y->size[0] * (Hm->size[1] - 1);
      cr = 0;
      while ((m > 0) && (cr <= b_R)) {
        i5 = cr + m;
        for (ic = cr; ic + 1 <= i5; ic++) {
          C->data[ic] = 0.0;
        }

        cr += m;
      }

      br = 0;
      cr = 0;
      while ((m > 0) && (cr <= b_R)) {
        ar = -1;
        i5 = br + k;
        for (ib = br; ib + 1 <= i5; ib++) {
          if (Hm->data[ib] != 0.0) {
            ia = ar;
            i6 = cr + m;
            for (ic = cr; ic + 1 <= i6; ic++) {
              ia++;
              C->data[ic] += Hm->data[ib] * b_y->data[ia];
            }
          }

          ar += m;
        }

        br += k;
        cr += m;
      }
    }
  }

  emxFree_real_T(&b_y);
  i5 = Hm->size[0] * Hm->size[1];
  Hm->size[0] = H->size[1];
  Hm->size[1] = H->size[0];
  emxEnsureCapacity((emxArray__common *)Hm, i5, (int)sizeof(double));
  br = H->size[0];
  for (i5 = 0; i5 < br; i5++) {
    ar = H->size[1];
    for (i6 = 0; i6 < ar; i6++) {
      Hm->data[i6 + Hm->size[0] * i5] = H->data[i5 + H->size[0] * i6];
    }
  }

  emxInit_real_T(&K, 2);
  if ((Pk->size[1] == 1) || (Hm->size[0] == 1)) {
    i5 = K->size[0] * K->size[1];
    K->size[0] = Pk->size[0];
    K->size[1] = Hm->size[1];
    emxEnsureCapacity((emxArray__common *)K, i5, (int)sizeof(double));
    br = Pk->size[0];
    for (i5 = 0; i5 < br; i5++) {
      ar = Hm->size[1];
      for (i6 = 0; i6 < ar; i6++) {
        K->data[i5 + K->size[0] * i6] = 0.0;
        b_R = Pk->size[1];
        for (cr = 0; cr < b_R; cr++) {
          K->data[i5 + K->size[0] * i6] += Pk->data[i5 + Pk->size[0] * cr] *
            Hm->data[cr + Hm->size[0] * i6];
        }
      }
    }
  } else {
    k = Pk->size[1];
    b_R = Pk->size[0];
    cr = Hm->size[1];
    i5 = K->size[0] * K->size[1];
    K->size[0] = b_R;
    K->size[1] = cr;
    emxEnsureCapacity((emxArray__common *)K, i5, (int)sizeof(double));
    m = Pk->size[0];
    i5 = K->size[0] * K->size[1];
    emxEnsureCapacity((emxArray__common *)K, i5, (int)sizeof(double));
    br = K->size[1];
    for (i5 = 0; i5 < br; i5++) {
      ar = K->size[0];
      for (i6 = 0; i6 < ar; i6++) {
        K->data[i6 + K->size[0] * i5] = 0.0;
      }
    }

    if ((Pk->size[0] == 0) || (Hm->size[1] == 0)) {
    } else {
      b_R = Pk->size[0] * (Hm->size[1] - 1);
      cr = 0;
      while ((m > 0) && (cr <= b_R)) {
        i5 = cr + m;
        for (ic = cr; ic + 1 <= i5; ic++) {
          K->data[ic] = 0.0;
        }

        cr += m;
      }

      br = 0;
      cr = 0;
      while ((m > 0) && (cr <= b_R)) {
        ar = -1;
        i5 = br + k;
        for (ib = br; ib + 1 <= i5; ib++) {
          if (Hm->data[ib] != 0.0) {
            ia = ar;
            i6 = cr + m;
            for (ic = cr; ic + 1 <= i6; ic++) {
              ia++;
              K->data[ic] += Hm->data[ib] * Pk->data[ia];
            }
          }

          ar += m;
        }

        br += k;
        cr += m;
      }
    }
  }

  emxInit_real_T(&b_C, 2);
  i5 = b_C->size[0] * b_C->size[1];
  b_C->size[0] = C->size[0];
  b_C->size[1] = C->size[1];
  emxEnsureCapacity((emxArray__common *)b_C, i5, (int)sizeof(double));
  br = C->size[0] * C->size[1];
  for (i5 = 0; i5 < br; i5++) {
    b_C->data[i5] = C->data[i5] + R->data[i5];
  }

  emxFree_real_T(&C);
  emxFree_real_T(&R);
  mrdivide(K, b_C);
  emxFree_real_T(&b_C);
  emxInit_real_T1(&c_C, 1);
  if ((K->size[1] == 1) || (y_tilde->size[0] == 1)) {
    i5 = c_C->size[0];
    c_C->size[0] = K->size[0];
    emxEnsureCapacity((emxArray__common *)c_C, i5, (int)sizeof(double));
    br = K->size[0];
    for (i5 = 0; i5 < br; i5++) {
      c_C->data[i5] = 0.0;
      ar = K->size[1];
      for (i6 = 0; i6 < ar; i6++) {
        c_C->data[i5] += K->data[i5 + K->size[0] * i6] * y_tilde->data[i6];
      }
    }
  } else {
    k = K->size[1];
    K_idx_0 = (unsigned int)K->size[0];
    i5 = c_C->size[0];
    c_C->size[0] = (int)K_idx_0;
    emxEnsureCapacity((emxArray__common *)c_C, i5, (int)sizeof(double));
    m = K->size[0];
    b_R = c_C->size[0];
    i5 = c_C->size[0];
    c_C->size[0] = b_R;
    emxEnsureCapacity((emxArray__common *)c_C, i5, (int)sizeof(double));
    for (i5 = 0; i5 < b_R; i5++) {
      c_C->data[i5] = 0.0;
    }

    if (K->size[0] == 0) {
    } else {
      cr = 0;
      while ((m > 0) && (cr <= 0)) {
        for (ic = 1; ic <= m; ic++) {
          c_C->data[ic - 1] = 0.0;
        }

        cr = m;
      }

      br = 0;
      cr = 0;
      while ((m > 0) && (cr <= 0)) {
        ar = -1;
        i5 = br + k;
        for (ib = br; ib + 1 <= i5; ib++) {
          if (y_tilde->data[ib] != 0.0) {
            ia = ar;
            for (ic = 0; ic + 1 <= m; ic++) {
              ia++;
              c_C->data[ic] += y_tilde->data[ib] * K->data[ia];
            }
          }

          ar += m;
        }

        br += k;
        cr = m;
      }
    }
  }

  emxFree_real_T(&y_tilde);
  i5 = r->size[0];
  emxEnsureCapacity((emxArray__common *)r, i5, (int)sizeof(double));
  br = r->size[0];
  for (i5 = 0; i5 < br; i5++) {
    r->data[i5] += c_C->data[i5];
  }

  emxFree_real_T(&c_C);
  eye((double)r->size[0], A);
  emxInit_real_T(&d_C, 2);
  if ((K->size[1] == 1) || (H->size[0] == 1)) {
    i5 = d_C->size[0] * d_C->size[1];
    d_C->size[0] = K->size[0];
    d_C->size[1] = H->size[1];
    emxEnsureCapacity((emxArray__common *)d_C, i5, (int)sizeof(double));
    br = K->size[0];
    for (i5 = 0; i5 < br; i5++) {
      ar = H->size[1];
      for (i6 = 0; i6 < ar; i6++) {
        d_C->data[i5 + d_C->size[0] * i6] = 0.0;
        b_R = K->size[1];
        for (cr = 0; cr < b_R; cr++) {
          d_C->data[i5 + d_C->size[0] * i6] += K->data[i5 + K->size[0] * cr] *
            H->data[cr + H->size[0] * i6];
        }
      }
    }
  } else {
    k = K->size[1];
    b_R = K->size[0];
    cr = H->size[1];
    i5 = d_C->size[0] * d_C->size[1];
    d_C->size[0] = b_R;
    d_C->size[1] = cr;
    emxEnsureCapacity((emxArray__common *)d_C, i5, (int)sizeof(double));
    m = K->size[0];
    i5 = d_C->size[0] * d_C->size[1];
    emxEnsureCapacity((emxArray__common *)d_C, i5, (int)sizeof(double));
    br = d_C->size[1];
    for (i5 = 0; i5 < br; i5++) {
      ar = d_C->size[0];
      for (i6 = 0; i6 < ar; i6++) {
        d_C->data[i6 + d_C->size[0] * i5] = 0.0;
      }
    }

    if ((K->size[0] == 0) || (H->size[1] == 0)) {
    } else {
      b_R = K->size[0] * (H->size[1] - 1);
      cr = 0;
      while ((m > 0) && (cr <= b_R)) {
        i5 = cr + m;
        for (ic = cr; ic + 1 <= i5; ic++) {
          d_C->data[ic] = 0.0;
        }

        cr += m;
      }

      br = 0;
      cr = 0;
      while ((m > 0) && (cr <= b_R)) {
        ar = -1;
        i5 = br + k;
        for (ib = br; ib + 1 <= i5; ib++) {
          if (H->data[ib] != 0.0) {
            ia = ar;
            i6 = cr + m;
            for (ic = cr; ic + 1 <= i6; ic++) {
              ia++;
              d_C->data[ic] += H->data[ib] * K->data[ia];
            }
          }

          ar += m;
        }

        br += k;
        cr += m;
      }
    }
  }

  emxFree_real_T(&K);
  emxFree_real_T(&H);
  i5 = A->size[0] * A->size[1];
  emxEnsureCapacity((emxArray__common *)A, i5, (int)sizeof(double));
  b_R = A->size[0];
  cr = A->size[1];
  br = b_R * cr;
  for (i5 = 0; i5 < br; i5++) {
    A->data[i5] -= d_C->data[i5];
  }

  emxFree_real_T(&d_C);
  i5 = Hm->size[0] * Hm->size[1];
  Hm->size[0] = Pk->size[0];
  Hm->size[1] = Pk->size[1];
  emxEnsureCapacity((emxArray__common *)Hm, i5, (int)sizeof(double));
  br = Pk->size[0] * Pk->size[1];
  for (i5 = 0; i5 < br; i5++) {
    Hm->data[i5] = Pk->data[i5];
  }

  emxInit_real_T(&b_A, 2);
  if ((A->size[1] == 1) || (Pk->size[0] == 1)) {
    i5 = b_A->size[0] * b_A->size[1];
    b_A->size[0] = A->size[0];
    b_A->size[1] = Pk->size[1];
    emxEnsureCapacity((emxArray__common *)b_A, i5, (int)sizeof(double));
    br = A->size[0];
    for (i5 = 0; i5 < br; i5++) {
      ar = Pk->size[1];
      for (i6 = 0; i6 < ar; i6++) {
        b_A->data[i5 + b_A->size[0] * i6] = 0.0;
        b_R = A->size[1];
        for (cr = 0; cr < b_R; cr++) {
          b_A->data[i5 + b_A->size[0] * i6] += A->data[i5 + A->size[0] * cr] *
            Pk->data[cr + Pk->size[0] * i6];
        }
      }
    }

    i5 = Pk->size[0] * Pk->size[1];
    Pk->size[0] = b_A->size[0];
    Pk->size[1] = b_A->size[1];
    emxEnsureCapacity((emxArray__common *)Pk, i5, (int)sizeof(double));
    br = b_A->size[1];
    for (i5 = 0; i5 < br; i5++) {
      ar = b_A->size[0];
      for (i6 = 0; i6 < ar; i6++) {
        Pk->data[i6 + Pk->size[0] * i5] = b_A->data[i6 + b_A->size[0] * i5];
      }
    }
  } else {
    k = A->size[1];
    b_R = A->size[0];
    cr = Pk->size[1];
    i5 = Pk->size[0] * Pk->size[1];
    Pk->size[0] = b_R;
    Pk->size[1] = cr;
    emxEnsureCapacity((emxArray__common *)Pk, i5, (int)sizeof(double));
    m = A->size[0];
    i5 = Pk->size[0] * Pk->size[1];
    emxEnsureCapacity((emxArray__common *)Pk, i5, (int)sizeof(double));
    br = Pk->size[1];
    for (i5 = 0; i5 < br; i5++) {
      ar = Pk->size[0];
      for (i6 = 0; i6 < ar; i6++) {
        Pk->data[i6 + Pk->size[0] * i5] = 0.0;
      }
    }

    if ((A->size[0] == 0) || (Hm->size[1] == 0)) {
    } else {
      b_R = A->size[0] * (Hm->size[1] - 1);
      cr = 0;
      while ((m > 0) && (cr <= b_R)) {
        i5 = cr + m;
        for (ic = cr; ic + 1 <= i5; ic++) {
          Pk->data[ic] = 0.0;
        }

        cr += m;
      }

      br = 0;
      cr = 0;
      while ((m > 0) && (cr <= b_R)) {
        ar = -1;
        i5 = br + k;
        for (ib = br; ib + 1 <= i5; ib++) {
          if (Hm->data[ib] != 0.0) {
            ia = ar;
            i6 = cr + m;
            for (ic = cr; ic + 1 <= i6; ic++) {
              ia++;
              Pk->data[ic] += Hm->data[ib] * A->data[ia];
            }
          }

          ar += m;
        }

        br += k;
        cr += m;
      }
    }
  }

  emxFree_real_T(&b_A);
  emxFree_real_T(&Hm);

  //  Lukas: zusätzlicher prior
  if (prior_fact > 0.0) {
    emxInit_real_T(&b_Pk, 2);
    i5 = b_Pk->size[0] * b_Pk->size[1];
    b_Pk->size[0] = Pk->size[0];
    b_Pk->size[1] = Pk->size[1];
    emxEnsureCapacity((emxArray__common *)b_Pk, i5, (int)sizeof(double));
    br = Pk->size[0] * Pk->size[1];
    for (i5 = 0; i5 < br; i5++) {
      b_Pk->data[i5] = Pk->data[i5] + prior_fact * Q->data[i5];
    }

    inv(b_Pk, A);
    emxFree_real_T(&b_Pk);
    emxInit_real_T(&c_y, 2);
    if ((Pk->size[1] == 1) || (A->size[0] == 1)) {
      i5 = c_y->size[0] * c_y->size[1];
      c_y->size[0] = Pk->size[0];
      c_y->size[1] = A->size[1];
      emxEnsureCapacity((emxArray__common *)c_y, i5, (int)sizeof(double));
      br = Pk->size[0];
      for (i5 = 0; i5 < br; i5++) {
        ar = A->size[1];
        for (i6 = 0; i6 < ar; i6++) {
          c_y->data[i5 + c_y->size[0] * i6] = 0.0;
          b_R = Pk->size[1];
          for (cr = 0; cr < b_R; cr++) {
            c_y->data[i5 + c_y->size[0] * i6] += Pk->data[i5 + Pk->size[0] * cr]
              * A->data[cr + A->size[0] * i6];
          }
        }
      }
    } else {
      k = Pk->size[1];
      b_R = Pk->size[0];
      cr = A->size[1];
      i5 = c_y->size[0] * c_y->size[1];
      c_y->size[0] = b_R;
      c_y->size[1] = cr;
      emxEnsureCapacity((emxArray__common *)c_y, i5, (int)sizeof(double));
      m = Pk->size[0];
      i5 = c_y->size[0] * c_y->size[1];
      emxEnsureCapacity((emxArray__common *)c_y, i5, (int)sizeof(double));
      br = c_y->size[1];
      for (i5 = 0; i5 < br; i5++) {
        ar = c_y->size[0];
        for (i6 = 0; i6 < ar; i6++) {
          c_y->data[i6 + c_y->size[0] * i5] = 0.0;
        }
      }

      if ((Pk->size[0] == 0) || (A->size[1] == 0)) {
      } else {
        b_R = Pk->size[0] * (A->size[1] - 1);
        cr = 0;
        while ((m > 0) && (cr <= b_R)) {
          i5 = cr + m;
          for (ic = cr; ic + 1 <= i5; ic++) {
            c_y->data[ic] = 0.0;
          }

          cr += m;
        }

        br = 0;
        cr = 0;
        while ((m > 0) && (cr <= b_R)) {
          ar = -1;
          i5 = br + k;
          for (ib = br; ib + 1 <= i5; ib++) {
            if (A->data[ib] != 0.0) {
              ia = ar;
              i6 = cr + m;
              for (ic = cr; ic + 1 <= i6; ic++) {
                ia++;
                c_y->data[ic] += A->data[ib] * Pk->data[ia];
              }
            }

            ar += m;
          }

          br += k;
          cr += m;
        }
      }
    }

    i5 = y->size[0] * y->size[1];
    y->size[0] = Q->size[0];
    y->size[1] = Q->size[1];
    emxEnsureCapacity((emxArray__common *)y, i5, (int)sizeof(double));
    br = Q->size[0] * Q->size[1];
    for (i5 = 0; i5 < br; i5++) {
      y->data[i5] = prior_fact * Q->data[i5];
    }

    if ((c_y->size[1] == 1) || (y->size[0] == 1)) {
      i5 = Pk->size[0] * Pk->size[1];
      Pk->size[0] = c_y->size[0];
      Pk->size[1] = y->size[1];
      emxEnsureCapacity((emxArray__common *)Pk, i5, (int)sizeof(double));
      br = c_y->size[0];
      for (i5 = 0; i5 < br; i5++) {
        ar = y->size[1];
        for (i6 = 0; i6 < ar; i6++) {
          Pk->data[i5 + Pk->size[0] * i6] = 0.0;
          b_R = c_y->size[1];
          for (cr = 0; cr < b_R; cr++) {
            Pk->data[i5 + Pk->size[0] * i6] += c_y->data[i5 + c_y->size[0] * cr]
              * y->data[cr + y->size[0] * i6];
          }
        }
      }
    } else {
      k = c_y->size[1];
      b_R = c_y->size[0];
      cr = y->size[1];
      i5 = Pk->size[0] * Pk->size[1];
      Pk->size[0] = b_R;
      Pk->size[1] = cr;
      emxEnsureCapacity((emxArray__common *)Pk, i5, (int)sizeof(double));
      m = c_y->size[0];
      i5 = Pk->size[0] * Pk->size[1];
      emxEnsureCapacity((emxArray__common *)Pk, i5, (int)sizeof(double));
      br = Pk->size[1];
      for (i5 = 0; i5 < br; i5++) {
        ar = Pk->size[0];
        for (i6 = 0; i6 < ar; i6++) {
          Pk->data[i6 + Pk->size[0] * i5] = 0.0;
        }
      }

      if ((c_y->size[0] == 0) || (y->size[1] == 0)) {
      } else {
        b_R = c_y->size[0] * (y->size[1] - 1);
        cr = 0;
        while ((m > 0) && (cr <= b_R)) {
          i5 = cr + m;
          for (ic = cr; ic + 1 <= i5; ic++) {
            Pk->data[ic] = 0.0;
          }

          cr += m;
        }

        br = 0;
        cr = 0;
        while ((m > 0) && (cr <= b_R)) {
          ar = -1;
          i5 = br + k;
          for (ib = br; ib + 1 <= i5; ib++) {
            if (y->data[ib] != 0.0) {
              ia = ar;
              i6 = cr + m;
              for (ic = cr; ic + 1 <= i6; ic++) {
                ia++;
                Pk->data[ic] += y->data[ib] * c_y->data[ia];
              }
            }

            ar += m;
          }

          br += k;
          cr += m;
        }
      }
    }

    emxFree_real_T(&c_y);
    i5 = y->size[0] * y->size[1];
    y->size[0] = Q->size[0];
    y->size[1] = Q->size[1];
    emxEnsureCapacity((emxArray__common *)y, i5, (int)sizeof(double));
    br = Q->size[0] * Q->size[1];
    for (i5 = 0; i5 < br; i5++) {
      y->data[i5] = prior_fact * Q->data[i5];
    }

    emxInit_real_T(&d_y, 2);
    if ((y->size[1] == 1) || (A->size[0] == 1)) {
      i5 = d_y->size[0] * d_y->size[1];
      d_y->size[0] = y->size[0];
      d_y->size[1] = A->size[1];
      emxEnsureCapacity((emxArray__common *)d_y, i5, (int)sizeof(double));
      br = y->size[0];
      for (i5 = 0; i5 < br; i5++) {
        ar = A->size[1];
        for (i6 = 0; i6 < ar; i6++) {
          d_y->data[i5 + d_y->size[0] * i6] = 0.0;
          b_R = y->size[1];
          for (cr = 0; cr < b_R; cr++) {
            d_y->data[i5 + d_y->size[0] * i6] += y->data[i5 + y->size[0] * cr] *
              A->data[cr + A->size[0] * i6];
          }
        }
      }
    } else {
      k = y->size[1];
      b_R = y->size[0];
      cr = A->size[1];
      i5 = d_y->size[0] * d_y->size[1];
      d_y->size[0] = b_R;
      d_y->size[1] = cr;
      emxEnsureCapacity((emxArray__common *)d_y, i5, (int)sizeof(double));
      m = y->size[0];
      i5 = d_y->size[0] * d_y->size[1];
      emxEnsureCapacity((emxArray__common *)d_y, i5, (int)sizeof(double));
      br = d_y->size[1];
      for (i5 = 0; i5 < br; i5++) {
        ar = d_y->size[0];
        for (i6 = 0; i6 < ar; i6++) {
          d_y->data[i6 + d_y->size[0] * i5] = 0.0;
        }
      }

      if ((y->size[0] == 0) || (A->size[1] == 0)) {
      } else {
        b_R = y->size[0] * (A->size[1] - 1);
        cr = 0;
        while ((m > 0) && (cr <= b_R)) {
          i5 = cr + m;
          for (ic = cr; ic + 1 <= i5; ic++) {
            d_y->data[ic] = 0.0;
          }

          cr += m;
        }

        br = 0;
        cr = 0;
        while ((m > 0) && (cr <= b_R)) {
          ar = -1;
          i5 = br + k;
          for (ib = br; ib + 1 <= i5; ib++) {
            if (A->data[ib] != 0.0) {
              ia = ar;
              i6 = cr + m;
              for (ic = cr; ic + 1 <= i6; ic++) {
                ia++;
                d_y->data[ic] += A->data[ib] * y->data[ia];
              }
            }

            ar += m;
          }

          br += k;
          cr += m;
        }
      }
    }

    i5 = zl->size[0];
    zl->size[0] = r->size[0];
    emxEnsureCapacity((emxArray__common *)zl, i5, (int)sizeof(double));
    br = r->size[0];
    for (i5 = 0; i5 < br; i5++) {
      zl->data[i5] = r->data[i5];
    }

    emxInit_real_T1(&e_y, 1);
    if ((d_y->size[1] == 1) || (r->size[0] == 1)) {
      i5 = e_y->size[0];
      e_y->size[0] = d_y->size[0];
      emxEnsureCapacity((emxArray__common *)e_y, i5, (int)sizeof(double));
      br = d_y->size[0];
      for (i5 = 0; i5 < br; i5++) {
        e_y->data[i5] = 0.0;
        ar = d_y->size[1];
        for (i6 = 0; i6 < ar; i6++) {
          e_y->data[i5] += d_y->data[i5 + d_y->size[0] * i6] * r->data[i6];
        }
      }

      i5 = r->size[0];
      r->size[0] = e_y->size[0];
      emxEnsureCapacity((emxArray__common *)r, i5, (int)sizeof(double));
      br = e_y->size[0];
      for (i5 = 0; i5 < br; i5++) {
        r->data[i5] = e_y->data[i5];
      }
    } else {
      k = d_y->size[1];
      K_idx_0 = (unsigned int)d_y->size[0];
      i5 = r->size[0];
      r->size[0] = (int)K_idx_0;
      emxEnsureCapacity((emxArray__common *)r, i5, (int)sizeof(double));
      m = d_y->size[0];
      i5 = r->size[0];
      emxEnsureCapacity((emxArray__common *)r, i5, (int)sizeof(double));
      br = r->size[0];
      for (i5 = 0; i5 < br; i5++) {
        r->data[i5] = 0.0;
      }

      if (d_y->size[0] == 0) {
      } else {
        cr = 0;
        while ((m > 0) && (cr <= 0)) {
          for (ic = 1; ic <= m; ic++) {
            r->data[ic - 1] = 0.0;
          }

          cr = m;
        }

        br = 0;
        cr = 0;
        while ((m > 0) && (cr <= 0)) {
          ar = -1;
          i5 = br + k;
          for (ib = br; ib + 1 <= i5; ib++) {
            if (zl->data[ib] != 0.0) {
              ia = ar;
              for (ic = 0; ic + 1 <= m; ic++) {
                ia++;
                r->data[ic] += zl->data[ib] * d_y->data[ia];
              }
            }

            ar += m;
          }

          br += k;
          cr = m;
        }
      }
    }

    emxFree_real_T(&e_y);
    emxFree_real_T(&d_y);
  }

  emxFree_real_T(&y);
  emxFree_real_T(&zl);
  emxFree_real_T(&A);

  // % Zustandsbegrenzungen
  //  Krümmungen
  //  bezogen auf Mittellinie, 0.72 entspricht einem minimalen Innenradius von 1m 
  i5 = r->size[0];
  float cut = 1.0;
  for (i = 2; i - 2 < (int)((double)i5 + -2.0); i++) {
    if ((-cut >= r->data[i]) || rtIsNaN(r->data[i])) {
      minval = -cut;
    } else {
      minval = r->data[i];
    }

    if ((cut <= minval) || rtIsNaN(minval)) {
      r->data[i] = cut;
    } else {
      r->data[i] = minval;
    }
  }

  //  y-Wert des ersten Punktes
  float cut_y = 0.7;
  if ((cut_y <= r->data[0]) || rtIsNaN(r->data[0])) {
    minval = cut_y;
  } else {
    minval = r->data[0];
  }

  if ((-cut_y >= minval) || rtIsNaN(minval)) {
    r->data[0] = -cut_y;
  } else {
    r->data[0] = minval;
  }

  //  Startwinkel
  float cutPhi = M_PI/2;
  if ((cutPhi <= r->data[1]) || rtIsNaN(r->data[1])) {
    minval = cutPhi;
  } else {
    minval = r->data[1];
  }

  if ((-cutPhi >= minval) || rtIsNaN(minval)) {
    r->data[1] = -cutPhi;
  } else {
    r->data[1] = minval;
  }

  //  0.79 rad = 45 Grad
}

//
// File trailer for kalman_filter_lr.cpp
//
// [EOF]
//
