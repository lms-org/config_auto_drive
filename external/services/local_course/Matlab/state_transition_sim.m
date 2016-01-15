
clear;
clc;

delta = 1;

r = [0.2 0.4 0.2 0.2 0.3 -0.2 -0.5]';

dx = 0.3;
dy = 0.1;
dphi = 0.1;

x_k = 0;
y_k = 0;
phi_k = 0;

[X, Y, PHI] = getPointsFromState(r, delta);
plot(X, Y, '-*', 'linewidth', 3)
hold on
grid on

for i=1:4
    
    %state transition
    [r, A] = state_transition(r, delta, dx/cos(phi_k) + dy*sin(phi_k), -dx*tan(phi_k) + dy*cos(phi_k), dphi, 1);
    
    
    %Punkte im k-System
    [X, Y, PHI] = getPointsFromState(r, delta);

    %Parameter des neuen KOSY 
    x_k = x_k + dx;
    y_k = y_k + dy;
    phi_k = phi_k + dphi;
    
    %Transormationsmatrix ins 0-System
    D_k0 = [cos(phi_k), -sin(phi_k), x_k; 
            sin(phi_k), cos(phi_k), y_k; 
            0, 0, 1];
%     A = [cos(phi_k), -sin(phi_k); 
%             sin(phi_k), cos(phi_k)];
    
    %Transformation ins 0-System
    Z = D_k0*[X'; Y'; ones(1, numel(X))];  
%     Z = A*[X'; Y'];  
%     Z(1,:) = Z(1,:) + x_k;
%     Z(2,:) = Z(2,:) + y_k;
    
    plot(Z(1,:), Z(2,:), '-o')
    axis equal
    
    t1 = D_k0 * [1;0;1];
    t2 = D_k0 * [0;1;1];
    plot([x_k t1(1) x_k t2(1)],[y_k t1(2) y_k t2(2)], '-b')

end


