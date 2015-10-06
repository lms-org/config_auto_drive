//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: messmatrix.cpp
//
// MATLAB Coder version            : 3.0
// C/C++ source code generated on  : 06-Oct-2015 19:14:10
//

// Include Files
#include "rt_nonfinite.h"
#include "kalman_filter_lr.h"
#include "messmatrix.h"
#include "kalman_filter_lr_emxutil.h"

// Function Definitions

//
// Arguments    : const emxArray_real_T *P
//                const emxArray_real_T *r
//                double delta
//                const emxArray_real_T *ind
//                const emxArray_real_T *lambda
//                emxArray_real_T *H
// Return Type  : void
//
void messmatrix(const emxArray_real_T *P, const emxArray_real_T *r, double delta,
                const emxArray_real_T *ind, const emxArray_real_T *lambda,
                emxArray_real_T *H)
{
  emxArray_real_T *d_phi0;
  int num_points;
  int i3;
  int loop_ub;
  int p;
  emxArray_real_T *d_c_x;
  emxArray_real_T *d_c_y;
  int c;
  unsigned int count;
  int i;
  emxInit_real_T(&d_phi0, 2);
  num_points = r->size[0] - 3;

  // Ableitungen nach phi0
  i3 = d_phi0->size[0] * d_phi0->size[1];
  d_phi0->size[0] = r->size[0];
  d_phi0->size[1] = 2;
  emxEnsureCapacity((emxArray__common *)d_phi0, i3, (int)sizeof(double));
  loop_ub = r->size[0] << 1;
  for (i3 = 0; i3 < loop_ub; i3++) {
    d_phi0->data[i3] = 0.0;
  }

  for (p = 1; p - 1 <= num_points + 1; p++) {
    d_phi0->data[p] = d_phi0->data[p - 1] - delta * sin(P->data[p + (P->size[0] <<
      1)]);

    // x-Komponenten
    d_phi0->data[p + d_phi0->size[0]] = d_phi0->data[(p + d_phi0->size[0]) - 1]
      + delta * cos(P->data[p + (P->size[0] << 1)]);

    // y-Komponenten
  }

  emxInit_real_T(&d_c_x, 2);

  // Ableitungen nach den Krümmungen c_i
  i3 = d_c_x->size[0] * d_c_x->size[1];
  d_c_x->size[0] = r->size[0];
  d_c_x->size[1] = r->size[0];
  emxEnsureCapacity((emxArray__common *)d_c_x, i3, (int)sizeof(double));
  loop_ub = r->size[0] * r->size[0];
  for (i3 = 0; i3 < loop_ub; i3++) {
    d_c_x->data[i3] = 0.0;
  }

  emxInit_real_T(&d_c_y, 2);

  // x-Komponenten
  i3 = d_c_y->size[0] * d_c_y->size[1];
  d_c_y->size[0] = r->size[0];
  d_c_y->size[1] = r->size[0];
  emxEnsureCapacity((emxArray__common *)d_c_y, i3, (int)sizeof(double));
  loop_ub = r->size[0] * r->size[0];
  for (i3 = 0; i3 < loop_ub; i3++) {
    d_c_y->data[i3] = 0.0;
  }

  // y-Komponenten
  for (p = 2; p - 2 <= num_points; p++) {
    for (c = 2; c - 2 <= num_points; c++) {
      if (1 + p > c) {
        d_c_x->data[p + d_c_x->size[0] * (c - 1)] = d_c_x->data[(p + d_c_x->
          size[0] * (c - 1)) - 1] - delta * delta * sin(P->data[p + (P->size[0] <<
          1)]) / sqrt(1.0 - delta * delta * (r->data[c] * r->data[c]) / 4.0);
        d_c_y->data[p + d_c_y->size[0] * (c - 1)] = d_c_y->data[(p + d_c_y->
          size[0] * (c - 1)) - 1] + delta * delta * cos(P->data[p + (P->size[0] <<
          1)]) / sqrt(1.0 - delta * delta * (r->data[c] * r->data[c]) / 4.0);
      }
    }
  }

  // Messmatrix zusammensetzen
  i3 = H->size[0] * H->size[1];
  H->size[0] = (int)(2.0 * (double)lambda->size[0]);
  H->size[1] = r->size[0];
  emxEnsureCapacity((emxArray__common *)H, i3, (int)sizeof(double));
  loop_ub = (int)(2.0 * (double)lambda->size[0]) * r->size[0];
  for (i3 = 0; i3 < loop_ub; i3++) {
    H->data[i3] = 0.0;
  }

  count = 2U;
  for (i = 0; i < ind->size[0]; i++) {
    // alle Messpunkte iterieren
    H->data[(int)count - 2] = 0.0;

    // Ableitung nach y0
    H->data[(int)count - 1] = 1.0;
    H->data[((int)count + H->size[0]) - 2] = d_phi0->data[(int)ind->data[i] - 1];

    // Ableitung nach phi0
    H->data[((int)count + H->size[0]) - 1] = d_phi0->data[((int)ind->data[i] +
      d_phi0->size[0]) - 1];
    for (c = 0; c < (int)(ind->data[i] + -2.0); c++) {
      // Ableitung nach Krümmungen c
      //          if c==ind(i)
      //              H(count, c)   = d_c_x(ind(i)-1, c-1) - lambda(i)*d^2*sin(P(ind(i), 3))/sqrt(1-d^2*r(c-1)^2/4); 
      //              H(count+1, c) = d_c_y(ind(i)-1, c-1) + lambda(i)*d^2*cos(P(ind(i), 3))/sqrt(1-d^2*r(c-1)^2/4); 
      //          else
      //              H(count, c)   = d_c_x(ind(i), c-1);
      //              H(count+1, c) = d_c_y(ind(i), c-1);
      //          end
      if (3.0 + (double)c == ind->data[i]) {
        H->data[((int)count + H->size[0] * (c + 2)) - 2] = d_c_x->data[((int)
          ind->data[i] + d_c_x->size[0] * ((int)((3.0 + (double)c) - 1.0) - 1))
          - 1] - lambda->data[i] * (delta * delta) * sin(P->data[((int)
          (ind->data[i] + 1.0) + (P->size[0] << 1)) - 1]) / sqrt(1.0 - delta *
          delta * (r->data[(int)((3.0 + (double)c) - 1.0) - 1] * r->data[(int)
                   ((3.0 + (double)c) - 1.0) - 1]) / 4.0);
        H->data[((int)count + H->size[0] * (c + 2)) - 1] = d_c_y->data[((int)
          ind->data[i] + d_c_y->size[0] * ((int)((3.0 + (double)c) - 1.0) - 1))
          - 1] + lambda->data[i] * (delta * delta) * cos(P->data[((int)
          (ind->data[i] + 1.0) + (P->size[0] << 1)) - 1]) / sqrt(1.0 - delta *
          delta * (r->data[(int)((3.0 + (double)c) - 1.0) - 1] * r->data[(int)
                   ((3.0 + (double)c) - 1.0) - 1]) / 4.0);
      } else {
        H->data[((int)count + H->size[0] * (c + 2)) - 2] = d_c_x->data[((int)
          ind->data[i] + d_c_x->size[0] * ((int)((3.0 + (double)c) - 1.0) - 1))
          - 1];
        H->data[((int)count + H->size[0] * (c + 2)) - 1] = d_c_y->data[((int)
          ind->data[i] + d_c_y->size[0] * ((int)((3.0 + (double)c) - 1.0) - 1))
          - 1];
      }
    }

    count += 2U;
  }

  emxFree_real_T(&d_c_y);
  emxFree_real_T(&d_c_x);
  emxFree_real_T(&d_phi0);
}

//
// File trailer for messmatrix.cpp
//
// [EOF]
//
