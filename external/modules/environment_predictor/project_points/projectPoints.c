/*
 * File: projectPoints.c
 *
 * MATLAB Coder version            : 2.7
 * C/C++ source code generated on  : 28-May-2015 16:56:25
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "projectPoints.h"
#include "projectPoints_emxutil.h"

/* Function Definitions */

/*
 * Koordinaten der Punkte für die mittlere Spur berechnen
 * Arguments    : const emxArray_real_T *r
 *                double delta
 *                double dist
 *                emxArray_real_T *xp
 *                emxArray_real_T *yp
 * Return Type  : void
 */
void projectPoints(const emxArray_real_T *r, double delta, double dist,
                   emxArray_real_T *xp, emxArray_real_T *yp)
{
  emxArray_real_T *P;
  int r_idx_0;
  int i0;
  double phi;
  double vy;
  double betr;
  double x;
  emxInit_real_T(&P, 2);
  r_idx_0 = r->size[0];
  i0 = P->size[0] * P->size[1];
  P->size[0] = r_idx_0;
  P->size[1] = 3;
  emxEnsureCapacity((emxArray__common *)P, i0, (int)sizeof(double));
  r_idx_0 = r->size[0] * 3;
  for (i0 = 0; i0 < r_idx_0; i0++) {
    P->data[i0] = 0.0;
  }

  /* x, y, phi */
  phi = r->data[1];

  /* erster Punkt  */
  P->data[0] = 0.0;
  P->data[P->size[0]] = r->data[0];
  P->data[P->size[0] << 1] = r->data[1];

  /* zweiter Punkt */
  P->data[1] = P->data[0] + delta * cos(r->data[1]);
  P->data[1 + P->size[0]] = P->data[P->size[0]] + delta * sin(r->data[1]);
  P->data[1 + (P->size[0] << 1)] = r->data[1];
  for (r_idx_0 = 2; r_idx_0 - 2 <= r->size[0] - 3; r_idx_0++) {
    phi = (phi - 2.0 * acos(delta * r->data[r_idx_0] / 2.0)) -
      3.1415926535897931;

    /*  -dw wegen VZ-Definition der Krümmung */
    P->data[r_idx_0] = P->data[r_idx_0 - 1] + delta * cos(phi);
    P->data[r_idx_0 + P->size[0]] = P->data[(r_idx_0 + P->size[0]) - 1] + delta *
      sin(phi);
    P->data[r_idx_0 + (P->size[0] << 1)] = phi;
  }

  i0 = xp->size[0];
  xp->size[0] = r->size[0];
  emxEnsureCapacity((emxArray__common *)xp, i0, (int)sizeof(double));
  r_idx_0 = r->size[0];
  for (i0 = 0; i0 < r_idx_0; i0++) {
    xp->data[i0] = 0.0;
  }

  i0 = yp->size[0];
  yp->size[0] = r->size[0];
  emxEnsureCapacity((emxArray__common *)yp, i0, (int)sizeof(double));
  r_idx_0 = r->size[0];
  for (i0 = 0; i0 < r_idx_0; i0++) {
    yp->data[i0] = 0.0;
  }

  /* % Projektion  */
  /*  erster Punkt */
  phi = -(P->data[1 + P->size[0]] - P->data[P->size[0]]);
  vy = P->data[1] - P->data[0];
  betr = sqrt(phi * phi + vy * vy);
  xp->data[0] = P->data[0] + 1.0 / betr * dist * phi;
  yp->data[0] = P->data[P->size[0]] + 1.0 / betr * dist * vy;

  /* innere Punkte */
  i0 = P->size[0];
  for (r_idx_0 = 1; r_idx_0 - 1 <= i0 - 3; r_idx_0++) {
    phi = (P->data[r_idx_0] - P->data[r_idx_0 - 1]) - 0.5 * (P->data[1 + r_idx_0]
      - P->data[r_idx_0 - 1]);
    vy = (P->data[r_idx_0 + P->size[0]] - P->data[(r_idx_0 + P->size[0]) - 1]) -
      0.5 * (P->data[(r_idx_0 + P->size[0]) + 1] - P->data[(r_idx_0 + P->size[0])
             - 1]);
    betr = sqrt(phi * phi + vy * vy);
    if (betr < 0.0001) {
      phi = -(P->data[r_idx_0 + P->size[0]] - P->data[(r_idx_0 + P->size[0]) - 1]);
      vy = P->data[r_idx_0] - P->data[r_idx_0 - 1];
      betr = sqrt(phi * phi + vy * vy);
      xp->data[r_idx_0] = P->data[r_idx_0] + 1.0 / betr * dist * phi;
      yp->data[r_idx_0] = P->data[r_idx_0 + P->size[0]] + 1.0 / betr * dist * vy;
    } else {
      if (r->data[r_idx_0 + 1] < 0.0) {
        x = -1.0;
      } else if (r->data[r_idx_0 + 1] > 0.0) {
        x = 1.0;
      } else if (r->data[r_idx_0 + 1] == 0.0) {
        x = 0.0;
      } else {
        x = r->data[r_idx_0 + 1];
      }

      xp->data[r_idx_0] = P->data[r_idx_0] - x / betr * dist * phi;
      if (r->data[r_idx_0 + 1] < 0.0) {
        x = -1.0;
      } else if (r->data[r_idx_0 + 1] > 0.0) {
        x = 1.0;
      } else if (r->data[r_idx_0 + 1] == 0.0) {
        x = 0.0;
      } else {
        x = r->data[r_idx_0 + 1];
      }

      yp->data[r_idx_0] = P->data[r_idx_0 + P->size[0]] - x / betr * dist * vy;
    }
  }

  /* letzter Punkt */
  i0 = P->size[0] - 1;
  r_idx_0 = P->size[0] - 2;
  phi = -(P->data[i0 + P->size[0]] - P->data[r_idx_0 + P->size[0]]);
  i0 = P->size[0] - 1;
  r_idx_0 = P->size[0] - 2;
  vy = P->data[i0] - P->data[r_idx_0];
  betr = sqrt(phi * phi + vy * vy);
  i0 = P->size[0] - 1;
  r_idx_0 = P->size[0] - 1;
  xp->data[i0] = P->data[r_idx_0] + 1.0 / betr * dist * phi;
  i0 = P->size[0] - 1;
  r_idx_0 = P->size[0] - 1;
  yp->data[i0] = P->data[r_idx_0 + P->size[0]] + 1.0 / betr * dist * vy;
  emxFree_real_T(&P);
}

/*
 * File trailer for projectPoints.c
 *
 * [EOF]
 */
