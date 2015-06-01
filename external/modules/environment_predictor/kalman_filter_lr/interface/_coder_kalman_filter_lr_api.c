/*
 * File: _coder_kalman_filter_lr_api.c
 *
 * MATLAB Coder version            : 2.7
 * C/C++ source code generated on  : 01-Jun-2015 15:11:47
 */

/* Include Files */
#include "_coder_kalman_filter_lr_api.h"

/* Function Declarations */
static void emlrt_marshallIn(const emlrtStack *sp, const mxArray *r, const
  char_T *identifier, emxArray_real_T *y);
static void b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, emxArray_real_T *y);
static void c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *A, const
  char_T *identifier, emxArray_real_T *y);
static void d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, emxArray_real_T *y);
static real_T e_emlrt_marshallIn(const emlrtStack *sp, const mxArray *R_fakt,
  const char_T *identifier);
static real_T f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId);
static void emlrt_marshallOut(const emxArray_real_T *u, const mxArray *y);
static void b_emlrt_marshallOut(const emxArray_real_T *u, const mxArray *y);
static void g_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, emxArray_real_T *ret);
static void h_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId, emxArray_real_T *ret);
static real_T i_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId);
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
 * Arguments    : const mxArray *prhs[12]
 *                const mxArray *plhs[2]
 * Return Type  : void
 */
void kalman_filter_lr_api(const mxArray *prhs[12], const mxArray *plhs[2])
{
  emxArray_real_T *r;
  emxArray_real_T *A;
  emxArray_real_T *Pk;
  emxArray_real_T *Q;
  emxArray_real_T *xl;
  emxArray_real_T *yl;
  emxArray_real_T *xr;
  emxArray_real_T *yr;
  emxArray_real_T *xm;
  emxArray_real_T *ym;
  real_T R_fakt;
  real_T delta;
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;
  emlrtHeapReferenceStackEnterFcnR2012b(&st);
  emxInit_real_T(&st, &r, 1, true);
  b_emxInit_real_T(&st, &A, 2, true);
  b_emxInit_real_T(&st, &Pk, 2, true);
  b_emxInit_real_T(&st, &Q, 2, true);
  emxInit_real_T(&st, &xl, 1, true);
  emxInit_real_T(&st, &yl, 1, true);
  emxInit_real_T(&st, &xr, 1, true);
  emxInit_real_T(&st, &yr, 1, true);
  emxInit_real_T(&st, &xm, 1, true);
  emxInit_real_T(&st, &ym, 1, true);
  prhs[0] = emlrtProtectR2012b(prhs[0], 0, true, -1);
  prhs[1] = emlrtProtectR2012b(prhs[1], 1, false, -1);
  prhs[2] = emlrtProtectR2012b(prhs[2], 2, true, -1);
  prhs[3] = emlrtProtectR2012b(prhs[3], 3, false, -1);
  prhs[6] = emlrtProtectR2012b(prhs[6], 6, false, -1);
  prhs[7] = emlrtProtectR2012b(prhs[7], 7, false, -1);
  prhs[8] = emlrtProtectR2012b(prhs[8], 8, false, -1);
  prhs[9] = emlrtProtectR2012b(prhs[9], 9, false, -1);
  prhs[10] = emlrtProtectR2012b(prhs[10], 10, false, -1);
  prhs[11] = emlrtProtectR2012b(prhs[11], 11, false, -1);

  /* Marshall function inputs */
  emlrt_marshallIn(&st, emlrtAlias(prhs[0]), "r", r);
  c_emlrt_marshallIn(&st, emlrtAlias(prhs[1]), "A", A);
  c_emlrt_marshallIn(&st, emlrtAlias(prhs[2]), "Pk", Pk);
  c_emlrt_marshallIn(&st, emlrtAlias(prhs[3]), "Q", Q);
  R_fakt = e_emlrt_marshallIn(&st, emlrtAliasP(prhs[4]), "R_fakt");
  delta = e_emlrt_marshallIn(&st, emlrtAliasP(prhs[5]), "delta");
  emlrt_marshallIn(&st, emlrtAlias(prhs[6]), "xl", xl);
  emlrt_marshallIn(&st, emlrtAlias(prhs[7]), "yl", yl);
  emlrt_marshallIn(&st, emlrtAlias(prhs[8]), "xr", xr);
  emlrt_marshallIn(&st, emlrtAlias(prhs[9]), "yr", yr);
  emlrt_marshallIn(&st, emlrtAlias(prhs[10]), "xm", xm);
  emlrt_marshallIn(&st, emlrtAlias(prhs[11]), "ym", ym);

  /* Invoke the target function */
  kalman_filter_lr(r, A, Pk, Q, R_fakt, delta, xl, yl, xr, yr, xm, ym);

  /* Marshall function outputs */
  emlrt_marshallOut(r, prhs[0]);
  plhs[0] = prhs[0];
  b_emlrt_marshallOut(Pk, prhs[2]);
  plhs[1] = prhs[2];
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
  A->canFreeData = false;
  emxFree_real_T(&A);
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
 *                const mxArray *A
 *                const char_T *identifier
 *                emxArray_real_T *y
 * Return Type  : void
 */
static void c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *A, const
  char_T *identifier, emxArray_real_T *y)
{
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  d_emlrt_marshallIn(sp, emlrtAlias(A), &thisId, y);
  emlrtDestroyArray(&A);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 *                emxArray_real_T *y
 * Return Type  : void
 */
static void d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId, emxArray_real_T *y)
{
  h_emlrt_marshallIn(sp, emlrtAlias(u), parentId, y);
  emlrtDestroyArray(&u);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *R_fakt
 *                const char_T *identifier
 * Return Type  : real_T
 */
static real_T e_emlrt_marshallIn(const emlrtStack *sp, const mxArray *R_fakt,
  const char_T *identifier)
{
  real_T y;
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  y = f_emlrt_marshallIn(sp, emlrtAlias(R_fakt), &thisId);
  emlrtDestroyArray(&R_fakt);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 * Return Type  : real_T
 */
static real_T f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId)
{
  real_T y;
  y = i_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
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
 *                emxArray_real_T *ret
 * Return Type  : void
 */
static void h_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
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
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 * Return Type  : real_T
 */
static real_T i_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
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
