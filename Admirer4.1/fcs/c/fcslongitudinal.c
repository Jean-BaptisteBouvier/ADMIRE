/***************************************/
/* Look-up mex-function for ADMIRE FCS */
/*                                     */
/*    Torbjorn Noren , FFA , 991126    */
/***************************************/
/*                                     */
/* INPUT                               */
/*   1. Altitude                       */
/*   2. Mach No.                       */
/*                                     */
/* OUTPUT                              */
/*   1. kalfa                          */
/*   2. ki                             */
/*   3. kp                             */
/*   4. kq                             */
/*   5. kvt                            */
/*                                     */
/***************************************/

#define S_FUNCTION_LEVEL 2
#define S_FUNCTION_NAME  fcslongitudinal

#define altitude		*u1[0]
#define machNo			*u2[0]

#define	kalfa_out		y1[0]
#define	ki_out			y2[0]
#define	kp_out			y3[0]
#define	kq_out			y4[0]
#define	kvt_out			y5[0]

#include <math.h>
#include <stdio.h>

#include "tmwtypes.h"
#include "simstruc.h"

#include "fcslongitudinal.h"


/* ============================================================================ */
/* ===== Function: lon_int ==================================================== */
/* ============================================================================ */
double lon_int(double x1, double x2,double y1, double y2,
			   double z11, double z12, double z21, double z22,
			   double x, double y){

	double temp1, temp2;

	if (x<x1)
		x = x1;
	else if (x>x2)
		x = x2;

	if (y<y1)
		y = y1;
	else if (y>y2)
		y = y2;

	temp1 = z11 + (x-x1)*(z12-z11)/(x2-x1);
	temp2 = z21 + (x-x1)*(z22-z21)/(x2-x1);

	return temp1 + (y-y1)*(temp2-temp1)/(y2-y1);
}

/* ============================================================================ */
/* ===== Function: mdlInitializeSizes ========================================= */
/* ============================================================================ */
static void mdlInitializeSizes(SimStruct *S) {
    
    ssSetNumSFcnParams(S,0);				/* Number of expected parameters */
	ssSetNumContStates(S,0);				/* Number of states */
    ssSetNumDiscStates(S,0);

	ssSetNumInputPorts(S,2);				/* Number of input ports */
    ssSetInputPortWidth(S,0,1);				/* Widht of input port no 1 */
	ssSetInputPortDirectFeedThrough(S,0,1);
    ssSetInputPortWidth(S,1,1);				/* Widht of input port no 2 */
	ssSetInputPortDirectFeedThrough(S,1,1);

    ssSetNumOutputPorts(S,5);				/* Number of output ports  */
    ssSetOutputPortWidth(S,0,1);			/* Widht of output port 1  */
	ssSetOutputPortWidth(S,1,1);			/* Widht of output port 2  */
	ssSetOutputPortWidth(S,2,1);			/* Widht of output port 3  */
	ssSetOutputPortWidth(S,3,1);			/* Widht of output port 4  */
	ssSetOutputPortWidth(S,4,1);			/* Widht of output port 5  */

	ssSetNumSampleTimes(S,1);				/* Number of sample times */

    ssSetNumRWork(S,0);						/* Size of real work vector */
    ssSetNumIWork(S,0);						/* Size of integer work vector */
    ssSetNumPWork(S,0);
    ssSetNumModes(S,0);
    ssSetNumNonsampledZCs(S,0);

	ssSetOptions(S,0);
}

/* ============================================================================ */
/* ===== Function: mdlInitializeSampleTimes =================================== */
/* ============================================================================ */
static void mdlInitializeSampleTimes(SimStruct *S) {
    ssSetSampleTime(S,0,CONTINUOUS_SAMPLE_TIME);
    ssSetOffsetTime(S,0,0.0);
}

/* ============================================================================ */
/* ===== Function: mdlInitializeConditions ==================================== */
/* ============================================================================ */
#define MDL_INITIALIZE_CONDITIONS   /* Change to #undef to remove function */
#if defined(MDL_INITIALIZE_CONDITIONS)
static void mdlInitializeConditions(SimStruct *S) {

	if (ssGetNumSFcnParams(S) != 0) {		/* Check no. of input parameters */
		ssSetErrorStatus(S,"Wrong number of functions parameters");
		return;
	}

}
#endif

/* ============================================================================ */
/* ===== Function: mdlStart =================================================== */
/* ============================================================================ */
#define MDL_START  /* Change to #undef to remove function */
#if defined(MDL_START)
static void mdlStart(SimStruct *S) {

	InputRealPtrsType u1 = ssGetInputPortRealSignalPtrs(S,0);
	InputRealPtrsType u2 = ssGetInputPortRealSignalPtrs(S,1);

	real_T *y1 = ssGetOutputPortRealSignal(S,0);
	real_T *y2 = ssGetOutputPortRealSignal(S,1);
	real_T *y3 = ssGetOutputPortRealSignal(S,2);
	real_T *y4 = ssGetOutputPortRealSignal(S,3);
	real_T *y5 = ssGetOutputPortRealSignal(S,4);

	int i,a,b;

	/* Look for nearest x-vector index OVER wanted value */
	i=1;
	if (mach_lon[0]>machNo) {
		b=i; i=m_lon; }
	else {
		while (i<m_lon)	{
			if ((mach_lon[i]>machNo) || (i==m_lon-1)) {
				b=i; i=m_lon; }
			else i++; } }

	/* Look for nearest y-vector index OVER wanted value */
	i=1;
	if (alt_lon[0]>altitude) {
		a=i; i=n_lon; }
	else {
		while (i<n_lon)	{
			if ((alt_lon[i]>altitude) || (i==n_lon-1)) {
				a=i; i=n_lon; }
			else i++; } }

	/* Perform linear interpolation for 'kalfa' */
	kalfa_out = lon_int(mach_lon[b-1],mach_lon[b],alt_lon[a-1],alt_lon[a],
		kalfa[a-1][b-1],kalfa[a-1][b],kalfa[a][b-1],kalfa[a][b],machNo,altitude);

	/* Perform linear interpolation for 'ki' */
	ki_out = lon_int(mach_lon[b-1],mach_lon[b],alt_lon[a-1],alt_lon[a],
		ki[a-1][b-1],ki[a-1][b],ki[a][b-1],ki[a][b],machNo,altitude);

	/* Perform linear interpolation for 'kp' */
	kp_out = lon_int(mach_lon[b-1],mach_lon[b],alt_lon[a-1],alt_lon[a],
		kp[a-1][b-1],kp[a-1][b],kp[a][b-1],kp[a][b],machNo,altitude);

	/* Perform linear interpolation for 'kq' */
	kq_out = lon_int(mach_lon[b-1],mach_lon[b],alt_lon[a-1],alt_lon[a],
		kq[a-1][b-1],kq[a-1][b],kq[a][b-1],kq[a][b],machNo,altitude);

	/* Perform linear interpolation for 'kvt' */
	kvt_out = lon_int(mach_lon[b-1],mach_lon[b],alt_lon[a-1],alt_lon[a],
		kvt[a-1][b-1],kvt[a-1][b],kvt[a][b-1],kvt[a][b],machNo,altitude);

}
#endif

/* ============================================================================ */
/* ===== Function: mdlOutputs ================================================= */
/* ============================================================================ */
/* ===== Abstract:                                                        ===== */
/* =====   In this function, you compute the outputs of your S-function   ===== */
/* =====   block. Generally outputs are placed in the output vector,      ===== */
/* =====   ssGetY(S).                                                     ===== */
/* ============================================================================ */
static void mdlOutputs(SimStruct *S, int_T tid) {

	InputRealPtrsType u1 = ssGetInputPortRealSignalPtrs(S,0);
	InputRealPtrsType u2 = ssGetInputPortRealSignalPtrs(S,1);

	real_T *y1 = ssGetOutputPortRealSignal(S,0);
	real_T *y2 = ssGetOutputPortRealSignal(S,1);
	real_T *y3 = ssGetOutputPortRealSignal(S,2);
	real_T *y4 = ssGetOutputPortRealSignal(S,3);
	real_T *y5 = ssGetOutputPortRealSignal(S,4);

	int i,a,b;

	/* Look for nearest x-vector index OVER wanted value */
	i=1;
	if (mach_lon[0]>machNo) {
		b=i; i=m_lon; }
	else {
		while (i<m_lon)	{
			if ((mach_lon[i]>machNo) || (i==m_lon-1)) {
				b=i; i=m_lon; }
			else i++; } }

	/* Look for nearest y-vector index OVER wanted value */
	i=1;
	if (alt_lon[0]>altitude) {
		a=i; i=n_lon; }
	else {
		while (i<n_lon)	{
			if ((alt_lon[i]>altitude) || (i==n_lon-1)) {
				a=i; i=n_lon; }
			else i++; } }

	/* Perform linear interpolation for 'kalfa' */
	kalfa_out = lon_int(mach_lon[b-1],mach_lon[b],alt_lon[a-1],alt_lon[a],
		kalfa[a-1][b-1],kalfa[a-1][b],kalfa[a][b-1],kalfa[a][b],machNo,altitude);

	/* Perform linear interpolation for 'ki' */
	ki_out = lon_int(mach_lon[b-1],mach_lon[b],alt_lon[a-1],alt_lon[a],
		ki[a-1][b-1],ki[a-1][b],ki[a][b-1],ki[a][b],machNo,altitude);

	/* Perform linear interpolation for 'kp' */
	kp_out = lon_int(mach_lon[b-1],mach_lon[b],alt_lon[a-1],alt_lon[a],
		kp[a-1][b-1],kp[a-1][b],kp[a][b-1],kp[a][b],machNo,altitude);

	/* Perform linear interpolation for 'kq' */
	kq_out = lon_int(mach_lon[b-1],mach_lon[b],alt_lon[a-1],alt_lon[a],
		kq[a-1][b-1],kq[a-1][b],kq[a][b-1],kq[a][b],machNo,altitude);

	/* Perform linear interpolation for 'kvt' */
	kvt_out = lon_int(mach_lon[b-1],mach_lon[b],alt_lon[a-1],alt_lon[a],
		kvt[a-1][b-1],kvt[a-1][b],kvt[a][b-1],kvt[a][b],machNo,altitude);
	
}


/* ============================================================================ */
/* ===== Function: mdlUpdate ================================================== */
/* ============================================================================ */
/* ===== Abstract:                                                        ===== */
/* =====   This function is called once for every major integration time  ===== */
/* =====   step. Discrete states are typically updated here, but this     ===== */
/* =====   function is useful for performing any tasks that should only   ===== */
/* =====   take place once per integration step.                          ===== */
/* ============================================================================ */
#undef MDL_UPDATE  /* Change to #undef to remove function */
#if defined(MDL_UPDATE)
static void mdlUpdate(SimStruct *S, int_T tid) {}
#endif


/* ============================================================================ */
/* ===== Function: mdlDerivatives ============================================= */
/* ============================================================================ */
/* ===== Abstract:                                                        ===== */
/* =====   In this function, you compute the S-function block's           ===== */
/* =====   derivatives. The derivatives are placed in the derivative      ===== */
/* =====   vector, ssGetdX(S).                                            ===== */
/* ============================================================================ */
#undef MDL_DERIVATIVES  /* Change to #undef to remove function */
#if defined(MDL_DERIVATIVES)
static void mdlDerivatives(SimStruct *S) {

}
#endif

/* ============================================================================ */
/* ===== Function: mdlTerminate =============================================== */
/* ============================================================================ */
/* ===== Abstract:                                                        ===== */
/* =====   In this function, you should perform any actions that are      ===== */
/* =====   necessary at the termination of a simulation.  For example, if ===== */
/* =====   memory was allocated in mdlInitializeConditions, this is the   ===== */
/* =====   place to free it.                                              ===== */
/* ============================================================================ */
static void mdlTerminate(SimStruct *S){}


/*======================================================*/
/* See sfuntmpl.doc for the optional S-function methods */
/*======================================================*/

/*=============================*/
/* Required S-function trailer */
/*=============================*/
#ifdef  MATLAB_MEX_FILE		/* Is this file being compiled as a MEX-file? */
	#include "simulink.c"	/* MEX-file interface mechanism				  */
#else
	#include "cg_sfun.h"	/* Code generation registration function	  */
#endif

