#include "pch.h"

#include "processbase.h"
#include "processmemoryarea.h"

using namespace XKLib;

ProcessMemoryArea::ModifiableProtectionFlags::ModifiableProtectionFlags(
  ProcessMemoryArea* pma)
 : _pma(pma)
{
}

auto ProcessMemoryArea::ModifiableProtectionFlags::change(mapf_t flags)
  -> mapf_t
{
    mapf_t old_flags = _flags;

    try
    {
        MemoryUtils::ProtectMemoryArea(_pma->_process_base.id(),
                                       _pma->begin(),
                                       _pma->size(),
                                       flags);

        _flags = flags;
    }
    catch (Exception& e)
    {
        throw Exception(e.msg());
    }

    return old_flags;
}

auto ProcessMemoryArea::ModifiableProtectionFlags::cachedValue()
  -> mapf_t&
{
    return _flags;
}

auto ProcessMemoryArea::ModifiableProtectionFlags::operator|(mapf_t flags)
  -> mapf_t
{
    return flags | _flags;
}

auto ProcessMemoryArea::ModifiableProtectionFlags::operator&(mapf_t flags)
  -> mapf_t
{
    return flags & _flags;
}

auto ProcessMemoryArea::ModifiableProtectionFlags::operator=(mapf_t flags)
  -> ModifiableProtectionFlags&
{
    change(flags);

    return *this;
}

auto ProcessMemoryArea::ModifiableProtectionFlags::operator|=(mapf_t flags)
  -> void
{
    change(flags | _flags);
}

auto ProcessMemoryArea::ModifiableProtectionFlags::operator&=(mapf_t flags)
  -> void
{
    change(flags & _flags);
}

ProcessMemoryArea::ProcessMemoryArea(ProcessBase process)
 : _protection_flags(ModifiableProtectionFlags(this)),
   _process_base(process)
{
}

auto ProcessMemoryArea::protectionFlags() -> ModifiableProtectionFlags&
{
    return _protection_flags;
}

auto ProcessMemoryArea::initProtectionFlags(mapf_t flags) -> void
{
    _protection_flags.cachedValue() = flags;
}

auto ProcessMemoryArea::processBase() -> ProcessBase
{
    return _process_base;
}

auto ProcessMemoryArea::read(std::size_t size, std::size_t shift)
  -> bytes_t
{
    if (ProcessBase::self().id() == _process_base.id())
    {
        return bytes_t(&begin<data_t>()[shift],
                       &begin<data_t>()[shift + size]);
    }

    return MemoryUtils::ReadProcessMemoryArea(_process_base.id(),
                                              begin<std::size_t>()
                                                + shift,
                                              size);
}

auto ProcessMemoryArea::write(const bytes_t& bytes, std::size_t shift)
  -> void
{
    if (ProcessBase::self().id() == _process_base.id())
    {
        std::copy(bytes.begin(), bytes.end(), begin<data_t>() + shift);
        return;
    }

    MemoryUtils::WriteProcessMemoryArea(_process_base.id(),
                                        bytes,
                                        begin<std::size_t>() + shift);
}

auto XKLib::ProcessMemoryArea::isDeniedByOS() -> bool
{
    return _protection_flags.cachedValue() == 0
#ifndef WIN32
           || name() == "[vvar]"
#endif
      ;
}

auto XKLib::ProcessMemoryArea::isReadable() -> bool
{
    return (_protection_flags.cachedValue() & ProtectionFlags::R)
           && !isDeniedByOS();
}

auto XKLib::ProcessMemoryArea::isWritable() -> bool
{
    return (_protection_flags.cachedValue() & ProtectionFlags::W)
           && !isDeniedByOS();
}
