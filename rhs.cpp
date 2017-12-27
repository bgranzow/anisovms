#include <apf.h>
#include <apfMesh2.h>
#include <apfShape.h>
#include "rhs.hpp"

namespace avms {

RHS::RHS(
    apf::Mesh2* mesh_,
    double k_,
    Vector const& a_,
    std::function<double(Vector const&)> f_) : apf::Integrator(1) {
  mesh = mesh_;
  dim = mesh->getDimension();
  k = k_;
  a = a_;
  f = f_;
  nodes = (dim == 2) ? 3 : 4;
  Fe.setSize(nodes);
  shape = apf::getLagrange(1);
}

void RHS::inElement(apf::MeshElement* me) {
  elem = me;
  for (int i = 0; i < nodes; ++i)
    Fe(i) = 0.0;
}

void RHS::atPoint(Vector const& p, double w, double dv) {
  (void)p;
  (void)w;
  (void)dv;
}

}
