#pragma once

#include <Common/Common.hpp>
#include <Common/MemoryEngine.hpp>

#include "Offsets.hpp"

#include <DeadLock/Protobuf/citadel_usercmd.pb.h>

enum InputBitMask_t : uint64_t
{
	IN_NONE = 0x0000000000000000 ,
	IN_ALL = 0xFFFFFFFFFFFFFFFF ,
	IN_ATTACK = 0x0000000000000001 ,
	IN_JUMP = 0x0000000000000002 ,
	IN_DUCK = 0x0000000000000004 ,
	IN_FORWARD = 0x0000000000000008 ,
	IN_BACK = 0x0000000000000010 ,
	IN_USE = 0x0000000000000020 ,
	IN_TURNLEFT = 0x0000000000000080 ,
	IN_TURNRIGHT = 0x0000000000000100 ,
	IN_MOVELEFT = 0x0000000000000200 ,
	IN_MOVERIGHT = 0x0000000000000400 ,
	IN_ATTACK2 = 0x0000000000000800 ,
	IN_RELOAD = 0x0000000000002000 ,
	IN_SPEED = 0x0000000000010000 ,
	IN_JOYAUTOSPRINT = 0x0000000000020000 ,
	IN_FIRST_MOD_SPECIFIC_BIT = 0x0000000100000000 ,
	IN_WEAPON1 = 0x0000000100000000 ,
	IN_ABILITY1 = 0x0000000200000000 ,
	IN_ABILITY2 = 0x0000000400000000 ,
	IN_ABILITY3 = 0x0000000800000000 ,
	IN_ABILITY4 = 0x0000001000000000 ,
	IN_ITEM1 = 0x0000002000000000 ,
	IN_ITEM2 = 0x0000004000000000 ,
	IN_ITEM3 = 0x0000008000000000 ,
	IN_ITEM4 = 0x0000010000000000 ,
	IN_ITEM5 = 0x0000020000000000 ,
	IN_ABILITY_HELD = 0x0000040000000000 ,
	IN_INNATE_1 = 0x0000100000000000 ,
	IN_INNATE_2 = 0x0000200000000000 ,
	IN_INNATE_3 = 0x0000400000000000 ,
	IN_MANTLE = 0x0001000000000000 ,
	IN_SPEC_NEXT = 0x0002000000000000 ,
	IN_SPEC_PREV = 0x0004000000000000 ,
	IN_SPEC_MODE = 0x0008000000000000 ,
	IN_SPEC_TOGGLE_TEAM = 0x0010000000000000 ,
	IN_ALT_CAST = 0x0020000000000000 ,
	IN_REPLAY_DEATH = 0x0040000000000000 ,
	IN_TELEPORT = 0x0080000000000000 ,
	IN_CANCEL_ABILITY = 0x0100000000000000 ,
	IN_ZIPLINE = 0x0200000000000000 ,
	IN_MOVE_UP = 0x0400000000000000 ,
	IN_MOVE_DOWN = 0x0800000000000000 ,
};

class CInButtonState
{
	void* vtable;
public:
	uint64_t buttonstate1;
	uint64_t buttonstate2;
	uint64_t buttonstate3;
};

class CUserCmdArray
{
public:
	CUSTOM_OFFSET_FIELD( uint32_t , m_nSequenceNumber , g_OFFSET_CUserCmdArray_m_nSequenceNumber );
};

#pragma pack(push, 1)
class CUserCmd
{
private:
	PAD( 0x10 );
public:
	CCitadelUserCmdPB cmd;
	CInButtonState button_states;
private:
	PAD( 0x18 );
};
#pragma pack(pop)

namespace google
{
	namespace protobuf
	{
#undef max

		template <typename T>
		struct RepeatedPtrField_t
		{
			struct Rep_t
			{
				int m_nAllocatedSize;
				T* m_tElements[( std::numeric_limits<int>::max() - 2 * sizeof( int ) ) / sizeof( void* )];
			};

			void* m_pArena;
			int m_nCurrentSize;
			int m_nTotalSize;
			Rep_t* m_pRep;
		};
	}
}
