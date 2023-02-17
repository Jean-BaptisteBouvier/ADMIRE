/* ============================================================================ */
/* ===== Function: getCctotCoeff ============================================== */
/* ============================================================================ */
/* ===== Abstract:                                                        ===== */
/* =====   Algorithm for centripetal force coefficient: CC                ===== */
/* ============================================================================ */
double getCctotCoeff(double Mach,double Alpha,double Beta,double abeta,double aalpha,
					 double sibeta,double dei,double dey,double dai,double day,
					 double adai,double aday,double sidai,double siday,double dn,
					 double dne,double dna,double dle,double dr,double betad,
					 double pc,double rc,double tflow,double tfhigh,double qacorr,
					 double cai,double ldg,int look) {
	
	/* Initialize to zero */
	double Ccbeta  = 0.0;
	double Ccblg   = 0.0;
	double Ccbde   = 0.0;
	double Ccbdn   = 0.0;
	double Ccbdle  = 0.0;
	double Ccbcai  = 0.0;
	double Ccdr    = 0.0;
	double Ccdai   = 0.0;
	double Ccdaib  = 0.0;
	double Ccday   = 0.0;
	double Ccdayb  = 0.0;
	double Ccdna   = 0.0;
	double Ccbetad = 0.0;
	double Ccp     = 0.0;
	double Ccpbeta = 0.0;
	double Ccr     = 0.0;
	double Ccrbeta = 0.0;
	double cctot   = 0.0;

	double eoeff,eieff;

    eoeff = ccdayod1(&ccdayod,Mach,look);
    eieff = 1.0-eoeff;

    if (Mach<=0.4) {
        Ccbeta   =  ccbal2(&ccbal,abeta,Alpha,look)*sibeta*
        	        cceb2(&cceb,qacorr,Mach,look);
		Ccblg    =  ccblst2(&ccblst,ldg,Alpha,look)*Beta*degtorad*ldg;
        Ccbde    = (ccdeb3(&ccdeb,abeta,dei,Alpha,look)*eieff+
        		    ccdeb3(&ccdeb,abeta,dey,Alpha,look)*eoeff)*sibeta;
        Ccbdn    =  ccdnb3(&ccdnb,abeta,dn,Alpha,look)*sibeta;
        Ccbdle   =  ccdleb3(&ccdleb,dle,abeta,Alpha,look)*sibeta;
        Ccbcai   =  ccbcai2(&ccbcai,cai,Alpha,look)*Beta*degtorad;
        Ccdr     =  ccbdral3(&ccbdral,abeta,dr,Alpha,look)*
        	        ccedr2(&ccedr,qacorr,Mach,look);
        Ccdai    = (ccdaal2(&ccdaal,adai,Alpha,look)*sidai+
        		   (ccdal2(&ccdal,dei,Alpha,look)-
                    ccdal2(&ccdal,0.0,Alpha,look))*dai*degtorad)*eieff*
                    ccedai2(&ccedai,qacorr,Mach,look);
        Ccdaib   =  ccdabal1(&ccdabal,aalpha,look)*(adai/30.0)*(abeta/20.0)*eieff;
        Ccday    = (ccdaal2(&ccdaal,aday,Alpha,look)*siday+
        		   (ccdal2(&ccdal,dey,Alpha,look)-
                    ccdal2(&ccdal,0.0,Alpha,look))*day*degtorad)*eoeff+
                    cceday2(&cceday,qacorr,Mach,look)*day*degtorad;
        Ccdayb   =  ccdabal1(&ccdabal,aalpha,look)*(aday/30.0)*(abeta/20.0)*eoeff;
        Ccdna    =  ccdnaal2(&ccdnaal,dne,Alpha,look)*dna*degtorad;
        Ccbetad  =  ccbdl1(&ccbdl,Alpha,look)*betad;
        Ccp      =  ccpal1(&ccpal,Alpha,look)*pc;
        Ccpbeta  =  ccpba2(&ccpba,abeta,Alpha,look)*pc;
        Ccr      =  ccral2(&ccral,dn,Alpha,look)*rc;
        Ccrbeta  =  ccrba2(&ccrba,abeta,aalpha,look)*rc;   }

    else if (Mach>0.4 && Mach<0.5) {
        Ccbeta   = (ccbal2(&ccbal,abeta,Alpha,look)*sibeta*tflow+
        		    ccbma2(&ccbma,0.5,Alpha,look)*Beta*degtorad*tfhigh)*
                    cceb2(&cceb,qacorr,Mach,look);
		Ccblg    =  ccblst2(&ccblst,ldg,Alpha,look)*Beta*degtorad*ldg;
		Ccbde    = (ccdeb3(&ccdeb,abeta,dei,Alpha,look)*eieff+
      		        ccdeb3(&ccdeb,abeta,dey,Alpha,look)*eoeff)*sibeta;
		Ccbdn    =  ccdnb3(&ccdnb,abeta,dn,Alpha,look)*sibeta;
		Ccbdle   =  ccdleb3(&ccdleb,dle,abeta,Alpha,look)*sibeta*tflow+
        		    ccbdle2(&ccbdle,0.5,Alpha,look)*Beta*degtorad*(dle/27.0)*tfhigh;
		Ccbcai   =  ccbcai2(&ccbcai,cai,Alpha,look)*Beta*degtorad;
		Ccdr     = (ccbdral3(&ccbdral,abeta,dr,Alpha,look)*tflow+
        	        ccdram2(&ccdram,Alpha,0.5,look)*dr*degtorad*tfhigh)*
                    ccedr2(&ccedr,qacorr,Mach,look);
		Ccdai    = ((ccdaal2(&ccdaal,adai,Alpha,look)*sidai+
        		   (ccdal2(&ccdal,dei,Alpha,look)-
                    ccdal2(&ccdal,0.0,Alpha,look))*dai*degtorad)*tflow+
				   (ccmdaa3(&ccmdaa,0.5,adai,Alpha,look)*sidai+
				   (ccda3(&ccda,0.5,dei,Alpha,look)-
				    ccda3(&ccda,0.5,0.0,Alpha,look))*dai*degtorad)*tfhigh)*eieff*
                    ccedai2(&ccedai,qacorr,Mach,look);
        Ccdaib   =  ccdabal1(&ccdabal,aalpha,look)*(adai/30.0)*(abeta/20.0)*eieff*tflow;
		Ccday    = ((ccdaal2(&ccdaal,aday,Alpha,look)*siday+
        		   (ccdal2(&ccdal,dey,Alpha,look)-
				    ccdal2(&ccdal,0.0,Alpha,look))*day*degtorad)*tflow+
				   (ccmdaa3(&ccmdaa,0.5,aday,Alpha,look)*siday+
				   (ccda3(&ccda,0.5,dey,Alpha,look)-
				    ccda3(&ccda,0.5,0.0,Alpha,look))*day*degtorad)*tfhigh)*eoeff+
                    cceday2(&cceday,qacorr,Mach,look)*day*degtorad;
		Ccdayb   =  ccdabal1(&ccdabal,aalpha,look)*(aday/30.0)*(abeta/20.0)*eoeff*tflow;
		Ccdna    = (ccdnaal2(&ccdnaal,dne,Alpha,look)*tflow+
        		    ccdnaam2(&ccdnaam,Alpha,0.5,look)*tfhigh)*dna*degtorad;
		Ccbetad  = (ccbdl1(&ccbdl,Alpha,look)*tflow+
        	        ccbdh2(&ccbdh,Alpha,0.5,look)*tfhigh)*betad;
		Ccp      = (ccpal1(&ccpal,Alpha,look)*tflow+
      		        ccpam2(&ccpam,Alpha,0.5,look)*tfhigh)*pc;
		Ccpbeta  =  ccpba2(&ccpba,abeta,Alpha,look)*pc*tflow;
		Ccr      = (ccral2(&ccral,dn,Alpha,look)*tflow+
      		        ccrm1(&ccrm,0.5,look)*tfhigh)*rc*
                    ccer2(&ccer,Mach,qacorr,look);
		Ccrbeta  =  ccrba2(&ccrba,abeta,aalpha,look)*rc*tflow;   }

	else if (Mach>=0.5 && Mach<=1.4) {
		Ccbeta   =  ccbma2(&ccbma,Mach,Alpha,look)*Beta*degtorad*
      			    cceb2(&cceb,qacorr,Mach,look);
		Ccblg    =  ccblst2(&ccblst,ldg,Alpha,look)*Beta*degtorad*ldg;
		Ccbde    = (ccdeb3(&ccdeb,abeta,dei,Alpha,look)*eieff+
       			    ccdeb3(&ccdeb,abeta,dey,Alpha,look)*eoeff)*sibeta*
                    ccbkm1(&ccbkm,Mach,look);
		Ccbdn    =  ccdnb3(&ccdnb,abeta,dn,Alpha,look)*sibeta;
		Ccbdle   =  ccbdle2(&ccbdle,Mach,Alpha,look)*Beta*degtorad*(dle/27.0);
		Ccbcai   =  ccbcai2(&ccbcai,cai,Alpha,look)*Beta*degtorad;
		Ccdr     =  ccdram2(&ccdram,Alpha,Mach,look)*dr*degtorad*
        	        ccedr2(&ccedr,qacorr,Mach,look);
		Ccdai    = (ccmdaa3(&ccmdaa,Mach,adai,Alpha,look)*sidai+
        		   (ccda3(&ccda,Mach,dei,Alpha,look)-
				    ccda3(&ccda,Mach,0.0,Alpha,look))*dai*degtorad)*eieff*
				    ccedai2(&ccedai,qacorr,Mach,look);
		Ccdaib   =  0;
		Ccday    = (ccmdaa3(&ccmdaa,Mach,aday,Alpha,look)*siday+
        		   (ccda3(&ccda,Mach,dey,Alpha,look)-
                    ccda3(&ccda,Mach,0.0,Alpha,look))*day*degtorad)*eoeff+
                    cceday2(&cceday,qacorr,Mach,look)*day*degtorad;
		Ccdayb   =  0;
		Ccdna    =  ccdnaam2(&ccdnaam,Alpha,Mach,look)*dna*degtorad;
		Ccbetad  =  ccbdh2(&ccbdh,Alpha,Mach,look)*betad;
		Ccp      =  ccpam2(&ccpam,Alpha,Mach,look)*pc;
		Ccpbeta  =  0;
		Ccr      =  ccrm1(&ccrm,Mach,look)*rc;
		Ccrbeta  =  0;   }

	else {
		Ccbeta   =  ccbma2(&ccbma,Mach,Alpha,look)*Beta*degtorad*
       			    cceb2(&cceb,qacorr,Mach,look);
		Ccbde    = (ccdeb3(&ccdeb,abeta,dei,Alpha,look)*eieff+
      			    ccdeb3(&ccdeb,abeta,dey,Alpha,look)*eoeff)*sibeta*
				    ccbkm1(&ccbkm,Mach,look);
		Ccbdn    =  ccdnb3(&ccdnb,abeta,dn,Alpha,look)*sibeta;
		Ccbdle   =  ccbdle2(&ccbdle,Mach,Alpha,look)*Beta*degtorad*(dle/27.0);
		Ccbcai   =  ccbcai2(&ccbcai,cai,Alpha,look)*Beta*degtorad;
		Ccdr     =  ccdram2(&ccdram,Alpha,Mach,look)*dr*degtorad*
			        ccedr2(&ccedr,qacorr,Mach,look);
		Ccblg    =  ccblst2(&ccblst,ldg,Alpha,look)*Beta*degtorad*ldg;
		Ccdai    =  ccda3(&ccda,Mach,dei,Alpha,look)*dai*degtorad*eieff*
      			    ccedai2(&ccedai,qacorr,Mach,look);
		Ccdaib   =  0;
		Ccday    = (ccda3(&ccda,Mach,dey,Alpha,look)*eoeff+
      			    cceday2(&cceday,qacorr,Mach,look))*day*degtorad;
		Ccdayb   =  0;
		Ccdna    =  ccdnaam2(&ccdnaam,Alpha,Mach,look)*dna*degtorad;
		Ccbetad  =  ccbdh2(&ccbdh,Alpha,Mach,look)*betad;
		Ccp      =  ccpam2(&ccpam,Alpha,Mach,look)*pc;
		Ccpbeta  =  0;
		Ccr      =  ccrm1(&ccrm,Mach,look)*rc*
      		        ccer2(&ccer,Mach,qacorr,look);
		Ccrbeta  =  0;   }

	cctot = Ccbeta+Ccblg+Ccbde+Ccbdn+Ccbdle+Ccbcai+Ccdr+Ccdai+Ccdaib+
		    Ccday+Ccdayb+Ccdna+Ccbetad+Ccp+Ccpbeta+Ccr+Ccrbeta;

	return cctot;
}


/* ============================================================================ */
/* ===== Function: getCrmtotCoeff ============================================= */
/* ============================================================================ */
/* ===== Abstract:                                                        ===== */
/* =====   Algorithm for rolling moment coefficient: CRM                  ===== */
/* ============================================================================ */
double getCrmtotCoeff(double Mach,double Alpha,double aalpha,double Beta,double betad,
					  double abeta,double sibeta,double dei,double dey,double dai,
					  double adai,double sidai,double day,double aday,double siday,
					  double dn,double dne,double dna,double dle,double dr,double pc,
					  double rc,double qacorr,double tflow,double tfhigh,double ldg,
					  int look,InputRealPtrsType error) {

	/* Initialize to zero */
	double Crmbasic = 0.0;
	double Crmbeta  = 0.0;
	double Crmbde   = 0.0;
	double Crmbdn   = 0.0;
	double Crmbdle  = 0.0;
	double Crmdr    = 0.0;
	double Crmdai   = 0.0;
	double Crmdaib  = 0.0;
	double Crmday   = 0.0;
	double Crmdayb  = 0.0;
	double Crmdna   = 0.0;
	double Crmbetad = 0.0;
	double Crmp     = 0.0;
	double Crmpbeta = 0.0;
	double Crmr     = 0.0;
	double Crmrbeta = 0.0;
	double Crmrde   = 0.0;
	double Crmblg   = 0.0;
	double crmtot   = 0.0;
	
	double eoeff,eieff;

	eoeff = crmdayod1(&crmdayod,Mach,look);
	eieff = 1.0-eoeff;

	if (Mach<=0.4) {
		Crmbasic =  crmzero1(&crmzero,aalpha,look);
		Crmbeta  =  crmbal2(&crmbal,abeta,Alpha,look)*sibeta+
      			    crmeb2(&crmeb,qacorr,Mach,look)*Beta*degtorad;
		Crmbde   = (crmdeb4(&crmdeb,abeta,dei,dn,Alpha,look)*eieff+
       			    crmdeb4(&crmdeb,abeta,dey,dn,Alpha,look)*eoeff)*sibeta;
		Crmbdn   =  crmdnb3(&crmdnb,abeta,dn,Alpha,look)*sibeta;
		Crmbdle  = (crmdleb4(&crmdleb,dle,abeta,dei,Alpha,look)*eieff+
       			    crmdleb4(&crmdleb,dle,abeta,dey,Alpha,look)*eoeff)*sibeta;
		Crmblg   =  crmblst2(&crmblst,ldg,Alpha,look)*Beta*degtorad*ldg;
		Crmdr    =  crmbdral3(&crmbdral,abeta,dr,Alpha,look)*
      			    crmedr2(&crmedr,qacorr,Mach,look);
		Crmdai   = (crmdaal2(&crmdaal,adai,Alpha,look)*sidai+
        		   (crmdal2(&crmdal,dei,Alpha,look)-
				    crmdal2(&crmdal,0.0,Alpha,look))*dai*degtorad)*eieff*
                    crmedai2(&crmedai,qacorr,Mach,look);
		Crmdaib  =  crmdabal1(&crmdabal,aalpha,look)*(adai/30.0)*(abeta/20.0)*eieff;
		Crmday   = (crmdaal2(&crmdaal,aday,Alpha,look)*siday+
      		       (crmdal2(&crmdal,dey,Alpha,look)-
                    crmdal2(&crmdal,0.0,Alpha,look))*day*degtorad)*eoeff*
                    crmeday2(&crmeday,qacorr,Mach,look);
		Crmdayb  =  crmdabal1(&crmdabal,aalpha,look)*(aday/30.0)*(abeta/20.0)*eoeff;
		Crmdna   =  crmdnaal2(&crmdnaal,dne,Alpha,look)*dna*degtorad;
		Crmbetad =  crmbdl1(&crmbdl,Alpha,look)*betad;
		Crmp     =  crmpal2(&crmpal,dn,Alpha,look)*pc*
			        crmep2(&crmep,qacorr,Mach,look);
		Crmpbeta =  crmpba2(&crmpba,abeta,Alpha,look)*pc;
		Crmr     =  crmral2(&crmral,dn,Alpha,look)*rc;
		Crmrbeta =  crmrba2(&crmrba,abeta,Alpha,look)*rc;
		Crmrde   =  crmrdem1(&crmrdem,Mach,look)*
      			    crmrka1(&crmrka,aalpha,look)*
					(eieff*(dei/30.0)+eoeff*(dey/30.0))*rc;
	}
	else if (Mach>0.4 && Mach<0.5) {
   		Crmbasic =  crmzero1(&crmzero,aalpha,look)*tflow;
		Crmbeta  =  crmbal2(&crmbal,abeta,Alpha,look)*sibeta*tflow+
      			    crmbma2(&crmbma,0.5,Alpha,look)*Beta*degtorad*tfhigh+
                    crmeb2(&crmeb,qacorr,Mach,look)*Beta*degtorad;
		Crmbde   = (crmdeb4(&crmdeb,abeta,dei,dn,Alpha,look)*eieff+
      			    crmdeb4(&crmdeb,abeta,dey,dn,Alpha,look)*eoeff)*sibeta;
		Crmbdn   =  crmdnb3(&crmdnb,abeta,dn,Alpha,look)*sibeta*tflow+
      			    crmbmdn2(&crmbmdn,dn,0.5,look)*Beta*degtorad*tfhigh;
		Crmbdle  = (crmdleb4(&crmdleb,dle,abeta,dei,Alpha,look)*(1.0-
      			    crmdayod1(&crmdayod,0.4,look))+
                    crmdleb4(&crmdleb,dle,abeta,dey,Alpha,look)*
                    crmdayod1(&crmdayod,0.4,look))*sibeta*tflow+
				   (crmbdle3(&crmbdle,0.5,dei,Alpha,look)*(1.0-
                    crmdayod1(&crmdayod,0.5,look))+
                    crmbdle3(&crmbdle,0.5,dey,Alpha,look)*
                    crmdayod1(&crmdayod,0.5,look))*Beta*degtorad*(dle/27.0)*tfhigh;
		Crmblg   =  crmblst2(&crmblst,ldg,Alpha,look)*Beta*degtorad*ldg;
		Crmdr    = (crmbdral3(&crmbdral,abeta,dr,Alpha,look)*tflow+
      		        crmdrm1(&crmdrm,0.5,look)*dr*degtorad*tfhigh)*
                    crmedr2(&crmedr,qacorr,Mach,look);
		Crmdai   = ((crmdaal2(&crmdaal,adai,Alpha,look)*sidai+
      			   (crmdal2(&crmdal,dei,Alpha,look)-
                    crmdal2(&crmdal,0.0,Alpha,look))*dai*degtorad)*tflow+
                   (crmmdaa3(&crmmdaa,0.5,adai,Alpha,look)*sidai+
				   (crmda3(&crmda,0.5,dei,Alpha,look)-
                    crmda3(&crmda,0.5,0.0,Alpha,look))*dai*degtorad)*tfhigh)*eieff*
                    crmedai2(&crmedai,qacorr,Mach,look);
		Crmdaib  =  crmdabal1(&crmdabal,aalpha,look)*(adai/30.0)*(abeta/20.0)*eieff*tflow;
		Crmday   = ((crmdaal2(&crmdaal,aday,Alpha,look)*siday+
      			   (crmdal2(&crmdal,dey,Alpha,look)-
                    crmdal2(&crmdal,0.0,Alpha,look))*day*degtorad)*tflow+
                   (crmmdaa3(&crmmdaa,0.5,aday,Alpha,look)*siday+
                   (crmda3(&crmda,0.5,dey,Alpha,look)-
                    crmda3(&crmda,0.5,0.0,Alpha,look))*day*degtorad)*tfhigh)*eoeff*
                    crmeday2(&crmeday,qacorr,Mach,look);
		Crmdayb  =  crmdabal1(&crmdabal,aalpha,look)*(aday/30.0)*(abeta/20.0)*eoeff*tflow;
		Crmdna   = (crmdnaal2(&crmdnaal,dne,Alpha,look)*tflow+
      			    crmdnaam2(&crmdnaam,Alpha,0.5,look)*tfhigh)*dna*degtorad;
		Crmbetad = (crmbdl1(&crmbdl,Alpha,look)*tflow+
      			    crmbdh2(&crmbdh,Alpha,0.5,look)*tfhigh)*betad;
		Crmp     = (crmpal2(&crmpal,dn,Alpha,look)*tflow+
        		    crmpam2(&crmpam,Alpha,0.5,look)*tfhigh)*pc*
        		    crmep2(&crmep,qacorr,Mach,look);
		Crmpbeta =  crmpba2(&crmpba,abeta,Alpha,look)*pc*tflow;
		Crmr     = (crmral2(&crmral,dn,Alpha,look)*tflow+
        		    crmram2(&crmram,Alpha,0.5,look)*tfhigh)*rc;
		Crmrbeta =  crmrba2(&crmrba,abeta,Alpha,look)*rc*tflow;
		Crmrde   =  crmrdem1(&crmrdem,Mach,look)*
                    crmrka1(&crmrka,aalpha,look)*
					(eieff*(dei/30.0)+eoeff*(dey/30.0))*rc;   }

   else if (Mach>=0.5 && Mach<=1.4) {
   		Crmbasic =  0;
		Crmbeta  = (crmbma2(&crmbma,Mach,Alpha,look)+
       			    crmeb2(&crmeb,qacorr,Mach,look))*Beta*degtorad;
		Crmbde   = (crmdeb4(&crmdeb,abeta,dei,dn,Alpha,look)*eieff+
     			    crmdeb4(&crmdeb,abeta,dey,dn,Alpha,look)*eoeff)*
                    crmbkm1(&crmbkm,Mach,look)*sibeta;
		Crmbdn   =  crmbmdn2(&crmbmdn,dn,Mach,look)*Beta*degtorad;
   		Crmbdle  = (crmbdle3(&crmbdle,Mach,dei,Alpha,look)*eieff+
     			    crmbdle3(&crmbdle,Mach,dey,Alpha,look)*eoeff)*Beta*degtorad*(dle/27.0);
		Crmblg   =  crmblst2(&crmblst,ldg,Alpha,look)*Beta*degtorad*ldg;
   		Crmdr    =  crmdrm1(&crmdrm,Mach,look)*dr*degtorad*
     			    crmedr2(&crmedr,qacorr,Mach,look);
   		Crmdai   = (crmmdaa3(&crmmdaa,Mach,adai,Alpha,look)*sidai+
     			   (crmda3(&crmda,Mach,dei,Alpha,look)-
				    crmda3(&crmda,Mach,0.0,Alpha,look))*dai*degtorad)*eieff*
            	    crmedai2(&crmedai,qacorr,Mach,look);
		Crmdaib  =  0;
		Crmday   = (crmmdaa3(&crmmdaa,Mach,aday,Alpha,look)*siday+
      			   (crmda3(&crmda,Mach,dey,Alpha,look)-
      			    crmda3(&crmda,Mach,0.0,Alpha,look))*day*degtorad)*eoeff*
                    crmeday2(&crmeday,qacorr,Mach,look);
		Crmdayb  =  0;
		Crmdna   =  crmdnaam2(&crmdnaam,Alpha,Mach,look)*dna*degtorad;
		Crmbetad =  crmbdh2(&crmbdh,Alpha,Mach,look)*betad;
		Crmp     =  crmpam2(&crmpam,Alpha,Mach,look)*pc*
      	  	        crmep2(&crmep,qacorr,Mach,look);
		Crmpbeta =  0;
		Crmr     =  crmram2(&crmram,Alpha,Mach,look)*rc;
		Crmrbeta =  0;
		Crmrde   =  crmrdem1(&crmrdem,Mach,look)*
      			    crmrka1(&crmrka,aalpha,look)*
					(eieff*(dei/30.0)+eoeff*(dey/30.0))*rc;   }

   else {
   		Crmbasic =  0;
		Crmbeta  = (crmbma2(&crmbma,Mach,Alpha,look)+
      			    crmeb2(&crmeb,qacorr,Mach,look))*Beta*degtorad;
		Crmbde   = (crmdeb4(&crmdeb,abeta,dei,dn,Alpha,look)*eieff+
      			    crmdeb4(&crmdeb,abeta,dey,dn,Alpha,look)*eoeff)*
                    crmbkm1(&crmbkm,Mach,look)*sibeta;
		Crmbdn   =  crmbmdn2(&crmbmdn,dn,Mach,look)*Beta*degtorad;
		Crmbdle  = (crmbdle3(&crmbdle,Mach,dei,Alpha,look)*eieff+
      			    crmbdle3(&crmbdle,Mach,dey,Alpha,look)*eoeff)*Beta*degtorad*(dle/27.0);
		Crmblg   =  crmblst2(&crmblst,ldg,Alpha,look)*Beta*degtorad*ldg;
		Crmdr    =  crmdrm1(&crmdrm,Mach,look)*dr*degtorad*
      		        crmedr2(&crmedr,qacorr,Mach,look);
		Crmdai   =  crmda3(&crmda,Mach,dei,Alpha,look)*dai*degtorad*eieff*
      			    crmedai2(&crmedai,qacorr,Mach,look);
		Crmdaib  =  0.0;
		Crmday   =  crmda3(&crmda,Mach,dey,Alpha,look)*day*degtorad*eoeff*
      			    crmeday2(&crmeday,qacorr,Mach,look);
		Crmdayb  =  0.0;
		Crmdna   =  crmdnaam2(&crmdnaam,Alpha,Mach,look)*dna*degtorad;
		Crmbetad =  crmbdh2(&crmbdh,Alpha,Mach,look)*betad;
		Crmp     =  crmpam2(&crmpam,Alpha,Mach,look)*pc*
      		        crmep2(&crmep,qacorr,Mach,look);
		Crmpbeta =  0.0;
		Crmr     =  crmram2(&crmram,Alpha,Mach,look)*rc;
		Crmrbeta =  0.0;
		Crmrde   =  crmrdem1(&crmrdem,Mach,look)*
      			    crmrka1(&crmrka,aalpha,look)*(eieff*(dei/30.0)+eoeff*(dey/30.0))*rc;   
   }
   crmtot = Crmbasic+Crmbeta+Crmbde+Crmbdn+Crmbdle+Crmdr+Crmdai+Crmdaib+Crmday+
	        Crmdayb+Crmdna+Crmbetad+Crmp+Crmpbeta+Crmr+Crmrbeta+Crmrde+Crmblg+
			dCl_beta*Beta*degtorad+dCl_p*pc+dCl_r*rc+dCl_day*day*degtorad+
			dCl_dai*dai*degtorad+dCl_dr*dr*degtorad;

	return crmtot;
}

/* ============================================================================ */
/* ===== Function: getCymtotCoeff ============================================= */
/* ============================================================================ */
/* ===== Abstract:                                                        ===== */
/* =====   Algorithm for yaw moment coefficient: CYM                      ===== */
/* ============================================================================ */
double getCymtotCoeff(double Mach,double Alpha,double aalpha,double Beta,double abeta,
					  double sibeta,double betad,double dei,double dey,double dai,
					  double day,double adai,double aday,double sidai,double siday,
					  double dn,double dne,double dna,double dle,double dr,double pc,
					  double rc,double cai,double tflow,double tfhigh,double qacorr,
					  double ldg,int look,InputRealPtrsType error) {

	/* Initialize to zero */
	double Cymbasic = 0.0;
	double Cymbeta  = 0.0;
	double Cymbde   = 0.0;
	double Cymbdn   = 0.0;
	double Cymbdle  = 0.0;
	double Cymbcai  = 0.0;
	double Cymdr    = 0.0;
	double Cymdai   = 0.0;
	double Cymdaib  = 0.0;
	double Cymday   = 0.0;
	double Cymdayb  = 0.0;
	double Cymdna   = 0.0;
	double Cymbetad = 0.0;
	double Cymp     = 0.0;
	double Cympbeta = 0.0;
	double Cymr     = 0.0;
	double Cymrbeta = 0.0;
	double Cymblg   = 0.0;
	double cymtot   = 0.0;

	double eieff,eoeff;

	eoeff = cymdayod1(&cymdayod,Mach,look);
	eieff = 1.0-eoeff;

	

	/* Start table look-up */
	if (Mach<=0.4) {
		Cymbasic =  cymzero1(&cymzero,aalpha,look);
		Cymbeta  =  cymbal2(&cymbal,abeta,Alpha,look)*sibeta+
      			    cymeb2(&cymeb,qacorr,Mach,look)*Beta*degtorad;
		Cymbde   = (cymdeb3(&cymdeb,abeta,dei,Alpha,look)*eieff+
      			    cymdeb3(&cymdeb,abeta,dey,Alpha,look)*eoeff)*sibeta;
		Cymbdn   =  cymdnb3(&cymdnb,abeta,dn,Alpha,look)*sibeta;
		Cymbdle  = (cymdleb4(&cymdleb,dle,abeta,dei,Alpha,look)*eieff+
      			    cymdleb4(&cymdleb,dle,abeta,dey,Alpha,look)*eoeff)*sibeta;
		Cymbcai  =  cymbcai2(&cymbcai,cai,Alpha,look)*Beta*degtorad;
		Cymblg   =  cymblst2(&cymblst,ldg,Alpha,look)*Beta*degtorad*ldg;
		Cymdr    =  cymbdral3(&cymbdral,Beta,dr,Alpha,look)*
      			    cymedr2(&cymedr,qacorr,Mach,look);
		Cymdai   = (cymdaal2(&cymdaal,adai,Alpha,look)*sidai+
      			   (cymdal2(&cymdal,dei,Alpha,look)-
                    cymdal2(&cymdal,0.0,Alpha,look))*dai*degtorad)*eieff*
                    cymedai2(&cymedai,qacorr,Mach,look);
		Cymdaib  =  cymdabal1(&cymdabal,aalpha,look)*(adai/30.0)*(abeta/20.0)*eieff;
		Cymday   = (cymdaal2(&cymdaal,aday,Alpha,look)*siday+
      			   (cymdal2(&cymdal,dey,Alpha,look)-
                    cymdal2(&cymdal,0.0,Alpha,look))*day*degtorad)*eoeff+
                    cymeday2(&cymeday,qacorr,Mach,look)*day*degtorad;
		Cymdayb  =  cymdabal1(&cymdabal,aalpha,look)*(aday/30.0)*(abeta/20.0)*eoeff;
		Cymdna   =  cymdnaal2(&cymdnaal,dne,Alpha,look)*dna*degtorad;
		Cymbetad =  cymbdl1(&cymbdl,Alpha,look)*betad;
		Cymp     = (cympal2(&cympal,dn,Alpha,look)+
      		        cymep2(&cymep,qacorr,Mach,look))*pc;
		Cympbeta =  cympba2(&cympba,abeta,Alpha,look)*pc;
		Cymr     =  cymral2(&cymral,dn,Alpha,look)*rc*
      		        cymer2(&cymer,Mach,qacorr,look);
		Cymrbeta =  cymrba2(&cymrba,abeta,Alpha,look)*rc;   }

	else if (Mach>0.4 && Mach<0.5) {
   		Cymbasic =  cymzero1(&cymzero,aalpha,look)*tflow;
		Cymbeta  =  cymbal2(&cymbal,abeta,Alpha,look)*sibeta*tflow+
      			    cymmab3(&cymmab,0.5,Alpha,abeta,look)*sibeta*tfhigh+
                    cymeb2(&cymeb,qacorr,Mach,look)*Beta*degtorad;
		Cymbde   = (cymdeb3(&cymdeb,abeta,dei,Alpha,look)*eieff+
      			    cymdeb3(&cymdeb,abeta,dey,Alpha,look)*eoeff)*sibeta;
		Cymbdn   =  cymdnb3(&cymdnb,abeta,dn,Alpha,look)*sibeta;
		Cymbdle  = (cymdleb4(&cymdleb,dle,abeta,dei,Alpha,look)*(1.0-
      			    cymdayod1(&cymdayod,0.4,look))+
                    cymdleb4(&cymdleb,dle,abeta,dey,Alpha,look)*
                    cymdayod1(&cymdayod,0.4,look))*sibeta*tflow+
      			   (cymbdle3(&cymbdle,0.5,dei,Alpha,look)*(1.0-
                    cymdayod1(&cymdayod,0.5,look))+
                    cymbdle3(&cymbdle,0.5,dey,Alpha,look)*
                    cymdayod1(&cymdayod,0.5,look))*Beta*degtorad*(dle/27.0)*tfhigh;
		Cymbcai  =  cymbcai2(&cymbcai,cai,Alpha,look)*Beta*degtorad;
		Cymblg   =  cymblst2(&cymblst,ldg,Alpha,look)*Beta*degtorad*ldg;
		Cymdr    = (cymbdral3(&cymbdral,Beta,dr,Alpha,look)*tflow+
      			    cymdram2(&cymdram,Alpha,0.5,look)*dr*degtorad*tfhigh)*
                    cymedr2(&cymedr,qacorr,Mach,look);
		Cymdai   = ((cymdaal2(&cymdaal,adai,Alpha,look)*sidai+
      			   (cymdal2(&cymdal,dei,Alpha,look)-
                    cymdal2(&cymdal,0.0,Alpha,look))*dai*degtorad)*tflow+
                   (cymmdaa3(&cymmdaa,0.5,adai,Alpha,look)*sidai+
                   (cymda3(&cymda,0.5,dei,Alpha,look)-
                    cymda3(&cymda,0.5,0.0,Alpha,look))*dai*degtorad)*tfhigh)*eieff*
                    cymedai2(&cymedai,qacorr,Mach,look);
		Cymdaib  =  cymdabal1(&cymdabal,aalpha,look)*(adai/30.0)*(abeta/20.0)*eieff*tflow;
		Cymday   = ((cymdaal2(&cymdaal,aday,Alpha,look)*siday+
      			   (cymdal2(&cymdal,dey,Alpha,look)-
                    cymdal2(&cymdal,0.0,Alpha,look))*day*degtorad)*tflow+
                   (cymmdaa3(&cymmdaa,0.5,aday,Alpha,look)*siday+
                   (cymda3(&cymda,0.5,dey,Alpha,look)-
                    cymda3(&cymda,0.5,0.0,Alpha,look))*day*degtorad)*tfhigh)*eoeff+
                    cymeday2(&cymeday,qacorr,Mach,look)*day*degtorad;
		Cymdayb  =  cymdabal1(&cymdabal,aalpha,look)*(aday/30.0)*(abeta/20.0)*eoeff*tflow;
		Cymdna   = (cymdnaal2(&cymdnaal,dne,Alpha,look)*tflow+
      			    cymdnaam2(&cymdnaam,Alpha,0.5,look)*tfhigh)*dna*degtorad;
		Cymbetad = (cymbdl1(&cymbdl,Alpha,look)*tflow+
      		 		cymbdh2(&cymbdh,Alpha,0.5,look)*tfhigh)*betad;
		Cymp     = (cympal2(&cympal,dn,Alpha,look)*tflow+
      		        cympam2(&cympam,Alpha,0.5,look)*tfhigh+
                    cymep2(&cymep,qacorr,Mach,look))*pc;
		Cympbeta =  cympba2(&cympba,abeta,Alpha,look)*pc*tflow;
		Cymr     = (cymral2(&cymral,dn,Alpha,look)*tflow+
      		        cymram3(&cymram,dn,Alpha,0.5,look)*tfhigh)*rc*
                    cymer2(&cymer,Mach,qacorr,look);
		Cymrbeta =  cymrba2(&cymrba,abeta,Alpha,look)*rc*tflow;   }

	else if (Mach>=0.5 && Mach<=1.4) {
   		Cymbasic =  0.0;
		Cymbeta  =  cymmab3(&cymmab,Mach,Alpha,abeta,look)*sibeta+
      			    cymeb2(&cymeb,qacorr,Mach,look)*Beta*degtorad;
		Cymbde   = (cymdeb3(&cymdeb,abeta,dei,Alpha,look)*eieff+
      			    cymdeb3(&cymdeb,abeta,dey,Alpha,look)*eoeff)*
                    cymbkm1(&cymbkm,Mach,look)*sibeta;
		Cymbdn   =  cymdnb3(&cymdnb,abeta,dn,Alpha,look)*sibeta;
		Cymbdle  = (cymbdle3(&cymbdle,Mach,dei,Alpha,look)*eieff+
      			    cymbdle3(&cymbdle,Mach,dey,Alpha,look)*eoeff)*Beta*degtorad*(dle/27.0);
		Cymbcai  =  cymbcai2(&cymbcai,cai,Alpha,look)*Beta*degtorad;
		Cymblg   =  cymblst2(&cymblst,ldg,Alpha,look)*Beta*degtorad*ldg;
		Cymdr    =  cymdram2(&cymdram,Alpha,Mach,look)*dr*degtorad*
      		        cymedr2(&cymedr,qacorr,Mach,look);
		Cymdai   = (cymmdaa3(&cymmdaa,Mach,adai,Alpha,look)*sidai+
      			   (cymda3(&cymda,Mach,dei,Alpha,look)-
                    cymda3(&cymda,Mach,0.0,Alpha,look))*dai*degtorad)*eieff*
                    cymedai2(&cymedai,qacorr,Mach,look);
		Cymdaib  =  0.0;
		Cymday   = (cymmdaa3(&cymmdaa,Mach,aday,Alpha,look)*siday+
      			   (cymda3(&cymda,Mach,dey,Alpha,look)-
                    cymda3(&cymda,Mach,0.0,Alpha,look))*day*degtorad)*eoeff+
                    cymeday2(&cymeday,qacorr,Mach,look)*day*degtorad;
		Cymdayb  =  0.0;
		Cymdna   =  cymdnaam2(&cymdnaam,Alpha,Mach,look)*dna*degtorad;
		Cymbetad =  cymbdh2(&cymbdh,Alpha,Mach,look)*betad;
		Cymp     = (cympam2(&cympam,Alpha,Mach,look)+
      		        cymep2(&cymep,qacorr,Mach,look))*pc;
		Cympbeta =  0.0;
		Cymr     =  cymram3(&cymram,dn,Alpha,Mach,look)*rc*
      		        cymer2(&cymer,Mach,qacorr,look);
		Cymrbeta =  0.0;   }

	else {
		Cymbasic =  0.0;
		Cymbeta  =  cymmab3(&cymmab,Mach,Alpha,abeta,look)*sibeta+
      			    cymeb2(&cymeb,qacorr,Mach,look)*Beta*degtorad;
		Cymbde   = (cymdeb3(&cymdeb,abeta,dei,Alpha,look)*eieff+
      			    cymdeb3(&cymdeb,abeta,dey,Alpha,look)*eoeff)*
                    cymbkm1(&cymbkm,Mach,look)*sibeta;
		Cymbdn   =  cymdnb3(&cymdnb,abeta,dn,Alpha,look)*sibeta;
		Cymbdle  = (cymbdle3(&cymbdle,Mach,dei,Alpha,look)*eieff+
      			    cymbdle3(&cymbdle,Mach,dey,Alpha,look)*eoeff)*Beta*degtorad*(dle/27.0);
		Cymbcai  =  cymbcai2(&cymbcai,cai,Alpha,look)*Beta*degtorad;
		Cymblg   =  cymblst2(&cymblst,ldg,Alpha,look)*Beta*degtorad*ldg;
		Cymdr    =  cymdram2(&cymdram,Alpha,Mach,look)*dr*degtorad*
      		        cymedr2(&cymedr,qacorr,Mach,look);
		Cymdai   =  cymda3(&cymda,Mach,dei,Alpha,look)*dai*degtorad*eieff*
      			    cymedai2(&cymedai,qacorr,Mach,look);
		Cymdaib  =  0.0;
		Cymday   = (cymda3(&cymda,Mach,dey,Alpha,look)*eoeff+
      			    cymeday2(&cymeday,qacorr,Mach,look))*day*degtorad;
		Cymdayb  =  0.0;
		Cymdna   =  cymdnaam2(&cymdnaam,Alpha,Mach,look)*dna*degtorad;
		Cymbetad =  cymbdh2(&cymbdh,Alpha,Mach,look)*betad;
		Cymp     = (cympam2(&cympam,Alpha,Mach,look)+
      		        cymep2(&cymep,qacorr,Mach,look))*pc;
		Cympbeta =  0.0;
		Cymr     =  cymram3(&cymram,dn,Alpha,Mach,look)*rc*
      		        cymer2(&cymer,Mach,qacorr,look);
		Cymrbeta =  0.0;   }

	cymtot = Cymbasic+Cymbeta+Cymbde+Cymbdn+Cymbdle+Cymbcai+Cymdr+Cymdai+Cymdaib+
		     Cymday+Cymdayb+Cymdna+Cymbetad+Cymp+Cympbeta+Cymr+Cymrbeta+Cymblg+
			 dCn_zero+dCn_beta*Beta*degtorad+dCn_p*pc+dCn_r*rc+dCn_dna*dna*degtorad+
			 dCn_dr*dr*degtorad;

	return cymtot;
}

