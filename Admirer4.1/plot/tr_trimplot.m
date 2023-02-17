% Plot aircraft outputs and states

H = figure(1);
set(H,'NumberTitle','off','Name','Params #1');
subplot(3,2,1);
plot(tr_tout,tr_yout(:,1));
title('Parameter: Vt [m/s]');
grid on;

subplot(3,2,2);
plot(tr_tout,tr_yout(:,2)*180/pi);
title('Parameter: Alpha [deg]');
grid on;

subplot(3,2,3);
plot(tr_tout,tr_yout(:,3)*180/pi);
title('Parameter: Beta [deg]');
grid on;

subplot(3,2,4);
plot(tr_tout,tr_yout(:,4)*180/pi);
title('Parameter: P [deg/s]');
grid on;

subplot(3,2,5);
plot(tr_tout,tr_yout(:,5)*180/pi);
title('Parameter: Q [deg/s]');
grid on;

subplot(3,2,6);
plot(tr_tout,tr_yout(:,6)*180/pi);
title('Parameter: R [deg/s]');
grid on;

H = figure(2);
set(H,'NumberTitle','off','Name','Params #2');
subplot(3,2,1);
plot(tr_tout,tr_yout(:,7)*180/pi);
title('Parameter: Psi [deg]');
grid on;

subplot(3,2,2);
plot(tr_tout,tr_yout(:,8)*180/pi);
title('Parameter: Theta [deg]');
grid on;

subplot(3,2,3);
plot(tr_tout,tr_yout(:,9)*180/pi);
title('Parameter: Phi [deg]');
grid on;

subplot(3,2,4);
plot(tr_tout,tr_yout(:,19),tr_tout,1.*cos(tr_yout(:,8)),'r');
title('Parameter: Nz [-]');
grid on;

subplot(3,2,5);
plot(tr_tout,tr_yout(:,21));
title('Parameter: Mach [-]');
grid on;

subplot(3,2,6);
plot(tr_tout,tr_yout(:,22)*180/pi);
title('Parameter: Gamma [deg]');
grid on;

H = figure(3);
set(H,'NumberTitle','off','Name','Params #3');
subplot(3,2,1);
plot(tr_tout,tr_yout(:,32)*180/pi);
title('Parameter: drc [deg]');
grid on;

subplot(3,2,2);
plot(tr_tout,tr_yout(:,33)*180/pi);
title('Parameter: dlc [deg]');
grid on;

subplot(3,2,3);
plot(tr_tout,tr_yout(:,34)*180/pi);
title('Parameter: droe [deg]');
grid on;

subplot(3,2,4);
plot(tr_tout,tr_yout(:,35)*180/pi);
title('Parameter: drie [deg]');
grid on;

subplot(3,2,5);
plot(tr_tout,tr_yout(:,36)*180/pi);
title('Parameter: dlie [deg]');
grid on;

subplot(3,2,6);
plot(tr_tout,tr_yout(:,37)*180/pi);
title('Parameter: dloe [deg]');
grid on;

H = figure(4);
set(H,'NumberTitle','off','Name','Params #4');
subplot(3,2,1);
plot(tr_tout,tr_yout(:,26));
title('Parameter: Cl [-]');
grid on;

subplot(3,2,2);
plot(tr_tout,tr_yout(:,27));
title('Parameter: Cm [-]');
grid on;

subplot(3,2,3);
plot(tr_tout,tr_yout(:,28));
title('Parameter: Cn [-]');
grid on;

subplot(3,2,4);
plot(tr_tout,tr_yout(:,29));
title('Parameter: Fx [N]');
grid on;

subplot(3,2,5);
plot(tr_tout,tr_yout(:,30));
title('Parameter: Fz [N]');
grid on;

subplot(3,2,6);
plot(tr_tout,tr_yout(:,31));
title('Parameter: My [Nm]');
grid on;