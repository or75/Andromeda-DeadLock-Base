#pragma once

/*
00007FFA8708713 | mov r15d,dword ptr ds:[rax+0x6318]                    | m_nSequenceNumber
00007FFA8708714 | mov edx,r15d                                          |
00007FFA8708714 | call client.7FFA86EDA400                              |
00007FFA8708714 | mov rcx,qword ptr ds:[0x7FFA883CC800]                 |
00007FFA8708714 | mov r8,rax                                            |
00007FFA8708715 | mov qword ptr ss:[rbp-0x69],rax                       |
00007FFA8708715 | mov dword ptr ss:[rbp+0x7F],esi                       |
00007FFA8708715 | mov r14d,dword ptr ds:[rcx+0x44]                      |
00007FFA8708715 | test rax,rax                                          |
*/
// client.dll -> 44 8B A8 ? ? ? ? 41 8B D5
static constexpr auto g_OFFSET_CUserCmdArray_m_nSequenceNumber = 0x6318;

static constexpr auto g_OFFSET_CDemoRecorder_ParseMessage_pProtobuf = 0x30;
static constexpr auto g_OFFSET_CMsgSosStartSoundEvent_SoundPos = 0x12;

// client.dll -> FF 89 ? ? ? ? EB ? 48 85 F6
static constexpr auto g_OFFSET_CGameEntitySystem_GetHighestEntityIndex = 0x20F0;
