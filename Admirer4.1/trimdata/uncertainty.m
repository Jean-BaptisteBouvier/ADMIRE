%---------------------------------
% ADMIRE Uncertainty (Unc.) parameters
% 2000-01-03, Fredrik Johansson FFA.
% 2001-01-03, Revised for dMach_err by Harrald Luijerink (DUT), line 55 to 59.
% 2002-12-18, Added uncertainties for leading edge actuators. 
%             Gunnar Hovmark
%---------------------------------

%---------------------------------
% Configuration:
%---------------------------------
dmass	=	0;
dxcg	=	0;
dycg	=	0;
dzcg	=	0;
dIxx	=	0;
dIyy	=	0;
dIzz	=	0;
dIxz	=	0;


%---------------------------------
% Roll:
%---------------------------------
dCl_beta	=	0;
dCl_p		=	0;
dCl_r		=	0;
dCl_day	=	0;
dCl_dai	=	0;
dCl_dr	=	0;


%---------------------------------
% Pitch:
%---------------------------------
dCm_alpha	=	0;
dCm_q			=	0;
dCm_dne		=	0;
dCm_dey		=	0;
dCm_dei		=	0;


%---------------------------------
% Yaw:
%---------------------------------
dCn_zero		=	0;
dCn_beta		=	0;
dCn_p			=	0;
dCn_r			=	0;
dCn_dna		=	0;
dCn_dr		=	0;


%---------------------------------
% Sensors:
%---------------------------------
% Revised 1 March 2001
SoS = atmosphere(Alt);  % Speed of Sound is computed
dMach_err	=  0;
dVt_err		=	SoS*dMach_err;
dalt_err		=	0;
dalpha_err	=	0*pi/180;
dbeta_err	=	0*pi/180; % Do not set this != 0. Only way to compensate for it is via pedal input.

%---------------------------------
% Actuators:
% Revised December 18th, 2002, Gunnar Hovmark
%---------------------------------
ddc_rate	=	0; % Canard
ddc_bw		=	0;
ddey_rate	=	0; % Elevon "yttre" (outer)
ddey_bw		=	0;
ddei_rate	=	0; % Elevon inner
ddei_bw		=	0;
ddr_rate    =	0; % Rudder
ddr_bw		=	0;				
ddle_rate   =   0; % Leading edge
ddle_bw     =   0;