#include "disc.hpp"
#include "error.hpp"
#include "tau.hpp"

namespace avms {

Error::Error(
    Disc* disc_,
    double k_,
    Vector const& a_,
    std::function<double(Vector const& x)> f_,
    std::function<double(Vector const& x)> q_) : apf::Integrator(1) {
  mesh = disc_->mesh;
  uh = disc_->u;
  zh = disc_->z;
  f = f_;
  q = q_;
  a = a_;
  k = k_;
  elem = 0;
  u_elem = 0;
  z_elem = 0;
  error = apf::createStepField(mesh, "e", apf::SCALAR);
}

Error::~Error() {
  apf::destroyField(error);
}

void Error::inElement(apf::MeshElement* me) {
  elem = me;
  u_elem = apf::createElement(uh, elem);
  z_elem = apf::createElement(zh, elem);
  Je_elem = 0.0;
}

void Error::atPoint(Vector const& p, double w, double dv) {

  apf::Vector3 grad_u;
  apf::Vector3 grad_z;
  apf::getGrad(u_elem, p, grad_u);
  apf::getGrad(z_elem, p, grad_z);

  apf::Vector3 x(0,0,0);
  apf::mapLocalToGlobal(elem, p, x);

  auto ent = apf::getMeshEntity(elem);
  auto tau = get_tau(mesh, ent, k, a);

  auto Ru = f(x) - (a*grad_u);
  auto Rz = f(x) - (a*grad_z);
  auto u_prime = tau*Ru;
  auto z_prime = tau*Rz;
  auto Lstarz = -(a*grad_z);

  Je_elem += (z_prime*Ru + Lstarz*u_prime)*w*dv;

}

void Error::outElement() {
  auto ent = apf::getMeshEntity(elem);
  apf::setScalar(error, ent, 0, Je_elem);
  apf::destroyElement(u_elem);
  apf::destroyElement(z_elem);
}




}
