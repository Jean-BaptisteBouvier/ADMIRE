function [x0t,u0t,y0t,dx0t] = admc_sub(Mach,Alt,x00,u00,AoA,dMach_err)

% Trimming routine for ADMIRE r3.3
% Written by Torbjorn Noren, Lars Forssell and Fredrik Johansson.
% Modified by Gunnar Hovmark and Harrald Luijerink for ADMIRE 3.4e.
% 2001-06-27: Change for ADMIRE3.4f: dMach_err removed from vel calculation. /Gunnar Hovmark

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
Beta  =  0;
P     =  0;
Q     =  0;
R     =  0;
Psi   =  0;
Phi   =  0;
X     =  0;
Y     =  0;
Z     = -Alt;
dle = 0;   

x0t(1) = Vt;

fas = 0;
frp = 0;

vel 			= (Mach) * atmosphere(Alt); % I.e. it's commanded Vt really...
ct = [g Mass Vt Beta P Q R Psi Phi X Y Z dle fas frp Mach vel];

% Add warnings...
if (abs(Mach*atmosphere(Alt) - x00(1))) > 1
   fprintf('\nWarning!!!!!!!!!!!!!!\nCalculated Speed and Interpolated value from fcsx0 do not match.\n');
   fprintf('You may be outside the envelope that the control system is designed for.\n');
   fprintf('Calculated: %8.1f\t interpolated: %8.1f \n',Mach*atmosphere(Alt),x00(1));  
end
if Z > 0 | Z < -6000
   fprintf('\nWarning!!!!!!!!!!!!!!\nYou are outside the control system altitude envelope.\n');
end

% Variables that vary during trim are stored in vector 'vt'.
integrator 	= 0;
Alpha 		= AoA;
Theta 		= Alpha; 
vt  			= [integrator Alpha];

% Options for the minimisation routine FMINSEARCH.
options = optimset('fminsearch');
options = optimset(options,'TolFun',1e-7,'TolX',1e-12);
options = optimset(options,'MaxIter',20000,'MaxFunEvals',20000);
options = optimset(options,'Display','off');

% Call minimisation routine FMINSEARCH.
fprintf(1,'\nTrimming ADMIRE complete trim..........',Alt,Mach);
[vt_out,fval,exitflag,out] = fminsearch('admc_cost_sl',vt,options);
if(~exitflag)
   fprintf(1,'\n\nOptimization exceeded maximum number of iterations.\n\n');
else
   fprintf(1,' DONE!\n');
end
