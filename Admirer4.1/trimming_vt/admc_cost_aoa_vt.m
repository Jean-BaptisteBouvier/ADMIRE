function COST = admc_cost_aoa_vt(vt)

% Cost function for ADMIRE 3.4e, for trimming
% including the speed controller. Not entirely 
% reliable.

% Set global parameters
global tt u0t y0t; 
global ct;
global x0t;
global dx0t;

% Set boundaries for input parameters
lb = [-7  0 -1]; % Lower boundary
ub = [ 7 500  1]; % Upper boundary

% Check boundaries
index1 = vt<lb; index2 = vt>ub;
if((sum(index1)>0) | (sum(index2)>0))
   COST = 1e25; return;
end

% Setup vectors
%   Stick pitch, demanded speed, stick roll, pedals, integrator
if ct(17) < 0.6 % Check for Mach No.
   fes = vt(3)/(22.8083/80)*180/pi; % Commands pitch rate
else 
   fes = vt(3)*ct(3)/9.81*80/9; % Commands Nz
end

u0t = [fes vt(2) ct(15) ct(16) vt(1)];
x0t(5) = vt(3); % Let Q vary
% Call model for outputs and derivatives
for i=1:3
   y0t  = admire_complete_trim(0,x0t,u0t,'outputs');
   dx0t = admire_complete_trim(0,x0t,u0t,'derivs');
end;

% Setup useful variables
Q      = (y0t(5))^2;   Cd   = (y0t(23))^2;
Xtot   = (y0t(29))^2;  Ztot = (y0t(30))^2;
Mtot   = (y0t(31))^2;  dVt  = (dx0t(1))^2;
dalpha = (dx0t(2))^2;  dQ   = (dx0t(5))^2;
alphaerr = (y0t(2) - ct(4))^2;

% Calc. cost. Try changing the factors if you do not succeed to trim.
COST = 80*dVt + 4*alphaerr + 10*dalpha + 2*dQ + 0.0000001*Mtot;
