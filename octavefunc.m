function y = octavefunc(x, param)
%% DESCIPTION

x = x(:);

disp('lala')

1
12

%% input checking %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%assert(size(x,2)==1); %mono input only

%% init return meassage %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

msg = {};

%% parameter update %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

persistent gain

if isempty(gain)
    gain = 1;
end

persistent pan

if isempty(pan)
    pan = 0;
end

if exist('param', 'var')

      if strcmpi(param.keys, 'gain')
        gain = param('gain'); 
      end
      
%    if strcmpi(param.id, 'gain')
%        assert(param.val(1)>=0 && param.val(1)<=1)
%        gain = param.val(1);     
%    elseif strcmpi(param.id, 'pan')
%        assert(param.val(1)>=-1 && param.val(1)<=1)
%        pan = param.val(1); 
%    end

end

%% processing loop %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

y = zeros(size(x,1), 2);

for n = 1:length(x)
  
  x_ = x(n) * gain;
  
  y(n,1) = x_ * (1 - pan)/2;
  y(n,2) = x_ * (1 + pan)/2;
 
end

endfunction