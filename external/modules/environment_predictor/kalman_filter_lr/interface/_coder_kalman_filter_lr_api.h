/* 
 * File: _coder_kalman_filter_lr_api.h 
 *  
 * MATLAB Coder version            : 2.7 
 * C/C++ source code generated on  : 26-Jun-2015 19:42:29 
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
#endif /*struct_emxArray_real_T*/
#ifndef typedef_emxArray_real_T
#define typedef_emxArray_real_T
typedef struct emxArray_real_T emxArray_real_T;
#endif /*typedef_emxArray_real_T*/

/* Function Declarations */ 
extern void kalman_filter_lr_initialize(emlrtContext *aContext);
extern void kalman_filter_lr_terminate(void);
extern void kalman_filter_lr_atexit(void);
extern void kalman_filter_lr_api(const mxArray *prhs[14], const mxArray *plhs[2]);
extern void kalman_filter_lr_xil_terminate(void);
extern void kalman_filter_lr(emxArray_real_T *r, real_T delta_x, real_T delta_y, real_T delta_phi, emxArray_real_T *Pk, emxArray_real_T *Q, real_T R_fakt, real_T delta, emxArray_real_T *xl, emxArray_real_T *yl, emxArray_real_T *xr, emxArray_real_T *yr, emxArray_real_T *xm, emxArray_real_T *ym);

#endif
/* 
 * File trailer for _coder_kalman_filter_lr_api.h 
 *  
 * [EOF] 
 */
