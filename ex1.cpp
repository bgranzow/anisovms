#include <apf.h>
#include <PCU.h>
#include <petsc.h>
#include "disc.hpp"
#include "primal.hpp"

namespace {

using Vector = apf::Vector3;

auto mfile = "../msh/lshape/lshape.dmg";
auto gfile = "../msh/lshape/lshape.smb";
auto ofile = "out_ex1";
double k = 0.001;
Vector a = {-1,1,0};

double f(Vector const&) { return 1.0; }

static void run() {
  avms::Disc disc(mfile, gfile, ofile);
  avms::solve_primal(&disc, k, a, f);
  disc.write(0);
}

}

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);
  PCU_Comm_Init();
  PetscInitialize(&argc, &argv, 0, 0);
  run();
  PetscFinalize();
  PCU_Comm_Free();
  MPI_Finalize();
}
