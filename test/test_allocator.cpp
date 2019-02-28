#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <allocator.hpp>

#pragma comment(lib, "ntdll.lib")

bool probe_for_read(void* ptr, std::size_t size)
{
    for(std::size_t i = 0; i < size; ++i) {
        volatile char c = *((volatile char*)ptr + size);
    }
    return true;
}

TEST_CASE("stack allocator")
{
    ntw::stack_alloc<256> alloc;
    REQUIRE(alloc.max_size() == 256);

    int* ptr;
    auto status = alloc.allocate(ptr, 256);
    REQUIRE(status.success());
    REQUIRE(probe_for_read(ptr, 256));

    alloc.deallocate(ptr);
}

TEST_CASE("heap allocator")
{
    ntw::heap_alloc alloc;
    REQUIRE(alloc.max_size() == std::numeric_limits<std::size_t>::max());

    unsigned* ptr;
    auto  status = alloc.allocate(ptr, 256);
    REQUIRE(status.success());
    REQUIRE(probe_for_read(ptr, 256));
    alloc.deallocate(ptr);
}

TEST_CASE("page allocator")
{
    ntw::page_alloc alloc;
    REQUIRE(alloc.max_size() == std::numeric_limits<std::size_t>::max());

    char* ptr;
    auto  status = alloc.allocate(ptr, 256);
    REQUIRE(status.success());
    REQUIRE(probe_for_read(ptr, 256));
    alloc.deallocate(ptr);
}