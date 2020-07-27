# `lib_atlas/pattern/subject.h`

### Synopsis
***

```Cpp
    namespace sonia_common {
    
    template <typename... Args>
    class Subject {
     public:
      explicit Subject() noexcept;
      virtual ~Subject() noexcept;
    
      void Notify(Args &&... args) noexcept;
      std::size_t ObserverCount() const noexcept;
      void Attach(Observer<Args...> &observer);
      void Detach(Observer<Args...> &observer);
      void DetachNoCallback(const Observer<Args...> &observer) noexcept;
    };
    
    }  // namespace sonia_common
```

### Usage
***
