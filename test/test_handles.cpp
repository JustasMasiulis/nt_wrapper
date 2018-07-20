#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <system/handles.hpp>

TEST_CASE("handles enumeration once")
{
    ntw::system::handles handles;
    REQUIRE(NT_SUCCESS(handles.update()));

    for(auto& handle : handles) {
        handle.object();
        handle.value();
        handle.pid();
        handle.access();
        handle.type_index();
        handle.attributes();
        handle.is_thread();
        handle.is_process();
    }
}

TEST_CASE("handles enumeration multiple")
{
    ntw::system::handles handles;

    for(int i = 0; i < 10; ++i) {
        REQUIRE(NT_SUCCESS(handles.update()));
        for(auto& handle : handles)
            ;
    }
}