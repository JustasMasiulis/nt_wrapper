#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <ob/process.hpp>

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
