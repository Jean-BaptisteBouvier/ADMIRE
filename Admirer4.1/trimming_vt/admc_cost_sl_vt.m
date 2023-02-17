function COST = admc_cost_sl_vt(vt)

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
if ct(16) < 0.6 % Check for Mach No.
   fes = 0; % Commands pitch rate
else 
   fes = 0; % Commands Nz
end

u0t = [fes vt(2) ct(14) ct(15) vt(1)];
x0t(2) = vt(3); % Let AoA vary
x0t(8) = vt(3); % Theta follows Alpha
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

% Calc. cost. Try changing the factors if you do not succeed in trimming.
COST = 0.1*Cd + 25*dVt + 14*dalpha + 22*dQ + 0.000001*Ztot + 10*Q + 0.000001*Mtot;
