#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include "tt.hpp"
#include "ttentry.hpp"
#include "uci/options/spin.hpp"

namespace settings {

extern options::Spin hash;

}  // namespace settings

extern TT<TTEntry> tt;

#endif
