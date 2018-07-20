#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <system/loaded_drivers.hpp>

TEST_CASE("loaded drivers enumeration once")
{
    ntw::system::loaded_drivers drivers;
    REQUIRE(NT_SUCCESS(drivers.update()));

    for(auto& driver : drivers) {
        driver.section();
        driver.mapped_base();
        driver.image_base();
        driver.default_base();
        driver.image_size();
        driver.flags();
        driver.name();
        driver.path();
        driver.checksum();
        driver.time_stamp();
        driver.load_order_index();
        driver.init_order_index();
        driver.load_count();
    }
}

TEST_CASE("loaded drivers enumeration multiple")
{
    ntw::system::loaded_drivers drivers;

    for(int i = 0; i < 10; ++i) {
        REQUIRE(NT_SUCCESS(drivers.update()));

        for(auto& driver : drivers)
            ;
    }
}