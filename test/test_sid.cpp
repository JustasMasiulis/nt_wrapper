#include <ntw/se/sid.hpp>
#define CATCH_CONFIG_MAIN
#define WIN32_NO_STATUS
#include <catch2/catch.hpp>

TEST_CASE("static_sid: default ctor")
{
    constexpr ntw::se::static_sid<1> s;
    REQUIRE(s.size() == 0);
    REQUIRE(s.max_size() == 1);
    REQUIRE(s.size() == 0);
    std::uint64_t zero = 0;
    REQUIRE(std::memcmp(&s.identifier_authority(), &zero, 8) == 0);
    REQUIRE(s.sub_authorities().size() == 0);
}

TEST_CASE("static_sid: authority ctor")
{
    constexpr ntw::se::static_sid<0> s(ntw::se::sid::authority::non_unique);
    REQUIRE(s.size() == 0);
    REQUIRE(s.max_size() == 0);
    REQUIRE(s.size() == 0);
    REQUIRE(std::memcmp(
                &s.identifier_authority(), &ntw::se::sid::authority::non_unique, 8) == 0);
}

TEST_CASE("static_sid: ") {}