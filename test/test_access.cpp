#include <ntw/ob/attributes.hpp>
#include <ntw/access.hpp>
#define CATCH_CONFIG_MAIN
#define WIN32_NO_STATUS
#include <catch2/catch.hpp>

struct mock_access : ntw::access_builder<mock_access> {};

TEST_CASE("access correctly default constructed") { REQUIRE(mock_access{}.get() == 0); }

TEST_CASE("access building works")
{
    auto access = mock_access{}
                      .generic_read()
                      .generic_write()
                      .generic_execute()
                      .generic_all()
                      .system_security()
                      .destroy()
                      .read_security()
                      .synchronize()
                      .write_dac()
                      .write_owner()
                      .get();

    REQUIRE(access == (GENERIC_READ | GENERIC_WRITE | GENERIC_EXECUTE | GENERIC_ALL |
                       ACCESS_SYSTEM_SECURITY | DELETE | READ_CONTROL | SYNCHRONIZE |
                       WRITE_DAC | WRITE_OWNER));
}