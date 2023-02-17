% Plot aircraft outputs and states

H = figure(1);
set(H,'NumberTitle','off','Name','Params #1');
subplot(3,2,1);
plot(tout,yout(:,1));
title('Parameter: Vt [m/s]');
grid on;

subplot(3,2,2);
plot(tout,yout(:,2)*180/pi);
title('Parameter: Alpha [deg]');
grid on;

subplot(3,2,3);
plot(tout,yout(:,3)*180/pi);
title('Parameter: Beta [deg]');
grid on;

subplot(3,2,4);
plot(tout,yout(:,4)*180/pi);
title('Parameter: P [deg/s]');
grid on;

subplot(3,2,5);
plot(tout,yout(:,5)*180/pi);
title('Parameter: Q [deg/s]');
grid on;

subplot(3,2,6);
plot(tout,yout(:,6)*180/pi);
title('Parameter: R [deg/s]');
grid on;

H = figure(2);
set(H,'NumberTitle','off','Name','Params #2');
subplot(3,2,1);
plot(tout,yout(:,7)*180/pi);
title('Parameter: Psi [deg]');
grid on;

subplot(3,2,2);
plot(tout,yout(:,8)*180/pi);
title('Parameter: Theta [deg]');
grid on;

subplot(3,2,3);
plot(tout,yout(:,9)*180/pi);
title('Parameter: Phi [deg]');
grid on;

subplot(3,2,4);
plot(tout,yout(:,19),tout,1.*cos(yout(:,8)),'r');
title('Parameter: Nz [-]');
grid on;

subplot(3,2,5);
plot(tout,yout(:,21));
title('Parameter: Mach [-]');
grid on;

subplot(3,2,6);
plot(tout,yout(:,22)*180/pi);
title('Parameter: Gamma [deg]');
grid on;

H = figure(3);
set(H,'NumberTitle','off','Name','Params #3');
subplot(3,2,1);
plot(tout,FCS_drc*180/pi);
title('Parameter: drc [deg]');
grid on;

subplot(3,2,2);
plot(tout,FCS_dlc*180/pi);
title('Parameter: dlc [deg]');
grid on;

subplot(3,2,3);
plot(tout,FCS_droe*180/pi);
title('Parameter: droe [deg]');
grid on;

subplot(3,2,4);
plot(tout,FCS_drie*180/pi);
title('Parameter: drie [deg]');
grid on;

subplot(3,2,5);
plot(tout,FCS_dlie*180/pi);
title('Parameter: dlie [deg]');
grid on;

subplot(3,2,6);
plot(tout,FCS_tss);
title('Parameter: tss');
grid on;

H = figure(4);
set(H,'NumberTitle','off','Name','Params #4');
subplot(3,2,1);
plot(tout,yout(:,26));
title('Parameter: Cl [-]');
grid on;

subplot(3,2,2);
plot(tout,yout(:,27));
title('Parameter: Cm [-]');
grid on;

subplot(3,2,3);
plot(tout,yout(:,28));
title('Parameter: Cn [-]');
grid on;

subplot(3,2,4);
plot(tout,yout(:,29));
title('Parameter: Fx [N]');
grid on;

subplot(3,2,5);
plot(tout,yout(:,30));
title('Parameter: Fz [N]');
grid on;

subplot(3,2,6);
plot(tout,yout(:,31));
title('Parameter: My [Nm]');
grid on;

