#include <apf.h>
#include <apfMesh2.h>
#include <apfShape.h>
#include "lhs.hpp"
#include "tau.hpp"

namespace avms {

LHS::LHS(
    apf::Mesh2* mesh_,
    double k_,
    Vector const& a_,
    bool is_dual) : apf::Integrator(1) {
  mesh = mesh_;
  dim = mesh->getDimension();
  k = k_;
  if (! is_dual) a = a_;
  else a = a_ * (-1.0);
  nodes = (dim == 2) ? 3 : 4;
  Ke.setSize(nodes, nodes);
  shape = apf::getLagrange(1);
}

void LHS::inElement(apf::MeshElement* me) {
  elem = me;
  for (int i = 0; i < nodes; ++i)
  for (int j = 0; j < nodes; ++j)
    Ke(i,j) = 0.0;
}

void LHS::atPoint(Vector const& p, double w, double dv) {
  
  apf::NewArray<double> BF;
  apf::NewArray<Vector> GBF;
  apf::getBF(shape, elem, p, BF);
  apf::getGradBF(shape, elem, p, GBF);

  for (int i = 0; i < nodes; ++i)
  for (int j = 0; j < nodes; ++j)
  for (int d = 0; d < dim; ++d)
    Ke(i,j) += (k*GBF[j][d]*GBF[i][d] + a[d]*GBF[j][d]*BF[i])*w*dv;

  auto ent = apf::getMeshEntity(elem);
  auto tau = get_tau(mesh, ent, k, a);

  for (int i = 0; i < nodes; ++i) {
    for (int j = 0; j < nodes; ++j) {
      double adv1 = 0.0;
      double adv2 = 0.0;
      for (int d = 0; d < dim; ++d) {
        adv1 += a[d]*GBF[j][d];
        adv2 += a[d]*GBF[i][d];
      }
      Ke(i,j) += tau*adv1*adv2*w*dv;
    }
  }

}

}
