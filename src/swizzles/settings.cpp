#include "settings.hpp"

namespace swizzles::settings {

Spin hash = Spin("Hash", 1, 128, 1);

Spin threads = Spin("Threads", 1, 4, 1);

}  // namespace swizzles::settings
