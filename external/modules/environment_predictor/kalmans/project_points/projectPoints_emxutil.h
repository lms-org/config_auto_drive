/*
 * File: projectPoints_emxutil.h
 *
 * MATLAB Coder version            : 2.7
 * C/C++ source code generated on  : 28-May-2015 16:56:25
 */

#ifndef __PROJECTPOINTS_EMXUTIL_H__
#define __PROJECTPOINTS_EMXUTIL_H__

/* Include Files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rtwtypes.h"
#include "projectPoints_types.h"

/* Function Declarations */
extern void b_emxInit_real_T(emxArray_real_T **pEmxArray, int numDimensions);
extern void emxEnsureCapacity(emxArray__common *emxArray, int oldNumel, int
  elementSize);
extern void emxFree_real_T(emxArray_real_T **pEmxArray);
extern void emxInit_real_T(emxArray_real_T **pEmxArray, int numDimensions);

#endif

/*
 * File trailer for projectPoints_emxutil.h
 *
 * [EOF]
 */
