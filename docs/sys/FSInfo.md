# `lib_atlas/sys/fsinfo.h`

### Synopsis
***

```Cpp
    namespace sonia_common {
    
    namespace fsinfo {
    
    static double total_physical_memory(BitUnit unit = BitUnit::KB,
                                        const char *path = ".") noexcept;
    
    static double free_physical_memory(BitUnit unit = BitUnit::KB,
                                       const char *path = ".") noexcept;
    
    static double available_physical_memory(BitUnit unit = BitUnit::KB,
                                            const char *path = ".") noexcept;
    
    static double used_physical_memory(BitUnit unit = BitUnit::KB,
                                       const char *path = ".") noexcept;
    
    static double percentage_used_physical_memory(const char *path = ".") noexcept;
    
    static double percentage_available_physical_memory(
        const char *path = ".") noexcept;
    
    static unsigned long block_size(const char *path = ".") noexcept;
    
    static unsigned long max_filename(const char *path = ".") noexcept;
    
    }  // namespace fsinfo
    
    }  // namespace sonia_common
```

### Usage
***

Here is an exemple that demonstrate a very simple usage of the available_physical_memory
function.

```Cpp
    #include <iostream>
    #include <math.h>
    #include <sonia_common/sys/fsinfo.h>
    auto avail_space = static_cast<int>(round(
        atlas::sys::available_physical_memory(atlas::sys::BitUnit::MB)));
    
    // Creating a file of 32MB
    unsigned int size = 32;
    FILE *fp = fopen("/tmp/atlas_fsinfo_test", "w");
    ftruncate(fileno(fp), size * 1024 * 1024);
    fclose(fp);
    
    auto new_avail_space = static_cast<int>(round(
        atlas::sys::available_physical_memory(atlas::sys::BitUnit::MB)));
    
    // Removing the created file
    remove("/tmp/atlas_fsinfo_test");
    
    // Displaying the datas.
    std::cout << "Available size before: " << avail_space std::endl
              << "Available size after:  " << new_avail_space;
```

To display the informations without any type conversion, you could
also simply do:

```Cpp
    #include <iostream>
    #include <sonia_common/sys/fsinfo.h>
    
    std::cout << "Total Disk space:"
              << atlas::sys::total_physical_memory(atlas::sys::BitUnit::GB)
              << "GB" std::endl
              << "Available Disk space:"
              << atlas::sys::available_physical_memory(
                     atlas::sys::BitUnit::GB) << "GB" std::endl
              << "Percentage Used:"
              << atlas::sys::percentage_used_physical_memory() * 100. << "%";
```
