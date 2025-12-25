#pragma once

#include <Common/Common.hpp>

#include <DeadLock/SDK/SDK.hpp>
#include <DeadLock/CBasePattern.hpp>

#define IVENGINE_TO_CLIENT_INTERFACE_VERSION "Source2EngineToClient001"

namespace IVEngineToClient_Search
{
	inline CBasePattern IsInGameFn = { VmpStr( "IVEngineToClient::IsInGame" ) , VmpStr( "48 8B ? ? ? ? ? 48 85 C0 74 15 80 B8 ? ? ? ? ? 75 0C 83 B8 ? ? ? ? 06" ) , ENGINE2_DLL };
	inline CBasePattern ExecuteClientCmdFn = { VmpStr( "IVEngineToClient::ExecuteClientCmd" ) , VmpStr( "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 81 EC ? ? ? ? 8D 42 ? 41 0F B6 E9" ) , ENGINE2_DLL };
}

class IVEngineToClient
{
public:
	DECLARATE_DEADLOCK_FUNCTION( bool , IsInGame , ( ) , IVEngineToClient , ( IVEngineToClient* ) , ( this ) );
	DECLARATE_DEADLOCK_FUNCTION( void , ExecuteClientCmd , ( const char* szStringCmd ) , IVEngineToClient , ( IVEngineToClient* , uint32_t , const char* , bool ) , ( this , 0 , szStringCmd , true ) );
};
