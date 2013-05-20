% bsp2.m
% mathematisches Seminar FS 2013
% Optimierung: Innere-Punkte-Verfahren
% 20.04.2013

c =     [ 1,  6,  0,  3,  2]'             % Zielfunktion, zu minimieren

A =     [ 2,  2,  0,  0,  0;              % Matrix mit Nebenbedingungen
         -1,  0,  2,  2,  0;
          0, -1, -1,  0,  2;
          0,  0,  0, -1, -1]

b =     [5, 0, 0, -5]'                    % Vektor für Nebenbedingungen

e =     0.1*ones(5,1)                     % Genauigkeitsgrenze (Toleranz)																											
I =     eye(5)	

n =     4														
k =     1                                 % Iterationsvariable (Zähler)
a =     1                                 % Kurzschrittalgorithmus: Schrittweite = 1

%------------------------------------------ Schritt 1.)
x = 0.1*ones(5,1)                         % Startvektor für primales Problem
y = 0.1*ones(4,1)                         % Startvektor für duales Problem
s = 0.1*ones(5,1)                         % Startvektor für Schlupfvariablen

%------------------------------------------ Schritt 2.)
u = x'*s/n;

while u>0.001                             % Abbruchbedingung	

S = diag(s);                              % Diagonalmatrix mit Diagonalwerte von Vektoren x,s
X = diag(x);

% ------------------------------------------ Schritt 3.)
u=u*(1-(1/6)/sqrt(n));                    % neuer Straftermfaktor


% ------------------------------------------ Schritt 4.)
dxdyds = [zeros(5), A', I; A, zeros(4), zeros(4,5); S, zeros(5,4), X ]  \  [c-A'*y-s; b-A*x; u*e-S*X*e];


% ------------------------------------------ Schritt 5/6.)
x= x+a*[dxdyds(1), dxdyds(2), dxdyds(3), dxdyds(4), dxdyds(5)]';      % xneu = xalt + dx 
y= y+a*[dxdyds(6), dxdyds(7), dxdyds(8), dxdyds(9)]';                 % yneu = yalt + dy 
s= s+a*[dxdyds(10), dxdyds(11), dxdyds(12), dxdyds(13), dxdyds(14)]'; % sneu = salt + ds 

dx=[dxdyds(1), dxdyds(2), dxdyds(3), dxdyds(4), dxdyds(5)]';

k=k+1;                                    % Inkrementiere Variable k (Anzahl Durchläufe)

endwhile

k                                         % Ausgabe des k-ten Schritts
x                                         % Optimaler Punkt
