function [xp, yp, phi] = projectPoints(r, delta, dist)

% Koordinaten der Punkte für die mittlere Spur berechnen
[x, y, phi] = getPointsFromState(r, delta); 

xp = zeros(numel(r), 1);
yp = zeros(numel(r), 1);

%% Projektion 

% erster Punkt
vx = -(y(2)-y(1));
vy =   x(2)-x(1);
betr = sqrt(vx^2 + vy^2);
xp(1) = x(1) + 1/betr * dist * vx;
yp(1) = y(1) + 1/betr * dist * vy;

%innere Punkte
for z=2:numel(x)-1   
        vx = x(z) - x(z-1) - 0.5*(x(z+1)-x(z-1));
        vy = y(z) - y(z-1) - 0.5*(y(z+1)-y(z-1));
        betr = sqrt(vx^2 + vy^2);
        if betr < 0.0001 
            vx = -(y(z)-y(z-1));
            vy = x(z)-x(z-1);
            betr = sqrt(vx^2 + vy^2);
            xp(z) = x(z) + 1/betr * dist * vx;
            yp(z) = y(z) + 1/betr * dist * vy;
        else
            xp(z) = x(z) - sign(r(z+1))* 1/betr * dist * vx;
            yp(z) = y(z) - sign(r(z+1))* 1/betr * dist * vy;
        end               
end

%letzter Punkt
z=numel(x);
vx = -(y(z)-y(z-1));
vy = x(z)-x(z-1);
betr = sqrt(vx^2 + vy^2);
xp(z) = x(z) + 1/betr * dist * vx;
yp(z) = y(z) + 1/betr * dist * vy;
        
        
        