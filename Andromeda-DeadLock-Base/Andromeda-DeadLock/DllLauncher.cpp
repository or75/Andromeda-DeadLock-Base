#include "DllLauncher.hpp"

#include <string>
#include <winternl.h>

#include <Common/CrashLog.hpp>
#include <Common/Helpers/StringHelper.hpp>

#include <DeadLock/CSDK_Loader.hpp>
#include <DeadLock/CHook_Loader.hpp>
#include <DeadLock/SDK/CFunctionList.hpp>

#include <AndromedaClient/CAndromedaGUI.hpp>
#include <AndromedaClient/CAndromedaClient.hpp>

#include <AndromedaClient/Settings/CSettingsJson.hpp>

static CDllLauncher g_CDllLauncher{};

auto CDllLauncher::OnDllMain( LPVOID lpReserved , HINSTANCE hInstace ) -> void
{
#if ENABLE_MANUAL_MAP == 0 || DEVELOPER_BUILD
	char szDllDir[MAX_PATH];

	GetModuleFileNameA( hInstace , szDllDir , MAX_PATH );
	
	m_DllDir = szDllDir;
	m_DllDir = m_DllDir.substr( 0 , m_DllDir.find_last_of( '\\' ) );
	m_DllDir += '\\';
#else
	m_DllDir = "D:\\";
#endif

#if ENABLE_MANUAL_MAP == 1
	if ( lpReserved )
	{
		ManualMapParam_t* pParam = reinterpret_cast<ManualMapParam_t*>( lpReserved );

		if ( pParam )
		{
			m_DllDir = pParam->m_dllDir;
			m_DllDir = m_DllDir.substr( 0, m_DllDir.find_last_of( '\\' ) + 1 );
		}
			
	}
#endif

	m_hDllImage = hInstace;

	m_SizeofImage = GetSizeOfImageInternal();
	m_BaseOfCode = GetBaseOfCodeInternal();

	char szGameFile[MAX_PATH] = { 0 };
	GetModuleFileNameA( 0 , szGameFile , MAX_PATH );

	m_CSGODir = szGameFile;
	m_CSGODir = m_CSGODir.substr( 0 , m_CSGODir.find_last_of( "\\/" ) );
	m_CSGODir += '\\';

	memset( szGameFile , 0 , MAX_PATH );

	CreateThread( 0 , 0 , StartCheatTheard , lpReserved , 0 , 0 );
}

auto CDllLauncher::OnDestroy() -> void
{
	if ( !m_bDestroyed )
	{
		GetDevLog()->Destroy();
		GetHook_Loader()->DestroyHooks();
		GetAndromedaGUI()->OnDestroy();
		GetCrashLog()->DestroyVectorExceptionHandler();
		
		m_bDestroyed = true;
	}
}

auto WINAPI CDllLauncher::StartCheatTheard( LPVOID lpThreadParameter ) -> DWORD
{
	GetDevLog()->Init();
	GetCrashLog()->InitVectorExceptionHandler();

	DEV_LOG( "[+] StartCheatThread: %s\n" , ansi_to_utf8( GetDllDir() ).c_str() );

	if ( !GetHook_Loader()->InitalizeMH() )
	{
		DEV_LOG( "[error] Hook_Loader::InitalizeMH\n" );
		return 0;
	}

	if ( !GetFunctionList()->OnInit() )
	{
		DEV_LOG( "[error] FunctionList::OnInit\n" );
		return 0;
	}

	if ( !GetSDK_Loader()->LoadSDK() )
	{
		DEV_LOG( "[error] CSDK_Loader::LoadSDK\n" );
		return 0;
	}

	if ( !GetHook_Loader()->InstallSecondHook() )
	{
		DEV_LOG( "[error] Hook_Loader::InstallSecondHook\n" );
		return 0;
	}

	GetSettingsJson()->UpdateConfigList();
	GetSettingsJson()->LoadConfig( CONFIG_FILE );

	while ( !GetAndromedaGUI()->IsInited() )
		Sleep( 100 );

	GetAndromedaClient()->OnInit();

	return 0;
}

auto GetDllDir()->std::string&
{
	return GetDllLauncher()->m_DllDir;
}

auto GetCSGODir() -> std::string
{
	return GetDllLauncher()->m_CSGODir;
}

auto GetDllLauncher() -> CDllLauncher*
{
	return &g_CDllLauncher;
}
