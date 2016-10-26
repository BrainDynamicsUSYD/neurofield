/**
 * @file coupleramp.h
 * Coupling class to produce piecewise linear segments to vary synaptic couplings nu.
 * In other words it produces a temporal profile for nu.
 *
 * @brief Can use "brief" tag to explicitly generate comments for file documentation.
 *
 * @author Paula Sanz-Leon , Romesh Abeysuriya
 *
 */

/**
   Define properties of CouplingRamp

   + param[in]     ns, tpts: vector with the values of nus at specific time points specified in vector tpts.
   + param[in]     pairs   : total number of pairs of (nu, time) to define the segments
*/

#ifndef NEUROFIELD_SRC_COUPLERAMP_H
#define NEUROFIELD_SRC_COUPLERAMP_H

#include"coupling.h"

using std::vector;

class CouplingRamp : public Coupling {
  CouplingRamp();
  CouplingRamp(CouplingRamp&);
 protected:
  vector<double> ns;
  vector<double> tpts;
  vector<double> deltanu;
  double time;
  int pairs;
 public:
  void init( Configf& configf ) override;
  void step(void) override;
  CouplingRamp( int nodes, double deltat, int index,
              const Propagator& prepropag, const Population& postpop );
  ~CouplingRamp(void) override;
};

#endif