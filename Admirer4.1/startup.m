% filename: startup.m
% From david.bennett@baesystems.com
% Add the following directories to the Matlab path.
% Revised by Gunnar Hovmark FOI, 2001-06-05.
admire = eval('pwd');

path(path,[admire,'/ac']),
path(path,[admire,'/ac/c']),
path(path,[admire,'/ac/dll']),
path(path,[admire,'/fcs/c']),
path(path,[admire,'/fcs/dll']),
path(path,[admire,'/linearisation']),
path(path,[admire,'/plot']),
path(path,[admire,'/simulink']),
path(path,[admire,'/trimdata']),
path(path,[admire,'/trimming']),
path(path,[admire,'/trimming_vt']),
path(path,[admire]),
