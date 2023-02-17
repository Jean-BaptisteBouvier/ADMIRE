/* ============================================================================ */
/* ===== Function: getCttotCoeff ============================================== */
/* ============================================================================ */
/* ===== Abstract:                                                        ===== */
/* =====   Algorithm for tangential force coefficient: CT                 ===== */
/* ============================================================================ */
/* ===== Change 201101 by Anton Vooren												  ===== */
/* =====	  High aoa data added in separate coefficients						  ===== */
/* =====   Search for "High aoa"														  ===== */
/* ============================================================================ */

double getCttotCoeff(double h_geo,double Mach,double Alpha,double Beta,double dei,
					 double dey,double de,double dn,double dle,double aalpha,
					 double abeta,double dai,double day,double adai,double aday,
					 double adr,double tflow,double tfhigh,double ldg,int look) {

	/* Initialize to zero */
	double Ctbasic   = 0.0;
	double Ctalfa    = 0.0;
	double Ctdei     = 0.0;
	double Ctdey     = 0.0;
	double Ctdedn    = 0.0;
	double Ctdn      = 0.0;
	double Ctdle     = 0.0;
	double Ctbeta    = 0.0;
	double Ctda      = 0.0;
	double Ctdr      = 0.0;
	double Ctlg      = 0.0;
	double cttot     = 0.0;
	/* High aoa */
	double Cthigh	  = 0.0;

	double eoefflow,eoeffhigh,eiefflow,eieffhigh;
    double hd1000,eoeff,eieff;
    
	hd1000 = h_geo*0.001;
	eoeff  = ctdeyode1(&ctdeyode,Mach,look);
	eieff  = 1.0-eoeff;

	if (Mach<=0.4) {
        Ctbasic	 =  ctzero1(&ctzero,Mach,look)+
			        cth2(&cth,hd1000,Mach,look);
        Ctalfa	 =  ctal1(&ctal,Alpha,look);
        Ctdei	 =  ctdeal2(&ctdeal,dei,Alpha,look)*eieff;
        Ctdey	 =  ctdeal2(&ctdeal,dey,Alpha,look)*eoeff;
        Ctdedn	 =  ctdnde3(&ctdnde,dn,de,Alpha,look);
        Ctdn	 =  ctdnal2(&ctdnal,dn,Alpha,look);
        Ctdle	 =  ctdleal2(&ctdleal,dle,Alpha,look)+
			        ctlednal2(&ctlednal,dn,Alpha,look)*(dle/27.0);
		Ctlg	 =  ctlst2(&ctlst,abeta,Alpha,look)*ldg;
        Ctbeta	 =  ctbeta2(&ctbeta,dn,Alpha,look)*
			        ctkb1(&ctkb,abeta,look);
        Ctda	 =  ctdaal2(&ctdaal,adai,aalpha,look)*eieff+
			        ctdaal2(&ctdaal,aday,aalpha,look)*eoeff;
        Ctdr	 =  ctmdr2(&ctmdr,adr,Mach,look);
			/* High aoa */
		  Cthigh  = cthzero1(&cthzero,Alpha,look)+
					  cthdn2(&cthdn,dn,Alpha,look)+
					  cthde2(&cthde,de,Alpha,look);   }

    else if (Mach>0.4 && Mach<0.5) {
        eoefflow  = ctdeyode1(&ctdeyode,0.4,look);
        eoeffhigh = ctdeyode1(&ctdeyode,0.5,look);
        eiefflow  = 1.0-eoefflow;
        eieffhigh = 1.0-eoeffhigh;

        Ctbasic	 =  ctzero1(&ctzero,Mach,look)+
			        cth2(&cth,hd1000,Mach,look);
        Ctalfa	 =  ctal1(&ctal,Alpha,look)*tflow+
			        ctma2(&ctma,0.5,Alpha,look)*tfhigh;
        Ctdei	 =  ctdeal2(&ctdeal,dei,Alpha,look)*eiefflow*tflow+
			        ctmdea3(&ctmdea,0.5,dei,Alpha,look)*eieffhigh*tfhigh;
        Ctdey	 =  ctdeal2(&ctdeal,dey,Alpha,look)*eoefflow*tflow+
			        ctmdea3(&ctmdea,0.5,dey,Alpha,look)*eoeffhigh*tfhigh;
        Ctdedn	 =  ctdnde3(&ctdnde,dn,de,Alpha,look)*tflow;
        Ctdn	 =  ctdnal2(&ctdnal,dn,Alpha,look)*tflow+
			        ctmdna3(&ctmdna,0.5,dn,Alpha,look)*tfhigh;
        Ctdle	 = (ctdleal2(&ctdleal,dle,Alpha,look)+
			        ctlednal2(&ctlednal,dn,Alpha,look)*(dle/27.0))*tflow+
			        ctmdlea3(&ctmdlea,0.5,dle,Alpha,look)*tfhigh;
		Ctlg	 =  ctlst2(&ctlst,abeta,Alpha,look)*ldg;
        Ctbeta	 =  ctbeta2(&ctbeta,dn,Alpha,look)*
			        ctkb1(&ctkb,abeta,look)*tflow;
        Ctda	 = (ctdaal2(&ctdaal,adai,aalpha,look)*eiefflow+
			        ctdaal2(&ctdaal,aday,aalpha,look)*eoefflow)*tflow+
			       (ctdam2(&ctdam,adai,0.5,look)*eieffhigh+
			        ctdam2(&ctdam,aday,0.5,look)*eoeffhigh)*tfhigh;
        Ctdr	 =  ctmdr2(&ctmdr,adr,Mach,look);   }

    else {
        Ctbasic	 =  ctzero1(&ctzero,Mach,look)+
			        cth2(&cth,hd1000,Mach,look);
        Ctalfa	 =  ctma2(&ctma,Mach,Alpha,look);
        Ctdei	 =  ctmdea3(&ctmdea,Mach,dei,Alpha,look)*eieff;
        Ctdey	 =  ctmdea3(&ctmdea,Mach,dey,Alpha,look)*eoeff;
        Ctdedn	 =  0.0;
        Ctdn	 =  ctmdna3(&ctmdna,Mach,dn,Alpha,look);
        Ctdle	 =  ctmdlea3(&ctmdlea,Mach,dle,Alpha,look);
		Ctlg	 =  ctlst2(&ctlst,abeta,Alpha,look)*ldg;
        Ctbeta	 =  0.0;
        Ctda	 =  ctdam2(&ctdam,adai,Mach,look)*eieff+
			        ctdam2(&ctdam,aday,Mach,look)*eoeff;
        Ctdr	 =  ctmdr2(&ctmdr,adr,Mach,look);   }

    cttot = Ctbasic+Ctalfa+Ctdei+Ctdey+Ctdedn+Ctdn+Ctdle+Ctbeta+Ctda+Ctdr+Ctlg+Cthigh;
	
	return cttot;
}

/* ============================================================================ */
/* ===== Function: getCntotCoeff ============================================== */
/* ============================================================================ */
/* ===== Abstract:                                                        ===== */
/* =====   Algorithm for normal force coefficient: CN                     ===== */
/* ============================================================================ */
double getCntotCoeff(double h_geo,double Mach,double Alpha,double Beta,double dei,
					 double dey,double de,double dn,double dle,double aalpha,
					 double abeta,double dai,double day,double adai,double aday,
					 double adr,double tflow,double tfhigh,double qacorr,double cai,
					 double alphad,double qc,double Nz,double qd,double ldg,int look) {

	/* Initialize to zero */
	double Cnbasic = 0.0;
	double Cnalfa  = 0.0;
	double Cndei   = 0.0;
	double Cndey   = 0.0;
	double Cndedn  = 0.0;
	double Cndn    = 0.0;
	double Cndle   = 0.0;
	double Cncai   = 0.0;
	double Cnbeta  = 0.0;
	double Cnalfad = 0.0;
	double Cnq     = 0.0;
	double Cnnz    = 0.0;
	double Cnqd    = 0.0;
	double Cnlg    = 0.0;
	double cntot   = 0.0;
	/* High aoa */
	double Cnhigh	  = 0.0;

	double eoeff,eieff;
	
	eoeff = cndeyode2(&cndeyode,aalpha,Mach,look);
    eieff = 1.0-eoeff;

    if (Mach<=0.4) {
        Cnbasic  =  cnzero1(&cnzero,Mach,look)+
        			cnezero2(&cnezero,qacorr,Mach,look);
        Cnalfa   =  cndeal2(&cndeal,0.0,Alpha,look)*
        			cnea2(&cnea,qacorr,Mach,look);
        Cndei    = (cndeal2(&cndeal,dei,Alpha,look)-
        		    cndeal2(&cndeal,0.0,Alpha,look))*eieff*
        		    cnedei2(&cnedei,qacorr,Mach,look);
        Cndey    = (cndeal2(&cndeal,dey,Alpha,look)-
        		    cndeal2(&cndeal,0.0,Alpha,look))*eoeff*
                    cnedey2(&cnedey,qacorr,Mach,look);
        Cndedn   =  cndnde3(&cndnde,dn,de,Alpha,look);
        Cndn     =  cndnal2(&cndnal,dn,Alpha,look)*
        			cnedn2(&cnedn,qacorr,Mach,look);
        Cndle    = (cndleal1(&cndleal,Alpha,look)+
                    cndldnal2(&cndldnal,dn,Alpha,look)+
        			cndldeal2(&cndldeal,de,Alpha,look))*(dle/27.0);
        Cncai    =  cncai2(&cncai,cai,Alpha,look);
        Cnbeta   =  cnbeta2(&cnbeta,dn,Alpha,look)*
        			cnkb1(&cnkb,abeta,look);
		Cnlg     =  cnlst2(&cnlst,abeta,Alpha,look)*ldg;
        Cnalfad  =  cnadam2(&cnadam,aalpha,Mach,look)*alphad;
        Cnq      =  cnqal1(&cnqal,aalpha,look)*qc*
           	        cneq2(&cneq,qacorr,Mach,look);
        Cnnz     =  cnenz2(&cnenz,qacorr,Mach,look)*(Nz - 1.0);
        Cnqd     =  cneqd2(&cneqd,qacorr,Mach,look)*qd;
			/* High aoa */
		  Cnhigh  = cnhzero1(&cnhzero,Alpha,look)+
					  cnhdn2(&cnhdn,dn,Alpha,look)+
					  cnhde2(&cnhde,de,Alpha,look);   }

	else if (Mach>0.4 && Mach<0.5) {
        tflow  = (0.5-Mach)/0.1;
        tfhigh = (Mach-0.4)/0.1;

        Cnbasic  =  cnzero1(&cnzero,Mach,look)+
        			cnezero2(&cnezero,qacorr,Mach,look);
        Cnalfa   = (cndeal2(&cndeal,0.0,Alpha,look)*tflow+
        		    cnmdea3(&cnmdea,0.5,0.0,Alpha,look)*tfhigh)*
                    cnea2(&cnea,qacorr,Mach,look);
        Cndei    = ((cndeal2(&cndeal,dei,Alpha,look)-
        		    cndeal2(&cndeal,0.0,Alpha,look))*tflow+
        		   (cnmdea3(&cnmdea,0.5,dei,Alpha,look)-
                    cnmdea3(&cnmdea,0.5,0.0,Alpha,look))*tfhigh)*eieff*
                    cnedei2(&cnedei,qacorr,Mach,look);
        Cndey    = ((cndeal2(&cndeal,dey,Alpha,look)-
        		    cndeal2(&cndeal,0.0,Alpha,look))*tflow+
        		   (cnmdea3(&cnmdea,0.5,dey,Alpha,look)-
                    cnmdea3(&cnmdea,0.5,0.0,Alpha,look))*tfhigh)*eoeff*
                    cnedey2(&cnedey,qacorr,Mach,look);
        Cndedn   =  cndnde3(&cndnde,dn,de,Alpha,look)*tflow;
        Cndn     = (cndnal2(&cndnal,dn,Alpha,look)*tflow+
        		    cndnh3(&cndnh,Alpha,dn,0.5,look)*tfhigh)*
                    cnedn2(&cnedn,qacorr,Mach,look);
        Cndle    = (cndleal1(&cndleal,Alpha,look)+
        		    cndldnal2(&cndldnal,dn,Alpha,look)+
                    cndldeal2(&cndldeal,de,Alpha,look))*(dle/27.0)*tflow+
                    cnmdlea3(&cnmdlea,0.5,dle,Alpha,look)*tfhigh;
        Cncai    =  cncai2(&cncai,cai,Alpha,look);
        Cnbeta   = (cnbeta2(&cnbeta,dn,Alpha,look)*
        		    cnkb1(&cnkb,abeta,look))*tflow;
		Cnlg     =  cnlst2(&cnlst,abeta,Alpha,look)*ldg;
        Cnalfad  =  cnadam2(&cnadam,aalpha,Mach,look)*alphad;
        Cnq      = (cnqal1(&cnqal,aalpha,look)*tflow+
        		    cnqam2(&cnqam,Alpha,0.5,look)*tfhigh)*qc*
                    cneq2(&cneq,qacorr,Mach,look);
        Cnnz     =  cnenz2(&cnenz,qacorr,Mach,look)*(Nz - 1.0);
        Cnqd     =  cneqd2(&cneqd,qacorr,Mach,look)*qd;   }

    else {
        Cnbasic  =  cnzero1(&cnzero,Mach,look)+
        			cnezero2(&cnezero,qacorr,Mach,look);

        Cnalfa   =  cnmdea3(&cnmdea,Mach,0.0,Alpha,look)*
        			cnea2(&cnea,qacorr,Mach,look);
        Cndei    = (cnmdea3(&cnmdea,Mach,dei,Alpha,look)-
        		    cnmdea3(&cnmdea,Mach,0.0,Alpha,look))*eieff*
                    cnedei2(&cnedei,qacorr,Mach,look);
        Cndey    = (cnmdea3(&cnmdea,Mach,dey,Alpha,look)-
        		    cnmdea3(&cnmdea,Mach,0.0,Alpha,look))*eoeff*
                    cnedey2(&cnedey,qacorr,Mach,look);
        Cndedn   =  0.0;
        Cndn     =  cndnh3(&cndnh,Alpha,dn,Mach,look)*
        			cnedn2(&cnedn,qacorr,Mach,look);
        Cndle    =  cnmdlea3(&cnmdlea,Mach,dle,Alpha,look);
        Cncai    =  cncai2(&cncai,cai,Alpha,look);
        Cnbeta   =  0.0;
		Cnlg     =  cnlst2(&cnlst,abeta,Alpha,look)*ldg;
        Cnalfad  =  cnadam2(&cnadam,aalpha,Mach,look)*alphad;
        Cnq      =  cnqam2(&cnqam,Alpha,Mach,look)*qc*
        		    cneq2(&cneq,qacorr,Mach,look);
        Cnnz     =  cnenz2(&cnenz,qacorr,Mach,look)*(Nz - 1.0);
        Cnqd     =  cneqd2(&cneqd,qacorr,Mach,look)*qd;   }
	
/*	printf("1.  %f\n",Cnbasic); */
/*	printf("2.  %f\n",Cnalfa); */
/*	printf("3.  %f\n",Cndei); */
/*	printf("4.  %f\n",Cndey); */
/*	printf("5.  %f\n",Cndedn); */
/*	printf("6.  %f\n",Cndn); */
/*	printf("7.  %f\n",Cndle); */
/*	printf("8.  %f\n",Cncai); */
/*	printf("9.  %f\n",Cnbeta); */
/*	printf("10. %f\n",Cnalfad); */
/*	printf("11. %f\n",Cnq); */
/*	printf("12. %f\n",Cnnz); */
/*	printf("13. %f\n",Cnqd); */
/*	printf("14. %f\n",Cnlg); */

	cntot = Cnbasic+Cnalfa+Cndei+Cndey+Cndedn+Cndn+Cndle+Cncai+
		    Cnbeta+Cnalfad+Cnq+Cnnz+Cnqd+Cnlg+Cnhigh;

	return cntot;
}

/* ============================================================================ */
/* ===== Function: getCpmtotCoeff ============================================= */
/* ============================================================================ */
/* ===== Abstract:                                                        ===== */
/* =====   Algorithm for pitch moment coefficient: CPM                    ===== */
/* ============================================================================ */
double getCpmtotCoeff(double Mach,double Alpha,double Beta,double dei,double dey,
					  double de,double dn,double dle,double aalpha,double abeta,
					  double dai,double day,double adai,double aday,double adr,
					  double tflow,double tfhigh,double qacorr,double cai,double alphad,
					  double qc,double Nz,double qd,double ldg,int look,
					  InputRealPtrsType error) {

	/* Initialize to zero */
	double Cpmbasic = 0.0;
	double Cpmalfa  = 0.0;
	double Cpmdei   = 0.0;
	double Cpmdey   = 0.0;
	double Cpmdn    = 0.0;
	double Cpmdle   = 0.0;
	double Cpmcai   = 0.0;
	double Cpmbeta  = 0.0;
	double Cpmda    = 0.0;
	double Cpmalfad = 0.0;
	double Cpmq     = 0.0;
	double Cpmnz    = 0.0;
	double Cpmqd    = 0.0;
	double Cpmdedn  = 0.0;
	double Cpmlg    = 0.0;
	double cpmtot   = 0.0;
	/* High aoa */
	double Cpmhigh	 = 0.0;

	double eoeff,eieff,eoefflow,eiefflow;

	eoeff = cpmdeyod2(&cpmdeyod,aalpha,Mach,look);
    eieff = 1.0-eoeff;

    if (Mach<=0.4) {

        Cpmbasic =  cpmzero1(&cpmzero,Mach,look)+
        		    cpmezero2(&cpmezero,qacorr,Mach,look);
        Cpmalfa  =  cpmdeal2(&cpmdeal,0.0,Alpha,look)+
        		    cpmea2(&cpmea,qacorr,Mach,look)*Alpha*degtorad;
        Cpmdei   = (cpmdeal2(&cpmdeal,dei,Alpha,look)-
        		    cpmdeal2(&cpmdeal,0.0,Alpha,look))*eieff*
                    cpmedei2(&cpmedei,qacorr,Mach,look);
        Cpmdey   = (cpmdeal2(&cpmdeal,dey,Alpha,look)-
        		    cpmdeal2(&cpmdeal,0.0,Alpha,look))*eoeff*
                    cpmedey2(&cpmedey,qacorr,Mach,look);
        Cpmdedn  =  cpmdnde3(&cpmdnde,dn,de,Alpha,look);
        Cpmdn    =  cpmdnal2(&cpmdnal,dn,Alpha,look)*
        		    cpmedn2(&cpmedn,qacorr,Mach,look);
        Cpmdle   = (cpmdlal1(&cpmdlal,Alpha,look)+
        		    cpmdldea2(&cpmdldea,de,Alpha,look)+
                    cpmdldna2(&cpmdldna,dn,Alpha,look))*(dle/27.0)+
                   (cpmedlei2(&cpmedlei,qacorr,Mach,look)+
                    cpmedley2(&cpmedley,qacorr,Mach,look))*dle*degtorad;
		Cpmlg    =  cpmlst2(&cpmlst,abeta,Alpha,look)*ldg;
        Cpmcai   =  cpmcai2(&cpmcai,cai,Alpha,look)+
        		    cpmcaidn3(&cpmcaidn,dn,cai,Alpha,look);
		Cpmbeta  =  cpmbeta3(&cpmbeta,abeta,dn,Alpha,look);
        Cpmda    =  cpmdaal2(&cpmdaal,adai,Alpha,look)*eieff+
        		    cpmdaal2(&cpmdaal,aday,Alpha,look)*eoeff;
        Cpmalfad =  cpmadam2(&cpmadam,aalpha,Mach,look)*alphad;
        Cpmq     = (cpmqal1(&cpmqal,Alpha,look)+
          		    cpmqdnal2(&cpmqdnal,dn,Alpha,look))*qc*
        		    cpmeq2(&cpmeq,qacorr,Mach,look);
        Cpmnz    =  cpmenz2(&cpmenz,qacorr,Mach,look)*(Nz-1.0);
        Cpmqd    =  cpmeqd2(&cpmeqd,qacorr,Mach,look)*qd;
			/* High aoa */
		  Cpmhigh  = cpmhzero1(&cpmhzero,Alpha,look)+
					  cpmhdn2(&cpmhdn,dn,Alpha,look)+
					  cpmhde2(&cpmhde,de,Alpha,look);   }

    else if (Mach>0.4 && Mach<0.5){
        tflow  = (0.5-Mach)/0.1;
        tfhigh = (Mach-0.4)/0.1;

        eoefflow = cpmdeyod2(&cpmdeyod,aalpha,0.4,look);
        eiefflow = 1.0-eoefflow;

        Cpmbasic =  cpmzero1(&cpmzero,Mach,look)+
        		    cpmezero2(&cpmezero,qacorr,Mach,look);
        Cpmalfa  =  cpmdeal2(&cpmdeal,0.0,Alpha,look)*tflow+
        		    cpmmdea3(&cpmmdea,0.5,0.0,Alpha,look)*tfhigh+
                    cpmea2(&cpmea,qacorr,Mach,look)*Alpha*degtorad;
        Cpmdei   = ((cpmdeal2(&cpmdeal,dei,Alpha,look)-
        		    cpmdeal2(&cpmdeal,0.0,Alpha,look))*tflow+
        		   (cpmmdea3(&cpmmdea,0.5,dei,Alpha,look)-
                    cpmmdea3(&cpmmdea,0.5,0.0,Alpha,look))*tfhigh)*eieff*
                    cpmedei2(&cpmedei,qacorr,Mach,look);
        Cpmdey   = ((cpmdeal2(&cpmdeal,dey,Alpha,look)-
        		    cpmdeal2(&cpmdeal,0.0,Alpha,look))*tflow+
        		   (cpmmdea3(&cpmmdea,0.5,dey,Alpha,look)-
                    cpmmdea3(&cpmmdea,0.5,0.0,Alpha,look))*tfhigh)*eoeff*
                    cpmedey2(&cpmedey,qacorr,Mach,look);
        Cpmdedn  =  cpmdnde3(&cpmdnde,dn,de,Alpha,look)*tflow;
        Cpmdn    = (cpmdnal2(&cpmdnal,dn,Alpha,look)*tflow+
        		    cpmmdna3(&cpmmdna,0.5,dn,Alpha,look)*tfhigh)*
                    cpmedn2(&cpmedn,qacorr,Mach,look);
        Cpmdle   = (cpmdlal1(&cpmdlal,Alpha,look)+
        		    cpmdldea2(&cpmdldea,de,Alpha,look)+
                    cpmdldna2(&cpmdldna,dn,Alpha,look))*(dle/27.0)*tflow+
                    cpmmdlea3(&cpmmdlea,0.5,dle,Alpha,look)*tfhigh+
        		   (cpmedlei2(&cpmedlei,qacorr,Mach,look)+
                    cpmedley2(&cpmedley,qacorr,Mach,look))*dle*degtorad;
		Cpmlg    =  cpmlst2(&cpmlst,abeta,Alpha,look)*ldg;
        Cpmcai   =  cpmcai2(&cpmcai,cai,Alpha,look)+
        		    cpmcaidn3(&cpmcaidn,dn,cai,Alpha,look);
        Cpmbeta  =  cpmbeta3(&cpmbeta,abeta,dn,Alpha,look)*tflow;
        Cpmda    = (cpmdaal2(&cpmdaal,adai,Alpha,look)*eiefflow+
        		    cpmdaal2(&cpmdaal,aday,Alpha,look)*eoefflow)*tflow;
        Cpmalfad =  cpmadam2(&cpmadam,aalpha,Mach,look)*alphad;
        Cpmq     = ((cpmqal1(&cpmqal,Alpha,look)+
        		    cpmqdnal2(&cpmqdnal,dn,Alpha,look))*tflow+
                    cpmqam2(&cpmqam,Alpha,0.5,look)*tfhigh)*qc*
                    cpmeq2(&cpmeq,qacorr,Mach,look);
        Cpmnz    =  cpmenz2(&cpmenz,qacorr,Mach,look)*(Nz-1.0);
        Cpmqd    =  cpmeqd2(&cpmeqd,qacorr,Mach,look)*qd;   }

    else {
        Cpmbasic =  cpmzero1(&cpmzero,Mach,look)+
        		    cpmezero2(&cpmezero,qacorr,Mach,look);
        Cpmalfa  =  cpmmdea3(&cpmmdea,Mach,0.0,Alpha,look)+
        		    cpmea2(&cpmea,qacorr,Mach,look)*Alpha*degtorad;
        Cpmdei   = (cpmmdea3(&cpmmdea,Mach,dei,Alpha,look)-
        		    cpmmdea3(&cpmmdea,Mach,0.0,Alpha,look))*eieff*
                    cpmedei2(&cpmedei,qacorr,Mach,look);
        Cpmdey   = (cpmmdea3(&cpmmdea,Mach,dey,Alpha,look)-
        		    cpmmdea3(&cpmmdea,Mach,0.0,Alpha,look))*eoeff*
				    cpmedey2(&cpmedey,qacorr,Mach,look);
        Cpmdedn  =  0.0;
        Cpmdn    =  cpmmdna3(&cpmmdna,Mach,dn,Alpha,look)*
        		    cpmedn2(&cpmedn,qacorr,Mach,look);
        Cpmdle   =  cpmmdlea3(&cpmmdlea,Mach,dle,Alpha,look)+
        		   (cpmedlei2(&cpmedlei,qacorr,Mach,look)+
                    cpmedley2(&cpmedley,qacorr,Mach,look))*dle*degtorad;
		Cpmlg    =  cpmlst2(&cpmlst,abeta,Alpha,look)*ldg;
        Cpmcai   =  cpmcai2(&cpmcai,cai,Alpha,look)+
        		    cpmcaidn3(&cpmcaidn,dn,cai,Alpha,look);
        Cpmbeta  =  0.0;
        Cpmda    =  0.0;
        Cpmalfad =  cpmadam2(&cpmadam,aalpha,Mach,look)*alphad;
        Cpmq     =  cpmqam2(&cpmqam,Alpha,Mach,look)*qc*
        	        cpmeq2(&cpmeq,qacorr,Mach,look);
        Cpmnz    =  cpmenz2(&cpmenz,qacorr,Mach,look)*(Nz-1.0);
        Cpmqd    =  cpmeqd2(&cpmeqd,qacorr,Mach,look)*qd;   }

    cpmtot = Cpmbasic+Cpmalfa+Cpmdei+Cpmdey+Cpmdedn+Cpmdn+Cpmdle+
		     Cpmcai+Cpmbeta+Cpmda+Cpmalfad+Cpmq+Cpmnz+Cpmqd+Cpmlg+
			 dCm_alpha*Alpha*degtorad+dCm_q*qc+dCm_dne*de*degtorad+
			 dCm_dey*dey*degtorad+dCm_dei*dei*degtorad+Cpmhigh;

	return cpmtot;
}

