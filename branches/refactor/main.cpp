/***************************************************************************
                          main.cpp  -  main program
                             -------------------
    copyright            : (C) 2005 by PeterDrysdale
    email                : peter@physics.usyd.edu.au
 ***************************************************************************/

#include<fstream>
using std::ofstream;
#include <iostream>
using std::endl;
#include<cstdlib>
#include<cstring>

#include"configf.h"
#include"connectmat.h"

int main(int argc, char* argv[])
{
  // print help message
  if( argc>2 )
    for( int i=1; i<argc-1; i++ )
      if( strcmp(argv[i],"-?")==0 || strcmp(argv[i],"-h")==0
        || strcmp(argv[i],"--help")==0 ) {
      std::cerr << "NeuroField usage: " << endl
        << "NeuroField [optional switches]" << endl
        << "where the optional switches are" << endl
        << "-d alternate.dump" << endl
        << "-i alternate.conf" << endl
        << "-o alternate.output" << endl;
        return 0;
      }

  // open conf file - default is neurofield.conf
  int iconfarg = 0;
  if( argc>2 )
    for( int i=1; i<argc-1; i++)
      if( strcmp(argv[i],"-i") == 0 )
        iconfarg = i + 1;
  const char* confname = iconfarg?argv[iconfarg]:"neurofield.conf";

  // if find keyword "restart" in the argument list, use restart mode
  bool restart = false;
  for( int i=0; i<argc-1; i++ )
    if( strcmp(argv[i],"restart")==0 )
      restart = true;

  // initialize inputf, the stream of the configuration file
  Configf* inputf = 0;
  if(restart) {
    std::cerr<<"Restart mode is not implemented yet!"<<endl;
    exit(EXIT_FAILURE);
    // istrm = new Restartf(confname);
  }
  else
    inputf = new Configf(confname);

  // read in connection matrix, determine number of populations and connections
  ConnectMat cntmat; *inputf>>cntmat;
  cntmat.solve();

  delete inputf;

  // open file for dumping data for restart - default is neurofield.dump
  int idumparg = 0; // Index No. of dump file name in argv
  if( argc>2 )
    for( int i=1; i<argc-1; i++)
      if( strcmp(argv[i],"-d") == 0 )
        idumparg = i + 1;
  ofstream dumpf(idumparg?argv[idumparg]:"neurofield.dump");
  if( !dumpf ) {
    std::cerr << "Unable to open "
      << (idumparg?argv[idumparg]:"neurofield.dump") << " for output.\n";
      exit(EXIT_FAILURE);
  }
  dumpf.precision(14);
  dumpf<<cntmat;

  return EXIT_SUCCESS;
}
