#ifndef avms_sys_hpp
#define avms_sys_hpp

#include <petsc.h>

namespace avms {

struct Disc;

class Sys {
  public:
    Sys(int n);
    ~Sys();
    void add_to_vector(int sz, int* rows, double* vals);
    void add_to_matrix(int sz, int* rows, double* vals);
    void zero_rows(int sz, int* rows);
    void synchronize();
    void solve();
    void attach(Disc* d, bool is_dual);
  private:
    Mat A;
    Vec x;
    Vec b;
};

}

#endif
