% Plot yout and yout_lin for Admire, for comparison
close all
figure(1)
subplot(3,1,1)
plot(tout,yout(:,13))
hold on
plot(tout_lin,yout_lin(:,13),'r'),grid
no = 'nonlinear and linear compared';
title(['ADMIRE - ' no '. (' date ').' ])
ylabel('u [m/s]')
subplot(3,1,2)
plot(tout,yout(:,14))
hold on
plot(tout_lin,yout_lin(:,14),'r'),grid
ylabel('v [m/s]')
subplot(3,1,3)
plot(tout,yout(:,15))
hold on
plot(tout_lin,yout_lin(:,15),'r'),grid
ylabel('w [m/s]')
xlabel('time [s]')

figure(2)
subplot(3,1,1)
plot(tout,yout(:,4)*180/pi)
hold on
plot(tout_lin,yout_lin(:,4)*180/pi,'r'),grid
% plot(tout_lin,FCS_lat_stick_p*180/pi,'r:') % Not available in linear model
title(['ADMIRE - ' no '. (' date ').' ])
ylabel('p [deg/s]')
legend('p','p_l_i_n')
subplot(3,1,2)
plot(tout,yout(:,5)*180/pi)
hold on
plot(tout_lin,yout_lin(:,5)*180/pi,'r'),grid
hold on
%plot(tout_lin,q_com_lim*180/pi,'r:') % Not available in linear model
ylabel('q [deg/s]')
legend('q','q_l_i_n')
subplot(3,1,3)
plot(tout,yout(:,6)*180/pi)
hold on
plot(tout_lin,yout_lin(:,6)*180/pi,'r'),grid
ylabel('r [deg/s]')
xlabel('time [s]')

figure(3)
subplot(3,1,1)
plot(tout,yout(:,7)*180/pi)
hold on
plot(tout_lin,yout_lin(:,7)*180/pi,'r'),grid
title(['ADMIRE - ' no '. (' date ').' ])
ylabel('psi [deg]')
subplot(3,1,2)
plot(tout,yout(:,8)*180/pi)
hold on
plot(tout_lin,yout_lin(:,8)*180/pi,'r'),grid
ylabel('theta [deg]')
subplot(3,1,3)
plot(tout,yout(:,9)*180/pi)
hold on
plot(tout_lin,yout_lin(:,9)*180/pi,'r'),grid
ylabel('phi [deg]')
xlabel('time [s]')

figure(4)
subplot(3,1,1)
plot(tout,yout(:,10))
hold on
plot(tout_lin,yout_lin(:,10),'r'),grid
title(['ADMIRE - ' no '. (' date ').' ])
ylabel('x [m]')
subplot(3,1,2)
plot(tout,yout(:,11))
hold on
plot(tout_lin,yout_lin(:,11),'r'),grid
ylabel('y [m]')
subplot(3,1,3)
plot(tout,yout(:,12))
hold on
plot(tout_lin,yout_lin(:,12),'r'),grid
ylabel('z [m]')
xlabel('time [s]')

figure(5)
subplot(3,1,1)
plot(tout,yout(:,1))
hold on
plot(tout_lin,yout_lin(:,1),'r'),grid
title(['ADMIRE - ' no '. (' date ').' ])
ylabel('Vt [m/s]')
subplot(3,1,2)
plot(tout,yout(:,2)*180/pi)
hold on
plot(tout_lin,yout_lin(:,2)*180/pi,'r'),grid
ylabel('alpha [deg]')
subplot(3,1,3)
plot(tout,yout(:,3)*180/pi)
hold on
plot(tout_lin,yout_lin(:,3)*180/pi,'r'),grid
%plot(tout_lin,FCS_lat_ped_beta*180/pi,'r:') % Not available in linear model
ylabel('beta [deg]')
legend('beta','beta_l_i_n')
xlabel('time [s]')

figure(6)
subplot(3,1,1)
plot(tout,yout(:,19))
hold on
plot(tout_lin,yout_lin(:,19),'r'),grid
%plot(tout_lin,FCS_long_Nz_stick_Nz,'r:') % Not available in linear model
title(['ADMIRE - ' no '. (' date ').' ])
ylabel('nz [-]')
legend('nz','nz_l_i_n')
subplot(3,1,2)
plot(tout,yout(:,20))
hold on
plot(tout_lin,yout_lin(:,20),'r'),grid
ylabel('ny [-]')
subplot(3,1,3)
plot(tout,yout(:,21))
hold on
plot(tout_lin,yout_lin(:,21),'r'),grid
ylabel('mach [-]')
xlabel('time [s]')

figure(7)
subplot(3,1,1)
plot(tout,yout(:,22)*180/pi)
hold on
plot(tout_lin,yout_lin(:,22)*180/pi,'r'),grid on
title(['ADMIRE - ' no '. (' date ').' ])
ylabel('gamma [deg]')
subplot(3,1,2)
%plot(tout_lin,dFes),grid on 
ylabel('fes [N]')
hold on
%plot(tout_lin,fes_in,'r:')
legend('dFes',0)
subplot(3,1,3)
%plot(tout_lin,dFas),hold on
ylabel('dFas')
xlabel('time [s]')

grid
figure
plot3(yout(:,10),yout(:,11),yout(:,12));
xlabel('x');
ylabel('y');
zlabel('z');