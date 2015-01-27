#include"burst.h"
using std::cerr;
using std::endl;
using std::cout;
void Population::init( Configf& configf )
{
  qinit = Qinit(configf);
  qhistory.push_back( vector<double>(nodes,qinit) );
  q.resize(nodes,qinit);

  configf.param("Length",length);

  if( qresponse ) { // neural population
    double temp; configf.param("Q",temp);
    configf.param( "Firing", *qresponse );
  }
  else { // stimulus population
    timeseries = new Timeseries(nodes,deltat,index);
    configf.param( "Stimulus", *timeseries );
  }
  settled = true;
}

Population::Population( int nodes, double deltat, int index )
    : NF(nodes,deltat,index),
    qresponse(0), timeseries(0), qkey(0), settled(false)
{
}

Population::~Population()
{
  if(qresponse) delete qresponse;
  if(timeseries) delete timeseries;
}

void Population::step(void)
{
  // move pointer to keyring
  qkey = (qkey+1) % qhistory.size();
  if( qresponse ) { // neural population
    qresponse->step();
    qresponse->fire( qhistory[qkey] );
  }
  else { // stimulus population
    timeseries->step();
    timeseries->fire( qhistory[qkey] );
  }

  // q is the current Q, only for output purpose
  for( int i=0; i<nodes; i++ )
    q[i] = qhistory[qkey][i];
}

double Population::Qinit( Configf& configf ) const
{
  if( qresponse ) {
    string buffer = configf.find( label("Population ",index+1)+"*Q:");
    return atof(buffer.c_str());
  }
  else {
    string temp = configf.find( label("Population ",index+1)+"*Mean:");
    return atof( temp.c_str() );
  }
}

const vector<double>& Population::glu(void) const
{
  if( qresponse )
    return qresponse->glu();
  else {
    cerr<<"Trying to access glu of a stimulus population."<<endl;
    exit(EXIT_FAILURE);
  }
}

void Population::add2Dendrite( int index,
    const Propag& prepropag, const Couple& precouple, Configf& configf )
{
  if( settled ) {
    cerr<<"Population is already settled, no more dendrites can be added!"
        <<endl;
    exit(EXIT_FAILURE);
  }

  if( !qresponse ) {
    string temp(configf.find( label("Population ",this->index+1)+"*Firing:" ));
    // PUT YOUR QRESPONSE HERE
    if( temp == "Bursting" )
      qresponse = new BurstResponse(nodes,deltat,this->index);
    //else if( temp == "FS" )
      //qresponse = new FS(nodes,deltat,this->index);
    // END PUT YOUR QRESPONSE HERE
    else
      qresponse = new QResponse(nodes,deltat,this->index);
  }
  qresponse->add2Dendrite( index, prepropag, precouple, configf );
}

const vector<double>& Population::Q( const Tau& tau) const
{
  if( tau.m.size() == 1 ) // homogeneous tau
    return qhistory[(qkey-tau.m[0]+qhistory.size())%qhistory.size()];
  else { // tau.m.size() == nodes, inhomogeneous tau
    static vector<double> temp(nodes);
    for( int i=0; i<nodes; i++ ){
      temp[i] = qhistory[(qkey-tau.m[i]+qhistory.size())%qhistory.size()][i];
    }
    return temp;
  }
}

const vector<double>& Population::V(void) const
{
  if( qresponse )
    return qresponse->V();
  else {
    cerr<<"Trying to access V of a stimulus population."<<endl;
    exit(EXIT_FAILURE);
  }
}

void Population::growHistory( const Tau& tau )
{
  /*if( settled ) {
    cerr<<"Population is already settled, cannot resize firing history!"
        <<endl;
    exit(EXIT_FAILURE);
  }*/

  if( size_t(tau.max) > qhistory.size() ) {
    qhistory.resize( tau.max+1 );
    for( unsigned int i=0; i<qhistory.size(); i++ )
      qhistory[i].resize( nodes, qinit );
  }
}

void Population::output( Output& output ) const
{
  if(qresponse) {
    output("Pop",index+1,"Q",q);
    qresponse->output(output);
  }
  else
    timeseries->output(output);
}

void Population::outputDendrite( int index, Output& output ) const
{
  if(qresponse) return qresponse->outputDendrite(index,output);
}
