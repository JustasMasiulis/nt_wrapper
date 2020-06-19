#include <ntw/sys/processes.hpp>
#define CATCH_CONFIG_MAIN
#define WIN32_NO_STATUS
#include <catch2/catch.hpp>

#pragma comment(lib, "ntdll.lib")

TEST_CASE("acquire_processes works")
{
    std::vector<std::uint8_t> arr(0x100000);
    auto                      processes = ntw::sys::processes(arr);
    REQUIRE(processes);
    for(auto& p : *processes) {
        CHECK(p.offset_to_next != 0);
        if(p.id == 0)
            continue;

        INFO(p.id);
        for(auto& t : p.threads())
            CHECK(t.id != 0);
    }
}