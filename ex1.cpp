#include <Omega_h.hpp>
#include <Omega_h_assoc.hpp>
#include <iostream>

namespace avms {

static char const* type_name[Omega_h::NSET_TYPES] = {
  "elem set", "side set", "node set" };

struct Disc {
  Disc(int argc, char** argv);
  void print();
  Omega_h::Library lib;
  Omega_h::Mesh mesh;
  Omega_h::Assoc assoc;
  Omega_h::MeshSets sets;
};

Disc::Disc(int argc, char** argv) :
  lib( Omega_h::Library(&argc, &argv) ),
  mesh( Omega_h::gmsh::read(argv[1], lib.world()) ) {
    Omega_h::update_from_file(&assoc, argv[2]);
    sets = Omega_h::invert(&mesh, assoc); 
}

void Disc::print() {
  if (! Omega_h::can_print(&mesh)) return;
  std::cout << "num elems: " << mesh.nelems() << std::endl;
  std::cout << "num verts: " << mesh.nverts() << std::endl;
  for (size_t set_type = 0; set_type < Omega_h::NSET_TYPES; ++set_type) {
    std::cout << "num " << type_name[set_type] << " ";
    std::cout << assoc[set_type].size() << std::endl;
    for (auto& name_pairs : assoc[set_type]) {
      auto name = name_pairs.first;
      std::cout << type_name[set_type] << " '" << name << "' ";
      std::cout << "has size: " << sets[set_type][name].size();
      std::cout << std::endl;
    }
  }
}

struct Data {
  Data(char** argv);
  void print(Disc* d);
  double kappa;
  Omega_h::Vector<2> alpha;
};

Data::Data(char** argv) {
  kappa = std::stod(argv[3]);
  alpha[0] = std::stod(argv[4]);
  alpha[1] = std::stod(argv[5]);
}

void Data::print(Disc* d) {
  if (! Omega_h::can_print(&(d->mesh))) return;
  std::cout << "kappa: " << kappa << std::endl;
  std::cout << "alpha: [" << alpha[0] << ", " << alpha[1] << "]\n";
}

}

int main(int argc, char** argv) {
  assert(argc == 6); 
  avms::Disc disc(argc, argv);
  avms::Data data(argv);
  disc.print();
  data.print(&disc);
}
