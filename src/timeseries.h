#ifndef NEUROFIELD_SRC_TIMESERIES_H
#define NEUROFIELD_SRC_TIMESERIES_H

//#include<string>
//#include<vector>
//#include<cmath>
#include"nf.h"
#include"random.h"
using std::string;
using std::vector;

class Timeseries : public NF {
  Timeseries(Timeseries&);
  Timeseries(void);
 protected:
  void init( Configf& configf ) override;

  vector<Timeseries*> series;
  vector<double> node;
  double t;
  double cease;
 public:
  Timeseries( int nodes, double deltat, int index );
  ~Timeseries(void) override;
  void step(void) override;
  virtual void fire( vector<double>& Q ) const;
};

namespace TIMESERIES {

struct Const : public Timeseries {
  double mean;
  Const(int nodes,double deltat,int index) : Timeseries(nodes,deltat,index) {}
  void init( Configf& configf ) override;
  void fire( vector<double>& Q ) const override;
};

struct Pulse : public Timeseries {
  double amp, width, period, pulses;
  Pulse(int nodes,double deltat,int index) : Timeseries(nodes,deltat,index) {}
  void init( Configf& configf ) override;
  void fire( vector<double>& Q ) const override;
};

struct White : public Timeseries {
  double seed, amp, mean, deltax;
  Random* random;
  White(int nodes,double deltat,int index) : Timeseries(nodes,deltat,index) {}
  ~White(void) override {
    delete random;
  }
  void init( Configf& configf ) override;
  void fire( vector<double>& Q ) const override;
};

struct WhiteCoherent : public Timeseries {
  double seed, amp, mean;
  Random* random;
  WhiteCoherent(int nodes,double deltat,int index) : Timeseries(nodes,deltat,index) {}
  ~WhiteCoherent(void) override {
    delete random;
  }
  void init( Configf& configf ) override;
  void fire( vector<double>& Q ) const override;
};

struct PAS : public Timeseries {
  double n20w, n20h, p25w, p25h, tmsw, tmsh, t_mns, isi;
  PAS(int nodes,double deltat,int index) : Timeseries(nodes,deltat,index) {}
  void init( Configf& configf ) override;
  void fire( vector<double>& Q ) const override;
};

struct Burst : public Timeseries {
  double amp, width, bursts, freq, oscillation_freq, on, off, total;
  Burst(int nodes,double deltat,int index) : Timeseries(nodes,deltat,index) {}
  void init( Configf& configf ) override;
  void fire( vector<double>& Q ) const override;
};

struct Sine : public Timeseries {
  double amp, width, period, phase, pulses;
  Sine(int nodes,double deltat,int index) : Timeseries(nodes,deltat,index) {}
  ~Sine(void) override = default;
  void init( Configf& configf ) override;
  void fire( vector<double>& Q ) const override;
};

} // namespace TIMESERIES
#endif
