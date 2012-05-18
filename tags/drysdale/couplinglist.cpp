/***************************************************************************
                          couplinglist.cpp  -  Holds a list of various
                                               synaptic coupling objects
                             -------------------
    copyright            : (C) 2009 by Peter Drysdale
    email                : peter@physics.usyd.edu.au
 ***************************************************************************/

#include<cstdlib>
#include"couplinglist.h"
#include"coupling.h"
#include"modcouple.h"
#include"stdp.h"
#include"cadp.h"
using std::endl;

//
// Constructor for Couplinglist creates an array of coupling objects
//

Couplinglist::Couplinglist(Istrm& inputf, ofstream& dumpf
  ,int numconnects, long nodes, double deltat):numcoup(numconnects){
  couparray = new Couple *[numcoup];
  inputf.validate("Coupling type",115);// search for "propagator types NB:- the s is ASCII 115
  dumpf << "Coupling types ";
  int optionnum;
  for(int i=0;i<numcoup;i++){
    inputf.ignore(200,58);
    optionnum=inputf.choose("Simple:1 Modulate:2 STDP:3 Calcium:4 Hebbian:5",32);
    if(1==optionnum){
      couparray[i] = new Coupling(nodes,deltat);
      dumpf << (i+1) << ": Simple ";
    }
    if(2==optionnum){
      couparray[i] = new Modcouple(nodes,deltat);
      dumpf << (i+1) << ": Modulate ";
    }
    if(3==optionnum){
      couparray[i] = new STDP(nodes,deltat);
      dumpf << (i+1) << ": STDP ";
    }
    if(4==optionnum){
      couparray[i] = new CaDP(nodes,deltat);
      dumpf << (i+1) << ": Calcium ";
    }
    if( optionnum<1 || optionnum>5 ){
      std::cerr << "Invalid Coupling type" << endl;
      exit(EXIT_FAILURE);
    }
  }
  inputf.validate("Lambda",58); inputf >> Lambda;
  inputf.validate("tGlu",58); inputf >> tGlu;
  dumpf << "Lambda: " << Lambda << endl;
  dumpf << "tGlu: " << tGlu << endl;
  numnodes = nodes;
  this->deltat = deltat;
  glu = new double[numnodes];
  for( int i=0; i<numnodes; i++ )
    glu[i] = 0;
  dglu = new double[numnodes];
  for( int i=0; i<numnodes; i++ )
    dglu[i] = 0;
}

// Destructor deletes each coupling object and then array which holds them
Couplinglist::~Couplinglist(){
  for(int i=0;i<numcoup; i++)
    delete getcoup(i);
  delete [ ] couparray;
  if(glu) delete[] glu;
}

void Couplinglist::init(Istrm& inputf){
  for(int i=0; i<numcoup; i++) {
    getcoup(i)->init(inputf,i);
  }
}

void Couplinglist::dump(ofstream& dumpf){
  for(int i=0; i<numcoup; i++){
    dumpf << "Coupling " << (i+1) << " ";
    getcoup(i)->dump(dumpf);
    dumpf << endl; //Append endl to coupling data
  }
  dumpf << "Lambda: " << Lambda << endl;
  dumpf << "tGlu: " << tGlu << endl;
}

// updateP method updates P via each coupling object 
void Couplinglist::updateP(double **P,double **Eta,Qhistorylist& qhistorylist,ConnectMat& connectmat){
  for( int j=0; j<numnodes; j++ )
    dglu[j] = 0;//double ddglu = 0; double ts = 200e-3; double td = 200e-3;
  for( int i=0; i<numcoup; i++ )
    if( getcoup(i)->sign )
      for( int j=0; j<numnodes; j++ )
        dglu[j] += Lambda*Eta[i][j]*deltat;
  for( int j=0; j<numnodes; j++ ) {
    dglu[j] -= glu[j]/tGlu*deltat;
    glu[j] += dglu[j];
    if( glu[j]<0 ) glu[j] = 0;
    /*double p1 = ddglu;
      double k1 = dglu[j];
      double p2 = p1 -p1/2/ts -k1/2/td;
      double k2 = dglu[j] +1/2*(dglu[j]+p1);
      double p3 = p1 -p2/2/ts -k2/2/td;
      double k3 = dglu[j] +1/2*(dglu[j]+p2);
      double p4 = p1 -p3/ts -k3/td;
      double k4 = dglu[j] +dglu[j]+p3;
      dglu[j] += 1/6*(p1+2*p2+2*p3+p4);
      glu[j] += 1/6*(k1+2*k2+2*k3+k4);
    */
  }
  for(int i=0;i<numcoup;i++){
    getcoup(i)->updatePa(P[i],Eta[i],qhistorylist,connectmat,*this);
  }
}

void Couplinglist::output(){
  for(int i=0;i<numcoup;i++){
    getcoup(i)->output();
  }
}