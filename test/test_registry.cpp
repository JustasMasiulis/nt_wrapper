#include <ntw/io/registry_key.hpp>
#define CATCH_CONFIG_MAIN
#define WIN32_NO_STATUS
#include <catch2/catch.hpp>

#pragma comment(lib, "ntdll.lib")

TEST_CASE("reg_key default ctor")
{
    ntw::io::unique_reg_key key;
    REQUIRE(key.get() == nullptr);
}

TEST_CASE("reg_key open")
{
    const auto key = ntw::io::unique_reg_key::open(
        L"\\Registry\\Machine\\System\\CurrentControlSet\\Services",
        ntw::io::reg_access{}.enum_sub_keys().query_value());
    REQUIRE(key);
    REQUIRE(key->get() != nullptr);
}

TEST_CASE("reg_key open with options")
{
    const auto key = ntw::io::unique_reg_key::open(
        L"\\Registry\\Machine\\System\\CurrentControlSet\\Services",
        ntw::io::reg_access{}.enum_sub_keys().query_value(),
        ntw::io::reg_open_options{}.dont_virtualize());
    REQUIRE(key);
    REQUIRE(key->get() != nullptr);
}

TEST_CASE("reg_key create")
{
    const auto key = ntw::io::unique_reg_key::create(
        L"\\Registry\\Machine\\System\\CurrentControlSet\\Services",
        ntw::io::reg_access{}.enum_sub_keys().query_value());
    REQUIRE(key);
    REQUIRE(key->get() != nullptr);
}

TEST_CASE("reg_key create with disposition")
{
    bool       opened;
    const auto key = ntw::io::unique_reg_key::create(
        L"\\Registry\\Machine\\System\\CurrentControlSet\\Services",
        ntw::io::reg_access{}.enum_sub_keys().query_value(),
        ntw::io::reg_create_options{}.dont_virtualize(),
        {},
        opened);
    REQUIRE(key);
    REQUIRE(key->get() != nullptr);
}