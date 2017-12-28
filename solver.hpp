#ifndef avms_solver_hpp
#define avms_solver_hpp

#include <functional>

namespace apf {
class Vector3;
}

namespace avms {

using Vector = apf::Vector3;

struct Disc;

void solve(
    Disc* disc,
    double k,
    Vector const& a,
    std::function<double(Vector const& x)> f,
    bool is_dual);

}

#endif
