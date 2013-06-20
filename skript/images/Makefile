#
# Makefile to build the images for the optimization script
#
# (c) 2012 Prof Dr Andreas Mueller, Hochschule Rapperswil
# $Id$
#
all:	lp nlp innerepunkte

lp:	lp.mp
	TEX=latex mpost lp.mp
	for f in lp.[0-9]*; \
	do \
		mptopdf $${f}; \
	done

nlp:	nlp.mp
	TEX=latex mpost nlp.mp
	for f in nlp.[0-9]*; \
	do \
		mptopdf $${f}; \
	done

innerepunkte:	innerepunkte.mp
	TEX=latex mpost innerepunkte.mp
	for f in innerepunkte.[0-9]*; \
	do \
		mptopdf $${f}; \
	done
