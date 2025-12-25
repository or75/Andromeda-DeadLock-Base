#include "SDK.hpp"

#include <DllLauncher.hpp>
#include <Common/MemoryEngine.hpp>

#include <DeadLock/SDK/Interface/Interface.hpp>
#include <DeadLock/SDK/Interface/CShemaSystemSDK.hpp>
#include <DeadLock/SDK/Interface/IEngineToClient.hpp>
#include <DeadLock/SDK/Interface/CInputSystem.hpp>
#include <DeadLock/SDK/Interface/CSoundOpSystem.hpp>

#define INCLUDE_DEADLOCK_SEARCH_FUNCTION(Interface,FuncName)\
if ( !##Interface##_Search::##FuncName##Fn.Search() )\
	bIsReady = false;

namespace SDK
{
	CSchemaSystem* Interfaces::g_pSchemaSystem = nullptr;
	IVEngineToClient* Interfaces::g_pEngineToClient = nullptr;
	CGameEntitySystem* Interfaces::g_pGameEntitySystem = nullptr;
	CInputSystem* Interfaces::g_pInputSystem = nullptr;
	CSoundOpSystem* Interfaces::g_pSoundOpSystem = nullptr;

	CUserCmd** Pointers::g_ppCUserCmd = nullptr;

	auto Interfaces::SchemaSystem() -> CSchemaSystem*
	{
		if ( !g_pSchemaSystem )
		{
			CreateInterfaceFn pfnFactory = CaptureFactory( SCHEMASYSTEM_DLL );
			g_pSchemaSystem = CaptureInterface<CSchemaSystem>( pfnFactory , XorStr( SCHEMA_SYSTEM_INTERFACE_VERSION ) );

			bool bIsReady = true;

			INCLUDE_DEADLOCK_SEARCH_FUNCTION( CSchemaSystem , GetAllTypeScope );

			if ( !bIsReady )
				return nullptr;
		}

		return g_pSchemaSystem;
	}

	auto Interfaces::EngineToClient() -> IVEngineToClient*
	{
		if ( !g_pEngineToClient )
		{
			CreateInterfaceFn pfnFactory = CaptureFactory( ENGINE2_DLL );
			g_pEngineToClient = CaptureInterface<IVEngineToClient>( pfnFactory , XorStr( IVENGINE_TO_CLIENT_INTERFACE_VERSION ) );

			bool bIsReady = true;

			INCLUDE_DEADLOCK_SEARCH_FUNCTION( IVEngineToClient , IsInGame );
			INCLUDE_DEADLOCK_SEARCH_FUNCTION( IVEngineToClient , ExecuteClientCmd );

			if ( !bIsReady )
				return nullptr;
		}

		return g_pEngineToClient;
	}

	auto Interfaces::GameEntitySystem() -> CGameEntitySystem*
	{
		if ( !g_pGameEntitySystem )
		{
			/*
			00007FFA820122D | lea rcx,qword ptr ds:[0x7FFA834FEE50]                 | 00007FFA834FEE50:"target"
			00007FFA820122D | mov r8d,0xFFFFFFFF                                    |
			00007FFA820122D | mov qword ptr ss:[rsp+0x28],rcx                       |
			00007FFA820122E | lea rdx,qword ptr ss:[rsp+0x20]                       |
			00007FFA820122E | mov rcx,rax                                           |
			00007FFA820122E | call client.7FFA8334FA90                              |
			00007FFA820122E | mov rcx,qword ptr ds:[0x7FFA84492FA0]                 | ppGameEntitySystem
			00007FFA820122F | mov edx,eax                                           |
			00007FFA820122F | call client.7FFA82A05750                              | GetBaseEntity
			*/

			auto ppGameEntitySystem = reinterpret_cast<uintptr_t>( FindPattern( CLIENT_DLL , XorStr( "48 8B 0D ? ? ? ? 8B D0 E8 ? ? ? ? 44 8B 87" ) ) );

			if ( !ppGameEntitySystem )
				return nullptr;

GetGameEntitySystemPointer:;

			g_pGameEntitySystem = *GetPtrAddress<CGameEntitySystem**>( ppGameEntitySystem );

			if ( !g_pGameEntitySystem )
			{
				Sleep( 500 );
				goto GetGameEntitySystemPointer;
			}
		}

		return g_pGameEntitySystem;
	}

	auto Interfaces::InputSystem() -> CInputSystem*
	{
		if ( !g_pInputSystem )
		{
			CreateInterfaceFn pfnFactory = CaptureFactory( INPUTSYSTEM_DLL );
			g_pInputSystem = CaptureInterface<CInputSystem>( pfnFactory , XorStr( INPUT_SYSTEM_INTERFACE_VERSION ) );
		}

		return g_pInputSystem;
	}

	auto Interfaces::SoundOpSystem() -> CSoundOpSystem*
	{
		if ( !g_pSoundOpSystem )
		{
			CreateInterfaceFn pfnFactory = CaptureFactory( SOUNDSYSTEM_DLL );
			g_pSoundOpSystem = CaptureInterface<CSoundOpSystem>( pfnFactory , XorStr( INTERFACE_SOUNDOPSYSTEM ) );
		}

		return g_pSoundOpSystem;
	}

	/*
	00007FFA8708712 | mov rcx,qword ptr ds:[0x7FFA883A40A0]                 | CUserCmd**
	00007FFA8708712 | call client.7FFA86EDA670                              | GetCUserCmdArray
	00007FFA8708713 | mov rcx,r12                                           |
	00007FFA8708713 | mov r13,rax                                           |
	00007FFA8708713 | mov r15d,dword ptr ds:[rax+0x6318]                    | offset SequenceNumber
	00007FFA8708714 | mov edx,r15d                                          |
	00007FFA8708714 | call client.7FFA86EDA400                              | GetUserCmdBySequenceNumber
	*/
	// 48 8B ? ? ? ? ? E8 ? ? ? ? 48 8B CF 4C 8B E8 44 8B B8 ? ? ? ? 41 8B D7 E8
	auto Pointers::GetFirstCUserCmdArray() -> CUserCmd**
	{
		if ( !g_ppCUserCmd )
		{
			auto ppCUserCmd = reinterpret_cast<uintptr_t>( FindPattern( CLIENT_DLL , XorStr( "48 8B 0D ? ? ? ? E8 ? ? ? ? 49 8B CC" ) ) );

			if ( !ppCUserCmd )
				return nullptr;

			g_ppCUserCmd = *GetPtrAddress<CUserCmd***>( ppCUserCmd );
		}

		return g_ppCUserCmd;
	}
}
