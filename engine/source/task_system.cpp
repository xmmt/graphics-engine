#include <engine/task_system.hpp>
#include <engine/platform/threading.hpp>

using namespace engine;
using namespace task_system;

timer::timer() noexcept {
    reset();
}

timer::timer(double ratio) noexcept
    : ratio_{ ratio } {
    reset();
}

chrono_clock::time_point timer::now() const noexcept {
    return start_time_point_ + elapsed_;
}

chrono_clock::time_point timer::real_now() const noexcept {
    return chrono_clock::now();
}

chrono_clock::duration timer::elapsed() const noexcept {
    return elapsed_;
}

chrono_clock::duration timer::elapsed(chrono_clock::time_point time_point) const noexcept {
    return time_point - start_time_point_;
}

chrono_clock::duration timer::real_elapsed() const noexcept {
    return real_elapsed_;
}

void timer::reset() noexcept {
    start_time_point_ = chrono_clock::now();
    elapsed_ = chrono_clock::duration::zero();
    real_elapsed_ = chrono_clock::duration::zero();
}

bool timer::set_ratio(double ratio) noexcept {
    if (ratio < MIN_TIMER_RATIO || ratio > MAX_TIMER_RATIO) {
        return false;
    }
    ratio_ = ratio;
    return true;
}

void timer::update() noexcept {
    auto new_real_elapsed = chrono_clock::now() - start_time_point_;
    auto new_elapsed = new_real_elapsed - real_elapsed_;
    real_elapsed_ = new_real_elapsed;
    elapsed_ += chrono_clock::duration{ static_cast<int64_t>(new_elapsed.count() * ratio_) };
}

/////////////////////////////////////////////////////////////////////////////////////////////////

thread_pool::thread_pool()
    : thread_pool{ MAX_THREADS_COUNT } {
}

thread_pool::thread_pool(int32_t threads_count, uint64_t affinity_mask)
    : threads_{ threads_count }
    , threads_count_{ threads_count }
    , affinity_mask_{ affinity_mask } {
}

void thread_pool::submit(std::function<void()> fn) {
    submit(
      [fn = std::move(fn)](std::stop_token) {
          fn();
      });
}

void thread_pool::submit(std::function<void(std::stop_token)> fn) {
    auto lg = std::lock_guard{ mtx_ };
    fn_queue_.emplace_back(std::move(fn));
}

void thread_pool::init_() {
    for (auto i = 0; i < threads_count_; ++i) {
        threads_[i] = std::jthread{
            [this, i](std::stop_token stop_token) {
                set_affinity_();
                thread_loop_(stop_token, i);
            }
        };
    }
}

void thread_pool::thread_loop_(std::stop_token stop_token, int32_t thread_id) {
    auto lock = std::unique_lock{ mtx_, std::defer_lock };
    while (not stop_token.stop_requested()) {

        lock.lock();
        cond_var_.wait(lock, [this] {
            return not fn_queue_.empty();
        });
        auto fn = std::move(fn_queue_.front());
        fn_queue_.pop_front();
        lock.unlock();

        fn(stop_token);
        std::this_thread::yield();
    }
}

void thread_pool::set_affinity_() {
    platform::threading::set_affinity_for_current_thread(affinity_mask_);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

task_runner::task_runner() {
}