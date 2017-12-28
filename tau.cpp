#include <apf.h>
#include <apfMesh2.h>
#include "tau.hpp"

namespace avms {

static double get_mesh_size(apf::Mesh2* m, apf::MeshEntity* e) {
  double h = 0.0;
  apf::Downward edges;
  int ne = m->getDownward(e, 1, edges);
  for (int i=0; i < ne; ++i) {
    double length = apf::measure(m, edges[i]);
    h += length*length;
  }
  return sqrt(h/ne);
}

double get_tau(
    apf::Mesh2* mesh,
    apf::MeshEntity* e,
    double k,
    apf::Vector3 const& a) {
  double h = get_mesh_size(mesh, e);
  double am = a.getLength();
  double alpha = h*am/(2.0*k);
  return h/(2.0*am)*(1.0/std::tanh(alpha)-1.0/alpha);
}

}
