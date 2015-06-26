/*
 * File: d_line_point.c
 *
 * MATLAB Coder version            : 2.7
 * C/C++ source code generated on  : 26-Jun-2015 19:42:29
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "kalman_filter_lr.h"
#include "d_line_point.h"

/* Function Definitions */

/*
 * Abstand d zwischen der Gerade g (von P in Richtung Q) und dem Punkt M
 * S ist der Punkt auf g mit kleinstem Abstand zu M
 * Arguments    : const double P[2]
 *                const double Q[2]
 *                const double M[2]
 *                double *d
 *                double *lambda
 *                double S[2]
 * Return Type  : void
 */
void d_line_point(const double P[2], const double Q[2], const double M[2],
                  double *d, double *lambda, double S[2])
{
  int i1;
  double a;
  double b_a;
  for (i1 = 0; i1 < 2; i1++) {
    S[i1] = Q[i1] - P[i1];
  }

  *lambda = -(S[0] * (P[0] - M[0]) + S[1] * (P[1] - M[1])) / (S[0] * S[0] + S[1]
    * S[1]);
  for (i1 = 0; i1 < 2; i1++) {
    S[i1] = P[i1] + *lambda * S[i1];
  }

  a = M[0] - S[0];
  b_a = M[1] - S[1];
  *d = sqrt(a * a + b_a * b_a);
}

/*
 * File trailer for d_line_point.c
 *
 * [EOF]
 */
