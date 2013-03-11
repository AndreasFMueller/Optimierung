#
# Makefile zum Skript ueber Optimierung
#
TEXFILES = lp.tex uebersicht.tex nlp.tex variation.tex \
	add/simpleximpl.tex

optimierung.pdf:	optimierung.tex $(TEXFILES) optimierung.ind 
	pdflatex optimierung.tex

optimierung.ind:	optimierung.tex $(TEXFILES)
	touch optimierung.ind
	pdflatex optimierung.tex
	makeindex optimierung.idx
