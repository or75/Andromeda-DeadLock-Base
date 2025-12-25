#pragma once

#include <Common/Common.hpp>

#include <vector>

#include <DeadLock/SDK/Types/CHandle.hpp>
#include <DeadLock/SDK/Types/CEntityData.hpp>

struct CachedEntity_t
{
    enum Type
    {
        UNKNOWN = 0 ,
        CITADEL_PLAYER_CONTROLLER ,
        CITADEL_PLAYER_PAWN ,
        NPC_TROOPER,
    };

    CHandle m_Handle = { INVALID_EHANDLE_INDEX };
    Type m_Type = UNKNOWN;

    Rect_t m_Bbox = { 0.f , 0.f , 0.f , 0.f };

    bool m_bDraw = false;
    bool m_bVisible = false; // for only players
};

class IEntityCache
{
public:
    virtual void OnAddEntity( CEntityInstance* pInst , CHandle handle ) = 0;
    virtual void OnRemoveEntity( CEntityInstance* pInst , CHandle handle ) = 0;
};

class CEntityCache final : public IEntityCache
{
public:
    using CachedEntityVec_t = std::vector<CachedEntity_t>;
    using Lock_t = std::recursive_mutex;

public:
    virtual void OnAddEntity( CEntityInstance* pInst , CHandle handle ) override;
    virtual void OnRemoveEntity( CEntityInstance* pInst , CHandle handle ) override;

public:
    auto GetEntityType( C_BaseEntity* pBaseEntity ) -> CachedEntity_t::Type;

public:
    inline auto GetCachedEntity() -> CachedEntityVec_t*
    {
        return &m_CachedEntity;
    }

    inline auto GetLock() -> Lock_t&
    {
        return m_Lock;
    }

private:
    CachedEntityVec_t m_CachedEntity;
    Lock_t m_Lock;
};

auto GetEntityCache() -> CEntityCache*;
