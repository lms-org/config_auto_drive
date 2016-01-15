function [d, lambda, S] = d_line_point(P, Q, M) 
%Abstand d zwischen der Gerade g (von P in Richtung Q) und dem Punkt M
%S ist der Punkt auf g mit kleinstem Abstand zu M

v = Q-P;

lambda = - (v(1)*(P(1)-M(1)) + v(2)*(P(2)-M(2)))/(v(1)^2 + v(2)^2);
S = P + lambda*v;

d = sqrt((M(1)-S(1))^2 + (M(2)-S(2))^2);