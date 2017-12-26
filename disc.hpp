#ifndef avms_disc_hpp
#define avms_disc_hpp

#include <apf.h>
#include <apfMesh2.h>
#include <apfNumbering.h>

namespace apf {
class Mesh2;
template <class T> class NumberingOf;
typedef NumberingOf<int> Numbering;
};

namespace avms {

struct Disc {
  Disc(
      std::string const& gfile,
      std::string const& mfile,
      std::string const& ofile);
  ~Disc();
  void update();
  void write(int i);
  void write_pvd(int end);
  int dim;
  int nodes;
  apf::Mesh2* mesh;
  apf::Numbering* nmbr;
  std::string outfile;
};

}

#endif
