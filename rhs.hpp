#ifndef avms_rhs_hpp
#define avms_rhs_hpp

#include <apfDynamicVector.h>
#include <functional>

namespace apf {
class Mesh2;
class Vector3;
}

namespace avms {

using Vector = apf::Vector3;

class RHS : public apf::Integrator {
  public:
    RHS(
        apf::Mesh2* mesh,
        double k,
        Vector const& a,
        std::function<double(Vector const&)> f);
    void inElement(apf::MeshElement* me);
    void atPoint(Vector const&, double w, double dv);
    apf::DynamicVector Fe;
  private:
    int dim;
    int nodes;
    double k;
    Vector a;
    apf::Mesh2* mesh;
    apf::MeshElement* elem;
    apf::FieldShape* shape;
    std::function<double(Vector const&)> f;
};

}

#endif
