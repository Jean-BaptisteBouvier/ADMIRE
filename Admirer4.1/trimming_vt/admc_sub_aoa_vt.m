function [x0t,u0t,y0t,dx0t] = admc_sub_aoa_vt(Mach,Alt,x00,u00,AoA)

% Trimming routine for ADMIRE r3.3
% Written by Torbjorn Noren, Lars
% Forssell and Fredrik Johansson.
% Modified by Gunnar Hovmark for 
% ADMIRE 3.4d.
% New name for ADMIRE 3.4e, to be used if
% you want to trim including the speed
% controller. Not entirely reliable.

% Setup global parameters
clear ct u0t y0t dx0t
global ct;   ct   = [];
global x0t;  x0t  = x00;
global u0t;  u0t  = [];
global y0t;  y0t  = [];
global dx0t; dx0t = [];

% Constants:
g    = 9.81;  % Gravity
Mass = 9100;  % Nominal aircraft mass

% Variables that are not allowed to vary
% during trim are stored in vector 'ct'.
Vt    =  Mach*atmosphere(Alt);
Alpha =  AoA; 
Beta  =  0;
P     =  0;
% Q     =  0;
R     =  0;
Psi   =  0;
Theta =  AoA; 
Phi   =  0;
X     =  0;
Y     =  0;
Z     = -Alt;
dle = 0;   

x0t(1) = Vt;
x0t(2) = AoA;
x0t(8) = Theta;

fas = 0;
frp = 0;

ct = [g Mass Vt Alpha Beta P R Psi Theta Phi X Y Z dle fas frp Mach];

% Add warnings...
if (abs(Mach*atmosphere(Alt) - x00(1))) > 1
   fprintf('\nWarning!!!!!!!!!!!!!!\nCalculated Speed and Interpolated value from fcsx0 do not match.\n');
   fprintf('You may be outside the envelope that the control system is designed for.\n');
   fprintf('Calculated: %8.1f\t interpolated: %8.1f \n',Mach*atmosphere(Alt),x00(1));  
end
if Z > 0 | Z < -6000
   fprintf('\nWarning!!!!!!!!!!!!!!\nYou are outside the control system altitude envelope.\n');
end

% Variables that vary during trim are stored
% in vector 'vt'.
Q = 0; % Improves the chance of getting a correct sign?
vel = Mach * atmosphere(Alt); % I.e. it's commanded Vt really...
integrator = 0;
vt  = [integrator vel Q];

% Options for the minimisation routine FMINSEARCH.
options = optimset('fminsearch');
options = optimset(options,'TolFun',1e-7,'TolX',1e-9);
options = optimset(options,'MaxIter',20000,'MaxFunEvals',20000);
options = optimset(options,'Display','off');

% Call minimisation routine FMINSEARCH.
fprintf(1,'\nTrimming ADMIRE complete trim..........',Alt,Mach);
[vt_out,fval,exitflag,out] = fminsearch('admc_cost_aoa_vt',vt,options);
if(~exitflag)
   fprintf(1,'\n\nOptimization exceeded maximum number of iterations.\n\n');
else
   fprintf(1,' DONE!\n');
end
