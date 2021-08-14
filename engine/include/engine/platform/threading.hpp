#pragma once

#include <cstdint>

namespace engine {
namespace platform {
namespace threading {

void set_affinity_for_current_thread(uint64_t affinity_mask);

} // namespace threading
} // namespace platform
} // namespace engine