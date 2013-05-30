/******************************************************************************
* Projekt:     MathSem - GPS-Optimierung  
* Dateiname:   gps.cpp    
* Created on:  22-Maerz-2013  
* Author:      Tmendez, Cschmid        
*
* Aenderungen:
* 2013-03-33  Tmendez
*             Matrixinitialisierung, ersten Mathematikcode geschrieben
* 2013-03-30  Cschmid
*             Umbau auf C++, dynamischer Anzahl Satelliten,
*             automatisierte Ein- und Ausgabe via Files
*             Nutzung: ./gps.sh < InputFile > OutputFile
* 2013-04-14  Tmendez
*             "diffFunk.h" und diffFunk.cpp" implementiert
*             Matrix für Gausalgorithmus und Hessesche Matrix
* 2013-04-19  Tmendez und Cschmid
*             Ergänzungen mathematische Berechnung (Matrizen etc.)
*             Einbindung Lapack-Funktionen
* 2013-04-21  Tmendez
*             Implementation des Optimierungsalgorithmus
* 2013-04-26  Tmendez
*             Berechung der Eigenwerte und Fallunterscheidung nach Extremaltyp
******************************************************************************/
#include <iostream>
#include <iomanip>
#include <math.h>
#include <assert.h>
#include "../lapack/f2c.h"
#include "../lapack/clapack.h"
#include "diffFunk.h"

using namespace std;

const int dim = 4;

//Dimension der Matrix(4x4), Anzahl Spalten rechte Seite (1), Pointer auf Matix (G), Anzahl Zeilen von Matrix (4), Speicherplatz um Pivot zu speichern, rechte-Seite-Vektor (b), Anzahl Zeilen von rechte Seite Vektor (4)
extern "C" int dgesv_(int* n, int* nrhs, double* a, int* lda, int* ipiv, double* b, int* ldb, int* info);

//Eigenwete 'N', lower triangle, 4x4 Matrix, Matrix A, Anzahl Zeilen, Eigenwerte, Arbeitsspeicher, Anzahl Zeilen, info
extern "C" int dsyev_(char* jobz, char* uplo, int* n, double* a, int* lda, double* w, double* work, int* lwork, int* info);

void matGausAlg(double* M, double* xi, double* yi, double* zi, double* ti, double* p, int n);
void vecBGausAlg(double* M, double* xi, double* yi, double* zi, double* ti, double* p, int n);
void matHess(double* M, double* xi, double* yi, double* zi, double* ti, double* p, int n);

int main(void)
{
  // Eigene Position festlegen (!to change!):
  double x0 = 21.213203436;
  double y0 = 21.213203436;
  double z0 = 40;
  double t0 = 300;
  //double p0[]={x0,y0,z0,t0};
  int erdradius = 50;
  int satelitenradius = 215;

  //Zählvariabeln:
  int i = 0;
  int j = 0;
  int k = 0;
  int l = 0;
  int m = 0;

  //Lapack-parameter  
  char job = 'V';
  char lo = 'L';
  int dimMat = dim;
  int dimVec = 1;
  int pivot[dim];
  double eigenwerte[dim];
  double work[dim*dim];
  int lwork = (dim*dim);
  int info;
  
  //Extremaltypen und Schupf bei Sattelpunkt
  int exTyp = 0;
  int schupf = 2; 

  //Matrix und rechte Seite Vektor für Gaus(Newton):
  double G[dim*dim];
  double b[dim];

  //Hessesche Matrix füllen:
  double H[dim*dim]; 

  //Newtonalgorithmus (!to change!):
  int anzSP = 26;
  double p[] = {50,0,0,300,
                28.8675134595,28.8675134595,28.8675134595,400,
                28.8675134595,28.8675134595,-28.8675134595,200,
                35.3553390593,35.3553390593,0,200,
                35.3553390593,-35.3553390593,0,300,
                -35.3553390593,35.3553390593,0,400,
                -50,0,0,400,
                28.8675134595,-28.8675134595,28.8675134595,100,
                28.8675134595,-28.8675134595,-28.8675134595,300,
                -35.3553390593,-35.3553390593,0,300,
                35.3553390593,0,35.3553390593,200,
                35.3553390593,0,-35.3553390593,300,
                0,50,0,200,
                -28.8675134595,28.8675134595,28.8675134595,500,
                -28.8675134595,28.8675134595,-28.8675134595,200,
                -35.3553390593,0,35.3553390593,400,
                -35.3553390593,0,-35.3553390593,300,
                0,35.3553390593,35.3553390593,200,
                0,-50,0,300,
                -28.8675134595,-28.8675134595,28.8675134595,400,
                -28.8675134595,-28.8675134595,-28.8675134595,300,
                0,35.3553390593,-35.3553390593,300,
                0,-35.3553390593,35.3553390593,400,
                0,-35.3553390593,-35.3553390593,300,
                0,0,-50,200,
                0,0,50,100}; //Startpunkte
  double pn[dim];               //Punkt für Newtonalgorithmus
  int maxAnzIt = 100;           //Maximale Anzahl Durchlaeufe
  double toleranz = 0.001;      //Abbruchgenauigkeit
  double delta = 0;             //Speicher für deltas
  double radius = 0;            //Entfernung des gefundenen Punktes von O
  double radTol = 2;            //Genauigkeit im Erdmantel 

  // Satelitendaten einlesen:
  int anzSat;
  //cout << "Anzahl Sateliten: " << endl;
  cin >> anzSat;      
  double* x = new double [anzSat];
  double* y = new double [anzSat];
  double* z = new double [anzSat];
  double* t = new double [anzSat];

  for(int i = 0; i < anzSat; i++)
  {
    //cout << "Satellit " << i+1 << ": x, y, und z-Koordinate sowie Zeit eingeben" << endl;
    cin >> x[i] >> y[i] >> z[i] >> t[i];
  }

  // Satellitendaten zur Kontrolle ausgeben:
  cout << endl;
  cout << "Satellitendaten:" << endl;
  cout << "                 x-Position:    y-Position:    z-Position:          Zeit:" << endl;
  for(int i = 0; i < anzSat; i++)
  {
    cout << "Satellit " << setw(2) << i+1 << ": ";
    cout << setw(15) << fixed << x[i];
    cout << setw(15) << fixed << y[i];
    cout << setw(15) << fixed << z[i];
    cout << setw(15) << fixed << t[i];
    cout << endl;
  }
  //Abmessungen
  cout << endl << "Modellabmessungen: " << endl;
  cout << "Erdradius:       " << erdradius << endl;
  cout << "Satelitenradius: " << satelitenradius << endl << endl;
  //cout << "Zielfunktion im optimalen Punkt: " << zf(x,y,z,t,p0,anzSat) << endl << endl;

  //Unsere Eigene Position:
  cout << "Unsere exakte Position: " << endl;
  cout << "x: " << setw(3) << fixed << x0 << endl;
  cout << "y: " << setw(3) << fixed << y0 << endl;
  cout << "z: " << setw(3) << fixed << z0 << endl;
  cout << "t: " << setw(3) << fixed << t0 << endl << endl;

  //Abbruch wenn in Erdnaehe oder keine Punkte mehr oder Extremaltyp kein Minimum
  for(k=0; (!((radius>(erdradius-radTol))&&(radius<(erdradius+radTol))&&(exTyp==1)))&&(k<anzSP); k++)
  {  
    l=0;//Zähler für Anzahl druchlaeufe mit Sattelpunkten
    exTyp = 0;    
    while((l<5)&&((exTyp==0)||(exTyp==3)))    
    {  
      i=0; //Anzahl Itterationen
      //Nullpunkt mittels Newton suchen
      do
      { 
        //Berechnung des neuen Punktes
        if((i==0) && (exTyp==0))
        {    
          for(j=0; j<dim; j++)
          {      
            pn[j] = p[k*dim + j];//Neuer Punkt aus Array laden
          }
          cout << "Startpunkt " << k+1 << ":" << endl;
        }
        else if(exTyp==0)
        {
          for(j=0; j<dim; j++)
          {      
            pn[j] = pn[j] + b[j];//zum alten Punkt deltas dazurechen
          }
        }
        else if(exTyp==3)
        {
          exTyp = 0;//wenn Sattelpunkt Punkt bereits generiert
          /*cout << endl;
          cout << "Punkt x: " << endl;
          for(j=0; j<dim; j++)
          {
            cout << pn[j] << endl;
          }*/
        }
        else
        {
          cout << "Error: Im 'else' der Berechung des neuen Punktes gelandet" << endl << endl;
          assert(0); 
        }

        matGausAlg(G,x,y,z,t,pn,anzSat);
        vecBGausAlg(b,x,y,z,t,pn,anzSat);
    
        //Aufruf des Gauss-Algorithmus:
        dgesv_(&dimMat, &dimVec, G, &dimMat, pivot, b, &dimMat, &info);
        assert(info==0);
        
        delta = 0;
        for(j=0; j<dim; j++)    
        {
          delta = delta + b[j]*b[j];
        }

        cout << "delta: " << delta << endl;
        i++;
      }while((delta > toleranz) && (i < maxAnzIt));
      cout << endl;
      
      //Gefundener Punkt
      for(j=0; j<dim; j++)
      {      
        pn[j] = pn[j] + b[j];
      }
      radius = sqrt(pn[0]*pn[0]+pn[1]*pn[1]+pn[2]*pn[2]);
      

      /*cout << endl;
      cout << "gefundener Punkt: " << endl;
      for(j=0; j<dim; j++)
      {
        cout << pn[j] << endl;
      }*/ 

      //Ueberpruefung ob Minimum
      matHess(H,x,y,z,t,pn,anzSat);
      dsyev_(&job, &lo, &dimMat, H, &dimMat, eigenwerte, work, &lwork, &info);
      assert(info == 0);

      /*cout << endl;
      cout << "Eigenwerte: " << endl;
      for(j=0; j<dim; j++)
      {
        cout << eigenwerte[j] << endl;
      }*/

      if((eigenwerte[0]>0) && (eigenwerte[1]>0) && (eigenwerte[2]>0) && (eigenwerte[3]>0))
      {
        exTyp = 1; //Ist ein Minimum
      }
      else if((eigenwerte[0]<0) && (eigenwerte[1]<0) && (eigenwerte[2]<0) && (eigenwerte[3]<0))
      {
        exTyp = 2; //Ist ein Maximum
      }
      else if((eigenwerte[0]==0) || (eigenwerte[1]==0) || (eigenwerte[2]==0) || (eigenwerte[3]==0))
      {
        exTyp = 4; //Unbekannt -> Weitere Abklaerungen waeren noetig
      }
      else
      {
        for(j=0; (j<dim)&&(exTyp!=3); j++)
        {
          if(eigenwerte[j]<0)
          {
            //cout << "Eigenvektor: " << endl;            
            for(m=0; m<dim ;m++)
            {
              if(l<2)              
              {
                pn[m] = pn[m] + schupf*H[j*dim+m];
              }
              else
              {
                pn[m] = pn[m] - schupf*H[j*dim+m];
              }
              //cout << H[j*dim+m] << endl;
            }
            exTyp = 3; //ist ein Sattelpunkt
            l++;
            //cout << endl;
          }
        }
      }     

        //pn[3] = pn[3] + schupf;

        /*for(j=0; (j<dim)&&(exTyp!=3); j++)
        {
          if(eigenwerte[j]<=0)
          {
            pn[j] = pn[j] + 5;
            exTyp = 3; //ist ein Sattelpunkt
            l++;
          }
        }*/
    }
  }
  
  if((radius>(erdradius-radTol))&&(radius<(erdradius+radTol))&&(exTyp==1))
  {
    cout << "Erfolgreicher Startpunkt: " << k << endl;
    cout << "x: " << p[(k-1)*dim + 0] << endl;
    cout << "y: " << p[(k-1)*dim + 1] << endl;
    cout << "z: " << p[(k-1)*dim + 2] << endl;
    cout << "t: " << p[(k-1)*dim + 3] << endl << endl;

    cout << "Berechnete Position: " << endl;
    cout << "x: " << pn[0] << endl;
    cout << "y: " << pn[1] << endl;
    cout << "z: " << pn[2] << endl;
    cout << "t: " << pn[3] << endl << endl;

    cout << "Radius:  " << radius << endl;

    cout << "Zielfunktion: " << zf(x,y,z,t,pn,anzSat) << endl;

    cout << "Abweichung vom exakten Punkt: " << sqrt((pn[0]-x0)*(pn[0]-x0)+(pn[1]-y0)*(pn[1]-y0)+(pn[2]-z0)*(pn[2]-z0)) << endl;
  }
  else
  {
    cout << "Kein Startpunkt war erfolgreich!" << endl;
  }
 /* //Berechnung der Eigenwerte:
  matHess(H,x,y,z,t,pn,anzSat);
  dsyev_(&job, &lo, &dimMat, H, &dimMat, eigenwerte, work, &lwork, &info);

  cout << endl;
  cout << "Eigenwerte: " << endl;
  for(j=0; j<dim; j++)
  {
    cout << eigenwerte[j] << endl;
  }
  cout << "Info: " << info << endl;*/

  // Freigabe der dynamisch erstellten Variablen und Arrays
  delete[] x;
  delete[] y;
  delete[] z;
  delete[] t;

  return 0;
}


void matGausAlg(double* M, double* xi, double* yi, double* zi, double* ti, double* p, int n)
{
  M[0] = fxx(xi,yi,zi,ti,p,n);
  M[1] = fxy(xi,yi,zi,ti,p,n);
  M[2] = fxz(xi,yi,zi,ti,p,n);
  M[3] = fxt(xi,yi,zi,ti,p,n);

  M[4] = M[1];
  M[5] = fyy(xi,yi,zi,ti,p,n);
  M[6] = fyz(xi,yi,zi,ti,p,n);
  M[7] = fyt(xi,yi,zi,ti,p,n);

  M[8] = M[2];
  M[9] = M[6];
  M[10] = fzz(xi,yi,zi,ti,p,n);
  M[11] = fzt(xi,yi,zi,ti,p,n);

  M[12] = M[3];
  M[13] = M[7];
  M[14] = M[11];
  M[15] = ftt(xi,yi,zi,ti,p,n);
}

void vecBGausAlg(double* M, double* xi, double* yi, double* zi, double* ti, double* p, int n)
{
  M[0] = -fx(xi,yi,zi,ti,p,n);
  M[1] = -fy(xi,yi,zi,ti,p,n);
  M[2] = -fz(xi,yi,zi,ti,p,n);
  M[3] = -ft(xi,yi,zi,ti,p,n);
}

void matHess(double* M, double* xi, double* yi, double* zi, double* ti, double* p, int n)
{
  M[0] = fxx(xi,yi,zi,ti,p,n);
  M[1] = fxy(xi,yi,zi,ti,p,n);
  M[2] = fxz(xi,yi,zi,ti,p,n);
  M[3] = fxt(xi,yi,zi,ti,p,n);

  M[4] = M[1];
  M[5] = fyy(xi,yi,zi,ti,p,n);
  M[6] = fyz(xi,yi,zi,ti,p,n);
  M[7] = fyt(xi,yi,zi,ti,p,n);

  M[8] = M[2];
  M[9] = M[6];
  M[10] = fzz(xi,yi,zi,ti,p,n);
  M[11] = fzt(xi,yi,zi,ti,p,n);

  M[12] = M[3];
  M[13] = M[7];
  M[14] = M[11];
  M[15] = ftt(xi,yi,zi,ti,p,n);
}

