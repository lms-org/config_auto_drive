/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_kalman_filter_lr_api.h
 *
 * MATLAB Coder version            : 3.0
 * C/C++ source code generated on  : 29-Nov-2015 11:30:09
 */

#ifndef ___CODER_KALMAN_FILTER_LR_API_H__
#define ___CODER_KALMAN_FILTER_LR_API_H__

/* Include Files */
#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"
#include <stddef.h>
#include <stdlib.h>
#include "_coder_kalman_filter_lr_api.h"

/* Type Definitions */
#ifndef struct_emxArray_real_T
#define struct_emxArray_real_T

struct emxArray_real_T
{
  real_T *data;
  int32_T *size;
  int32_T allocatedSize;
  int32_T numDimensions;
  boolean_T canFreeData;
};

#endif                                 /*struct_emxArray_real_T*/

#ifndef typedef_emxArray_real_T
#define typedef_emxArray_real_T

typedef struct emxArray_real_T emxArray_real_T;

#endif                                 /*typedef_emxArray_real_T*/

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

/* Function Declarations */
extern void kalman_filter_lr(emxArray_real_T *r, real_T delta_x, real_T delta_y,
  real_T delta_phi, emxArray_real_T *Pk, emxArray_real_T *Q, real_T R_fakt,
  real_T delta, emxArray_real_T *xl, emxArray_real_T *yl, emxArray_real_T *xr,
  emxArray_real_T *yr, emxArray_real_T *xm, emxArray_real_T *ym, uint8_T
  interp_mode, real_T prior_fact);
extern void kalman_filter_lr_api(const mxArray *prhs[16], const mxArray *plhs[2]);
extern void kalman_filter_lr_atexit(void);
extern void kalman_filter_lr_initialize(void);
extern void kalman_filter_lr_terminate(void);
extern void kalman_filter_lr_xil_terminate(void);

#endif

/*
 * File trailer for _coder_kalman_filter_lr_api.h
 *
 * [EOF]
 */
