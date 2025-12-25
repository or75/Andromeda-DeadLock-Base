#include "CHandle.hpp"

#include <DeadLock/SDK/SDK.hpp>
#include <DeadLock/SDK/Interface/CGameEntitySystem.hpp>

auto CHandle::GetBaseEntity() const -> C_BaseEntity*
{
	if ( auto pEntity = (C_BaseEntity*)SDK::Interfaces::GameEntitySystem()->GetBaseEntity( GetEntryIndex() ); pEntity )
		return pEntity;

	return nullptr;
}
