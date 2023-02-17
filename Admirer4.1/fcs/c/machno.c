/**********************************************************************/
/* Function that calculates the speed of sound, a, and density for    */
/*                                                                    */
/* It is valid up to an altitude of 25 km.                            */
/*                                                                    */
/* Created by Johansson Fredrik, FFA, 1999-12-06.                     */
/**********************************************************************/

#define S_FUNCTION_LEVEL 2
#define S_FUNCTION_NAME  machno

#define vt_in			*u1[0]
#define alt_in			*u2[0]

#define mach_out		y[0]

#define R				ssGetRWork(S)[0]
#define g				ssGetRWork(S)[1]
#define Kappa			ssGetRWork(S)[2]
#define T01				ssGetRWork(S)[3]
#define Tgrad1			ssGetRWork(S)[4]
#define T1				ssGetRWork(S)[5]
#define a1				ssGetRWork(S)[6]
#define T02				ssGetRWork(S)[7]
#define Tgrad2			ssGetRWork(S)[8]
#define T2				ssGetRWork(S)[9]
#define a2				ssGetRWork(S)[10]

#include <math.h>
#include <stdio.h>

#include "tmwtypes.h"
#include "simstruc.h"


/* ============================================================================ */
/* ===== Function: mdlInitializeSizes ========================================= */
/* ============================================================================ */
static void mdlInitializeSizes(SimStruct *S) {
    
    ssSetNumSFcnParams(S,0);				/* Number of expected parameters */
	ssSetNumContStates(S,0);				/* Number of states */
    ssSetNumDiscStates(S,0);

	ssSetNumInputPorts(S,2);				/* Number of input ports */
    ssSetInputPortWidth(S,0,1);				/* Widht of input port no  1 */
	ssSetInputPortDirectFeedThrough(S,0,1);
	ssSetInputPortWidth(S,1,1);				/* Widht of input port no  2 */
	ssSetInputPortDirectFeedThrough(S,1,1);

    ssSetNumOutputPorts(S,1);				/* Number of output ports  */
    ssSetOutputPortWidth(S,0,1);			/* Widht of output port 1  */

	ssSetNumSampleTimes(S,1);				/* Number of sample times */

    ssSetNumRWork(S,11);					/* Size of real work vector */
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

    R		=	287.05287; 	/* Universal Gas constant; Unit [m^2/s^2/K]. */
	Kappa	=	1.4;		/* Specific Heat constant                    */

	if (alt_in<=11000) {
		/*Reference temperature for alt_in=0; Unit [K].      */
		T01      = 288.15;
		/*Temperature Gradient; Unit [K/m].                  */
		Tgrad1   = 0.0065;
		/*Actual temperature for altitude alt_in; Unit [K].  */
		T1       = T01-Tgrad1*alt_in;
		/*Speed of Sound for altitude alt_in; Unit [m/s].    */
		a1       = sqrt(Kappa*R*T1);
		/*Mach No                                            */
		mach_out = vt_in/a1;
	}    

	/*Iso-thermal for  11000<alt_in<25000 [m]. */
	else if ((alt_in>11000) && (alt_in<=25000)) {
		/*Reference temperature for alt_in=11000; Unit [K].  */
		T02      = 216.65;
		/*Temperature gradient; Unit [K/m].                  */
		Tgrad2   = 0;
		/*Actual temperature for altitude alt_in; Unit [K].  */
		T2       = T02-Tgrad2*alt_in;
		/*Speed of Sound for altitude alt_in; Unit [m/s].    */
		a2       = sqrt(Kappa*R*T2);
		/*Mach No                                            */
		mach_out = vt_in/a2;
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

	real_T *y = ssGetOutputPortRealSignal(S,0);

    R		=	287.05287; 	/* Universal Gas constant; Unit [m^2/s^2/K]. */
	Kappa	=	1.4;		/* Specific Heat constant                    */

	if (alt_in<=11000) {
		/*Reference temperature for alt_in=0; Unit [K].      */
		T01      = 288.15;
		/*Temperature Gradient; Unit [K/m].                  */
		Tgrad1   = 0.0065;
		/*Actual temperature for altitude alt_in; Unit [K].  */
		T1       = T01-Tgrad1*alt_in;
		/*Speed of Sound for altitude alt_in; Unit [m/s].    */
		a1       = sqrt(Kappa*R*T1);
		/*Mach No                                            */
		mach_out = vt_in/a1;
	}    

	/*Iso-thermal for  11000<alt_in<25000 [m]. */
	else if ((alt_in>11000) && (alt_in<=25000)) {
		/*Reference temperature for alt_in=11000; Unit [K].  */
		T02      = 216.65;
		/*Temperature gradient; Unit [K/m].                  */
		Tgrad2   = 0;
		/*Actual temperature for altitude alt_in; Unit [K].  */
		T2       = T02-Tgrad2*alt_in;
		/*Speed of Sound for altitude alt_in; Unit [m/s].    */
		a2       = sqrt(Kappa*R*T2);
		/*Mach No                                            */
		mach_out = vt_in/a2;
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

