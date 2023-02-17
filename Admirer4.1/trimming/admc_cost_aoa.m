function COST = admc_cost(vt)

% Cost function for ADMIRE FCS release 3.4e

% Set global parameters
global tt u0t y0t; 
global ct;
global x0t;
global dx0t;

% Set boundaries for input parameters
lb = [-7 -1]; 	% Lower boundary
ub = [ 7 2]; 	% Upper boundary

% Check boundaries
index1 = vt<lb; index2 = vt>ub;
if((sum(index1)>0) | (sum(index2)>0))
   COST = 1e25; 
   return;
end

% Setup vectors
% Stick pitch, demanded speed, stick roll, pedals, integrator
if ct(17) < 0.6 							% Check for Mach No.
   fes = vt(2)/(22.8083/80)*180/pi; % Commands pitch rate
else 
   fes = vt(2)*ct(3)/9.81*80/9; 		% Commands Nz
end

%if fes > 80
%   fprintf('\nRequired fes too large: %5.2f\n',fes);
%elseif fes < -53
%   fprintf('\nRequired fes too small: %6.2f\n',fes);
%end

u0t = [fes ct(18) ct(15) ct(16) vt(1)]; 	% Speed demand with consideration of dMach_err = ct(18)
x0t(5) = vt(2); 									% Let Q vary
% Call model for outputs and derivatives
for i=1:5
   y0t  = admire_complete_trim(0,x0t,u0t,'outputs');
   dx0t = admire_complete_trim(0,x0t,u0t,'derivs');
end;

% Setup useful variables
Q      = (y0t(5))^2;   Cd   = (y0t(23))^2;
Xtot   = (y0t(29))^2;  Ztot = (y0t(30))^2;
Mtot   = (y0t(31))^2;  dVt  = (dx0t(1))^2;
dalpha = (dx0t(2))^2;  dQ   = (dx0t(5))^2;
alphaerr = (y0t(2) - ct(4))^2;

% Calc. cost
integrator 	= vt(1);
vel_command = ct(3);
pitch_rate 	= vt(2)*180/pi;

%NOTE: New trimming method for ADMIRE 3.4e, dVt is deleted from the COST function!!!

COST = 4*alphaerr + 10*dalpha + 2*dQ + 0.0000001*Mtot;
