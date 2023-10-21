#
# Makefile
#
# (c) 2023 Prof Dr Andreas Müller
#
all:	front.jpg

cover.png:	cover.pdf
	convert -density 300 cover.pdf cover.png

front.png:	cover.png Makefile
	convert -extract 2063x2717+2200+0 cover.png front.png

front.jpg:	front.png
	convert front.png -scale 50% front.jpg

