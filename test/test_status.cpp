#include <ntw/status.hpp>
#define CATCH_CONFIG_MAIN
#define WIN32_NO_STATUS
#include <catch2/catch.hpp>

TEST_CASE("default constructed status") { REQUIRE(ntw::status{}.get() == 0); }

TEST_CASE("assignment works correctly")
{
    ntw::status s;
    s = STATUS_INVALID_PARAMETER;
    REQUIRE(s.get() == STATUS_INVALID_PARAMETER);
}

TEST_CASE("comparison operator works correctly")
{
    ntw::status s;
    s = STATUS_INVALID_PARAMETER;
    REQUIRE(s == STATUS_INVALID_PARAMETER);
}

TEST_CASE("conversion operator works correctly")
{
    ntw::status s;
    s = STATUS_INVALID_PARAMETER;
    std::int32_t i{ s };
    REQUIRE(i == STATUS_INVALID_PARAMETER);
    REQUIRE(static_cast<std::int32_t>(s) == STATUS_INVALID_PARAMETER);
}

TEST_CASE("returns correct severity")
{
    SECTION("success")
    {
        std::int32_t codes[]{ STATUS_SUCCESS,
                              STATUS_WAIT_1,
                              STATUS_PENDING,
                              STATUS_MORE_ENTRIES,
                              STATUS_BUFFER_ALL_ZEROS };
        for(auto c : codes) {
            auto s = ntw::status{ c };
            REQUIRE(!s);
            REQUIRE(s.severity() == ntw::severity::success);
            REQUIRE(s.success());
            REQUIRE_FALSE(s.warning());
            REQUIRE_FALSE(s.error());
        }
    }

    SECTION("information")
    {
        std::int32_t codes[]{ STATUS_OBJECT_NAME_EXISTS,
                              STATUS_THREAD_WAS_SUSPENDED,
                              STATUS_IMAGE_NOT_AT_BASE,
                              STATUS_BAD_CURRENT_DIRECTORY };
        for(auto c : codes) {
            auto s = ntw::status{ c };
            REQUIRE(!s);
            REQUIRE(s.severity() == ntw::severity::information);
            REQUIRE(s.success());
            REQUIRE_FALSE(s.warning());
            REQUIRE_FALSE(s.error());
        }
    }

    SECTION("warning")
    {
        std::int32_t codes[]{ STATUS_SINGLE_STEP,
                              STATUS_BREAKPOINT,
                              STATUS_BUFFER_OVERFLOW,
                              STATUS_NO_MORE_FILES };
        for(auto c : codes) {
            auto s = ntw::status{ c };
            REQUIRE(!!s);
            REQUIRE(s.severity() == ntw::severity::warning);
            REQUIRE_FALSE(s.success());
            REQUIRE(s.warning());
            REQUIRE_FALSE(s.error());
        }
    }


    SECTION("error")
    {
        std::int32_t codes[]{ STATUS_INFO_LENGTH_MISMATCH,
                              STATUS_UNSUCCESSFUL,
                              STATUS_NOT_IMPLEMENTED,
                              STATUS_INVALID_INFO_CLASS };
        for(auto c : codes) {
            auto s = ntw::status{ c };
            REQUIRE(!!s);
            REQUIRE(s.severity() == ntw::severity::error);
            REQUIRE_FALSE(s.success());
            REQUIRE_FALSE(s.warning());
            REQUIRE(s.error());
        }
    }
}

TEST_CASE("returns correct facility")
{
    std::int32_t codes[]{ STATUS_UNSUCCESSFUL,
                          STATUS_NONEXISTENT_SECTOR,
                          STATUS_THREAD_NOT_IN_PROCESS,
                          STATUS_KDC_UNABLE_TO_REFER,
                          STATUS_CSS_AUTHENTICATION_FAILURE,
                          RPC_NT_INVALID_ENDPOINT_FORMAT,
                          STATUS_ACPI_INVALID_OPCODE,
                          STATUS_FLT_MUST_BE_NONPAGED_POOL };

    for(auto c : codes)
        REQUIRE(static_cast<std::int32_t>(ntw::status{ c }.facility()) == NT_FACILITY(c));
}

TEST_CASE("returns correct code")
{
    std::int32_t codes[]{ STATUS_UNSUCCESSFUL,
                          STATUS_NONEXISTENT_SECTOR,
                          STATUS_THREAD_NOT_IN_PROCESS,
                          STATUS_KDC_UNABLE_TO_REFER,
                          STATUS_CSS_AUTHENTICATION_FAILURE,
                          RPC_NT_INVALID_ENDPOINT_FORMAT,
                          STATUS_ACPI_INVALID_OPCODE,
                          STATUS_FLT_MUST_BE_NONPAGED_POOL };

    for(auto c : codes)
        REQUIRE(static_cast<std::int32_t>(ntw::status{ c }.code()) ==
                WIN32_FROM_NTSTATUS(c));
}

TEST_CASE("and_then")
{
    SECTION("with unsuccess")
    {
        ntw::status s{ STATUS_UNSUCCESSFUL };

        s.and_then([](ntw::status s) {
            REQUIRE(false);
            return STATUS_SUCCESS;
        });
        REQUIRE(s == STATUS_UNSUCCESSFUL);

        s.and_then([](ntw::status s) { REQUIRE(false); });
        REQUIRE(s == STATUS_UNSUCCESSFUL);
    }

    SECTION("with success")
    {
        ntw::status s{ STATUS_SUCCESS };
        s.and_then([](ntw::status s) {
            REQUIRE(s == STATUS_SUCCESS);
            return STATUS_UNSUCCESSFUL;
        });
        REQUIRE(s == STATUS_UNSUCCESSFUL);
        s = STATUS_SUCCESS;

        s.and_then([](ntw::status s) { REQUIRE(s == STATUS_SUCCESS); });
        REQUIRE(s == STATUS_SUCCESS);
    }
}

TEST_CASE("or_else")
{
    SECTION("with unsuccess")
    {
        ntw::status s{ STATUS_UNSUCCESSFUL };

        s.or_else([](ntw::status s) {
            REQUIRE(s == STATUS_UNSUCCESSFUL);
            return STATUS_SUCCESS;
        });
        REQUIRE(s == STATUS_SUCCESS);

        s = STATUS_UNSUCCESSFUL;
        s.or_else([](ntw::status s) { REQUIRE(s == STATUS_UNSUCCESSFUL); });
        REQUIRE(s == STATUS_UNSUCCESSFUL);
    }

    SECTION("with success")
    {
        ntw::status s{ STATUS_SUCCESS };
        s.or_else([](ntw::status s) {
            REQUIRE(false);
            return STATUS_UNSUCCESSFUL;
        });
        REQUIRE(s == STATUS_SUCCESS);

        s.or_else([](ntw::status s) { REQUIRE(false); });
        REQUIRE(s == STATUS_SUCCESS);
    }
}