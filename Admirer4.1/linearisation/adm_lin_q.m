% filename: adm_lin_q.m
% Made by Harrald Luijerink (DUT/FOI) for the Garteur/AG11 Admire3.4e Clearance Analysis

% For Nichols plot of pitch rate command q
% This m-file is intended as an example or help for AG11 Admire3.4e users 
% This file can and should only be used for the Nichols Stability Criterion
% The use of MATLAB command 'connect' will prevent any algebraic loops (of course other approaches might give the same results!)

% The following uses a modified 'linmod.m'
[Abare,Bbare,Cbare,Dbare] = linmod_nnt('admire_bare_lin',x0(10:37),zeros(16,1));

% Linearise control system excluding control selector.
[Afcs,Bfcs,Cfcs,Dfcs] = linmod_nnt('admire_fcs_lin',x0(38:41),zeros(40,1));

% Linearisation of admire_linear_cut.mdl
[Aopen,Bopen,Copen,Dopen] = linmod('admire_linear_cut');

SYS0 = ss(Aopen,Bopen,Copen,Dopen);
% Pitchrate_command_cl is a 1x1 SiSo system 
% For making a Nichols plot for the Worst Case Stabilty margin, simply use the MATLAB command 'nichols' like:
% [gain,phase,wrad] = nichols(-Pitchrate_command_cl,logspace(..,..,..)); etc, etc...

Pitchrate_command_cl = connect(SYS0,[[1,3:31]',[1,3:31]'],2,2);


