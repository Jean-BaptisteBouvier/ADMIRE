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
/*   1. x0_01                          */
/*   2. x0_02                          */
/*   3. x0_03                          */
/*   4. x0_04                          */
/*   5. x0_05                          */
/*   6. x0_06                          */
/*   7. x0_07                          */
/*   8. x0_08                          */
/*   9. x0_09                          */
/*  10. x0_10                          */
/*  11. x0_11                          */
/*  12. x0_12                          */
/*                                     */
/***************************************/

#define S_FUNCTION_LEVEL 2
#define S_FUNCTION_NAME  fcsx0

#define altitude		*u1[0]
#define machNo			*u2[0]

#define	x0_01_out		y[0]
#define	x0_02_out		y[1]
#define	x0_03_out		y[2]
#define	x0_04_out		y[3]
#define	x0_05_out		y[4]
#define	x0_06_out		y[5]
#define	x0_07_out		y[6]
#define	x0_08_out		y[7]
#define	x0_09_out		y[8]
#define	x0_10_out		y[9]
#define	x0_11_out		y[10]
#define	x0_12_out		y[11]

#include <math.h>
#include <stdio.h>

#include "tmwtypes.h"
#include "simstruc.h"

#include "fcsx0.h"


/* ============================================================================ */
/* ===== Function: x0_int ================================================ */
/* ============================================================================ */
double x0_int(double x1, double x2,double y1, double y2,
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

    ssSetNumOutputPorts(S,1);				/* Number of output ports  */
    ssSetOutputPortWidth(S,0,12);			/* Widht of output port 1  */

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

	real_T *y = ssGetOutputPortRealSignal(S,0);

	int i,a,b;

	/* Look for nearest x-vector index OVER wanted value */
	i=1;
	if (mach_x0[0]>machNo) {
		b=i; i=m_x0; }
	else {
		while (i<m_x0)	{
			if ((mach_x0[i]>machNo) || (i==m_x0-1)) {
				b=i; i=m_x0; }
			else i++; } }

	/* Look for nearest y-vector index OVER wanted value */
	i=1;
	if (alt_x0[0]>altitude) {
		a=i; i=n_x0; }
	else {
		while (i<n_x0)	{
			if ((alt_x0[i]>altitude) || (i==n_x0-1)) {
				a=i; i=n_x0; }
			else i++; } }

	/* Perform linear interpolation for 'x0_01' */
	x0_01_out = x0_int(mach_x0[b-1],mach_x0[b],alt_x0[a-1],alt_x0[a],
		x0_01[a-1][b-1],x0_01[a-1][b],x0_01[a][b-1],x0_01[a][b],machNo,altitude);

	/* Perform linear interpolation for 'x0_02' */
	x0_02_out = x0_int(mach_x0[b-1],mach_x0[b],alt_x0[a-1],alt_x0[a],
		x0_02[a-1][b-1],x0_02[a-1][b],x0_02[a][b-1],x0_02[a][b],machNo,altitude);

	/* Perform linear interpolation for 'x0_03' */
	x0_03_out = x0_int(mach_x0[b-1],mach_x0[b],alt_x0[a-1],alt_x0[a],
		x0_03[a-1][b-1],x0_03[a-1][b],x0_03[a][b-1],x0_03[a][b],machNo,altitude);

	/* Perform linear interpolation for 'x0_04' */
	x0_04_out = x0_int(mach_x0[b-1],mach_x0[b],alt_x0[a-1],alt_x0[a],
		x0_04[a-1][b-1],x0_04[a-1][b],x0_04[a][b-1],x0_04[a][b],machNo,altitude);

	/* Perform linear interpolation for 'x0_05' */
	x0_05_out = x0_int(mach_x0[b-1],mach_x0[b],alt_x0[a-1],alt_x0[a],
		x0_05[a-1][b-1],x0_05[a-1][b],x0_05[a][b-1],x0_05[a][b],machNo,altitude);

	/* Perform linear interpolation for 'x0_06' */
	x0_06_out = x0_int(mach_x0[b-1],mach_x0[b],alt_x0[a-1],alt_x0[a],
		x0_06[a-1][b-1],x0_06[a-1][b],x0_06[a][b-1],x0_06[a][b],machNo,altitude);

	/* Perform linear interpolation for 'x0_07' */
	x0_07_out = x0_int(mach_x0[b-1],mach_x0[b],alt_x0[a-1],alt_x0[a],
		x0_07[a-1][b-1],x0_07[a-1][b],x0_07[a][b-1],x0_07[a][b],machNo,altitude);

	/* Perform linear interpolation for 'x0_08' */
	x0_08_out = x0_int(mach_x0[b-1],mach_x0[b],alt_x0[a-1],alt_x0[a],
		x0_08[a-1][b-1],x0_08[a-1][b],x0_08[a][b-1],x0_08[a][b],machNo,altitude);

	/* Perform linear interpolation for 'x0_09' */
	x0_09_out = x0_int(mach_x0[b-1],mach_x0[b],alt_x0[a-1],alt_x0[a],
		x0_09[a-1][b-1],x0_09[a-1][b],x0_09[a][b-1],x0_09[a][b],machNo,altitude);

	/* Perform linear interpolation for 'x0_10' */
	x0_10_out = x0_int(mach_x0[b-1],mach_x0[b],alt_x0[a-1],alt_x0[a],
		x0_10[a-1][b-1],x0_10[a-1][b],x0_10[a][b-1],x0_10[a][b],machNo,altitude);

	/* Perform linear interpolation for 'x0_11' */
	x0_11_out = x0_int(mach_x0[b-1],mach_x0[b],alt_x0[a-1],alt_x0[a],
		x0_11[a-1][b-1],x0_11[a-1][b],x0_11[a][b-1],x0_11[a][b],machNo,altitude);

	/* Perform linear interpolation for 'x0_12' */
	x0_12_out = x0_int(mach_x0[b-1],mach_x0[b],alt_x0[a-1],alt_x0[a],
		x0_12[a-1][b-1],x0_12[a-1][b],x0_12[a][b-1],x0_12[a][b],machNo,altitude);

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

	real_T *y = ssGetOutputPortRealSignal(S,0);

	int i,a,b;

	/* Look for nearest x-vector index OVER wanted value */
	i=1;
	if (mach_x0[0]>machNo) {
		b=i; i=m_x0; }
	else {
		while (i<m_x0)	{
			if ((mach_x0[i]>machNo) || (i==m_x0-1)) {
				b=i; i=m_x0; }
			else i++; } }

	/* Look for nearest y-vector index OVER wanted value */
	i=1;
	if (alt_x0[0]>altitude) {
		a=i; i=n_x0; }
	else {
		while (i<n_x0)	{
			if ((alt_x0[i]>altitude) || (i==n_x0-1)) {
				a=i; i=n_x0; }
			else i++; } }

	/* Perform linear interpolation for 'x0_01' */
	x0_01_out = x0_int(mach_x0[b-1],mach_x0[b],alt_x0[a-1],alt_x0[a],
		x0_01[a-1][b-1],x0_01[a-1][b],x0_01[a][b-1],x0_01[a][b],machNo,altitude);

	/* Perform linear interpolation for 'x0_02' */
	x0_02_out = x0_int(mach_x0[b-1],mach_x0[b],alt_x0[a-1],alt_x0[a],
		x0_02[a-1][b-1],x0_02[a-1][b],x0_02[a][b-1],x0_02[a][b],machNo,altitude);

	/* Perform linear interpolation for 'x0_03' */
	x0_03_out = x0_int(mach_x0[b-1],mach_x0[b],alt_x0[a-1],alt_x0[a],
		x0_03[a-1][b-1],x0_03[a-1][b],x0_03[a][b-1],x0_03[a][b],machNo,altitude);

	/* Perform linear interpolation for 'x0_04' */
	x0_04_out = x0_int(mach_x0[b-1],mach_x0[b],alt_x0[a-1],alt_x0[a],
		x0_04[a-1][b-1],x0_04[a-1][b],x0_04[a][b-1],x0_04[a][b],machNo,altitude);

	/* Perform linear interpolation for 'x0_05' */
	x0_05_out = x0_int(mach_x0[b-1],mach_x0[b],alt_x0[a-1],alt_x0[a],
		x0_05[a-1][b-1],x0_05[a-1][b],x0_05[a][b-1],x0_05[a][b],machNo,altitude);

	/* Perform linear interpolation for 'x0_06' */
	x0_06_out = x0_int(mach_x0[b-1],mach_x0[b],alt_x0[a-1],alt_x0[a],
		x0_06[a-1][b-1],x0_06[a-1][b],x0_06[a][b-1],x0_06[a][b],machNo,altitude);

	/* Perform linear interpolation for 'x0_07' */
	x0_07_out = x0_int(mach_x0[b-1],mach_x0[b],alt_x0[a-1],alt_x0[a],
		x0_07[a-1][b-1],x0_07[a-1][b],x0_07[a][b-1],x0_07[a][b],machNo,altitude);

	/* Perform linear interpolation for 'x0_08' */
	x0_08_out = x0_int(mach_x0[b-1],mach_x0[b],alt_x0[a-1],alt_x0[a],
		x0_08[a-1][b-1],x0_08[a-1][b],x0_08[a][b-1],x0_08[a][b],machNo,altitude);

	/* Perform linear interpolation for 'x0_09' */
	x0_09_out = x0_int(mach_x0[b-1],mach_x0[b],alt_x0[a-1],alt_x0[a],
		x0_09[a-1][b-1],x0_09[a-1][b],x0_09[a][b-1],x0_09[a][b],machNo,altitude);

	/* Perform linear interpolation for 'x0_10' */
	x0_10_out = x0_int(mach_x0[b-1],mach_x0[b],alt_x0[a-1],alt_x0[a],
		x0_10[a-1][b-1],x0_10[a-1][b],x0_10[a][b-1],x0_10[a][b],machNo,altitude);

	/* Perform linear interpolation for 'x0_11' */
	x0_11_out = x0_int(mach_x0[b-1],mach_x0[b],alt_x0[a-1],alt_x0[a],
		x0_11[a-1][b-1],x0_11[a-1][b],x0_11[a][b-1],x0_11[a][b],machNo,altitude);

	/* Perform linear interpolation for 'x0_12' */
	x0_12_out = x0_int(mach_x0[b-1],mach_x0[b],alt_x0[a-1],alt_x0[a],
		x0_12[a-1][b-1],x0_12[a-1][b],x0_12[a][b-1],x0_12[a][b],machNo,altitude);
	
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

