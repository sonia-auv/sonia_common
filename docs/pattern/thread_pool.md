# `lib_atlas/pattern/thread_pool.h`

This header provide an easy to use Thread Pool.

It has been implemented using the last features of C++ for managing concurrecy.
Its interface is based on this simple yet really powerfull
[Thread Pool](https://github.com/progschj/ThreadPool)

More generally, a thread pool is usefull when you want to save the creation
and the deletion of threads. Let's say for exemple, that you will have to
create several threads for image processing in your control loop, well you
should really consider using a Thread Pool.

### Synopsis
***

```Cpp
    namespace sonia_common {
    
    class ThreadPool {
     public:
      ThreadPool(size_t);
      template <class T, class... Args>
      std::future<typename std::result_of<T(Args...)>::type> Enqueue(
          T &&f, Args &&... args);
      ~ThreadPool();
    };
    
    }  // namespace sonia_common
```

### Usage
***

```Cpp
    #include <iostream>
    #include <vector>
    #include <chrono>
    
    #include <thread_pool.h"
    
    int main() {
      ThreadPool pool(4);
      std::vector<std::future<int> > results;
    
      for (int i = 0; i < 8; ++i) {
        results.emplace_back(pool.enqueue([i] {
          std::cout << "hello " << i << std::endl;
          std::this_thread::sleep_for(std::chrono::seconds(1));
          std::cout << "world " << i << std::endl;
          return i * i;
        }));
      }
    
      for (auto &&result : results) {
        std::cout << result.get() << ' ';
      }
      std::cout << std::endl;
    
      return 0;
    }
```
