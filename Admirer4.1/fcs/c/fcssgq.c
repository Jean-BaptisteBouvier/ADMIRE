/***************************************/
/* Look-up mex-function for ADMIRE FCS */
/*                                     */
/*    Torbjorn Noren , FFA , 000306    */
/***************************************/
/*                                     */
/* INPUT                               */
/*   1. Stick Input                    */
/*   2. Altitude                       */
/*   3. Mach No.                       */
/*                                     */
/* OUTPUT                              */
/*   1. q-gain                         */
/*                                     */
/***************************************/

#define S_FUNCTION_LEVEL 2
#define S_FUNCTION_NAME  fcssgq

#define sforce		*u1[0]
#define altitude	*u2[0]
#define machNo		*u3[0]

#define	sgain		y[0]

#define sx1			ssGetRWork(S)[0]
#define sx2			ssGetRWork(S)[1]
#define sx3			ssGetRWork(S)[2]
#define sx4			ssGetRWork(S)[3]

#define sy1			ssGetRWork(S)[4]
#define sy2			ssGetRWork(S)[5]
#define sy3			ssGetRWork(S)[6]
#define sy4			ssGetRWork(S)[7]

#define sdy			ssGetRWork(S)[8]

#define sk1			ssGetRWork(S)[9]
#define sk2			ssGetRWork(S)[10]
#define sk3			ssGetRWork(S)[11]

#include <math.h>
#include <stdio.h>

#include "tmwtypes.h"
#include "simstruc.h"

#include "fcssgq.h"


/* ============================================================================ */
/* ===== Function: sg_int ===================================================== */
/* ============================================================================ */
double sg_int(double x1, double x2,double y1, double y2,
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
    
    ssSetNumSFcnParams(S,3);				/* Number of expected parameters */
	ssSetNumContStates(S,0);				/* Number of states */
    ssSetNumDiscStates(S,0);

	ssSetNumInputPorts(S,3);				/* Number of input ports */
    ssSetInputPortWidth(S,0,1);				/* Widht of input port no 1 */
	ssSetInputPortDirectFeedThrough(S,0,1);
    ssSetInputPortWidth(S,1,1);				/* Widht of input port no 2 */
	ssSetInputPortDirectFeedThrough(S,1,1);
	ssSetInputPortWidth(S,2,1);				/* Widht of input port no 3 */
	ssSetInputPortDirectFeedThrough(S,2,1);

    ssSetNumOutputPorts(S,1);				/* Number of output ports  */
    ssSetOutputPortWidth(S,0,1);			/* Widht of output port 1  */

	ssSetNumSampleTimes(S,1);				/* Number of sample times */

    ssSetNumRWork(S,12);					/* Size of real work vector */
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

	if (ssGetNumSFcnParams(S) != 3) {		/* Check no. of input parameters */
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
	InputRealPtrsType u3 = ssGetInputPortRealSignalPtrs(S,2);

	real_T *y = ssGetOutputPortRealSignal(S,0);

	int i,a,b;

	/* Get parameter values */
	sx1 = mxGetPr(ssGetSFcnParam(S,0))[0];
	sx2 = mxGetPr(ssGetSFcnParam(S,0))[1];
	sx3 = mxGetPr(ssGetSFcnParam(S,0))[2];
	sx4 = mxGetPr(ssGetSFcnParam(S,0))[3];

	sy1 = mxGetPr(ssGetSFcnParam(S,1))[0];
	sy2 = mxGetPr(ssGetSFcnParam(S,1))[1];
	sy3 = mxGetPr(ssGetSFcnParam(S,1))[2];
	sy4 = mxGetPr(ssGetSFcnParam(S,1))[3];

	sdy = mxGetPr(ssGetSFcnParam(S,2))[0];

	/* Check mach limit */
	if(machNo>mach_sgq[m_sgq])
	{
		sgain = 0;
		return;
	}

	/* Look for nearest x-vector index OVER wanted value */
	i=1;
	if (mach_sgq[0]>machNo) {
		b=i; i=m_sgq; }
	else {
		while (i<m_sgq)	{
			if ((mach_sgq[i]>machNo) || (i==m_sgq-1)) {
				b=i; i=m_sgq; }
			else i++; } }

	/* Look for nearest y-vector index OVER wanted value */
	i=1;
	if (alt_sgq[0]>altitude) {
		a=i; i=n_sgq; }
	else {
		while (i<n_sgq)	{
			if ((alt_sgq[i]>altitude) || (i==n_sgq-1)) {
				a=i; i=n_sgq; }
			else i++; } }

	/* Perform linear interpolation for 'sg_drc' */
	sk2 = sg_int(mach_sgq[b-1],mach_sgq[b],alt_sgq[a-1],alt_sgq[a],
		sgq[a-1][b-1],sgq[a-1][b],sgq[a][b-1],sgq[a][b],machNo,altitude);

	/* Calculate stick gain */
	sy2 = sk2*sx2+sdy;
	sy3 = sk2*sx3+sdy;

	sk1 = (sy2-sy1)/(sx2-sx1);
	sk3 = (sy4-sy3)/(sx4-sx3);

	if(sforce<sx2)
	{
		if(sforce<sx1)
			sgain = sk1*(sx1-sx2)+sy2;
		else
			sgain = sk1*(sforce-sx2)+sy2;
	}
	else if((sforce>=sx2) & (sforce<=sx3))
		sgain = sk2*sforce+(sy2+sy3)/2;
	else if(sforce>sx3)
	{
		if(sforce>sx4)
			sgain = sk3*(sx4-sx3)+sy3;
		else
			sgain = sk3*(sforce-sx3)+sy3;
	}

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
	InputRealPtrsType u3 = ssGetInputPortRealSignalPtrs(S,2);

	real_T *y = ssGetOutputPortRealSignal(S,0);

	int i,a,b;

	/* Check mach limit */
	if(machNo>mach_sgq[m_sgq-1])
	{
		sgain = 0;
		return;
	}

	/* Look for nearest x-vector index OVER wanted value */
	i=1;
	if (mach_sgq[0]>machNo) {
		b=i; i=m_sgq; }
	else {
		while (i<m_sgq)	{
			if ((mach_sgq[i]>machNo) || (i==m_sgq-1)) {
				b=i; i=m_sgq; }
			else i++; } }

	/* Look for nearest y-vector index OVER wanted value */
	i=1;
	if (alt_sgq[0]>altitude) {
		a=i; i=n_sgq; }
	else {
		while (i<n_sgq)	{
			if ((alt_sgq[i]>altitude) || (i==n_sgq-1)) {
				a=i; i=n_sgq; }
			else i++; } }

	/* Perform linear interpolation for 'sg_drc' */
	sk2 = sg_int(mach_sgq[b-1],mach_sgq[b],alt_sgq[a-1],alt_sgq[a],
		sgq[a-1][b-1],sgq[a-1][b],sgq[a][b-1],sgq[a][b],machNo,altitude);

	/* Calculate stick gain */
	sy2 = sk2*sx2+sdy;
	sy3 = sk2*sx3+sdy;

	sk1 = (sy2-sy1)/(sx2-sx1);
	sk3 = (sy4-sy3)/(sx4-sx3);

	if(sforce<sx2)
	{
		if(sforce<sx1)
			sgain = sy1;
		else
			sgain = sk1*(sforce-sx2)+sy2;
	}
	else if((sforce>=sx2) & (sforce<=sx3))
		sgain = sk2*sforce+(sy2+sy3)/2;
	else if(sforce>sx3)
	{
		if(sforce>sx4)
			sgain = sy4;
		else
			sgain = sk3*(sforce-sx3)+sy3;
	}
	
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