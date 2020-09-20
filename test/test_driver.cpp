#include <ntw/sys/driver.hpp>
#include <ntw/ob/token.hpp>
#define CATCH_CONFIG_MAIN
#define WIN32_NO_STATUS
#include <catch2/catch.hpp>

#pragma comment(lib, "ntdll.lib")

TEST_CASE("driver loader")
{
    using namespace ntw::ob;
    using namespace ntw::sys;

    ntw::sys::load_driver({});
    ntw::sys::unload_driver({});
}