#include <Omega_h.hpp>
#include <Omega_h_assoc.hpp>
#include <iostream>

namespace avms {

static char const* set_names[Omega_h::NSET_TYPES] = {
  "elem set", "side set", "node set" };

static void print_mesh_stats(Omega_h::Mesh* m) {
  if (! Omega_h::can_print(m)) return;
  std::cout << "num elems: " << m->nelems() << std::endl;
  std::cout << "num verts: " << m->nverts() << std::endl;
}

static void print_assoc_stats(
    Omega_h::Mesh* m,
    Omega_h::Assoc& a,
    Omega_h::MeshSets& s) {
  if (! Omega_h::can_print(m)) return;
  std::cout << "num elem sets: " << a[Omega_h::ELEM_SET].size() << std::endl;
  std::cout << "num side sets: " << a[Omega_h::SIDE_SET].size() << std::endl;
  std::cout << "num node sets: " << a[Omega_h::NODE_SET].size() << std::endl;
  for (size_t set_type = 0; set_type < Omega_h::NSET_TYPES; ++set_type) {
    for (auto& name_pairs : a[set_type]) {
      auto name = name_pairs.first;
      std::cout << set_names[set_type] << " '" << name << "' ";
      std::cout << "has size : " << s[set_type][name].size();
      std::cout << std::endl;
    }
  }
}

}

int main(int argc, char** argv) {
  auto lib = Omega_h::Library(&argc, &argv);
  auto mesh = Omega_h::gmsh::read("./msh/lshape/lshape.msh", lib.world());
  Omega_h::Assoc assoc;
  Omega_h::update_from_file(&assoc, "./msh/lshape/assoc.txt");
  auto sets = Omega_h::invert(&mesh, assoc);
  avms::print_mesh_stats(&mesh);
  avms::print_assoc_stats(&mesh, assoc, sets);
  Omega_h::vtk::write_vtu("debug.vtu", &mesh);
}
