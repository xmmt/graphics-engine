#include <engine/platform/threading.hpp>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace engine {
namespace platform {
namespace threading {

void set_affinity_for_current_thread(uint64_t affinity_mask) {
    auto handle = GetCurrentThread();
    SetThreadAffinityMask(handle, affinity_mask);
}

} // namespace threading
} // namespace platform
} // namespace engine