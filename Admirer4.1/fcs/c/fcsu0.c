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
/*   1. u0_01                          */
/*   2. u0_02                          */
/*   3. u0_03                          */
/*   4. u0_04                          */
/*   5. u0_05                          */
/*   6. u0_06                          */
/*   7. u0_07                          */
/*   8. u0_08                          */
/*   9. u0_09                          */
/*  10. u0_10                          */
/*  11. u0_11                          */
/*  12. u0_12                          */
/*  13. u0_13                          */
/*  14. u0_14                          */
/*  15. u0_15                          */
/*  16. u0_16                          */
/*                                     */
/***************************************/

#define S_FUNCTION_LEVEL 2
#define S_FUNCTION_NAME  fcsu0

#define altitude		*u1[0]
#define machNo			*u2[0]

#define	u0_01_out		y[0]
#define	u0_02_out		y[1]
#define	u0_03_out		y[2]
#define	u0_04_out		y[3]
#define	u0_05_out		y[4]
#define	u0_06_out		y[5]
#define	u0_07_out		y[6]
#define	u0_08_out		y[7]
#define	u0_09_out		y[8]
#define	u0_10_out		y[9]
#define	u0_11_out		y[10]
#define	u0_12_out		y[11]
#define	u0_13_out		y[12]
#define	u0_14_out		y[13]
#define	u0_15_out		y[14]
#define	u0_16_out		y[15]

#include <math.h>
#include <stdio.h>

#include "tmwtypes.h"
#include "simstruc.h"

#include "fcsu0.h"


/* ============================================================================ */
/* ===== Function: u0_int ===================================================== */
/* ============================================================================ */
double u0_int(double x1, double x2,double y1, double y2,
				   double z11, double z12, double z21, double z22,
				   double x, double y){

	double temp1, temp2;

	if (x<x1) x = x1;
	else if (x>x2) x = x2;

	if (y<y1) y = y1;
	else if (y>y2) y = y2;

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
    ssSetOutputPortWidth(S,0,16);			/* Widht of output port 1  */

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
	if (mach_u0[0]>machNo) {
		b=i; i=m_u0; }
	else {
		while (i<m_u0)	{
			if ((mach_u0[i]>machNo) || (i==m_u0-1)) {
				b=i; i=m_u0; }
			else i++; } }

	/* Look for nearest y-vector index OVER wanted value */
	i=1;
	if (alt_u0[0]>altitude) {
		a=i; i=n_u0; }
	else {
		while (i<n_u0)	{
			if ((alt_u0[i]>altitude) || (i==n_u0-1)) {
				a=i; i=n_u0; }
			else i++; } }

	/* Perform linear interpolation for 'u0_01' */
	u0_01_out = u0_int(mach_u0[b-1],mach_u0[b],alt_u0[a-1],alt_u0[a],
		u0_01[a-1][b-1],u0_01[a-1][b],u0_01[a][b-1],u0_01[a][b],machNo,altitude);

	/* Perform linear interpolation for 'u0_02' */
	u0_02_out = u0_int(mach_u0[b-1],mach_u0[b],alt_u0[a-1],alt_u0[a],
		u0_02[a-1][b-1],u0_02[a-1][b],u0_02[a][b-1],u0_02[a][b],machNo,altitude);

	/* Perform linear interpolation for 'u0_03' */
	u0_03_out = u0_int(mach_u0[b-1],mach_u0[b],alt_u0[a-1],alt_u0[a],
		u0_03[a-1][b-1],u0_03[a-1][b],u0_03[a][b-1],u0_03[a][b],machNo,altitude);

	/* Perform linear interpolation for 'u0_04' */
	u0_04_out = u0_int(mach_u0[b-1],mach_u0[b],alt_u0[a-1],alt_u0[a],
		u0_04[a-1][b-1],u0_04[a-1][b],u0_04[a][b-1],u0_04[a][b],machNo,altitude);

	/* Perform linear interpolation for 'u0_05' */
	u0_05_out = u0_int(mach_u0[b-1],mach_u0[b],alt_u0[a-1],alt_u0[a],
		u0_05[a-1][b-1],u0_05[a-1][b],u0_05[a][b-1],u0_05[a][b],machNo,altitude);

	/* Perform linear interpolation for 'u0_06' */
	u0_06_out = u0_int(mach_u0[b-1],mach_u0[b],alt_u0[a-1],alt_u0[a],
		u0_06[a-1][b-1],u0_06[a-1][b],u0_06[a][b-1],u0_06[a][b],machNo,altitude);

	/* Perform linear interpolation for 'u0_07' */
	u0_07_out = u0_int(mach_u0[b-1],mach_u0[b],alt_u0[a-1],alt_u0[a],
		u0_07[a-1][b-1],u0_07[a-1][b],u0_07[a][b-1],u0_07[a][b],machNo,altitude);

	/* Perform linear interpolation for 'u0_08' */
	u0_08_out = u0_int(mach_u0[b-1],mach_u0[b],alt_u0[a-1],alt_u0[a],
		u0_08[a-1][b-1],u0_08[a-1][b],u0_08[a][b-1],u0_08[a][b],machNo,altitude);

	/* Perform linear interpolation for 'u0_09' */
	u0_09_out = u0_int(mach_u0[b-1],mach_u0[b],alt_u0[a-1],alt_u0[a],
		u0_09[a-1][b-1],u0_09[a-1][b],u0_09[a][b-1],u0_09[a][b],machNo,altitude);

	/* Perform linear interpolation for 'u0_10' */
	u0_10_out = u0_int(mach_u0[b-1],mach_u0[b],alt_u0[a-1],alt_u0[a],
		u0_10[a-1][b-1],u0_10[a-1][b],u0_10[a][b-1],u0_10[a][b],machNo,altitude);

	/* Perform linear interpolation for 'u0_11' */
	u0_11_out = u0_int(mach_u0[b-1],mach_u0[b],alt_u0[a-1],alt_u0[a],
		u0_11[a-1][b-1],u0_11[a-1][b],u0_11[a][b-1],u0_11[a][b],machNo,altitude);

	/* Perform linear interpolation for 'u0_12' */
	u0_12_out = u0_int(mach_u0[b-1],mach_u0[b],alt_u0[a-1],alt_u0[a],
		u0_12[a-1][b-1],u0_12[a-1][b],u0_12[a][b-1],u0_12[a][b],machNo,altitude);

	/* Perform linear interpolation for 'u0_13' */
	u0_13_out = u0_int(mach_u0[b-1],mach_u0[b],alt_u0[a-1],alt_u0[a],
		u0_13[a-1][b-1],u0_13[a-1][b],u0_13[a][b-1],u0_13[a][b],machNo,altitude);

	/* Perform linear interpolation for 'u0_14' */
	u0_14_out = u0_int(mach_u0[b-1],mach_u0[b],alt_u0[a-1],alt_u0[a],
		u0_14[a-1][b-1],u0_14[a-1][b],u0_14[a][b-1],u0_14[a][b],machNo,altitude);

	/* Perform linear interpolation for 'u0_15' */
	u0_15_out = u0_int(mach_u0[b-1],mach_u0[b],alt_u0[a-1],alt_u0[a],
		u0_15[a-1][b-1],u0_15[a-1][b],u0_15[a][b-1],u0_15[a][b],machNo,altitude);

	/* Perform linear interpolation for 'u0_16' */
	u0_16_out = u0_int(mach_u0[b-1],mach_u0[b],alt_u0[a-1],alt_u0[a],
		u0_16[a-1][b-1],u0_16[a-1][b],u0_16[a][b-1],u0_16[a][b],machNo,altitude);

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
	if (mach_u0[0]>machNo) {
		b=i; i=m_u0; }
	else {
		while (i<m_u0)	{
			if ((mach_u0[i]>machNo) || (i==m_u0-1)) {
				b=i; i=m_u0; }
			else i++; } }

	/* Look for nearest y-vector index OVER wanted value */
	i=1;
	if (alt_u0[0]>altitude) {
		a=i; i=n_u0; }
	else {
		while (i<n_u0)	{
			if ((alt_u0[i]>altitude) || (i==n_u0-1)) {
				a=i; i=n_u0; }
			else i++; } }

	/* Perform linear interpolation for 'u0_01' */
	u0_01_out = u0_int(mach_u0[b-1],mach_u0[b],alt_u0[a-1],alt_u0[a],
		u0_01[a-1][b-1],u0_01[a-1][b],u0_01[a][b-1],u0_01[a][b],machNo,altitude);

	/* Perform linear interpolation for 'u0_02' */
	u0_02_out = u0_int(mach_u0[b-1],mach_u0[b],alt_u0[a-1],alt_u0[a],
		u0_02[a-1][b-1],u0_02[a-1][b],u0_02[a][b-1],u0_02[a][b],machNo,altitude);

	/* Perform linear interpolation for 'u0_03' */
	u0_03_out = u0_int(mach_u0[b-1],mach_u0[b],alt_u0[a-1],alt_u0[a],
		u0_03[a-1][b-1],u0_03[a-1][b],u0_03[a][b-1],u0_03[a][b],machNo,altitude);

	/* Perform linear interpolation for 'u0_04' */
	u0_04_out = u0_int(mach_u0[b-1],mach_u0[b],alt_u0[a-1],alt_u0[a],
		u0_04[a-1][b-1],u0_04[a-1][b],u0_04[a][b-1],u0_04[a][b],machNo,altitude);

	/* Perform linear interpolation for 'u0_05' */
	u0_05_out = u0_int(mach_u0[b-1],mach_u0[b],alt_u0[a-1],alt_u0[a],
		u0_05[a-1][b-1],u0_05[a-1][b],u0_05[a][b-1],u0_05[a][b],machNo,altitude);

	/* Perform linear interpolation for 'u0_06' */
	u0_06_out = u0_int(mach_u0[b-1],mach_u0[b],alt_u0[a-1],alt_u0[a],
		u0_06[a-1][b-1],u0_06[a-1][b],u0_06[a][b-1],u0_06[a][b],machNo,altitude);

	/* Perform linear interpolation for 'u0_07' */
	u0_07_out = u0_int(mach_u0[b-1],mach_u0[b],alt_u0[a-1],alt_u0[a],
		u0_07[a-1][b-1],u0_07[a-1][b],u0_07[a][b-1],u0_07[a][b],machNo,altitude);

	/* Perform linear interpolation for 'u0_08' */
	u0_08_out = u0_int(mach_u0[b-1],mach_u0[b],alt_u0[a-1],alt_u0[a],
		u0_08[a-1][b-1],u0_08[a-1][b],u0_08[a][b-1],u0_08[a][b],machNo,altitude);

	/* Perform linear interpolation for 'u0_09' */
	u0_09_out = u0_int(mach_u0[b-1],mach_u0[b],alt_u0[a-1],alt_u0[a],
		u0_09[a-1][b-1],u0_09[a-1][b],u0_09[a][b-1],u0_09[a][b],machNo,altitude);

	/* Perform linear interpolation for 'u0_10' */
	u0_10_out = u0_int(mach_u0[b-1],mach_u0[b],alt_u0[a-1],alt_u0[a],
		u0_10[a-1][b-1],u0_10[a-1][b],u0_10[a][b-1],u0_10[a][b],machNo,altitude);

	/* Perform linear interpolation for 'u0_11' */
	u0_11_out = u0_int(mach_u0[b-1],mach_u0[b],alt_u0[a-1],alt_u0[a],
		u0_11[a-1][b-1],u0_11[a-1][b],u0_11[a][b-1],u0_11[a][b],machNo,altitude);

	/* Perform linear interpolation for 'u0_12' */
	u0_12_out = u0_int(mach_u0[b-1],mach_u0[b],alt_u0[a-1],alt_u0[a],
		u0_12[a-1][b-1],u0_12[a-1][b],u0_12[a][b-1],u0_12[a][b],machNo,altitude);

	/* Perform linear interpolation for 'u0_13' */
	u0_13_out = u0_int(mach_u0[b-1],mach_u0[b],alt_u0[a-1],alt_u0[a],
		u0_13[a-1][b-1],u0_13[a-1][b],u0_13[a][b-1],u0_13[a][b],machNo,altitude);

	/* Perform linear interpolation for 'u0_14' */
	u0_14_out = u0_int(mach_u0[b-1],mach_u0[b],alt_u0[a-1],alt_u0[a],
		u0_14[a-1][b-1],u0_14[a-1][b],u0_14[a][b-1],u0_14[a][b],machNo,altitude);

	/* Perform linear interpolation for 'u0_15' */
	u0_15_out = u0_int(mach_u0[b-1],mach_u0[b],alt_u0[a-1],alt_u0[a],
		u0_15[a-1][b-1],u0_15[a-1][b],u0_15[a][b-1],u0_15[a][b],machNo,altitude);

	/* Perform linear interpolation for 'u0_16' */
	u0_16_out = u0_int(mach_u0[b-1],mach_u0[b],alt_u0[a-1],alt_u0[a],
		u0_16[a-1][b-1],u0_16[a-1][b],u0_16[a][b-1],u0_16[a][b],machNo,altitude);
	
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

