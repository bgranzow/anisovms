#ifndef avms_primal_hpp
#define avms_primal_hpp

#include <functional>

namespace apf {
class Vector3;
}

namespace avms {

using Vector = apf::Vector3;

struct Disc;

void solve_primal(
    Disc* disc,
    double k,
    Vector const& a,
    std::function<double(Vector const& x)> f);

}

#endif
