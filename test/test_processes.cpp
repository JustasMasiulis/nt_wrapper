#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <system/processes.hpp>

TEST_CASE("processes enumeration once")
{
    ntw::system::processes processes;
    REQUIRE(NT_SUCCESS(processes.update()));

    for(auto& process : processes) {
        process.num_threads();
        process.name();
        process.id();
        process.parent_id();

        process.private_working_set();
        process.working_set();

        process.virtual_size();

        process.handle_count();

        process.get();
        for(auto& thread : process.threads()) {
            thread.start_address();
            thread.id();
            thread.process_id();
            thread.state();
        }
    }
}

TEST_CASE("processes enumeration multiple")
{
    ntw::system::processes processes;

    for(int i = 0; i < 10; ++i) {
        REQUIRE(NT_SUCCESS(processes.update()));
        for(auto& process : processes)
            ;
    }
}