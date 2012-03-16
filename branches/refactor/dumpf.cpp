#include<iomanip>
#include<cstdlib>
using std::setw;
#include<iostream>
using std::endl;
#include"dumpf.h"

void Dumpf::open(void)
{
  if( !file.is_open() ) {
    file.open( filename.c_str() );
    if( !file ) {
      std::cerr<<"Error, cannot open file'"<<filename.c_str()<<"' for output."
          <<endl;
      exit(EXIT_FAILURE);
    }
  }
}

Dumpf::Dumpf(void)
{
  file.precision(14); file<<std::scientific;
  ss.precision(14); ss<<std::scientific;
  s = &ss; // defaults to output when program terminates
}

Dumpf::~Dumpf(void)
{
  if( !ss.str().empty() ) {
    verbose();
    file.close();
  }
}

void Dumpf::open( const string& filename )
{
  if( !this->filename.empty() ) {
    std::cerr<<"Attempting to dump into two files."<<endl;
    exit(EXIT_FAILURE);
  }
  this->filename = filename;
}

void Dumpf::verbose(void)
{
  open();
  s = &file;
  if( !ss.str().empty() )
    file<<ss.str().c_str();
}

Dumpf& Dumpf::operator<< ( double f )
{
  if( f>0 ) *s<<" ";
  *s<<f;
  return *this;
}

Dumpf& Dumpf::operator<< ( const string& str )
{
  *s<<setw(20)<<str.c_str();
  return *this;
}

Dumpf& Dumpf::operator<< ( int i )
{
  *s<<i;
  return *this;
}

Dumpf& Dumpf::operator<< ( unsigned int i )
{
  *s<<i;
  return *this;
}

Dumpf& Dumpf::operator<< ( ostream& (*pf)(ostream&) )
{
  *s<<pf;
  return *this;
}

ostream& septor( ostream& os )
{
  os<<"|";
  return os;
}

ostream& space( ostream& os )
{
  os<<" ";
  return os;
}

ostream& setw( ostream& os )
{
  os<<setw(20);
  return os;
}
