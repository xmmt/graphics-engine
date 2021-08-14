#pragma once

#include <cstdint>

namespace engine {
namespace task_system {

constexpr int32_t const MAX_THREADS_COUNT{ 32 };
constexpr double const MIN_TIMER_RATIO{ 0.0001 };
constexpr double const MAX_TIMER_RATIO{ 10000.0 };

}
} // namespace engine