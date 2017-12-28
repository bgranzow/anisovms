#include <apf.h>
#include <gmi.h>
#include <iostream>
#include "disc.hpp"
#include "sys.hpp"
#include "lhs.hpp"
#include "rhs.hpp"
#include "primal.hpp"

namespace avms {

static void add_to_system(
    apf::DynamicMatrix& Ke,
    apf::DynamicVector& Fe,
    apf::MeshEntity* e,
    apf::Numbering* n,
    Sys* sys) {
  apf::NewArray<int> numbers;
  int sz = apf::getElementNumbers(n, e, numbers);
  sys->add_to_matrix(sz, &numbers[0], &Ke(0,0));
  sys->add_to_vector(sz, &numbers[0], &Fe(0));
}

static void assemble_volumetric(
    Disc* disc,
    double k,
    Vector const& a,
    std::function<double(Vector const& x)> f,
    Sys* sys) {
  auto m = disc->mesh;
  LHS lhs(m, k, a, false);
  RHS rhs(m, k, a, f);
  apf::MeshEntity* elem;
  auto it = m->begin(disc->dim);
  while ((elem = m->iterate(it))) {
    auto me = apf::createMeshElement(m, elem);
    lhs.process(me);
    rhs.process(me);
    add_to_system(lhs.Ke, rhs.Fe, elem, disc->nmbr, sys);
    apf::destroyMeshElement(me);
  }
  m->end(it);
  sys->synchronize();
}

static void assemble_dbcs(Disc* disc, Sys* sys) {
  auto m = disc->mesh;
  auto model = m->getModel();
  int nbdry = 0;
  gmi_ent* boundary;
  auto boundaries = gmi_begin(model, disc->dim-1);
  while ((boundary = gmi_next(model, boundaries))) {
    apf::DynamicArray<apf::Node> nodes;
    auto b = reinterpret_cast<apf::ModelEntity*>(boundary);
    apf::getNodesOnClosure(m, b, nodes, m->getShape());
    int nnodes = nodes.getSize();
    int rows[nnodes];
    for (int n = 0; n < nnodes; ++n)
      rows[n] = apf::getNumber(disc->nmbr, nodes[n].entity, 0, 0);
    sys->zero_rows(nnodes, rows);
    nbdry++;
  }
  gmi_end(model, boundaries);
  sys->synchronize();
  std::cout << "homongenous dbcs applied to " << nbdry << " boundaries\n";
}

void solve_primal(
    Disc* disc,
    double k,
    Vector const& a,
    std::function<double(Vector const& x)> f) {
  std::cout << "primal solve: " << disc->nodes << " dofs\n";
  Sys sys(disc->nodes);
  assemble_volumetric(disc, k, a, f, &sys);
  assemble_dbcs(disc, &sys);
  sys.solve();
  sys.attach(disc, false);
}

}
