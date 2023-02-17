% Plot yout_lin for Admire
close all
figure(1)
subplot(3,1,1)
plot(tout_lin,yout_lin(:,13)),grid
hold on
plot(tout_lin,feedback(:,13),'r'),grid
no = 'linearised version';
title(['ADMIRE - ' no '. (' date ').' ])
ylabel('u [m/s]')
subplot(3,1,2)
plot(tout_lin,yout_lin(:,14)),grid
hold on
plot(tout_lin,feedback(:,14),'r'),grid
ylabel('v [m/s]')
subplot(3,1,3)
plot(tout_lin,yout_lin(:,15)),grid
hold on
plot(tout_lin,feedback(:,15),'r'),grid
ylabel('w [m/s]')
xlabel('time [s]')

figure(2)
subplot(3,1,1)
plot(tout_lin,yout_lin(:,4)*180/pi),grid
hold on
plot(tout_lin,feedback(:,4)*180/pi,'r'),grid
% plot(tout,FCS_lat_stick_p*180/pi,'r:') % Not available in linear model
title(['ADMIRE - ' no '. (' date ').' ])
ylabel('p [deg/s]')
legend('p','p_c_o_m')
subplot(3,1,2)
plot(tout_lin,yout_lin(:,5)*180/pi),grid
hold on
plot(tout_lin,feedback(:,5)*180/pi,'r'),grid
%plot(tout,q_com_lim*180/pi,'r:') % Not available in linear model
ylabel('q [deg/s]')
legend('q','q_c_o_m')
subplot(3,1,3)
plot(tout_lin,yout_lin(:,6)*180/pi),grid
hold on
plot(tout_lin,feedback(:,6)*180/pi,'r'),grid
ylabel('r [deg/s]')
xlabel('time [s]')

figure(3)
subplot(3,1,1)
plot(tout_lin,yout_lin(:,7)*180/pi),grid
hold on
plot(tout_lin,feedback(:,7)*180/pi,'r'),grid
title(['ADMIRE - ' no '. (' date ').' ])
ylabel('psi [deg]')
subplot(3,1,2)
plot(tout_lin,yout_lin(:,8)*180/pi),grid
hold on
plot(tout_lin,feedback(:,8)*180/pi,'r'),grid
ylabel('theta [deg]')
subplot(3,1,3)
plot(tout_lin,yout_lin(:,9)*180/pi),grid
hold on
plot(tout_lin,feedback(:,9)*180/pi,'r'),grid
ylabel('phi [deg]')
xlabel('time [s]')

figure(4)
subplot(3,1,1)
plot(tout_lin,yout_lin(:,10)),grid
hold on
plot(tout_lin,feedback(:,10),'r'),grid
title(['ADMIRE - ' no '. (' date ').' ])
ylabel('x [m]')
subplot(3,1,2)
plot(tout_lin,yout_lin(:,11)),grid
hold on
plot(tout_lin,feedback(:,11),'r'),grid
ylabel('y [m]')
subplot(3,1,3)
plot(tout_lin,yout_lin(:,12)),grid
hold on
plot(tout_lin,feedback(:,12),'r'),grid
ylabel('z [m]')
xlabel('time [s]')

figure(5)
subplot(3,1,1)
plot(tout_lin,yout_lin(:,1)),grid
hold on
plot(tout_lin,feedback(:,1),'r'),grid
title(['ADMIRE - ' no '. (' date ').' ])
ylabel('Vt [m/s]')
subplot(3,1,2)
plot(tout_lin,yout_lin(:,2)*180/pi),grid
hold on
plot(tout_lin,feedback(:,2)*180/pi,'r'),grid
ylabel('alpha [deg]')
subplot(3,1,3)
plot(tout_lin,yout_lin(:,3)*180/pi),grid
hold on
plot(tout_lin,feedback(:,3)*180/pi,'r'),grid
%plot(tout,FCS_lat_ped_beta*180/pi,'r:') % Not available in linear model
ylabel('beta [deg]')
legend('beta','beta_c_o_m')
xlabel('time [s]')

figure(6)
subplot(3,1,1)
plot(tout_lin,yout_lin(:,19)),grid
hold on
plot(tout_lin,feedback(:,19),'r')
%plot(tout,FCS_long_Nz_stick_Nz,'r:') % Not available in linear model
title(['ADMIRE - ' no '. (' date ').' ])
ylabel('nz [-]')
legend('nz','nz_c_o_m')
subplot(3,1,2)
plot(tout_lin,yout_lin(:,20)),grid
hold on
plot(tout_lin,feedback(:,20),'r')
ylabel('ny [-]')
subplot(3,1,3)
plot(tout_lin,yout_lin(:,21)),grid
hold on
plot(tout_lin,feedback(:,21),'r')
ylabel('mach [-]')
xlabel('time [s]')

figure(7)
subplot(3,1,1)
plot(tout_lin,yout_lin(:,22)*180/pi),grid on
hold on
plot(tout_lin,feedback(:,22)*180/pi,'r')
title(['ADMIRE - ' no '. (' date ').' ])
ylabel('gamma [deg]')
subplot(3,1,2)
%plot(tout_lin,FCS_Fes_in),grid on 
ylabel('fes [N]')
hold on
%plot(tout,fes_in,'r:')
legend('dFes',0)
subplot(3,1,3)
%plot(tout,qVt_tss),hold on
ylabel('qVt_tss [0..1]')
xlabel('time [s]')
grid

figure(8)
subplot(3,1,1)
plot(tout_lin,uout_lin(:,1)*180/pi,'g'),grid on
hold on
ylabel('Canard deflection [deg]');
plot(tout_lin,FCS_drc*180/pi,'r');
legend('From FCS','Servo position');

subplot(3,1,2)
plot(tout_lin,uout_lin(:,3)*180/pi,'g'),grid on
hold on
ylabel('Elevon deflection [deg]');
plot(tout_lin,FCS_droe*180/pi,'r');
legend('From FCS','Servo position');

