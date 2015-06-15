/*
 * File: _coder_kalman_filter_lr_api.c
 *
 * MATLAB Coder version            : 2.7
 * C/C++ source code generated on  : 15-Jun-2015 13:18:28
 */

/* Include Files */
#include "_coder_kalman_filter_lr_api.h"

/* Function Declarations */
static void emlrt_marshallIn(const emlrtStack *sp, const mxArray *r, const
  char_T *identifier, emxArray_real_T *y);
static void b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, emxArray_real_T *y);
static real_T c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *delta_x,
  const char_T *identifier);
static real_T d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId);
static void e_emlrt_marshallIn(const emlrtStack *sp, const mxArray *Pk, const
  char_T *identifier, emxArray_real_T *y);
static void f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, emxArray_real_T *y);
static void emlrt_marshallOut(const emxArray_real_T *u, const mxArray *y);
static void b_emlrt_marshallOut(const emxArray_real_T *u, const mxArray *y);
static void g_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, emxArray_real_T *ret);
static real_T h_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId);
static void i_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, emxArray_real_T *ret);
static void emxInit_real_T(const emlrtStack *sp, emxArray_real_T **pEmxArray,
  int32_T numDimensions, boolean_T doPush);
static void emxFree_real_T(emxArray_real_T **pEmxArray);
static void b_emxInit_real_T(const emlrtStack *sp, emxArray_real_T **pEmxArray,
  int32_T numDimensions, boolean_T doPush);

/* Function Definitions */

/*
 * Arguments    : emlrtContext *aContext
 * Return Type  : void
 */
void kalman_filter_lr_initialize(emlrtContext *aContext)
{
  emlrtStack st = { NULL, NULL, NULL };

  emlrtCreateRootTLS(&emlrtRootTLSGlobal, aContext, NULL, 1);
  st.tls = emlrtRootTLSGlobal;
  emlrtClearAllocCountR2012b(&st, false, 0U, 0);
  emlrtEnterRtStackR2012b(&st);
  emlrtFirstTimeR2012b(emlrtRootTLSGlobal);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void kalman_filter_lr_terminate(void)
{
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void kalman_filter_lr_atexit(void)
{
  emlrtStack st = { NULL, NULL, NULL };

  emlrtCreateRootTLS(&emlrtRootTLSGlobal, &emlrtContextGlobal, NULL, 1);
  st.tls = emlrtRootTLSGlobal;
  emlrtEnterRtStackR2012b(&st);
  emlrtLeaveRtStackR2012b(&st);
  emlrtDestroyRootTLS(&emlrtRootTLSGlobal);
  kalman_filter_lr_xil_terminate();
}

/*
 * Arguments    : const mxArray *prhs[14]
 *                const mxArray *plhs[2]
 * Return Type  : void
 */
void kalman_filter_lr_api(const mxArray *prhs[14], const mxArray *plhs[2])
{
  emxArray_real_T *r;
  emxArray_real_T *Pk;
  emxArray_real_T *Q;
  emxArray_real_T *xl;
  emxArray_real_T *yl;
  emxArray_real_T *xr;
  emxArray_real_T *yr;
  emxArray_real_T *xm;
  emxArray_real_T *ym;
  real_T delta_x;
  real_T delta_y;
  real_T delta_phi;
  real_T R_fakt;
  real_T delta;
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;
  emlrtHeapReferenceStackEnterFcnR2012b(&st);
  emxInit_real_T(&st, &r, 1, true);
  b_emxInit_real_T(&st, &Pk, 2, true);
  b_emxInit_real_T(&st, &Q, 2, true);
  emxInit_real_T(&st, &xl, 1, true);
  emxInit_real_T(&st, &yl, 1, true);
  emxInit_real_T(&st, &xr, 1, true);
  emxInit_real_T(&st, &yr, 1, true);
  emxInit_real_T(&st, &xm, 1, true);
  emxInit_real_T(&st, &ym, 1, true);
  prhs[0] = emlrtProtectR2012b(prhs[0], 0, true, -1);
  prhs[4] = emlrtProtectR2012b(prhs[4], 4, true, -1);
  prhs[5] = emlrtProtectR2012b(prhs[5], 5, false, -1);
  prhs[8] = emlrtProtectR2012b(prhs[8], 8, false, -1);
  prhs[9] = emlrtProtectR2012b(prhs[9], 9, false, -1);
  prhs[10] = emlrtProtectR2012b(prhs[10], 10, false, -1);
  prhs[11] = emlrtProtectR2012b(prhs[11], 11, false, -1);
  prhs[12] = emlrtProtectR2012b(prhs[12], 12, false, -1);
  prhs[13] = emlrtProtectR2012b(prhs[13], 13, false, -1);

  /* Marshall function inputs */
  emlrt_marshallIn(&st, emlrtAlias(prhs[0]), "r", r);
  delta_x = c_emlrt_marshallIn(&st, emlrtAliasP(prhs[1]), "delta_x");
  delta_y = c_emlrt_marshallIn(&st, emlrtAliasP(prhs[2]), "delta_y");
  delta_phi = c_emlrt_marshallIn(&st, emlrtAliasP(prhs[3]), "delta_phi");
  e_emlrt_marshallIn(&st, emlrtAlias(prhs[4]), "Pk", Pk);
  e_emlrt_marshallIn(&st, emlrtAlias(prhs[5]), "Q", Q);
  R_fakt = c_emlrt_marshallIn(&st, emlrtAliasP(prhs[6]), "R_fakt");
  delta = c_emlrt_marshallIn(&st, emlrtAliasP(prhs[7]), "delta");
  emlrt_marshallIn(&st, emlrtAlias(prhs[8]), "xl", xl);
  emlrt_marshallIn(&st, emlrtAlias(prhs[9]), "yl", yl);
  emlrt_marshallIn(&st, emlrtAlias(prhs[10]), "xr", xr);
  emlrt_marshallIn(&st, emlrtAlias(prhs[11]), "yr", yr);
  emlrt_marshallIn(&st, emlrtAlias(prhs[12]), "xm", xm);
  emlrt_marshallIn(&st, emlrtAlias(prhs[13]), "ym", ym);

  /* Invoke the target function */
  kalman_filter_lr(r, delta_x, delta_y, delta_phi, Pk, Q, R_fakt, delta, xl, yl,
                   xr, yr, xm, ym);

  /* Marshall function outputs */
  emlrt_marshallOut(r, prhs[0]);
  plhs[0] = prhs[0];
  b_emlrt_marshallOut(Pk, prhs[4]);
  plhs[1] = prhs[4];
  ym->canFreeData = false;
  emxFree_real_T(&ym);
  xm->canFreeData = false;
  emxFree_real_T(&xm);
  yr->canFreeData = false;
  emxFree_real_T(&yr);
  xr->canFreeData = false;
  emxFree_real_T(&xr);
  yl->canFreeData = false;
  emxFree_real_T(&yl);
  xl->canFreeData = false;
  emxFree_real_T(&xl);
  Q->canFreeData = false;
  emxFree_real_T(&Q);
  Pk->canFreeData = false;
  emxFree_real_T(&Pk);
  r->canFreeData = false;
  emxFree_real_T(&r);
  emlrtHeapReferenceStackLeaveFcnR2012b(&st);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *r
 *                const char_T *identifier
 *                emxArray_real_T *y
 * Return Type  : void
 */
static void emlrt_marshallIn(const emlrtStack *sp, const mxArray *r, const
  char_T *identifier, emxArray_real_T *y)
{
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  b_emlrt_marshallIn(sp, emlrtAlias(r), &thisId, y);
  emlrtDestroyArray(&r);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 *                emxArray_real_T *y
 * Return Type  : void
 */
static void b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, emxArray_real_T *y)
{
  g_emlrt_marshallIn(sp, emlrtAlias(u), parentId, y);
  emlrtDestroyArray(&u);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *delta_x
 *                const char_T *identifier
 * Return Type  : real_T
 */
static real_T c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *delta_x,
  const char_T *identifier)
{
  real_T y;
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  y = d_emlrt_marshallIn(sp, emlrtAlias(delta_x), &thisId);
  emlrtDestroyArray(&delta_x);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 * Return Type  : real_T
 */
static real_T d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId)
{
  real_T y;
  y = h_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *Pk
 *                const char_T *identifier
 *                emxArray_real_T *y
 * Return Type  : void
 */
static void e_emlrt_marshallIn(const emlrtStack *sp, const mxArray *Pk, const
  char_T *identifier, emxArray_real_T *y)
{
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  f_emlrt_marshallIn(sp, emlrtAlias(Pk), &thisId, y);
  emlrtDestroyArray(&Pk);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 *                emxArray_real_T *y
 * Return Type  : void
 */
static void f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, emxArray_real_T *y)
{
  i_emlrt_marshallIn(sp, emlrtAlias(u), parentId, y);
  emlrtDestroyArray(&u);
}

/*
 * Arguments    : const emxArray_real_T *u
 *                const mxArray *y
 * Return Type  : void
 */
static void emlrt_marshallOut(const emxArray_real_T *u, const mxArray *y)
{
  mxSetData((mxArray *)y, (void *)u->data);
  emlrtSetDimensions((mxArray *)y, u->size, 1);
}

/*
 * Arguments    : const emxArray_real_T *u
 *                const mxArray *y
 * Return Type  : void
 */
static void b_emlrt_marshallOut(const emxArray_real_T *u, const mxArray *y)
{
  mxSetData((mxArray *)y, (void *)u->data);
  emlrtSetDimensions((mxArray *)y, u->size, 2);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 *                emxArray_real_T *ret
 * Return Type  : void
 */
static void g_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, emxArray_real_T *ret)
{
  int32_T iv0[1];
  boolean_T bv0[1];
  int32_T iv1[1];
  iv0[0] = -1;
  bv0[0] = true;
  emlrtCheckVsBuiltInR2012b(sp, msgId, src, "double", false, 1U, iv0, bv0, iv1);
  ret->size[0] = iv1[0];
  ret->allocatedSize = ret->size[0];
  ret->data = (real_T *)mxGetData(src);
  ret->canFreeData = false;
  emlrtDestroyArray(&src);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 * Return Type  : real_T
 */
static real_T h_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId)
{
  real_T ret;
  emlrtCheckBuiltInR2012b(sp, msgId, src, "double", false, 0U, 0);
  ret = *(real_T *)mxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 *                emxArray_real_T *ret
 * Return Type  : void
 */
static void i_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, emxArray_real_T *ret)
{
  int32_T iv2[2];
  boolean_T bv1[2];
  int32_T i;
  int32_T iv3[2];
  for (i = 0; i < 2; i++) {
    iv2[i] = -1;
    bv1[i] = true;
  }

  emlrtCheckVsBuiltInR2012b(sp, msgId, src, "double", false, 2U, iv2, bv1, iv3);
  ret->size[0] = iv3[0];
  ret->size[1] = iv3[1];
  ret->allocatedSize = ret->size[0] * ret->size[1];
  ret->data = (real_T *)mxGetData(src);
  ret->canFreeData = false;
  emlrtDestroyArray(&src);
}

/*
 * Arguments    : const emlrtStack *sp
 *                emxArray_real_T **pEmxArray
 *                int32_T numDimensions
 *                boolean_T doPush
 * Return Type  : void
 */
static void emxInit_real_T(const emlrtStack *sp, emxArray_real_T **pEmxArray,
  int32_T numDimensions, boolean_T doPush)
{
  emxArray_real_T *emxArray;
  int32_T i;
  *pEmxArray = (emxArray_real_T *)emlrtMallocMex(sizeof(emxArray_real_T));
  if (doPush) {
    emlrtPushHeapReferenceStackR2012b(sp, (void *)pEmxArray, (void (*)(void *))
      emxFree_real_T);
  }

  emxArray = *pEmxArray;
  emxArray->data = (real_T *)NULL;
  emxArray->numDimensions = numDimensions;
  emxArray->size = (int32_T *)emlrtMallocMex((uint32_T)(sizeof(int32_T)
    * numDimensions));
  emxArray->allocatedSize = 0;
  emxArray->canFreeData = true;
  for (i = 0; i < numDimensions; i++) {
    emxArray->size[i] = 0;
  }
}

/*
 * Arguments    : emxArray_real_T **pEmxArray
 * Return Type  : void
 */
static void emxFree_real_T(emxArray_real_T **pEmxArray)
{
  if (*pEmxArray != (emxArray_real_T *)NULL) {
    if (((*pEmxArray)->data != (real_T *)NULL) && (*pEmxArray)->canFreeData) {
      emlrtFreeMex((void *)(*pEmxArray)->data);
    }

    emlrtFreeMex((void *)(*pEmxArray)->size);
    emlrtFreeMex((void *)*pEmxArray);
    *pEmxArray = (emxArray_real_T *)NULL;
  }
}

/*
 * Arguments    : const emlrtStack *sp
 *                emxArray_real_T **pEmxArray
 *                int32_T numDimensions
 *                boolean_T doPush
 * Return Type  : void
 */
static void b_emxInit_real_T(const emlrtStack *sp, emxArray_real_T **pEmxArray,
  int32_T numDimensions, boolean_T doPush)
{
  emxArray_real_T *emxArray;
  int32_T i;
  *pEmxArray = (emxArray_real_T *)emlrtMallocMex(sizeof(emxArray_real_T));
  if (doPush) {
    emlrtPushHeapReferenceStackR2012b(sp, (void *)pEmxArray, (void (*)(void *))
      emxFree_real_T);
  }

  emxArray = *pEmxArray;
  emxArray->data = (real_T *)NULL;
  emxArray->numDimensions = numDimensions;
  emxArray->size = (int32_T *)emlrtMallocMex((uint32_T)(sizeof(int32_T)
    * numDimensions));
  emxArray->allocatedSize = 0;
  emxArray->canFreeData = true;
  for (i = 0; i < numDimensions; i++) {
    emxArray->size[i] = 0;
  }
}

/*
 * File trailer for _coder_kalman_filter_lr_api.c
 *
 * [EOF]
 */
