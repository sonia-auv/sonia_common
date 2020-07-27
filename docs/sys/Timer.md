# `lib_atlas/sys/thread_pool.h`

This header provide an easy to use Thread Pool.

It has been implemented using the last features of C++ for managing concurrecy.
Its interface is based on this simple yet really powerfull [Thread Pool](https://github.com/progschj/ThreadPool)

More generally, a thread pool is usefull when you want to save the creation
and the deletion of threads. Let's say for exemple, that you will have to
create several threads for image processing in your control loop, well you
should really consider using a Thread Pool.

### Synopsis
***

```Cpp
    namespace sonia_common {
    
    template <class Ut_ = std::chrono::milliseconds,
              class Tp_ = std::chrono::steady_clock>
    class Timer {
     public:
      explicit Timer() noexcept;
    
      ~Timer() noexcept;
    
      static int64_t now() noexcept;
    
      static void sleep(long int sleeping_time) noexcept;
    
      void start() noexcept;
    
      void pause();
    
      void unpause();
    
      void reset() noexcept;
    
      bool is_running() noexcept;
    
      template <class Yp_ = Ut_>
      double time() const noexcept;
    
      int64_t nanoseconds() const noexcept;
    
      int64_t microseconds() const noexcept;
    
      int64_t milliseconds() const noexcept;
    
      int64_t seconds() const noexcept;
    
      int64_t minutes() const noexcept;
    
      int64_t hours() const noexcept;
    };
    
    }  // namespace sonia_common
```

### Usage
***

```Cpp
    #include <iostream>
    #include <sonia_common/sys/timer>
    
    atlas::NanoTimer timer;
    
    timer.start();
    timer.sleep(5000);
    std::cout << "Total: " << timer.time() << "s = " << timer.microseconds()
              << "ms = " << timer.nanoseconds() << "ns.";
```
