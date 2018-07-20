# nt_wrapper
A header only wrapper library around native windows sytem APIs.

### Highlights of currently implemented features
System information:
* Handles enumeration -> ntw::system::handles
* Process enumeration -> ntw::system::processes
    * Threads enumeration -> ntw::system::thread_info
* Loaded drivers enumeration -> ntw::system::loaded_drivers
* Memory allocation, locking protection and queries with RAII constructs to avoid leaks -> ntw::memory

Object manipulation:
* Process -> ntw::obj::unique_process
    * Loaded modules enumeration -> ntw::obj::process_module_info
* Thread -> ntw::obj::unique_thread
* File -> ntw::obj::unique_file
* Token -> ntw::obj::unique_token
* Registry -> ntw::obj::unique_registry
* Handle -> ntw::obj::unique_handle

### Design goals
* No exceptions - if a function can fail it will return a status code.
* No overhead - every single abstraction is costless and is as fast as it can be.
* Strict control over imports - every call is made trough a macro which allows turning the lazy importer off or easy switched to direct syscalls.
* Ease of developer use - it is a much more consistent and pleasant to work with compared to win32.

### Why?
The main reason to choose this library over existing windows constructs are:
* Speed - even with lazy_importer you code is likely to be faster compared to win32 API.
* Ease of use - the language of choice is c++ and allows you to use modern alternatives to old C.
* Obfuscation - it is a lot harder to reverse engineer an application which uses obfuscated imports or direct syscalls.
