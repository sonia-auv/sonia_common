# `lib_atlas/pattern/singleton.h`

This class is an implementation of the GOF pattern Singleton.

This aims to provide a class to extend in order to have a unique object
through the whole runtime. You can extends this class to have the behavior
applied to your own class.

### Synopsis
***
```Cpp
    namespace sonia_common {
    
    template <class T>
    class Singleton {
     public:
      explicit Singleton(const Singleton &) noexcept = delete;
      virtual ~Singleton() noexcept = default;
    
      void operator=(const Singleton &) noexcept = delete;
      static T &instance() noexcept(std::is_nothrow_constructible<Tp_>::value);
    
     protected:
      explicit Singleton() ATLAS_NOEXCEPT = default;
    };
    
    }  // namespace sonia_common
```

### Usage
***

```Cpp
    class Foo: public Singleton<Foo>
    {
     private:
      explicit Foo() {};
      ~Foo() {};
      
      // Frienship so Singleton<Foo> can access the constructor and destructor.
      friend class Singleton<Foo>;
    };
```
