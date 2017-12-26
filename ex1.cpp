#include <PCU.h>
#include "disc.hpp"

namespace {

auto mfile = "../msh/lshape/lshape.dmg";
auto gfile = "../msh/lshape/lshape.smb";
auto ofile = "out_ex1";

static void run() {
  avms::Disc disc(mfile, gfile, ofile);
  disc.write(0);
}

}

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);
  PCU_Comm_Init();
  run();
  PCU_Comm_Free();
  MPI_Finalize();
}
