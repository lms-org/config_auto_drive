//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: state_transition.h
//
// MATLAB Coder version            : 3.0
// C/C++ source code generated on  : 29-Nov-2015 11:30:09
//
#ifndef __STATE_TRANSITION_H__
#define __STATE_TRANSITION_H__

// Include Files
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "kalman_filter_lr_types.h"

// Function Declarations
extern void state_transition(emxArray_real_T *r, double delta, double delta_x,
  double delta_y, double delta_phi, unsigned char interp_mode, emxArray_real_T
  *A);

#endif

//
// File trailer for state_transition.h
//
// [EOF]
//
