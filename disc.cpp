#include "disc.hpp"
#include <apfMDS.h>
#include <gmi_mesh.h>
#include <sstream>
#include <fstream>

namespace avms {

Disc::Disc(
    std::string const& gfile,
    std::string const& mfile,
    std::string const& ofile) {
  gmi_register_mesh();
  auto m = mfile.c_str();
  auto g = gfile.c_str();
  mesh = apf::loadMdsMesh(g, m);
  apf::verify(mesh);
  dim = mesh->getDimension();
  nmbr = 0;
  outfile = ofile;
  update();
}

Disc::~Disc() {
  if (nmbr) apf::destroyNumbering(nmbr);
  mesh->destroyNative();
  apf::destroyMesh(mesh);
}

void Disc::update() {
  if (nmbr) apf::destroyNumbering(nmbr);
  nmbr = apf::numberOwnedNodes(mesh, "n");
  nodes = apf::countNodes(nmbr);
}

void Disc::write(int i) {
  auto n = outfile + "_" + std::to_string(i);
  apf::writeVtkFiles(n.c_str(), mesh);
}

void Disc::write_pvd(const int end) {
  std::string pvd = outfile + ".pvd";
  std::fstream pvdf;
  pvdf.open(pvd.c_str(), std::ios::out);
  pvdf << "<VTKFile type=\"Collection\" version=\"0.1\">" << std::endl;
  pvdf << "  <Collection>" << std::endl;
  for (int t=0; t < end; ++t) {
    std::ostringstream oss;
    oss << outfile << "_" << t;
    std::string vtu = oss.str();
    pvdf << "    <DataSet timestep=\"" << t << "\" group=\"\" ";
    pvdf << "part=\"0\" file=\"" << vtu << "/" << vtu;
    pvdf << ".pvtu\"/>" << std::endl;
  }
  pvdf << "  </Collection>" << std::endl;
  pvdf << "</VTKFile>" << std::endl;
}

}
