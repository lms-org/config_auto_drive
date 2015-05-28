/*
 * File: projectPoints.c
 *
 * MATLAB Coder version            : 2.7
 * C/C++ source code generated on  : 28-May-2015 15:10:27
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "kalman_filter_lr.h"
#include "projectPoints.h"
#include "kalman_filter_lr_emxutil.h"
#include "getPointsFromState.h"

/* Function Definitions */

/*
 * Koordinaten der Punkte für die mittlere Spur berechnen
 * Arguments    : const emxArray_real_T *r
 *                double delta
 *                emxArray_real_T *xp
 *                emxArray_real_T *yp
 *                emxArray_real_T *phi
 * Return Type  : void
 */
void b_projectPoints(const emxArray_real_T *r, double delta, emxArray_real_T *xp,
                     emxArray_real_T *yp, emxArray_real_T *phi)
{
  emxArray_real_T *x;
  emxArray_real_T *y;
  int z;
  int loop_ub;
  double vx;
  double vy;
  double betr;
  double b_x;
  emxInit_real_T(&x, 1);
  emxInit_real_T(&y, 1);
  getPointsFromState(r, delta, x, y, phi);
  z = xp->size[0];
  xp->size[0] = r->size[0];
  emxEnsureCapacity((emxArray__common *)xp, z, (int)sizeof(double));
  loop_ub = r->size[0];
  for (z = 0; z < loop_ub; z++) {
    xp->data[z] = 0.0;
  }

  z = yp->size[0];
  yp->size[0] = r->size[0];
  emxEnsureCapacity((emxArray__common *)yp, z, (int)sizeof(double));
  loop_ub = r->size[0];
  for (z = 0; z < loop_ub; z++) {
    yp->data[z] = 0.0;
  }

  /* % Projektion  */
  /*  erster Punkt */
  vx = -(y->data[1] - y->data[0]);
  vy = x->data[1] - x->data[0];
  betr = sqrt(vx * vx + vy * vy);
  xp->data[0] = x->data[0] + 1.0 / betr * -0.38 * vx;
  yp->data[0] = y->data[0] + 1.0 / betr * -0.38 * vy;

  /* innere Punkte */
  for (z = 1; z - 1 <= x->size[0] - 3; z++) {
    vx = (x->data[z] - x->data[z - 1]) - 0.5 * (x->data[z + 1] - x->data[z - 1]);
    vy = (y->data[z] - y->data[z - 1]) - 0.5 * (y->data[z + 1] - y->data[z - 1]);
    betr = sqrt(vx * vx + vy * vy);
    if (betr < 0.0001) {
      vx = -(y->data[z] - y->data[z - 1]);
      vy = x->data[z] - x->data[z - 1];
      betr = sqrt(vx * vx + vy * vy);
      xp->data[z] = x->data[z] + 1.0 / betr * -0.38 * vx;
      yp->data[z] = y->data[z] + 1.0 / betr * -0.38 * vy;
    } else {
      if (r->data[z + 1] < 0.0) {
        b_x = -1.0;
      } else if (r->data[z + 1] > 0.0) {
        b_x = 1.0;
      } else if (r->data[z + 1] == 0.0) {
        b_x = 0.0;
      } else {
        b_x = r->data[z + 1];
      }

      xp->data[z] = x->data[z] - b_x / betr * -0.38 * vx;
      if (r->data[z + 1] < 0.0) {
        b_x = -1.0;
      } else if (r->data[z + 1] > 0.0) {
        b_x = 1.0;
      } else if (r->data[z + 1] == 0.0) {
        b_x = 0.0;
      } else {
        b_x = r->data[z + 1];
      }

      yp->data[z] = y->data[z] - b_x / betr * -0.38 * vy;
    }
  }

  /* letzter Punkt */
  vx = -(y->data[x->size[0] - 1] - y->data[x->size[0] - 2]);
  vy = x->data[x->size[0] - 1] - x->data[x->size[0] - 2];
  betr = sqrt(vx * vx + vy * vy);
  xp->data[x->size[0] - 1] = x->data[x->size[0] - 1] + 1.0 / betr * -0.38 * vx;
  yp->data[x->size[0] - 1] = y->data[x->size[0] - 1] + 1.0 / betr * -0.38 * vy;
  emxFree_real_T(&y);
  emxFree_real_T(&x);
}

/*
 * Koordinaten der Punkte für die mittlere Spur berechnen
 * Arguments    : const emxArray_real_T *r
 *                double delta
 *                emxArray_real_T *xp
 *                emxArray_real_T *yp
 *                emxArray_real_T *phi
 * Return Type  : void
 */
void projectPoints(const emxArray_real_T *r, double delta, emxArray_real_T *xp,
                   emxArray_real_T *yp, emxArray_real_T *phi)
{
  emxArray_real_T *x;
  emxArray_real_T *y;
  int z;
  int loop_ub;
  double vx;
  double vy;
  double betr;
  double b_x;
  emxInit_real_T(&x, 1);
  emxInit_real_T(&y, 1);
  getPointsFromState(r, delta, x, y, phi);
  z = xp->size[0];
  xp->size[0] = r->size[0];
  emxEnsureCapacity((emxArray__common *)xp, z, (int)sizeof(double));
  loop_ub = r->size[0];
  for (z = 0; z < loop_ub; z++) {
    xp->data[z] = 0.0;
  }

  z = yp->size[0];
  yp->size[0] = r->size[0];
  emxEnsureCapacity((emxArray__common *)yp, z, (int)sizeof(double));
  loop_ub = r->size[0];
  for (z = 0; z < loop_ub; z++) {
    yp->data[z] = 0.0;
  }

  /* % Projektion  */
  /*  erster Punkt */
  vx = -(y->data[1] - y->data[0]);
  vy = x->data[1] - x->data[0];
  betr = sqrt(vx * vx + vy * vy);
  xp->data[0] = x->data[0] + 1.0 / betr * 0.38 * vx;
  yp->data[0] = y->data[0] + 1.0 / betr * 0.38 * vy;

  /* innere Punkte */
  for (z = 1; z - 1 <= x->size[0] - 3; z++) {
    vx = (x->data[z] - x->data[z - 1]) - 0.5 * (x->data[z + 1] - x->data[z - 1]);
    vy = (y->data[z] - y->data[z - 1]) - 0.5 * (y->data[z + 1] - y->data[z - 1]);
    betr = sqrt(vx * vx + vy * vy);
    if (betr < 0.0001) {
      vx = -(y->data[z] - y->data[z - 1]);
      vy = x->data[z] - x->data[z - 1];
      betr = sqrt(vx * vx + vy * vy);
      xp->data[z] = x->data[z] + 1.0 / betr * 0.38 * vx;
      yp->data[z] = y->data[z] + 1.0 / betr * 0.38 * vy;
    } else {
      if (r->data[z + 1] < 0.0) {
        b_x = -1.0;
      } else if (r->data[z + 1] > 0.0) {
        b_x = 1.0;
      } else if (r->data[z + 1] == 0.0) {
        b_x = 0.0;
      } else {
        b_x = r->data[z + 1];
      }

      xp->data[z] = x->data[z] - b_x / betr * 0.38 * vx;
      if (r->data[z + 1] < 0.0) {
        b_x = -1.0;
      } else if (r->data[z + 1] > 0.0) {
        b_x = 1.0;
      } else if (r->data[z + 1] == 0.0) {
        b_x = 0.0;
      } else {
        b_x = r->data[z + 1];
      }

      yp->data[z] = y->data[z] - b_x / betr * 0.38 * vy;
    }
  }

  /* letzter Punkt */
  vx = -(y->data[x->size[0] - 1] - y->data[x->size[0] - 2]);
  vy = x->data[x->size[0] - 1] - x->data[x->size[0] - 2];
  betr = sqrt(vx * vx + vy * vy);
  xp->data[x->size[0] - 1] = x->data[x->size[0] - 1] + 1.0 / betr * 0.38 * vx;
  yp->data[x->size[0] - 1] = y->data[x->size[0] - 1] + 1.0 / betr * 0.38 * vy;
  emxFree_real_T(&y);
  emxFree_real_T(&x);
}

/*
 * File trailer for projectPoints.c
 *
 * [EOF]
 */
