#pragma once
#include "../detail/offset_iterator.hpp"
#include "../result.hpp"
#include "../unicode_string.hpp"
#include "../chrono.hpp"

namespace ntw::sys {

    /// \brief A wrapper around SYSTEM_THREAD_INFORMATION class
    struct thread {
        duration       kernel_time;
        duration       user_time;
        std::uint64_t  create_time;
        std::uint32_t  wait_time;
        void*          start_address;
        std::uintptr_t process_id; // ClientId.UniqueProcess
        std::uintptr_t id; // ClientId.UniqueThread
        std::int32_t   priority;
        std::int32_t   base_priority;
        std::uint32_t  context_switches;
        KTHREAD_STATE  state;
        KWAIT_REASON   wait_reason;
    };

    /// \brief A wrapper around SYSTEM_PROCESS_INFORMATION class
    struct process {
        std::uint32_t  offset_to_next;
        std::uint32_t  thread_count; // NumberOfThreads
        std::uint64_t  working_set_private_size;
        std::uint32_t  hard_fault_count;
        std::uint32_t  thread_count_high_watermark; // NumberOfThreadsHighWatermark
        std::uint64_t  cycle_time;
        std::uint64_t  create_time;
        duration       user_time;
        duration       kernel_time;
        unicode_string image_name;
        std::int32_t   base_priority;
        std::uintptr_t id; // UniqueProcessId
        std::uintptr_t parent_id; // InheritedFromUniqueProcessId
        std::uint32_t  handle_count;
        std::uint32_t  session_id;
        std::uintptr_t key; // UniqueProcessKey
        std::size_t    peak_virtual_size;
        std::size_t    virtual_size;
        std::uint32_t  page_fault_count;
        std::size_t    peak_working_set_size;
        std::size_t    working_set_size;

        struct {
            std::size_t peak_paged; // QuotaPeakPagedPoolUsage
            std::size_t paged; // QuotaPagedPoolUsage
            std::size_t peak_non_paged; // QuotaPeakNonPagedPoolUsage
            std::size_t non_paged; // QuotaNonPagedPoolUsage
        } qouta_pool_usage;

        std::size_t pagefile_usage;
        std::size_t peak_pagefile_usage;
        std::size_t private_page_count;

        struct {
            std::uint64_t read; // ReadOperationCount;
            std::uint64_t write; // WriteOperationCount;
            std::uint64_t other; // OtherOperationCount;
        } operation_count;

        struct {
            std::uint64_t read; // ReadTransferCount
            std::uint64_t write; // WriteTransferCount
            std::uint64_t other; // OtherTransferCount
        } transfer_count;

        /// \brief Returns a span of this process thread information
        NTW_INLINE gsl::span<thread> threads() noexcept;

        /// \brief Returns a span of this process thread information
        NTW_INLINE gsl::span<const thread> threads() const noexcept;

        using range_type = detail::offset_iterator_range<process, true>;
    };

    /// \brief Acquires a list of processes using NtQuerySystemInformation with
    ///        SystemProcessInformation class.
    /// \param buffer Buffer into which process information will be read into.
    /// \param returned The amount of bytes used inside the buffer.
    template<class Range>
    NTW_INLINE ntw::result<process::range_type> processes(Range&&  buffer,
                                                          ulong_t* returned = nullptr);

} // namespace ntw::sys

#include "../../../impl/sys/processes.inl"
