#pragma once

#include <Common/Common.hpp>

#include <DeadLock/SDK/Types/CHandle.hpp>

class CGameEntitySystem;
class CEntityInstance;

auto Hook_OnAddEntity( CGameEntitySystem* pCGameEntitySystem , CEntityInstance* pInst , CHandle handle ) -> void;

using OnAddEntity_t = decltype( &Hook_OnAddEntity );
inline OnAddEntity_t OnAddEntity_o = nullptr;
