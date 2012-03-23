/***************************************************************************
                          wave.h  -  damped wave propagator
                             -------------------
    copyright            : (C) 2006 by Peter Drysdale
    email                : peter@physics.usyd.edu.au
 ***************************************************************************/

#ifndef WAVE_H
#define WAVE_H

#include"propag.h"
#include"stencil.h"

class Wave : public Propag
{
  Wave(); // no default constructor
  Wave(Wave&); // no copy constructor

  double deltax; // spatial length of a node
  double range;  // characteristic damping length
  double gamma;  // damping coefficient
protected:
  void init( Configf& configf );
  void restart( Restartf& restartf );
  void dump( Dumpf& dumpf ) const;

  Stencil* oldp[2]; // stencil to past phi, oldp[0]==most recent
  vector<double> oldpval[2]; // keyring of past phi
  Stencil* oldQ[2]; // stencil to past Q, oldQ[0]==most recent
  vector<double> oldQval[2]; // keyring of past Q
  int key;

  // variables that's initialized once only
  double dt2on12;
  double dfact;
  double dt2ondx2;
  double p2;
  double tenminus3p2;
  double twominus3p2;
  double exp1;
  double exp2;

  // variables that's changed every timestep
  double sump;
  double diagsump;
  double sumQ;
  double diagsumQ;
  double drive;
public: 
  Wave( int nodes, double deltat, int index, Population& prepop,
      Population& postpop, int longside );
  virtual ~Wave(void);
  virtual void step(void); 
};

#endif
