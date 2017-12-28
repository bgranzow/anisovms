#include "disc.hpp"
#include <apfMDS.h>
#include <gmi_mesh.h>
#include <sstream>
#include <fstream>
#include <cassert>

namespace avms {

Disc::Disc(
    std::string const& gfile,
    std::string const& mfile,
    std::string const& ofile) {
  gmi_register_mesh();
  auto m = mfile.c_str();
  auto g = gfile.c_str();
  mesh = apf::loadMdsMesh(g, m);
  model = mesh->getModel();
  apf::disownMdsModel(mesh);
  apf::verify(mesh);
  dim = mesh->getDimension();
  nmbr = 0;
  outfile = ofile;
  u = apf::createFieldOn(mesh, "u", apf::SCALAR);
  z = apf::createFieldOn(mesh, "z", apf::SCALAR);
  update();
}

Disc::~Disc() {
  if (nmbr) apf::destroyNumbering(nmbr);
  if (mesh) mesh->destroyNative();
  if (mesh) apf::destroyMesh(mesh);
  if (model) gmi_destroy(model);
}

void Disc::clear() {
  if (nmbr) apf::destroyNumbering(nmbr);
  if (u) apf::destroyField(u);
  if (z) apf::destroyField(z);
  mesh->destroyNative();
  apf::destroyMesh(mesh);
  mesh = 0;
  nmbr = 0;
  u = 0;
  z = 0;
}

void Disc::update() {
  if (nmbr) apf::destroyNumbering(nmbr);
  nmbr = apf::numberOwnedNodes(mesh, "n");
  nodes = apf::countNodes(nmbr);
  u = mesh->findField("u");
  z = mesh->findField("z");
  assert(u);
  assert(z);
  auto s = mesh->findField("size");
  auto q = mesh->findField("quality");
  if (s) apf::destroyField(s);
  if (q) apf::destroyField(q);
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
