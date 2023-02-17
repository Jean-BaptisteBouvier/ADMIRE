% Plot rt_yout for Admire
close all
figure(1)
subplot(3,1,1)
plot(rt_tout,rt_yout(:,13)),grid
title(['ADMIRE - simulation data from RTW generated program.' ])
ylabel('u [m/s]')
subplot(3,1,2)
plot(rt_tout,rt_yout(:,14)),grid
ylabel('v [m/s]')
subplot(3,1,3)
plot(rt_tout,rt_yout(:,15)),grid
ylabel('w [m/s]')
xlabel('time [s]')

figure(2)
subplot(3,1,1)
plot(rt_tout,rt_yout(:,4)*180/pi),grid
hold on
plot(rt_tout,rt_FCS_lat_stick_p*180/pi,'r:')
title(['ADMIRE - simulation data from RTW generated program.' ])
ylabel('p [deg/s]')
legend('p','p_c_o_m')
subplot(3,1,2)
plot(rt_tout,rt_yout(:,5)*180/pi),grid
hold on
plot(rt_tout,rt_qVt_stick_q_com*180/pi,'g');
plot(rt_tout,rt_q_com_lim*180/pi,'r:')
ylabel('q [deg/s]')
legend('q','q_c_o_m','after alpha limiter')
subplot(3,1,3)
plot(rt_tout,rt_yout(:,6)*180/pi),grid
ylabel('r [deg/s]')
xlabel('time [s]')

figure(3)
subplot(3,1,1)
plot(rt_tout,rt_yout(:,7)*180/pi),grid
title(['ADMIRE - simulation data from RTW generated program.' ])
ylabel('psi [deg]')
subplot(3,1,2)
plot(rt_tout,rt_yout(:,8)*180/pi),grid
ylabel('theta [deg]')
subplot(3,1,3)
plot(rt_tout,rt_yout(:,9)*180/pi),grid
ylabel('phi [deg]')
xlabel('time [s]')

figure(4)
subplot(3,1,1)
plot(rt_tout,rt_yout(:,10)),grid
title(['ADMIRE - simulation data from RTW generated program.' ])
ylabel('x [m]')
subplot(3,1,2)
plot(rt_tout,rt_yout(:,11)),grid
ylabel('y [m]')
subplot(3,1,3)
plot(rt_tout,rt_yout(:,12)),grid
ylabel('z [m]')
xlabel('time [s]')

figure(5)
subplot(3,1,1)
plot(rt_tout,rt_yout(:,1)),grid
title(['ADMIRE - simulation data from RTW generated program.' ])
ylabel('Vt [m/s]')
subplot(3,1,2)
plot(rt_tout,rt_yout(:,2)*180/pi),grid
ylabel('alpha [deg]')
subplot(3,1,3)
plot(rt_tout,rt_yout(:,3)*180/pi),grid
hold on
plot(rt_tout,rt_FCS_lat_ped_beta*180/pi,'r:')
ylabel('beta [deg]')
legend('beta','beta_c_o_m')
xlabel('time [s]')

figure(6)
subplot(3,1,1)
plot(rt_tout,rt_yout(:,19)),grid
hold on
plot(rt_tout,rt_FCS_long_Nz_stick_Nz,'r:')
title(['ADMIRE - simulation data from RTW generated program.' ])
ylabel('nz [-]')
legend('nz','nz_c_o_m')
subplot(3,1,2)
plot(rt_tout,rt_yout(:,20)),grid
ylabel('ny [-]')
subplot(3,1,3)
plot(rt_tout,rt_yout(:,21)),grid
ylabel('mach [-]')
xlabel('time [s]')

figure(7)
subplot(3,1,1)
plot(rt_tout,rt_yout(:,22)*180/pi),grid on
title(['ADMIRE - simulation data from RTW generated program.' ])
ylabel('gamma [deg]')

subplot(3,1,2)
plot(rt_tout,rt_FCS_drc*180/pi,'g'),grid on
hold on
ylabel('canard deflection (deg)')
plot(rt_tout,rt_FCS_dlc*180/pi,'r')
legend('Right','Left')

subplot(3,1,3)
plot(rt_tout,rt_FCS_droe*180/pi,'g'),hold on
ylabel('Elevon deflection')
plot(rt_tout,rt_FCS_drie*180/pi,'c'),grid on
plot(rt_tout,rt_FCS_dlie*180/pi,'m')
plot(rt_tout,rt_FCS_dloe*180/pi,'r')
legend('Outer right','Inner right','Inner left','Outer left');
xlabel('time [s]')

figure(8)
subplot(3,1,1)
plot(rt_tout,rt_FCS_dle*180/pi),grid on
title(['ADMIRE - simulation data from RTW generated program.' ])
ylabel('Le deflection [deg]')

subplot(3,1,2)
plot(rt_tout,rt_FCS_dr*180/pi),grid on
hold on
ylabel('Rudder deflection (deg)')
plot(rt_tout,rt_FCS_dr*180/pi)

subplot(3,1,3)
plot(rt_tout,rt_FCS_tss),hold on
ylabel('Thrust setting')
grid
xlabel('time [s]')

figure
subplot(3,1,1)
plot(rt_tout,rt_dlc_l*180/pi,'g');
hold on
grid
ylabel('Canard actuator (left) [deg].');
xlabel('Time [s]');
plot(rt_tout,rt_FCS_dlc*180/pi,'r');
legend('Input','Output');

subplot(3,1,2)
plot(rt_tout,rt_dloe_l*180/pi,'g');
hold on
grid
ylabel('Left elevon actuator (outer) [deg].');
xlabel('Time [s]');
plot(rt_tout,rt_FCS_dloe*180/pi,'r');
legend('Input','Output');

subplot(3,1,3)
plot(rt_tout,rt_droe_l*180/pi,'g');
hold on
grid
ylabel('Right elevon actuator (outer) [deg].');
xlabel('Time [s]');
plot(rt_tout,rt_FCS_droe*180/pi,'r');
legend('Input','Output');

