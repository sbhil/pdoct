function y = octavefunc(x, param)
%% DESCRIPTION
%
%   x is the input signal. Each column is a separate channel.
%
%   param is an optional parameter message contained in a struct. The field 'id' 
%   contains a string indetifying the parameter and the field 'val' contains a 
%   float vector of arbitrary length containing the parameter values.  


%% input checking %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

if (size(x,2) ~= 1) %mono input only (one input channel only)
  error('only mono input allowed')
end

%% parameter storage %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

persistent bypass
if isempty(bypass)
    bypass = 0;
end

persistent gain
if isempty(gain)
    gain = 1;
end

persistent pan
if isempty(pan)
    pan = 0;
end

%% memory storage %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%% parameter update %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

if exist('param', 'var')
      
  switch(param.id)  
    case {'reset'}

    case {'bypass'}
      bypass = param.val(1);
    case {'gain'}
        gain = param.val(1); %TODO: do some bounds checking     
    case {'pan'}
        pan = param.val(1); %TODO: do some bounds checking
    otherwise
      printf('unrecognized parameter id "%s"', param.id)    
  end

end

%% processing loop %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

y = zeros(size(x,1), 2);

if ~bypass %normal processing
  
  for n = 1:length(x)
  
    x_ = x(n) * gain;
  
    y(n,1) = x_ * (1 - pan)/2;
    y(n,2) = x_ * (1 + pan)/2;
 
  end

else %bypass
  
  y(:,1) = x/2;
  y(:,2) = x/2;

end

endfunction