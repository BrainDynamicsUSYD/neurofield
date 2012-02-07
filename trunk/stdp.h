/***************************************************************************
                          stdp.h  - STDP rule developed by Park Fung, Adam Haber and Peter Robinson
                             -------------------
    copyright            : (C) 2009
    email                : peter@physics.usyd.edu.au
 ***************************************************************************/

#ifndef STDP_H
#define STDP_H

#include<cstdlib>
#include<fstream>
using std::ofstream;
#include<iostream>
#include<complex>
#include"istrm.h"
#include"couple.h"
#include"timeseries.h"
#include"qhistorylist.h"
#include"connectmat.h"
#include"parameter.h"
using std::complex;

class STDP: public Couple {
public: 
  STDP(long nodes, double deltat);
  ~STDP();
  void init(Istrm& inputf, int coupleid); 
  void dump(ofstream& dumpf); // output values for restart
  void output(); // output variables as needed
  void updatePa(double *Pa,double *Etaa,Qhistorylist& qhistorylist,ConnectMat& connectmat,Couplinglist& couplinglist);

private:
  STDP(STDP& ); // no copy constructor
  const long nodes;
  ofstream synapoutf;

  const std::complex<double> i0; // purely imaginary number i
  const std::complex<double> c1; // real number 1

  double nu; // evolving coupling constant
  double rho; // constant linearized sigmoid slope
  double alpha; // constant dendritic constant
  double beta; // constant dendritic constant
  double gamma; // constant axonal constant
  double Ap;
  double Am;
  double Taup;
  double Taum;
  double B;
  double N;
  double Q_max; // maximal firing, may be different from the sigmoid function

public:
  virtual complex<double> X( int i ) const; // returns X[i]

private:
  double deltat;
  int coupleid;
  int sign; // sign of nu for this population
#define W_CUTOFF	int(5000)
#define W_STEP		int(1)
  complex<double> L[int(W_CUTOFF/W_STEP)];
  complex<double> H[int(W_CUTOFF/W_STEP)];
  complex<double> Gamma[int(W_CUTOFF/W_STEP)];
  double filter[int(W_CUTOFF/W_STEP)];
};

#endif