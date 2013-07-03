#include "propag.h"

void Propag::init( Configf& configf )
{
  double Q = prepop.Qinit(configf);
  string buffer("Steady");
  configf.optional("phi",buffer);
  if( buffer != "Steady" )
    Q = atof(buffer.c_str());
  p.clear(); p.resize(nodes,Q);
  configf.param("Tau",tau);
  prepop.growHistory(tau);
}

Propag::Propag( int nodes, double deltat, int index, Population& prepop,
        Population& postpop, int longside, string topology )
    : NF(nodes,deltat,index), prepop(prepop), postpop(postpop),
         tau(nodes,deltat,index), longside(longside), p(nodes)
{
}

Propag::~Propag(void)
{
}

void Propag::step(void)
{
  p = prepop(tau);
}

double Propag::phiinit( Configf& configf ) const
{
  return prepop.Qinit(configf);
}

const vector<double>& Propag::phi(void) const
{
  return p;
}

double Propag::operator[]( int node ) const
{
  return p[node];
}

void Propag::output( Output& output ) const
{
  output("Propag",index+1,"phi",p);
}
