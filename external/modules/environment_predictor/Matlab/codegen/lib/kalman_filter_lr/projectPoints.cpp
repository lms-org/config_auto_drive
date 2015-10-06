//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: projectPoints.cpp
//
// MATLAB Coder version            : 3.0
// C/C++ source code generated on  : 06-Oct-2015 19:14:10
//

// Include Files
#include "rt_nonfinite.h"
#include "kalman_filter_lr.h"
#include "projectPoints.h"
#include "kalman_filter_lr_emxutil.h"
#include "getPointsFromState.h"

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
  emxArray_real_T *x;
  emxArray_real_T *y;
  int i1;
  int loop_ub;
  double vx;
  double vy;
  double betr;
  int z;
  double b_x;
  emxInit_real_T1(&x, 1);
  emxInit_real_T1(&y, 1);
  getPointsFromState(r, delta, x, y, phi);
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
  vx = -(y->data[1] - y->data[0]);
  vy = x->data[1] - x->data[0];
  betr = sqrt(vx * vx + vy * vy);
  xp->data[0] = x->data[0] + 1.0 / betr * dist * vx;
  yp->data[0] = y->data[0] + 1.0 / betr * dist * vy;

  // innere Punkte
  for (z = 1; z - 1 <= x->size[0] - 3; z++) {
    vx = (x->data[z] - x->data[z - 1]) - 0.5 * (x->data[1 + z] - x->data[z - 1]);
    vy = (y->data[z] - y->data[z - 1]) - 0.5 * (y->data[1 + z] - y->data[z - 1]);
    betr = sqrt(vx * vx + vy * vy);
    if (betr < 0.0001) {
      vx = -(y->data[z] - y->data[z - 1]);
      vy = x->data[z] - x->data[z - 1];
      betr = sqrt(vx * vx + vy * vy);
      xp->data[z] = x->data[z] + 1.0 / betr * dist * vx;
      yp->data[z] = y->data[z] + 1.0 / betr * dist * vy;
    } else {
      if (r->data[1 + z] < 0.0) {
        b_x = -1.0;
      } else if (r->data[1 + z] > 0.0) {
        b_x = 1.0;
      } else if (r->data[1 + z] == 0.0) {
        b_x = 0.0;
      } else {
        b_x = r->data[1 + z];
      }

      xp->data[z] = x->data[z] - b_x / betr * dist * vx;
      if (r->data[1 + z] < 0.0) {
        b_x = -1.0;
      } else if (r->data[1 + z] > 0.0) {
        b_x = 1.0;
      } else if (r->data[1 + z] == 0.0) {
        b_x = 0.0;
      } else {
        b_x = r->data[1 + z];
      }

      yp->data[z] = y->data[z] - b_x / betr * dist * vy;
    }
  }

  // letzter Punkt
  vx = -(y->data[x->size[0] - 1] - y->data[x->size[0] - 2]);
  vy = x->data[x->size[0] - 1] - x->data[x->size[0] - 2];
  betr = sqrt(vx * vx + vy * vy);
  xp->data[x->size[0] - 1] = x->data[x->size[0] - 1] + 1.0 / betr * dist * vx;
  yp->data[x->size[0] - 1] = y->data[x->size[0] - 1] + 1.0 / betr * dist * vy;
  emxFree_real_T(&y);
  emxFree_real_T(&x);
}

//
// File trailer for projectPoints.cpp
//
// [EOF]
//
