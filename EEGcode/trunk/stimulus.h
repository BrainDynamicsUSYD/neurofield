/***************************************************************************
                          stimulus.h  -  description
                             -------------------
    copyright            : (C) 2005 by Peter Drysdale
    email                : peter@physics.usyd.edu.au
 ***************************************************************************/

#ifndef STIMULUS_H
#define STIMULUS_H

#include<fstream>
using std::ofstream;
#include<iostream>
using std::cerr;
using std::endl;

#include"istrm.h"
#include"random.h"

class Stimulus {
public: 
  Stimulus();
  ~Stimulus();
  void init(Istrm& inputf);
  void dump(ofstream& dumpf);
  void restart(Istrm& restartf);
  void getQstim(double timestep, double *Q, const long nodes);
private:
  double t; // Current time
  int mode; // Number representing mode of stimulus
  double ts; // Time before stimulus onset
  double amp; // Amplitude of stimulus
  double freq; // Frequency of sine stimulus
  double pdur; // Pulse duration of stimulus
  double tperiod; // Period of between pulse repetitions
  double mean; // Mean firing rate of stimulus
  Random random; // Random number generator object
};

#endif
