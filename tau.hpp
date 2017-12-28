#ifndef vms_tau_hpp
#define vms_tau_hpp

namespace apf {
class Mesh2;
class Vector3;
};

namespace avms {

using Vector = apf::Vector3;

double get_mesh_size(
    apf::Mesh2* m,
    apf::MeshEntity* e);

double get_tau(
    apf::Mesh2* m,
    apf::MeshEntity* e,
    double k,
    Vector const& a);

}

#endif
