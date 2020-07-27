# `lib_atlas/pattern/observer.h`

### Synopsis
***

```Cpp
    namespace sonia_common {
    
    template <typename... Args>
    class Observer {
     public:
      explicit Observer() noexcept;
      explicit Observer(Subject<Args...> &subject) noexcept;
      virtual ~Observer() noexcept;
      void Observe(Subject<Args...> &subject);
    
     protected:
      virtual void OnSubjectNotify(const Subject<Args...> &subject,
                                   Args &&... args) noexcept = 0;
      virtual void OnSubjectConnected(Subject<Args...> &subject) noexcept;
      virtual void OnSubjectDisconnected(const Subject<Args_...> &subject) noexcept;
      bool IsAttached(const Subject<Args_...> &subject) const noexcept;
      void DetachFromAllSubject() noexcept;
    
      std::vector<const std::shared_ptr<Subject<Args_...>>> subjects_;
      std::mutex subjects_mutex_;
    };
    
    }  // namespace sonia_common
```

### Usage
***
