function [x0t,u0t,y0t,dx0t] = admc_sub(Mach,Alt,x00,u00,AoA,dMach_err)

% Trimming routine for ADMIRE r3.3
% Written by Torbjorn Noren, Lars Forssell and Fredrik Johansson.
% Modified by Gunnar Hovmark for ADMIRE 3.4d.
% Modified by Gunnar Hovmark and Harrald Luijerink for ADMIRE 3.4e
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

% Variables that are not allowed to vary during trim are stored in vector 'ct'.
Vt    =  Mach*atmosphere(Alt);
Alpha =  AoA; 
Beta  =  0;
P     =  0;
R     =  0;
Psi   =  0;
Theta =  AoA; 
Phi   =  0;
X     =  0;
Y     =  0;
Z     = -Alt;
dle   =  0;   

x0t(1) = Vt;
x0t(2) = AoA;
x0t(8) = Theta;

fas = 0;
frp = 0;

% Variables that vary during trim are stored in vector 'vt'.
Q 				= 0;					% Improves the chance of getting a correct sign?
integrator 	= 0;
vt  			= [integrator Q]; % We don't trim the speed here.

% Vel is now used to get a reasonable thrust setting. 
vel = (Mach) * atmosphere(Alt);  

ct = [g Mass Vt Alpha Beta P R Psi Theta Phi X Y Z dle fas frp Mach vel];

% Options for the minimisation routine FMINSEARCH.
options = optimset('fminsearch');
options = optimset(options,'TolFun',1e-7,'TolX',1e-9);
options = optimset(options,'MaxIter',10000,'MaxFunEvals',10000);
options = optimset(options,'Display','off');

% Call minimisation routine FMINSEARCH.
[vt_out,fval,exitflag,out] = fminsearch('admc_cost_aoa',vt,options);

if(~exitflag)
   fprintf(1,'\n\nOptimization exceeded maximum number of iterations.\n\n');
   pause
else
   fprintf(1,' DONE!\n');
end





