clear;
clc;
close all;

delta = 0.2;

r = [0.0 0.4 0.2 0.2 0.3 -0.2 -0.5]';

Pk = eye(numel(r));
Q = eye(numel(r)).*1; %Kovarianz des Zustandsübergangs
for i=1:numel(r)
    for k=1:numel(r)
        Q(i, k) = 15*(1 - 0.2^(1/abs(k-i)));
    end
end
R_fakt = 1;

[X, Y, PHI] = getPointsFromState(r, delta);
plot(X,Y)
axis equal;
grid on;
hold on;

for i = 1:100
    clf
%     [r, Pk] = kalman_filter_lr (r, eye(numel(r)), Pk, Q, R_fakt, delta, [0 0.2], [0.4 0.4], [0.5 1.2], [-0.4 -0.4]);
    
    xl = [0 0.2 0.8];
    yl = [0.4 0.4 0.4];
    
    xr = [0.4 1];
    yr = [-0.4 -0.4];
    
    xm = [0.5 0.7];
    ym = [0 0];   
    
    plot([xl xr xm], [yl yr ym], 'o')
    hold on;
    
    [r, Pk] = kalman_filter_lr (r, 0, 0, 0, Pk, Q, ...
        R_fakt, delta, xl, yl, xr, yr, xm, ym, 1);
    
    [xp, yp, phi] = projectPoints(r, delta, 0.4);
    plot(xp, yp, '-*')    
    hold on;
    
    [xp, yp, phi] = projectPoints(r, delta, -0.4)
    plot(xp, yp, '-*')    
    hold on;
    
    [X, Y, PHI] = getPointsFromState(r, delta);
    plot(X,Y,'-o')
    axis ([0 1.2 -0.6 0.6])
    
    pause(0.05)
    
end

