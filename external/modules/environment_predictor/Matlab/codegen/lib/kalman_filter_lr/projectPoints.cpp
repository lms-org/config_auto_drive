//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: projectPoints.cpp
//
// MATLAB Coder version            : 3.0
// C/C++ source code generated on  : 07-Oct-2015 12:34:33
//

// Include Files
#include "rt_nonfinite.h"
#include "kalman_filter_lr.h"
#include "projectPoints.h"
#include "kalman_filter_lr_emxutil.h"

// Function Definitions

//
// Koordinaten der Punkte für die mittlere Spur berechnen
// Arguments    : const emxArray_real_T *r
//                double delta
//                double dist
//                emxArray_real_T *xp
//                emxArray_real_T *yp
//                emxArray_real_T *phi
// Return Type  : void
//
void projectPoints(const emxArray_real_T *r, double delta, double dist,
                   emxArray_real_T *xp, emxArray_real_T *yp, emxArray_real_T
                   *phi)
{
  emxArray_real_T *P;
  int i1;
  int loop_ub;
  double b_phi;
  int s;
  double vx;
  double vy;
  double betr;
  int z;
  double x;
  emxInit_real_T(&P, 2);
  i1 = P->size[0] * P->size[1];
  P->size[0] = r->size[0];
  P->size[1] = 3;
  emxEnsureCapacity((emxArray__common *)P, i1, (int)sizeof(double));
  loop_ub = r->size[0] * 3;
  for (i1 = 0; i1 < loop_ub; i1++) {
    P->data[i1] = 0.0;
  }

  // x, y, phi
  b_phi = r->data[1];

  // erster Punkt
  P->data[0] = 0.0;
  P->data[P->size[0]] = r->data[0];
  P->data[P->size[0] << 1] = r->data[1];

  // zweiter Punkt
  P->data[1] = P->data[0] + delta * cos(r->data[1]);
  P->data[1 + P->size[0]] = P->data[P->size[0]] + delta * sin(r->data[1]);
  P->data[1 + (P->size[0] << 1)] = r->data[1];
  for (s = 2; s - 2 <= r->size[0] - 3; s++) {
    b_phi = (b_phi + 3.1415926535897931) - 2.0 * acos(delta * r->data[s] / 2.0);
    P->data[s] = P->data[s - 1] + delta * cos(b_phi);
    P->data[s + P->size[0]] = P->data[(s + P->size[0]) - 1] + delta * sin(b_phi);
    P->data[s + (P->size[0] << 1)] = b_phi;
  }

  loop_ub = P->size[0];
  i1 = phi->size[0];
  phi->size[0] = loop_ub;
  emxEnsureCapacity((emxArray__common *)phi, i1, (int)sizeof(double));
  for (i1 = 0; i1 < loop_ub; i1++) {
    phi->data[i1] = P->data[i1 + (P->size[0] << 1)];
  }

  i1 = xp->size[0];
  xp->size[0] = r->size[0];
  emxEnsureCapacity((emxArray__common *)xp, i1, (int)sizeof(double));
  loop_ub = r->size[0];
  for (i1 = 0; i1 < loop_ub; i1++) {
    xp->data[i1] = 0.0;
  }

  i1 = yp->size[0];
  yp->size[0] = r->size[0];
  emxEnsureCapacity((emxArray__common *)yp, i1, (int)sizeof(double));
  loop_ub = r->size[0];
  for (i1 = 0; i1 < loop_ub; i1++) {
    yp->data[i1] = 0.0;
  }

  // % Projektion
  //  erster Punkt
  vx = -(P->data[1 + P->size[0]] - P->data[P->size[0]]);
  vy = P->data[1] - P->data[0];
  betr = sqrt(vx * vx + vy * vy);
  xp->data[0] = P->data[0] + 1.0 / betr * dist * vx;
  yp->data[0] = P->data[P->size[0]] + 1.0 / betr * dist * vy;

  // innere Punkte
  i1 = P->size[0];
  for (z = 1; z - 1 <= i1 - 3; z++) {
    vx = (P->data[z] - P->data[z - 1]) - 0.5 * (P->data[1 + z] - P->data[z - 1]);
    vy = (P->data[z + P->size[0]] - P->data[(z + P->size[0]) - 1]) - 0.5 *
      (P->data[(z + P->size[0]) + 1] - P->data[(z + P->size[0]) - 1]);
    betr = sqrt(vx * vx + vy * vy);
    if (betr < 0.0001) {
      vx = -(P->data[z + P->size[0]] - P->data[(z + P->size[0]) - 1]);
      vy = P->data[z] - P->data[z - 1];
      betr = sqrt(vx * vx + vy * vy);
      xp->data[z] = P->data[z] + 1.0 / betr * dist * vx;
      yp->data[z] = P->data[z + P->size[0]] + 1.0 / betr * dist * vy;
    } else {
      if (r->data[1 + z] < 0.0) {
        x = -1.0;
      } else if (r->data[1 + z] > 0.0) {
        x = 1.0;
      } else if (r->data[1 + z] == 0.0) {
        x = 0.0;
      } else {
        x = r->data[1 + z];
      }

      xp->data[z] = P->data[z] - x / betr * dist * vx;
      if (r->data[1 + z] < 0.0) {
        x = -1.0;
      } else if (r->data[1 + z] > 0.0) {
        x = 1.0;
      } else if (r->data[1 + z] == 0.0) {
        x = 0.0;
      } else {
        x = r->data[1 + z];
      }

      yp->data[z] = P->data[z + P->size[0]] - x / betr * dist * vy;
    }
  }

  // letzter Punkt
  i1 = P->size[0] - 1;
  loop_ub = P->size[0] - 2;
  vx = -(P->data[i1 + P->size[0]] - P->data[loop_ub + P->size[0]]);
  i1 = P->size[0] - 1;
  loop_ub = P->size[0] - 2;
  vy = P->data[i1] - P->data[loop_ub];
  betr = sqrt(vx * vx + vy * vy);
  i1 = P->size[0] - 1;
  loop_ub = P->size[0] - 1;
  xp->data[i1] = P->data[loop_ub] + 1.0 / betr * dist * vx;
  i1 = P->size[0] - 1;
  loop_ub = P->size[0] - 1;
  yp->data[i1] = P->data[loop_ub + P->size[0]] + 1.0 / betr * dist * vy;
  emxFree_real_T(&P);
}

//
// File trailer for projectPoints.cpp
//
// [EOF]
//
