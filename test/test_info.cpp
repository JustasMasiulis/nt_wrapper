#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <system/info.hpp>

TEST_CASE("firmware info")
{
    ntw::system::firmware_info('RSMB', 0, [](uint8_t* buffer, unsigned long size) { ; });
}
