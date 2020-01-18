#pragma once
#include "../detail/common.hpp"
#include "../detail/unwrap.hpp"
#include <cstdint>

namespace ntw::se {

    /// \brief CRTP class to extend different types of security descriptors.
    template<class D>
    struct base_security_desc {
        /// \brief Returns the Control member of security descriptor
        NTW_INLINE std::uint16_t control() const
        {
            return static_cast<const D*>(this)->get()->Control;
        }

        /// \brief Returns the Sbz1/resource manager control member.
        /// \warning Requires rm_control_present
        NTW_INLINE std::uint8_t rm_control() const
        {
            return static_cast<const D*>(this)->get()->Sbz1;
        }

        /// \brief Returns whether rm_control member is present checking for
        /// SE_RM_CONTROL_VALID flag.
        NTW_INLINE bool rm_control_present() const
        {
            return control() & SE_RM_CONTROL_VALID;
        }

        /// \brief Returns whether a dacl is present checking for SE_DACL_PRESENT flag.
        NTW_INLINE bool dacl_present() const { return control() & SE_DACL_PRESENT; }

        /// \brief Returns whether a dacl is protected checking for SE_DACL_PROTECTED
        /// flag.
        NTW_INLINE bool dacl_protected() const { return control() & SE_DACL_PROTECTED; }

        /// \brief Returns whether a present dacl is defaulted.
        /// \warning Requires dacl_present
        NTW_INLINE bool dacl_defaulted() const { return control() & SE_DACL_DEFAULTED; }


        /// \brief Returns whether a sacl is present checking for SE_SACL_PRESENT flag.
        NTW_INLINE bool sacl_present() const { return control() & SE_SACL_PRESENT; }

        /// \brief Returns whether a sacl is protected checking for SE_SACL_PROTECTED
        /// flag.
        NTW_INLINE bool sacl_protected() const { return control() & SE_SACL_PROTECTED; }

        /// \brief Returns whether a present sacl is defaulted.
        /// \warning Requires sacl_present.
        NTW_INLINE bool sacl_defaulted() const { return control() & SE_SACL_DEFAULTED; }

        /// \brief Returns whether the group is defaulted checking for SE_GROUP_DEFAULTED
        /// flag.
        NTW_INLINE bool group_defaulted() const { return control() & SE_GROUP_DEFAULTED; }

        /// \brief Returns whether the owner is defaulted checking for SE_OWNER_DEFAULTED
        /// flag.
        NTW_INLINE bool owner_defaulted() const { return control() & SE_OWNER_DEFAULTED; }
    };

    class security_desc : base_security_desc<security_desc> {
        SECURITY_DESCRIPTOR _sd;

    public:
        NTW_INLINE constexpr security_desc() : _sd{ 0 }
        {
            _sd.Revision = SECURITY_DESCRIPTOR_REVISION;
        }

        /// \brief Returns the dacl.
        NTW_INLINE ACL* dacl() { return _sd.Dacl; }

        /// \brief Returns the sacl.
        NTW_INLINE ACL* sacl() { return _sd.Sacl; }

        /// \brief Returns the group.
        NTW_INLINE SID* group() { return _sd.Group; }

        /// \brief Returns the owner.
        NTW_INLINE SID* owner() { return _sd.Owner; }

        /// \brief Returns pointer to the internal security descriptor.
        NTW_INLINE SECURITY_DESCRIPTOR* get() { return &_sd; }


        /// \brief Sets the owner member and clears defaulted flag.
        NTW_INLINE security_desc& owner(SID* o)
        {
            _sd.Control &= ~SE_OWNER_DEFAULTED;
            _sd.Owner = o;
            return *this;
        }

        /// \brief Sets the defaulted owner flag.
        NTW_INLINE security_desc& default_owner()
        {
            _sd.Control |= SE_OWNER_DEFAULTED;
            return *this;
        }

        /// \brief Sets the group member and clears defaulted flag.
        NTW_INLINE security_desc& group(SID* g)
        {
            _sd.Control &= ~SE_GROUP_DEFAULTED;
            _sd.Group = g;
            return *this;
        }

        /// \brief Sets the defaulted group flag.
        NTW_INLINE security_desc& default_group()
        {
            _sd.Control |= SE_GROUP_DEFAULTED;
            return *this;
        }

        /// \brief Sets the sacl member, clears defaulted and sets present flags.
        NTW_INLINE security_desc& sacl(ACL* g)
        {
            _sd.Control &= ~SE_SACL_DEFAULTED;
            _sd.Control |= SE_SACL_PRESENT;
            _sd.Sacl = g;
            return *this;
        }

        /// \brief Sets the present and defaulted flags for sacl.
        NTW_INLINE security_desc& default_sacl()
        {
            _sd.Control |= (SE_SACL_PRESENT | SE_SACL_DEFAULTED);
            return *this;
        }

        /// \brief Clears present flag for sacl.
        NTW_INLINE security_desc& no_sacl()
        {
            _sd.Control &= ~SE_SACL_PRESENT;
            return *this;
        }

        /// \brief Enables or disables SE_SACL_PROTECTED flag
        NTW_INLINE security_desc& protect_sacl(bool enable = true)
        {
            if(enable)
                _sd.Control |= SE_SACL_PROTECTED;
            else
                _sd.Control &= ~SE_SACL_PROTECTED;
            return *this;
        }

        /// \brief Sets the dacl member, clears defaulted and sets present flags.
        NTW_INLINE security_desc& dacl(ACL* d)
        {
            _sd.Control &= ~SE_DACL_DEFAULTED;
            _sd.Control |= SE_DACL_PRESENT;
            _sd.Dacl = d;
            return *this;
        }

        /// \brief Sets the present and defaulted flags for dacl.
        NTW_INLINE security_desc& default_dacl()
        {
            _sd.Control |= (SE_DACL_PRESENT | SE_DACL_DEFAULTED);
            return *this;
        }

        /// \brief Clears present flag for dacl.
        NTW_INLINE security_desc& no_dacl()
        {
            _sd.Control &= ~SE_DACL_PRESENT;
            return *this;
        }

        /// \brief Enables or disables SE_DACL_PROTECTED flag
        NTW_INLINE security_desc& protect_dacl(bool enable = true)
        {
            if(enable)
                _sd.Control |= SE_DACL_PROTECTED;
            else
                _sd.Control &= ~SE_DACL_PROTECTED;
            return *this;
        }

        /// \brief Sets the sbz1 member and sets the present flag.
        NTW_INLINE security_desc& rm_control(std::uint8_t c)
        {
            _sd.Control |= SE_RM_CONTROL_VALID;
            _sd.Sbz1 = c;
            return *this;
        }

        /// \brief Clears present flag and sbz1 member.
        NTW_INLINE security_desc& no_rm_control()
        {
            _sd.Control &= ~SE_RM_CONTROL_VALID;
            _sd.Sbz1 = 0;
            return *this;
        }
    };

    class rel_security_desc : base_security_desc<rel_security_desc> {
        SECURITY_DESCRIPTOR_RELATIVE* _sd = nullptr;

        template<class T>
        T* _at_offset(std::uint32_t offset) const
        {
            return offset ? reinterpret_cast<T*>(&_sd->Revision + offset) : nullptr;
        }

    public:
        NTW_INLINE constexpr rel_security_desc() = default;
        NTW_INLINE constexpr rel_security_desc(SECURITY_DESCRIPTOR_RELATIVE* sd) : _sd(sd)
        {}


        NTW_INLINE ACL* dacl() { return _at_offset<ACL*>(_sd->Dacl); }

        NTW_INLINE ACL* sacl() { return _at_offset<ACL*>(_sd->Sacl); }

        NTW_INLINE SID* group() { return _at_offset<SID*>(_sd->Group); }

        NTW_INLINE SID* owner() { return _at_offset<SID*>(_sd->Owner); }

        NTW_INLINE SECURITY_DESCRIPTOR_RELATIVE* get() { return _sd; }
    };

} // namespace ntw::se
