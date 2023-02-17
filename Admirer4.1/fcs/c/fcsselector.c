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
/*   1. p_drc                          */
/*   2. p_dlc                          */
/*   3. p_droe                         */
/*   4. p_drie                         */
/*   5. p_dlie                         */
/*   6. p_dloe                         */
/*   7. p_r                            */
/*   8. q_drc                          */
/*   9. q_dlc                          */
/*  10. q_droe                         */
/*  11. q_drie                         */
/*  12. q_dlie                         */
/*  13. q_dloe                         */
/*  14. q_r                            */
/*  15. beta_drc                       */
/*  16. beta_dlc                       */
/*  17. beta_droe                      */
/*  18. beta_drie                      */
/*  19. beta_dlie                      */
/*  20. beta_dloe                      */
/*  21. beta_r                         */
/*                                     */
/***************************************/

#define S_FUNCTION_LEVEL 2
#define S_FUNCTION_NAME  fcsselector

#define altitude		*u1[0]
#define machNo			*u2[0]

#define	p_drc_out		y[0]
#define	p_dlc_out		y[1]
#define	p_droe_out		y[2]
#define	p_drie_out		y[3]
#define	p_dlie_out		y[4]
#define	p_dloe_out		y[5]
#define	p_dr_out		y[6]

#define	q_drc_out		y[7]
#define	q_dlc_out		y[8]
#define	q_droe_out		y[9]
#define	q_drie_out		y[10]
#define	q_dlie_out		y[11]
#define	q_dloe_out		y[12]
#define	q_dr_out		y[13]

#define	beta_drc_out	y[14]
#define	beta_dlc_out	y[15]
#define	beta_droe_out	y[16]
#define	beta_drie_out	y[17]
#define	beta_dlie_out	y[18]
#define	beta_dloe_out	y[19]
#define	beta_dr_out		y[20]

#include <math.h>
#include <stdio.h>

#include "tmwtypes.h"
#include "simstruc.h"

#include "fcsselector.h"


/* ============================================================================ */
/* ===== Function: cs_int ===================================================== */
/* ============================================================================ */
double cs_int(double x1, double x2,double y1, double y2,
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
    ssSetOutputPortWidth(S,0,21);			/* Widht of output port 1  */

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
	if (mach_cs[0]>machNo) {
		b=i; i=m_cs; }
	else {
		while (i<m_cs)	{
			if ((mach_cs[i]>machNo) || (i==m_cs-1)) {
				b=i; i=m_cs; }
			else i++; } }

	/* Look for nearest y-vector index OVER wanted value */
	i=1;
	if (alt_cs[0]>altitude) {
		a=i; i=n_cs; }
	else {
		while (i<n_cs)	{
			if ((alt_cs[i]>altitude) || (i==n_cs-1)) {
				a=i; i=n_cs; }
			else i++; } }

	/* Perform linear interpolation for 'p_drc' */
	p_drc_out = cs_int(mach_cs[b-1],mach_cs[b],alt_cs[a-1],alt_cs[a],
		p_drc[a-1][b-1],p_drc[a-1][b],p_drc[a][b-1],p_drc[a][b],machNo,altitude);

	/* Perform linear interpolation for 'p_dlc' */
	p_dlc_out = cs_int(mach_cs[b-1],mach_cs[b],alt_cs[a-1],alt_cs[a],
		p_dlc[a-1][b-1],p_dlc[a-1][b],p_dlc[a][b-1],p_dlc[a][b],machNo,altitude);

	/* Perform linear interpolation for 'p_droe' */
	p_droe_out = cs_int(mach_cs[b-1],mach_cs[b],alt_cs[a-1],alt_cs[a],
		p_droe[a-1][b-1],p_droe[a-1][b],p_droe[a][b-1],p_droe[a][b],machNo,altitude);

	/* Perform linear interpolation for 'p_drie' */
	p_drie_out = cs_int(mach_cs[b-1],mach_cs[b],alt_cs[a-1],alt_cs[a],
		p_drie[a-1][b-1],p_drie[a-1][b],p_drie[a][b-1],p_drie[a][b],machNo,altitude);

	/* Perform linear interpolation for 'p_dlie' */
	p_dlie_out = cs_int(mach_cs[b-1],mach_cs[b],alt_cs[a-1],alt_cs[a],
		p_dlie[a-1][b-1],p_dlie[a-1][b],p_dlie[a][b-1],p_dlie[a][b],machNo,altitude);

	/* Perform linear interpolation for 'p_dloe' */
	p_dloe_out = cs_int(mach_cs[b-1],mach_cs[b],alt_cs[a-1],alt_cs[a],
		p_dloe[a-1][b-1],p_dloe[a-1][b],p_dloe[a][b-1],p_dloe[a][b],machNo,altitude);

	/* Perform linear interpolation for 'p_dr' */
	p_dr_out = cs_int(mach_cs[b-1],mach_cs[b],alt_cs[a-1],alt_cs[a],
		p_dr[a-1][b-1],p_dr[a-1][b],p_dr[a][b-1],p_dr[a][b],machNo,altitude);

	/* ==================================================================== */

	/* Perform linear interpolation for 'q_drc' */
	q_drc_out = cs_int(mach_cs[b-1],mach_cs[b],alt_cs[a-1],alt_cs[a],
		q_drc[a-1][b-1],q_drc[a-1][b],q_drc[a][b-1],q_drc[a][b],machNo,altitude);

	/* Perform linear interpolation for 'q_dlc' */
	q_dlc_out = cs_int(mach_cs[b-1],mach_cs[b],alt_cs[a-1],alt_cs[a],
		q_dlc[a-1][b-1],q_dlc[a-1][b],q_dlc[a][b-1],q_dlc[a][b],machNo,altitude);

	/* Perform linear interpolation for 'q_droe' */
	q_droe_out = cs_int(mach_cs[b-1],mach_cs[b],alt_cs[a-1],alt_cs[a],
		q_droe[a-1][b-1],q_droe[a-1][b],q_droe[a][b-1],q_droe[a][b],machNo,altitude);

	/* Perform linear interpolation for 'q_drie' */
	q_drie_out = cs_int(mach_cs[b-1],mach_cs[b],alt_cs[a-1],alt_cs[a],
		q_drie[a-1][b-1],q_drie[a-1][b],q_drie[a][b-1],q_drie[a][b],machNo,altitude);

	/* Perform linear interpolation for 'q_dlie' */
	q_dlie_out = cs_int(mach_cs[b-1],mach_cs[b],alt_cs[a-1],alt_cs[a],
		q_dlie[a-1][b-1],q_dlie[a-1][b],q_dlie[a][b-1],q_dlie[a][b],machNo,altitude);

	/* Perform linear interpolation for 'q_dloe' */
	q_dloe_out = cs_int(mach_cs[b-1],mach_cs[b],alt_cs[a-1],alt_cs[a],
		q_dloe[a-1][b-1],q_dloe[a-1][b],q_dloe[a][b-1],q_dloe[a][b],machNo,altitude);

	/* Perform linear interpolation for 'q_dr' */
	q_dr_out = cs_int(mach_cs[b-1],mach_cs[b],alt_cs[a-1],alt_cs[a],
		q_dr[a-1][b-1],q_dr[a-1][b],q_dr[a][b-1],q_dr[a][b],machNo,altitude);

	/* ==================================================================== */

	/* Perform linear interpolation for 'beta_drc' */
	beta_drc_out = cs_int(mach_cs[b-1],mach_cs[b],alt_cs[a-1],alt_cs[a],
		beta_drc[a-1][b-1],beta_drc[a-1][b],beta_drc[a][b-1],beta_drc[a][b],machNo,altitude);

	/* Perform linear interpolation for 'beta_dlc' */
	beta_dlc_out = cs_int(mach_cs[b-1],mach_cs[b],alt_cs[a-1],alt_cs[a],
		beta_dlc[a-1][b-1],beta_dlc[a-1][b],beta_dlc[a][b-1],beta_dlc[a][b],machNo,altitude);

	/* Perform linear interpolation for 'beta_droe' */
	beta_droe_out = cs_int(mach_cs[b-1],mach_cs[b],alt_cs[a-1],alt_cs[a],
		beta_droe[a-1][b-1],beta_droe[a-1][b],beta_droe[a][b-1],beta_droe[a][b],machNo,altitude);

	/* Perform linear interpolation for 'beta_drie' */
	beta_drie_out = cs_int(mach_cs[b-1],mach_cs[b],alt_cs[a-1],alt_cs[a],
		beta_drie[a-1][b-1],beta_drie[a-1][b],beta_drie[a][b-1],beta_drie[a][b],machNo,altitude);

	/* Perform linear interpolation for 'beta_dlie' */
	beta_dlie_out = cs_int(mach_cs[b-1],mach_cs[b],alt_cs[a-1],alt_cs[a],
		beta_dlie[a-1][b-1],beta_dlie[a-1][b],beta_dlie[a][b-1],beta_dlie[a][b],machNo,altitude);

	/* Perform linear interpolation for 'beta_dloe' */
	beta_dloe_out = cs_int(mach_cs[b-1],mach_cs[b],alt_cs[a-1],alt_cs[a],
		beta_dloe[a-1][b-1],beta_dloe[a-1][b],beta_dloe[a][b-1],beta_dloe[a][b],machNo,altitude);

	/* Perform linear interpolation for 'beta_dr' */
	beta_dr_out = cs_int(mach_cs[b-1],mach_cs[b],alt_cs[a-1],alt_cs[a],
		beta_dr[a-1][b-1],beta_dr[a-1][b],beta_dr[a][b-1],beta_dr[a][b],machNo,altitude);

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
	if (mach_cs[0]>machNo) {
		b=i; i=m_cs; }
	else {
		while (i<m_cs)	{
			if ((mach_cs[i]>machNo) || (i==m_cs-1)) {
				b=i; i=m_cs; }
			else i++; } }

	/* Look for nearest y-vector index OVER wanted value */
	i=1;
	if (alt_cs[0]>altitude) {
		a=i; i=n_cs; }
	else {
		while (i<n_cs)	{
			if ((alt_cs[i]>altitude) || (i==n_cs-1)) {
				a=i; i=n_cs; }
			else i++; } }

	/* Perform linear interpolation for 'p_drc' */
	p_drc_out = cs_int(mach_cs[b-1],mach_cs[b],alt_cs[a-1],alt_cs[a],
		p_drc[a-1][b-1],p_drc[a-1][b],p_drc[a][b-1],p_drc[a][b],machNo,altitude);

	/* Perform linear interpolation for 'p_dlc' */
	p_dlc_out = cs_int(mach_cs[b-1],mach_cs[b],alt_cs[a-1],alt_cs[a],
		p_dlc[a-1][b-1],p_dlc[a-1][b],p_dlc[a][b-1],p_dlc[a][b],machNo,altitude);

	/* Perform linear interpolation for 'p_droe' */
	p_droe_out = cs_int(mach_cs[b-1],mach_cs[b],alt_cs[a-1],alt_cs[a],
		p_droe[a-1][b-1],p_droe[a-1][b],p_droe[a][b-1],p_droe[a][b],machNo,altitude);

	/* Perform linear interpolation for 'p_drie' */
	p_drie_out = cs_int(mach_cs[b-1],mach_cs[b],alt_cs[a-1],alt_cs[a],
		p_drie[a-1][b-1],p_drie[a-1][b],p_drie[a][b-1],p_drie[a][b],machNo,altitude);

	/* Perform linear interpolation for 'p_dlie' */
	p_dlie_out = cs_int(mach_cs[b-1],mach_cs[b],alt_cs[a-1],alt_cs[a],
		p_dlie[a-1][b-1],p_dlie[a-1][b],p_dlie[a][b-1],p_dlie[a][b],machNo,altitude);

	/* Perform linear interpolation for 'p_dloe' */
	p_dloe_out = cs_int(mach_cs[b-1],mach_cs[b],alt_cs[a-1],alt_cs[a],
		p_dloe[a-1][b-1],p_dloe[a-1][b],p_dloe[a][b-1],p_dloe[a][b],machNo,altitude);

	/* Perform linear interpolation for 'p_dr' */
	p_dr_out = cs_int(mach_cs[b-1],mach_cs[b],alt_cs[a-1],alt_cs[a],
		p_dr[a-1][b-1],p_dr[a-1][b],p_dr[a][b-1],p_dr[a][b],machNo,altitude);

	/* ==================================================================== */

	/* Perform linear interpolation for 'q_drc' */
	q_drc_out = cs_int(mach_cs[b-1],mach_cs[b],alt_cs[a-1],alt_cs[a],
		q_drc[a-1][b-1],q_drc[a-1][b],q_drc[a][b-1],q_drc[a][b],machNo,altitude);

	/* Perform linear interpolation for 'q_dlc' */
	q_dlc_out = cs_int(mach_cs[b-1],mach_cs[b],alt_cs[a-1],alt_cs[a],
		q_dlc[a-1][b-1],q_dlc[a-1][b],q_dlc[a][b-1],q_dlc[a][b],machNo,altitude);

	/* Perform linear interpolation for 'q_droe' */
	q_droe_out = cs_int(mach_cs[b-1],mach_cs[b],alt_cs[a-1],alt_cs[a],
		q_droe[a-1][b-1],q_droe[a-1][b],q_droe[a][b-1],q_droe[a][b],machNo,altitude);

	/* Perform linear interpolation for 'q_drie' */
	q_drie_out = cs_int(mach_cs[b-1],mach_cs[b],alt_cs[a-1],alt_cs[a],
		q_drie[a-1][b-1],q_drie[a-1][b],q_drie[a][b-1],q_drie[a][b],machNo,altitude);

	/* Perform linear interpolation for 'q_dlie' */
	q_dlie_out = cs_int(mach_cs[b-1],mach_cs[b],alt_cs[a-1],alt_cs[a],
		q_dlie[a-1][b-1],q_dlie[a-1][b],q_dlie[a][b-1],q_dlie[a][b],machNo,altitude);

	/* Perform linear interpolation for 'q_dloe' */
	q_dloe_out = cs_int(mach_cs[b-1],mach_cs[b],alt_cs[a-1],alt_cs[a],
		q_dloe[a-1][b-1],q_dloe[a-1][b],q_dloe[a][b-1],q_dloe[a][b],machNo,altitude);

	/* Perform linear interpolation for 'q_dr' */
	q_dr_out = cs_int(mach_cs[b-1],mach_cs[b],alt_cs[a-1],alt_cs[a],
		q_dr[a-1][b-1],q_dr[a-1][b],q_dr[a][b-1],q_dr[a][b],machNo,altitude);

	/* ==================================================================== */

	/* Perform linear interpolation for 'beta_drc' */
	beta_drc_out = cs_int(mach_cs[b-1],mach_cs[b],alt_cs[a-1],alt_cs[a],
		beta_drc[a-1][b-1],beta_drc[a-1][b],beta_drc[a][b-1],beta_drc[a][b],machNo,altitude);

	/* Perform linear interpolation for 'beta_dlc' */
	beta_dlc_out = cs_int(mach_cs[b-1],mach_cs[b],alt_cs[a-1],alt_cs[a],
		beta_dlc[a-1][b-1],beta_dlc[a-1][b],beta_dlc[a][b-1],beta_dlc[a][b],machNo,altitude);

	/* Perform linear interpolation for 'beta_droe' */
	beta_droe_out = cs_int(mach_cs[b-1],mach_cs[b],alt_cs[a-1],alt_cs[a],
		beta_droe[a-1][b-1],beta_droe[a-1][b],beta_droe[a][b-1],beta_droe[a][b],machNo,altitude);

	/* Perform linear interpolation for 'beta_drie' */
	beta_drie_out = cs_int(mach_cs[b-1],mach_cs[b],alt_cs[a-1],alt_cs[a],
		beta_drie[a-1][b-1],beta_drie[a-1][b],beta_drie[a][b-1],beta_drie[a][b],machNo,altitude);

	/* Perform linear interpolation for 'beta_dlie' */
	beta_dlie_out = cs_int(mach_cs[b-1],mach_cs[b],alt_cs[a-1],alt_cs[a],
		beta_dlie[a-1][b-1],beta_dlie[a-1][b],beta_dlie[a][b-1],beta_dlie[a][b],machNo,altitude);

	/* Perform linear interpolation for 'beta_dloe' */
	beta_dloe_out = cs_int(mach_cs[b-1],mach_cs[b],alt_cs[a-1],alt_cs[a],
		beta_dloe[a-1][b-1],beta_dloe[a-1][b],beta_dloe[a][b-1],beta_dloe[a][b],machNo,altitude);

	/* Perform linear interpolation for 'beta_dr' */
	beta_dr_out = cs_int(mach_cs[b-1],mach_cs[b],alt_cs[a-1],alt_cs[a],
		beta_dr[a-1][b-1],beta_dr[a-1][b],beta_dr[a][b-1],beta_dr[a][b],machNo,altitude);
	
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

