% Check how much an altitude error affects the gains in the control system.
% One altitude at a time on Mach No. at a time.

% ************** q control
ki = [ -4.7841 -2.4298 -1.3177 -0.9684 -0.6586 
 -2.7152 -2.7152 -1.7509 -1.1463 -0.6617 
 -1.6027 -1.6027 -1.6027 -1.6027 -0.6568 ];

 kp = [ -1.5000 -0.7000 -0.4000 -0.3000 -0.2000 
 -0.9000 -0.9000 -0.6000 -0.4000 -0.2000 
 -0.5000 -0.5000 -0.5000 -0.5000 -0.3000 ];

 kalfa = [ 0.4321 0.2443 0.6388 0.3875 0.1274 
 0.4630 0.4630 0.6000 0.4436 0.1615 
 0.4574 0.4574 0.4574 0.4574 0.1861 ];

 kq = [ 1.5729 0.6554 0.3646 0.2407 0.1705 
 0.8810 0.8810 0.5235 0.3186 0.1538 
 0.4693 0.4693 0.4693 0.4693 0.1631 ];

 kvt = [ 0.0500 0.0500 0.0500 0.0500 0.0500 
 0.0500 0.0500 0.0500 0.0500 0.0500 
 0.0500 0.0500 0.0500 0.0500 0.0500 ];

%*************** Nz

 nz_alpha = [ 1.5000 0.7000 0.8000 0.3000 0.3000 0.1000 0.1400 0.1100 
 1.6000 0.9000 0.5500 0.3300 0.1400 0.1400 0.1000 0.0800 
 2.2000 1.1000 0.7000 0.3000 0.0500 0.0500 0.0500 0.0500 ];

 nz_i = [ -0.0700 -0.0318 -0.0182 -0.0350 -0.0350 -0.0300 -0.0500 -0.0500 
 -0.0800 -0.0410 -0.0250 -0.0385 -0.0500 -0.0500 -0.0500 -0.0500 
 -0.1000 -0.0501 -0.0318 -0.0400 -0.0600 -0.0600 -0.0600 -0.0600 ];

 nz_p = [ -0.0045 -0.0048 -0.0027 -0.0010 -0.0010 -0.0005 -0.0005 -0.0005 
 -0.0090 -0.0061 -0.0037 -0.0011 -0.0005 -0.0005 -0.0005 -0.0005 
 -0.0150 -0.0075 -0.0048 -0.0005 -0.0002 -0.0002 -0.0002 -0.0002 ];

 nz_q = [ 0.5000 0.2418 0.1382 0.1200 0.1500 0.3000 0.2000 0.2000 
 0.5500 0.3110 0.1900 0.1320 0.2000 0.2000 0.2000 0.2000 
 0.7600 0.3801 0.2418 0.1700 0.2200 0.2200 0.2200 0.2200 ];

%**************** And lateral

 fg = [ -0.2988 -0.1482 -0.1127 -0.0929 -0.0786 -0.0808 -0.1271 -0.1207 -0.1437 -0.1913 -0.2500 
 -0.3930 -0.1837 -0.1218 -0.0969 -0.0758 -0.0742 -0.0740 -0.1033 -0.1249 -0.1572 -0.1879 
 -0.1134 -0.1134 -0.1134 -0.1134 -0.0761 -0.0711 -0.0711 -0.0958 -0.1069 -0.1366 -0.1512 ];

 gari = [ -0.4888 0.1812 0.3863 0.6153 0.5331 0.5797 0.3758 0.7605 0.7017 0.4860 -0.0711 
 -0.8864 0.0663 0.3354 0.5493 0.5463 0.6073 0.5874 0.7290 0.8397 0.6899 0.2014 
 0.4023 0.4023 0.4023 0.4023 0.5389 0.6068 0.5857 0.7672 0.7247 0.8314 0.4463 ];

 gbeta = [ 7.9713 4.6485 3.6331 3.0139 1.2534 0.7872 0.8657 0.3034 -0.8020 -0.7749 -1.7562 
 11.3443 6.0008 4.9229 4.1498 2.1949 1.4637 1.4697 1.0492 0.2989 0.4559 -0.0319 
 6.5288 6.5288 6.5288 6.5288 3.6469 2.4639 2.3471 2.1049 1.7867 2.1131 1.8223 ];

 groll = [ 0.5658 0.1739 0.0706 0.0601 0.0389 0.0599 0.0623 0.0657 0.0595 0.0563 0.1529 
 0.9497 0.1236 0.1457 0.1218 0.0835 0.1075 0.1095 0.1329 0.1588 0.1945 0.2294 
 0.0485 0.0485 0.0485 0.0485 0.1495 0.1752 0.1644 0.2115 0.2510 0.2797 0.2902 ];

 dgbeta = [ 5.3589 2.8177 1.9977 1.5167 0.8883 0.7492 0.8232 1.3147 2.0038 2.4470 2.5507 
 6.4372 3.4630 2.6464 1.9909 1.1410 0.9081 0.9374 1.3687 1.9664 2.3818 2.6020 
 2.8443 2.8443 2.8443 2.8443 1.5635 1.1767 1.1462 1.4884 2.0241 2.4413 2.7999 ];

% Check the worst percentual error for every point

worst = alterrcheck(ki,100) % Variable name and altitude error
worst = alterrcheck(kp,100) % Variable name and altitude error
worst = alterrcheck(kalfa,100) % Variable name and altitude error
worst = alterrcheck(kq,100) % Variable name and altitude error
worst = alterrcheck(kvt,100) % Variable name and altitude error
worst = alterrcheck(nz_alpha,100) % Variable name and altitude error
worst = alterrcheck(nz_i,100) % Variable name and altitude error
worst = alterrcheck(nz_p,100) % Variable name and altitude error
worst = alterrcheck(nz_q,100) % Variable name and altitude error
worst = alterrcheck(fg,100) % Variable name and altitude error
worst = alterrcheck(gari,100) % Variable name and altitude error
worst = alterrcheck(gbeta,100) % Variable name and altitude error
worst = alterrcheck(groll,100) % Variable name and altitude error
worst = alterrcheck(dgbeta,100) % Variable name and altitude error



