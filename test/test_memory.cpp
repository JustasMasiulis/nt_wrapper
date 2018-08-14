#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <memory.hpp>

#define REQUIRE_SUCCESS(x) REQUIRE(NT_SUCCESS(x))

TEST_CASE("memory::allocate and memory::free")
{
    ntw::memory(nullptr);
    SECTION("void* address")
    {
        void* address = nullptr;
        REQUIRE_SUCCESS(ntw::memory().allocate(address, 0x1000, PAGE_READWRITE));
        REQUIRE_SUCCESS(ntw::memory().free(address));
    }

    SECTION("std::uintptr_t address")
    {
        std::uintptr_t address = 0;
        REQUIRE_SUCCESS(ntw::memory().allocate(address, 0x1000, PAGE_READWRITE));
        REQUIRE_SUCCESS(ntw::memory().free(address));
    }

    SECTION("std::uint32_t* address")
    {
        std::uint32_t* address = nullptr;
        REQUIRE_SUCCESS(ntw::memory().allocate(address, 0x1000, PAGE_READWRITE));
        REQUIRE_SUCCESS(ntw::memory().free(address));
    }
}

TEST_CASE("memory::unique_alloc")
{
    SECTION("acccess functions")
    {
        ntw::memory::unique_alloc<void> alloc;
        REQUIRE(alloc.get() == nullptr);
        REQUIRE(!alloc);
        REQUIRE(alloc.addressof());

        auto new_val = reinterpret_cast<void*>(1);
        alloc.reset(new_val);
        REQUIRE(alloc);
        REQUIRE(alloc.get() == new_val);
        REQUIRE(alloc.release() == new_val);
        REQUIRE(alloc.get() == nullptr);
    }

    SECTION("alloc(size, prot)")
    {
        ntw::memory::unique_alloc<void*> alloc;
        REQUIRE_SUCCESS(alloc.allocate(0x1000, PAGE_READWRITE));
    }

    SECTION("alloc(addr, size, prot)")
    {
        void* addr;
        {
            ntw::memory::unique_alloc<void*> alloc;
            REQUIRE_SUCCESS(alloc.allocate(0x1000, PAGE_READWRITE));
            addr = alloc.get();
        }
        ntw::memory::unique_alloc<void*> alloc;
        REQUIRE_SUCCESS(alloc.allocate(addr, 0x1000, PAGE_READWRITE));
    }
}

TEST_CASE("memory::unique_lock")
{
    ntw::memory::unique_alloc<void> alloc;
    REQUIRE_SUCCESS(alloc.allocate(0x1000, PAGE_READWRITE));

    {
        ntw::memory::unique_lock lock;
        REQUIRE_SUCCESS(lock.lock(alloc.get(), 0x1000));
    }

    {
        ntw::memory::unique_lock lock;
        REQUIRE_SUCCESS(lock.lock_strong(alloc.get(), 0x1000));
    }
}

TEST_CASE("memory::protect")
{
    ntw::memory::unique_alloc<void> alloc;
    REQUIRE_SUCCESS(alloc.allocate(0x1000, PAGE_READWRITE));
    REQUIRE_SUCCESS(ntw::memory().protect(alloc.get(), 0x1000, PAGE_EXECUTE_READWRITE));

    MEMORY_BASIC_INFORMATION info;
    REQUIRE_SUCCESS(ntw::memory().info(alloc.get(), MemoryBasicInformation, info));
    REQUIRE(info.Protect == PAGE_EXECUTE_READWRITE);
}

TEST_CASE("memory::enumerate_regions")
{
    REQUIRE_SUCCESS(
        ntw::memory().enumerate_regions(0ULL, 0xFFFF'FFFF'FFFF'FFFF, [](auto& info) {
            info.base();
            info.size();
            info.end();
            info.protection();
            !!info;
            info.is_commited();
            info.is_reserved();
            info.state();
            info.type();
            info.is_mapped();
            info.is_private();
            info.is_image();
            info.allocation_base();
            info.allocation_protection();
            info.get();
        }));
}

TEST_CASE("memory example")
{
    ntw::memory::unique_alloc<void> alloc;
    REQUIRE_SUCCESS(alloc.allocate(0x1000, PAGE_READWRITE));

    ntw::memory::unique_lock lock;
    REQUIRE_SUCCESS(lock.lock(alloc.get(), 0x1000));

    // the default constructed memory object refers to our own process
    REQUIRE_SUCCESS(ntw::memory().protect(alloc.get(), 0x1000, PAGE_EXECUTE_READWRITE));

    ntw::memory::region_info region_info;
    REQUIRE_SUCCESS(
        ntw::memory().info(alloc.get(), MemoryBasicInformation, region_info.get()));

    REQUIRE(region_info.protection().native() == PAGE_EXECUTE_READWRITE);

    auto status = ntw::memory().enumerate_regions(
        static_cast<void*>(0), alloc.get(), [](ntw::memory::region_info& info) {
            // not free
            if(info) {
                // ...
            }
        });

    REQUIRE_SUCCESS(status);
}