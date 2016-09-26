#ifndef NEUROFIELD_SRC_DE_H
#define NEUROFIELD_SRC_DE_H

#include<vector>
using std::vector;

class DE {
  DE(void);
  DE(DE&);
  void operator=(DE&);
 protected:
  // if the number of field variables need to be extended, use this function
  void extend( int extension ) {
    n += extension;
    variables.resize(n);
    for( int i=0; i<n; i++ ) {
      variables[i].resize(nodes,0);
    }
  }
 public:
  int nodes;
  double deltat;
  int n; // dimension of system == y.size()
  vector<vector<double> > variables;

  DE( int nodes, double deltat, int n )
    : nodes(nodes), deltat(deltat), n(n), variables(n) {
    for( int i=0; i<n; i++ ) {
      variables[i].resize(nodes);
    }
  }
  virtual ~DE(void) = default;

  virtual vector<double>& operator[] ( int index ) {
    return variables[index];
  }
  virtual const vector<double>& operator[] ( int index ) const {
    return variables[index];
  }
  // define dydt here
  virtual void rhs( const vector<double>& y, vector<double>& dydt ) = 0;
};

class Integrator {
  Integrator(void);
  Integrator(Integrator&);
  void operator=(Integrator&);
 protected:
  DE& de;
 public:
  explicit Integrator( DE& de ) : de(de) {}
  virtual ~Integrator(void) = default;
  virtual void step(void) = 0;
};

class Euler : public Integrator {
  Euler(void);
  Euler(Euler&);
  void operator=(Euler&);
 protected:
  vector<double> dydt;
 public:
  explicit Euler( DE& de ) : Integrator(de), dydt(de.n) {}
  ~Euler(void) override = default;
  void step(void) override {
    for( int j=0; j<de.nodes; j++ ) {
      de.rhs( de.variables[j], dydt );
      for( int i=0; i<de.n; i++ ) {
        de.variables[i][j] += dydt[i]*de.deltat;
      }
    }
  }
};

class RK4 : public Integrator {
  RK4(void);
  RK4(RK4&);
  void operator=(RK4&);
 protected:
  double h6; // == deltat/6

  vector<double> k1;
  vector<double> k2;
  vector<double> k3;
  vector<double> k4;
  vector<double> temp;
 public:
  explicit RK4( DE& de ) : Integrator(de), h6(de.deltat/6.),
    k1(de.n), k2(de.n), k3(de.n), k4(de.n), temp(de.n) {}
  ~RK4(void) override = default;

  void step(void) override {
    for( int j=0; j<de.nodes; j++ ) {
      for( int i=0; i<de.n; i++ ) {
        temp[i] = de.variables[i][j];
      }
      de.rhs(temp,k1);
      for( int i=0; i<de.n; i++ ) {
        temp[i] = de.variables[i][j] +de.deltat*.5*k1[i];
      }
      de.rhs(temp,k2);
      for( int i=0; i<de.n; i++ ) {
        temp[i] = de.variables[i][j] +de.deltat*.5*k2[i];
      }
      de.rhs(temp,k3);
      for( int i=0; i<de.n; i++ ) {
        temp[i] = de.variables[i][j] +de.deltat*k3[i];
      }
      de.rhs(temp,k4);
      for( int i=0; i<de.n; i++ ) {
        de.variables[i][j] += h6*( k1[i] +2*k2[i] +2*k3[i] +k4[i] );
      }
    }
  }
};

#endif
