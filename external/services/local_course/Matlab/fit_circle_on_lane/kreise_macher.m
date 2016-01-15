clear;
clc;

data = csvread('env_20151002-130550_kalman.csv');
t = linspace(0,2*pi, 1000);
r_old = 0;

for i=480:size(data,1)
    clf;
    i
    [X, Y] = getPointsFromState(data(i,2:11), 0.2);
    kappa = mean([data(i,4:6)]);
%     r = 0.9*r_old + 0.1*1/kappa;
%     r_old = r;
    r = 1/kappa;
    
    ot = [X(2)-X(1); Y(2)-Y(1)]; %vector from point1 to point2
    v = [-ot(2) ot(1)]; %vector normal to "ot"
    c = [0 data(i,2)] + v./norm(v)*r; %center of circle
    
    plot(c(1)+abs(r)*cos(t),c(2)+abs(r)*sin(t))
    hold on;
    plot(X,Y,'-o')
    hold off;
    axis([-0.5 1.2 -1 1])
%     axis equal
    waitforbuttonpress
end
