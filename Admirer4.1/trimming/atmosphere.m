function a = atmosphere(H)

% Function that calculates speed of sound for any
% given altitude H.

R = 287.05287; 					% Universal Gas constant; Unit [m^2/s^2/K].
g = 9.80665;   					% Gravitation; Unit [m/s^2].
Kappa = 1.4;						% Specific Heat constant

if H <= 11000
	T01    = 288.15;				% Reference temperature for H=0; Unit [K].
	Tgrad1 = 0.0065; 				% Temperature Gradient; Unit [K/m].
	T1     = T01-Tgrad1*H;		% Actual temperature for altitude H; Unit [K].
	a      = sqrt(Kappa*R*T1);	% Speed of Sound for altitude H; Unit [m/s].
elseif H > 11000					% Iso-thermal for  11000<H<25000 [m].
	T02    = 216.65;				% Reference temperature for H=11000; Unit [K].
	Tgrad2 = 0; 					% Temperature gradient; Unit [K/m].
	T2     = T02-Tgrad2*H;		% Actual temperature for altitude H; Unit [K].
	a      = sqrt(Kappa*R*T2);	% Speed of Sound for altitude H; Unit [m/s].
end