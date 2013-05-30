/******************************************************************************
* Projekt:     MathSem - GPS-Optimierung  
* Dateiname:   diffFunk.h    
* Created on:  14-April-2013  
* Author:      Tmendez, Cschmid        
*
* Aenderungen:
* 2013-04-14  Tmendez
*             Funktionen der 1. und 2. Ableitungen und Zielfunktion geschrieben
* 2013-04-28  Tmendez
*             Erweiterung mit Gewichtungsfoktoren c
******************************************************************************/

#ifndef DIFFFUNK_H_
#define DIFFFUNK_H_

//Inputparameter:
//************************************
//Satelitendaten: xi,yi,zi,ti
//Punkt: p[x,y,z,t]
//Anzahl Sateliten: n
//Standardabweichung der Sateliten: c
//************************************

//Zielfunktion zf
double zf(const double* xi, const double* yi, const double* zi, const double* ti, const double* p, const int n, const double* c);

//1.Ableitungen
double fx(const double* xi, const double* yi, const double* zi, const double* ti, const double* p, const int n, const double* c);
double fy(const double* xi, const double* yi, const double* zi, const double* ti, const double* p, const int n, const double* c);
double fz(const double* xi, const double* yi, const double* zi, const double* ti, const double* p, const int n, const double* c);
double ft(const double* xi, const double* yi, const double* zi, const double* ti, const double* p, const int n, const double* c);

//2.Ableitngen
double fxx(const double* xi, const double* yi, const double* zi, const double* ti, const double* p, const int n, const double* c);
double fyy(const double* xi, const double* yi, const double* zi, const double* ti, const double* p, const int n, const double* c);
double fzz(const double* xi, const double* yi, const double* zi, const double* ti, const double* p, const int n, const double* c);
double ftt(const double* xi, const double* yi, const double* zi, const double* ti, const double* p, const int n, const double* c);
double fxy(const double* xi, const double* yi, const double* zi, const double* ti, const double* p, const int n, const double* c);
double fxz(const double* xi, const double* yi, const double* zi, const double* ti, const double* p, const int n, const double* c);
double fxt(const double* xi, const double* yi, const double* zi, const double* ti, const double* p, const int n, const double* c);
double fyz(const double* xi, const double* yi, const double* zi, const double* ti, const double* p, const int n, const double* c);
double fyt(const double* xi, const double* yi, const double* zi, const double* ti, const double* p, const int n, const double* c);
double fzt(const double* xi, const double* yi, const double* zi, const double* ti, const double* p, const int n, const double* c);

#endif /*DIFFFUNK_H_*/
