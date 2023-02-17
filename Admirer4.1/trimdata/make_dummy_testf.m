% Produce a dummy testf.mat
T = [0 10 10.3 12 12.3 15 15.3 17 17.3 22 22.3 25 25.3 30 30.3 32 32.3 35 35.3 37 37.3 60];
U = [0 0  15   15 -10  -10 0   0  -20 -20 20   20 0     0 30   30 -30  -30 30  30  0    0];

T_pitch = [0  7 8 20 22 25 26 30 30.4 35 35.3 40 40.3 44 45.3 60];
U_pitch = [0  0 3 3  -3 -3 5  5  -2   -2 2     2  -4   -4 0   0];

T = T';
U = U';
T_pitch = T_pitch';
U_pitch = U_pitch';

save dummy_testf T U T_pitch U_pitch;

figure;
plot(T,U,'r',T_pitch,U_pitch,'b');
