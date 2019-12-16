# nt_wrapper
A header only wrapper library around native windows system APIs.

### Highlights of currently implemented features
System information:
* [Handles enumeration](https://github.com/JustasMasiulis/nt_wrapper/blob/e2ff4b930c189b55548b4ba805756d1abf38053f/test/test_handles.cpp#L7) -> ntw::system::handles
* [Process enumeration](https://github.com/JustasMasiulis/nt_wrapper/blob/e2ff4b930c189b55548b4ba805756d1abf38053f/test/test_processes.cpp#L7) -> ntw::system::processes
    * Threads enumeration -> ntw::system::thread_info
* [Loaded drivers enumeration](https://github.com/JustasMasiulis/nt_wrapper/blob/e2ff4b930c189b55548b4ba805756d1abf38053f/test/test_loaded_drivers.cpp#L10) -> ntw::system::loaded_drivers
* [Memory operations with RAII constructs](https://github.com/JustasMasiulis/nt_wrapper/blob/2f9ca30dc3098bf6884f325c77b44f77f2845f5a/test/test_memory.cpp#L132) -> ntw::memory

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
* Strict control over imports - every call is made through a macro which allows turning the lazy importer off or easy switched to direct syscalls.
* Ease of developer use - it is much more consistent and pleasant to work with compared to win32.

### Why?
The main reason to choose this library over existing windows constructs are:
* Speed - even with lazy_importer you code is likely to be faster compared to win32 API.
* Ease of use - the language of choice is c++ and allows you to use modern alternatives to old C.
* Obfuscation - it is a lot harder to reverse engineer an application which uses obfuscated imports or direct syscalls.

### Acknowledgements
nt_wrapper uses modified [headers](https://github.com/processhacker/processhacker/tree/master/phnt) licensed under GNU GPL v3 that were taken from [process hacker](https://github.com/processhacker/processhacker).
