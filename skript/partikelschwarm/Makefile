#
# Reformat images that don't work directly
#
# (c) 2013 Prof Dr Andreas Mueller, Hochschule Rapperswil
#
all:	gbest-crop.pdf \
	lbest-neumann-crop.pdf lbest-ring-crop.pdf lbest-wheel-crop.pdf \
	hs-pso-crop.pdf

gbest.pdf:	gbest.tex
	pdflatex gbest.tex
gbest-crop.pdf:	gbest.pdf
	pdfcrop gbest.pdf gbest-crop.pdf

lbest-neumann.pdf:	lbest-neumann.tex
	pdflatex lbest-neumann.tex
lbest-neumann-crop.pdf:	lbest-neumann.pdf
	pdfcrop lbest-neumann.pdf lbest-neumann-crop.pdf

lbest-ring.pdf:	lbest-ring.tex
	pdflatex lbest-ring.tex
lbest-ring-crop.pdf:	lbest-ring.pdf
	pdfcrop lbest-ring.pdf lbest-ring-crop.pdf

lbest-wheel.pdf:	lbest-wheel.tex
	pdflatex lbest-wheel.tex
lbest-wheel-crop.pdf:	lbest-wheel.pdf
	pdfcrop lbest-wheel.pdf lbest-wheel-crop.pdf

hs-pso.pdf:	hs-pso.tex
	pdflatex hs-pso.tex
hs-pso-crop.pdf:	hs-pso.pdf
	pdfcrop hs-pso.pdf hs-pso-crop.pdf
