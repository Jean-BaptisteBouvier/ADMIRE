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
/*  This version does not contain landing gear and airbrakes. It is based			*/
/*  upon the third version of GAM delivered by SAAB MA.					*/
/*											*/
/*  File "admire_aerotesthigh.c"			    					*/
/*  From admire_main.c Version 2.0.5, 2001-08-15								*/
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
/*  8. 2001-08-15:  Gunnar Hovmark: More ssSetErrorStatus replaced with                 */
/*                  ssSetStopRequested.                                                 */
/*  9. 2001-09-17:  admire_aerotest.c. Similar to gamtest.f. Program that takes input   */
/*                  variables and presents aerodynamic forces and moments from the      */
/*                  aerodata tables without any flight dynamics calculations.           */
/* 10. 2001-11-21:  admire_aerotesthigh.c. Taking in the data tables for high angles of */
/*                  attack by Anton Vooren */
/* 11. 2006-03-22:  Gunnar Hovmark: Including aerodata_asym_ext.h and setup_aerodata_asym_ext.h */
/*                  instead of aerodata_asym.h and setup_aerodata_asym.h                */
/*											*/
/*  Used variables:									*/
/*											*/
/*      Input variables: (port 1)							*/
/*      drc         right canard angle                      [rad]					*/
/*      dlc         left canard angle                       [rad]					*/
/*      droe        right inboard elevon angle              [rad]					*/
/*      drie        right outboard elevon angle             [rad]					*/
/*      dlie        left outboard elevon angle              [rad]					*/
/*      dloe        left inboard elevon angle               [rad]					*/
/*      dr          rudder angle                            [rad]					*/
/*      dle         leading-edge flap angle                 [rad]					*/
/*	     ldg         landing gear		             [0/1]					*/
/*      tss         trottle setting                         [-]					*/
/*		  dty  		   engine nozzle deflection in xy-plane	 [rad]					*/
/*		  dtz  		   engine nozzle deflection in xz-plane	 [rad]					*/
/*																					*/
/*      Vt          airspeed                                [m/s]					*/
/*      alpha       angle of attack                         [rad]					*/
/*      beta        angle of sideslip                       [rad]					*/
/*      pb          roll angular rate                       [rad/s]				*/
/*      qb          pitch angular rate                      [rad/s]				*/
/*      rb          yaw angular rate                        [rad/s]				*/
/*      z           z                                       [m]					*/
/*																					*/
/*      Vt_der       airspeed time derivative               [m/s]					*/
/*      alpha_der    rate of change in alpha                [rad/s]				*/
/*      beta_der     rate of change in beta                 [rad/s]				*/
/*      pb_der       rate of change in pb                   [rad/s^2]				*/
/*      qb_der       rate of change in qb                   [rad/s^2]				*/
/*      rb_der       rate of change in rb                   [rad/s^2]				*/
/*      cai         inströmningstal (Engine inlet flow ratio?)                */
/*      nz          load factor                                               */

/*
/*																					*/
/*      ct_out       tangential force coefficient            [-]					*/
/*      cn_out       normal force coefficient                [-]					*/
/*      cc_out       side force coefficient                  [-]					*/
/*      crm_out      roll coefficient                        [-]					*/
/*      cpm_out      pitch coefficient                       [-]					*/
/*      cym_out      yaw coefficient                         [-]					*/
/*      cd_out       drag coefficient                        [-]					*/
/*      cl_out       lift coefficient                        [-]					*/
/*      mach_out                                             [-]              */

#define S_FUNCTION_LEVEL 2
#define S_FUNCTION_NAME  admire_aerotesthigh

#define drc_in  (*u[0])
#define dlc_in  (*u[1])
#define droe_in (*u[2])
#define drie_in (*u[3])
#define dlie_in (*u[4])
#define dloe_in (*u[5])
#define dr_in   (*u[6])
#define dle_in  (*u[7])
#define ldg_in  (*u[8])
#define tss_in  (*u[9])
#define dty_in  (*u[10])
#define dtz_in  (*u[11])

#define Vt_in    (*u[12]) 
#define alpha_in (*u[13])
#define beta_in  (*u[14])
#define pb_in    (*u[15])
#define qb_in    (*u[16])
#define rb_in    (*u[17])
#define z_in     (*u[18])

#define Vt_der_in       (*u[19])
#define alpha_der_in    (*u[20])
#define beta_der_in     (*u[21])
#define pb_der_in       (*u[22])
#define qb_der_in       (*u[23])
#define rb_der_in       (*u[24])
#define cai_in          (*u[25])
#define nz_in           (*u[26]) 

#define dummy_state     x[0]
#define dummy_der       dx[0]

/* Outputs */
#define ct_out          y[0]
#define cn_out          y[1]
#define cc_out          y[2]
#define crm_out         y[3]
#define cpm_out         y[4]
#define cym_out         y[5]
#define cd_out          y[6]
#define cl_out          y[7]
#define mach_out        y[8]

/* Errors */
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
#define	dCl_day		(*error[11])
#define	dCl_dai		(*error[12])
#define	dCl_dr		(*error[13])
#define dCm_alpha		(*error[14])
#define dCm_q			(*error[15])
#define dCm_dne		(*error[16])
#define dCm_dey		(*error[17])
#define dCm_dei		(*error[18])
#define	dCn_zero		(*error[19])
#define	dCn_beta		(*error[20])
#define	dCn_p			(*error[21])
#define	dCn_r			(*error[22])
#define	dCn_dna		(*error[23])
#define	dCn_dr		(*error[24])

#define u_dist			0
#define v_dist			0
#define w_dist			0
#define p_dist			0

#define r_t_d           57.29577951
#define r_t_d_05        28.64788976
#define degtorad        0.017453293
#define Pi				   3.141592654

#include <math.h>

#include "tmwtypes.h"
#include "simstruc.h"

#include "admire_data.h"
#include "admire_test.h"
#include "admire_tvc.h"

#include "aerodata_sym.h"
#include "setup_aerodata_sym.h"
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
    
    ssSetNumSFcnParams(S,0);		/* Number of expected parameters */
	 ssSetNumContStates(S,0);		/* Number of states */
    ssSetNumDiscStates(S,0);
	 ssSetNumInputPorts(S,2);		/* Number of input ports */
    ssSetInputPortWidth(S,0,27);	/* Width of input port no 1 */
    ssSetInputPortWidth(S,1,25);	/* Width of input port no 2 */
    ssSetNumOutputPorts(S,1);		/* Number of output ports */
    ssSetOutputPortWidth(S,0,9);	/* Width of output port */
    ssSetNumSampleTimes(S,1);		/* Number of sample times */
    ssSetInputPortDirectFeedThrough(S, 0, 1);  /* need direct feedthrough for control inputs */
    ssSetInputPortDirectFeedThrough(S, 1, 1);  /* need direct feedthrough  for errors */
    ssSetNumRWork(S,0);			   /* Size of real work vector */
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
/* ===== Function: mdlOutputs ================================================= */
/* ============================================================================ */
/* ===== Abstract:                                                        ===== */
/* =====   In this function, you compute the outputs of your S-function   ===== */
/* =====   block. Generally outputs are placed in the output vector,      ===== */
/* =====   ssGetY(S).                                                     ===== */
/* ============================================================================ */
static void mdlOutputs(SimStruct *S, int_T tid) {

	real_T *y  = ssGetOutputPortRealSignal(S,0);
	
	InputRealPtrsType u     = ssGetInputPortRealSignalPtrs(S,0);
	InputRealPtrsType error = ssGetInputPortRealSignalPtrs(S,1);

	int	look = 1;

	double rho,p_atm,T_atm,qa,qacorr;
	double tflow=0.0,tfhigh=0.0,cd2v,bd2v;
	double m_dot_fuel_low,m_dot_fuel_high;
	double dei,dey,de,dai,adai,da,day,aday,dne,dn,dna, sidai, siday, adr;
	double h_geo;
   double drc, dlc, droe, drie, dlie, dloe, dr, dle, ldg, tss, dty, dtz;
   double Vt, alpha, beta, pb, qb, rb, z, Vt_der;
   double alpha_der, beta_der, pb_der, qb_der, rb_der, cai;
   double alphad, betad, pc, qc, rc, mach;
   double Cttot, Cntot, Cctot, Crmtot, Cpmtot, Cymtot, nz;
   double aalpha, abeta, sibeta, q_dot, p_dot;

   drc = drc_in;
   dlc = dlc_in;
   droe = droe_in;
   drie = drie_in;
   dlie = dlie_in;
   dloe = dloe_in;
   dr = dr_in;
   dle = dle_in;
   ldg = ldg_in;
   tss = tss_in;
   dty = dty_in;
   dtz = dtz_in;

   Vt = Vt_in; 
   alpha = alpha_in*r_t_d;
   beta = beta_in*r_t_d;
   pb = pb_in;
   qb = qb_in;
   rb = rb_in;
   z  = z_in;
   h_geo = - z;

   Vt_der = Vt_der_in;
   alpha_der = alpha_der_in;
   beta_der = beta_der_in;
   pb_der = pb_der_in;
   qb_der = qb_der_in;
   rb_der = rb_der_in;
   cai = cai_in;
   nz = nz_in;

   p_dot = pb_der;                      
   q_dot = qb_der;

	aalpha = fabs(alpha);
	abeta  = fabs(beta);

	if (beta < 0)
		sibeta = -1;
	else if (beta > 0)
		sibeta = 1;
	else 
		sibeta = 0;


		dei  = (dlie+drie)*r_t_d_05;  /* Compute generic control */
		dey  = (dloe+droe)*r_t_d_05;  /* surfaces angles.		   */
		de   = (dei+dey)*0.5;
		dai  = (dlie-drie)*r_t_d_05;
		adai = fabs(dai);

		if (dai < 0)
			sidai = -1;
		else if (dai > 0)
			sidai = 1;
		else
			sidai = 0;

		day  = (dloe-droe)*r_t_d_05;
		aday = fabs(day);

		if (day < 0)
			siday = -1;
		else if (day > 0)
			siday = 1;
		else
			siday = 0;

		da	   = (dai+day)*0.5;
		dne	   = (dlc+drc)*r_t_d_05;
		dn	   =  dne;
		dna	   = (dlc-drc)*r_t_d_05;
		dle	   =  dle*r_t_d;
		dr	   =  dr*r_t_d;
		adr	   =  fabs(dr);
		h_geo  = -z;               /* Set some parameters */

		cd2v = cref/(2.0*Vt);
		bd2v = bref/(2.0*Vt);

		alphad =  alpha_der_in*cd2v;
		betad  =  beta_der_in*bd2v;
		pc	   = (pb_in+p_dist)*bd2v; /* Compute nondimensional rotation */
		qc	   =  qb_in*cd2v;         /* rates and accelerations.        */
		rc	   =  rb_in*bd2v;

		if (h_geo<0.1) {
          printf("\nWarning, low altitude !\n\n");
      }

		/* ATMOSPHERIC CALCULATION */
		if (h_geo<=11000) {
			T_atm = 288.15-0.0065*h_geo;
			p_atm = 101325*pow((T_atm/288.15),9.81/(287*0.0065)); }
		else {
			T_atm = 216.65;
			p_atm = 22632*exp(-9.81*(h_geo-11000)/(287*216.65)); }

		rho	   = p_atm/(287*T_atm);
		qa	   = 0.5*rho*Vt*Vt;
		qacorr = qa*0.0001;					/* (1/10000.0 = 0.0001)  */
		mach   = Vt/sqrt(1.4*287*T_atm);	/* Calculate mach number */

		if (testAdmEnvelope(mach,alpha_in,beta_in)) {
          /* printf("\nWarning, out of envelope, mach: %5.2f alpha: %10.4f beta: %10.4f !\n\n",mach, alpha, beta); */

      }

		/* TRANSITION FACTORS */
		if (mach>0.4 && mach<0.5) {				/* Compute transition factors */
			tflow  = (0.5-mach)*10.0;			/* low M -> high M data       */
			tfhigh = (mach-0.4)*10.0; 
      }


/*		  printf("Vt\t\t%5.10f\n",Vt);   
        printf("mach\t\t%5.10f\n",mach);   
		  printf("alpha\t\t%5.10f\n",alpha);   
		  printf("beta\t\t%5.10f\n",beta);   
		  printf("dei\t\t%5.10f\n",dei);   
		  printf("dey\t\t%5.10f\n",dey);   
		  printf("de\t\t%5.10f\n",de);   
		  printf("dn\t\t%5.10f\n",dn);   
		  printf("dle\t\t%5.10f\n",dle);   
		  printf("aalpha\t\t%5.10f\n",aalpha);   
		  printf("abeta\t\t%5.10f\n",abeta);   
		  printf("dai\t\t%5.10f\n",dai);   
		  printf("day\t\t%5.10f\n",day);   
		  printf("adai\t\t%5.10f\n",adai);   
		  printf("aday\t\t%5.10f\n",aday);   
		  printf("adr\t\t%5.10f\n",adr);   
		  printf("tflow\t\t%5.10f\n",tflow);   
		  printf("tfhigh\t\t%5.10f\n",tfhigh);   
		  printf("qacorr\t\t%5.10f\n",qacorr);   
		  printf("cai\t\t%5.10f\n",cai);   
		  printf("alphad\t\t%5.10f\n",alphad);   
		  printf("qc\t\t%5.10f\n",qc);   
		  printf("nz\t\t%5.10f\n",nz);   
		  printf("q_dot\t\t%5.10f\n",q_dot);   
		  printf("ldg\t\t%5.10f\n",ldg);   
		  printf("look\t\t%d\n",look);   
		  printf("error\t\t%d\n\n",error);   */

   /* ALGORITHMS FOR FORCE COEFFICIENTS */
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

	Cymtot = getCymtotCoeff(mach,alpha,aalpha,beta,abeta,sibeta,betad,dei,dey,dai,day,
		                    adai,aday,sidai,siday,dn,dne,dna,dle,dr,pc,rc,cai,tflow,
		                    tfhigh,qacorr,ldg,look,error);

/* Outputs */
   ct_out = Cttot;
   cn_out = Cntot;
	cc_out = Cctot;

	crm_out = Crmtot;
	cpm_out = Cpmtot;
	cym_out = Cymtot;

	cd_out = sin(alpha_in)*Cntot+cos(alpha_in)*Cttot;
   cl_out = cos(alpha_in)*Cntot-sin(alpha_in)*Cttot;
   mach_out = mach;
}


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

