#
# Makefile zum Skript ueber Optimierung
#
# (c) 2013 Prof Dr Andreas Mueller, Hochschule Rapperswil
#

DOWNHILLFILES = \
	downhill/algorithm.tex downhill/basics.tex \
	downhill/downhill.tex downhill/examples.tex downhill/introduction.tex \
	downhill/komp.tex downhill/kon1.tex downhill/kon2.tex \
	downhill/reflexion.tex downhill/start.tex downhill/streckung.tex \
	downhill/variations.tex downhill/bib.bib

PARTIKELSCHWARMFILES = \
	partikelschwarm/anwendungen.tex partikelschwarm/code.tex \
	partikelschwarm/einfuehrung.tex partikelschwarm/gbest.tex \
	partikelschwarm/geschwindigkeit.tex partikelschwarm/hs-pso.tex \
	partikelschwarm/lbest-neumann.tex partikelschwarm/lbest-ring.tex \
	partikelschwarm/lbest-wheel.tex partikelschwarm/max-velocity.tex \
	partikelschwarm/new-velocity.tex \
	partikelschwarm/Partikelschwarmalgorithmus.tex \
	partikelschwarm/Schwarmalgorithmus.tex \
	partikelschwarm/stopbedingungen.tex partikelschwarm/topologien.tex \
	partikelschwarm/literature.bib

TEXFILES = lp.tex uebersicht.tex nlp.tex variation.tex \
	add/simpleximpl.tex \
	innerepunkte/innerepunkte.tex innerepunkte/bib.bib \
	innerepunkte.tex \
	sa/sa.tex \
	gps/gps.tex gps/bib.bib \
	$(PARTIKELSCHWARMFILES) \
	$(DOWNHILLFILES)

optimierung.pdf:	optimierung.tex $(TEXFILES) optimierung.ind \
	optimierung1-blx.bbl optimierung2-blx.bbl optimierung3-blx.bbl \
	optimierung4-blx.bbl
	pdflatex optimierung.tex

optimierung1-blx.bbl: optimierung1-blx.aux
	bibtex optimierung1-blx

optimierung2-blx.bbl: optimierung2-blx.aux
	bibtex optimierung2-blx

optimierung3-blx.bbl: optimierung3-blx.aux
	bibtex optimierung3-blx

optimierung4-blx.bbl: optimierung4-blx.aux
	bibtex optimierung4-blx

optimierung.ind:	optimierung.tex $(TEXFILES)
	touch optimierung.ind
	pdflatex optimierung.tex
	makeindex optimierung.idx
