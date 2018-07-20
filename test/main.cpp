#define NTW_ENABLE_DEBUG_TRACING
#define NTW_INLINE inline
#define NOMINMAX
#include "../include/object/thread.hpp"
#include "../include/mutex.hpp"
#include "../include/acl.hpp"
#include "../include/system/processes.hpp"
#include "../include/object/process.hpp"
#include <algorithm>

int main() noexcept
{
    ntw::system::firmware_info(
        'RSMB', 0, [](uint8_t* buffer, unsigned long size) { std::cout << size; });

    ntw::system::processes processes;
    ret_on_err(processes.update());

    for(auto& process : processes) {
        const auto pid = reinterpret_cast<std::uintptr_t>(process.id());

        auto write_error = [&](NTSTATUS status) {
            std::cerr << "\n\tstatus: " << std::hex << status << "\n\tid: " << std::dec
                      << pid << "\n\timage name: ";
            std::wcerr.write(process.name().Buffer, process.name().Length / 2);
            std::cerr << '\n';
        };

        ntw::obj::unique_process proc;
        auto                     status = proc.open(pid, 0x001F0000L | 0x0000FFFFL);

        if(!NT_SUCCESS(status)) {
            if(status != STATUS_ACCESS_DENIED) {
                std::cerr << "open_process failed";
                write_error(status);
            }
            continue;
        }
        else if(status != STATUS_SUCCESS) {
            std::cerr << "open_process warning";
            write_error(status);
        }

        status = proc.enum_modules([&](const ntw::obj::process_module_info& module) {
            std::wcout.write(module.path_data(), module.path_length());
            std::wcout << '\n';
        });

        if(!NT_SUCCESS(status)) {
            std::cerr << "enum_modules failed\n\thandle: ";
            write_error(status);
            continue;
        }
    }

    /*struct mtest {
        ntw::mutex        mut;
        std::atomic<bool> ready{ false };
    } test;

    test.mut.lock();

    ntw::thread t;
    for(int i = 0; i < 100; ++i)
        t.create(
            [](void* ptest) -> NTSTATUS {
                auto& test = *static_cast<mtest*>(ptest);
                while(!test.ready)
                    ;

                test.mut.lock();
                test.mut.unlock();
                return STATUS_SUCCESS;
            },
            &test);

    Sleep(5000);
    test.ready = true;
    test.mut.unlock();
    Sleep(1000);*/
}
