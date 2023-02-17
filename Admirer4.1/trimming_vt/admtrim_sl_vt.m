% Filename: admtrim_sl_vt.m
% ADMIRE r3.4e Trimming start script. For trimming 
% including the speed controller.
% Written by Torbjorn Noren and 
% Gunnar Hovmark.
%
% The script will ask for whatever
% it might need.
global u00;
global x0t;

% Feed the system...
Mach = input('\nMach number [-] : ');
Alt  = input('Altitude    [m] : ');
%Alpha = input('Required angle of attack: [deg] ');

% Load basic flight data
uncertainty;		% Load uncertainty parameters	
setup_ratelims;	% Ratelimiter data
setup_tasks;      % Only necessary if you want to use pitch and/or roll
                  % tracking tasks.
r2deg = 180/pi;

% Get interpolated data (x00 and u00)
get_x0u0([],[],[],'compile');
u0x0 = get_x0u0(0,[],[],'outputs');
get_x0u0([],[],[],'term');
u00 = u0x0( 1:16);
x00 = u0x0(17:28);

get_x0u0unc([],[],[],'compile');
u0x0unc = get_x0u0unc(0,[],[],'outputs');
get_x0u0unc([],[],[],'term');
x00unc = u0x0unc(17:28);

Alpha = x00(2);

% For compatibility with admire_sim.mdl:
x0bare = x00;
dx0bare = zeros(12,1);
x0diff = zeros(12,1);
u0bare = u00;
u0new = u00;
u0corr = zeros(16,1);

% Trim aircraft including flight control system
admire_complete_trim([],[],[],'compile');
[x0c,u0c,y0c,dx0c] = admc_sub_sl_vt(Mach,Alt,x00,u00,Alpha);
admire_complete_trim([],[],[],'term');

x0bare = x0c; u0fcs = u0c; y0bare = y0c; dx0bare = dx0c; % Just to fit admire_sim.mdl

% Setup u0new for computation of actuator states
u0new(1:7) = y0bare(32:38); % Canards, elevons and rudder
u0new(8) = y0bare(39); % LE flaps
u0new(9)   = 0; % Landing gear in
u0new( 10) = y0bare(40); % Thrust 

% For use with admire_bare_sim:
u0bare(1) = u0new(1); %Canards
u0bare(2) = u0new(3); %Elevons
u0bare(4) = u0new(8); % LE flap
u0bare(3) = u0new(10); % Thrust

% Assemble complete x0-vector
admtrim_states_aoa;

fprintf('u0new(1)  = %8.5f deg\t( Canard deflections , + = trailing edge down )\n',u0new(1)*r2deg);
fprintf('u0new(3)  = %8.5f deg\t( Elevon deflections , + = trailing edge down )\n',u0new(3)*r2deg);
fprintf('u0new(7)  = %8.5f deg\t( Rudder deflection  , + = trailing edge left )\n',u0new(7)*r2deg);
fprintf('u0new(8)  = %8.5f deg\t( Leading edge defl. , + = leading edge down  )\n',u0new(8)*r2deg);
fprintf('u0new(10) = %8.5f    \t( Throttle setting                            )\n\n',u0new(10));

% Show trimmed values for admire_main´s states:
fprintf('After trimming for AoA:\n');
fprintf('x0(1)  = %8.5f\t( Vt    [m/s]   )\n',x0bare(1));
fprintf('x0(2)  = %8.5f\t( AoA   [deg]   )\n',x0bare(2)*r2deg);
fprintf('x0(3)  = %8.5f\t( Beta  [deg]   )\n',x0bare(3)*r2deg);
fprintf('x0(4)  = %8.5f\t( P     [deg/s] )\n',x0bare(4)*r2deg);
fprintf('x0(5)  = %8.5f\t( Q     [deg/s] )\n',x0bare(5)*r2deg);
fprintf('x0(6)  = %8.5f\t( R     [deg/s] )\n',x0bare(6)*r2deg);
fprintf('x0(7)  = %8.5f\t( Psi   [deg]   )\n',x0bare(7)*r2deg);
fprintf('x0(8)  = %8.5f\t( Theta [deg]   )\n',x0bare(8)*r2deg);
fprintf('x0(9)  = %8.5f\t( Phi   [deg]   )\n',x0bare(9)*r2deg);
fprintf('x0(10) = %8.5f\t( X     [m]     )\n',x0bare(10));
fprintf('x0(11) = %8.5f\t( Y     [m]     )\n',x0bare(11));
fprintf('x0(12) = %8.5f\t( Z     [m]     )\n\n',x0bare(12));

if abs(u0fcs(1)) > 80
   fprintf('Warning: required stick pitch input exceeds the +-80 N saturation limit in the Simulink model!');
elseif abs(dx0bare(1)) > 0.1
   fprintf('Warning: not able to maintain speed at specified conditions.');
else
   fprintf('Run simulations with ''admire_sim.mdl'' and view the results with command ''trimplot''\n\n');
end
