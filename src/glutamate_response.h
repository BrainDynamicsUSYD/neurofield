/** @file glutamate_response.h
  @brief Defines the GlutamateResponse class, for the soma response of neural populations.

  @author Peter Drysdale, Felix Fung,
*/

#ifndef NEUROFIELD_SRC_GLUTAMATE_RESPONSE_H
#define NEUROFIELD_SRC_GLUTAMATE_RESPONSE_H

// Forward declaration to break circular collaboration
class GlutamateResponse;

// Other neurofield headers
#include "configf.h"    // Configf;
#include "de.h"         // DE; RK4;
#include "qresponse.h"  // QResponse;

// C++ standard library headers
#include <vector> // std::vector;

class GlutamateResponse : public QResponse {
  GlutamateResponse(const GlutamateResponse& ); // No copy constructor allowed.
  GlutamateResponse();                          // No default constructor allowed.

 protected:
  void init( Configf& configf ) override;

  // glutamate concentration in synaptic cleft
  struct Glu : public DE {
    double Lambda = 0.0; ///< glutamate concentration per action potential
    double tGlu = 0.0;   ///< time scale of glutamate

    Glu( vvd_size_type nodes, double deltat ) : DE(nodes,deltat,2) {}
    ~Glu() override = default;
    void init( Configf& configf );
    void rhs( const std::vector<double>& y, std::vector<double>& dydt ) override;
  };
  Glu glu_m;
  RK4 glu_rk4;
 public:
  void step() override;
  GlutamateResponse( size_type nodes, double deltat, size_type index );
  ~GlutamateResponse() override;
  const std::vector<double>& glu() const;
};

#endif //NEUROFIELD_SRC_GLUTAMATE_RESPONSE_H
