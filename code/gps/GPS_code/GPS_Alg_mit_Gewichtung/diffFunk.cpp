/******************************************************************************
* Projekt:     MathSem - GPS-Optimierung  
* Dateiname:   diffFunk.cpp   
* Created on:  14-April-2013  
* Author:      Tmendez, Cschmid        
*
* Aenderungen:
* 2013-04-14  Tmendez
*             Funktionen der 1. und 2. Ableitungen und Zielfunktion geschrieben
* 2013-04-28  Tmendez
*             Erweiterung mit Gewichtungsfoktoren c
******************************************************************************/

#include "diffFunk.h"

const int c2 = (1*1);

//****************************************************************************
//Zielfunktion zf
double zf(const double* xi, const double* yi, const double* zi, const double* ti, const double* p, const int n, const double* c)
{
  double f = 0;
  int j;

  for(j=0; j<n; j++)
  {
    f = f + (1/(c[j]*c[j])) * ((xi[j]-p[0])*(xi[j]-p[0]) + (yi[j]-p[1])*(yi[j]-p[1]) + (zi[j]-p[2])*(zi[j]-p[2]) - c2*(p[3]-ti[j])*(p[3]-ti[j])) 
                            * ((xi[j]-p[0])*(xi[j]-p[0]) + (yi[j]-p[1])*(yi[j]-p[1]) + (zi[j]-p[2])*(zi[j]-p[2]) - c2*(p[3]-ti[j])*(p[3]-ti[j]));
  }
  return f;
}


//****************************************************************************
//1.Ableitng fx
double fx(const double* xi, const double* yi, const double* zi, const double* ti, const double* p, const int n, const double* c)
{
  double f = 0;
  int j;

  for(j=0; j<n; j++)
  {
    f = f + (1/(c[j]*c[j])) * (xi[j]-p[0]) * ((xi[j]-p[0])*(xi[j]-p[0]) + (yi[j]-p[1])*(yi[j]-p[1]) + (zi[j]-p[2])*(zi[j]-p[2]) - c2*(p[3]-ti[j])*(p[3]-ti[j]));
  }
  return -4*f;
}

//1.Ableitng fy
double fy(const double* xi, const double* yi, const double* zi, const double* ti, const double* p, const int n, const double* c)
{
  double f = 0;
  int j;

  for(j=0; j<n; j++)
  {
    f = f + (1/(c[j]*c[j])) * (yi[j]-p[1]) * ((xi[j]-p[0])*(xi[j]-p[0]) + (yi[j]-p[1])*(yi[j]-p[1]) + (zi[j]-p[2])*(zi[j]-p[2]) - c2*(p[3]-ti[j])*(p[3]-ti[j]));
  }
  return -4*f;
}

//1.Ableitng fz
double fz(const double* xi, const double* yi, const double* zi, const double* ti, const double* p, const int n, const double* c)
{
  double f = 0;
  int j;

  for(j=0; j<n; j++)
  {
    f = f + (1/(c[j]*c[j])) * (zi[j]-p[2]) * ((xi[j]-p[0])*(xi[j]-p[0]) + (yi[j]-p[1])*(yi[j]-p[1]) + (zi[j]-p[2])*(zi[j]-p[2]) - c2*(p[3]-ti[j])*(p[3]-ti[j]));
  }
  return -4*f;
}

//1.Ableitng ft
double ft(const double* xi, const double* yi, const double* zi, const double* ti, const double* p, const int n, const double* c)
{
  double f = 0;
  int j;

  for(j=0; j<n; j++)
  {
    f = f + (1/(c[j]*c[j])) * (p[3]-ti[j]) * ((xi[j]-p[0])*(xi[j]-p[0]) + (yi[j]-p[1])*(yi[j]-p[1]) + (zi[j]-p[2])*(zi[j]-p[2]) - c2*(p[3]-ti[j])*(p[3]-ti[j]));
  }
  return -4*c2*f;
}

//****************************************************************************
//2.Ableitng fxx
double fxx(const double* xi, const double* yi, const double* zi, const double* ti, const double* p, const int n, const double* c)
{
  double f = 0;
  int j;

  for(j=0; j<n; j++)
  {
    f = f + (1/(c[j]*c[j])) * (3*(xi[j]-p[0])*(xi[j]-p[0]) + (yi[j]-p[1])*(yi[j]-p[1]) + (zi[j]-p[2])*(zi[j]-p[2]) - c2*(p[3]-ti[j])*(p[3]-ti[j]));
  }
  return 4*f;
}

//2.Ableitng fyy
double fyy(const double* xi, const double* yi, const double* zi, const double* ti, const double* p, const int n, const double* c)
{
  double f = 0;
  int j;

  for(j=0; j<n; j++)
  {
    f = f + (1/(c[j]*c[j])) * ((xi[j]-p[0])*(xi[j]-p[0]) + 3*(yi[j]-p[1])*(yi[j]-p[1]) + (zi[j]-p[2])*(zi[j]-p[2]) - c2*(p[3]-ti[j])*(p[3]-ti[j]));
  }
  return 4*f;
}

//2.Ableitng fzz
double fzz(const double* xi, const double* yi, const double* zi, const double* ti, const double* p, const int n, const double* c)
{
  double f = 0;
  int j;

  for(j=0; j<n; j++)
  {
    f = f + (1/(c[j]*c[j])) * ((xi[j]-p[0])*(xi[j]-p[0]) + (yi[j]-p[1])*(yi[j]-p[1]) + 3*(zi[j]-p[2])*(zi[j]-p[2]) - c2*(p[3]-ti[j])*(p[3]-ti[j]));
  }
  return 4*f;
}

//2.Ableitng ftt
double ftt(const double* xi, const double* yi, const double* zi, const double* ti, const double* p, const int n, const double* c)
{
  double f = 0;
  int j;

  for(j=0; j<n; j++)
  {
    f = f + (1/(c[j]*c[j])) * ((xi[j]-p[0])*(xi[j]-p[0]) + (yi[j]-p[1])*(yi[j]-p[1]) + (zi[j]-p[2])*(zi[j]-p[2]) - 3*c2*(p[3]-ti[j])*(p[3]-ti[j]));
  }
  return -4*c2*f;
}

//2.Ableitng fxy
double fxy(const double* xi, const double* yi, const double* zi, const double* ti, const double* p, const int n, const double* c)
{
  double f = 0;
  int j;

  for(j=0; j<n; j++)
  {
    f = f + (1/(c[j]*c[j]))*(xi[j]-p[0])*(yi[j]-p[1]);
  }
  return 8*f;
}

//2.Ableitng fxz
double fxz(const double* xi, const double* yi, const double* zi, const double* ti, const double* p, const int n, const double* c)
{
  double f = 0;
  int j;

  for(j=0; j<n; j++)
  {
    f = f + (1/(c[j]*c[j]))*(xi[j]-p[0])*(zi[j]-p[2]);
  }
  return 8*f;
}

//2.Ableitng fxt
double fxt(const double* xi, const double* yi, const double* zi, const double* ti, const double* p, const int n, const double* c)
{
  double f = 0;
  int j;

  for(j=0; j<n; j++)
  {
    f = f + (1/(c[j]*c[j]))*(xi[j]-p[0])*(p[3]-ti[j]);
  }
  return 8*c2*f;
}

//2.Ableitng fyz
double fyz(const double* xi, const double* yi, const double* zi, const double* ti, const double* p, const int n, const double* c)
{
  double f = 0;
  int j;

  for(j=0; j<n; j++)
  {
    f = f + (1/(c[j]*c[j]))*(yi[j]-p[1])*(zi[j]-p[2]);
  }
  return 8*f;
}

//2.Ableitng fyt
double fyt(const double* xi, const double* yi, const double* zi, const double* ti, const double* p, const int n, const double* c)
{
  double f = 0;
  int j;

  for(j=0; j<n; j++)
  {
    f = f + (1/(c[j]*c[j]))*(yi[j]-p[1])*(p[3]-ti[j]);
  }
  return 8*c2*f;
}

//2.Ableitng fzt
double fzt(const double* xi, const double* yi, const double* zi, const double* ti, const double* p, const int n, const double* c)
{
  double f = 0;
  int j;

  for(j=0; j<n; j++)
  {
    f = f + (1/(c[j]*c[j]))*(zi[j]-p[2])*(p[3]-ti[j]);
  }
  return 8*c2*f;
}
