#include "CSDK_Loader.hpp"

#include <DeadLock/SDK/SDK.hpp>
#include <DeadLock/SDK/CSchemaOffset.hpp>
#include <DeadLock/SDK/SDL3/SDL3_Functions.hpp>

#include <Common/Helpers/ModuleLoaderHelper.hpp>

#define RETURN_FALSE_IF_INTERFACE_ERROR(Interface)\
if ( !Interface )\
{\
	DEV_LOG( "[error] Interface::%s\n" , #Interface );\
	return false;\
}

#define RETURN_FALSE_IF_POINTER_ERROR(Pointer)\
if ( !Pointer )\
{\
	DEV_LOG( "[error] Pointer::%s\n" , #Pointer );\
	return false;\
}

static CSDK_Loader g_SDK_Loader{};

auto CSDK_Loader::LoadSDK() -> bool
{
	// Wait for module load:
	if ( !IsModuleLoad( NAVSYSTEM_DLL ) )
		return false;

	// Log SDK:
#if LOG_SDK == 1
	DEV_LOG( "\n[+] CSDK_Loader::LoadSDK:\n\n" );
#endif

	// Interfaces:
	auto pSchemaSystem = SDK::Interfaces::SchemaSystem();
	auto pEngineToClient = SDK::Interfaces::EngineToClient();
	auto pGameEntitySystem = SDK::Interfaces::GameEntitySystem();
	auto pInputSystem = SDK::Interfaces::InputSystem();
	auto pSoundOpSystem = SDK::Interfaces::SoundOpSystem();

	// Pointers:
	auto pCUserCmdArray = SDK::Pointers::GetFirstCUserCmdArray();
	
	// Return false if interface error
	RETURN_FALSE_IF_INTERFACE_ERROR( pSchemaSystem );
	RETURN_FALSE_IF_INTERFACE_ERROR( pEngineToClient );
	RETURN_FALSE_IF_INTERFACE_ERROR( pGameEntitySystem );
	RETURN_FALSE_IF_INTERFACE_ERROR( pInputSystem );
	RETURN_FALSE_IF_INTERFACE_ERROR( pSoundOpSystem );

	// Return false if pointer error
	RETURN_FALSE_IF_POINTER_ERROR( pCUserCmdArray );
	
	// Log:
#if LOG_SDK == 1
	DEV_LOG( "\n" );
#endif

	// Log:
#if LOG_SDK == 1

	// Interfaces:
	DEV_LOG( "[+] pSchemaSystem: %p\n" , pSchemaSystem );
	DEV_LOG( "[+] pEngineToClient: %p\n" , pEngineToClient );
	DEV_LOG( "[+] pGameEntitySystem: %p\n" , pGameEntitySystem );
	DEV_LOG( "[+] pGameEntitySystem: %p\n" , pInputSystem );
	DEV_LOG( "[+] pSoundOpSystem: %p\n" , pSoundOpSystem );

	// Pointers:
	DEV_LOG( "[+] pCUserCmdArray: %p\n" , pCUserCmdArray );

#endif

#if LOG_SDK == 1
	DEV_LOG( "\n" );
#endif

	if ( !GetSDL3Functions()->OnInit() )
		return false;

	GetSchemaOffset()->Init();

	return true;
}

auto GetSDK_Loader() -> CSDK_Loader*
{
	return &g_SDK_Loader;
}
