%------------------------------
% Gradientenverfahren
% Dario Wikart
% 10.05.2013
%------------------------------

x = -4:.02:4; y = -4:.02:4;
[X,Y] = meshgrid(x,y);

% Schrittweite
step = 0.015;

%Funktion
%---------
f = @(X,Y)(X.^2+Y-11).^2+(X+Y.^2-7).^2;
%Z = X.^2.*Y.^2;
%Z = sin(3*Y-X.^2+1)+cos(2*Y.^2-2*X);
%Z = (X.^2+Y-11).^2+(X+Y.^2-7).^2;

Z = f(X,Y);

% richtungsableitungen:
%----------------------
% gradx = @(x,y)2*x*y^2;
% grady = @(x,y)x^2*2*y;
%gradx = @(X,Y)-2*(X*cos(1 - X^2 + 3*Y)+sin(2*(X - Y^2)));
%grady = @(X,Y)3*cos(1 - X^2 + 3*Y) + 4*Y*sin(2*(X - Y^2));
gradx = @(X,Y)2*(-7+X+Y^2+2*X*(-11 + X^2 + Y));
grady = @(X,Y)2*(-11+X^2+Y+2*Y*(-7 + X + Y^2));


% Plot Grundfunktion (Höhenlinien)
%---------------------------------

contour(x,y,Z,40)
hold on;

%Start Wert:{{181.617, {X -> -0.270845, Y -> -0.923039}}}
xi = 0;
yi = 0;

arrayx = xi;% zeros(100,1)];
arrayy = yi;% zeros(100,1)];

for i=1:1000
        
    if abs(gradx(xi,yi))<0.00001 && abs(grady(xi,yi))<0.00001
        break
    end
    
    %t oder step
     xi = xi - step*gradx(xi,yi);
     yi = yi - step*grady(xi,yi);
  
    arrayx(i+1)=xi;
    arrayy(i+1)=yi;
end
i


%PLOT------------------------
plot(arrayx,arrayy,'-r.','MarkerSize',20)

hold off;