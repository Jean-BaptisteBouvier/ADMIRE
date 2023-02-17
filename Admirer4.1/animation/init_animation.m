%   Initiation program for animation_sim.
%   Plots all the variables from a FENIX run.
%   If used as stand-alone program, only the 
%   plots will be drawn without animation.
%   Program by Anton Vooren, Royal Norwegian Air Force

if exist('yout')
    ok=questdlg('Want to use data already in memory?','Data from file or memory','Yes','No','Yes');
    if strcmp(ok,'No')
        try
            [fname,sv]=uigetfile('*.mat','Load Fenix output file');
            now=cd;
            cd(sv);
            eval(['load ' fname]);
            cd(now);
        catch
            return
        end
    else
        rt_yout=yout;
        rt_tout=tout;
    end
else
    try
        [fname,sv]=uigetfile('*.mat','Load Fenix output file');
        now=cd;
        cd(sv);
        eval(['load ' fname]);
        cd(now);
    catch
        return
    end 
end

% When less points from FENIX are wanted (in case of very big files), 
% div can be sett to a number higher than 1

div=1;

all=length(rt_yout);
euler_input=rt_yout(1:div:all,[9 8 7]);     % Extract euler angles in right order: roll, pitch and yaw
pos_input=rt_yout(1:div:all,[10 11 12]);    % Extract x, y and z
rt_tout=rt_tout(1:div:all);

figure(1)
name='Velocity, angle of attack and sideslip';
set(gcf,'PaperUnits','inches','papertype','A4','PaperOrientation','landscape',...
   'PaperPositionMode','manual','paperposition',[0.25 0.25 11.193 7.7677],...
   'NumberTitle','off','name',name);    %'paperposition',[0.25 0.25 11.193 7.7677]
subplot(3,1,1)
plot(rt_tout,rt_yout(1:div:all,1))
ylabel(['V_t',sprintf('\n'),'m/s'],'rotation',[0])
axis tight
grid on
subplot(3,1,2)
plot(rt_tout,rt_yout(1:div:all,2)*180/pi)
ylabel(['\alpha',sprintf('\n'),'degrees'],'rotation',[0])
axis tight
grid on
subplot(3,1,3)
plot(rt_tout,rt_yout(1:div:all,3)*180/pi)
ylabel(['\beta',sprintf('\n'),'degrees'],'rotation',[0])
xlabel('Time / s')
axis tight
grid on

figure(2)
name='Angular speed';
set(gcf,'PaperUnits','inches','papertype','A4','PaperOrientation','landscape',...
   'PaperPositionMode','manual','paperposition',[0.25 0.25 11.193 7.7677],...
   'NumberTitle','off','name',name);    %'paperposition',[0.25 0.25 11.193 7.7677]
subplot(3,1,1)
plot(rt_tout,rt_yout(1:div:all,4))
ylabel(['p_b'],'rotation',[0])
axis tight
grid on
subplot(3,1,2)
plot(rt_tout,rt_yout(1:div:all,5))
ylabel(['q_b'],'rotation',[0])
axis tight
grid on
subplot(3,1,3)
plot(rt_tout,rt_yout(1:div:all,6))
ylabel(['r_b'],'rotation',[0])
xlabel('Time / s')
axis tight
grid on

figure(3)
name='Euler angles';
set(gcf,'PaperUnits','inches','papertype','A4','PaperOrientation','landscape',...
   'PaperPositionMode','manual','paperposition',[0.25 0.25 11.193 7.7677],...
   'NumberTitle','off','name',name);    %'paperposition',[0.25 0.25 11.193 7.7677]
subplot(3,1,1)
plot(rt_tout,rt_yout(1:div:all,7)*180/pi)
ylabel(['\psi',sprintf('\n'),'degrees'],'rotation',[0])
axis tight
grid on
subplot(3,1,2)
plot(rt_tout,rt_yout(1:div:all,8)*180/pi)
ylabel(['\theta',sprintf('\n'),'degrees'],'rotation',[0])
axis tight
grid on
subplot(3,1,3)
plot(rt_tout,rt_yout(1:div:all,9)*180/pi)
ylabel(['\phi',sprintf('\n'),'degrees'],'rotation',[0])
xlabel('Time / s')
axis tight
grid on

figure(4)
name='Aerodynamic forces';
set(gcf,'PaperUnits','inches','papertype','A4','PaperOrientation','landscape',...
   'PaperPositionMode','manual','paperposition',[0.25 0.25 11.193 7.7677],...
   'NumberTitle','off','name',name);    %'paperposition',[0.25 0.25 11.193 7.7677]
subplot(3,1,1)
plot(rt_tout,rt_yout(1:div:all,24))
ylabel(['C_L'],'rotation',[0])
axis tight
grid on
subplot(3,1,2)
plot(rt_tout,rt_yout(1:div:all,23))
ylabel(['C_D'],'rotation',[0])
axis tight
grid on
subplot(3,1,3)
plot(rt_tout,rt_yout(1:div:all,25))
ylabel(['C_C'],'rotation',[0])
xlabel('Time / s')
axis tight
grid on

figure(5)
name='Aerodynamic moments';
set(gcf,'PaperUnits','inches','papertype','A4','PaperOrientation','landscape',...
   'PaperPositionMode','manual','paperposition',[0.25 0.25 11.193 7.7677],...
   'NumberTitle','off','name',name);    %'paperposition',[0.25 0.25 11.193 7.7677]
subplot(3,1,1)
plot(rt_tout,rt_yout(1:div:all,27))
ylabel(['C_m'],'rotation',[0])
axis tight
grid on
subplot(3,1,2)
plot(rt_tout,rt_yout(1:div:all,26))
ylabel(['C_l'],'rotation',[0])
axis tight
grid on
subplot(3,1,3)
plot(rt_tout,rt_yout(1:div:all,28))
ylabel(['C_n'],'rotation',[0])
xlabel('Time / s')
axis tight
grid on

material metal
%sim('animation_sim',[0,rt_tout(all)]);
