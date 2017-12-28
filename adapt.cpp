#include <apfMDS.h>
#include <apfOmega_h.h>
#include <Omega_h_mesh.hpp>
#include <Omega_h_adapt.hpp>
#include <Omega_h_teuchos.hpp>
#include <Teuchos_YamlParameterListHelpers.hpp>
#include "adapt.hpp"
#include "disc.hpp"

namespace avms {

void adapt(Omega_h::Library* lib, Disc* disc, std::string const& afile) {

  // transfer from apf to omega_h
  auto osh_mesh = Omega_h::Mesh(lib);
  apf::to_omega_h(&osh_mesh, disc->mesh);
  disc->clear();

  // update parameters adapt params from input file
  auto params = Teuchos::getParametersFromYamlFile(afile);
  Omega_h::AdaptOpts adapt_opts(disc->dim);
  Omega_h::MetricInput metric_opts;
  Omega_h::update_adapt_opts(&adapt_opts, *params);
  auto& metric_pl = params->sublist("Metric");
  Omega_h::update_metric_input(&metric_opts, metric_pl);

  // adapt the mesh - godspeed
  Omega_h::add_implied_metric_tag(&osh_mesh);
  Omega_h::generate_target_metric_tag(&osh_mesh, metric_opts);
  while (Omega_h::approach_metric(&osh_mesh, adapt_opts)) {
    Omega_h::adapt(&osh_mesh, adapt_opts);
  }

  // transfer back from omega_h to apf
  disc->mesh = apf::makeEmptyMdsMesh(disc->model, disc->dim, false);
  apf::disownMdsModel(disc->mesh);
  apf::from_omega_h(disc->mesh, &osh_mesh);
  osh_mesh = Omega_h::Mesh(lib);
  disc->mesh->verify();
}

}
