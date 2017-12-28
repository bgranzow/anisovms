#ifndef avms_adapt_hpp
#define avms_adapt_hpp

namespace Omega_h {
class Library;
}

namespace avms {

struct Disc;

void adapt(
    Omega_h::Library* lib,
    Disc* d,
    std::string const& afile);

}

#endif
