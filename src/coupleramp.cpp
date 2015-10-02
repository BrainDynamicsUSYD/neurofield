#include<iostream>
using std::cerr;
using std::endl;
using std::cout;
#include "coupleramp.h"

void CoupleRamp::init( Configf& configf )
{ 

  // Number of different nus and timepoints
  configf.param("pairs", pairs);
  // Read values of nus
  vector<double> tempn;
  configf.next("nus");
  tempn = configf.numbers();
  // Read timepoints at which nu should change
  vector<double> tempt;
  configf.next("timepoints");
  tempt = configf.numbers();

  double temp;
  // Check that the lengths of both arrays match
  // if( tempn.size() == tempt.size()) throw some errors

  for ( int i=0; i<=pairs; i++ ){
     temp = deltat*((tempn[i+1]-tempn[i])/(tempt[i+1]-tempt[i]));
     deltanu.push_back(temp);
  }
  // Assume that at t=0, nu=nus[0], ie, the segment between t=0 and timepoints[0] is constant.
  n.clear(); n.resize(nodes,tempn[0]);
  pos = (tempn[0]>0)?1:-1;
  
  for( int i=0; i<nodes; i++ )
    P[i] = n[i]*prepropag.phiinit(configf);

  time = 0;
  for(int i=0; i<pairs; i++)
  {
     tpts.push_back(tempt[i]);
  }
  
}


void CoupleRamp::step(void)
{  
  time += deltat;
  for ( int j=1; j<pairs; j++ )
    if( time >= tpts[j-1] && time < tpts[j] )
      for( int i=0; i<nodes; i++ )
        n[i] += deltanu[j-1];

  Couple::step();
}
CoupleRamp::CoupleRamp( int nodes, double deltat, int index,
        const Propag& prepropag, const Population& postpop )
    : Couple(nodes,deltat,index,prepropag,postpop)
{
}

CoupleRamp::~CoupleRamp(void)
{
}