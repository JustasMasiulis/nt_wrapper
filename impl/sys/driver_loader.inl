#pragma once
#include "../../include/ntw/sys/driver_loader.hpp"
#include <algorithm>

namespace ntw::sys {

    NTW_INLINE driver::~driver() noexcept { static_cast<void>(unload()); }

    NTW_INLINE driver::driver(driver&& other) noexcept
        : _service_path(_service_path_buffer, other._service_path.size())
    {
        auto& sp  = other._service_path.get();
        sp.Buffer = nullptr;
        for(auto i = 0; i < (sp.Length >> 1); ++i)
            _service_path_buffer[i] = other._service_path_buffer[i];
    }

    NTW_INLINE driver& driver::operator=(driver&& other) noexcept
    {
        unload();
        _service_path.get().Length        = other._service_path.get().Length;
        _service_path.get().MaximumLength = other._service_path.get().MaximumLength;
        for(auto i = 0; i < (other._service_path.get().Length >> 1); ++i)
            _service_path_buffer[i] = other._service_path_buffer[i];
        return *this;
    }

    NTW_INLINE status driver::_build_service_path(unicode_string file_path) noexcept
    {
        // load into service path buffer
        // L"\\Registry\\Machine\\System\\CurrentControlSet\\Services\\"
        // obfuscation + speed
        auto uptr = reinterpret_cast<std::uint64_t*>(_service_path_buffer);
        uptr[0]   = 28992356398268508;
        uptr[1]   = 32088645568757865;
        uptr[2]   = 27303403459444857;
        uptr[3]   = 30962698416554083;
        uptr[4]   = 34058828670304357;
        uptr[5]   = 30681206260760691;
        uptr[6]   = 32088649860579420;
        uptr[7]   = 32651569751457906;
        uptr[8]   = 32651569752113219;
        uptr[9]   = 23362886930727026;
        uptr[10]  = 23362818211577957;
        uptr[11]  = 29555379368231013;
        uptr[12]  = 25896191785238627;

        // find first separator
        const auto first = std::find(file_path.rbegin(), file_path.rend(), L'\\');
        if(first == file_path.rend())
            STATUS_OBJECT_PATH_SYNTAX_BAD;

        // find file extension
        const auto last = std::find(file_path.rbegin(), first, L'.');

        // check size
        const std::size_t size = (first - last) - 1;
        if(size > 256 - 52)
            return STATUS_NAME_TOO_LONG;

        // copy file name without extension
        std::copy_n(first.base(), size, _service_path_buffer + 52);

        _service_path = { _service_path_buffer, static_cast<std::uint16_t>(size + 52) };
        return STATUS_SUCCESS;
    }

    NTW_INLINE status driver::_init_service_fields(unicode_string            path,
                                                   const io::unique_reg_key& reg,
                                                   start                     start_type,
                                                   error_control error_control_type,
                                                   type          driver_type) noexcept
    {
        alignas(8) wchar_t buffer[16];
        auto               uibuffer = reinterpret_cast<std::uint64_t*>(buffer);

        // ImagePath
        uibuffer[0] = 28992339220168777;
        uibuffer[1] = 32651513915506789;
        uibuffer[2] = 104;

        auto status =
            reg.set({ buffer, 9 }, REG_EXPAND_SZ, path.begin(), path.byte_size() + 2);
        if(!status.success())
            return status;

        // Type
        uibuffer[0] = 28429453692043348;

        status = reg.set({ buffer, 4 }, static_cast<ulong_t>(driver_type));
        if(!status.success())
            return status;

        // ErrorControl
        uibuffer[0] = 31244212048625733;
        uibuffer[1] = 30962724183933042;
        uibuffer[2] = 30399774233591924;

        status = reg.set({ buffer, 12 }, static_cast<ulong_t>(error_control_type));
        if(!status.success())
            return status;

        uibuffer[0] = 32088563964444755;
        uibuffer[1] = 116;

        return reg.set({ buffer, 5 }, static_cast<ulong_t>(start_type));
    }

    NTW_INLINE result<driver> driver::load(unicode_string path,
                                           start          start_type,
                                           error_control  error_control_type,
                                           type           driver_type) noexcept
    {
        driver d;
        auto   status = d._build_service_path(path);
        if(!status.success())
            return status;

        io::reg_create_options options;
        if(start_type == start::manual_with_cleanup) {
            start_type = start::manual;
            options.non_preserved();
        }

        const auto reg = io::unique_reg_key::create(
            d._service_path, io::reg_access{}.write(), options);
        if(!reg)
            return { reg.status() };

        _init_service_fields(path, *reg, start_type, error_control_type, driver_type);

        status = NTW_SYSCALL(NtLoadDriver)(&d._service_path.get());
        return { status, std::move(d) };
    }

    NTW_INLINE status driver::unload() noexcept
    {
        if(_service_path.begin()) {
            const auto s = NTW_SYSCALL(NtUnloadDriver(&_service_path.get()));
            if(NT_SUCCESS(s))
                _service_path.get().Buffer = nullptr;
            return s;
        }
        return STATUS_NOT_FOUND;
    }

    NTW_INLINE void driver::release() noexcept { _service_path.get().Buffer = nullptr; }

} // namespace ntw::sys