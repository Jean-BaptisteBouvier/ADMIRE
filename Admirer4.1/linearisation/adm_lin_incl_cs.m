% Linearisation for ADMIRE FCS 3.4e
%
% This linearisation should be used when the trimming has been done with admtrim_aoa or admtrim_sl.
% Written by Torbjörn Norén and Gunnar Hovmark.

% The following uses a modified 'linmod.m'
[Abare,Bbare,Cbare,Dbare] = linmod_nnt('admire_bare_lin',x0(10:37),zeros(16,1));

% Linearise control system INCLUDING control selector.
[Afcs_cs,Bfcs_cs,Cfcs_cs,Dfcs_cs] = linmod_nnt('admire_fcs_lin_incl_cs',x0(38:41),zeros(40,1));

whos Abare Bbare Cbare Dbare

whos Afcs_cs Bfcs_cs Cfcs_cs Dfcs_cs