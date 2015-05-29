/*
 * File: projectPoints_emxAPI.h
 *
 * MATLAB Coder version            : 2.7
 * C/C++ source code generated on  : 28-May-2015 16:56:25
 */

#ifndef __PROJECTPOINTS_EMXAPI_H__
#define __PROJECTPOINTS_EMXAPI_H__

/* Include Files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rtwtypes.h"
#include "projectPoints_types.h"

/* Function Declarations */
extern emxArray_real_T *emxCreateND_real_T(int numDimensions, int *size);
extern emxArray_real_T *emxCreateWrapperND_real_T(double *data, int
  numDimensions, int *size);
extern emxArray_real_T *emxCreateWrapper_real_T(double *data, int rows, int cols);
extern emxArray_real_T *emxCreate_real_T(int rows, int cols);
extern void emxDestroyArray_real_T(emxArray_real_T *emxArray);

#endif

/*
 * File trailer for projectPoints_emxAPI.h
 *
 * [EOF]
 */