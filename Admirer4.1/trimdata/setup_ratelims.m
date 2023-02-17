% Set-up the rate limiters in the new Saturators\nRatelimiters\nand\nActuators block.
% Gunnar Hovmark 2000-02-18
% Note that this file is adapted to Admire 3.4c. In Admire 3.4d
% you will always get the standard MATLAB rate limiter with the
% rlimit_* settings given below.
% See the folder 'simulink\Old but may be useful' to see the 
% Saab and DASA phase compensating rate limiters.

% rlselect = 1; % No rate limiter
 rlselect = 2; % Standard rate limiter
% rlselect = 3; % DASA rate limiter
% rlselect = 4; % Saab rate limiter

% Canard rate limiters
rlimit_c = 50/57.3; %rad/s
% Nominal settings
dasatc1_c = 0.05;
dasatc2_c = 1;
saabgain_c = 8;
saabtc1_c = 0.1;
saabtc2_c = 1;


% Outer elevon rate limiters
rlimit_oe = 150/57.3; %rad/s
% Nominal settings
dasatc1_oe = 0.05;
dasatc2_oe = 1;
saabgain_oe = 8;
saabtc1_oe = 0.1;
saabtc2_oe = 1;

% Inner elevon rate limiters
rlimit_ie = 150/57.3; %rad/s
% Nominal settings
dasatc1_ie = 0.05;
dasatc2_ie = 1;
saabgain_ie = 8;
saabtc1_ie = 0.1;
saabtc2_ie = 1;

% Rudder rate limiters.
rlimit_r = 100/57.3; %rad/s
% Nominal settings
dasatc1_r = 0.05;
dasatc2_r = 1;
saabgain_r = 8;
saabtc1_r = 0.1;
saabtc2_r = 1;

% Leading edge rate limiters
rlimit_le = 10/57.3; %rad/s. No deep thought behind the value 10 deg/s
% Nominal settings
dasatc1_le = 0.05;
dasatc2_le = 1;
saabgain_le = 8;
saabtc1_le = 0.1;
saabtc2_le = 1;