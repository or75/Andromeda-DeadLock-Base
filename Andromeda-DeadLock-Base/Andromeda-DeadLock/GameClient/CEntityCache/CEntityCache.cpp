#include "CEntityCache.hpp"

static CEntityCache g_CEntityCache{};

void CEntityCache::OnAddEntity( CEntityInstance* pInst , CHandle handle )
{
	std::scoped_lock lock( m_Lock );

	auto pBaseEntity = pInst->pEntityIdentity()->pBaseEntity();

	if ( pBaseEntity )
	{
		auto it = std::find_if( m_CachedEntity.begin() , m_CachedEntity.end() , [handle]( const CachedEntity_t& i )
		{
			return i.m_Handle == handle;
		} );

		if ( it == m_CachedEntity.end() )
		{
			CachedEntity_t CachedBoxEntity;

			CachedBoxEntity.m_Handle = handle;
			CachedBoxEntity.m_Type = GetEntityType( pBaseEntity );

			if ( CachedBoxEntity.m_Type != CachedEntity_t::UNKNOWN )
				m_CachedEntity.emplace_back( CachedBoxEntity );
		}
		else
		{
			it->m_Handle = handle;
			it->m_Type = GetEntityType( pBaseEntity );
		}
	}
}

void CEntityCache::OnRemoveEntity( CEntityInstance* pInst , CHandle handle )
{
	std::scoped_lock lock( m_Lock );

	auto it = std::find_if( m_CachedEntity.begin() , m_CachedEntity.end() , [handle]( const CachedEntity_t& i )
	{
		return i.m_Handle == handle;
	} );

	if ( it != m_CachedEntity.end() )
	{
		it->m_Type = CachedEntity_t::UNKNOWN;

		auto NewEnd = std::remove_if( m_CachedEntity.begin() , m_CachedEntity.end() , []( const CachedEntity_t& i )
		{
			return i.m_Type == CachedEntity_t::UNKNOWN;
		} );

		m_CachedEntity.erase( NewEnd , m_CachedEntity.end() );
	}
}

auto CEntityCache::GetEntityType( C_BaseEntity* pBaseEntity ) -> CachedEntity_t::Type
{
	if ( pBaseEntity->IsCitadelPlayerController() )
		return CachedEntity_t::CITADEL_PLAYER_CONTROLLER;
	else if ( pBaseEntity->IsCitadelPlayerPawn() )
		return CachedEntity_t::CITADEL_PLAYER_PAWN;
	else if ( pBaseEntity->IsNpcTrooper() )
		return CachedEntity_t::NPC_TROOPER;

	return CachedEntity_t::UNKNOWN;
}

auto GetEntityCache() -> CEntityCache*
{
	return &g_CEntityCache;
}
