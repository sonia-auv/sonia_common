/**
 * \file	thread_pool.h
 * \author	Thibaut Mattio <thibaut.mattio@gmail.com>
 * \date	28/06/2015
 *
 * \copyright Copyright (c) 2015 S.O.N.I.A. All rights reserved.
 *
 * \section LICENSE
 *
 * This file is part of S.O.N.I.A. software.
 *
 * S.O.N.I.A. software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * S.O.N.I.A. software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with S.O.N.I.A. software. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SONIA_COMMON_PATTERN_THREAD_POOL_H_
#define SONIA_COMMON_PATTERN_THREAD_POOL_H_

#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <vector>

#include <sonia_common/macros.h>

namespace sonia_common {

//==============================================================================
// C L A S S E S

/**
 * Creates a specific number of thread in order to perform a specific number
 * of tasks.
 * All the thread are organized in a queue an an amount of time will be assigned
 * one by one for the threads.
 *
 * Creating a thread pool can be particulary usefull when you need to create
 * an instance of a task in, for exemple, a loop. You could use a thread pool
 * for your application and simply add a thread in this pool fo your need.
 *
 * This thread pool is based on this open implementation from:
 * https://github.com/progschj/ThreadPool
 */
class ThreadPool {
 public:
  //==========================================================================
  // T Y P E D E F   A N D   E N U M

  using Ptr = std::shared_ptr<ThreadPool>;

  //============================================================================
  // P U B L I C   C / D T O R S

  explicit ThreadPool(size_t) ATLAS_NOEXCEPT;

  ~ThreadPool() ATLAS_NOEXCEPT;

  //============================================================================
  // P U B L I C  M E T H O D S

  template <class Tp_, class... Args_>
  std::future<typename std::result_of<Tp_(Args_...)>::type> Enqueue(
      Tp_ &&f, Args_ &&... args);

 private:
  //============================================================================
  // P R I V A T E   M E M B E R S

  std::vector<std::thread> workers_;

  std::queue<std::function<void()>> tasks_;

  mutable std::mutex queue_mutex_;

  std::condition_variable condition_;

  bool is_stoped_;
};

//==============================================================================
// C / D T O R   S E C T I O N

//------------------------------------------------------------------------------
//
ThreadPool::ThreadPool(size_t threads) ATLAS_NOEXCEPT : workers_(),
                                                        tasks_(),
                                                        queue_mutex_(),
                                                        condition_(),
                                                        is_stoped_(false) {
  for (size_t i = 0; i < threads; ++i) {
    workers_.emplace_back([this] {
      for (;;) {
        std::function<void()> task;

        {
          auto lock = std::unique_lock<std::mutex>{queue_mutex_};
          condition_.wait(lock,
                          [this] { return is_stoped_ || !tasks_.empty(); });
          if (is_stoped_ && tasks_.empty()) {
            return;
          }
          task = std::move(tasks_.front());
          tasks_.pop();
        }

        task();
      }
    });
  }
}

//------------------------------------------------------------------------------
//
ThreadPool::~ThreadPool() ATLAS_NOEXCEPT {
  {
    auto lock = std::unique_lock<std::mutex>{queue_mutex_};
    is_stoped_ = true;
  }
  condition_.notify_all();
  for (std::thread &worker : workers_) {
    worker.join();
  }
}

//==============================================================================
// M E T H O D S   S E C T I O N

//------------------------------------------------------------------------------
//
template <class Tp_, class... Args_>
auto ThreadPool::Enqueue(Tp_ &&f, Args_ &&... args)
    -> std::future<typename std::result_of<Tp_(Args_...)>::type> {
  using return_type = typename std::result_of<Tp_(Args_...)>::type;

  auto task = std::make_shared<std::packaged_task<return_type()>>(
      std::bind(std::forward<Tp_>(f), std::forward<Args_>(args)...));

  std::future<return_type> res = task->get_future();

  {
    auto lock = std::unique_lock<std::mutex>{queue_mutex_};

    if (is_stoped_) {
      throw std::runtime_error("enqueue on stopped ThreadPool");
    }

    tasks_.emplace([task]() { (*task)(); });
  }

  condition_.notify_one();
  return res;
}

}  // namespace sonia_common

#endif  // SONIA_COMMON_PATTERN_THREAD_POOL_H_
