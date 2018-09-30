#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <io/async_file.hpp>

TEST_CASE("file attribute options")
{
    constexpr auto opt = ntw::io::file_options{}
                             .archive()
                             .encrypted()
                             .hidden()
                             .normal()
                             .offline()
                             .readonly()
                             .system()
                             .temporary()
                             .reset_attributes();
}

TEST_CASE("common file options")
{
    constexpr auto share_options = ntw::io::file_options{}
                                       .share_all()
                                       .share_read()
                                       .share_write()
                                       .share_delete()
                                       .reset_share_access();

    constexpr auto create_options = ntw::io::file_options{}
                                        .directory()
                                        .non_directory()
                                        .write_trough()
                                        .sequential_access()
                                        .random_access()
                                        .sync_io_alert()
                                        .sync_io_nonalert()
                                        .create_tree_connection()
                                        .no_ea_knownledge()
                                        .open_reparse_point()
                                        .delete_on_close()
                                        .open_by_file_id()
                                        .open_for_backup()
                                        .reserve_opfilter()
                                        .requires_oplock()
                                        .complete_if_oplocked()
                                        .reset_create_options();

    constexpr auto access_options = ntw::io::file_options{}
                                        .deleteable()
                                        .synchronizable()
                                        .executeable()
                                        .traversible()
                                        .listable_directory()
                                        .generic_readable()
                                        .generic_writeable()
                                        .generic_executeable()
                                        .readable_data()
                                        .readable_attributes()
                                        .readable_extended_attributes()
                                        .readable_access_control()
                                        .writeable_data()
                                        .writeable_extended_attributes()
                                        .writeable_access_control()
                                        .writeable_ownership()
                                        .appendable_data()
                                        .full_access()
                                        .reset_access();
}

TEST_CASE("pipe options")
{
    constexpr auto opt = ntw::io::pipe_options{}
                             .qouta(1, 1)
                             .inbound_qouta(2)
                             .outbound_qouta(2)
                             .async()
                             .sync()
                             .byte_stream()
                             .message_stream()
                             .accept_remote_clients()
                             .reject_remote_clients()
                             .reset_type()
                             .instances_limit(-1)
                             .timeout(-5000);
}

TEST_CASE("file")
{

    ntw::io::unique_async_file f;
    f.open(std::wstring_view{ L"\\??\\C:\\Users\\justa\\OneDrive\\Desktop\\test.txt" });

	ntw::io::async_query query([](auto& q) { __debugbreak();
		});
    char buf[]{ 'a', 'b', 'c', 'd','e','f','g','h','i','j','k','l','z','x' };
    if(auto status = f.write(buf, sizeof(buf), 0, query); status == STATUS_PENDING) {
        auto timeout = ntw::make_large_int(-10'000'000);
        status = LI_NT(NtWaitForSingleObject)(f.handle().get(), TRUE, &timeout);
        __debugbreak();
	}
}
