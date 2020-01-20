#include <ntw/io/registry_key.hpp>
#define CATCH_CONFIG_MAIN
#define WIN32_NO_STATUS
#include <catch2/catch.hpp>

#pragma comment(lib, "ntdll.lib")

TEST_CASE("reg_key create") { ntw::io::unique_reg_key key; }