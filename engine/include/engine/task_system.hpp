#pragma once

#include <engine/task_system/constants.hpp>

#include <chrono>
#include <deque>
#include <functional>
#include <thread>
#include <mutex>

namespace engine {
namespace task_system {

using chrono_clock = std::chrono::high_resolution_clock;

class timer {
public:
    timer() noexcept;
    explicit timer(double ratio) noexcept;

    timer(timer const&) = delete;
    timer(timer&&) = default;
    timer& operator=(timer const&) = delete;
    timer& operator=(timer&&) = default;

    ~timer() = default;

public:
    chrono_clock::time_point now() const noexcept;
    chrono_clock::time_point real_now() const noexcept;
    chrono_clock::duration elapsed() const noexcept;
    chrono_clock::duration elapsed(chrono_clock::time_point time_point) const noexcept;
    chrono_clock::duration real_elapsed() const noexcept;
    void reset() noexcept;
    bool set_ratio(double ratio) noexcept;
    void update() noexcept;

private:
    chrono_clock::time_point start_time_point_{ chrono_clock::now() };
    chrono_clock::duration elapsed_{ chrono_clock::duration::zero() };
    chrono_clock::duration real_elapsed_{ chrono_clock::duration::zero() };
    double ratio_{ 1. };
};

class thread_pool {
public:
    thread_pool();
    explicit thread_pool(int32_t threads_count, uint64_t affinity_mask = ~0ull);

    thread_pool(thread_pool const&) = delete;
    thread_pool(thread_pool&&) = default;
    thread_pool& operator=(thread_pool const&) = delete;
    thread_pool& operator=(thread_pool&&) = default;

    ~thread_pool() = default;

public:
    void submit(std::function<void()> fn);
    void submit(std::function<void(std::stop_token)>);

private:
    void init_();
    void thread_loop_(std::stop_token stop_token, int32_t thread_id);
    void set_affinity_();

private:
    std::vector<std::jthread> threads_;
    int32_t threads_count_{ MAX_THREADS_COUNT };
    uint64_t affinity_mask_{ ~0ull };
    std::deque<std::function<void(std::stop_token)>> fn_queue_;
    std::mutex mtx_;
    std::condition_variable cond_var_;
};

class task_runner {
public:
    task_runner();
    ~task_runner() = default;
    template <typename Fn>
    void submit(Fn&& fn) {
        work_threads_.submit(std::forward<Fn>(fn));
    }
    template <typename Fn>
    void submit_ui(Fn&& fn) {
        ui_thread_.submit(std::forward<Fn>(fn));
    }

private:
    thread_pool ui_thread_{ 1 };
    thread_pool work_threads_{ std::thread::hardware_concurrency() - 1 };
};


using tick_number = int64_t;
using tick_frequency = int64_t;
using callback = std::function<void()>;
using callback_id = int64_t;

class tick_generator {
public:
    tick_generator(tick_frequency frequency);
    callback_id add_callback(callback callback);
    bool remove_callback(tick_number number);
    bool set_frequency(tick_frequency frequency);

private:
    tick_frequency frequency_;
    std::deque<std::tuple<callback_id, callback>> callbacks_;
};
} // namespace task_system
} // namespace engine