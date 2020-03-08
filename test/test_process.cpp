#include <ntw/ob/process.hpp>
#define CATCH_CONFIG_MAIN
#define WIN32_NO_STATUS
#include <catch2/catch.hpp>

#pragma comment(lib, "ntdll.lib")

TEST_CASE("process access building")
{
    auto access = ntw::ob::process_access{}
                      .terminate()
                      .create_thread()
                      .set_session_id()
                      .vm_operation()
                      .vm_read()
                      .vm_write()
                      .dup_handle()
                      .create_process()
                      .set_qouta()
                      .set_info()
                      .query_info()
                      .suspend_resume()
                      .query_limited_info()
                      .set_limited_info()
                      .get();

    REQUIRE(access == (PROCESS_TERMINATE | PROCESS_CREATE_THREAD | PROCESS_SET_SESSIONID |
                       PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE |
                       PROCESS_DUP_HANDLE | PROCESS_CREATE_PROCESS | PROCESS_SET_QUOTA |
                       PROCESS_SET_INFORMATION | PROCESS_QUERY_INFORMATION |
                       PROCESS_SUSPEND_RESUME | PROCESS_QUERY_LIMITED_INFORMATION |
                       PROCESS_SET_LIMITED_INFORMATION));
}

TEST_CASE("process default constructors")
{
    REQUIRE(NtCurrentProcess() == ntw::ob::process_ref{}.get());
}

TEST_CASE("memory read")
{
    SECTION("buffer and size")
    {
        std::size_t variable{ 6 };
        std::size_t copy{ 0 };
        auto status = ntw::ob::process_ref{}.read_mem(&variable, &copy, sizeof(copy));
        REQUIRE(copy == variable);
        REQUIRE(status.success());
    }

    SECTION("range")
    {
        std::size_t variable{ 6 };
        std::size_t copy{ 0 };
        auto        status = ntw::ob::process_ref{}.read_mem(
            &variable, gsl::as_writeable_bytes(gsl::span{ &copy, 1 }));

        REQUIRE(copy == variable);
        REQUIRE(status.success());
    }
}

TEST_CASE("memory write")
{
    SECTION("buffer and size")
    {
        std::size_t variable{ 6 };
        std::size_t copy{ 0 };
        auto        status =
            ntw::ob::process_ref{}.write_mem(&copy, &variable, sizeof(variable));
        REQUIRE(copy == variable);
        REQUIRE(status.success());
    }

    SECTION("range")
    {
        std::size_t variable{ 6 };
        std::size_t copy{ 0 };
        auto        status = ntw::ob::process_ref{}.write_mem(
            &copy, gsl::as_bytes(gsl::span{ &variable, 1 }));

        REQUIRE(copy == variable);
        REQUIRE(status.success());
    }
}