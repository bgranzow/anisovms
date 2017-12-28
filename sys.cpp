#include <PCU.h>
#include <iostream>
#include "disc.hpp"
#include "sys.hpp"

namespace avms {

Sys::Sys(int n) {
  VecCreateMPI(PETSC_COMM_WORLD, n, n, &b);
  MatCreateAIJ(PETSC_COMM_WORLD, n, n, n, n,
      300, PETSC_NULL, 300, PETSC_NULL, &A);
  MatSetOption(A, MAT_NEW_NONZERO_ALLOCATION_ERR, PETSC_FALSE);
  VecDuplicate(b, &x);
}

Sys::~Sys() {
  MatDestroy(&A);
  VecDestroy(&x);
  VecDestroy(&b);
}

void Sys::add_to_vector(int sz, int* rows, double* vals) {
  VecSetValues(b, sz, rows, vals, ADD_VALUES);
}

void Sys::add_to_matrix(int sz, int* rows, double* vals) {
  MatSetValues(A, sz, rows, sz, rows, vals, ADD_VALUES);
}

void Sys::zero_rows(int sz, int* rows) {
  double vals[sz];
  for (int i = 0; i < sz; ++i) vals[i] = 0.0;
  VecSetValues(b, sz, rows, vals, INSERT_VALUES);
  MatZeroRows(A, sz, rows, 1.0, PETSC_NULL, PETSC_NULL);
}

void Sys::synchronize() {
  VecAssemblyBegin(b);
  VecAssemblyEnd(b);
  MatAssemblyBegin(A, MAT_FINAL_ASSEMBLY);
  MatAssemblyEnd(A, MAT_FINAL_ASSEMBLY);
}

void Sys::solve() {
  double t0 = PCU_Time();
  KSP solver;
  KSPCreate(PETSC_COMM_WORLD,&solver);
  KSPSetTolerances(solver, 1.0e-10, 1.0e-10, PETSC_DEFAULT, 2000);
  KSPSetOperators(solver, A, A);
  KSPSetFromOptions(solver);
  KSPSolve(solver, b, x);
  double t1 = PCU_Time();
  int its;
  KSPGetIterationNumber(solver, &its);
  KSPDestroy(&solver);
  std::cout << "lin sys solved in " << t1 - t0 << " seconds\n";
  std::cout << "lin sys converged in " << its << " iters\n";
}

void Sys::attach(Disc* d, bool is_dual) {
  auto f = is_dual ? d->z : d->u;
  double* X;
  VecGetArray(x, &X);
  apf::DynamicArray<apf::Node> nodes;
  apf::getNodes(d->nmbr, nodes);
  for (size_t n = 0; n < nodes.getSize(); ++n)
    apf::setScalar(f, nodes[n].entity, 0, X[n]);
  VecRestoreArray(x, &X);
}

}
