#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <obj/attributes.hpp>

TEST_CASE("OBJECT_ATTRIBUTES/attributes building works")
{
    void* mock_pointer = (void*)0xDEADBEEF;
    auto  attr         = ntw::obj::attributes{}
                    .parent(mock_pointer)
                    .security_desc(mock_pointer)
                    .security_quality(mock_pointer)
                    .inherit()
                    .permanent()
                    .exclusive()
                    .case_insensitive()
                    .open()
                    .open_symlink()
                    .kernel_handle()
                    .enforce_access_check()
                    .ignore_impersonated_devicemap()
                    .dont_reparse();
    auto built = attr.get();

    SECTION(" invariants maintained")
    {
        REQUIRE(built.ObjectName == nullptr);
        REQUIRE(built.Length == sizeof(OBJECT_ATTRIBUTES));
    }

    SECTION("pointers set accordingly")
    {
        REQUIRE(built.RootDirectory == mock_pointer);
        REQUIRE(built.SecurityDescriptor == mock_pointer);
        REQUIRE(built.SecurityQualityOfService == mock_pointer);
    }

    SECTION("all flags set correctly")
    {
        REQUIRE(built.Attributes == OBJ_VALID_ATTRIBUTES);
    }
}
