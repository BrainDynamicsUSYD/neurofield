/***************************************************************************
                          poplist.cpp  -  Poplist is an object holding
                                          an array of (neural) populations
                             -------------------
    copyright            : (C) 2005 by Peter Drysdale
    email                : peter@physics.usyd.edu.au
 ***************************************************************************/

#include "poplist.h"
using std::endl;

// Constructor for Poplist creates an array of (neural) populations
Poplist::Poplist(long nodes, int numberofpops, ConnectMat& connectmat): numpops(numberofpops){
  //poparray = new Population *[numpops];
  for(int i=0;i<numpops;i++)
    poparray.push_back( new Population(nodes,i,connectmat) );
}

// Destructor deletes each population object and then array which holds them
Poplist::~Poplist(){
  for(int i=0;i<numpops; i++)
    if( poparray[i] )
      delete poparray[i];
  //delete [ ] poparray;
}

// get method returns a pointer to the "index"th Population in the population list
inline Population& Poplist::get(int index) const{ return *poparray[index]; }

void Poplist::init(Configf& inputf,PropagNet& propagnet,ConnectMat& connectmat){
  for(int i=0; i<numpops; i++)
    get(i).init(inputf,propagnet,connectmat);
}

void Poplist::dump(std::ofstream& dumpf){
  for(int i=0; i<numpops; i++)
    get(i).dump(dumpf);
}

void Poplist::restart(Configf& restartf,PropagNet& propagnet, ConnectMat& connectmat){
  for(int i=0; i<numpops; i++)
    get(i).restart(restartf,propagnet,connectmat);
}

// Step forward in time each of the neural populations method
void Poplist::stepPops(double timestep){
  for(int i=0; i<numpops; i++)
    get(i).stepPop(timestep);
}
