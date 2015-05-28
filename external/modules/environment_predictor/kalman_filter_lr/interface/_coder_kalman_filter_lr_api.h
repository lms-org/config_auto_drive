/* 
 * File: _coder_kalman_filter_lr_api.h 
 *  
 * MATLAB Coder version            : 2.7 
 * C/C++ source code generated on  : 27-May-2015 19:40:39 
 */

#ifndef ___CODER_KALMAN_FILTER_LR_API_H__
#define ___CODER_KALMAN_FILTER_LR_API_H__
/* Include Files */ 
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"

/* Function Declarations */ 
extern void kalman_filter_lr_initialize(emlrtContext *aContext);
extern void kalman_filter_lr_terminate(void);
extern void kalman_filter_lr_atexit(void);
extern void kalman_filter_lr_api(const mxArray *prhs[10], const mxArray *plhs[2]);
extern void kalman_filter_lr_xil_terminate(void);
extern void kalman_filter_lr(real_T r[10], real_T A[100], real_T Pk[100], real_T Q[100], real_T R_fakt, real_T delta, real_T xl[10], real_T yl[10], real_T xr[10], real_T yr[10]);

#endif
/* 
 * File trailer for _coder_kalman_filter_lr_api.h 
 *  
 * [EOF] 
 */
