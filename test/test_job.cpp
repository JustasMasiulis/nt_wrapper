#include <ntw/ob/job.hpp>
#include <ntw/ob/job_info.hpp>
#define CATCH_CONFIG_MAIN
#define WIN32_NO_STATUS
#include <catch2/catch.hpp>

#pragma comment(lib, "ntdll.lib")

TEST_CASE("job")
{
    using namespace ntw::ob;

    auto res = job::create(job_access{}.all());
    CHECK(res);

    SECTION("assign") { CHECK(res->assign_curr_process().success()); }

    SECTION("query")
    {
        auto info = res->query<ntw::job::accounting_info>();
        CHECK(info);
    }
}

TEST_CASE("job access")
{
    using namespace ntw::ob;

    CHECK(job_access{}.all().get() == JOB_OBJECT_ALL_ACCESS);
    CHECK((job_access{}
               .assign_process()
               .set_attributes()
               .query()
               .terminate()
               .set_security_attr()
               .impersonate()
               .synchronize()
               .get() |
           STANDARD_RIGHTS_REQUIRED) == JOB_OBJECT_ALL_ACCESS);
}
