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

z0 = (0,0);
z1 = (280,0);
z2 = (0,200);
z3 = (200,0);
z20 = 0.5 * z2;
z30 = 0.5 * z3;

pickup pencircle scaled 2pt;
draw (-0.1)[z2,z3]--(1.1)[z2,z3] withcolor(1,0,0);
draw (-0.2)[z20,z30]--(1.2)[z20,z30] withcolor(1,0,0);

pickup pencircle scaled 1pt;
drawarrow (-0.03)[z0,z1]--(1.03)[z0,z1];
drawarrow (-0.05)[z0,z2]--(1.05)[z0,z2];

draw (z3 shifted (0,-3))--(z3 shifted (0,3));
draw (z2 shifted (-3,0))--(z2 shifted (3,0));
label.llft(btex $1$ etex, z3);
label.llft(btex $1$ etex, z2);
draw (-0.1)[z2,(0.5 * z3)]--(1.1)[z2,z30];

pickup pencircle scaled 4pt;
draw 0.5 * z3;
draw z2;

pickup pencircle scaled 2pt;
draw z2--(0.5 * z3);

label.top(btex $Z=x+y=1$ etex, (0,0)) rotated -45 shifted 0.5[z2,z3]
	withcolor (1,0,0);
label.bot(btex $Z=x+y=0.5$ etex, (0,0)) rotated -45 shifted 0.5[z20,z30]
	withcolor (1,0,0);

label.bot(btex $2x+y=1$ etex, (0,0)) rotated -63.43 shifted 0.5[z2,z30];

label.urt(btex $y$ etex, z2) shifted (3,1);
label.urt(btex $x$ etex, z1) shifted (-1,3);

endfig;

end
