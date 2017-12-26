#include "gmodel.hpp"

using namespace gmod;

Vector vec(double x, double y, double z) {
  return Vector{x,y,z};
}

int main() {
  default_size = 0.2;
  std::vector<Vector> pts = {
    vec(.5,0,0),
    vec(1,0,0),
    vec(1,1,0),
    vec(0,1,0),
    vec(0,.5,0),
    vec(.5,.5,0) };
  auto f = new_polygon(pts);
  write_closure_to_geo(f, "lshape.geo");
  write_closure_to_dmg(f, "lshape.dmg");
}
