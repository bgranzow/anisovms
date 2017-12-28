#ifndef avms_disc_hpp
#define avms_disc_hpp

#include <apf.h>
#include <apfMesh2.h>
#include <apfNumbering.h>

struct gmi_model;

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
  void clear();
  void update();
  void write(int i);
  void write_pvd(int end);
  int dim;
  int nodes;
  gmi_model* model;
  apf::Mesh2* mesh;
  apf::Numbering* nmbr;
  apf::Field* u;
  apf::Field* z;
  std::string outfile;
};

}

#endif
