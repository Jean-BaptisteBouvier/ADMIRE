close all
for i = 1:16,
  figure
  plot(tout,uout(:,i)) 
  hold on
  plot(tout_lin,uout_lin(:,i),'r')
end