#include <ntw/info/memory.hpp>
#include <ntw/ob/process.hpp>
#define CATCH_CONFIG_MAIN
#define WIN32_NO_STATUS
#include <catch2/catch.hpp>

#pragma comment(lib, "ntdll.lib")

TEST_CASE("enumerating memory::basic_info")
{
    std::size_t iterations = 0;
    auto        mem = ntw::ob::process_ref{}.query_mem<ntw::memory::basic_info>(0ull);
    while(true) {
        REQUIRE((mem || iterations > 16));
        if(!mem)
            break;

        ++iterations;
        mem = ntw::ob::process_ref{}.query_mem<ntw::memory::basic_info>(mem->end());
    }
}