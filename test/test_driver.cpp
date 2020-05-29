#include <ntw/sys/driver_loader.hpp>
#include <ntw/ob/token.hpp>
#define CATCH_CONFIG_MAIN
#define WIN32_NO_STATUS
#include <catch2/catch.hpp>

#pragma comment(lib, "ntdll.lib")

TEST_CASE("driver loader")
{
    using namespace ntw::ob;
    using namespace ntw::sys;

    const auto token = token::open(process_ref{}, token_access{}.adjust_privileges());
    REQUIRE(token);

    REQUIRE(token->replace_privilege(privilege::load_driver().enable()).success());

    const auto driver = driver::load(L"\\??\\C:\\Windows\\System32\\drivers\\MEME.sys",
                                     driver::start::manual_with_cleanup,
                                     driver::error_control::normal,
                                     driver::type::device_driver);

    INFO(std::hex << driver.status().get());
    REQUIRE(driver);
}
