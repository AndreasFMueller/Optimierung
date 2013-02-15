\verbatimtex
\documentclass{article}
\usepackage{times}
\usepackage{amsmath}
\usepackage{amssymb}
\usepackage{amsfonts}
\usepackage{color}
\begin{document}
etex;

beginfig(1)

z0 = (0, 0);
z10 = (0, 160);
z20 = (120, 200);
z30 = (240, 160);
z40 = (280, 0);
fill z0--z10--z20--z30--z40--cycle withcolor (0.97,0.97,0.97);

% draw shadows of lines
z11 = (-0.1)[z10,z20];
z12 = 1.1[z10,z20];
z13 = z11 shifted ((1,-3) * (10/sqrt(10)));
z14 = z12 shifted ((1,-3) * (10/sqrt(10)));
fill z11--z12--z14--z13--cycle withcolor (0.9,0.9,0.9);
label.bot(btex $-x + 3y\le 12$ etex, (0,0))  rotated 18 shifted 0.5[z10,z20];

z21 = (-0.1)[z20,z30];
z22 = (1.1)[z20,z30];
z23 = z21 shifted ((-1,-3) * (10/sqrt(10)));
z24 = z22 shifted ((-1,-3) * (10/sqrt(10)));
fill z21--z22--z24--z23--cycle withcolor (0.9,0.9,0.9);
label.bot(btex $x+3y\le 18$ etex, (0,0)) rotated -18 shifted 0.5[z20,z30];

z31 = (-0.1)[z30,z40];
z32 = (1.1)[z30,z40];
z33 = z31 shifted ((-4,-1) * (10/sqrt(17)));
z34 = z32 shifted ((-4,-1) * (10/sqrt(17)));
fill z31--z32--z34--z33--cycle withcolor (0.9,0.9,0.9);
label.bot(btex $4x+y\le 28$ etex, (0,0)) rotated -75 shifted 0.5[z30,z40];

z51 = (0,-20);
z52 = (0,200);
z53 = z51 shifted (10,0);
z54 = z52 shifted (10,0);
fill z51--z52--z54--z53--cycle withcolor (0.9,0.9,0.9);
label.bot(btex $x\ge 0$ etex, (0,0)) rotated 90 shifted 0.5[z0,z10];

z61 = (-20,0);
z62 = (300,0);
z63 = z61 shifted (0,10);
z64 = z62 shifted (0,10);
fill z61--z62--z64--z63--cycle withcolor (0.9,0.9,0.9);
label.top(btex $y\ge 0$ etex, 0.5[z0,z40]);

% draw dots for the points
pickup pencircle scaled 4pt;
draw z10--z10;
draw z20--z20;
draw z30--z30;
draw z40--z40;

% draw lines
pickup pencircle scaled 0.5pt;
draw z11--z12;
draw z21--z22;
draw z31--z32;

% coordinate axes
pickup pencircle scaled 0.5pt
draw (-20,0)--(300,0);
draw (0,-20)--(0, 200);

draw (40, -2)--(40,2);	label.bot(btex $1$ etex, (40,-2));
draw (80, -2)--(80,2);	label.bot(btex $2$ etex, (80,-2));
draw (120, -2)--(120,2);	label.bot(btex $3$ etex, (120,-2));
draw (160, -2)--(160,2);	label.bot(btex $4$ etex, (160,-2));
draw (200, -2)--(200,2);	label.bot(btex $5$ etex, (200,-2));
draw (240, -2)--(240,2);	label.bot(btex $6$ etex, (240,-2));

draw (-2,40)--(2,40);	label.lft(btex $1$ etex, (-2,40));
draw (-2,80)--(2,80);	label.lft(btex $2$ etex, (-2,80));
draw (-2,120)--(2,120);	label.lft(btex $3$ etex, (-2,120));

label(btex $G$ etex, (120,100));

endfig;

beginfig(2)

z0 = (0, 0);
z10 = (0, 160);
z20 = (120, 200);
z30 = (240, 160);
z40 = (280, 0);
fill z0--z10--z20--z30--z40--cycle withcolor (0.97,0.97,0.97);

% draw shadows of lines
z11 = (-0.1)[z10,z20];
z12 = 1.1[z10,z20];
z13 = z11 shifted ((1,-3) * (10/sqrt(10)));
z14 = z12 shifted ((1,-3) * (10/sqrt(10)));
fill z11--z12--z14--z13--cycle withcolor (0.9,0.9,0.9);
label.bot(btex $-x + 3y\le 12$ etex, (0,0))  rotated 18 shifted 0.5[z10,z20];

z21 = (-0.1)[z20,z30];
z22 = (1.1)[z20,z30];
z23 = z21 shifted ((-1,-3) * (10/sqrt(10)));
z24 = z22 shifted ((-1,-3) * (10/sqrt(10)));
fill z21--z22--z24--z23--cycle withcolor (0.9,0.9,0.9);
label.bot(btex $x+3y\le 18$ etex, (0,0)) rotated -18 shifted 0.5[z20,z30];

z31 = (-0.1)[z30,z40];
z32 = (1.1)[z30,z40];
z33 = z31 shifted ((-4,-1) * (10/sqrt(17)));
z34 = z32 shifted ((-4,-1) * (10/sqrt(17)));
fill z31--z32--z34--z33--cycle withcolor (0.9,0.9,0.9);
label.bot(btex $4x+y\le 28$ etex, (0,0)) rotated -75 shifted 0.5[z30,z40];

z51 = (0,-20);
z52 = (0,200);
z53 = z51 shifted (10,0);
z54 = z52 shifted (10,0);
fill z51--z52--z54--z53--cycle withcolor (0.9,0.9,0.9);
label.bot(btex $x\ge 0$ etex, (0,0)) rotated 90 shifted 0.5[z0,z10];

z61 = (-20,0);
z62 = (300,0);
z63 = z61 shifted (0,10);
z64 = z62 shifted (0,10);
fill z61--z62--z64--z63--cycle withcolor (0.9,0.9,0.9);
label.top(btex $y\ge 0$ etex, 0.5[z0,z40]);

% draw dots for the points
pickup pencircle scaled 4pt;
draw z10--z10;
draw z20--z20;
draw z30--z30;
draw z40--z40;

% draw lines
pickup pencircle scaled 0.5pt;
draw z11--z12;
draw z21--z22;
draw z31--z32;

% coordinate axes
pickup pencircle scaled 0.5pt
draw (-20,0)--(300,0);
draw (0,-20)--(0, 200);

draw (40, -2)--(40,2);	label.bot(btex $1$ etex, (40,-2));
draw (80, -2)--(80,2);	label.bot(btex $2$ etex, (80,-2));
draw (120, -2)--(120,2);	label.bot(btex $3$ etex, (120,-2));
draw (160, -2)--(160,2);	label.bot(btex $4$ etex, (160,-2));
draw (200, -2)--(200,2);	label.bot(btex $5$ etex, (200,-2));
draw (240, -2)--(240,2);	label.bot(btex $6$ etex, (240,-2));

draw (-2,40)--(2,40);	label.lft(btex $1$ etex, (-2,40));
draw (-2,80)--(2,80);	label.lft(btex $2$ etex, (-2,80));
draw (-2,120)--(2,120);	label.lft(btex $3$ etex, (-2,120));

%label(btex $G$ etex, (80,80));

pickup pencircle scaled 1pt;
z7 = (6,-4);
draw (z0 shifted -3 z7)--(z0 shifted 4 z7) withcolor (1,0,0);
label.top(btex $Z = 0$ etex, z0) rotated -35 withcolor (1,0,0);

draw (z10 shifted -3 z7)--(z10 shifted 44 z7) withcolor (1,0,0);
label.top(btex $Z = 12$ etex, (0,0)) rotated -35 shifted z10 shifted 20 z7 withcolor (1,0,0);

draw (z40 shifted -48 z7)--(z40 shifted 2 z7) withcolor (1,0,0);
label.top(btex $Z=14$ etex, (0,0)) rotated -35 shifted z40 shifted -21 z7 withcolor (1,0,0);

draw (z20 shifted -2 z7)--(z20 shifted 28 z7) withcolor (1,0,0);
label.top(btex $Z=21$ etex, (0,0)) rotated -35 shifted z20 shifted 15 z7 withcolor (1,0,0);

draw (z30 shifted -11 z7)--(z30 shifted 8 z7) withcolor (1,0,0);
label.top(btex $Z=24$ etex, (0,0)) rotated -35 shifted z30 shifted -6 z7 withcolor (1,0,0);

endfig;

%
% Normals at a optimum solution
%
beginfig(3)

pickup pencircle scaled 1pt;

fill (-60,20)--(24,-8)--(24-3,-8-9)--(-60-3,20-9)--cycle withcolor(0.95,0.95,0.95);
fill (-5,20)--(20,-80)--(20-8,-80-2)--(-5-8,20-2)--cycle withcolor(0.95,0.95,0.95);

z1=(4,1)/sqrt(17);
z2=(1,3)/sqrt(10);

fill (0,0)--(60*z1){dir 104.036}..{dir 161.565}(60*z2)--cycle withcolor(0.9,0.9,0.9);

draw (-5,20)--(20,-80);
draw (-60,20)--(24,-8);

drawarrow (0,0)--(10,30);
label.top(btex $\vec n_2$ etex, (10-4,30));
fill (2-30,0+10)..(0-30,2+10)..(-2-30,0+10)..(0-30,-2+10)..cycle;
drawarrow (0-30,0+10)--(10-30,30+10);

drawarrow (0,0)--(40,10);
label.rt(btex $\vec n_3$ etex, (40,10-3));
drawarrow (0+10,0-40)--(40+10,10-40);
fill (2+10,0-40)..(0+10,2-40)..(-2+10,0-40)..(0+10,-2-40)..cycle;


fill (3,0)..(0,3)..(-3,0)..(0,-3)..cycle;

draw (-60,40)--(60,-40) withcolor(1,0,0);

fill (2,0)..(0,2)..(-2,0)..(0,-2)..cycle withcolor(1,0,0);

pickup pencircle scaled 2pt;

drawarrow (0,0)--(20,30) withcolor(1,0,0);
label.urt(btex $\vec n_Z$ etex, (20,30));

label(btex $G$ etex, (-20, -20));

endfig;

%
% admissible region which does not contain (0,0), and is not contained in x >=0
%
beginfig(4)

pickup pencircle scaled 0.5pt;

draw (-220,0)--(20,0);
draw (0,-20)--(0,120);

draw (-200,-2)--(-200,2);	label.bot(btex $-2$ etex, (-200,-2));
draw (-100,-2)--(-100,2);	label.llft(btex $-1$ etex, (-100,-2));
draw (-2,100)--(2,100);	label.rt(btex $1$ etex, (2,100));

z1 = (-200,100);
z2 = (-100,0);
z3 = (-100,100);

pickup pencircle scaled 1pt;

fill z1--z2--z3--cycle withcolor(0.9,0.9,0.9);

draw ((-0.1)[z1,z2])--((1.1)[z1,z2]);
draw ((-0.14)[z1,z3])--((1.14)[z1,z3]);
draw ((-0.14)[z2,z3])--((1.14)[z2,z3]);

endfig;

beginfig(5)

z1 = (-200,0);
z2 = (-100,0);
z3 = (0,100);
z4 = (0,200);

z5 = (-0.25)[z1,z4];
z6 = 1.25[z1,z4];
z7 = (-0.5)[z2,z3];
z8 = 1.5[z2,z3];

fill z5--z6--z8--z7--cycle withcolor(0.9,0.9,0.9);

pickup pencircle scaled 0.5pt;

draw (-250,0)--(50,0);
draw (0,-50)--(0,250);
draw (-200,-2)--(-200,2);	label.top(btex $-2$ etex, (-203,2));
draw (-100,-2)--(-100,2);	label.bot(btex $-1$ etex, (-100,-2));
draw (-2,100)--(2,100);	label.rt(btex $1$ etex, (2,100));
draw (-2,200)--(2,200);	label.lft(btex $2$ etex, (-2,200));

draw (-200,0)--(-100,0) withcolor(0.8,0.8,0.8);
draw (0,200)--(0,100) withcolor(0.8,0.8,0.8);

pickup pencircle scaled 1pt;

draw z5--z6;
draw z7--z8;

%fill (-83,83)--(-67,83)--(-67,67)--(-83,67)--cycle withcolor (0.95,0.95,0.95);
label(btex $G_2$ etex, (-75,75));

label.top(btex $x_1$ etex, (45,1));
label.lft(btex $x_2$ etex, (-1,245));

pickup pencircle scaled 2pt;
draw (-200,0)--(-100,0) withcolor(1,0,0);
pickup pencircle scaled 5pt;
draw (-200,0)--(-200,0) withcolor(1,0,0);
draw (-100,0)--(-100,0) withcolor(1,0,0);
label.top(btex $\color{red} G_1$ etex, (-150,2));

endfig;

beginfig(6)

pickup pencircle scaled 0.5pt;
draw (-20,0)--(120,0);	label.top(btex $x_1$ etex, (115,2));
draw (0,-20)--(0,320);	label.rt(btex $x_2$ etex, (2,315));
draw (100,-2)--(100,2);	label.bot(btex $1$ etex, (100,-2));
draw (-2,100)--(2,100);	label.lft(btex $1$ etex, (-2,100));
draw (-2,200)--(2,200);	label.lft(btex $2$ etex, (-2,200));
draw (-2,300)--(2,300);	label.lft(btex $3$ etex, (-2,300));

z1 = (0,100);
z2 = (120,220);
z3 = (0,200);
z4 = (120,320);
fill z1--z2--z4--z3--cycle withcolor(0.9,0.9,0.9);
pickup pencircle scaled 2pt;
draw z4--z3--z1--z2;

label(btex $G$ etex, (60,210));

pickup pencircle scaled 1pt;
draw (-20,80)--(140,240) withcolor(1,0,0);
draw (-20,180)--(140,340) withcolor(1,0,0);
drawarrow(50,150)--(80,120) withcolor(1,0,0);
drawarrow(50,250)--(20,280) withcolor(1,0,0);
pickup pencircle scaled 5pt;
draw (50,150)--(50,150) withcolor(1,0,0);
draw (50,250)--(50,250) withcolor(1,0,0);
label.bot(btex Fall $c_1 > 0$ etex, (80,120)) withcolor(1,0,0);
label.top(btex Fall $c_1 < 0$ etex, (30,280)) withcolor(1,0,0);

endfig;

%
%  illustration of the individual steps in the Simplex algorithm
%
beginfig(7)

z0 = (0, 0);
z10 = (0, 160);
z20 = (120, 200);
z30 = (240, 160);
z40 = (280, 0);
fill z0--z10--z20--z30--z40--cycle withcolor (0.97,0.97,0.97);

% draw shadows of lines
z11 = (-0.1)[z10,z20];
z12 = 1.1[z10,z20];
z13 = z11 shifted ((1,-3) * (10/sqrt(10)));
z14 = z12 shifted ((1,-3) * (10/sqrt(10)));
%fill z11--z12--z14--z13--cycle withcolor (0.9,0.9,0.9);
%label.bot(btex $-x + 3y\le 12$ etex, (0,0))  rotated 18 shifted 0.5[z10,z20];

z21 = (-0.1)[z20,z30];
z22 = (1.1)[z20,z30];
z23 = z21 shifted ((-1,-3) * (10/sqrt(10)));
z24 = z22 shifted ((-1,-3) * (10/sqrt(10)));
%fill z21--z22--z24--z23--cycle withcolor (0.9,0.9,0.9);
%label.bot(btex $x+3y\le 18$ etex, (0,0)) rotated -18 shifted 0.5[z20,z30];

z31 = (-0.1)[z30,z40];
z32 = (1.1)[z30,z40];
z33 = z31 shifted ((-4,-1) * (10/sqrt(17)));
z34 = z32 shifted ((-4,-1) * (10/sqrt(17)));
%fill z31--z32--z34--z33--cycle withcolor (0.9,0.9,0.9);
%label.bot(btex $4x+y\le 28$ etex, (0,0)) rotated -75 shifted 0.5[z30,z40];

z51 = (0,-20);
z52 = (0,200);
z53 = z51 shifted (10,0);
z54 = z52 shifted (10,0);
%fill z51--z52--z54--z53--cycle withcolor (0.9,0.9,0.9);
%label.bot(btex $x\ge 0$ etex, (0,0)) rotated 90 shifted 0.5[z0,z10];

z61 = (-20,0);
z62 = (300,0);
z63 = z61 shifted (0,10);
z64 = z62 shifted (0,10);
%fill z61--z62--z64--z63--cycle withcolor (0.9,0.9,0.9);
%label.top(btex $y\ge 0$ etex, 0.5[z0,z40]);

% draw dots for the points
pickup pencircle scaled 4pt;
draw z10--z10;
draw z20--z20;
draw z30--z30;
draw z40--z40;

% draw lines
pickup pencircle scaled 0.5pt;
draw z11--z12;
draw z21--z22;
draw z31--z32;

% coordinate axes
pickup pencircle scaled 0.5pt
draw (-20,0)--(300,0);
draw (0,-20)--(0, 200);

draw (40, -2)--(40,2);	label.bot(btex $1$ etex, (40,-2));
draw (80, -2)--(80,2);	label.bot(btex $2$ etex, (80,-2));
draw (120, -2)--(120,2);	label.bot(btex $3$ etex, (120,-2));
draw (160, -2)--(160,2);	label.bot(btex $4$ etex, (160,-2));
draw (200, -2)--(200,2);	label.bot(btex $5$ etex, (200,-2));
draw (240, -2)--(240,2);	label.bot(btex $6$ etex, (240,-2));

draw (-2,40)--(2,40);	label.lft(btex $1$ etex, (-2,40));
draw (-2,80)--(2,80);	label.lft(btex $2$ etex, (-2,80));
draw (-2,120)--(2,120);	label.lft(btex $3$ etex, (-2,120));

label(btex $G$ etex, (120,100));

pickup pencircle scaled 5pt;
draw z0--z0 withcolor(1,0,0);
draw z10--z10 withcolor(1,0,0);
draw z20--z20 withcolor(1,0,0);
draw z30--z30 withcolor(1,0,0);

pickup pencircle scaled 1pt;
drawarrow (0.02[z0,z10])--(0.98[z0,z10]) withcolor(1,0,0);
drawarrow (0.02[z10,z20])--(0.98[z10,z20]) withcolor(1,0,0);
drawarrow (0.02[z20,z30])--(0.98[z20,z30]) withcolor(1,0,0);
label.urt(btex $x=0,y=0$ etex, z0);
label.rt(btex $\quad y\leftrightarrow s_1$ etex, 0.5[z0,z10]) withcolor(1,0,0);
label.lrt(btex $x=0,s_1=0$ etex, z10);
label.ulft(btex $x\leftrightarrow s_2$ etex, 0.5[z10,z20]) withcolor(1,0,0);
label.top(btex $s_1=0,s_2=0$ etex, z20);
label.urt(btex $s_1\leftrightarrow s_3$ etex, 0.5[z20,z30]) withcolor(1,0,0);
label.urt(btex $s_2=0,s_3=0$ etex, z30);
label.ulft(btex $y=0,s_3=0$ etex, z40 shifted(-3,0));

endfig;

%
% After extension, vertices of the polygon are characterised by the two variables
% that are set to 0 to get that vertex.
%
beginfig(8)

z0 = (0, 0);
z10 = (0, 160);
z20 = (120, 200);
z30 = (240, 160);
z40 = (280, 0);
fill z0--z10--z20--z30--z40--cycle withcolor (0.97,0.97,0.97);

% draw shadows of lines
z11 = (-0.1)[z10,z20];
z12 = 1.1[z10,z20];
z13 = z11 shifted ((1,-3) * (10/sqrt(10)));
z14 = z12 shifted ((1,-3) * (10/sqrt(10)));
%fill z11--z12--z14--z13--cycle withcolor (0.9,0.9,0.9);
%label.bot(btex $-x + 3y\le 12$ etex, (0,0))  rotated 18 shifted 0.5[z10,z20];

z21 = (-0.1)[z20,z30];
z22 = (1.1)[z20,z30];
z23 = z21 shifted ((-1,-3) * (10/sqrt(10)));
z24 = z22 shifted ((-1,-3) * (10/sqrt(10)));
%fill z21--z22--z24--z23--cycle withcolor (0.9,0.9,0.9);
%label.bot(btex $x+3y\le 18$ etex, (0,0)) rotated -18 shifted 0.5[z20,z30];

z31 = (-0.1)[z30,z40];
z32 = (1.1)[z30,z40];
z33 = z31 shifted ((-4,-1) * (10/sqrt(17)));
z34 = z32 shifted ((-4,-1) * (10/sqrt(17)));
%fill z31--z32--z34--z33--cycle withcolor (0.9,0.9,0.9);
%label.bot(btex $4x+y\le 28$ etex, (0,0)) rotated -75 shifted 0.5[z30,z40];

z51 = (0,-20);
z52 = (0,200);
z53 = z51 shifted (10,0);
z54 = z52 shifted (10,0);
%fill z51--z52--z54--z53--cycle withcolor (0.9,0.9,0.9);
%label.bot(btex $x\ge 0$ etex, (0,0)) rotated 90 shifted 0.5[z0,z10];

z61 = (-20,0);
z62 = (300,0);
z63 = z61 shifted (0,10);
z64 = z62 shifted (0,10);
%fill z61--z62--z64--z63--cycle withcolor (0.9,0.9,0.9);
%label.top(btex $y\ge 0$ etex, 0.5[z0,z40]);

% draw dots for the points
pickup pencircle scaled 4pt;
draw z10--z10;
draw z20--z20;
draw z30--z30;
draw z40--z40;

% draw lines
pickup pencircle scaled 0.5pt;
draw z11--z12;
draw z21--z22;
draw z31--z32;

% coordinate axes
pickup pencircle scaled 0.5pt
draw (-20,0)--(300,0);
draw (0,-20)--(0, 200);

draw (40, -2)--(40,2);	label.bot(btex $1$ etex, (40,-2));
draw (80, -2)--(80,2);	label.bot(btex $2$ etex, (80,-2));
draw (120, -2)--(120,2);	label.bot(btex $3$ etex, (120,-2));
draw (160, -2)--(160,2);	label.bot(btex $4$ etex, (160,-2));
draw (200, -2)--(200,2);	label.bot(btex $5$ etex, (200,-2));
draw (240, -2)--(240,2);	label.bot(btex $6$ etex, (240,-2));

draw (-2,40)--(2,40);	label.lft(btex $1$ etex, (-2,40));
draw (-2,80)--(2,80);	label.lft(btex $2$ etex, (-2,80));
draw (-2,120)--(2,120);	label.lft(btex $3$ etex, (-2,120));

label(btex $G$ etex, (120,100));

pickup pencircle scaled 5pt;
draw z0--z0 withcolor(1,0,0);
draw z10--z10 withcolor(1,0,0);
draw z20--z20 withcolor(1,0,0);
draw z30--z30 withcolor(1,0,0);
draw z40--z40 withcolor(1,0,0);

pickup pencircle scaled 1pt;
label.urt(btex $x=0,y=0$ etex, z0) withcolor (1,0,0);
label.lrt(btex $x=0,s_1=0$ etex, z10) withcolor (1,0,0);
label.top(btex $s_1=0,s_2=0$ etex, z20) withcolor (1,0,0);
label.urt(btex $s_2=0,s_3=0$ etex, z30) withcolor (1,0,0);
label.ulft(btex $y=0,s_3=0$ etex, z40 shifted(-3,0)) withcolor (1,0,0);

endfig;

end
