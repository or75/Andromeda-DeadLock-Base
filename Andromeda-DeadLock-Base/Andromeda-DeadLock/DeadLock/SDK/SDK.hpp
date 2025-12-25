#pragma once

#include <Common/Common.hpp>

class CSchemaSystem;
class IVEngineToClient;
class CGameEntitySystem;
class CInputSystem;
class CUserCmd;
class CSoundOpSystem;

#define CLIENT_DLL				"client.dll"
#define ENGINE2_DLL				"engine2.dll"
#define NAVSYSTEM_DLL			"navsystem.dll"
#define GAMEOVERLAYRENDER64_DLL "gameoverlayrenderer64.dll"
#define SCHEMASYSTEM_DLL		"schemasystem.dll"
#define INPUTSYSTEM_DLL			"inputsystem.dll"
#define SOUNDSYSTEM_DLL			"soundsystem.dll"

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

	private:
		static CUserCmd** g_ppCUserCmd;
	};
}
