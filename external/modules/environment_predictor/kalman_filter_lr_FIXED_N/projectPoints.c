/*
 * File: projectPoints.c
 *
 * MATLAB Coder version            : 2.7
 * C/C++ source code generated on  : 28-May-2015 15:37:30
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "kalman_filter_lr.h"
#include "projectPoints.h"

/* Function Definitions */

/*
 * Koordinaten der Punkte für die mittlere Spur berechnen
 * Arguments    : const double r[10]
 *                double delta
 *                double dist
 *                double xp[10]
 *                double yp[10]
 *                double phi[10]
 * Return Type  : void
 */
void projectPoints(const double r[10], double delta, double dist, double xp[10],
                   double yp[10], double phi[10])
{
  double P[30];
  double b_phi;
  int s;
  double vy;
  double betr;
  double b_r;
  double c_r;
  memset(&P[0], 0, 30U * sizeof(double));

  /* x, y, phi */
  b_phi = r[1];

  /* erster Punkt  */
  P[0] = 0.0;
  P[10] = r[0];
  P[20] = r[1];

  /* zweiter Punkt */
  P[1] = delta * cos(r[1]);
  P[11] = r[0] + delta * sin(r[1]);
  P[21] = r[1];
  for (s = 0; s < 8; s++) {
    b_phi = (b_phi - 2.0 * acos(delta * r[s + 2] / 2.0)) - 3.1415926535897931;

    /*  -dw wegen VZ-Definition der Krümmung */
    P[s + 2] = P[1 + s] + delta * cos(b_phi);
    P[s + 12] = P[s + 11] + delta * sin(b_phi);
    P[s + 22] = b_phi;
  }

  memcpy(&phi[0], &P[20], 10U * sizeof(double));

  /* % Projektion  */
  /*  erster Punkt */
  b_phi = -(P[11] - P[10]);
  vy = P[1] - P[0];
  betr = sqrt(b_phi * b_phi + vy * vy);
  xp[0] = P[0] + 1.0 / betr * dist * b_phi;
  yp[0] = P[10] + 1.0 / betr * dist * vy;

  /* innere Punkte */
  for (s = 0; s < 8; s++) {
    b_phi = (P[s + 1] - P[s]) - 0.5 * (P[2 + s] - P[s]);
    vy = (P[s + 11] - P[10 + s]) - 0.5 * (P[s + 12] - P[10 + s]);
    betr = sqrt(b_phi * b_phi + vy * vy);
    if (betr < 0.0001) {
      b_phi = -(P[s + 11] - P[10 + s]);
      vy = P[s + 1] - P[s];
      betr = sqrt(b_phi * b_phi + vy * vy);
      xp[s + 1] = P[s + 1] + 1.0 / betr * dist * b_phi;
      yp[s + 1] = P[s + 11] + 1.0 / betr * dist * vy;
    } else {
      if (r[2 + s] < 0.0) {
        b_r = -1.0;
      } else if (r[2 + s] > 0.0) {
        b_r = 1.0;
      } else if (r[2 + s] == 0.0) {
        b_r = 0.0;
      } else {
        b_r = r[2 + s];
      }

      xp[s + 1] = P[s + 1] - b_r / betr * dist * b_phi;
      if (r[2 + s] < 0.0) {
        c_r = -1.0;
      } else if (r[2 + s] > 0.0) {
        c_r = 1.0;
      } else if (r[2 + s] == 0.0) {
        c_r = 0.0;
      } else {
        c_r = r[2 + s];
      }

      yp[s + 1] = P[s + 11] - c_r / betr * dist * vy;
    }
  }

  /* letzter Punkt */
  b_phi = -(P[19] - P[18]);
  vy = P[9] - P[8];
  betr = sqrt(b_phi * b_phi + vy * vy);
  xp[9] = P[9] + 1.0 / betr * dist * b_phi;
  yp[9] = P[19] + 1.0 / betr * dist * vy;
}

/*
 * File trailer for projectPoints.c
 *
 * [EOF]
 */
