/******************************************************************************
* Projekt:     MathSem - GPS-Optimierung  
* Dateiname:   diffFunk.h    
* Created on:  14-April-2013  
* Author:      Tmendez, Cschmid        
*
* Aenderungen:
* 2013-04-14  Tmendez
*             Funktionen der 1. und 2. Ableitungen und Zielfunktion geschrieben
******************************************************************************/

#ifndef DIFFFUNK_H_
#define DIFFFUNK_H_

//Inputparameter:
//************************************
//Satelitendaten: xi,yi,zi,ti
//Punkt: p[x,y,z,t]
//Anzahl Sateliten: n
//************************************

//Zielfunktion zf
double zf(const double* xi, const double* yi, const double* zi, const double* ti, const double* p, const int n);

//1.Ableitungen
double fx(const double* xi, const double* yi, const double* zi, const double* ti, const double* p, const int n);
double fy(const double* xi, const double* yi, const double* zi, const double* ti, const double* p, const int n);
double fz(const double* xi, const double* yi, const double* zi, const double* ti, const double* p, const int n);
double ft(const double* xi, const double* yi, const double* zi, const double* ti, const double* p, const int n);

//2.Ableitngen
double fxx(const double* xi, const double* yi, const double* zi, const double* ti, const double* p, const int n);
double fyy(const double* xi, const double* yi, const double* zi, const double* ti, const double* p, const int n);
double fzz(const double* xi, const double* yi, const double* zi, const double* ti, const double* p, const int n);
double ftt(const double* xi, const double* yi, const double* zi, const double* ti, const double* p, const int n);
double fxy(const double* xi, const double* yi, const double* zi, const double* ti, const double* p, const int n);
double fxz(const double* xi, const double* yi, const double* zi, const double* ti, const double* p, const int n);
double fxt(const double* xi, const double* yi, const double* zi, const double* ti, const double* p, const int n);
double fyz(const double* xi, const double* yi, const double* zi, const double* ti, const double* p, const int n);
double fyt(const double* xi, const double* yi, const double* zi, const double* ti, const double* p, const int n);
double fzt(const double* xi, const double* yi, const double* zi, const double* ti, const double* p, const int n);

#endif /*DIFFFUNK_H_*/
