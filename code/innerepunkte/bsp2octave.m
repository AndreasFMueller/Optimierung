% bsp2octave.m
% mathematisches Seminar FS 2013
% Optimierung: Innere-Punkte-Verfahren
% 20.04.2013



c =     [ 1,  6,  0,  3,  2]'   % Zielfunktion, interpretieren als... (siehe Wert der Variable s)

A =     [ 2,  2,  0,  0,  0;
         -1,  0,  2,  2,  0;
          0, -1, -1,  0,  2;
          0,  0,  0, -1, -1]

b =     [5, 0, 0, -5]'
lb=     zeros(5,1)              % lower bound (=0, falls keine Untergrenze angegeben)
ub=     []                      % upper bound (=unendlich, falls keine Obergrenze angegeben)
ctype = "UUUU"                  % U = Ungleichung mit Obergrenze (b enthält grösstmögliche Werte)
vartype="CCCCC"                 % C = Continuous Variable; I = Integer Variable;
s =     1                       % -1 = Maximierungsproblem; 1 = Minimierungsproblem;

        
param.msglev =    3             % Parameter: Level of messages output by solver routines (1=no - 4=full)
param.itlim =     1000          % Parameter: Simplex max. Anzahl Iterationen
param.lpsolver = 	2             % Parameter: 1=Simplex / 2=innerePunkte

          
[xopt, fopt, status, extra] = glpk (c, A, b, lb, ub, ctype, vartype, s, param);


status                          % 150=innerPointMethod is undefined oder 151=innerPointMethod is optimal
extra                           % lambda=dual variables; redcosts = reduced costs; 
                                % time = time in seconds for LP/MIP problem; 
                                % mem = memory used for LP/MIP in bytes

		
xopt                            % Optimum, bester Punkt	
fopt                            % Wert des Optimums / des besten Punktes
