/*  Copyright (C) 1996 Saab Military Aircraft, S-581 88 Linköping Sweden			*/
/*  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION.			*/
/*  This program software may be copied and distributed in any medium,			*/
/*  free of charge, provided that the copyright statement and these terms and		*/
/*  conditions are kept intact and included in every copy. The software may		*/
/*  be modified or included, in part or as a whole in another program. This		*/
/*  modified software or program may be copied and distributed under the above		*/
/*  mentioned terms, providing that the nature of changes, who changed it and		*/
/*  when it was changed are clearly stated in the program, preferably in		*/
/*  conjunction with this copyright notice. As this is a free of charge			*/
/*  software it is provided "as is" without warranty of any kind including,		*/
/*  but not limited to, the implied warranties of merchantability and fitness		*/
/*  for a particular purpose. The copyright holder or any other part  who has		*/
/*  modified and/or redistributed the software or any thereupon based program,		*/
/*  in accordance with above noted terms, may not be held responsible for any		*/
/*  kind of damages caused by the use or inability to use the program.			*/
/*											*/
/*  Modified by L Forssell to be used as a Mex-file in Simulink and Real-		*/
/*  Time WorkShop.						         		*/
/*  This version do not contain landing gear and airbrakes. It is based			*/
/*  upon the third version of GAM delivered by SAAB MA.					*/
/*											*/
/*  File "admire_main.c"			    					*/
/*  Version 2.0.5, 2001-04-05								*/
/*  Change log:		    							        */
/*  1.  991026      nnt       trouble shooting                 				*/
/*	 2.  991202	     fll		   changed "ssSetSampleTime"		*/
/*	 3.  991208	     fll		   integrate uncertainties		*/
/*  4.  000313		  fll		   added extra outputs                           */
/*  5.  000605      nnt       moved code from mdlStart to mdlOutputs                    */
/*  6.  2001-04-05: Gunnar Hovmark. Corrected calculation of inertial coupling          */
/*                  terms C3, C5 and C8 according to e-mail from Binh Dang-Vu           */
/*                  of ONERA Salon.                                                     */
/*                  According to Binh, the effect of the error was minor.               */
/*                  One thing you can see is a change in roll time constant.            */
/*  7. 2001-05-22:  Maria Sjöblom: A number of calls to ssSetErrorStatus have been      */
/*                  replaced by calls to ssSetStopRequested. This means that if you     */
/*                  run batches of simulations, the script will not crash just because  */
/*                  one simulation exceeds the envelope limits.                         */
/*                  You must yourself perform the check if each simulation was          */
/*                  completed or if it was stopped prematurely. E.g. by checking the    */
/*                  last element in the tout vector.                                    */
/*  8. 2001-11-20:  Anton Vooren: Data for high angle of attack has been added in the   */
/*				        of new coefficients. These are included as aerodata_high.h and      */
/*				        setup_aerodata_high.h. The envelope limit in respect to alpha has   */
/*					     been changed to 70 degrees.									    	*/
/* 9. 2002-12-16:   Gunnar Hovmark: Adaptation to MATLAB 6.5. All code in mdlOutputs    */
/*                  that requires access to the input ports has been moved to mdlUpdate.*/
/*                  Added printed messages when ssSetStopRequested is called.           */
/* 10. 2002-12-30:  Gunnar Hovmark: Moving code to mdlUpdate was not a good idea.       */
/*                  The code is now moved to mdlDerivatives instead. Executed only      */
/*                  at time = 0.     
/* 11. 2004? 		  Lars Forssell: Including aerodata_asym_ext.h and setup_aerodata_asym_ext.h */
/*                  instead of aerodata_asym.h and setup_aerodata_asym.h                */
/* 12. 2006-03-22:  Sign change for last term of C8 calculation, i.e. it's now back to  */
/*                  its original form. */  
/*																													 */
/*  Used variables:																							 */
/*																													 */
/*      Input variables: (port 1)																*/
/*      drc_in       right canard angle                      [rad]					*/
/*      dlc_in       left canard angle                       [rad]					*/
/*      droe_in      right inboard elevon angle              [rad]					*/
/*      drie_in      right outboard elevon angle             [rad]					*/
/*      dlie_in      left outboard elevon angle              [rad]					*/
/*      dloe_in      left inboard elevon angle               [rad]					*/
/*      dr_in        rudder angle                            [rad]					*/
/*      dle_in       leading-edge flap angle                 [rad]					*/
/*	     ldg_in       landing gear		             [0/1]					*/
/*      tss_in       trottle setting                         [-]					*/
/*		  dty_in		 engine nozzle deflection in xy-plane	 [rad]					*/
/*		  dtz_in		 engine nozzle deflection in xz-plane	 [rad]					*/
/*      u_dist       turbulence in x axis                    [m/s]					*/
/*      v_dist                     y axis                    [m/s]					*/
/*      w_dist                     z axis                    [m/s]					*/
/*      p_dist       turbulence around roll axis             [rad/s]				*/
/*																					*/
/*      Configuration parameters: (port 2)											*/
/*		mass         aircraft mass							 [kg]					*/
/*		xcg	         location of cg on x axis				 [m]					*/
/*		ycg	                           y axis				 [m]					*/
/*		zcg	                           z axis				 [m]					*/
/*		Ix			 Moment of inertia around x-axis		 [kgm^2]				*/
/*		Iy									  y-axis		 [kgm^2]				*/
/*		Iz									  z-axis		 [kgm^2]				*/
/*		Ixz			 Deviation moment						 [kgm^2]				*/
/*																					*/
/*      Uncertainty parameters: (port 3)											*/
/*		dCl_beta	 error in roll stability derivate		 [-]					*/
/*		dCm_alpha	 error in pitch	stability derivate		 [-]					*/
/*		dCn_beta	 error in yaw stability derivate		 [-]					*/
/*		dCl_da		 error in roll control power derivate    [%]					*/
/*		dCm_de		 error in pitch control power derivate   [%]					*/
/*		dCn_da		 error in yaw control power derivate     [%]					*/
/*		dxcg	     error of cg position in x axis			 [m]					*/
/*		dycg	                             y axis			 [m]					*/
/*		dzcg	                             z axis			 [m]
/*		dmass	     error in aircraft mass					 [%]					*/
/*		dIxx	     error in inertia moment around x axis   [%]					*/
/*		dIyy	                             around y axis   [%]					*/
/*		dIzz	                             around z axis   [%]					*/
/*																					*/
/*      State variables:															*/
/*      Vt_st        airspeed                                [m/s]					*/
/*      alpha_st     angle of attack                         [rad]					*/
/*      beta_st      angle of sideslip                       [rad]					*/
/*      pb_st        roll angular rate                       [rad/s]				*/
/*      qb_st        pitch angular rate                      [rad/s]				*/
/*      rb_st        yaw angular rate                        [rad/s]				*/
/*      psi_st       euler angles (azimuth)                  [rad]					*/
/*      theta_st                  (elevation)                [rad]					*/
/*      phi_st                    (bank)                     [rad]					*/
/*      x_st         x position in Fv                        [m]					*/
/*      y_st         y                                       [m]					*/
/*      z_st         z                                       [m]					*/
/*																					*/
/*      State derivative:															*/
/*      Vt_der       rate of change in airspeed              [m/s^2]				*/
/*      alpha_der    rate of change in alpha_st              [rad/s]				*/
/*      beta_der     rate of change in beta_st               [rad/s]				*/
/*      pb_der       rate of change in pb_st                 [rad/s^2]				*/
/*      qb_der       rate of change in qb_st                 [rad/s^2]				*/
/*      rb_der       rate of change in rb_st                 [rad/s^2]				*/
/*      psi_der      rate of change in psi_st                [rad/s]				*/
/*      theta_der    rate of change in theta_st              [rad/s]				*/
/*      phi_der      rate of change in phi_st                [rad/s]				*/
/*      x_der        rate of change in x_st                  [m/s]					*/
/*      y_der        rate of change in y_st                  [m/s]					*/
/*      z_der        rate of change in z_st                  [m/s]					*/
/*																					*/
/*      Output variables:															*/
/*      Vt_out       airspeed.                               [m/s]					*/
/*      alpha_out    angle of attack.                        [rad]					*/
/*      beta_out     angle of sideslip.                      [rad]					*/
/*      pb_out       roll angular rate.                      [rad/s]				*/
/*      qb_out       pitch angular rate.                     [rad/s]				*/
/*      rb_out       yaw angular rate.                       [rad/s]				*/
/*      psi_out      euler angles (azimuth)                  [rad]					*/
/*      theta_out                 (elevation)                [rad]					*/
/*      phi_out                   (bank)                     [rad]					*/
/*      x_out        x position in Fv                        [m]					*/
/*      y_out        y                                       [m]					*/
/*      z_out        z                                       [m]					*/
/*      ub_out       velocity in x-axis                      [m/s]					*/
/*      vb_out                   y-axis                      [m/s]					*/
/*      wb_out                   z-axis                      [m/s]					*/
/*      uv_out       velocity in earth parallell x-axis      [m/s]					*/
/*      vv_out                                   y-axis      [m/s]					*/
/*      wv_out                                   z-axis      [m/s]					*/
/*      nz_out       normal acceleration of c.g.             ["g"]					*/
/*      ny_out       side acceleration of c.g.               ["g"]					*/
/*      mach_out     mach number                             [-]					*/
/*      gamma_out    climb angle                             [rad]					*/
/*      cd_out       drag coefficient                        [-]					*/
/*      cl_out       lift coefficient                        [-]					*/
/*      cc_out       side force coefficient                  [-]					*/
/*      crm_out      roll coefficient                        [-]					*/
/*      cpm_out      pitch coefficient                       [-]					*/
/*      cym_out      yaw coefficient                         [-]					*/
/*																					*/
/*      Initial Conditions:															*/
/*      Vt_init      initial airspeed.						 [m/s]					*/
/*      alpha_init   initial angle of attack.				 [rad]					*/
/*      beta_init    initial angle of sideslip.				 [rad]					*/
/*      pb_init      initial roll angular rate.				 [rad/s]				*/
/*      qb_init      initial pitch angular rate.			 [rad/s]				*/
/*      rb_init      initial yaw angular rate.				 [rad/s]				*/
/*      psi_init     initial euler angles (azimuth)			 [rad]					*/
/*      theta_init   initial              (elevation)		 [rad]					*/
/*      phi_init     initial              (bank)			 [rad]					*/
/*      x_init       initial x position in Fv				 [m]					*/
/*      y_init       initial y								 [m]					*/
/*      z_init       initial z								 [m]					*/
/*																					*/
/*		Real work vector: (Persistant memory)										*/
/*      mach         mach number							 [m]					*/
/*      nz           normal acceleration of c.g.             ["g"]					*/
/*      ny           side acceleration of c.g.               ["g"]					*/
/*      alpha_dot    rate of change in alpha_st              [rad/s]				*/
/*      beta_dot     rate of change in beta_st               [rad/s]				*/
/*      p_dot        rate of change in pb_st                 [rad/s^2]				*/
/*      q_dot        rate of change in qb_st                 [rad/s^2]				*/
/*      C1           coefficients used in moment equations   [-]					*/
/*      C2                                                   [-]					*/
/*      C3                                                   [-]					*/
/*      C4                                                   [-]					*/
/*      C5                                                   [-]					*/
/*      C6                                                   [-]					*/
/*      C7                                                   [-]					*/
/*      C8                                                   [-]					*/
/*      C9                                                   [-]					*/
/*      Cttot		 tangential force coefficient            [-]					*/
/*      Cntot        normal force coefficient                [-]					*/
/*      Cctot        side force coefficient                  [-]					*/
/*      Crmtot       roll moment coefficient                 [-]					*/
/*      Cpmtot       pitch moment coefficient                [-]					*/
/*      Cymtot       yaw moment coefficient                  [-]					*/
/*		totXForce	 total force in body fixed x-axis		 [N]					*/
/*		totZForce	 total force in body fixed z-axis		 [N]					*/
/*		totPitchMom	 total moment around y-axix			 [Nm]					*/

#define S_FUNCTION_LEVEL 2
#define S_FUNCTION_NAME  admire_main

#define drc_in			(*u[0])
#define dlc_in			(*u[1])
#define droe_in			(*u[2])
#define drie_in			(*u[3])
#define dlie_in			(*u[4])
#define dloe_in			(*u[5])
#define dr_in			(*u[6])
#define dle_in			(*u[7])
#define ldg_in			(*u[8])
#define tss_in			(*u[9])
#define dty_in			(*u[10])
#define dtz_in			(*u[11])
#define u_dist			(*u[12])
#define v_dist			(*u[13])
#define w_dist			(*u[14])
#define p_dist			(*u[15])

#define mass			(*param[0])
#define	xcg				(*param[1])
#define ycg				(*param[2])
#define zcg				(*param[3])
#define Ix				(*param[4])
#define Iy				(*param[5])
#define Iz				(*param[6])
#define Ixz				(*param[7])

#define dmass			(*error[0])
#define	dxcg			(*error[1])
#define	dycg			(*error[2])
#define	dzcg			(*error[3])
#define dIxx			(*error[4])
#define dIyy			(*error[5])
#define dIzz			(*error[6])
#define dIxz			(*error[7])
#define	dCl_beta		(*error[8])
#define	dCl_p			(*error[9])
#define	dCl_r			(*error[10])
#define	dCl_day			(*error[11])
#define	dCl_dai			(*error[12])
#define	dCl_dr			(*error[13])
#define dCm_alpha		(*error[14])
#define dCm_q			(*error[15])
#define dCm_dne			(*error[16])
#define dCm_dey			(*error[17])
#define dCm_dei			(*error[18])
#define	dCn_zero		(*error[19])
#define	dCn_beta		(*error[20])
#define	dCn_p			(*error[21])
#define	dCn_r			(*error[22])
#define	dCn_dna			(*error[23])
#define	dCn_dr			(*error[24])

#define Vt_st           x[0]
#define alpha_st        x[1]
#define beta_st         x[2]
#define pb_st           x[3]
#define qb_st           x[4]
#define rb_st           x[5]
#define psi_st          x[6]
#define theta_st        x[7]
#define phi_st          x[8]
#define x_st            x[9]
#define y_st            x[10]
#define z_st            x[11]

#define Vt_der          dx[0]
#define alpha_der       dx[1]
#define beta_der        dx[2]
#define pb_der          dx[3]
#define qb_der          dx[4]
#define rb_der          dx[5]
#define psi_der         dx[6]
#define theta_der       dx[7]
#define phi_der         dx[8]
#define x_der           dx[9]
#define y_der           dx[10]
#define z_der           dx[11]

#define Vt_out          y[0]
#define alpha_out       y[1]
#define beta_out        y[2]
#define pb_out          y[3]
#define qb_out          y[4]
#define rb_out          y[5]
#define psi_out         y[6]
#define theta_out       y[7]
#define phi_out         y[8]
#define x_out           y[9]
#define y_out           y[10]
#define z_out           y[11]
#define ub_out          y[12]
#define vb_out          y[13]
#define wb_out          y[14]
#define uv_out          y[15]
#define vv_out          y[16]
#define wv_out          y[17]
#define nz_out          y[18]
#define ny_out          y[19]
#define mach_out        y[20]
#define gamma_out       y[21]
#define cd_out          y[22]
#define cl_out          y[23]
#define cc_out          y[24]
#define crm_out         y[25]
#define cpm_out         y[26]
#define cym_out         y[27]
#define totXForce_out   y[28]
#define totZForce_out   y[29]
#define totPitchMom_out y[30]

#define mach			ssGetRWork(S)[0]
#define nz				ssGetRWork(S)[1]
#define ny				ssGetRWork(S)[2]
#define alpha_dot		ssGetRWork(S)[3]
#define beta_dot		ssGetRWork(S)[4]
#define p_dot			ssGetRWork(S)[5]
#define q_dot			ssGetRWork(S)[6]
#define C1				ssGetRWork(S)[7]
#define C2              ssGetRWork(S)[8]
#define C3              ssGetRWork(S)[9]
#define C4              ssGetRWork(S)[10]
#define C5              ssGetRWork(S)[11]
#define C6              ssGetRWork(S)[12]
#define C7              ssGetRWork(S)[13]
#define C8              ssGetRWork(S)[14]
#define C9              ssGetRWork(S)[15]
#define Cttot			ssGetRWork(S)[16]
#define Cntot			ssGetRWork(S)[17]
#define Cctot			ssGetRWork(S)[18]
#define Crmtot			ssGetRWork(S)[19]
#define Cpmtot			ssGetRWork(S)[20]
#define Cymtot			ssGetRWork(S)[21]
#define totXForce		ssGetRWork(S)[22]
#define totZForce		ssGetRWork(S)[23]
#define totPitchMom		ssGetRWork(S)[24]

#define r_t_d           57.29577951
#define r_t_d_05        28.64788976
#define degtorad        0.017453293
#define Pi				3.141592654

#include <math.h>

#include "tmwtypes.h"
#include "simstruc.h"

#include "admire_data.h"
#include "admire_test.h"
#include "admire_tvc.h"

#include "aerodata_sym.h"
#include "setup_aerodata_sym.h"
			/* High aoa */
#include "aerodata_high.h"
#include "setup_aerodata_high.h"

#include "aerodata_asym_ext.h"
#include "setup_aerodata_asym_ext.h"
#include "aerodata_fuel01.h"
#include "setup_aerodata_fuel01.h"
#include "aerodata_fuel02.h"
#include "setup_aerodata_fuel02.h"
#include "aerodata_thrust01.h"
#include "setup_aerodata_thrust01.h"
#include "aerodata_thrust02.h"
#include "setup_aerodata_thrust02.h"
#include "calc_sym_coeff.h"
#include "calc_asym_coeff.h"



/* ============================================================================ */
/* ===== Function: mdlInitializeSizes ========================================= */
/* ============================================================================ */
static void mdlInitializeSizes(SimStruct *S) {
    
    ssSetNumSFcnParams(S,2);		/* Number of expected parameters */
	ssSetNumContStates(S,12);		/* Number of states */
    ssSetNumDiscStates(S,0);
	ssSetNumInputPorts(S,3);		/* Number of input ports */
    ssSetInputPortWidth(S,0,16);	/* Widht of input port no 1 */
    ssSetInputPortWidth(S,1,8);		/* Widht of input port no 2 */
    ssSetInputPortWidth(S,2,25);	/* Widht of input port no 3 */
    ssSetNumOutputPorts(S,1);		/* Number of output ports */
    ssSetOutputPortWidth(S,0,31);	/* Widht of output port */
	ssSetNumSampleTimes(S,1);		/* Number of sample times */
    ssSetNumRWork(S,25);			/* Size of real work vector */
    ssSetNumIWork(S,0);
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

#ifdef PRINTOUTS
   printf("MDL_INITIALIZE_CONDITIONS ");
#endif

	if (ssGetNumSFcnParams(S) != 2) {		/* Check no. of input parameters */
		ssSetErrorStatus(S,"Wrong number of functions parameters");
		return;
	}
	if (mxGetNumberOfElements(ssGetSFcnParam(S,0)) != 12) {
		ssSetErrorStatus(S,"All 12 states must be initiated");
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
   real_T *x  = ssGetContStates(S);
   int i;

#ifdef PRINTOUTS
   printf("MDL_START ");
#endif

	/* Initialise state vector */
	for (i=0;i<12;i++)
	{
		x[i]  = mxGetPr(ssGetSFcnParam(S,0))[i];
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

	real_T *x  = ssGetContStates(S);
	real_T *y  = ssGetOutputPortRealSignal(S,0);
	
	double Cph,Cth,Cps,Sph,Sth,Sps;
   int i;

#ifdef PRINTOUTS
   printf("MDL_OUTPUTS\n ");
#endif

	for(i=0;i<12;i++) /* copy state vector to part of output vector */
		y[i] = x[i];

	ub_out = Vt_st*cos(alpha_st)*cos(beta_st); /* additional output calculations */
	vb_out = Vt_st*sin(beta_st);
	wb_out = Vt_st*sin(alpha_st)*cos(beta_st);

	Cph = cos(phi_st);
	Cth = cos(theta_st);
	Cps = cos(psi_st);

	Sph = sin(phi_st);
	Sth = sin(theta_st);
	Sps = sin(psi_st);

	uv_out =  Cth*Cps*ub_out+(Sph*Sth*Cps-Cph*Sps)*vb_out+(Cph*Sth*Cps+Sph*Sps)*wb_out;
	vv_out =  Cth*Sps*ub_out+(Sph*Sth*Sps+Cph*Cps)*vb_out+(Cph*Sth*Sps-Sph*Cps)*wb_out;
	wv_out = -Sth*ub_out+Sph*Cth*vb_out+Cph*Cth*wb_out;

	nz_out = nz;
	ny_out = ny;

	mach_out  = mach;
	gamma_out = asin(cos(alpha_st)*cos(beta_st)*sin(theta_st)-(sin(phi_st)*sin(beta_st)+
		        cos(phi_st)*sin(alpha_st)*cos(beta_st))*cos(theta_st));

	cd_out = sin(alpha_st)*Cntot+cos(alpha_st)*Cttot;
	cl_out = cos(alpha_st)*Cntot-sin(alpha_st)*Cttot;
	cc_out = Cctot;

	crm_out = Crmtot;
	cpm_out = Cpmtot;
	cym_out = Cymtot;

	totXForce_out   = totXForce;
	totZForce_out   = totZForce;
	totPitchMom_out = totPitchMom;

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
#undef MDL_UPDATE  /* Change to #define to use function */
#if defined(MDL_UPDATE)
static void mdlUpdate(SimStruct *S, int_T tid) {	

#ifdef PRINTOUTS
   printf("MDL_UPDATE ");
#endif

}
#endif


/* ============================================================================ */
/* ===== Function: mdlDerivatives ============================================= */
/* ============================================================================ */
/* ===== Abstract:                                                        ===== */
/* =====   In this function, you compute the S-function block's           ===== */
/* =====   derivatives. The derivatives are placed in the derivative      ===== */
/* =====   vector, ssGetdX(S).                                            ===== */
/* ============================================================================ */
#define MDL_DERIVATIVES  /* Change to #undef to remove function */
#if defined(MDL_DERIVATIVES)
static void mdlDerivatives(SimStruct *S) { /* tjohooo */

	real_T *x  = ssGetContStates(S);
	real_T *dx = ssGetdX(S);

	InputRealPtrsType u     = ssGetInputPortRealSignalPtrs(S,0);
	InputRealPtrsType param = ssGetInputPortRealSignalPtrs(S,1);
	InputRealPtrsType error = ssGetInputPortRealSignalPtrs(S,2);

	double Gama;
	double rho,p_atm,T_atm,qa,qacorr;
	double tflow,tfhigh,cd2v,bd2v;
	double ubody,vbody,wbody,uderiv,vderiv,wderiv,Vt_new,Vt_dist, vtder;
	double Fx,Fy,Fz,Mx,My,Mz;
	double Cph,Cth,Cps,Sph,Sth,Sps;
	double Tx,Ty,Tz,Thrust,Thrust_low,Thrust_high,m_dot_fuel,cai,ldg;
	double m_dot_fuel_low,m_dot_fuel_high;
	double dei,dey,de,dai,adai,sidai,da,day,aday,siday,dne,dn,dna,dle,dr,adr;
	double alpha,aalpha,alphad,beta,abeta,sibeta,betad,h_geo,pc,qc,rc;
	double mach_xb;
	
	int	look = 1 , i, nnt;

#ifdef PRINTOUTS
      printf("MDL_DERIVATIVES ");
#endif
	if(ssGetT(S)==0) {
   /* The code below is executed only when time is 0. */
   /************* This code moved here on 2002-12-30 *****************/
		for(nnt=0;nnt<2;nnt++) { /* Torbjörn Norén knows why this must be done twice. */
		   Tz = 0;

		   Vt_dist = sqrt(u_dist*u_dist+v_dist*v_dist+w_dist*w_dist);
		   Vt_new  = sqrt(Vt_st*Vt_st+Vt_dist*Vt_dist);  /* New true airspeed */

		   if (Vt_new < 0.01) {
			   cd2v = 260;       /* Limes value limit for V->0 */
			   bd2v = 500; }     /* Limes value limit for V->0 */
		   else {
			   cd2v = cref/(2.0*Vt_new);
			   bd2v = bref/(2.0*Vt_new); 
         }

		   ubody = Vt_st*cos(alpha_st)*cos(beta_st);  /* Needed for calculation of contribution */
		   vbody = Vt_st*sin(beta_st);                /* of turbulence.						 */
		   wbody = Vt_st*sin(alpha_st)*cos(beta_st);

		   alpha = atan((wbody+w_dist)/(ubody+u_dist))*r_t_d; /* Disturbed aerodynamic angles */
	 	   beta  = asin((vbody+v_dist)/Vt_new)*r_t_d;

		   aalpha = fabs(alpha);
		   abeta  = fabs(beta);

		   if (beta < 0)
			   sibeta = -1;
		   else if (beta > 0)
			   sibeta = 1;
		   else
			   sibeta = 0;

		   dei  = (dlie_in+drie_in)*r_t_d_05;  /* Compute generic control */
		   dey  = (dloe_in+droe_in)*r_t_d_05;  /* surfaces angles.		  */
		   de   = (dei+dey)*0.5;
		   dai  = (dlie_in-drie_in)*r_t_d_05;
		   adai = fabs(dai);

		   if (dai < 0)
			   sidai = -1;
		   else if (dai > 0)
			   sidai = 1;
		   else
			   sidai = 0;

		   day  = (dloe_in-droe_in)*r_t_d_05;
		   aday = fabs(day);

		   if (day < 0)
			   siday = -1;
		   else if (day > 0)
			   siday = 1;
		   else
			   siday = 0;

		   da	   = (dai+day)*0.5;
		   dne	   = (dlc_in+drc_in)*r_t_d_05;
		   dn	   =  dne;
		   dna	   = (dlc_in-drc_in)*r_t_d_05;
		   dle	   =  dle_in*r_t_d;
		   dr	   =  dr_in*r_t_d;
		   adr	   =  fabs(dr);
		   h_geo  = -z_st;               /* Set some parameters */
		   alphad =  alpha_dot*cd2v;
		   betad  =  beta_dot*bd2v;
		   pc	   = (pb_st+p_dist)*bd2v; /* Compute nondimensional rotation */
		   qc	   =  qb_st*cd2v;         /* rates and accelerations.        */
		   rc	   =  rb_st*bd2v;
		   ldg	   =  ldg_in;

		   if (h_geo<0.1) {
			   printf("Low altitude, simulation stopped.\n");
			   ssSetStopRequested(S,1);
            /*Maria*/
			   return; 
         }

		   /* ATMOSPHERIC CALCULATION */
		   if (h_geo<=11000) {
			   T_atm = 288.15-0.0065*h_geo;
			   p_atm = 101325*pow((T_atm/288.15),9.81/(287*0.0065)); 
         }
		   else {
			   T_atm = 216.65;
			   p_atm = 22632*exp(-9.81*(h_geo-11000)/(287*216.65)); 
         }

		   rho	   = p_atm/(287*T_atm);
		   qa	   = 0.5*rho*Vt_new*Vt_new;
		   qacorr = qa*0.0001;					/* (1/10000.0 = 0.0001)  */
		   mach   = Vt_st/sqrt(1.4*287*T_atm);	/* Calculate mach number */
		   mach_xb = ubody/sqrt(1.4*287*T_atm);	/* Calculate mach number parallel to the body x-axis */

		   if (testAdmEnvelope(mach,alpha_st,beta_st)) {
			   printf("Model out of envelope, simulation stopped.\n");
			   ssSetStopRequested(S,1); /*Maria*/
			   return; 
         }

		   /* THRUST CALCULATIONS */
		   if (tss_in<0.78)
			   Thrust = thrust013(&thrust01,tss_in,h_geo,mach_xb,look);
		   else if (tss_in>= 0.78 && tss_in < 0.82) {
			   Thrust_low  = thrust013(&thrust01,0.78,h_geo,mach_xb,look);
			   Thrust_high = thrust023(&thrust02,0.82,h_geo,mach_xb,look);
			   Thrust      = (Thrust_high-Thrust_low)*(tss_in-0.78)*25.0+Thrust_low; /* 1/0.04 = 25 */ }
		   else
			   Thrust = thrust023(&thrust02,tss_in,h_geo,mach_xb,look);

		   /* Old thrust increase */
		   /* Thrust = 1.2*Thrust; */

		   /* Better with the following? */
		   Thrust = ( tss_in*0.4+0.8 ) * Thrust;
		   /* Gives a linearly varying thrust increase factor with
		        Thrust = 0.8*Thrust
		      when tss_in = 0 and
		        Thrust = 1.2*Thrust
		      when tss_in = 1.0 */

		   /* FUEL CONSUMPTION */
		   if (tss_in<0.78)
			   m_dot_fuel = fuel013(&fuel01,tss_in,h_geo,mach_xb,look);
		   else if (tss_in>= 0.78 && tss_in < 0.82) {
			   m_dot_fuel_low  = fuel013(&fuel01,0.78,h_geo,mach_xb,look);
			   m_dot_fuel_high = fuel023(&fuel02,0.82,h_geo,mach_xb,look);
			   m_dot_fuel      = (m_dot_fuel_high-m_dot_fuel_low)*
				(tss_in-0.78)*25.0+m_dot_fuel_low; /* 1/0.04 = 25 */ }
		   else
			   m_dot_fuel = fuel023(&fuel02,tss_in,h_geo,mach_xb,look);

		   /* Old increase in fuel consumption */
		   /* m_dot_fuel = 1.2*m_dot_fuel/3600.0; */
		   /* Corresponding modification to the fuel consumption as to the thrust above */
		   m_dot_fuel = ( tss_in*0.4+0.8 ) * m_dot_fuel/3600.0;

		   /* TVC */
		   engVectorCalculation(Thrust,tss_in,rho,m_dot_fuel,Vt_st,alpha_st,beta_st,
			dty_in,dtz_in,&Tx,&Ty,&Tz,&cai);

		   /* TRANSITION FACTORS */
		   if (mach>0.4 && mach<0.5) {				/* Compute transition factors */
			   tflow  = (0.5-mach)*10.0;			/* low M -> high M data       */
			   tfhigh = (mach-0.4)*10.0; 
         }

		   /* ALGORITHMS FOR FORCE COEFFICIENTS  */
		   Cttot = getCttotCoeff(h_geo,mach,alpha,beta,dei,dey,de,dn,dle,aalpha,
			                  abeta,dai,day,adai,aday,adr,tflow,tfhigh,ldg,look);
		   Cntot = getCntotCoeff(h_geo,mach,alpha,beta,dei,dey,de,dn,dle,aalpha,abeta,
			                  dai,day,adai,aday,adr,tflow,tfhigh,qacorr,cai,alphad,
			                  qc,nz,q_dot,ldg,look);
		   Cctot = getCctotCoeff(mach,alpha,beta,abeta,aalpha,sibeta,dei,dey,dai,day,
			                  adai,aday,sidai,siday,dn,dne,dna,dle,dr,betad,pc,rc,tflow,
			                  tfhigh,qacorr,cai,ldg,look);

		   /* ALGORITHMS FOR MOMENT COEFFICIENT */
		   Crmtot = getCrmtotCoeff(mach,alpha,aalpha,beta,betad,abeta,sibeta,dei,dey,dai,
			                    adai,sidai,day,aday,siday,dn,dne,dna,dle,dr,pc,rc,qacorr,
			                    tflow,tfhigh,ldg,look,error);

		   Cpmtot = getCpmtotCoeff(mach,alpha,beta,dei,dey,de,dn,dle,aalpha,abeta,
			                    dai,day,adai,aday,adr,tflow,tfhigh,qacorr,cai,
			                    alphad,qc,nz,q_dot,ldg,look,error);

#ifdef PRINTOUTS
         printf("Initializing...\n");
		   printf("mach\t\t%10.5f\n",mach); 
		   printf("alpha\t\t%10.5f\n",alpha); 
		   printf("beta\t\t%10.5f\n",beta); 
		   printf("dei\t\t%10.5f\n",dei); 
		   printf("dey\t\t%10.5f\n",dey); 
		   printf("de\t\t%10.5f\n",de); 
		   printf("dn\t\t%10.5f\n",dn); 
		   printf("dle\t\t%10.5f\n",dle); 
		   printf("aalpha\t\t%10.5f\n",aalpha); 
		   printf("abeta\t\t%10.5f\n",abeta); 
		   printf("dai\t\t%10.5f\n",dai); 
		   printf("day\t\t%10.5f\n",day); 
		   printf("adai\t\t%10.5f\n",adai); 
		   printf("aday\t\t%10.5f\n",aday); 
		   printf("adr\t\t%10.5f\n",adr); 
		   printf("tflow\t\t%10.5f\n",tflow); 
		   printf("tfhigh\t\t%10.5f\n",tfhigh); 
		   printf("qacorr\t\t%10.5f\n",qacorr); 
		   printf("cai\t\t%10.5f\n",cai); 
		   printf("alphad\t\t%10.5f\n",alphad); 
		   printf("qc\t\t%10.5f\n",qc); 
		   printf("nz\t\t%10.5f\n",nz); 
		   printf("q_dot\t\t%10.5f\n",q_dot); 
		   printf("ldg\t\t%10.5f\n",ldg); 
		   printf("look\t\t%d\n",look); 
		   printf("error\t\t%d\n\n",error); 
#endif

		   Cymtot = getCymtotCoeff(mach,alpha,aalpha,beta,abeta,sibeta,betad,dei,dey,dai,day,
			                    adai,aday,sidai,siday,dn,dne,dna,dle,dr,pc,rc,cai,tflow,
			                    tfhigh,qacorr,ldg,look,error);

		   Fx = -qa*Sref*Cttot; /* Force and moment calculation */
		   Fy = -qa*Sref*Cctot; /* changed sign                 */
		   Fz = -qa*Sref*Cntot;
		   Mx =  qa*Sref*bref*Crmtot-(zcg+dzcg)*Fy+(ycg+dycg)*Fz;                /* Transformed forces to the */
		   My =  qa*Sref*cref*Cpmtot-(xcg+dxcg)*Fz+(zcg+dzcg)*Fx+Tx*zeng+Tz*5.5; /* center of grav.           */
		   Mz =  qa*Sref*bref*Cymtot+(xcg+dxcg)*Fy-(ycg+dycg)*Fx-Tx*yeng-Ty*5.5;

#ifdef PRINTOUTS
         printf("My     --> %10.5f\n",My); 
		   printf("qa     --> %10.5f\n",qa); 
		   printf("Sref   --> %10.5f\n",Sref); 
		   printf("cref   --> %10.5f\n",cref); 
		   printf("Cpmtot --> %10.5f\n",Cpmtot); 
		   printf("xcg    --> %10.5f\n",xcg); 
		   printf("dxcg   --> %10.5f\n",dxcg); 
		   printf("Fz     --> %10.5f\n",Fz); 
		   printf("zcg    --> %10.5f\n",zcg); 
		   printf("dzcg   --> %10.5f\n",dzcg); 
		   printf("Fx     --> %10.5f\n",Fx); 
		   printf("Tx     --> %10.5f\n",Tx); 
		   printf("zeng   --> %10.5f\n",zeng); 
		   printf("Tz     --> %10.5f\n\n",Tz); 
#endif

		   uderiv =  rb_st*vbody-qb_st*wbody-grav*sin(theta_st)+(Fx+Tx)/(mass*(1+dmass));
		   vderiv = -rb_st*ubody+pb_st*wbody+grav*sin(phi_st)*cos(theta_st)+(Fy+Ty)/(mass*(1+dmass));
		   wderiv =  qb_st*ubody-pb_st*vbody+grav*cos(phi_st)*cos(theta_st)+(Fz+Tz)/(mass*(1+dmass));

		   ubody  = Vt_st*cos(alpha_st)*cos(beta_st);
		   vbody  = Vt_st*sin(beta_st);
		   wbody  = Vt_st*sin(alpha_st)*cos(beta_st);

		   vtder  = (ubody*uderiv+vbody*vderiv+wbody*wderiv)/Vt_st;

		   alpha_dot = (ubody*wderiv-wbody*uderiv)/(ubody*ubody+wbody*wbody);
		   beta_dot  = (vderiv*Vt_st-vbody*vtder)/(Vt_st*Vt_st*cos(beta_st));
		
		   Gama = Ix*(1+dIxx)*Iz*(1+dIzz)-Ixz*(1+dIxz)*Ixz*(1+dIxz);						/* Calculate inertia coefficients */
		   C1	 = ((Iy*(1+dIyy)-Iz*(1+dIzz))*Iz*(1+dIzz)-Ixz*(1+dIxz)*Ixz*(1+dIxz))/Gama;	/* in order to gain speed         */
		   C2	 = ((Ix*(1+dIxx)-Iy*(1+dIyy)+Iz*(1+dIzz))*Ixz*(1+dIxz))/Gama;
  /*		C3	 = Iy*(1+dIyy)/Gama; Replaced by the following line on 2001-04-05  */
	      C3       = Iz*(1+dIzz)/Gama;
		   C4	 = Ixz*(1+dIxz)/Gama;
 /*		C5	 = (Iy*(1+dIyy)-Ix*(1+dIxx))/(Iy*(1+dIyy)); Replaced by the following line on 2001-04-05  */
		   C5       = (Iz*(1+dIzz)-Ix*(1+dIxx))/(Iy*(1+dIyy)); 
		   C6	 = Ixz*(1+dIxz)/(Iy*(1+dIyy));
		   C7	 = 1/(Iy*(1+dIyy));
 		   C8	 = (Ix*(1+dIxx)*(Ix*(1+dIxx)-Iy*(1+dIyy))+Ixz*(1+dIxz)*Ixz*(1+dIxz))/Gama; /* Replaced by the following line on 2001-04-05 */
         	/* C8    = (Ix*(1+dIxx)*(Ix*(1+dIxx)-Iy*(1+dIyy))-Ixz*(1+dIxz)*Ixz*(1+dIxz))/Gama; Changed back on 2006-03-22. */
		   C9	 = (Ix*(1+dIxx))/Gama;

		   p_dot = (C1*rb_st+C2*pb_st)*qb_st+C3*Mx+C4*Mz;
		   q_dot =  C5*pb_st*rb_st-C6*(pb_st*pb_st-rb_st*rb_st)+C7*My;

		   nz = -Fz/(mass*(1+dmass)*grav);
		   ny = -Fy/(mass*(1+dmass)*grav);

		   totXForce   = -grav*mass*(1+dmass)*sin(theta_st)+(Fx+Tx);
		   totZForce   =  grav*mass*(1+dmass)*cos(phi_st)*cos(theta_st)+(Fz+Tz);
         totPitchMom =  qa*Sref*cref*Cpmtot-(xcg+dxcg)*Fz+(zcg+dzcg)*Fx+Tx*zeng+Tz*5.5;
		}
		/******** End of Torbjörn's code that was moved here from mdlOutputs via mdlUpdate *******/
	
		for (i=0;i<12;i++)
		{
			dx[i]  = mxGetPr(ssGetSFcnParam(S,1))[i];
		}

	}

	if (testAdmEnvelope(mach,alpha_st,beta_st)) {
	   printf("Model out of envelope, simulation stopped\n");
		ssSetStopRequested(S,1);
        /*Maria*/
		return; }

	Vt_dist = sqrt(u_dist*u_dist+v_dist*v_dist+w_dist*w_dist);
	Vt_new  = sqrt(Vt_st*Vt_st+Vt_dist*Vt_dist);       /* New true airspeed */

	if (Vt_new < 0.01) {
		cd2v = 260;             /* Limes value limit for V->0 */
		bd2v = 500;             /* Limes value limit for V->0 */ }
	else {
		cd2v = cref/(2.0*Vt_new);
		bd2v = bref/(2.0*Vt_new); }

	ubody = Vt_st*cos(alpha_st)*cos(beta_st);  /* Needed for calculation of   */
	vbody = Vt_st*sin(beta_st);                /* contribution of turbulence. */
	wbody = Vt_st*sin(alpha_st)*cos(beta_st);

	alpha = atan((wbody+w_dist)/(ubody+u_dist))*r_t_d; /* Disturbed aerodynamic angles */
	beta  = asin((vbody+v_dist)/Vt_new)*r_t_d;

	aalpha = fabs(alpha);
	abeta  = fabs(beta);

	if (beta < 0)
		sibeta = -1;
	else if (beta > 0)
		sibeta = 1;
	else 
		sibeta = 0;

	dei  = (dlie_in+drie_in)*r_t_d_05;       /* Compute generic control */
	dey  = (dloe_in+droe_in)*r_t_d_05;       /* surfaces angles.        */
	de   = (dei+dey)*0.5;
	dai  = (dlie_in-drie_in)*r_t_d_05;
	adai =  fabs(dai);

	if (dai < 0)
		sidai = -1;
	else if (dai > 0)
		sidai = 1;
	else
		sidai = 0;

	day  = (dloe_in-droe_in)*r_t_d_05;
	aday = fabs(day);

	if (day < 0)
		siday = -1;
	else if (day > 0)
		siday = 1;
	else
		siday = 0;

	da     = (dai+day)*0.5;
	dne    = (dlc_in+drc_in)*r_t_d_05;
	dn     =  dne;
	dna    = (dlc_in-drc_in)*r_t_d_05;
	dle    =  dle_in*r_t_d;
	dr     =  dr_in*r_t_d;
	adr    =  fabs(dr);
	h_geo  = -z_st;                 /* Set some parameters */
	alphad =  alpha_dot*cd2v;
	betad  =  beta_dot*bd2v;
	pc     = (pb_st+p_dist)*bd2v;   /* Compute nondimensional rotation */
	qc     =  qb_st*cd2v;           /* rates and accelerations         */
	rc     =  rb_st*bd2v;
	ldg    =  ldg_in;

	if (h_geo<0.1) {
      printf("Low altitude, simulation stopped.\n");
		ssSetStopRequested(S,1);
        /*Maria*/
		return; }

/* ATMOSPHERIC CALCULATION */
    if (h_geo<=11000) {
        T_atm = 288.15-0.0065*h_geo;
        p_atm = 101325*pow((T_atm/288.15),9.81/(287*0.0065)); }
    else {
        T_atm = 216.65;
        p_atm = 22632*exp(-9.81*(h_geo-11000)/(287*216.65)); }

    rho    = p_atm/(287*T_atm);
    qa     = 0.5*rho*Vt_new*Vt_new;
    qacorr = qa*0.0001;						/* (1/10000.0 = 0.0001) */
    mach   = Vt_new/sqrt(1.4*287*T_atm);
    mach_xb= (ubody+u_dist)/sqrt(1.4*287*T_atm);

/* THRUST CALCULATIONS */
	if (tss_in<0.78)
		Thrust = thrust013(&thrust01,tss_in,h_geo,mach_xb,look);
	else if (tss_in>= 0.78 && tss_in < 0.82) {
		Thrust_low  = thrust013(&thrust01,0.78,h_geo,mach_xb,look);
		Thrust_high = thrust023(&thrust02,0.82,h_geo,mach_xb,look);
		Thrust      = (Thrust_high-Thrust_low)*(tss_in-0.78)*25.0+Thrust_low; /* 1/0.04 = 25 */ }
	else
		Thrust = thrust023(&thrust02,tss_in,h_geo,mach_xb,look);

	/* Thrust = 1.2*Thrust; */ /* Thrust correction */
	Thrust = ( tss_in*0.4+0.8 ) * Thrust; /* Better thrust correction */

/* FUEL CONSUMPTION */
	if (tss_in<0.78)
		m_dot_fuel = fuel013(&fuel01,tss_in,h_geo,mach_xb,look);
	else if (tss_in>= 0.78 && tss_in < 0.82) {
		m_dot_fuel_low  = fuel013(&fuel01,0.78,h_geo,mach_xb,look);
		m_dot_fuel_high = fuel023(&fuel02,0.82,h_geo,mach_xb,look);
		m_dot_fuel      = (m_dot_fuel_high-m_dot_fuel_low)*
			(tss_in-0.78)*25.0+m_dot_fuel_low; /* 1/0.04 = 25 */ }
	else
		m_dot_fuel = fuel023(&fuel02,tss_in,h_geo,mach_xb,look);

	/* m_dot_fuel = 1.2*m_dot_fuel/3600.0; */
	m_dot_fuel = ( tss_in*0.4+0.8 ) * m_dot_fuel;

/* TVC */
	engVectorCalculation(Thrust,tss_in,rho,m_dot_fuel,Vt_st,alpha_st,
		                 beta_st,dty_in,dtz_in,&Tx,&Ty,&Tz,&cai);

/* TRANSITION FACTORS */
	if (mach>0.4 && mach<0.5) {		  /* Compute transition factors */
		tflow  = (0.5-mach)*10.0;	  /* low M -> high M data       */
		tfhigh = (mach-0.4)*10.0; }

/* ALGORITHMS FOR FORCE COEFFICIENTS  */
	Cttot = getCttotCoeff(h_geo,mach,alpha,beta,dei,dey,de,dn,dle,aalpha,abeta,
		                  dai,day,adai,aday,adr,tflow,tfhigh,ldg,look);
	Cntot = getCntotCoeff(h_geo,mach,alpha,beta,dei,dey,de,dn,dle,aalpha,abeta,
						  dai,day,adai,aday,adr,tflow,tfhigh,qacorr,cai,alphad,
						  qc,nz,q_dot,ldg,look);
	Cctot = getCctotCoeff(mach,alpha,beta,abeta,aalpha,sibeta,dei,dey,dai,day,
						  adai,aday,sidai,siday,dn,dne,dna,dle,dr,betad,pc,rc,tflow,
						  tfhigh,qacorr,cai,ldg,look);

/* ALGORITHM FOR MOMENT COEFFICIENT */
	Crmtot = getCrmtotCoeff(mach,alpha,aalpha,beta,betad,abeta,sibeta,dei,dey,dai,
							adai,sidai,day,aday,siday,dn,dne,dna,dle,dr,pc,rc,qacorr,
							tflow,tfhigh,ldg,look,error);
	Cpmtot = getCpmtotCoeff(mach,alpha,beta,dei,dey,de,dn,dle,aalpha,abeta,
							dai,day,adai,aday,adr,tflow,tfhigh,qacorr,cai,
							alphad,qc,nz,q_dot,ldg,look,error);

#ifdef PRINTOUTS
   if (fabs(q_dot)<0.0000000001) {
	    printf("\nfabs(q_dot)<0.0000000001:\n"); 
		 printf("mach\t\t%10.10f\n",mach); 
		 printf("alpha\t\t%10.10f\n",alpha); 
		 printf("beta\t\t%10.10f\n",beta);
		 printf("dei\t\t%10.10f\n",dei); 
		 printf("dey\t\t%10.10f\n",dey); 
		 printf("de\t\t%10.10f\n",de); 
		 printf("dn\t\t%10.10f\n",dn); 
		 printf("dle\t\t%10.10f\n",dle); 
		 printf("aalpha\t\t%10.10f\n",aalpha); 
		 printf("abeta\t\t%10.10f\n",abeta); 
		 printf("dai\t\t%10.10f\n",dai); 
		 printf("day\t\t%10.10f\n",day); 
		 printf("adai\t\t%10.10f\n",adai); 
		 printf("aday\t\t%10.10f\n",aday); 
		 printf("adr\t\t%10.10f\n",adr); 
		 printf("tflow\t\t%10.10f\n",tflow); 
		 printf("tfhigh\t\t%10.10f\n",tfhigh); 
		 printf("qacorr\t\t%10.10f\n",qacorr); 
		 printf("cai\t\t%10.10f\n",cai); 
		 printf("alphad\t\t%10.10f\n",alphad); 
		 printf("qc\t\t%10.10f\n",qc); 
		 printf("nz\t\t%10.10f\n",nz); 
		 printf("q_dot\t\t%10.10f\n",q_dot); 
		 printf("ldg\t\t%10.10f\n",ldg); 
		 printf("look\t\t%d\n",look); 
		 printf("vt_st\t\t%10.10f\n",Vt_st);
		 printf("error\t\t%d\n\n",error);
   }
#endif

	Cymtot = getCymtotCoeff(mach,alpha,aalpha,beta,abeta,sibeta,betad,dei,dey,dai,day,
							adai,aday,sidai,siday,dn,dne,dna,dle,dr,pc,rc,cai,tflow,
							tfhigh,qacorr,ldg,look,error);

/* FORCE AND MOMENT CALCULATION */
    Fx = -qa*Sref*Cttot;                                     /* changed sign */
    Fy = -qa*Sref*Cctot;
    Fz = -qa*Sref*Cntot;                                     /* changed sign */
    Mx =  qa*Sref*bref*Crmtot-(zcg+dzcg)*Fy+(ycg+dycg)*Fz;                 /* transformed forces to the */
    My =  qa*Sref*cref*Cpmtot-(xcg+dxcg)*Fz+(zcg+dzcg)*Fx+Tx*zeng+Tz*5.5;  /* center of grav.           */
    Mz =  qa*Sref*bref*Cymtot+(xcg+dxcg)*Fy-(ycg+dycg)*Fx-Tx*yeng-Ty*5.5;

#ifdef PRINTOUTS
	 printf("\nValues in mdlDerivatives:\n"); 
    printf("My     --> %10.5f\n",My); 
	 printf("qa     --> %10.5f\n",qa); 
	 printf("Sref   --> %10.5f\n",Sref); 
	 printf("cref   --> %10.5f\n",cref); 
	 printf("Cpmtot --> %10.5f\n",Cpmtot); 
	 printf("xcg    --> %10.5f\n",xcg); 
	 printf("dxcg   --> %10.5f\n",dxcg); 
	 printf("Fz     --> %10.5f\n",Fz); 
	 printf("zcg    --> %10.5f\n",zcg); 
	 printf("dzcg   --> %10.5f\n",dzcg); 
	 printf("Fx     --> %10.5f\n",Fx); 
	 printf("Tx     --> %10.5f\n",Tx); 
	 printf("zeng   --> %10.5f\n",zeng); 
	 printf("Tz     --> %10.5f\n\n",Tz); 
#endif

/* DYNAMIC CALCULATIONS */
    if (Vt_new<1.0) {
        printf("Low speed, simulation stopped.\n");
        ssSetStopRequested(S,1);
        return; 
    }

    uderiv =  rb_st*vbody-qb_st*wbody-grav*sin(theta_st)+(Fx+Tx)/(mass*(1+dmass));
    vderiv = -rb_st*ubody+pb_st*wbody+grav*sin(phi_st)*cos(theta_st)+(Fy+Ty)/(mass*(1+dmass));
    wderiv =  qb_st*ubody-pb_st*vbody+grav*cos(phi_st)*cos(theta_st)+(Fz+Tz)/(mass*(1+dmass));

    Vt_der    = (ubody*uderiv+vbody*vderiv+wbody*wderiv)/Vt_st;
    alpha_der = (ubody*wderiv-wbody*uderiv)/(ubody*ubody+wbody*wbody);
    beta_der  = (vderiv*Vt_st-vbody*Vt_der)/(Vt_st*Vt_st*cos(beta_st));

    pb_der    = (C1*rb_st+C2*pb_st)*qb_st+C3*Mx+C4*Mz;
    qb_der    =  C5*pb_st*rb_st-C6*(pb_st*pb_st-rb_st*rb_st)+C7*My;
    rb_der    = (C8*pb_st-C2*rb_st)*qb_st+C4*Mx+C9*Mz;

    psi_der   = (qb_st*sin(phi_st)+rb_st*cos(phi_st))/cos(theta_st);
    theta_der =  qb_st*cos(phi_st)-rb_st*sin(phi_st);
    phi_der   =  pb_st+tan(theta_st)*(qb_st*sin(phi_st)+rb_st*cos(phi_st));

    Cph = cos(phi_st);
    Cth = cos(theta_st);
    Cps = cos(psi_st);

    Sph = sin(phi_st);
    Sth = sin(theta_st);
    Sps = sin(psi_st);

    x_der =  Cth*Cps*ubody+(Sph*Sth*Cps-Cph*Sps)*vbody+(Cph*Sth*Cps+Sph*Sps)*wbody;                                                         
    y_der =  Cth*Sps*ubody+(Sph*Sth*Sps+Cph*Cps)*vbody+(Cph*Sth*Sps-Sph*Cps)*wbody;                                                         
    z_der = -Sth*ubody+Sph*Cth*vbody+Cph*Cth*wbody;

    nz = -Fz/(mass*(1+dmass)*grav);     /* Saving data */
    ny = -Fy/(mass*(1+dmass)*grav);     /* NOTE !! Check age of data */

    p_dot = pb_der;                      
    q_dot = qb_der;

    alpha_dot = alpha_der;
    beta_dot  = beta_der;

	totXForce   = -grav*mass*(1+dmass)*sin(theta_st)+(Fx+Tx);
	totZForce   =  grav*mass*(1+dmass)*cos(phi_st)*cos(theta_st)+(Fz+Tz);
	totPitchMom =  qa*Sref*cref*Cpmtot-(xcg+dxcg)*Fz+(zcg+dzcg)*Fx+Tx*zeng+Tz*5.5;

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

