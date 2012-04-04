#ifndef CADP_H
#define CADP_H

#include"couple.h"

class CaDP : public Couple
{
  CaDP();
  CaDP(CaDP&);

  double sig( double x, double beta ) const;
  double potentiate(double Ca) const;
  double depress(double Ca) const;

  vector<double> Ca;
  vector<double> binding; // glutamate binding

  double B; // 1/stanard deviation of glutamate binding

  double nu_max; // maximum synaptic strength
  double nu_th; // threshold time-scale of plasticity
  double nu_ltd; // time-scale of depression
  double nu_ltp; // time-scale of potentiation

  double tCa; // time-scale of calcium influx/cascade
protected:
  virtual void init( Configf& configf );
  virtual void restart( Restartf& restartf );
  virtual void dump( Dumpf& dumpf ) const;
public: 
  CaDP( int nodes, double deltat, int index, const vector<double>& glu,
          const Population& prepop, const Population& postpop );
  virtual ~CaDP(void);
  virtual void step(void);
  virtual vector<Output*> output(void) const;
};

#endif