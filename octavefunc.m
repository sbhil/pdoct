function y = octavefunc(x, param)
%% DESCRIPTION
%
%   x is the input signal. Each column is a separate channel.
%
%   param is an optional parameter message contained in a struct. The field 'id' 
%   contains a string indetifying the parameter and the field 'val' contains a 
%   float vector of arbitrary length containing the parameter values.  


%% input checking %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

x = x(:);

%assert(size(x,2)==1); %mono input only

%% parameter storage %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

persistent gain

if isempty(gain)
    gain = 1;
end

persistent pan

if isempty(pan)
    pan = 0;
end

%% parameter update %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

if exist('param', 'var')
      
    if strcmpi(param.id, 'gain')
        gain = param.val(1); %TODO: do some bounds checking     
    elseif strcmpi(param.id, 'pan')
        pan = param.val(1); %TODO: do some bounds checking
    end

end

%% processing loop %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

y = zeros(size(x,1), 2);

for n = 1:length(x)
  
  x_ = x(n) * gain;
  
  y(n,1) = x_ * (1 - pan)/2;
  y(n,2) = x_ * (1 + pan)/2;
 
end

endfunction