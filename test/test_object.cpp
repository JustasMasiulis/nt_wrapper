#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <obj/object.hpp>
#include <obj/object_info.hpp>

#pragma comment(lib, "ntdll.lib")

TEST_CASE("object")
{
    ntw::obj::unique_object obj{ NtCurrentProcess() };
}
