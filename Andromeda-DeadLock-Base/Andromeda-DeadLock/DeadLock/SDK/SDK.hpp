#pragma once

#include <Common/Common.hpp>

class CSchemaSystem;
class IVEngineToClient;
class CGameEntitySystem;
class CInputSystem;
class CUserCmd;
class CSoundOpSystem;
class CVPhys2World;
class CGlobalVarsBase;

#define CLIENT_DLL				"client.dll"
#define ENGINE2_DLL				"engine2.dll"
#define NAVSYSTEM_DLL			"navsystem.dll"
#define GAMEOVERLAYRENDER64_DLL "gameoverlayrenderer64.dll"
#define SCHEMASYSTEM_DLL		"schemasystem.dll"
#define INPUTSYSTEM_DLL			"inputsystem.dll"
#define SOUNDSYSTEM_DLL			"soundsystem.dll"
#define TIER0_DLL				"tier0.dll"

namespace SDK
{
	class Interfaces
	{
	public:
		static auto SchemaSystem() -> CSchemaSystem*;
		static auto EngineToClient() -> IVEngineToClient*;
		static auto GameEntitySystem() -> CGameEntitySystem*;
		static auto InputSystem() -> CInputSystem*;
		static auto SoundOpSystem() -> CSoundOpSystem*;

	private:
		static CSchemaSystem* g_pSchemaSystem;
		static IVEngineToClient* g_pEngineToClient;
		static CGameEntitySystem* g_pGameEntitySystem;
		static CInputSystem* g_pInputSystem;
		static CSoundOpSystem* g_pSoundOpSystem;
	};

	class Pointers
	{
	public:
		static auto GetFirstCUserCmdArray() -> CUserCmd**;
		static auto GetCVPhys2World() -> CVPhys2World**;
		static auto GlobalVarsBase() -> CGlobalVarsBase*;

	private:
		static CUserCmd** g_ppCUserCmd;
		static CVPhys2World** g_ppCVPhys2World;
		static CGlobalVarsBase** g_ppCGlobalVarsBase;
	};
}
