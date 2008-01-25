/***************************************************************************
                          dendriticr.cpp  -  description
                             -------------------
    copyright            : (C) 2005 by Peter Drysdale
    email                : peter@physics.usyd.edu.au
 ***************************************************************************/

#include "dendriticr.h"

DendriticR::DendriticR(long numnodes):nodes(numnodes),
             alphaobj("alpha"), betaobj("beta"){
  previousPab = new double [nodes];
}
DendriticR::~DendriticR(){
  delete[ ] previousPab;
}

void DendriticR::init(Istrm& inputf, double& Vinit){
  inputf.validate("V initial",58);
  inputf >> Vinit;
  for(long i=0; i<nodes; i++){
    previousPab[i]=Vinit;
  } 
  alphaobj.init(inputf);
  betaobj.init(inputf);
}

void DendriticR::dump(ofstream& dumpf){
  dumpf << "Dendritic Response from population ";
  alphaobj.dump(dumpf);
  betaobj.dump(dumpf);
  dumpf << "Pab_previous:";
  for(long i=0; i<nodes; i++){
    dumpf << previousPab[i] << " ";
  }
  dumpf << endl; // Add endline to dendritic response input
}

void DendriticR::restart(Istrm& restartf){
  alphaobj.restart(restartf);
  betaobj.restart(restartf);
  double tempPab;
  restartf.validate("Pab_previous",58);
  for(long i=0; i<nodes; i++){
    restartf >> tempPab;
    previousPab[i]=tempPab;
  }
}

void DendriticR::stepVab(double *Pab, double * Vab, double *dVabdt, double timestep){
//
// Steps Pab(t+Timestep) using current Pab(t) and current Vab(t)
//
//  The program assumes that alpha, beta are constant and Pab(t) is linear for the time step
//  This is since it is very costly to obtain Pab(t).
//  Under these assumptions the solution can be explicitly obtained.
//  Calculating the explicit solution is computationally faster than using
//  Runge-Kutta to evaluate the time step.
//  At current time alpha and beta are not functions of space and so 
//  computed variables are used to speed up the calculation.
//
  double adjustedPab;
  double alphaminusbeta;
  double factoralphabeta;
  double deltaPdeltat;
  double C1;
  double C1expalpha;
  double C2expbeta;

  alpha=alphaobj.get();
  beta=betaobj.get();
  expalpha=exp(-alpha*timestep);
  expbeta=exp(-beta*timestep);
  alphaminusbeta=alpha-beta;
  factoralphabeta=(1.0/alpha)+(1.0/beta);
  for(long i=0; i<nodes; i++){
    deltaPdeltat=(Pab[i]-previousPab[i])/timestep;
    adjustedPab=previousPab[i]-factoralphabeta*deltaPdeltat-Vab[i];
    C1=(adjustedPab*beta-dVabdt[i]+deltaPdeltat)/alphaminusbeta;
    C1expalpha=C1*expalpha;
    C2expbeta=expbeta*(-C1-adjustedPab);
    Vab[i]=C1expalpha+C2expbeta+Pab[i]-factoralphabeta*deltaPdeltat;
    dVabdt[i]=C1expalpha*(-alpha)+C2expbeta*(-beta)+deltaPdeltat;
    previousPab[i]=Pab[i]; //Save current pulse density for next step
  }
}
