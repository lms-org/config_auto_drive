/*
 * File: projectPoints.c
 *
 * MATLAB Coder version            : 2.7
 * C/C++ source code generated on  : 27-May-2015 19:40:39
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "kalman_filter_lr.h"
#include "projectPoints.h"

/* Function Definitions */

/*
 * Arguments    : const double r[10]
 *                double delta
 *                double dist
 *                double xp[10]
 *                double yp[10]
 * Return Type  : void
 */
void projectPoints(const double r[10], double delta, double dist, double xp[10],
                   double yp[10])
{
  double P[30];
  double phi;
  int i;
  double vy;
  double betr;
  double b_r;
  double c_r;
  memset(&P[0], 0, 30U * sizeof(double));

  /* x, y, phi */
  phi = r[1];
  for (i = 0; i < 10; i++) {
    if (1 + i == 1) {
      P[i] = 0.0;
      P[10 + i] = r[0];
      P[20 + i] = phi;
    } else if (1 + i == 2) {
      P[i] = P[i - 1] + delta * cos(r[1]);
      P[10 + i] = P[i + 9] + delta * sin(r[1]);
      P[20 + i] = phi;
    } else {
      phi = (phi - 2.0 * acos(delta * r[i] / 2.0)) - 3.1415926535897931;

      /*  -dw wegen VZ-Definition der Krümmung */
      P[i] = P[i - 1] + delta * cos(phi);
      P[10 + i] = P[i + 9] + delta * sin(phi);
      P[20 + i] = phi;
    }
  }

  for (i = 0; i < 10; i++) {
    xp[i] = 0.0;
    yp[i] = 0.0;
  }

  for (i = 0; i < 10; i++) {
    if (1 + i == 1) {
      phi = -(P[11] - P[10]);
      vy = P[1] - P[0];
      betr = sqrt(phi * phi + vy * vy);
      xp[0] = P[0] + 1.0 / betr * dist * phi;
      yp[0] = P[10] + 1.0 / betr * dist * vy;
    } else if (1 + i == 10) {
      phi = -(P[10 + i] - P[i + 9]);
      vy = P[i] - P[i - 1];
      betr = sqrt(phi * phi + vy * vy);
      xp[i] = P[i] + 1.0 / betr * dist * phi;
      yp[i] = P[10 + i] + 1.0 / betr * dist * vy;
    } else {
      phi = (P[i] - P[i - 1]) - 0.5 * (P[i + 1] - P[i - 1]);
      vy = (P[10 + i] - P[i + 9]) - 0.5 * (P[i + 11] - P[i + 9]);
      betr = sqrt(phi * phi + vy * vy);
      if (betr < 0.0001) {
        phi = -(P[10 + i] - P[i + 9]);
        vy = P[i] - P[i - 1];
        betr = sqrt(phi * phi + vy * vy);
        xp[i] = P[i] + 1.0 / betr * dist * phi;
        yp[i] = P[10 + i] + 1.0 / betr * dist * vy;
      } else {
        if (r[1 + i] < 0.0) {
          b_r = -1.0;
        } else if (r[1 + i] > 0.0) {
          b_r = 1.0;
        } else if (r[1 + i] == 0.0) {
          b_r = 0.0;
        } else {
          b_r = r[1 + i];
        }

        xp[i] = P[i] - b_r / betr * dist * phi;
        if (r[1 + i] < 0.0) {
          c_r = -1.0;
        } else if (r[1 + i] > 0.0) {
          c_r = 1.0;
        } else if (r[1 + i] == 0.0) {
          c_r = 0.0;
        } else {
          c_r = r[1 + i];
        }

        yp[i] = P[10 + i] - c_r / betr * dist * vy;
      }
    }
  }
}

/*
 * File trailer for projectPoints.c
 *
 * [EOF]
 */
