function [H, z, zm] = handle_measurements(r, delta, proj_dist, xm, ym)

if proj_dist ~= 0
    [xp, yp, phi] = projectPoints(r, delta, proj_dist); %Punkte von Mittellinie nach links/rechts projizieren
else
    [xp, yp, phi] = getPointsFromState(r, delta); 
end

P = [xp, yp, phi];
D = 10000*ones(numel(xm), 3);

% Fuer jeden Messpunkt den nähesten Punkt der aktuellen Praediktion finden 
for s=1:numel(r)
   for m=1:numel(xm)            
        dist_point = sqrt((P(s, 1)-(xm(m)))^2 + (P(s, 2)-(ym(m)))^2);
        if dist_point < D(m, 3)
            D(m, 1) = s;
            D(m, 2) = 0;
            D(m, 3) = dist_point;            
        end            
        if s > 1
            [dist_line, lambda, ~] = d_line_point([P(s-1, 1), P(s-1, 2)], [P(s, 1), P(s, 2)], [xm(m), ym(m)]);
            if dist_line < D(m, 3) && lambda > 0 && lambda < 1
                D(m, 1) = s-1;
                D(m, 2) = lambda;
                D(m, 3) = dist_line;              
            end
        end         
    end 
end

% Messpunkte ausfiltern, die vor dem letzten prädizierten Punkt oder zu weit entfernt liegen
ind = (D(:, 1) == numel(r) | D(:, 3) > 0.5);
xm(ind) = [];
ym(ind) = [];
D(ind, :) = [];


% Messmatrix H, Messvektor z und Erwartungsvektor zm berechnen
H = messmatrix(P, r, delta, D(:, 1), D(:, 2));
z =  zeros(2*numel(xm), 1);
zm = zeros(2*numel(xm), 1);
count = 1;
for m=1:numel(xm)
        z(count) = xm(m);
        z(count + 1) = ym(m);
        zm(count)     = P(D(m, 1), 1) + D(m, 2) * (P(D(m, 1)+1, 1) - P(D(m, 1), 1));
        zm(count + 1) = P(D(m, 1), 2) + D(m, 2) * (P(D(m, 1)+1, 2) - P(D(m, 1), 2));
        count = count + 2;
end
