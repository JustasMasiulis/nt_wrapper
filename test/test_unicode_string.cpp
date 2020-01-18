#include <ntw/unicode_string.hpp>
#define CATCH_CONFIG_MAIN
#define WIN32_NO_STATUS
#include <catch2/catch.hpp>

const wchar_t* str = L"abc";

TEST_CASE("default constructed")
{
    SECTION("non const")
    {
        ntw::unicode_string ustr;
        REQUIRE(ustr.size() == 0);
        REQUIRE(ustr.begin() == nullptr);
        REQUIRE(ustr.empty());
        REQUIRE(ustr.view() == std::wstring_view{});
    }
    SECTION("const")
    {
        const ntw::unicode_string ustr;
        REQUIRE(ustr.size() == 0);
        REQUIRE(ustr.begin() == nullptr);
        REQUIRE(ustr.empty());
        REQUIRE(ustr.view() == std::wstring_view{});
    }
}

TEST_CASE("UNICODE_STRING constructed")
{
    UNICODE_STRING _ustr;
    _ustr.Buffer = const_cast<wchar_t*>(str);
    _ustr.Length = _ustr.MaximumLength = 6;

    SECTION("non const")
    {
        ntw::unicode_string ustr(_ustr);
        REQUIRE_FALSE(ustr.empty());
        REQUIRE(ustr.size() == 3);
        REQUIRE(ustr.begin() == str);
        REQUIRE(ustr.end() == ustr.begin() + ustr.size());
        REQUIRE(ustr.view() == std::wstring_view(str, 3));
    }

    SECTION("const")
    {
        const ntw::unicode_string ustr(_ustr);
        REQUIRE_FALSE(ustr.empty());
        REQUIRE(ustr.size() == 3);
        REQUIRE(ustr.begin() == str);
        REQUIRE(ustr.end() == ustr.begin() + ustr.size());
        REQUIRE(ustr.view() == std::wstring_view(str, 3));
    }
}

TEST_CASE("view constructed")
{
    std::wstring_view view{ str, 3 };

    SECTION("non const")
    {
        ntw::unicode_string ustr(view);
        REQUIRE_FALSE(ustr.empty());
        REQUIRE(ustr.size() == 3);
        REQUIRE(ustr.begin() == str);
        REQUIRE(ustr.end() == ustr.begin() + ustr.size());
        REQUIRE(ustr.view() == view);
    }

    SECTION("const")
    {
        const ntw::unicode_string ustr(view);
        REQUIRE_FALSE(ustr.empty());
        REQUIRE(ustr.size() == 3);
        REQUIRE(ustr.begin() == str);
        REQUIRE(ustr.end() == ustr.begin() + ustr.size());
        REQUIRE(ustr.view() == view);
    }
}

TEST_CASE("ptr constructed")
{
    SECTION("non const")
    {
        ntw::unicode_string ustr(str, 3);
        REQUIRE_FALSE(ustr.empty());
        REQUIRE(ustr.size() == 3);
        REQUIRE(ustr.begin() == str);
        REQUIRE(ustr.end() == ustr.begin() + ustr.size());
        REQUIRE(ustr.view() == std::wstring_view(str, 3));
    }
    SECTION("const")
    {
        const ntw::unicode_string ustr(str, 3);
        REQUIRE_FALSE(ustr.empty());
        REQUIRE(ustr.size() == 3);
        REQUIRE(ustr.begin() == str);
        REQUIRE(ustr.end() == ustr.begin() + ustr.size());
        REQUIRE(ustr.view() == std::wstring_view(str, 3));
    }
}

TEST_CASE("get member")
{
    ntw::unicode_string ustr;
    ustr.get();
    ntw::unicode_string custr;
    custr.get();
}