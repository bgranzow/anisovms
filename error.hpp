#ifndef avms_error_hpp
#define avms_error_hpp

#include <apf.h>
#include <functional>

namespace apf {
class Mesh2;
}

namespace avms {

using Vector = apf::Vector3;

class Error : public apf::Integrator {
  public:
    Error(
        Disc* disc,
        double k,
        Vector const& a,
        std::function<double(Vector const& x)> f,
        std::function<double(Vector const& x)> q);
    ~Error();
    void inElement(apf::MeshElement* me);
    void atPoint(Vector const& x, double w, double dv);
    void outElement();
    apf::Field* error;
  private:
    int dim;
    int nodes;
    double k;
    Vector a;
    apf::Mesh2* mesh;
    apf::MeshElement* elem;
    apf::Field* uh;
    apf::Field* zh;
    apf::Element* u_elem;
    apf::Element* z_elem;
    double Je_elem;
    std::function<double(Vector const& x)> f;
    std::function<double(Vector const& x)> q;
};

}

#endif
