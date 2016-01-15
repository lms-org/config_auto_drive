//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: main.cpp
//
// MATLAB Coder version            : 3.0
// C/C++ source code generated on  : 29-Nov-2015 11:30:09
//

//***********************************************************************
// This automatically generated example C main file shows how to call
// entry-point functions that MATLAB Coder generated. You must customize
// this file for your application. Do not modify this file directly.
// Instead, make a copy of this file, modify it, and integrate it into
// your development environment.
//
// This file initializes entry-point function arguments to a default
// size and value before calling the entry-point functions. It does
// not store or use any values returned from the entry-point functions.
// If necessary, it does pre-allocate memory for returned values.
// You can use this file as a starting point for a main function that
// you can deploy in your application.
//
// After you copy the file, and before you deploy it, you must make the
// following changes:
// * For variable-size function arguments, change the example sizes to
// the sizes that your application requires.
// * Change the example values of function arguments to the values that
// your application requires.
// * If the entry-point functions return values, store these values or
// otherwise use them as required by your application.
//
//***********************************************************************
// Include Files
#include "rt_nonfinite.h"
#include "kalman_filter_lr.h"
#include "main.h"
#include "kalman_filter_lr_terminate.h"
#include "kalman_filter_lr_emxAPI.h"
#include "kalman_filter_lr_initialize.h"

// Function Declarations
static emxArray_real_T *argInit_Unboundedx1_real_T();
static double argInit_real_T();
static unsigned char argInit_uint8_T();
static emxArray_real_T *c_argInit_UnboundedxUnbounded_r();
static void main_kalman_filter_lr();

// Function Definitions

//
// Arguments    : void
// Return Type  : emxArray_real_T *
//
static emxArray_real_T *argInit_Unboundedx1_real_T()
{
  emxArray_real_T *result;
  static int iv0[1] = { 2 };

  int idx0;

  // Set the size of the array.
  // Change this size to the value that the application requires.
  result = emxCreateND_real_T(1, *(int (*)[1])&iv0[0]);

  // Loop over the array to initialize each element.
  for (idx0 = 0; idx0 < result->size[0U]; idx0++) {
    // Set the value of the array element.
    // Change this value to the value that the application requires.
    result->data[idx0] = argInit_real_T();
  }

  return result;
}

//
// Arguments    : void
// Return Type  : double
//
static double argInit_real_T()
{
  return 0.0;
}

//
// Arguments    : void
// Return Type  : unsigned char
//
static unsigned char argInit_uint8_T()
{
  return 0;
}

//
// Arguments    : void
// Return Type  : emxArray_real_T *
//
static emxArray_real_T *c_argInit_UnboundedxUnbounded_r()
{
  emxArray_real_T *result;
  static int iv1[2] = { 2, 2 };

  int idx0;
  int idx1;

  // Set the size of the array.
  // Change this size to the value that the application requires.
  result = emxCreateND_real_T(2, *(int (*)[2])&iv1[0]);

  // Loop over the array to initialize each element.
  for (idx0 = 0; idx0 < result->size[0U]; idx0++) {
    for (idx1 = 0; idx1 < result->size[1U]; idx1++) {
      // Set the value of the array element.
      // Change this value to the value that the application requires.
      result->data[idx0 + result->size[0] * idx1] = argInit_real_T();
    }
  }

  return result;
}

//
// Arguments    : void
// Return Type  : void
//
static void main_kalman_filter_lr()
{
  emxArray_real_T *r;
  double delta_x;
  double delta_y;
  double delta_phi;
  emxArray_real_T *Pk;
  emxArray_real_T *Q;
  double R_fakt;
  double delta;
  emxArray_real_T *xl;
  emxArray_real_T *yl;
  emxArray_real_T *xr;
  emxArray_real_T *yr;
  emxArray_real_T *xm;
  emxArray_real_T *ym;

  // Initialize function 'kalman_filter_lr' input arguments.
  // Initialize function input argument 'r'.
  r = argInit_Unboundedx1_real_T();
  delta_x = argInit_real_T();
  delta_y = argInit_real_T();
  delta_phi = argInit_real_T();

  // Initialize function input argument 'Pk'.
  Pk = c_argInit_UnboundedxUnbounded_r();

  // Initialize function input argument 'Q'.
  Q = c_argInit_UnboundedxUnbounded_r();
  R_fakt = argInit_real_T();
  delta = argInit_real_T();

  // Initialize function input argument 'xl'.
  xl = argInit_Unboundedx1_real_T();

  // Initialize function input argument 'yl'.
  yl = argInit_Unboundedx1_real_T();

  // Initialize function input argument 'xr'.
  xr = argInit_Unboundedx1_real_T();

  // Initialize function input argument 'yr'.
  yr = argInit_Unboundedx1_real_T();

  // Initialize function input argument 'xm'.
  xm = argInit_Unboundedx1_real_T();

  // Initialize function input argument 'ym'.
  ym = argInit_Unboundedx1_real_T();

  // Call the entry-point 'kalman_filter_lr'.
  kalman_filter_lr(r, delta_x, delta_y, delta_phi, Pk, Q, R_fakt, delta, xl, yl,
                   xr, yr, xm, ym, argInit_uint8_T(), argInit_real_T());
  emxDestroyArray_real_T(ym);
  emxDestroyArray_real_T(xm);
  emxDestroyArray_real_T(yr);
  emxDestroyArray_real_T(xr);
  emxDestroyArray_real_T(yl);
  emxDestroyArray_real_T(xl);
  emxDestroyArray_real_T(Q);
  emxDestroyArray_real_T(Pk);
  emxDestroyArray_real_T(r);
}

//
// Arguments    : int argc
//                const char * const argv[]
// Return Type  : int
//
int main(int, const char * const [])
{
  // Initialize the application.
  // You do not need to do this more than one time.
  kalman_filter_lr_initialize();

  // Invoke the entry-point functions.
  // You can call entry-point functions multiple times.
  main_kalman_filter_lr();

  // Terminate the application.
  // You do not need to do this more than one time.
  kalman_filter_lr_terminate();
  return 0;
}

//
// File trailer for main.cpp
//
// [EOF]
//
