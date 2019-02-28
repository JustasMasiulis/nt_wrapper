#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <resource.hpp>

#pragma comment(lib, "ntdll.lib")

bool probe_for_read(void* ptr, std::size_t size)
{
    for(std::size_t i = 0; i < size; ++i) {
        volatile char c = *((volatile char*)ptr + size);
    }
    return true;
}

TEST_CASE("stack buffer")
{
    ntw::stack_buffer<256> buff;
    REQUIRE(probe_for_read(buff.data(), 256));
    REQUIRE(buff.size() == 256);
}

TEST_CASE("heap allocator")
{
    ntw::heap_alloc alloc;
    unsigned* ptr;
    auto      status = alloc.allocate(ptr, 256);
    REQUIRE(status.success());
    REQUIRE(probe_for_read(ptr, 256));
    alloc.deallocate(ptr);
}

TEST_CASE("page allocator")
{
    ntw::page_alloc alloc;
    char* ptr;
    auto  status = alloc.allocate(ptr, 256);
    REQUIRE(status.success());
    REQUIRE(probe_for_read(ptr, 256));
    alloc.deallocate(ptr);
}