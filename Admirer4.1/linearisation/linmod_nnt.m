function [A,B,C,D]=linmod(fcn,varargin)
%LINMOD Obtains linear models from systems of ord. diff. equations (ODEs).
%   [A,B,C,D]=LINMOD('SYS') obtains the state-space linear model of the
%   system of ordinary differential equations described in the
%   block diagram 'SYS' when the state variables and inputs are set
%   to zero.
%
%   [A,B,C,D]=LINMOD('SYS',X,U) allows the state vector, X, and
%   input, U, to be specified. A linear model will then be obtained
%   at this operating point.
%
%   [A,B,C,D]=LINMOD('SYS',X,U,PARA) allows a vector of parameters
%   to be set.  PARA(1) sets the perturbation level for obtaining the
%   linear model (default PARA(1)=1e-5).  For systems that are functions
%   of time PARA(2) may be set with the value of t at which the linear
%   model is to be obtained (default PARA(2)=0). Set PARA(3)=1 to remove
%   extra states associated with blocks that have no path from input to output.
%
%   To see more help, enter TYPE LINMOD
%   See also LINMOD2, DLINMOD, TRIM.

%   Copyright (c) 1990-1998 by The MathWorks, Inc. All Rights Reserved.
%   $Revision: 1.27 $
%   Andrew Grace 11-12-90, 7-24-97
%   Revised: Marc Ullman 8-26-96

%   [A,B,C,D]=LINMOD('SYS',X,U,PARA,XPERT,UPERT) allows the perturbation
%   levels for all of the elements of X and U to be set.
%   The default is otherwise  XPERT=PARA(1)+1e-3*PARA(1)*abs(X),
%   UPERT=PARA(1)+1e-3*PARA(1)*abs(U).
%
%   Any or all of  PARA, XPERT, UPERT may be empty matrices in which case
%   these parameters will be assumed to be undefined and the default
%   option will be used.

warning_state=warning;
warning('on')

% Pre-compile the model
feval(fcn, [], [], [], 'lincompile');

% Run the algorithm as a subroutine so we can trap errors and <CTRL-C>
lasterr('')
errmsg='';
eval('[A,B,C,D]=linmod_alg(fcn,varargin{:});','errmsg=lasterr;')

% Release the compiled model
feval(fcn, [], [], [], 'term');

warning(warning_state)

% Issue an error if one occured during the trim.
if ~isempty(errmsg),
  error(errmsg);
end

% end linmod

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [A,B,C,D]=linmod_alg(fcn,x,u,para,xpert,upert)

% ---------------Options--------------------
sizes = feval(fcn, [], [], [], 'sizes');
sizes=[sizes(:); zeros(6-length(sizes),1)];
nxz=sizes(1)+sizes(2); nu=sizes(4); ny=sizes(3);
nx=sizes(1);

if nargin<2, x=[]; end
if nargin<3, u=[]; end
if nargin<4, para=[]; end
if nargin<5, xpert=[]; end
if nargin<6, upert=[]; end

if isempty(u), u=zeros(nu,1); end
if isempty(x), x=zeros(nxz,1); end
if isempty(para) , para=[0;0;0]; end
if para(1)==0, para(1)=1e-5; end
if isempty(upert), upert=para(1)+1e-3*para(1)*abs(u); upert=ones(size(u))*0.1; end
if isempty(xpert), xpert=para(1)+1e-3*para(1)*abs(x); xpert=ones(size(x))*1e-3; end
if length(para)>1, t=para(2); else t=0; end
if length(para)<3, para(3)=0; end
if length(x)<nxz
  warning('Extra states are being set to zero.')
  x=[x(:); zeros(nxz-length(x),1)];
end

if nxz > nx
  warning('Ignoring discrete states (use dlinmod for proper handling).');
end

A=zeros(nx,nx); B=zeros(nx,nu); C=zeros(ny,nx); D=zeros(ny,nu);

% Initialization
oldx=x; oldu=u;
for nnt=1:7
   y  = feval(fcn, t, x ,u, 'outputs');
   dx = feval(fcn, t, x, u, 'derivs');
end

oldy=y; olddx=dx;

% A and C matrices
for i=1:nx;
   x(i)=x(i)+xpert(i);
   for nnt=1:7
      y  = feval(fcn, t, x ,u, 'outputs');
      dx = feval(fcn, t, x, u, 'derivs');
   end
  A(:,i)=(dx-olddx)./xpert(i);
  if ny > 0
    C(:,i)=(y-oldy)./xpert(i);
  end
  x=oldx;
end

% B and D matrices
for i=1:nu
   u(i)=u(i)+upert(i);
   for nnt=1:7
      y  = feval(fcn, t, x ,u, 'outputs');
      dx = feval(fcn, t, x, u, 'derivs');
   end

  if ~isempty(B),
    B(:,i)=(dx-olddx)./upert(i);
  end
  if ny > 0
    D(:,i)=(y-oldy)./upert(i);
  end
  u=oldu;
end

% para(3) is set to 1 to remove extra states from blocks that are not in the
% input/output path. This removes a class of uncontrollable and unobservable
% states but does not remove states caused by pole/zero cancellation.
if para(3) == 1 
   [A,B,C] = minlin(A,B,C);
end

% Return transfer function model
if nargout == 2
  disp('Returning transfer function model')
  % Eval it in case it's not on the path
  for nnt=1:5
     [A, B] = feval('ss2tf',A,B,C,D,1);
  end
end
