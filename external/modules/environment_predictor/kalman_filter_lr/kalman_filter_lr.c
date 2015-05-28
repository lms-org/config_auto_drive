/*
 * File: kalman_filter_lr.c
 *
 * MATLAB Coder version            : 2.7
 * C/C++ source code generated on  : 27-May-2015 19:40:39
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "kalman_filter_lr.h"
#include "d_line_point.h"
#include "eye.h"
#include "mrdivide.h"
#include "messmatrix_new.h"
#include "projectPoints.h"

/* Function Definitions */

/*
 * % linke Seitenlinie
 * Arguments    : double r[10]
 *                const double A[100]
 *                double Pk[100]
 *                const double Q[100]
 *                double R_fakt
 *                double delta
 *                const double xl[10]
 *                const double yl[10]
 *                const double xr[10]
 *                const double yr[10]
 * Return Type  : void
 */
void kalman_filter_lr(double r[10], const double A[100], double Pk[100], const
                      double Q[100], double R_fakt, double delta, const double
                      xl[10], const double yl[10], const double xr[10], const
                      double yr[10])
{
  double yp[10];
  double xp[10];
  double P[30];
  int i;
  double D[30];
  int m;
  double dist_point;
  double a;
  double b_P[2];
  double c_P[2];
  double b_xl[2];
  double unusedU1[2];
  double Hl[200];
  double zl[20];
  double zml[20];
  double d_P[2];
  double e_P[2];
  double b_xr[2];
  double Hr[200];
  double zr[20];
  double zmr[20];
  double H[400];
  double R[1600];
  double b_A[100];
  int i2;
  double b_Pk[400];
  double K[400];
  double b_H[1600];
  double b_zl[40];
  double b_zml[40];
  double c_zl[40];
  double b_a[100];
  double c_a[100];
  projectPoints(r, delta, 0.38, xp, yp);

  /* Punkte von Mittellinie nach links projizieren */
  for (i = 0; i < 10; i++) {
    P[i] = xp[i];
    P[10 + i] = yp[i];
    P[20 + i] = 0.0;
  }

  for (i = 0; i < 30; i++) {
    D[i] = 10000.0;
  }

  for (i = 0; i < 10; i++) {
    for (m = 0; m < 10; m++) {
      dist_point = P[i] - xl[m];
      a = P[10 + i] - yl[m];
      dist_point = sqrt(dist_point * dist_point + a * a);
      if (dist_point < D[20 + m]) {
        D[m] = 1.0 + (double)i;
        D[10 + m] = 0.0;
        D[20 + m] = dist_point;
      }

      if (1 + i > 1) {
        b_P[0] = P[i - 1];
        b_P[1] = P[i + 9];
        c_P[0] = P[i];
        c_P[1] = P[10 + i];
        b_xl[0] = xl[m];
        b_xl[1] = yl[m];
        d_line_point(b_P, c_P, b_xl, &dist_point, &a, unusedU1);
        if ((dist_point < D[20 + m]) && (a > 0.0) && (a < 1.0)) {
          D[m] = 1.0 + (double)i;
          D[10 + m] = a;
          D[20 + m] = dist_point;
        }
      }
    }
  }

  /*  Messpunkte ausfiltern, die vor dem letzten prädizierten Punkt liegen */
  /*  ind = ((D(:, 1) == numel(r) & D(:, 2) == 0) | D(:, 3) > 0.5); */
  /*  xl(ind) = []; */
  /*  yl(ind) = []; */
  /*  D(ind, :) = []; */
  /*  Messmatrix, Mess- und Erwartungsvektor bauen */
  messmatrix_new(P, r, delta, *(double (*)[10])&D[0], *(double (*)[10])&D[10],
                 Hl);
  for (i = 0; i < 20; i++) {
    zl[i] = 0.0;
    zml[i] = 0.0;
  }

  i = 0;
  for (m = 0; m < 10; m++) {
    zl[i] = xl[m];
    zl[i + 1] = yl[m];
    if (D[10 + m] > 0.0) {
      zml[i] = P[(int)(D[m] - 1.0) - 1] + D[10 + m] * (P[(int)D[m] - 1] - P[(int)
        (D[m] - 1.0) - 1]);
      zml[i + 1] = P[(int)(D[m] - 1.0) + 9] + D[10 + m] * (P[(int)D[m] + 9] - P
        [(int)(D[m] - 1.0) + 9]);
    } else {
      zml[i] = P[(int)D[m] - 1];
      zml[i + 1] = P[(int)D[m] + 9];
    }

    i += 2;
  }

  /* % rechte Seitenlinie */
  projectPoints(r, delta, -0.38, xp, yp);

  /* Punkte von Mittellinie nach rechts projizieren */
  for (i = 0; i < 10; i++) {
    P[i] = xp[i];
    P[10 + i] = yp[i];
    P[20 + i] = 0.0;
  }

  for (i = 0; i < 30; i++) {
    D[i] = 10000.0;
  }

  for (i = 0; i < 10; i++) {
    for (m = 0; m < 10; m++) {
      dist_point = P[i] - xr[m];
      a = P[10 + i] - yr[m];
      dist_point = sqrt(dist_point * dist_point + a * a);
      if (dist_point < D[20 + m]) {
        D[m] = 1.0 + (double)i;
        D[10 + m] = 0.0;
        D[20 + m] = dist_point;
      }

      if (1 + i > 1) {
        d_P[0] = P[i - 1];
        d_P[1] = P[i + 9];
        e_P[0] = P[i];
        e_P[1] = P[10 + i];
        b_xr[0] = xr[m];
        b_xr[1] = yr[m];
        d_line_point(d_P, e_P, b_xr, &dist_point, &a, unusedU1);
        if ((dist_point < D[20 + m]) && (a > 0.0) && (a < 1.0)) {
          D[m] = 1.0 + (double)i;
          D[10 + m] = a;
          D[20 + m] = dist_point;
        }
      }
    }
  }

  /*  Messpunkte ausfiltern, die vor dem letzten prädizierten Punkt liegen */
  /*  ind = ((D(:, 1) == numel(r) & D(:, 2) == 0) | D(:, 3) > 0.5 ); */
  /*  xr(ind) = []; */
  /*  yr(ind) = []; */
  /*  D(ind, :) = []; */
  /*  Messmatrix, Mess- und Erwartungsvektor bauen */
  messmatrix_new(P, r, delta, *(double (*)[10])&D[0], *(double (*)[10])&D[10],
                 Hr);
  for (i = 0; i < 20; i++) {
    zr[i] = 0.0;
    zmr[i] = 0.0;
  }

  i = 0;
  for (m = 0; m < 10; m++) {
    zr[i] = xr[m];
    zr[i + 1] = yr[m];
    if (D[10 + m] > 0.0) {
      zmr[i] = P[(int)(D[m] - 1.0) - 1] + D[10 + m] * (P[(int)D[m] - 1] - P[(int)
        (D[m] - 1.0) - 1]);
      zmr[i + 1] = P[(int)(D[m] - 1.0) + 9] + D[10 + m] * (P[(int)D[m] + 9] - P
        [(int)(D[m] - 1.0) + 9]);
    } else {
      zmr[i] = P[(int)D[m] - 1];
      zmr[i + 1] = P[(int)D[m] + 9];
    }

    i += 2;
  }

  /* % beide Linien kombinieren */
  for (i = 0; i < 10; i++) {
    memcpy(&H[40 * i], &Hl[20 * i], 20U * sizeof(double));
    memcpy(&H[20 + 40 * i], &Hr[20 * i], 20U * sizeof(double));
  }

  /*  H = Hl; */
  /*  z = zl; */
  /*  zm =zml; */
  /*  H = Hr; */
  /*  z = zr; */
  /*  zm =zmr; */
  /* % KALMAN FILTER     */
  /*  r = A*r + B*u */
  eye(R);
  for (i = 0; i < 1600; i++) {
    R[i] *= R_fakt;
  }

  for (i = 0; i < 10; i++) {
    for (m = 0; m < 10; m++) {
      b_A[i + 10 * m] = 0.0;
      for (i2 = 0; i2 < 10; i2++) {
        b_A[i + 10 * m] += A[i + 10 * i2] * Pk[i2 + 10 * m];
      }
    }
  }

  for (i = 0; i < 10; i++) {
    for (m = 0; m < 10; m++) {
      dist_point = 0.0;
      for (i2 = 0; i2 < 10; i2++) {
        dist_point += b_A[i + 10 * i2] * A[m + 10 * i2];
      }

      Pk[i + 10 * m] = dist_point + Q[i + 10 * m];
    }
  }

  for (i = 0; i < 10; i++) {
    for (m = 0; m < 40; m++) {
      b_Pk[i + 10 * m] = 0.0;
      for (i2 = 0; i2 < 10; i2++) {
        b_Pk[i + 10 * m] += Pk[i + 10 * i2] * H[m + 40 * i2];
      }
    }
  }

  for (i = 0; i < 40; i++) {
    for (m = 0; m < 10; m++) {
      K[i + 40 * m] = 0.0;
      for (i2 = 0; i2 < 10; i2++) {
        K[i + 40 * m] += H[i + 40 * i2] * Pk[i2 + 10 * m];
      }
    }
  }

  for (i = 0; i < 40; i++) {
    for (m = 0; m < 40; m++) {
      dist_point = 0.0;
      for (i2 = 0; i2 < 10; i2++) {
        dist_point += K[i + 40 * i2] * H[m + 40 * i2];
      }

      b_H[i + 40 * m] = dist_point + R[i + 40 * m];
    }
  }

  mrdivide(b_Pk, b_H, K);
  memcpy(&b_zl[0], &zl[0], 20U * sizeof(double));
  for (i = 0; i < 20; i++) {
    b_zl[i + 20] = zr[i];
    b_zml[i] = zml[i];
  }

  memcpy(&b_zml[20], &zmr[0], 20U * sizeof(double));
  for (i = 0; i < 40; i++) {
    c_zl[i] = b_zl[i] - b_zml[i];
  }

  for (i = 0; i < 10; i++) {
    xp[i] = 0.0;
    for (m = 0; m < 40; m++) {
      xp[i] += K[i + 10 * m] * c_zl[m];
    }

    r[i] += xp[i];
  }

  b_eye(b_A);
  for (i = 0; i < 10; i++) {
    for (m = 0; m < 10; m++) {
      dist_point = 0.0;
      for (i2 = 0; i2 < 40; i2++) {
        dist_point += K[i + 10 * i2] * H[i2 + 40 * m];
      }

      b_a[i + 10 * m] = b_A[i + 10 * m] - dist_point;
    }
  }

  for (i = 0; i < 10; i++) {
    for (m = 0; m < 10; m++) {
      c_a[i + 10 * m] = 0.0;
      for (i2 = 0; i2 < 10; i2++) {
        c_a[i + 10 * m] += b_a[i + 10 * i2] * Pk[i2 + 10 * m];
      }
    }
  }

  for (i = 0; i < 10; i++) {
    memcpy(&Pk[10 * i], &c_a[10 * i], 10U * sizeof(double));
  }

  /* % Zustandsbegrenzungen */
  /*  % Krümmung */
  /*  bezogen auf Mittellinie, 0.72 entspricht einem minimalen Innenradius von 1m */
  for (i = 0; i < 8; i++) {
    if ((-0.75 >= r[i + 2]) || rtIsNaN(r[i + 2])) {
      r[i + 2] = -0.75;
    }

    if ((0.75 <= r[i + 2]) || rtIsNaN(r[i + 2])) {
      r[i + 2] = 0.75;
    }
  }

  /*  % y-Wert des ersten Punktes */
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

  /*  % Startwinkel  */
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
