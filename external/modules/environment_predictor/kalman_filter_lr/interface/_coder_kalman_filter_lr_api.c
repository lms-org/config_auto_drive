/*
 * File: _coder_kalman_filter_lr_api.c
 *
 * MATLAB Coder version            : 2.7
 * C/C++ source code generated on  : 27-May-2015 19:40:39
 */

/* Include Files */
#include "_coder_kalman_filter_lr_api.h"

/* Function Declarations */
static real_T (*emlrt_marshallIn(const emlrtStack *sp, const mxArray *r, const
  char_T *identifier))[10];
static real_T (*b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId))[10];
static real_T (*c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *A, const
  char_T *identifier))[100];
static real_T (*d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId))[100];
static real_T e_emlrt_marshallIn(const emlrtStack *sp, const mxArray *R_fakt,
  const char_T *identifier);
static real_T f_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u, const
  emlrtMsgIdentifier *parentId);
static void emlrt_marshallOut(const real_T u[10], const mxArray *y);
static void b_emlrt_marshallOut(const real_T u[100], const mxArray *y);
static real_T (*g_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId))[10];
static real_T (*h_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId))[100];
static real_T i_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src, const
  emlrtMsgIdentifier *msgId);

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
 * Arguments    : const mxArray *prhs[10]
 *                const mxArray *plhs[2]
 * Return Type  : void
 */
void kalman_filter_lr_api(const mxArray *prhs[10], const mxArray *plhs[2])
{
  real_T (*r)[10];
  real_T (*A)[100];
  real_T (*Pk)[100];
  real_T (*Q)[100];
  real_T R_fakt;
  real_T delta;
  real_T (*xl)[10];
  real_T (*yl)[10];
  real_T (*xr)[10];
  real_T (*yr)[10];
  emlrtStack st = { NULL, NULL, NULL };

  st.tls = emlrtRootTLSGlobal;
  prhs[0] = emlrtProtectR2012b(prhs[0], 0, true, -1);
  prhs[1] = emlrtProtectR2012b(prhs[1], 1, false, -1);
  prhs[2] = emlrtProtectR2012b(prhs[2], 2, true, -1);
  prhs[3] = emlrtProtectR2012b(prhs[3], 3, false, -1);
  prhs[6] = emlrtProtectR2012b(prhs[6], 6, false, -1);
  prhs[7] = emlrtProtectR2012b(prhs[7], 7, false, -1);
  prhs[8] = emlrtProtectR2012b(prhs[8], 8, false, -1);
  prhs[9] = emlrtProtectR2012b(prhs[9], 9, false, -1);

  /* Marshall function inputs */
  r = emlrt_marshallIn(&st, emlrtAlias(prhs[0]), "r");
  A = c_emlrt_marshallIn(&st, emlrtAlias(prhs[1]), "A");
  Pk = c_emlrt_marshallIn(&st, emlrtAlias(prhs[2]), "Pk");
  Q = c_emlrt_marshallIn(&st, emlrtAlias(prhs[3]), "Q");
  R_fakt = e_emlrt_marshallIn(&st, emlrtAliasP(prhs[4]), "R_fakt");
  delta = e_emlrt_marshallIn(&st, emlrtAliasP(prhs[5]), "delta");
  xl = emlrt_marshallIn(&st, emlrtAlias(prhs[6]), "xl");
  yl = emlrt_marshallIn(&st, emlrtAlias(prhs[7]), "yl");
  xr = emlrt_marshallIn(&st, emlrtAlias(prhs[8]), "xr");
  yr = emlrt_marshallIn(&st, emlrtAlias(prhs[9]), "yr");

  /* Invoke the target function */
  kalman_filter_lr(*r, *A, *Pk, *Q, R_fakt, delta, *xl, *yl, *xr, *yr);

  /* Marshall function outputs */
  emlrt_marshallOut(*r, prhs[0]);
  plhs[0] = prhs[0];
  b_emlrt_marshallOut(*Pk, prhs[2]);
  plhs[1] = prhs[2];
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *r
 *                const char_T *identifier
 * Return Type  : real_T (*)[10]
 */
static real_T (*emlrt_marshallIn(const emlrtStack *sp, const mxArray *r, const
  char_T *identifier))[10]
{
  real_T (*y)[10];
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  y = b_emlrt_marshallIn(sp, emlrtAlias(r), &thisId);
  emlrtDestroyArray(&r);
  return y;
}
/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 * Return Type  : real_T (*)[10]
 */
  static real_T (*b_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
  const emlrtMsgIdentifier *parentId))[10]
{
  real_T (*y)[10];
  y = g_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *A
 *                const char_T *identifier
 * Return Type  : real_T (*)[100]
 */
static real_T (*c_emlrt_marshallIn(const emlrtStack *sp, const mxArray *A, const
  char_T *identifier))[100]
{
  real_T (*y)[100];
  emlrtMsgIdentifier thisId;
  thisId.fIdentifier = identifier;
  thisId.fParent = NULL;
  y = d_emlrt_marshallIn(sp, emlrtAlias(A), &thisId);
  emlrtDestroyArray(&A);
  return y;
}
/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *u
 *                const emlrtMsgIdentifier *parentId
 * Return Type  : real_T (*)[100]
 */
  static real_T (*d_emlrt_marshallIn(const emlrtStack *sp, const mxArray *u,
  const emlrtMsgIdentifier *parentId))[100]
{
  real_T (*y)[100];
  y = h_emlrt_marshallIn(sp, emlrtAlias(u), parentId);
  emlrtDestroyArray(&u);
  return y;
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
 * Arguments    : const real_T u[10]
 *                const mxArray *y
 * Return Type  : void
 */
static void emlrt_marshallOut(const real_T u[10], const mxArray *y)
{
  static const int32_T iv0[2] = { 1, 10 };

  mxSetData((mxArray *)y, (void *)u);
  emlrtSetDimensions((mxArray *)y, iv0, 2);
}

/*
 * Arguments    : const real_T u[100]
 *                const mxArray *y
 * Return Type  : void
 */
static void b_emlrt_marshallOut(const real_T u[100], const mxArray *y)
{
  static const int32_T iv1[2] = { 10, 10 };

  mxSetData((mxArray *)y, (void *)u);
  emlrtSetDimensions((mxArray *)y, iv1, 2);
}

/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 * Return Type  : real_T (*)[10]
 */
static real_T (*g_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId))[10]
{
  real_T (*ret)[10];
  int32_T iv2[2];
  int32_T i0;
  for (i0 = 0; i0 < 2; i0++) {
    iv2[i0] = 1 + 9 * i0;
  }

  emlrtCheckBuiltInR2012b(sp, msgId, src, "double", false, 2U, iv2);
  ret = (real_T (*)[10])mxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
}
/*
 * Arguments    : const emlrtStack *sp
 *                const mxArray *src
 *                const emlrtMsgIdentifier *msgId
 * Return Type  : real_T (*)[100]
 */
  static real_T (*h_emlrt_marshallIn(const emlrtStack *sp, const mxArray *src,
  const emlrtMsgIdentifier *msgId))[100]
{
  real_T (*ret)[100];
  int32_T iv3[2];
  int32_T i;
  for (i = 0; i < 2; i++) {
    iv3[i] = 10;
  }

  emlrtCheckBuiltInR2012b(sp, msgId, src, "double", false, 2U, iv3);
  ret = (real_T (*)[100])mxGetData(src);
  emlrtDestroyArray(&src);
  return ret;
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
 * File trailer for _coder_kalman_filter_lr_api.c
 *
 * [EOF]
 */
