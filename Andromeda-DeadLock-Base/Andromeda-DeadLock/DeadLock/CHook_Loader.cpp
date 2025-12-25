#include "CHook_Loader.hpp"

#include <Common/MemoryEngine.hpp>
#include <MinHook/MinHook.h>

#include <DeadLock/SDK/SDK.hpp>

#include <DeadLock/Hook/Hook_Present.hpp>
#include <DeadLock/Hook/Hook_ResizeBuffers.hpp>
#include <DeadLock/Hook/Hook_CreateSwapChain.hpp>
#include <DeadLock/Hook/Hook_FireEventClientSide.hpp>
#include <DeadLock/Hook/Hook_MouseInputEnabled.hpp>
#include <DeadLock/Hook/Hook_IsRelativeMouseMode.hpp>
#include <DeadLock/Hook/Hook_OnAddEntity.hpp>
#include <DeadLock/Hook/Hook_OnRemoveEntity.hpp>
#include <DeadLock/Hook/Hook_CreateMove.hpp>
#include <DeadLock/Hook/Hook_ParseMessage.hpp>
#include <DeadLock/Hook/Hook_OnClientOutput.hpp>
#include <DeadLock/Hook/Hook_GetMatricesForView.hpp>

static CHook_Loader g_CHook_Loader{};

auto CHook_Loader::InitalizeMH() -> bool
{
	return MH_Initialize() == MH_OK;
}

auto CHook_Loader::InstallSecondHook() -> bool
{
	m_Hooks =
	{
		/*
		sub_180095150 -> PresentOverlay
		sub_18008ED80(*(_QWORD *)(v4 + 64), sub_180095150, &qword_180162258, 1, "DXGISwapChain_Present");
		*/
		{ { XorStr( "Hook::PresentOverlay" ) , XorStr( "48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 54 41 56 41 57 48 83 EC ? 41 8B E8" ) , GAMEOVERLAYRENDER64_DLL } , &Hook_Present , reinterpret_cast<LPVOID*>( &Present_o ) },
		/*
		sub_180095520 -> ResizeBuffers
		sub_18008ED80(*(_QWORD *)(v4 + 104), sub_180095520, &qword_180162260, 1, "DXGISwapChain_ResizeBuffers");
		*/
		{ { XorStr( "Hook::ResizeBuffers" ) , XorStr( "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 54 41 56 41 57 48 83 EC ? 44 8B E2" ) , GAMEOVERLAYRENDER64_DLL } , &Hook_ResizeBuffers , reinterpret_cast<LPVOID*>( &ResizeBuffers_o ) },
		/*
		sub_1800AA7B0("IWrapDXGIFactory::CreateSwapChain called\n");
		*/
		{ { XorStr( "Hook::CreateSwapChain" ) , XorStr( "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 48 83 EC 20 48 8B F9 49 8B F1 48 8D ? ? ? ? ? 49 8B D8 48 8B EA E8 ? ? ? ? 48 8D ? ? ? ? ? E8 ? ? ? ? 48 8D ? ? ? ? ? E8 ? ? ? ? 48 8D ? ? ? ? ? E8 ? ? ? ? 48 8B ? ? ? ? ? 4C 8B CE 4C 8B C3 48 8B D5 48 8B CF FF D0 8B D8 85 C0 78 18 48 85 F6 74 13 48 83 3E 00 74 0D 48 8B D5 48 8B CE E8 ? ? ? ? 8B C3 48 8B 5C 24 30 48 8B 6C 24 38 48 8B 74 24 40 48 83 C4 20 5F C3 CC CC CC CC CC CC CC CC 48 83 EC 38 48 8B 01 4C 8D 44 24 40" ) , GAMEOVERLAYRENDER64_DLL } , &Hook_CreateSwapChain , reinterpret_cast<LPVOID*>( &CreateSwapChain_o ) } ,
		{ { XorStr( "Hook::FireEventClientSide" ) , XorStr( "48 89 5C 24 ? 57 41 54 41 56 48 83 EC ? 4C 8B F2" ) , CLIENT_DLL } , &Hook_FireEventClientSide , reinterpret_cast<LPVOID*>( &FireEventClientSide_o ) },
		{ { XorStr( "Hook::MouseInputEnabled" ) , XorStr( "48 83 EC 28 E8 ? ? ? ? 84 C0 0F 85" ) , CLIENT_DLL } , &Hook_MouseInputEnabled , reinterpret_cast<LPVOID*>( &MouseInputEnabled_o ) },
		{ { XorStr( "Hook::IsRelativeMouseMode" ) , XorStr( "48 89 6C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 41 56 48 83 EC ? 0F B6 F2" ) , INPUTSYSTEM_DLL } , &Hook_IsRelativeMouseMode , reinterpret_cast<LPVOID*>( &IsRelativeMouseMode_o ) },
		{ { XorStr( "Hook::OnAddEntity" ) , XorStr( "48 89 74 24 ? 57 48 83 EC ? 41 B9 ? ? ? ? 41 8B C0 41 23 C1 48 8B F2 41 83 F8 ? 48 8B F9 44 0F 45 C8 41 81 F9 ? ? ? ? 73 ? FF 81" ) , CLIENT_DLL } , &Hook_OnAddEntity , reinterpret_cast<LPVOID*>( &OnAddEntity_o ) },
		{ { XorStr( "Hook::OnRemoveEntity" ) , XorStr( "48 89 74 24 ? 57 48 83 EC ? 41 B9 ? ? ? ? 41 8B C0 41 23 C1 48 8B F2 41 83 F8 ? 48 8B F9 44 0F 45 C8 41 81 F9 ? ? ? ? 73 ? FF 89" ) , CLIENT_DLL } , &Hook_OnRemoveEntity , reinterpret_cast<LPVOID*>( &OnRemoveEntity_o ) },
		{ { XorStr( "Hook::CreateMove" ) , XorStr( "85 D2 0F 85 ? ? ? ? 48 8B C4 44 88 40 18" ) , CLIENT_DLL } , &Hook_CreateMove , reinterpret_cast<LPVOID*>( &CreateMove_o ) },
		{ { XorStr( "Hook::ParseMessage" ) , XorStr( "40 56 57 41 57 48 83 EC ? 4C 8B F9" ) , ENGINE2_DLL } , &Hook_ParseMessage , reinterpret_cast<LPVOID*>( &ParseMessage_o ) },
		{ { XorStr( "Hook::OnClientOutput" ) , XorStr( "48 89 5C 24 ? 55 56 57 41 54 41 56 48 83 EC ? 48 8D 05" ) , ENGINE2_DLL } , &Hook_OnClientOutput , reinterpret_cast<LPVOID*>( &OnClientOutput_o ) },
		{ { XorStr( "Hook::GetMatricesForView" ) , XorStr( "40 53 48 81 EC ? ? ? ? 49 8B C1" ) , CLIENT_DLL } , &Hook_GetMatricesForView , reinterpret_cast<LPVOID*>( &GetMatricesForView_o ) },
	};

	return InstallHooks();
}

auto CHook_Loader::InstallHooks() -> bool
{
	for ( auto& Hook : m_Hooks )
	{
		while ( !GetModuleHandleA( Hook.m_Pattern.GetDllName() ) )
			   Sleep( 1 );

		if ( !Hook.m_Pattern.Search( Hook.m_bSkipError ) )
		{
			if ( !Hook.m_bSkipError )
				DEV_LOG( "[error] Hook #1 -> '%s'\n" , Hook.m_Pattern.GetPatternName() );

			if ( !Hook.m_bSkipIfNotFound )
				return false;
			else
				continue;
		}

		auto Status = MH_CreateHook( Hook.m_Pattern.GetFunction() , Hook.m_pDetour , Hook.m_pOriginal );

		if ( Status != MH_OK )
		{
			DEV_LOG( "[error] Hook #2 [%i] -> '%s'\n" , Status , Hook.m_Pattern.GetPatternName() );
			return false;
		}
	}

	MH_EnableHook( MH_ALL_HOOKS );

	m_Hooks.clear();

	return true;
}

auto CHook_Loader::DestroyHooks() -> void
{
	MH_DisableHook( MH_ALL_HOOKS );
	MH_Uninitialize();
}

auto GetHook_Loader() -> CHook_Loader*
{
	return &g_CHook_Loader;
}
