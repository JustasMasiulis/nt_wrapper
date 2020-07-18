#include <ntw/ob/thread.hpp>
#define CATCH_CONFIG_MAIN
#define WIN32_NO_STATUS
#include <catch2/catch.hpp>

#pragma comment(lib, "ntdll.lib")

TEST_CASE("thread creation")
{
    REQUIRE(ntw::ob::thread::create().local([](void*) -> NTSTATUS { return 0; }));
}

TEST_CASE("thread enumeration")
{
    auto first = ntw::ob::thread::first(ntw::ob::thread_access{}.all());
    REQUIRE(first);
    while(true) {
        first = first->next(ntw::ob::thread_access{}.all());
        if(first.status() == STATUS_NO_MORE_ENTRIES)
            break;

        REQUIRE(first);
    }
}