function H = messmatrix(P, r, delta, ind, lambda)

num_points = numel(r);
d = delta;

%Ableitungen nach phi0
d_phi0 = zeros(num_points, 2);
for p=2:num_points
    d_phi0(p, 1) = d_phi0(p-1, 1) - d*sin(P(p, 3)); %x-Komponenten
    d_phi0(p, 2) = d_phi0(p-1, 2) + d*cos(P(p, 3)); %y-Komponenten
end

%Ableitungen nach den Krümmungen c_i
d_c_x = zeros(num_points, num_points); %x-Komponenten
d_c_y = zeros(num_points, num_points); %y-Komponenten
for p=3:num_points
    for c=2:num_points-1
        if p>c     
            d_c_x(p, c) = d_c_x(p-1, c) - d^2*sin(P(p, 3))/sqrt(1-d^2*r(c+1)^2/4);
            d_c_y(p, c) = d_c_y(p-1, c) + d^2*cos(P(p, 3))/sqrt(1-d^2*r(c+1)^2/4);
        end        
    end   
end


%Messmatrix zusammensetzen
H = zeros(2*numel(lambda), numel(r));
count = 1;
for i=1:numel(ind) %alle Messpunkte iterieren
    H(count, 1)   = 0;   %Ableitung nach y0
    H(count+1, 1) = 1;   
    
    H(count, 2)   = d_phi0(ind(i), 1);   %Ableitung nach phi0
    H(count+1, 2) = d_phi0(ind(i), 2);   
    
    for c=3:ind(i) %Ableitung nach Krümmungen c
%         if c==ind(i)
%             H(count, c)   = d_c_x(ind(i)-1, c-1) - lambda(i)*d^2*sin(P(ind(i), 3))/sqrt(1-d^2*r(c-1)^2/4);
%             H(count+1, c) = d_c_y(ind(i)-1, c-1) + lambda(i)*d^2*cos(P(ind(i), 3))/sqrt(1-d^2*r(c-1)^2/4);
%         else
%             H(count, c)   = d_c_x(ind(i), c-1);
%             H(count+1, c) = d_c_y(ind(i), c-1);
%         end   
        if c==ind(i)
            H(count, c)   = d_c_x(ind(i), c-1) - lambda(i)*d^2*sin(P(ind(i)+1, 3))/sqrt(1-d^2*r(c-1)^2/4);
            H(count+1, c) = d_c_y(ind(i), c-1) + lambda(i)*d^2*cos(P(ind(i)+1, 3))/sqrt(1-d^2*r(c-1)^2/4);
        else
            H(count, c)   = d_c_x(ind(i), c-1);
            H(count+1, c) = d_c_y(ind(i), c-1);
        end           
    end
    
    count = count + 2;
end

end





