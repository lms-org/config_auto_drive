function [r, Pk] = kalman_filter_lr (r, delta_x, delta_y, delta_phi, Pk, Q, R_fakt, delta, xl, yl, xr, yr, xm, ym, interp_mode) %#codegen

% "kalman_filter_lr" 
% 
% ----Nomenklatur----
%  * r: Zustandsvektor = [y0, phi0, kappa_1, kappa_2, ... , kappa_(n-1)] 
% 	--> y0: y-Wert des ersten Punktes (x-Wert ist immer 0)
% 	--> phi0: Anfangssteigung zwischen erstem und zweitem Punkt
% 	--> kappa_i: Krümmungen an den entsprechenden Punkten (Krümmungsdefinition: 1/R, 
% 	    wobei R der Radius des Kreises ist, auf dem der betrachtete Punkt, sein Vorgänger 
% 	    und sein Nachfolger liegen)
%  * delta_x, delta_y, delta_phi: skalare Werte für die Eigenbewegung des Fahrzeugs seit dem letzten Aufruf des Filters (im KOSY des letzten Aufrufs); 
%    ist delta_x = delta_y = delta_phi = 0, wird die Berechnung einer state-transition Matrix deaktiviert
%  * Pk: Kovarianzmatrix des Zustands dim[n x n], mit n = Dimension des Zustandsvektors
%  * Q: Kovarianzmatrix des Zustandsübergangs (Prozessrauschen) dim[n x n] (symmetrische Matrix, mit weg von der 
% 	    Diagonalen abnehmenden Eintraegen -> wie stark hängen die Krümmungen an den verschiedenen Punkten zusammen?)
%  * R_fakt: Varianz der Messwerte (Messrauschen)
%  * delta: Abstand zwischen aufeinanderfolgenden Punkten (delta*n ergibt die Länge des praedizierten Fahrstreifens)
%  * xl, yl: Vektoren mit den Messwerten für die linke Spur
%  * xr, yr: Vektoren mit den Messwerten für die rechte Spur
%  * xm, ym: Vektoren mit den Messwerten für die Mittellinie
%  * interp_mode: 0 für lineare Interpolation der Krümmungen bei state transition, 1 für quadratische Interpolation
% 
%  ----Grober Ablauf des Algorithmus----
%  1. Projektion der Punkte von der Mittellinie nach links bzw. rechts
%  2. Fuer jeden Messpunkt: Berechnung des kleinsten Abstands zum aktuell praedizierten Strassenverlauf
%  3. Assemblierung der Jakobimatrix fuer die Projektion aus dem Zustandsraum von r auf x-y-Koordinaten
%  4. Kalman-Filter: Praediktion -> Messwerte einbeziehen -> Update
%  5. Zustandsbegrenzungen einbringen (mathematisch unschön aber praktisch funktionierts)


%% Messpunkte verarbeiten

% linke Seitenlinie
[Hl, zl, zml] = handle_measurements(r, delta, 0.38, xl, yl);

% rechte Seitenlinie
[Hr, zr, zmr] = handle_measurements(r, delta, -0.38, xr, yr);

% Mittellinie
[Hm, zm, zmm] = handle_measurements(r, delta, 0, xm, ym);

% Linien in eine Matrix kombinieren
H = [Hl; Hr; Hm]; %Messmatrix
z = [zl; zr; zm]; %Messwerte
zm = [zml; zmr; zmm]; %erwartete Werte


%% KALMAN FILTER    

% state transition
if delta_x == 0 && delta_y == 0 && delta_phi == 0
    A = eye(numel(r));
else
    [r, A] = state_transition(r, delta, delta_x, delta_y, delta_phi, interp_mode);    
end

R = R_fakt.*eye(2*(numel(xl) + numel(xr) + numel(xm))); 

% Filtergleichungen EKF
Pk = A*Pk*A' + Q;
y_tilde = z - zm;
K = Pk*H'/(H*Pk*H' + R);
r = r + K*y_tilde;
Pk = (eye(numel(r)) - K*H)*Pk;


%% Zustandsbegrenzungen
% Krümmungen
kr = 0.75; % bezogen auf Mittellinie, 0.72 entspricht einem minimalen Innenradius von 1m
for i=3:numel(r)
    r(i) = min(kr, max(-kr, r(i)));
end

% y-Wert des ersten Punktes
y_max = 0.5;
r(1) =  max(-y_max, min(y_max, r(1)));

% Startwinkel 
phi_max = 0.79;
r(2) =  max(-phi_max, min(phi_max, r(2))); % 0.79 rad = 45 Grad










