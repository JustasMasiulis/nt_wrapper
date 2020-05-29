#pragma once
#include <optional>
#include "../chrono.hpp"

namespace ntw::job {

    struct accounting_info : detail::query_info {
        duration      user_time; // TotalUserTime
        duration      kernel_time; // TotalKernelTime
        duration      this_period_user_time; // ThisPeriodTotalUserTime
        duration      this_period_kernel_time; // ThisPeriodTotalKernelTime
        std::uint32_t page_faults; // TotalPageFaultCount
        std::uint32_t processes; // TotalProcesses
        std::uint32_t active_processes; // ActiveProcesses
        std::uint32_t terminated_processes; // TotalTerminatedProcesses

        NTW_INLINE JOBOBJECT_BASIC_ACCOUNTING_INFORMATION& get() noexcept
        {
            return *reinterpret_cast<JOBOBJECT_BASIC_ACCOUNTING_INFORMATION*>(this);
        }

        constexpr static JOBOBJECTINFOCLASS info_class =
            static_cast<JOBOBJECTINFOCLASS>(JobObjectBasicAccountingInformation);
    };

    static_assert(sizeof(accounting_info) ==
                  sizeof(JOBOBJECT_BASIC_ACCOUNTING_INFORMATION));

    class limits : public detail::query_info, detail::set_info {
        JOBOBJECT_BASIC_LIMIT_INFORMATION _info;

    public:
        NTW_INLINE limits() noexcept { _info.LimitFlags = 0; }

        constexpr static JOBOBJECTINFOCLASS info_class =
            static_cast<JOBOBJECTINFOCLASS>(JobObjectBasicLimitInformation);

        NTW_INLINE JOBOBJECT_BASIC_LIMIT_INFORMATION& get() noexcept { return _info; }

        NTW_INLINE std::optional<duration> process_max_user_time() const noexcept
        {
            if(_info.LimitFlags & JOB_OBJECT_LIMIT_PROCESS_TIME)
                return ntw::duration{ _info.PerProcessUserTimeLimit.QuadPart };
            return std::nullopt;
        }

        NTW_INLINE std::optional<duration> max_user_time() const noexcept
        {
            if(_info.LimitFlags & JOB_OBJECT_LIMIT_JOB_TIME)
                return ntw::duration{ _info.PerJobUserTimeLimit.QuadPart };
            return std::nullopt;
        }

        NTW_INLINE std::optional<std::size_t> min_working_set() const noexcept
        {
            if(_info.LimitFlags & JOB_OBJECT_LIMIT_WORKINGSET)
                return _info.MinimumWorkingSetSize;
            return std::nullopt;
        }

        NTW_INLINE std::optional<std::size_t> max_working_set() const noexcept
        {
            if(_info.LimitFlags & JOB_OBJECT_LIMIT_WORKINGSET)
                return _info.MaximumWorkingSetSize;
            return std::nullopt;
        }

        NTW_INLINE std::optional<std::uint32_t> max_processes() const noexcept
        {
            if(_info.LimitFlags & JOB_OBJECT_LIMIT_ACTIVE_PROCESS)
                return _info.ActiveProcessLimit;
            return std::nullopt;
        }

        NTW_INLINE std::optional<std::uintptr_t> affinity() const noexcept
        {
            if(_info.LimitFlags & JOB_OBJECT_LIMIT_AFFINITY)
                return _info.Affinity;
            return std::nullopt;
        }

        NTW_INLINE std::optional<std::uint32_t> priority() const noexcept
        {
            if(_info.LimitFlags & JOB_OBJECT_LIMIT_PRIORITY_CLASS)
                return _info.PriorityClass;
            return std::nullopt;
        }

        NTW_INLINE std::optional<std::uint32_t> scheduling() const noexcept
        {
            if(_info.LimitFlags & JOB_OBJECT_LIMIT_SCHEDULING_CLASS)
                return _info.SchedulingClass;
            return std::nullopt;
        }

        NTW_INLINE limits& process_max_user_time(duration value) noexcept
        {
            _info.LimitFlags |= JOB_OBJECT_LIMIT_PROCESS_TIME;
            _info.PerProcessUserTimeLimit.QuadPart = value.count();
            return *this;
        }

        NTW_INLINE limits& max_user_time(duration value) noexcept
        {
            _info.LimitFlags |= JOB_OBJECT_LIMIT_JOB_TIME;
            _info.PerJobUserTimeLimit.QuadPart = value.count();
            return *this;
        }

        NTW_INLINE limits& min_working_set(std::size_t value) noexcept
        {
            _info.LimitFlags |= JOB_OBJECT_LIMIT_WORKINGSET;
            _info.MinimumWorkingSetSize = value;
            return *this;
        }

        NTW_INLINE limits& max_working_set(std::size_t value) noexcept
        {
            _info.LimitFlags |= JOB_OBJECT_LIMIT_WORKINGSET;
            _info.MaximumWorkingSetSize = value;
            return *this;
        }

        NTW_INLINE limits& max_processes(std::uint32_t value) noexcept
        {
            _info.LimitFlags |= JOB_OBJECT_LIMIT_ACTIVE_PROCESS;
            _info.ActiveProcessLimit = value;
            return *this;
        }

        NTW_INLINE limits& affinity(std::uintptr_t value) noexcept
        {
            _info.LimitFlags |= JOB_OBJECT_LIMIT_AFFINITY;
            _info.Affinity = value;
            return *this;
        }

        NTW_INLINE limits& priority(std::uint32_t value) noexcept
        {
            _info.LimitFlags |= JOB_OBJECT_LIMIT_PRIORITY_CLASS;
            _info.PriorityClass = value;
            return *this;
        }

        NTW_INLINE limits& scheduling(std::uint32_t value) noexcept
        {
            _info.LimitFlags |= JOB_OBJECT_LIMIT_SCHEDULING_CLASS;
            _info.SchedulingClass = value;
            return *this;
        }
    };

    class extended_limits : public detail::query_info, detail::set_info {
        JOBOBJECT_EXTENDED_LIMIT_INFORMATION _info;

    public:
        NTW_INLINE extended_limits() noexcept
        {
            _info.BasicLimitInformation.LimitFlags = 0;
        }

        constexpr static JOBOBJECTINFOCLASS info_class =
            static_cast<JOBOBJECTINFOCLASS>(JobObjectExtendedLimitInformation);

        NTW_INLINE JOBOBJECT_EXTENDED_LIMIT_INFORMATION& get() noexcept { return _info; }

        NTW_INLINE std::optional<duration> process_max_user_time() const noexcept
        {
            if(_info.BasicLimitInformation.LimitFlags & JOB_OBJECT_LIMIT_PROCESS_TIME)
                return ntw::duration{
                    _info.BasicLimitInformation.PerProcessUserTimeLimit.QuadPart
                };
            return std::nullopt;
        }

        NTW_INLINE std::optional<duration> max_user_time() const noexcept
        {
            if(_info.BasicLimitInformation.LimitFlags & JOB_OBJECT_LIMIT_JOB_TIME)
                return ntw::duration{
                    _info.BasicLimitInformation.PerJobUserTimeLimit.QuadPart
                };
            return std::nullopt;
        }

        NTW_INLINE std::optional<std::size_t> min_working_set() const noexcept
        {
            if(_info.BasicLimitInformation.LimitFlags & JOB_OBJECT_LIMIT_WORKINGSET)
                return _info.BasicLimitInformation.MinimumWorkingSetSize;
            return std::nullopt;
        }

        NTW_INLINE std::optional<std::size_t> max_working_set() const noexcept
        {
            if(_info.BasicLimitInformation.LimitFlags & JOB_OBJECT_LIMIT_WORKINGSET)
                return _info.BasicLimitInformation.MaximumWorkingSetSize;
            return std::nullopt;
        }

        NTW_INLINE std::optional<std::uint32_t> max_processes() const noexcept
        {
            if(_info.BasicLimitInformation.LimitFlags & JOB_OBJECT_LIMIT_ACTIVE_PROCESS)
                return _info.BasicLimitInformation.ActiveProcessLimit;
            return std::nullopt;
        }

        NTW_INLINE std::optional<std::uintptr_t> affinity() const noexcept
        {
            if(_info.BasicLimitInformation.LimitFlags & JOB_OBJECT_LIMIT_AFFINITY)
                return _info.BasicLimitInformation.Affinity;
            return std::nullopt;
        }

        NTW_INLINE std::optional<std::uint32_t> priority() const noexcept
        {
            if(_info.BasicLimitInformation.LimitFlags & JOB_OBJECT_LIMIT_PRIORITY_CLASS)
                return _info.BasicLimitInformation.PriorityClass;
            return std::nullopt;
        }

        NTW_INLINE std::optional<std::uint32_t> scheduling() const noexcept
        {
            if(_info.BasicLimitInformation.LimitFlags & JOB_OBJECT_LIMIT_SCHEDULING_CLASS)
                return _info.BasicLimitInformation.SchedulingClass;
            return std::nullopt;
        }

        NTW_INLINE std::optional<std::size_t> process_max_memory() const noexcept
        {
            if(_info.BasicLimitInformation.LimitFlags & JOB_OBJECT_LIMIT_PROCESS_MEMORY)
                return _info.ProcessMemoryLimit;
            return std::nullopt;
        }

        NTW_INLINE std::optional<std::size_t> max_memory() const noexcept
        {
            if(_info.BasicLimitInformation.LimitFlags & JOB_OBJECT_LIMIT_JOB_MEMORY)
                return _info.JobMemoryLimit;
            return std::nullopt;
        }

        NTW_INLINE extended_limits& process_max_user_time(duration value) noexcept
        {
            _info.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_PROCESS_TIME;
            _info.BasicLimitInformation.PerProcessUserTimeLimit.QuadPart = value.count();
            return *this;
        }

        NTW_INLINE extended_limits& max_user_time(duration value) noexcept
        {
            _info.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_JOB_TIME;
            _info.BasicLimitInformation.PerJobUserTimeLimit.QuadPart = value.count();
            return *this;
        }

        NTW_INLINE extended_limits& min_working_set(std::size_t value) noexcept
        {
            _info.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_WORKINGSET;
            _info.BasicLimitInformation.MinimumWorkingSetSize = value;
            return *this;
        }

        NTW_INLINE extended_limits& max_working_set(std::size_t value) noexcept
        {
            _info.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_WORKINGSET;
            _info.BasicLimitInformation.MaximumWorkingSetSize = value;
            return *this;
        }

        NTW_INLINE extended_limits& max_processes(std::uint32_t value) noexcept
        {
            _info.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_ACTIVE_PROCESS;
            _info.BasicLimitInformation.ActiveProcessLimit = value;
            return *this;
        }

        NTW_INLINE extended_limits& affinity(std::uintptr_t value) noexcept
        {
            _info.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_AFFINITY;
            _info.BasicLimitInformation.Affinity = value;
            return *this;
        }

        NTW_INLINE extended_limits& priority(std::uint32_t value) noexcept
        {
            _info.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_PRIORITY_CLASS;
            _info.BasicLimitInformation.PriorityClass = value;
            return *this;
        }

        NTW_INLINE extended_limits& scheduling(std::uint32_t value) noexcept
        {
            _info.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_SCHEDULING_CLASS;
            _info.BasicLimitInformation.SchedulingClass = value;
            return *this;
        }

        NTW_INLINE extended_limits& process_max_memory(std::size_t value) noexcept
        {
            _info.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_PROCESS_MEMORY;
            _info.ProcessMemoryLimit = value;
            return *this;
        }

        NTW_INLINE extended_limits& max_memory(std::size_t value) noexcept
        {
            _info.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_JOB_MEMORY;
            _info.JobMemoryLimit = value;
            return *this;
        }

        NTW_INLINE const IO_COUNTERS& io_counters() const noexcept
        {
            return _info.IoInfo;
        }

        NTW_INLINE std::size_t peak_memory() const noexcept
        {
            return _info.PeakJobMemoryUsed;
        }

        NTW_INLINE std::size_t peak_process_memory() const noexcept
        {
            return _info.PeakProcessMemoryUsed;
        }
    };

} // namespace ntw::job
