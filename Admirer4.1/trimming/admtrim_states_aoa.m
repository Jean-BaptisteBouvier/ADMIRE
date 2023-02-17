% Sets up the full state vector 'x0' by
% calculating all states except for the
% original 12 (states 10 to 21).
% Changes:
% 2002-12-18: Change for ADMIRE 3.4h: Added individual uncertainties for actuator bandwidths.
%             Gunnar Hovmark.
% STATES
% 1.	DRC Actuator
% 2.	DLC Actuator
% 3.	DROE Actuator
% 4.	DRIE Actuator
% 5.	DLIE Actuator
% 6.	DLOE Actuator
% 7.	DR Actuator
% 8.	DLE Actuator
% 9.	Engine dynamics
% 10.	Vt
% 11.	Alfa
% 12.	Beta
% 13.	P
% 14.	Q
% 15.	R
% 16.	Psi
% 17.	Theta
% 18.	Phi
% 19.	X
% 20.	Y
% 21.	Z
% 22.	Vt sensor
% 23.	Alpha sensor
% 24.	Beta sensor
% 25.	P sensor #1
% 26.	P sensor #2
% 27.	Q sensor #1
% 28.	Q sensor #2
% 29.	R sensor #1
% 30.	R sensor #2
% 31.	Theta sensor #1
% 32.	Theta sensor #2
% 33.	Phi sensor #1
% 34.	Phi sensor #2
% 35.	Altitude sensor
% 36.	Nz sensor #1
% 37.	Nz sensor #2
% 38.	Integrator
% 39.	Phase lag roll
% 40.	Wash-out beta
% 41.	Compensator

% The rest of the states exist only if you use
% phase compensating rate limiters
% 42.	DROE DASA RL TF 2
% 43.	DROE SAAB RL TF 2
% 44.	DROE SAAB RL TF 1
% 45.	DLIE DASA RL TF 2
% 46.	DLIE SAAB RL TF 2
% 47.	DLIE SAAB RL TF 1
% 48.	DLC DASA RL TF 2
% 49.	DLC SAAB RL TF 2
% 50.	DLC SAAB RL TF 1
% 51.	DLE DASA RL TF 2
% 52.	DLE SAAB RL TF 2
% 53.	DLE SAAB RL TF 1
% 54.	DRC DASA RL TF 2
% 55.	DRC SAAB RL TF 2
% 56.	DRC SAAB RL TF 1
% 57.	DLOE DASA RL TF 2
% 58.	DLOE SAAB RL TF 2
% 59.	DLOE SAAB RL TF 1
% 60.	DRIE DASA RL TF 2
% 61.	DRIE SAAB RL TF 2
% 62.	DRIE SAAB RL TF 1
% 63.	DR DASA RL TF 2
% 64.	DR SAAB RL TF 2
% 65.	DR SAAB RL TF 1
% 66.	DLC DASA RL TF 1
% 67.	DLE DASA RL TF 1
% 68.	DLIE DASA RL TF 1
% 69.	DLOE DASA RL TF 1
% 70.	DR DASA RL TF 1
% 71.	DRC DASA RL TF 1
% 72.	DRIE DASA RL TF 1
% 73.	DROE DASA RL TF 1

% Create new 'x0'
x0 = zeros(87,1);
x0(10:21) = x0bare(1:12);

% Assemble trimdata-vector
trimdata( 1: 4) = y0bare(41:44);
trimdata( 5:11) = y0bare(32:38);
trimdata(12:20) = y0bare(32:40);
trimdata(21:30) = y0bare(44+[1:6 8:9 12 19]);

% "Compensator"
num        = [15 10];
den        = [ 1 10];
[A,B,C,D]  = tf2ss(num,den);
x0(41)     = -inv(A)*B*trimdata(1);

% "Phase Lag Roll"
num        = [1];
den        = [0.25 1];
[A,B,C,D]  = tf2ss(num,den);
x0(39)     = -inv(A)*B*trimdata(2);

% "Wash-Out Beta"
num        = [5 0];
den        = [5 1];
[A,B,C,D]  = tf2ss(num,den);
x0(40)     = -inv(A)*B*trimdata(3);

% "Integrator"
num        = [0 1];
den        = [1 0];
[A,B,C,D]  = tf2ss(num,den);
x0(38)     = trimdata(4);
%x0(38)     = 0;

% "DRC DASA RL TF 1"
x0(71) = 0;
% "DRC DASA RL TF 2"
x0(54) = 0;
% "DRC SAAB RL TF 1"
num           = [1];
den           = [saabtc1_c 1];
[A,B,C,D]     = tf2ss(num,den);
x0(56)        = -inv(A)*B*trimdata(12);
% "DRC SAAB RL TF 2"
x0(55) = 0;
% "DRC Actuator"
num       = [1];
den       = [0.05+ddc_bw 1];
[A,B,C,D] = tf2ss(num,den);
x0(1)     = -inv(A)*B*trimdata(12);

% "DLC DASA RL TF 1"
x0(66) = 0;
% "DLC DASA RL TF 2"
x0(48) = 0;
% "DLC SAAB RL TF 1"
num           = [1];
den           = [saabtc1_c 1];
[A,B,C,D]     = tf2ss(num,den);
x0(50)        = -inv(A)*B*trimdata(13);
% "DLC SAAB RL TF 2"
x0(49) = 0;
% "DLC Actuator"
num       = [1];
den       = [0.05+ddc_bw 1];
[A,B,C,D] = tf2ss(num,den);
x0(2)     = -inv(A)*B*trimdata(13);

% "DROE DASA RL TF 1"
x0(73) = 0;
% "DROE DASA RL TF 2"
x0(42) = 0;
% "DROE SAAB RL TF 1"
num           = [1];
den           = [saabtc1_oe 1];
[A,B,C,D]     = tf2ss(num,den);
x0(44)        = -inv(A)*B*trimdata(14);
% "DROE SAAB RL TF 2"
x0(43) = 0;
% "DROE Actuator"
num       = [1];
den       = [0.05+ddey_bw 1];
[A,B,C,D] = tf2ss(num,den);
x0(3)     = -inv(A)*B*trimdata(14);

% "DRIE DASA RL TF 1"
x0(72) = 0;
% "DRIE DASA RL TF 2"
x0(60) = 0;
% "DRIE SAAB RL TF 1"
num           = [1];
den           = [saabtc1_ie 1];
[A,B,C,D]     = tf2ss(num,den);
x0(62)        = -inv(A)*B*trimdata(15);
% "DRIE SAAB RL TF 2"
x0(61) = 0;
% "DRIE Actuator"
num       = [1];
den       = [0.05+ddei_bw 1];
[A,B,C,D] = tf2ss(num,den);
x0(4)     = -inv(A)*B*trimdata(15);

% "DLIE DASA RL TF 1"
x0(68) = 0;
% "DLIE DASA RL TF 2"
x0(45) = 0;
% "DLIE SAAB RL TF 1"
num           = [1];
den           = [saabtc1_ie 1];
[A,B,C,D]     = tf2ss(num,den);
x0(47)        = -inv(A)*B*trimdata(16);
% "DLIE SAAB RL TF 2"
x0(46) = 0;
% "DLIE Actuator"
num       = [1];
den       = [0.05+ddei_bw 1];
[A,B,C,D] = tf2ss(num,den);
x0(5)     = -inv(A)*B*trimdata(16);

% "DLOE DASA RL TF 1"
x0(69) = 0;
% "DLOE DASA RL TF 2"
x0(57) = 0;
% "DLOE SAAB RL TF 1"
num           = [1];
den           = [saabtc1_oe 1];
[A,B,C,D]     = tf2ss(num,den);
x0(59)        = -inv(A)*B*trimdata(17);
% "DLOE SAAB RL TF 2"
x0(58) = 0;
% "DLOE Actuator"
num       = [1];
den       = [0.05+ddey_bw 1];
[A,B,C,D] = tf2ss(num,den);
x0(6)     = -inv(A)*B*trimdata(17);

% "DR DASA RL TF 1"
x0(70) = 0;
% "DR DASA RL TF 2"
x0(63) = 0;
% "DR SAAB RL TF 1"
num           = [1];
den           = [saabtc1_r 1];
[A,B,C,D]     = tf2ss(num,den);
x0(65)        = -inv(A)*B*trimdata(18);
% "DR SAAB RL TF 2"
x0(64) = 0;
% "DR Actuator"
num       = [1];
den       = [0.05+ddr_bw 1];
[A,B,C,D] = tf2ss(num,den);
x0(7)     = -inv(A)*B*trimdata(18);

% "DLE DASA RL TF 1"
x0(67) = 0;
% "DLE DASA RL TF 2"
x0(51) = 0;
% "DLE SAAB RL TF 1"
num           = [1];
den           = [saabtc1_le 1];
[A,B,C,D]     = tf2ss(num,den);
x0(53)        = -inv(A)*B*trimdata(19);
% "DLE SAAB RL TF 2"
x0(52) = 0;
% "DLE Actuator"
num       = [1];
den       = [0.05+ddle_bw 1];
[A,B,C,D] = tf2ss(num,den);
x0(8)     = -inv(A)*B*trimdata(19);

% "Engine dynamics"
num       = [1];
den       = [2 1];
[A,B,C,D] = tf2ss(num,den);
x0(9)     = -inv(A)*B*trimdata(20);

% "Vt sensor"
num        = [1];
den        = [0.02 1];
[A,B,C,D]  = tf2ss(num,den);
x0(22)     = -inv(A)*B*trimdata(21);

% "Alpha sensor"
num        = [1];
den        = [0.02 1];
[A,B,C,D]  = tf2ss(num,den);
x0(23)     = -inv(A)*B*trimdata(22);

% "Beta sensor"
num        = [1];
den        = [0.02 1];
[A,B,C,D]  = tf2ss(num,den);
x0(24)     = -inv(A)*B*trimdata(23);

% "Pb sensor"
num           = [0.0001903 0.005346 1];
den           = [0.0004942 0.030820 1];
[A,B,C,D]     = tf2ss(num,den);
x0(25:26)     = -inv(A)*B*trimdata(24);

% "Qb sensor"
num           = [0.0001903 0.005346 1];
den           = [0.0004942 0.030820 1];
[A,B,C,D]     = tf2ss(num,den);
x0(27:28)     = -inv(A)*B*trimdata(25);

% "Rb sensor"
num           = [0.0001903 0.005346 1];
den           = [0.0004942 0.030820 1];
[A,B,C,D]     = tf2ss(num,den);
x0(29:30)     = -inv(A)*B*trimdata(26);

% "Theta sensor"
num           = [1];
den           = [0.00104 0.0323 1];
[A,B,C,D]     = tf2ss(num,den);
x0(31:32)     = -inv(A)*B*trimdata(27);

% "Phi sensor"
num           = [1];
den           = [0.00104 0.0323 1];
[A,B,C,D]     = tf2ss(num,den);
x0(33:34)     = -inv(A)*B*trimdata(28);

% "Altitude sensor"
num           = [1];
den           = [0.02 1];
[A,B,C,D]     = tf2ss(num,den);
x0(35)        = inv(A)*B*trimdata(29);

% "Nz sensor"
num           = [0.0001903 0.005346 1];
den           = [0.0004942 0.030820 1];
[A,B,C,D]     = tf2ss(num,den);
x0(36:37)     = -inv(A)*B*trimdata(30);

% Cut off the states that are not needed in this version of Admire:
x0 = x0(1:41);