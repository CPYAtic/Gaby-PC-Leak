#include <Windows.h>
#include "SDK.h"
/*////////////////////////////////////////////////PARAMS////////////////////////////////////////////////*/
typedef int8_t(__thiscall* tServerInfo)(uint32_t);
typedef int8_t(__thiscall* tChannelInfo)(uint32_t);
typedef int8_t(__thiscall* tGrabInfo)(uint32_t, LPCSTR);
/*////////////////////////////////////////////////PARAMS////////////////////////////////////////////////*/

/*/////////////////////////////////////////Functions - Address/////////////////////////////////////////*/
tServerInfo ServerInfo = reinterpret_cast<tServerInfo>(0x008A85A0); //ServerInfo
tChannelInfo ChannelInfo = reinterpret_cast<tChannelInfo>(0x0088ADE0); //ChannelInfo
tGrabInfo GrabInfo = reinterpret_cast<tGrabInfo>(0x008B1E00); //GrabInfo from database


