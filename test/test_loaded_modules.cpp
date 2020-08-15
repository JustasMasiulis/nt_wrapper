#include <ntw/sys/modules.hpp>
#define CATCH_CONFIG_MAIN
#define WIN32_NO_STATUS
#include <catch2/catch.hpp>

#pragma comment(lib, "ntdll.lib")

TEST_CASE("loaded_modules")
{
    alignas(16) std::uint8_t arr[0x1000 * 32] = { 0 };
    const auto               r = ntw::sys::loaded_modules(arr);
    INFO(std::hex << r.get());
    REQUIRE(r.success());

    for(const auto& lm : *r)
        CHECK(lm.image_base != 0);
}