\verbatimtex
\documentclass{article}
\usepackage{times}
\usepackage{amsmath}
\usepackage{amssymb}
\usepackage{amsfonts}
\begin{document}
etex;

beginfig(1)

pickup pencircle scaled 1pt;

draw (-205,0)--(205,0);
draw (0,-145)--(0,145);

z0 = (0,0);
z1 = (100, -100);
z2 = (-100,100);
z3 = (200, 100);
z4 = (-200, -100);
z5 = (-50, 68.75);
z6 = (50, -68.75);

pickup pencircle scaled 2pt;
draw z4{dir 83.66}..{dir 0}z2..z5..{dir -71.565}z0..z6..z1..z3;

endfig;

beginfig(2)

pickup pencircle scaled 0.5pt;

draw (-10,0)--(310,0);	label.lft(btex $0$ etex, (-10,0));
draw (-10,50)--(310,50);	label.lft(btex $\frac{\pi}2$ etex, (-10,50));
draw (-10,100)--(310,100);	label.lft(btex $\pi$ etex, (-10,100));
draw (-10,150)--(310,150);	label.lft(btex $\frac{\pi}2+\pi$ etex, (-10,150));
draw (-10,200)--(310,200);	label.lft(btex $2\pi$ etex, (-10,200));

draw (0,-10)--(0,210);	label.bot(btex $0$ etex, (0,-10));
draw (50,-10)--(50,210);	label.bot(btex $\frac{\pi}2$ etex, (50,-10));
draw (100,-10)--(100,210);	label.bot(btex $\pi$ etex, (100,-10));
draw (150,-10)--(150,210);	label.bot(btex $\frac{\pi}2+\pi$ etex, (150,-10));
draw (200,-10)--(200,210);	label.bot(btex $2\pi$ etex, (200,-10));
draw (250,-10)--(250,210);	label.bot(btex $\frac{\pi}2+2\pi$ etex, (250,-10));
draw (300,-10)--(300,210);	label.bot(btex $3\pi$ etex, (300,-10));


for x = 0 step 100 until 300:
	for y = 0 step 100 until 200:
		pickup pencircle scaled 5pt;
		if 0 = (((x + y) / 100) mod 2):
			draw (x,y)--(x,y) withcolor(1,0,0);
		else:
			draw (x,y)--(x,y) withcolor(0,0,1);
		fi;
		pickup pencircle scaled 3pt;
		draw (x,y)--(x,y) withcolor(1,1,1);
	endfor;
endfor;

for x = 50 step 100 until 250:
	for y = 50 step 100 until 150:
		pickup pencircle scaled 5pt;
		draw (x,y)--(x,y);
		pickup pencircle scaled 3pt;
		draw (x,y)--(x,y) withcolor(1,1,1);
	endfor;
endfor;

endfig;

%
% Illustration zu Lagrange Multiplikatoren
%
beginfig(3)

z0 = (0,0);
z1 = (100,0);
z2 = (120,-60);

z10 = (20,60);
z12 = (180,-20);

path p;
p = z10..{dir -30}z1{dir -30}..z12;

pickup pencircle scaled 2pt;
draw z0..{dir -30}z1{dir -30}..z2;
label.bot(btex $g(x)=0$ etex, z0);

pickup pencircle scaled 1pt;
draw p withcolor (1,0,0);

for s = -2 step 1 until 2:
	draw (p shifted (10 * s, 15 * s));
endfor;
label.rt(btex $f(x)=0$ etex, z12);
label.rt(btex $f(x)=-2$ etex, z12 shifted (-20,-30));
label.rt(btex $f(x)=2$ etex, z12 shifted (20,30));

pickup pencircle scaled 4pt;
draw z1 withcolor (1,0,0);
label.rt(btex $x_*$ etex, z1 shifted (1,2)) withcolor (1,0,0);

pickup pencircle scaled 1pt;
z20 = z1 shifted (40 * cosd(60), 40 * sind(60));
drawarrow z1--z20 withcolor(1,0,0);
label.urt(btex $\operatorname{grad}f$ etex, z20) withcolor (1,0,0);
z21 = z1 shifted (-30 * cosd(60), -30 * sind(60));
drawarrow z1--z21 withcolor(1,0,0);
label.llft(btex $\operatorname{grad}g$ etex, z21) withcolor (1,0,0);

pickup pencircle scaled 0.5pt;

z22 = z1 shifted (-80 * sind(60), 80 * cosd(60));
z23 = z1 shifted ( 80 * sind(60),-80 * cosd(60));
draw z22--z23 withcolor (1,0,0);

endfig;

%
% Illustration Extrema am Rande 
%
beginfig(4)

pickup pencircle scaled 0.5pt;

drawarrow (-110,0)--(210,0); 	label.top(btex $x$ etex, (200,0));
drawarrow (0,-10)--(0,210); 	label.rt(btex $y$ etex, (0,200));

z0 = (-100,100);
z1 = (-30,30);
z2 = (40,120);
z3 = (110,70);
z4 = (180,200);

pickup pencircle scaled 0.1pt;
draw z0--(-100,0);
draw z4--(180,0);

pickup pencircle scaled 0.5pt;
draw (-100,-2)--(-100,2);	label.bot(btex $a\mathstrut$ etex, (-100,-2));
draw ( 180,-2)--( 180,2);	label.bot(btex $b\mathstrut$ etex, ( 180,-2));

pickup pencircle scaled 1.5pt;

draw z0{dir -45}..{dir 0}z1{dir 0}..{dir 0}z2{dir 0}..{dir 0}z3{dir 0}..{dir 45}z4;

pickup pencircle scaled 1pt;
draw (z4 shifted (-20,-20))--(z4 shifted (10,10)) withcolor (1,0,0);
draw (z0 shifted (-10,10))--(z0 shifted (20,-20)) withcolor (1,0,0);

draw (z1 shifted (-15,0))--(z1 shifted (15,0)) withcolor (1,0,0);
draw (z2 shifted (-15,0))--(z2 shifted (15,0)) withcolor (1,0,0);
draw (z3 shifted (-15,0))--(z3 shifted (15,0)) withcolor (1,0,0);

pickup pencircle scaled 3pt;
draw z0--z0 withcolor (1,0,0);
draw z1--z1 withcolor (1,0,0);
draw z2--z2 withcolor (1,0,0);
draw z3--z3 withcolor (1,0,0);
draw z4--z4 withcolor (1,0,0);

label.llft(btex $P_0$ etex, z0);
label.top(btex $P_1$ etex, z1);
label.bot(btex $P_2$ etex, z2);
label.top(btex $P_3$ etex, z3);
label.lrt(btex $P_4$ etex, z4);

label.ulft(btex $f'(x) > 0$ etex, z4) withcolor(1,0,0);
label.urt(btex $f'(x) < 0$ etex, z0) withcolor(1,0,0);

label.bot(btex $f'(x) = 0, f''(x) > 0$ etex, z1) withcolor(1,0,0);
label.top(btex $f'(x) = 0, f''(x) < 0$ etex, z2) withcolor(1,0,0);
label.bot(btex $f'(x) = 0, f''(x) > 0$ etex, z3) withcolor(1,0,0);

endfig;

%
%
%
beginfig(5)

z0 = (0,0);
z1 = z0 shifted (100 * (cosd(30), sind(30)));
z2 = z0 shifted (100 * (cosd(150), sind(150)));
z3 = z0 shifted (100 * (cosd(-90), sind(-90)));

z12 = z1 shifted (15 * (cosd(-10), sind(-10)));
z21 = z2 shifted (-15 * (cosd(10), sind(10)));

z13 = z1 shifted (15 * (cosd(70), sind(70)));
z31 = z3 shifted (15 * (cosd(-130), sind(-130)));

z23 = z2 shifted (15 * (cosd(110), sind(110)));
z32 = z3 shifted (15 * (cosd(-50), sind(-50)));

pickup pencircle scaled 1pt;

path p;
p = z12..z1..z2..z21;
z5 = point 1.5 of p;

path r;
r = z23..z2..z3..z32;

path q;
q = z31..z3..z1..z13;

fill (subpath (1,2) of p)--(subpath (1,2) of r)--(subpath (1,2) of q)--cycle withcolor(0.95,0.95,0.95);

fill p--(reverse p shifted (0,-15))--cycle withcolor (0.9,0.9,0.9);
fill q--(reverse q shifted (-15 * (cosd(-30), sind(-30))))--cycle withcolor (0.9,0.9,0.9);
fill r--(reverse r shifted (15 * (cosd(30), sind(30))))--cycle withcolor (0.9,0.9,0.9);

draw p;
draw r;
draw q;

z40 = (20,100);
z41 = (0,0);
z42 = (-20,-100);
path s;
s = z40{dir -70}..{dir -135}z41{dir -135}..{dir -70}z42;
pickup pencircle scaled 0.5pt;
for shift = -100 step 40 until 100:
	draw s shifted (z2 * (shift / 100));
endfor

pickup pencircle scaled 1pt;
draw s shifted z2 withcolor (1,0,0);
draw s shifted (-0.5342 * z2) withcolor (1,0,0);
z43 = q intersectionpoint  (s shifted (-0.5342 * z2));
draw s shifted (-0.6 * z2) withcolor (1,0,0);
z44 = q intersectionpoint  (reverse s shifted (-0.595 * z2));

draw s shifted (-0.724 * z2) withcolor (1,0,0);

pickup pencircle scaled 4pt;

draw z3;
draw z2 withcolor (1,0,0);
draw z1 withcolor (1,0,0);

draw z43 withcolor (1,0,0);
draw z44 withcolor (1,0,0);

label(btex $G$ etex , z0);

label.bot(btex $g_1(x) \le 0$ etex, point 1.5 of p);
label.top(btex $g_2(x) \le 0$ etex, z0) rotated -60 shifted point 1.5 of r;
label.top(btex $g_3(x) \le 0$ etex, z0) rotated 60 shifted point 1.5 of q;

clip currentpicture to (119,85)--(119,-120)--(-119,-120)--(-119,85)--cycle;

endfig;

%
% Karush-Kuhn-Tucker Bedingungen
%
beginfig(6)

z0 = (0,0);
z1 = (100,0);
z2 = (100,100);
z3 = (0,100);

z23 = z2 shifted (10 * (cosd(-10), sind(-10)));
z21 = z2 shifted (10 * (cosd(100), sind(100)));

path p, q;
p = z1..z2..z21;
q = z3..z2..z23;

fill z0--(subpath (0,1) of p)--(subpath (1,2) of reverse q)--cycle withcolor (0.95,0.95,0.95);

pickup pencircle scaled 1pt;

z32 = point 0.5 of q;
z34 = z32 shifted (0,35);
label.bot(btex $h_1(x)\le 0$ etex, z32);
drawarrow z32--z34;
label.top(btex $\operatorname{grad} h_1(x)$ etex, z34);
z33 = point 0.5 of p;
z35 = z33 shifted (35,0);
label.top(btex $h_2(x)\le 0$ etex, z0) rotated 90 shifted z33;
drawarrow z33--z35;
label.top(btex $\operatorname{grad}h_2(x)$ etex, z35);

draw p;
draw q;

label(btex $G$ etex, (50,50));

z30 = z2 shifted (50 * (cosd(10), sind(10)));
z31 = z2 shifted (50 * (cosd(80), sind(80)));

fill z2--z30{dir 100}..{dir 170}z31--z2--cycle withcolor(0.95,0.95,0.95);

drawarrow z2--0.7[z2,z30];
drawarrow z2--0.7[z2,z31];

pickup pencircle scaled 3pt;

draw z33;
draw z32;

pickup pencircle scaled 0.5pt;

path r;
r = (-50,150)..{dir -25}z2..(230,20);

for shift = -120 step 40 until 80:
	draw r shifted (shift * (cosd(65), sind(65)));
endfor

pickup pencircle scaled 1pt;
draw r withcolor (1,0,0);

z24 = z2 shifted (45 * (cosd(65), sind(65)));
drawarrow z2--z24 withcolor(1,0,0);

label.bot(btex $\operatorname{grad}f(x_*)$ etex, z0) rotated 65 shifted z24 withcolor (1,0,0);

pickup pencircle scaled 4pt;
draw z2 withcolor (1,0,0);
label.llft(btex $x_*$ etex, z2) withcolor (1,0,0);

clip currentpicture to (0,0)--(200,0)--(200,160)--(0,160)--cycle;

endfig;

%
% Newton-Verfahren
%
beginfig(7)

pickup pencircle scaled 0.5pt;

z0 = (0,0);
z1 = (200,0);
z2 = (0,-20);
z3 = (0,120);

drawarrow (-0.05)[z0,z1]--1.05[z0,z1];
drawarrow (-0.05)[z2,z3]--1.05[z2,z3];

label.lft(btex $y$ etex, z3);
label.top(btex $x$ etex, z1);

z11 = (0,-20);
z12 = (70,0);
z13 = (140,40);
z14 = (200,120);


pickup pencircle scaled 1pt;

path p;
p = z11..z12..z13..z14;
draw p;
label.ulft(btex $y=f(x)$ etex, point 2.5 of p);
z20 = direction 2 of p;

pickup pencircle scaled 0.2pt;
x15 = x13; y15 = 0;
y16 = y13; x16 = 0;
draw z13--z15;
label.bot(btex $x_k$ etex, z15);
draw z13--z16;
label.lft(btex $f(x_k)$ etex, z16);

z17 = z13 + (t * z20) = z0 + (u * (z1 - z0));

draw (-1)[z13,z17]--1.5[z13,z17];

label.lrt(btex $x_{k+1}$ etex, z17);

label.lrt(btex $f'(x_k)$ etex, (-0.3)[z13,z17]);

pickup pencircle scaled 4pt;

draw z15;
draw z13;
draw z17;

endfig;

end
