/* 
 * File: _coder_projectPoints_api.h 
 *  
 * MATLAB Coder version            : 2.7 
 * C/C++ source code generated on  : 28-May-2015 16:56:25 
 */

#ifndef ___CODER_PROJECTPOINTS_API_H__
#define ___CODER_PROJECTPOINTS_API_H__
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
extern void projectPoints_initialize(emlrtContext *aContext);
extern void projectPoints_terminate(void);
extern void projectPoints_atexit(void);
extern void projectPoints_api(const mxArray *prhs[3], const mxArray *plhs[2]);
extern void projectPoints(emxArray_real_T *r, real_T delta, real_T dist, emxArray_real_T *xp, emxArray_real_T *yp);
extern void projectPoints_xil_terminate(void);

#endif
/* 
 * File trailer for _coder_projectPoints_api.h 
 *  
 * [EOF] 
 */
