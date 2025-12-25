#include "Hook_ParseMessage.hpp"

#include <DeadLock/SDK/SDK.hpp>
#include <DeadLock/SDK/Math/Vector3.hpp>
#include <DeadLock/SDK/Update/Offsets.hpp>

#include <DeadLock/SDK/Network/CNetworkMessages.hpp>
#include <DeadLock/SDK/Interface/CSoundOpSystem.hpp>

#include <DeadLock/Protobuf/gameevents.pb.h>

#include <AndromedaClient/CAndromedaClient.hpp>

auto Hook_ParseMessage( CDemoRecorder* pDemoRecorder , CNetworkSerializerPB* pSerializer , CNetMessagePB* pNetMessage ) -> bool
{
	if ( pSerializer->messageID == GE_SosStartSoundEvent )
	{
		CMsgSosStartSoundEvent* pMessage = reinterpret_cast<CMsgSosStartSoundEvent*>( (PBYTE)pNetMessage + g_OFFSET_CDemoRecorder_ParseMessage_pProtobuf );

		if ( pMessage )
		{
			std::string SoundName = "null";
			Vector3 SoundPos;
			auto SourceEntityIndex = 0;

			if ( pMessage->has_source_entity_index() )
				SourceEntityIndex = pMessage->source_entity_index();

			if ( pMessage->has_packed_params() && pMessage->packed_params().data() )
			{
				SoundPos = *(Vector3*)( pMessage->packed_params().data() + g_OFFSET_CMsgSosStartSoundEvent_SoundPos );

				const char* szSoundEventName = SDK::Interfaces::SoundOpSystem()->GetCSoundEventManager()->GetSoundEventName( pMessage->soundevent_hash() );

				if ( szSoundEventName )
					SoundName = szSoundEventName;

				GetAndromedaClient()->OnStartSound( SoundPos , SourceEntityIndex , SoundName.c_str() );
			}
		}
	}

	return ParseMessage_o( pDemoRecorder , pSerializer , pNetMessage );
}
