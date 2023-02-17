% Plot yout for Admire
close all
load admire_augmented_no_dtz_1Hz;
yout_n = yout;
time_n = time;
load admire_augmented_1Hz;
figure(1)
subplot(3,1,1)
plot(time_n,yout_n(:,13),'g')
hold on;
plot(time,yout(:,13)),grid
title(['ADMIRE - /admireR3.0/. Flight case ' no '. (' date ').' ])
ylabel('u [m/s]')
subplot(3,1,2)
plot(time_n,yout_n(:,14),'g')
hold on;
plot(time,yout(:,14)),grid
ylabel('v [m/s]')
subplot(3,1,3)
plot(time_n,yout_n(:,15),'g')
hold on;
plot(time,yout(:,15)),grid
ylabel('w [m/s]')
xlabel('time [s]')

figure(2)
subplot(3,1,1)
plot(time_n,yout_n(:,4)*180/pi,'g')
hold on
plot(time,yout(:,4)*180/pi),grid
hold on
plot(time,FCS_lat_stick_p*180/pi,'r:')
title(['ADMIRE - /admireR3.0/. Flight case ' no '. (' date ').' ])
ylabel('p [deg/s]')
legend('p','p_c_o_m')
subplot(3,1,2)
plot(time_n,yout_n(:,5)*180/pi,'g')
hold on
plot(time,yout(:,5)*180/pi),grid
hold on
plot(time,q_com_lim*180/pi,'r:')
ylabel('q [deg/s]')
legend('q','q_c_o_m')
subplot(3,1,3)
plot(time_n,yout_n(:,6)*180/pi,'g')
hold on
plot(time,yout(:,6)*180/pi),grid
ylabel('r [deg/s]')
xlabel('time [s]')

figure(3)
subplot(3,1,1)
plot(time_n,yout_n(:,7)*180/pi,'g')
hold on
plot(time,yout(:,7)*180/pi),grid
title(['ADMIRE - /admireR3.0/. Flight case ' no '. (' date ').' ])
ylabel('psi [deg]')
subplot(3,1,2)
plot(time_n,yout_n(:,8)*180/pi,'g')
hold on
plot(time,yout(:,8)*180/pi),grid
ylabel('theta [deg]')
subplot(3,1,3)
plot(time_n,yout_n(:,9)*180/pi,'g')
hold on
plot(time,yout(:,9)*180/pi),grid
ylabel('phi [deg]')
xlabel('time [s]')

figure(4)
subplot(3,1,1)
plot(time,yout(:,10)),grid
title(['ADMIRE - /admireR3.0/. Flight case ' no '. (' date ').' ])
ylabel('x [m]')
subplot(3,1,2)
plot(time,yout(:,11)),grid
ylabel('y [m]')
subplot(3,1,3)
plot(time,yout(:,12)),grid
ylabel('z [m]')
xlabel('time [s]')

figure(5)
subplot(3,1,1)
plot(time,yout(:,1)),grid
title(['ADMIRE - /admireR3.0/. Flight case ' no '. (' date ').' ])
ylabel('Vt [m/s]')
subplot(3,1,2)
plot(time_n,yout_n(:,2)*180/pi,'g')
hold on
plot(time,yout(:,2)*180/pi),grid
ylabel('alpha [deg]')
subplot(3,1,3)
plot(time_n,yout_n(:,3)*180/pi,'g')
hold on
plot(time,yout(:,3)*180/pi),grid
hold on
plot(time,FCS_lat_ped_beta*180/pi,'r:')
ylabel('beta [deg]')
legend('beta','beta_c_o_m')
xlabel('time [s]')

figure(6)
subplot(3,1,1)
plot(time,yout(:,19)),grid
hold on
plot(time,FCS_long_Nz_stick_Nz,'r:')
title(['ADMIRE - /admireR3.0/. Flight case ' no '. (' date ').' ])
ylabel('nz [-]')
legend('nz','nz_c_o_m')
subplot(3,1,2)
plot(time,yout(:,20)),grid
ylabel('ny [-]')
subplot(3,1,3)
plot(time,yout(:,21)),grid
ylabel('mach [-]')
xlabel('time [s]')

figure(7)
subplot(3,1,1)
plot(time_n,yout_n(:,22)*180/pi,'g')
hold on
plot(time,yout(:,22)*180/pi),grid on
title(['ADMIRE - /admireR3.0/. Flight case ' no '. (' date ').' ])
ylabel('gamma [deg]')
subplot(3,1,2)
plot(time,fes_newton),grid on 
ylabel('fes [N]')
hold on
plot(time,fes_in,'r:')
legend('Fes','Fes_i_n',0)
subplot(3,1,3)
%plot(time,qVt_tss),hold on
ylabel('qVt_tss [0..1]')
xlabel('time [s]')

grid