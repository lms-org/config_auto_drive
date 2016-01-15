# lane_model

This is the MATLAB implementation of a road lane model, which gets adapted by an Extended-Kalman-Filter (EKF). 

----Nomenklatur----
 * r: Zustandsvektor = [y0, phi0, kappa_1, kappa_2, ... , kappa_(n-1)] 
	--> y0: y-Wert des ersten Punktes (x-Wert ist immer 0)
	--> phi0: Anfangssteigung zwischen erstem und zweitem Punkt
	--> kappa_i: Krümmungen an den entsprechenden Punkten (Krümmungsdefinition: 1/R, 
	    wobei R der Radius des Kreises ist, auf dem der betrachtete Punkt, sein Vorgänger 
	    und sein Nachfolger liegen)
 * delta_x, delta_y, delta_phi: skalare Werte für die Eigenbewegung des Fahrzeugs seit dem letzten Aufruf des Filters (im KOSY des letzten Aufrufs); 
   ist delta_x = delta_y = delta_phi = 0, wird die Berechnung einer state-transition Matrix deaktiviert
 * Pk: Kovarianzmatrix des Zustands dim[n x n], mit n = Dimension des Zustandsvektors
 * Q: Kovarianzmatrix des Zustandsübergangs (Prozessrauschen) dim[n x n] (symmetrische Matrix, mit weg von der 
	    Diagonalen abnehmenden Eintraegen -> wie stark hängen die Krümmungen an den verschiedenen Punkten zusammen?)
 * R_fakt: Varianz der Messwerte (Messrauschen)
 * delta: Abstand zwischen aufeinanderfolgenden Punkten (delta*n ergibt die Länge des praedizierten Fahrstreifens)
 * xl, yl: Vektoren mit den Messwerten für die linke Spur
 * xr, yr: Vektoren mit den Messwerten für die rechte Spur
 * xm, ym: Vektoren mit den Messwerten für die Mittellinie
 * interp_mode: 0 für lineare Interpolation der Krümmungen bei state transition, 1 für quadratische Interpolation

 ----Grober Ablauf des Algorithmus----
 1. Projektion der Punkte von der Mittellinie nach links bzw. rechts
 2. Fuer jeden Messpunkt: Berechnung des kleinsten Abstands zum aktuell praedizierten Strassenverlauf
 3. Assemblierung der Jakobimatrix fuer die Projektion aus dem Zustandsraum von r auf x-y-Koordinaten
 4. Kalman-Filter: Praediktion -> Messwerte einbeziehen -> Update
 5. Zustandsbegrenzungen einbringen (mathematisch unschön aber praktisch funktionierts)
 