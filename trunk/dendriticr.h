/***************************************************************************
                          dendriticr.h  -  description
                             -------------------
    copyright            : (C) 2005 by Peter Drysdale
    email                : peter@physics.usyd.edu.au
 ***************************************************************************/

#ifndef DENDRITICR_H
#define DENDRITICR_H

#include<math.h>
#include<fstream>
using std::ofstream;
#include<iostream>
#include"istrm.h"

class DendriticR {
public: 
  DendriticR(long nodes);
  ~DendriticR();
  void init(Istrm& inputf, double& Vinit,int propindex,int qindex);
  void dump(ofstream& dumpf);
  void restart(Istrm& restartf);
  void stepVab(double *Pab, double * Vab, double *dVabdt, double timestep);
private:
  DendriticR(DendriticR& ); // no copy constructor
  const long nodes;
  double* previousPab; // Holds Pab from one time step in the past 
  double alpha; // Object which initializes and get alpha values
  double beta; // Object which initializes and gets beta values
};

#endif
