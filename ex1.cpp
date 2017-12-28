#include <apf.h>
#include <PCU.h>
#include <petsc.h>
#include <Omega_h_library.hpp>
#include "disc.hpp"
#include "adapt.hpp"
#include "solver.hpp"

namespace {

using Vector = apf::Vector3;

auto gfile = "../msh/lshape/lshape.dmg";
auto mfile = "../msh/lshape/lshape.smb";
auto afile = "../ex1_adapt.yaml";
auto ofile = "out_ex1";
double k = 0.001;
Vector a = {-1,1,0};
int adapt_iters = 5;

double f(Vector const&) { return 1.0; }
double q(Vector const&) { return 1.0; }

static void run(int argc, char** argv) {
  int at = 0;
  Omega_h::Library lib(&argc, &argv);
  avms::Disc disc(gfile, mfile, ofile);
  for (int i = 0; i < adapt_iters; ++i) {
    avms::solve(&disc, k, a, f, false);
    avms::solve(&disc, k, a, q, true);
    avms::adapt(&lib, &disc, afile);
    disc.update();
    disc.write(at++);
  }
  disc.write_pvd(at);
}

}

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);
  PCU_Comm_Init();
  PetscInitialize(&argc, &argv, 0, 0);
  run(argc, argv);
  PetscFinalize();
  PCU_Comm_Free();
  MPI_Finalize();
}
