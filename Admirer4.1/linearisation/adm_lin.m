% Linearisation for ADMIRE FCS 3.4e
%
% This linearisation should be used when the trimming has been done with admtrim_aoa or admtrim_sl.
% Written by Torbjörn Norén and Gunnar Hovmark.

% The following uses a modified 'linmod.m'
[Abare,Bbare,Cbare,Dbare] = linmod_nnt('admire_bare_lin',x0(10:37),zeros(16,1));

% Linearise control system EXCLUDING control selector.
[Afcs,Bfcs,Cfcs,Dfcs] = linmod_nnt('admire_fcs_lin',x0(38:41),zeros(40,1));

whos Abare Bbare Cbare Dbare

whos Afcs Bfcs Cfcs Dfcs