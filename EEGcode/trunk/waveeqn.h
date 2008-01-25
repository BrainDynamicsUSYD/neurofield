/***************************************************************************
                          waveeqn.h  -  description
                             -------------------
    copyright            : (C) 2005 by Peter Drysdale
    email                : peter@physics.usyd.edu.au
 ***************************************************************************/

#ifndef WAVEEQN_H
#define WAVEEQN_H

#include<fstream>
using std::ofstream;
#include<iostream>
using std::cerr;
using std::endl;

#include"istrm.h"
#include"parameter.h"
#include"qhistory.h"
#include"propag.h"

class WaveEqn: public Propag {
public: 
  WaveEqn(long gridsize, double deltat);
  ~WaveEqn();
  void init(Istrm& inputf);
  void dump(ofstream& dumpf);
  void restart(Istrm& restartf);
  void stepwaveeq(double *Phi, Qhistory* qhistory);
private:
  Parameter gammaobj;
  Parameter effrangeobj;
  double gamma;
  double effrange;
  int tauab;
  double p2; // Square of mesh ratio, dimensionless equal to (deltat effrange gamma/deltax)^2
  const long gridsize; // size of grids for integration of wave equation
  const double deltat; // Grid spacing in time
  double deltax; // Grid spacing in space
  double deltatdivideddeltaxallsquared; //Factor in p2 definition
//  double twominusfourp2; // factor in wave algorithm
  double twominusthreep2; // factor in wave algorithm
//  double tenminusfourp2; //factor in wave algorithm
  double tenminusthreep2; //factor in wave algorithm
  double deltat2divided12; //factor in wave equation
  double dfact; //factor in wave equation equal to ((gamma deltat)^2)/12.
  double expfact1; //factor of Exp(- gamma deltat)
  double expfact2; //factor of Exp(- 2 gamma deltat)
  double *Phi_1; // Eta one step into past stored for use by DE solver routine
  double *Phi_2; // Eta two steps into past stored for use by DE solver routine  
  long rowlength;
  long sidelength; // number of nodes along one side length of square node population
  long startfirstrow;
  long startlastrow;
  long icentre, itop, ibottom, ileft, iright;
  long itopleft,itopright,ibottomleft,ibottomright;
  long iphi;
};

#endif
