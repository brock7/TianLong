//-----------------------------------------------------------------------------
// �ļ��� : LuaFnTbl_Guild.h
// ģ��	:	Script
// ����	 :  ����ű��ӿ�
// �޸���ʷ:
//-----------------------------------------------------------------------------
#ifndef __LUAFNTBL_GUILD_H__
#define __LUAFNTBL_GUILD_H__

#include "LuaInterface.h"
#include "LuaFnMacro.h"
#include "GCGuildApply.h"
#include "Obj_Human.h"
#include "Obj_Character.h"
#include "ServerManager.h"
#include "PacketFactoryManager.h"
#include "GWGuild.h"
namespace LuaFnTbl
{
	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-3-21
	*     �������� : LuaFnGuildList
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					Npc���
	*				
	*/
	INT	LuaFnGuildList(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		ObjID_t targetId		=	Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnGuildList")
			GWGuild* pMsg = (GWGuild*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_GUILD);
			pMsg->SetGUID( pHuman->GetGUID() );
			_GUILD_CGW_PACKET* pGuildPacket = pMsg->GetGuildPacket();
			pGuildPacket->m_uPacketType = GUILD_PACKET_GW_ASKLIST;

			GUILD_CGW_ASKLIST* pPacketAskList = (GUILD_CGW_ASKLIST*)(pGuildPacket->GetPacket(GUILD_PACKET_GW_ASKLIST));

			pPacketAskList->m_uStart = 0;
			pPacketAskList->m_Camp = pHuman->GetCampData()->m_nCampID;

			g_pServerManager->SendPacket( pMsg, INVALID_ID );
		ENDHUMANDEFINE
		return	0;

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-3-21
	*     �������� : LuaFnGuildCreate
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					Npc���
	*				
	*/
	INT	LuaFnGuildCreate(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		ObjID_t targetId		=	Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnGuildCreate")
			GCGuildApply Msg;
			Msg.SetNpcId(targetId);
			pHuman->GetPlayer()->SendPacket( &Msg );
		ENDHUMANDEFINE
			return	0;

		LUA_LEAVE_FUNCTION
			return 0 ;
	}


}
#endif