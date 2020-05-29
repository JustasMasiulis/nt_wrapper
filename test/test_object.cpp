#include <ntw/ob/object.hpp>
#include <ntw/ob/object_info.hpp>
#define CATCH_CONFIG_MAIN
#define WIN32_NO_STATUS
#include <catch2/catch.hpp>

#pragma comment(lib, "ntdll.lib")

TEST_CASE("object constructors and basic funcs")
{
    SECTION("default constructed")
    {
        ntw::ob::object obj;
        REQUIRE(obj.get() == nullptr);
    }

    SECTION("handle constructed")
    {
        ntw::ob::object obj{ NtCurrentProcess() };
        REQUIRE(obj.get() == NtCurrentProcess());
    }

    SECTION("move constructed")
    {
        ntw::ob::object obj{ NtCurrentProcess() };
        ntw::ob::object obj2{ std::move(obj) };

        REQUIRE(obj2.get() == NtCurrentProcess());
        REQUIRE(obj.get() == nullptr);
    }

    SECTION("move assigned")
    {
        ntw::ob::object obj{ NtCurrentProcess() };
        ntw::ob::object obj2{ nullptr };
        obj2 = std::move(obj);

        REQUIRE(obj2.get() == NtCurrentProcess());
        REQUIRE(obj.get() == nullptr);
    }
}

TEST_CASE("reset")
{
    SECTION("non null")
    {
        ntw::ob::object obj{ NtCurrentProcess() };
        obj.reset(nullptr);
        REQUIRE(obj.get() == nullptr);
    }

    SECTION("null")
    {
        ntw::ob::object obj;
        obj.reset(NtCurrentProcess());
        REQUIRE(obj.get() == NtCurrentProcess());
    }
}

TEST_CASE("release")
{
    ntw::ob::object obj{ NtCurrentProcess() };
    REQUIRE(obj.release() == NtCurrentProcess());
    REQUIRE(obj.get() == nullptr);
}

TEST_CASE("operator bool")
{
    SECTION("non null")
    {
        ntw::ob::object obj{ NtCurrentProcess() };
        REQUIRE(!!obj);
    }

    SECTION("null")
    {
        ntw::ob::object obj;
        REQUIRE(!obj);
    }
}

TEST_CASE("conversion betweeen types of objects")
{
    SECTION("assignment")
    {
        ntw::ob::object unique;
        ntw::ob::object_ref    ref;

        ref = unique;
    }

    SECTION("constructor")
    {
        ntw::ob::object_ref    ref;
        ntw::ob::object unique(ref);
        ntw::ob::object_ref    ref2(unique);
    }
}