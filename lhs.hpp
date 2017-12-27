#ifndef avms_lhs_hpp
#define avms_lhs_hpp

#include <apf.h>
#include <apfDynamicMatrix.h>

namespace apf {
class Mesh2;
}

namespace avms {

using Vector = apf::Vector3;

class LHS : public apf::Integrator {
  public:
    LHS(
        apf::Mesh2* mesh,
        double k,
        Vector const& a,
        bool is_dual);
    void inElement(apf::MeshElement* me);
    void atPoint(Vector const&, double w, double dv);
    apf::DynamicMatrix Ke;
  private:
    int dim;
    int nodes;
    double k;
    Vector a;
    apf::Mesh2* mesh;
    apf::MeshElement* elem;
    apf::FieldShape* shape;
};

}

#endif
