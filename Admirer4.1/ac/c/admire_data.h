/*																			*/
/*  Copyright (c) 1997 by Lars Forssell, FFA Sweden							*/
/*  All Rights Reserved														*/
/*																			*/
/*  File: adm_data.h														*/
/*																			*/
/*  Include file for geometry reference data for the Generic Aerodata Model	*/
/*																			*/
/*	  Sref		reference area (m^2)										*/
/*	  cref		reference chord (m)											*/
/*	  bref		reference span (m)											*/
/*	  Sei			reference area, inboard elevon (m^2)					*/
/*	  cei			reference chord, inboard elevon (m)						*/
/*	  Sey			reference area, outboard elevon (m^2)					*/
/*	  cey			reference chord, outboard elevon (m)					*/
/*	  Slei		reference area, inboard leading-edge flap (m^2)				*/
/*	  clei		reference chord, inboard leading-edge flap (m)				*/
/*	  Sley		reference area, outboard leading-edge flap (m^2)			*/
/*	  cley		reference chord, outboard leading-edge flap (m)				*/
/*	  Sr			reference area, rudder (m^2)							*/
/*	  cr			reference chord, rudder (m)								*/
/*	  clbr		reference lenght, air brake (width of brake) (m)			*/
/*	  Snv			reference area, canard (m^2)							*/
/*	  cnv			reference chord, canard (m)								*/
/*	  bnv			reference span, canard (m)								*/

static double Sref =  45.0;
static double cref =  5.20;
static double bref =  10.0;
static double Sei	 =  1.20;
static double cei	 =  0.95;
static double Sey	 =  1.35;
static double cey	 =  0.68;
static double Slei =  0.45;
static double clei =  0.40;
static double Sley =  0.45;
static double cley =  0.35;
static double Sr	 =  0.80;
static double cr	 =  0.60;
static double clbr =  1.10;
static double Snv	 =  3.20;
static double cnv	 =  1.30;
static double bnv	 =  2.60;

/*static double Ix	 =  21000.0; */
/*static double Iy	 =  81000.0; */
/*static double Iz	 =  101000.0; */
/*static double Ixz	 =  2500.0; */

/*static double ycg =   0.0; */
/*static double xcg =   0.25*5.20;   Stable if sign is negative */
/*static double zcg =   0.0; */

static double yeng =  0.0;
static double zeng = -0.15;

/*static double mass =  9100.0; */
static double grav =  9.81;

