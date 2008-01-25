/***************************************************************************
                          firingr.h  -  description
                             -------------------
    copyright            : (C) 2005 by Peter Drysdale
    email                : peter@physics.usyd.edu.au
 ***************************************************************************/

#ifndef FIRINGR_H
#define FIRINGR_H

#include<fstream>
using std::ofstream;
#include<iostream>
using std::cerr;
using std::endl;

#include"istrm.h"
#include"parameter.h"

class FiringR {
public: 
  FiringR();
  ~FiringR();
  void init(Istrm& inputf);
  void dump(ofstream& dumpf);
  void restart(Istrm& restartf);
  void getQ(double *V, double *Q, long totalnodes);
private: 
  Parameter thetaobj;
  Parameter sigmaobj;
  Parameter qmaxobj;
  double theta;
  double sigma;
  double qmax;
};

#endif
