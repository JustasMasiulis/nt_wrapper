#include <ntw/ob/token.hpp>
#define CATCH_CONFIG_MAIN
#define WIN32_NO_STATUS
#include <catch2/catch.hpp>

#pragma comment(lib, "ntdll.lib")

TEST_CASE("token access correctly default constructed")
{
    REQUIRE(ntw::ob::token_access{}.get() == 0);
}

TEST_CASE("token access building works")
{
    const auto tok = ntw::ob::token_access{}
                         .adjust_default()
                         .adjust_groups()
                         .adjust_privileges()
                         .adjust_sessionid()
                         .assign_primary()
                         .duplicate()
                         .execute()
                         .impersonate()
                         .query()
                         .query_source()
                         .all();

    REQUIRE(tok.get() == TOKEN_ALL_ACCESS);
}

TEST_CASE("token open works")
{
    const auto tok = ntw::ob::token::open(
        ntw::ob::process_ref{}, ntw::ob::token_access{}.adjust_privileges());

    INFO(std::hex << tok.status().get());
    REQUIRE(tok);
    REQUIRE(tok->get() != nullptr);
}

TEST_CASE("token.reset_privileges")
{
    const auto tok = ntw::ob::token::open(
        ntw::ob::process_ref{}, ntw::ob::token_access{}.adjust_privileges());

    REQUIRE(tok);
    REQUIRE(tok->reset_privileges().success());
}

TEST_CASE("token.replace_privilege")
{
    const auto tok = ntw::ob::token::open(
        ntw::ob::process_ref{}, ntw::ob::token_access{}.adjust_privileges().query());

    REQUIRE(tok);
    const auto res = tok->replace_privilege(ntw::ob::privilege::debug().enable());
    REQUIRE(res.success());
}

TEST_CASE("token.adjust_privilege")
{
    const auto tok = ntw::ob::token::open(
        ntw::ob::process_ref{}, ntw::ob::token_access{}.adjust_privileges().query());
    REQUIRE(tok);
    const auto res = tok->adjust_privilege(ntw::ob::privilege::debug().enable());
    REQUIRE(res);
}