/* Changed by Anton Vooren 301101 */
/* Under Mach 0.5 alpha envelope in [-30..90] and beta in [-30..30] */
/* Output in degrees */


int testAdmEnvelope(double Mach, double Alpha, double Beta) {
	if (Mach<=0.5) {
		if ((Alpha>90*degtorad) || (Alpha<-30*degtorad)) {
            printf("Out of envelope: Alpha = %f\n",Alpha*180/3.14);
            return 1;
        }
        else if ((Beta>30*degtorad) || (Beta<-30*degtorad)) {
            printf("Out of envelope: Beta = %f\n",Beta*180/3.14);
            return 1;
        }
    }
    if ((Mach<=0.8) && (Mach>0.5)) {
        if ((Alpha>30*degtorad) || (Alpha<-10*degtorad)) {
            printf("Out of envelope: Alpha = %f\n",Alpha*180/3.14);
            return 1;
		}
        else if ((Beta>15*degtorad) || (Beta<-15*degtorad)) {
			printf("Out of envelope: Beta = %f\n",Beta*180/3.14);
            return 1;
        }
    }
    if ((Mach<=0.9) && (Mach>0.8)) {
        if ((Alpha>25*degtorad) || (Alpha<-10*degtorad)) {
			printf("Out of envelope: Alpha = %f\n",Alpha*180/3.14);
            return 1;
		}
		else if ((Beta>15*degtorad) || (Beta<-15*degtorad)) {
			printf("Out of envelope: Beta = %f\n",Beta*180/3.14);
            return 1;
		}
    }
    if ((Mach<=0.95) && (Mach>0.9)) {
        if ((Alpha>25*degtorad) || (Alpha<-10*degtorad)) {
			printf("Out of envelope: Alpha = %f\n",Alpha*180/3.14);
            return 1;
        }
        else if ((Beta>10*degtorad) || (Beta<-10*degtorad)) {
            printf("Out of envelope: Beta = %f\n",Beta*180/3.14);
            return 1;
		}

    }
    if ((Mach<=1.0) && (Mach>0.95)) {
        if ((Alpha>25*degtorad) || (Alpha<-10*degtorad)) {
			printf("Out of envelope: Alpha = %f\n",Alpha*180/3.14);
			return 1;
		}
        else if ((Beta>10*degtorad) || (Beta<-10*degtorad)) {
			printf("Out of envelope: Beta = %f\n",Beta*180/3.14);
            return 1;
		  }
	 }
    if ((Mach<=1.1) && (Mach>1.0)) {
        if ((Alpha>20*degtorad) || (Alpha<-10*degtorad)) {
			printf("Out of envelope: Alpha = %f\n",Alpha*180/3.14);
            return 1;
		}
        else if ((Beta>10*degtorad) || (Beta<-10*degtorad)) {
			printf("Out of envelope: Beta = %f\n",Beta*180/3.14);
            return 1;
		}

    }
    if ((Mach<=1.3) && (Mach>1.1)) {
        if ((Alpha>20*degtorad) || (Alpha<-10*degtorad)) {
            printf("Out of envelope: Alpha = %f\n",Alpha*180/3.14);
            return 1;
		}
        else if ((Beta>7.5*degtorad) || (Beta<-7.5*degtorad)) {
			printf("Out of envelope: Beta = %f\n",Beta*180/3.14);
            return 1;
		}
    }
    if ((Mach<=1.4) && (Mach>1.3)) {
        if ((Alpha>15*degtorad) || (Alpha<-5*degtorad)) {
			printf("Out of envelope: Alpha = %f\n",Alpha*180/3.14);
            return 1;
		}
        else if ((Beta>7.5*degtorad) || (Beta<-7.5*degtorad)) {
			printf("Out of envelope: Beta = %f\n",Beta*180/3.14);
            return 1;
        }
	}
    if ((Mach<=1.5) && (Mach>1.4)) {
        if ((Alpha>15*degtorad) || (Alpha<-5*degtorad)) {
			printf("Out of envelope: Alpha = %f\n",Alpha*180/3.14);
            return 1;
		}
        else if ((Beta>7.5*degtorad) || (Beta<-7.5*degtorad)) {
			printf("Out of envelope: Beta = %f\n",Beta*180/3.14);
            return 1;
		}
    }
    if ((Mach<=2.5) && (Mach>1.5)) {
        if ((Alpha>15*degtorad) || (Alpha<-5*degtorad)) {
			printf("Out of envelope: Alpha = %f\n",Alpha*180/3.14);
            return 1;
		}
        else if ((Beta>7.5*degtorad) || (Beta<-7.5*degtorad)) {
			printf("Out of envelope: Beta = %f\n",Beta*180/3.14);
            return 1;
		}
    }
    return 0;
}

