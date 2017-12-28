#include <apf.h>
#include <apfMesh2.h>
#include <apfShape.h>
#include "rhs.hpp"
#include "tau.hpp"

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

  apf::NewArray<double> BF;
  apf::NewArray<Vector> GBF;
  apf::getBF(shape, elem, p, BF);
  apf::getGradBF(shape, elem, p, GBF);

  Vector x(0,0,0);
  apf::mapLocalToGlobal(elem, p, x);

  for (int i = 0; i < nodes; ++i)
    Fe(i) += f(x)*BF[i]*w*dv;

  auto ent = apf::getMeshEntity(elem);
  auto tau = get_tau(mesh, ent, k, a);

  for (int i = 0; i < nodes; ++i) {
    double adv = 0.0;
    for (int d = 0; d < dim; ++d)
      adv += a[d]*GBF[i][d];
    Fe(i) += tau*f(x)*adv*w*dv;
  }

}

}
