clear;
clc;

delta = 1;
r = [0.2 0.0 0.2 0 0.3 0.5 0.5 0.3 ]';
% r = [0.2 0.3 0.3 0.5 -0.5 -0.4 -0.5 -0.3 -0.4 -0.3]';

[X, Y, PHI] = getPointsFromState(r, delta);
plot(X, Y, '-o', 'linewidth', 2)
axis equal;
hold on;

x0 = 0;
y0 = 0;
dd = 0.3;

A = zeros(numel(r));

A(1,1) = 1;
A(2,2) = 1;
A(2,3) = dd/sqrt(1-delta^2*r(3)^2/4);
for i=3:numel(r)-1
    A(i,i) = 1-dd/delta;
    A(i,i+1) = dd/delta;
end
A(numel(r),numel(r)) = 1;

% for i=1:3
%     
%     x0 = x0 + dd*cos(r(2));
%     y0 = y0 + dd*sin(r(2));
%     
%     r = A*r;
%     
%     [X, Y, PHI] = getPointsFromState(r, delta);
%     plot(X+x0, Y+y0, '-o')
%     axis equal;
%     
% end

% for i=1:1
%     x0 = x0 + dd*cos(r(2));
%     y0 = y0 + dd*sin(r(2));
%     
%     r_new = r;
%     for p=3:numel(r)-1        
%         r_new(p) = r(p) +  dd/delta*(r(p+1)-r(p));        
%     end
%     r_new(end) = r(end);
%     
%     r_new(2) = r_new(2) + dd/delta*(2*acos(-delta*r(3)/2) - pi);
% 
%     r = r_new;
%     
%     [X, Y, PHI] = getPointsFromState(r, delta);
%     plot(X+x0, Y+y0, '-o')
%     axis equal;
%     
% end


%state transition (experimentell und vereinfacht)
dd = 0.3;
A = zeros(numel(r));
A(1,1) = 1;
A(2,2) = 1;
A(2,3) = dd/sqrt(1-delta^2*r(3)^2/4);
for i=3:numel(r)-1
    A(i,i) = 1-dd/delta;
    A(i,i+1) = dd/delta;
end
A(numel(r),numel(r)) = 1;

r = A*r;
% r(2) =r(2)-0.1;
[X, Y, PHI] = getPointsFromState(r, delta);
plot(X+dd, Y+y0, '-o')

r = A*r;
% r(2) =r(2)-0.1;
[X, Y, PHI] = getPointsFromState(r, delta);
plot(X+2*dd, Y+y0, '-o')