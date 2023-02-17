% An example to show how to plot Cm as a function of AoA
sim('aerotesthigh');
plot(uin(:,14)*180/pi,yout(:,5))

