#
# Makefile zum Skript ueber Optimierung
#
# (c) 2013 Prof Dr Andreas Mueller, Hochschule Rapperswil
#

DOWNHILLFILES = \
	downhill/algorithm.tex downhill/basics.tex downhill/bib.tex \
	downhill/downhill.tex downhill/examples.tex downhill/introduction.tex \
	downhill/komp.tex downhill/kon1.tex downhill/kon2.tex \
	downhill/reflexion.tex downhill/start.tex downhill/streckung.tex \
	downhill/titlepage.tex downhill/variations.tex

TEXFILES = lp.tex uebersicht.tex nlp.tex variation.tex \
	add/simpleximpl.tex innerepunkte.tex sa/sa.tex gps/gps.tex \
	$(DOWNHILLFILES)

optimierung.pdf:	optimierung.tex $(TEXFILES) optimierung.ind 
	pdflatex optimierung.tex

optimierung.ind:	optimierung.tex $(TEXFILES)
	touch optimierung.ind
	pdflatex optimierung.tex
	makeindex optimierung.idx
