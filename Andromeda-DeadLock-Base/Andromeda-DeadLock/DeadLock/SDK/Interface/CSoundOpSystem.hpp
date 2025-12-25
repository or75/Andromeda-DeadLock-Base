#pragma once

#include <Common/Common.hpp>
#include <Common/MemoryEngine.hpp>

#define INTERFACE_SOUNDOPSYSTEM "SoundOpSystem001"

class CSoundEventManager
{
public:
	virtual uint32_t GetSoundEventHash( const char* szSound ) = 0;
	virtual bool IsValidSoundEventHash( uint32_t hash ) = 0;
	virtual const char* GetSoundEventName( uint32_t hash ) = 0;
};

class CSoundOpSystem
{
public:
	inline auto GetCSoundEventManager() -> CSoundEventManager*
	{
		return CUSTOM_OFFSET_RAW( CSoundEventManager , 0x8 );
	}
};
