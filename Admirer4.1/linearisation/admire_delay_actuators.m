% Filename: admire_delay_actuators.m
% Created on 25 January 2001
% Last revised 7 June 2001
% Harrald Luijerink, DUT, Faculty of Aerospace Engineering & FOI, Flyg och Autonoma System 
% mailto: h.r.luijerink@student.tudelft.nl 

% This m-file creates the same Time Delays, Actuator Dynamics and Engine Dynamics as have been 
% specified in SIMULINK model 'admire_linear.mdl' 

N = 2; 															% Order of time delay is 2
time_delay = 20e-003; 										% 20 msec time delay
[num_Tdelay,den_Tdelay] = pade(time_delay,N);
Tdelay = tf(num_Tdelay,den_Tdelay);

% SEE: uncertainty.m for the ADMIRE Uncertainty (Unc.) parameters by Fredrik Johansson FFA.
% For running 'Admire_delay_actuators.m' independently while ddc_bw is not in the workspace
ddc_bw = 0;                       % Note: The same uncertainty is here used for all actuators.

num_act  = [0 1];					
den_act  = [0.05+ddc_bw  1];  	
Actuator = tf(num_act,den_act);							% For the leading edge deflection (dle)

% For the right&left canard deflections (drc and dlc), the right outer and inner elevon deflections (droe and drie),
% the left inner and outer elevon delflections (dlie and dloe) and rudder deflection (dr):
TdelayActuator = Tdelay*Actuator;

num_engdyn = [0 1]; 			
den_engdyn = [2 1];
EngineDynamics = tf(num_engdyn,den_engdyn); 			% For the throttle stick setting (tss):

% For the landing gear (ldg), engine nozzle deflection in both x and z plane (dty and dtz), 
% turbulance in X_b, Y_b and Z_b axis (u_dist, v_dist and w_dist) and turbulance around roll axis (p_dist) 
% transfer function is simply 1

systeem = append(TdelayActuator,TdelayActuator,TdelayActuator,TdelayActuator,TdelayActuator,TdelayActuator,...
   TdelayActuator,Actuator,EngineDynamics,1,1,1,1,1,1,1);
