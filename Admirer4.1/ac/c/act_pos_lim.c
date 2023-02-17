/********************************************************************************/
/*																				*/
/*  Copyright (C) 1998 FFA/FOSIM,The Aeronautical Research Institute Of Sweden	*/
/*  SE-100 44, Stockholm Sweden													*/
/*																				*/
/*  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION.			*/
/*  This program software may be copied and distributed in any medium,free		*/
/*  of charge, provided that the copyright statement and these terms and		*/
/*  conditions are kept intact and included in every copy. The software may		*/
/*  be modified or included, in part or as a whole in another program. This		*/
/*  modified software or program may be copied and distributed under the above	*/
/*  mentioned terms, providing that the nature of changes, who changed it and	*/
/*  when it was changed are clearly stated in the program, preferably in		*/
/*  conjunction with this copyright notice. As this is a free of charge			*/
/*  software it is provided "as is" without warranty of any kind including,		*/
/*  but not limited to, the implied warranties of merchantability and fitness	*/
/*  for a particular purpose. The copyright holder or any other part  who has	*/
/*  modified and/or redistributed the software or any thereupon based program,	*/
/*  in accordance with above noted terms, may not be held responsible for any	*/
/*  kind of damages caused by the use or inability to use the program.			*/
/*																				*/
/*  Programmed by F Johansson to be used as a Mex-file in Simulink and Real-	*/
/*  Time WorkShop.																*/
/*  This version contains limits for the actuators used in ADMIRE.				*/
/*																				*/
/********************************************************************************/
/*																				*/
/*  File act_pos_lim.c															*/
/*  Version 1																	*/
/*																				*/
/*  Used variables:																*/
/*																				*/
/*	  Input varables:															*/
/*	  drc_in	   right canard angle				 		[rad]				*/
/*	  dlc_in	   left canard angle				 		[rad]				*/
/*	  droe_in	   right inboard elevon angle			 	[rad]				*/
/*	  drie_in	   right outboard elevon angle		 		[rad]				*/
/*	  dlie_in	   left outboard elevon angle			 	[rad]				*/
/*	  dloe_in	   left inboard elevon angle			 	[rad]				*/
/*	  dr_in	   	   rudder angle					 			[rad]				*/
/*	  dle_in	   leading-edge flap angle			 		[rad]				*/
/*	  tss_in	   throttle setting					 		[-]					*/
/*	  mach_in	   Mach number					 			[..]				*/
/*																				*/
/*																				*/
/*																				*/
/*	  Output varables:															*/
/*	  drc_out	   right canard angle				 		[rad]				*/
/*	  dlc_out	   left canard angle				 		[rad]				*/
/*	  droe_out	   right inboard elevon angle			 	[rad]				*/
/*	  drie_out	   right outboard elevon angle		 		[rad]				*/
/*	  dlie_out	   left outboard elevon angle			 	[rad]				*/
/*	  dloe_out	   left inboard elevon angle			 	[rad]				*/
/*	  dr_out	   rudder angle					 			[rad]				*/
/*	  dle_out	   leading-edge flap angle			 		[rad]				*/
/*	  tss_out	   throttle setting					 		[-]					*/
/*																				*/
/********************************************************************************/

#define S_FUNCTION_LEVEL 2
#define S_FUNCTION_NAME  act_pos_lim

#define drc_in			*u01[0]
#define dlc_in			*u02[0]
#define droe_in			*u03[0]
#define drie_in			*u04[0]
#define dlie_in			*u05[0]
#define dloe_in			*u06[0]
#define dr_in			*u07[0]
#define dle_in			*u08[0]
#define tss_in			*u09[0]
#define mach_in			*u10[0]

#define drc_out			y01[0]
#define dlc_out			y02[0]
#define droe_out		y03[0]
#define drie_out		y04[0]
#define dlie_out		y05[0]
#define dloe_out		y06[0]
#define dr_out			y07[0]
#define dle_out			y08[0]
#define tss_out			y09[0]

#define degtorad		ssGetRWork(S)[0]
#define radtodeg		ssGetRWork(S)[1]
#define lim_N55			ssGetRWork(S)[2]
#define lim_N50			ssGetRWork(S)[3]
#define lim_N45			ssGetRWork(S)[4]
#define lim_N40			ssGetRWork(S)[5]
#define lim_N35			ssGetRWork(S)[6]
#define lim_N30			ssGetRWork(S)[7]
#define lim_N25			ssGetRWork(S)[8]
#define lim_N20			ssGetRWork(S)[9]
#define lim_N15			ssGetRWork(S)[10]
#define lim_N10			ssGetRWork(S)[11]
#define lim_N05			ssGetRWork(S)[12]
#define lim_000			ssGetRWork(S)[13]
#define lim_P05			ssGetRWork(S)[14]
#define lim_P10			ssGetRWork(S)[15]
#define lim_P15			ssGetRWork(S)[16]
#define lim_P20			ssGetRWork(S)[17]
#define lim_P25			ssGetRWork(S)[18]
#define lim_P30			ssGetRWork(S)[19]
#define lim_P35			ssGetRWork(S)[20]
#define lim_P40			ssGetRWork(S)[21]
#define lim_P45			ssGetRWork(S)[22]
#define lim_P50 		ssGetRWork(S)[23]
#define lim_P55			ssGetRWork(S)[24]

#include <math.h>
#include <stdio.h>

#include "tmwtypes.h"
#include "simstruc.h"


/* ============================================================================ */
/* ===== Function: adm_limit ================================================== */
/* ============================================================================ */
#if (!defined(ADM_LIMIT))
#define ADM_LIMIT
double adm_limit(double variable, double max_limit, double min_limit, SimStruct *S){
	 	if (variable>max_limit)
			return(max_limit);
	  	else if (variable<min_limit)
			return(min_limit);
	  	else
 			return(variable);
}
#endif

/* ============================================================================ */
/* ===== Function: mdlInitializeSizes ========================================= */
/* ============================================================================ */
static void mdlInitializeSizes(SimStruct *S) {
    
    ssSetNumSFcnParams(S,0);				/* Number of expected parameters */
	ssSetNumContStates(S,0);				/* Number of states */
    ssSetNumDiscStates(S,0);

	ssSetNumInputPorts(S,10);				/* Number of input ports */
    ssSetInputPortWidth(S,0,1);				/* Widht of input port no  1 */
	ssSetInputPortDirectFeedThrough(S,0,1);
    ssSetInputPortWidth(S,1,1);				/* Widht of input port no  2 */
	ssSetInputPortDirectFeedThrough(S,1,1);
	ssSetInputPortWidth(S,2,1);				/* Widht of input port no  3 */
	ssSetInputPortDirectFeedThrough(S,2,1);
	ssSetInputPortWidth(S,3,1);				/* Widht of input port no  4 */
	ssSetInputPortDirectFeedThrough(S,3,1);
	ssSetInputPortWidth(S,4,1);				/* Widht of input port no  5 */
	ssSetInputPortDirectFeedThrough(S,4,1);
	ssSetInputPortWidth(S,5,1);				/* Widht of input port no  6 */
	ssSetInputPortDirectFeedThrough(S,5,1);
	ssSetInputPortWidth(S,6,1);				/* Widht of input port no  7 */
	ssSetInputPortDirectFeedThrough(S,6,1);
	ssSetInputPortWidth(S,7,1);				/* Widht of input port no  8 */
	ssSetInputPortDirectFeedThrough(S,7,1);
	ssSetInputPortWidth(S,8,1);				/* Widht of input port no  9 */
	ssSetInputPortDirectFeedThrough(S,8,1);
	ssSetInputPortWidth(S,9,1);				/* Widht of input port no 10 */
	ssSetInputPortDirectFeedThrough(S,9,1);

    ssSetNumOutputPorts(S,9);				/* Number of output ports  */
    ssSetOutputPortWidth(S,0,1);			/* Widht of output port 1  */
	ssSetOutputPortWidth(S,1,1);			/* Widht of output port 2  */
	ssSetOutputPortWidth(S,2,1);			/* Widht of output port 3  */
	ssSetOutputPortWidth(S,3,1);			/* Widht of output port 4  */
	ssSetOutputPortWidth(S,4,1);			/* Widht of output port 5  */
	ssSetOutputPortWidth(S,5,1);			/* Widht of output port 6  */
	ssSetOutputPortWidth(S,6,1);			/* Widht of output port 7  */
	ssSetOutputPortWidth(S,7,1);			/* Widht of output port 8  */
	ssSetOutputPortWidth(S,8,1);			/* Widht of output port 9  */

	ssSetNumSampleTimes(S,1);				/* Number of sample times */

    ssSetNumRWork(S,25);					/* Size of real work vector */
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

	degtorad = 0.017453293;
	radtodeg = 57.2958;

	lim_N55 = -55*degtorad;
	lim_N50 = -50*degtorad;
	lim_N45 = -45*degtorad;
	lim_N40 = -40*degtorad;
	lim_N35 = -35*degtorad;
	lim_N30 = -30*degtorad;
	lim_N25 = -25*degtorad;
	lim_N20 = -20*degtorad;
	lim_N15 = -15*degtorad;
	lim_N10 = -10*degtorad;
	lim_N05 =  -5*degtorad;
	lim_000 =   0;
	lim_P05 =   5*degtorad;
	lim_P10 =  10*degtorad;
	lim_P15 =  15*degtorad;
	lim_P20 =  20*degtorad;
	lim_P25 =  25*degtorad;
	lim_P30 =  30*degtorad;
	lim_P35 =  35*degtorad;
	lim_P40 =  40*degtorad;
	lim_P45 =  45*degtorad;
	lim_P50 =  50*degtorad;
	lim_P55 =  55*degtorad;

}
#endif

/* ============================================================================ */
/* ===== Function: mdlStart =================================================== */
/* ============================================================================ */
#define MDL_START  /* Change to #undef to remove function */
#if defined(MDL_START)
static void mdlStart(SimStruct *S) {}
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

	InputRealPtrsType u01 = ssGetInputPortRealSignalPtrs(S,0);
	InputRealPtrsType u02 = ssGetInputPortRealSignalPtrs(S,1);
	InputRealPtrsType u03 = ssGetInputPortRealSignalPtrs(S,2);
	InputRealPtrsType u04 = ssGetInputPortRealSignalPtrs(S,3);
	InputRealPtrsType u05 = ssGetInputPortRealSignalPtrs(S,4);
	InputRealPtrsType u06 = ssGetInputPortRealSignalPtrs(S,5);
	InputRealPtrsType u07 = ssGetInputPortRealSignalPtrs(S,6);
	InputRealPtrsType u08 = ssGetInputPortRealSignalPtrs(S,7);
	InputRealPtrsType u09 = ssGetInputPortRealSignalPtrs(S,8);
	InputRealPtrsType u10 = ssGetInputPortRealSignalPtrs(S,9);

	real_T *y01 = ssGetOutputPortRealSignal(S,0);
	real_T *y02 = ssGetOutputPortRealSignal(S,1);
	real_T *y03 = ssGetOutputPortRealSignal(S,2);
	real_T *y04 = ssGetOutputPortRealSignal(S,3);
	real_T *y05 = ssGetOutputPortRealSignal(S,4);
	real_T *y06 = ssGetOutputPortRealSignal(S,5);
	real_T *y07 = ssGetOutputPortRealSignal(S,6);
	real_T *y08 = ssGetOutputPortRealSignal(S,7);
	real_T *y09 = ssGetOutputPortRealSignal(S,8);

	if (mach_in<=0.5) {		     /* allowed flight envelop */
	 	drc_out  = adm_limit(drc_in, lim_P25,lim_N55,S);
	 	dlc_out  = adm_limit(dlc_in, lim_P25,lim_N55,S);
	 	droe_out = adm_limit(droe_in,lim_P30,lim_N30,S);
	 	drie_out = adm_limit(drie_in,lim_P30,lim_N30,S);
	 	dlie_out = adm_limit(dlie_in,lim_P30,lim_N30,S);
	 	dloe_out = adm_limit(dloe_in,lim_P30,lim_N30,S);
	 	dr_out   = adm_limit(dr_in,  lim_P30,lim_N30,S);
	 	dle_out  = adm_limit(dle_in, lim_P30,lim_N10,S);
	 	tss_out  = adm_limit(tss_in,1,0,S);
	}
    if ((mach_in<=0.8) && (mach_in>0.5)) {
	  	drc_out  = adm_limit(drc_in, lim_P25,lim_N25,S);
	 	dlc_out  = adm_limit(dlc_in, lim_P25,lim_N25,S);
	 	droe_out = adm_limit(droe_in,lim_P25,lim_N30,S);
	 	drie_out = adm_limit(drie_in,lim_P25,lim_N30,S);
	 	dlie_out = adm_limit(dlie_in,lim_P25,lim_N30,S);
	 	dloe_out = adm_limit(dloe_in,lim_P25,lim_N30,S);
	 	dr_out   = adm_limit(dr_in,  lim_P25,lim_N25,S);
	 	dle_out  = adm_limit(dle_in, lim_P30,lim_N10,S);
	 	tss_out  = adm_limit(tss_in,1,0,S);
    }
    if ((mach_in<=0.9) && (mach_in>0.8)) {
	 	drc_out  = adm_limit(drc_in, lim_P15,lim_N15,S);
	 	dlc_out  = adm_limit(dlc_in, lim_P15,lim_N15,S);
	 	droe_out = adm_limit(droe_in,lim_P25,lim_N30,S);
	 	drie_out = adm_limit(drie_in,lim_P25,lim_N30,S);
	 	dlie_out = adm_limit(dlie_in,lim_P25,lim_N30,S);
	 	dloe_out = adm_limit(dloe_in,lim_P25,lim_N30,S);
	 	dr_out   = adm_limit(dr_in,  lim_P20,lim_N20,S);
	 	dle_out  = adm_limit(dle_in, lim_P20,lim_N10,S);
	 	tss_out  = adm_limit(tss_in,1,0,S);
    }
    if ((mach_in<=0.95) && (mach_in>0.9)) {
	  	drc_out  = adm_limit(drc_in, lim_P15,lim_N15,S);
	 	dlc_out  = adm_limit(dlc_in, lim_P15,lim_N15,S);
	 	droe_out = adm_limit(droe_in,lim_P25,lim_N30,S);
	 	drie_out = adm_limit(drie_in,lim_P25,lim_N30,S);
	 	dlie_out = adm_limit(dlie_in,lim_P25,lim_N30,S);
	 	dloe_out = adm_limit(dloe_in,lim_P25,lim_N30,S);
	 	dr_out   = adm_limit(dr_in,  lim_P15,lim_N15,S);
	 	dle_out  = adm_limit(dle_in, lim_P20,lim_N10,S);
	 	tss_out  = adm_limit(tss_in,1,0,S);
    }
    if ((mach_in<=1.4) && (mach_in>0.95)) {
		drc_out  = adm_limit(drc_in, lim_P15,lim_N15,S);
	 	dlc_out  = adm_limit(dlc_in, lim_P15,lim_N15,S);
	 	droe_out = adm_limit(droe_in,lim_P25,lim_N30,S);
	 	drie_out = adm_limit(drie_in,lim_P25,lim_N30,S);
	 	dlie_out = adm_limit(dlie_in,lim_P25,lim_N30,S);
	 	dloe_out = adm_limit(dloe_in,lim_P25,lim_N30,S);
	 	dr_out   = adm_limit(dr_in,  lim_P15,lim_N15,S);
	 	dle_out  = adm_limit(dle_in, lim_P15,lim_N10,S);
	 	tss_out  = adm_limit(tss_in,1,0,S);
    }
    if ((mach_in<=1.5) && (mach_in>1.4)) {
	 	drc_out  = adm_limit(drc_in, lim_P15,lim_N15,S);
	 	dlc_out  = adm_limit(dlc_in, lim_P15,lim_N15,S);
	 	droe_out = adm_limit(droe_in,lim_P25,lim_N30,S);
	 	drie_out = adm_limit(drie_in,lim_P25,lim_N30,S);
	 	dlie_out = adm_limit(dlie_in,lim_P25,lim_N30,S);
	 	dloe_out = adm_limit(dloe_in,lim_P25,lim_N30,S);
	 	dr_out   = adm_limit(dr_in,  lim_P15,lim_N15,S);
	 	dle_out  = adm_limit(dle_in, lim_P10,lim_N10,S);
	 	tss_out  = adm_limit(tss_in,1,0,S);
    }
    if ((mach_in<=2.5) && (mach_in>1.5)) {
	 	drc_out  = adm_limit(drc_in, lim_P15,lim_N15,S);
	 	dlc_out  = adm_limit(dlc_in, lim_P15,lim_N15,S);
	 	droe_out = adm_limit(droe_in,lim_P25,lim_N30,S);
	 	drie_out = adm_limit(drie_in,lim_P25,lim_N30,S);
	 	dlie_out = adm_limit(dlie_in,lim_P25,lim_N30,S);
	 	dloe_out = adm_limit(dloe_in,lim_P25,lim_N30,S);
	 	dr_out   = adm_limit(dr_in,  lim_P10,lim_N10,S);
	 	dle_out  = adm_limit(dle_in, lim_P10,lim_N10,S);
	 	tss_out  = adm_limit(tss_in,1,0,S);
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
static void mdlDerivatives(SimStruct *S) {}
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

