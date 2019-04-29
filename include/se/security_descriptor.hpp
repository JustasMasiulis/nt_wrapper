#pragma once
#include "../detail/common.hpp"
#include "../detail/unwrap.hpp"
#include <cstdint>
#include <memory>

namespace ntw::se {

    /// \brief CRTP class to extend different types of security descriptors.
    template<class D>
    struct security_desc_base {
        NTW_INLINE std::uint16_t control() const
        {
            return static_cast<const D*>(this)->get()->Control;
        }

        NTW_INLINE std::uint8_t rm_control() const
        {
            return static_cast<const D*>(this)->get()->Sbz1;
        }

        NTW_INLINE bool rm_control_present() const
        {
            return control() & SE_RM_CONTROL_VALID;
        }

        /// \brief Checks whether a dacl is present checking for SE_DACL_PRESENT flag.
        NTW_INLINE bool dacl_present() const { return control() & SE_DACL_PRESENT; }

        /// \brief Checks whether a _present_ dacl is defaulted.
        /// \warning dacl _must_ be present.
        NTW_INLINE bool dacl_defaulted() const { return control() & SE_DACL_DEFAULTED; }


        /// \brief Checks whether a sacl is present checking for SE_SACL_PRESENT flag.
        NTW_INLINE bool sacl_present() const { return control() & SE_SACL_PRESENT; }

        /// \brief Checks whether a _present_ sacl is defaulted.
        /// \warning sacl _must_ be present.
        NTW_INLINE bool sacl_defaulted() const { return control() & SE_SACL_DEFAULTED; }


        NTW_INLINE bool group_defaulted() const { return control() & SE_GROUP_DEFAULTED; }

        NTW_INLINE bool owner_defaulted() const { return control() & SE_OWNER_DEFAULTED; }
    };

    class security_builder {
        SECURITY_DESCRIPTOR _sd;

    public:
        security_builder& owner(SID* o)
        {
            _sd.Control &= ~SE_OWNER_DEFAULTED;
            _sd.Owner = o;
            return *this;
        }

        security_builder& default_owner()
        {
            _sd.Control |= SE_OWNER_DEFAULTED;
            return *this;
        }

        security_builder& group(SID* g)
        {
            _sd.Control &= ~SE_GROUP_DEFAULTED;
            _sd.Group = g;
            return *this;
        }

        security_builder& default_group()
        {
            _sd.Control |= SE_GROUP_DEFAULTED;
            return *this;
        }

        security_builder& sacl(ACL* g)
        {
            _sd.Control &= ~SE_SACL_DEFAULTED;
            _sd.Control |= SE_SACL_PRESENT;
            _sd.Sacl = g;
            return *this;
        }

        security_builder& default_sacl()
        {
            _sd.Control |= SE_SACL_DEFAULTED;
            return *this;
        }

        security_builder& no_sacl()
        {
            _sd.Control &= ~SE_SACL_PRESENT;
            return *this;
        }

        security_builder& dacl(ACL* d)
        {
            _sd.Control &= ~SE_DACL_DEFAULTED;
            _sd.Control |= SE_DACL_PRESENT;
            _sd.Dacl = d;
            return *this;
        }

        security_builder& default_dacl()
        {
            _sd.Control |= SE_DACL_DEFAULTED;
            return *this;
        }

        security_builder& no_dacl()
        {
            _sd.Control &= ~SE_DACL_PRESENT;
            return *this;
        }

        security_builder& rm_control(std::uint8_t c)
        {
            _sd.Control |= SE_RM_CONTROL_VALID;
            _sd.Sbz1 = c;
            return *this;
        }

        security_builder& no_rm_control()
        {
            _sd.Control &= ~SE_RM_CONTROL_VALID;
            _sd.Sbz1 = 0;
            return *this;
        }
    };

    class security_desc : security_desc_base<security_desc> {
        SECURITY_DESCRIPTOR _sd;

    public:
        NTW_INLINE constexpr security_desc() : _sd{ 0 }
        {
            _sd.Revision = SECURITY_DESCRIPTOR_REVISION;
        }

        NTW_INLINE ACL* dacl() const { return _sd.Dacl; }

        NTW_INLINE ACL* sacl() const { return _sd.Sacl; }

        NTW_INLINE SID* group() const { return _sd.Group; }

        NTW_INLINE SID* owner() const { return _sd.Owner; }

        SECURITY_DESCRIPTOR* get() { return &_sd; }
    };

    class rel_security_desc : security_desc_base<rel_security_desc> {
        SECURITY_DESCRIPTOR_RELATIVE* _sd = nullptr;

        template<class T>
        T* _at_offset(std::uint32_t offset) const
        {
            return offset ? reinterpret_cast<T*>(&_sd->Revision + offset) : nullptr;
        }

    public:
        constexpr rel_security_desc() = default;

        NTW_INLINE ACL* dacl() const { return _at_offset<ACL*>(_sd->Dacl); }

        NTW_INLINE ACL* sacl() const { return _at_offset<ACL*>(_sd->Sacl); }

        NTW_INLINE SID* group() const { return _at_offset<SID*>(_sd->Group); }

        NTW_INLINE SID* owner() const { return _at_offset<SID*>(_sd->Owner); }

        SECURITY_DESCRIPTOR_RELATIVE* get() { return _sd; }
    };

} // namespace ntw::se
