#pragma once

#include "CUserCmd.hpp"

#include <DeadLock/SDK/SDK.hpp>
#include <DeadLock/SDK/FunctionListSDK.hpp>

class CCitadelInput
{
public:
	auto GetSequenceNumber( CCitadelPlayerController* pLocalCitadelPlayerController ) -> uint32_t
	{
		int32_t OutputTick = 0;
		GetCUserCmdTick( pLocalCitadelPlayerController , &OutputTick );

		int32_t Tick = OutputTick - 1;

		if ( OutputTick == -1 )
			Tick = -1;

		auto* pCUserCmdArray = GetCUserCmdArray( SDK::Pointers::GetFirstCUserCmdArray() , Tick );

		if ( pCUserCmdArray )
			return pCUserCmdArray->m_nSequenceNumber();

		return 0;
	}

	auto GetUserCmd( CCitadelPlayerController* pLocalCitadelPlayerController ) -> CUserCmd*
	{
		const auto SequenceNumber = GetSequenceNumber( pLocalCitadelPlayerController );

		if ( SequenceNumber )
			return GetCUserCmdBySequenceNumber( pLocalCitadelPlayerController , SequenceNumber );

		return nullptr;
	}
};
