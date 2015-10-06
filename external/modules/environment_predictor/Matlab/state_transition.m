function [r, A] = state_transition(r, delta, delta_x, delta_y, delta_phi, interp_mode)

phi0 = r(2);
theta = pi/2 + phi0 - delta_phi;
a = r(1) - delta_y + delta_x*tan(phi0);
c = delta_x/cos(phi0) - a*sin(delta_phi)/sin(theta);
dx = c;
r(1) = a*cos(phi0)/sin(theta);
r(2) = phi0 - delta_phi;

A = eye(numel(r));    
A(2,3) = dx/sqrt(1-delta^2*r(3)^2/4);

if interp_mode ==  0    
    for i=3:numel(r)-1 %lineare Interpolation der Krümmungen
        A(i,i) = 1-dx/delta;
        A(i,i+1) = dx/delta;
    end
    A(numel(r),numel(r)) = 1;
else        
    d = dx/delta;
    A(3,3) = 1/2*(d^2-d) + 1-d^2; %Krümmung am ersten Punkt = Krümmung am zweiten Punkt
    A(3,4) = 1/2*(d^2+d);
    for i=4:numel(r)-1 %quadratische Interpolation der Krümmungen    
        A(i,i-1) = 1/2*(d^2-d);
        A(i,i) = 1-d^2;
        A(i,i+1) = 1/2*(d^2+d);
    end
    A(numel(r),numel(r)) = 1;
end

r = A*r;

A(1,1) = cos(phi0)/cos(phi0-delta_phi); 
A(1,2) = (delta_x*cos(delta_phi) + delta_y*sin(delta_phi) - r(1)*sin(delta_phi))/cos(delta_phi - phi0)^2;

end