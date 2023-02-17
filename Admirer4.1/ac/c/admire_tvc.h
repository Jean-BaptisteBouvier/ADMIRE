void engVectorCalculation(double Thrust, double tss ,double rho, double m_dot_fuel,
						  double Vt,double alpha, double beta, double dth,double dtv,
						  double *Tx,double *Ty,double *Tz,double *cai) {

	double Ainf = 0.38;					/* engine inlet area */

	double m_dot_air;	/* Mass flow [kg/s] */
	double Vx;
	double Vy;
	double Vz;
	double f;
	double Tv;
	double delta_max;
	double R_rad;
	double fi;
	double R_max;
	double andel;
	double R_ny;
	double delta_av;
	double delta_ah;
	double rad_delta_v;
	double rad_delta_h;
	double S;
	double Tvx;
	double Tvy;
	double Tvz;

	/*extern double Tx,Ty,Tz,cai;		 output values */
	if (tss < 0.8)
		f = 1/38.0;
	else
		f = 1/25.0;

	*cai = m_dot_fuel/(f*rho*Vt*Ainf);	/* capture air ratio */

	m_dot_air = m_dot_fuel/f;			/* air mass flow */

	Vx = Vt*cos(alpha)*cos(beta);
    Vy = Vt*sin(beta);
	Vz = Vt*sin(alpha)*cos(beta);

	Tv = Thrust+m_dot_air*Vx;

	delta_max = 25*Pi/180;

	rad_delta_v = dtv;
	rad_delta_h = dth;

	R_rad = sqrt(rad_delta_v*rad_delta_v+rad_delta_h*rad_delta_h);

	if (R_rad==0)
        fi=0;
	else if (rad_delta_v<0)
        fi=2*Pi-acos(rad_delta_h/R_rad);
	else
        fi=acos(rad_delta_h/R_rad);
	
	if (fabs(rad_delta_v) == fabs(rad_delta_h))
        R_max = sqrt(2*delta_max*delta_max);
	if (fabs(rad_delta_h) > fabs(rad_delta_v))
        R_max = fabs(delta_max/cos(fi));
	else if (fabs(rad_delta_h) < fabs(rad_delta_v)) 
        R_max = fabs(delta_max/sin(fi));
	if (R_rad==0) 
        andel=0;
	else
        andel=R_rad/R_max;
	
	R_ny = delta_max*andel;

	delta_av = R_ny*sin(fi);
	delta_ah = R_ny*cos(fi);

	S = 1/sqrt(cos(delta_av)*cos(delta_av)+cos(delta_ah)*
		cos(delta_ah)*sin(delta_av)*sin(delta_av));

	Tvx =  Tv*cos(delta_av)*cos(delta_ah)*S;
	Tvy =  Tv*cos(delta_av)*sin(delta_ah)*S;
	Tvz = -Tv*sin(delta_av)*cos(delta_ah)*S;

	*Tx = Tvx-m_dot_air*Vx;
	*Ty = Tvy;
	*Tz = Tvz;

	return;	
}

