//-----------------------------------------------------------------------------
// �ļ��� : LuaFnTbl_Team.h
// ģ��	:	Script
// ����	 :	�����ص�������
// �޸���ʷ:
//-----------------------------------------------------------------------------
#ifndef __LUAFNTBL_TEAM_H__
#define __LUAFNTBL_TEAM_H__

#include "LuaInterface.h"
#include "LuaFnMacro.h"
#include "SceneManager.h"
#include "HumanItemLogic.h"
#include "MonsterManager.h"
#include "Obj_Monster.h"
#include "Config.h"
#include "ScanOperator_ActiveTeammates.h"
#include "GCChat.h"


namespace LuaFnTbl
{
	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetTeamId
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������			
	*     ����˵�� :
	*				������Ҷ���ID
	*				û�в���
	*/
	INT LuaFnGetTeamId(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
	
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetTeamId")
		Lua_PushNumber(L,pHuman->__GetTeamID());
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE
		
	    LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetTeamSize
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				����������ڶ�������
	*				û�в���
	*/
	INT LuaFnGetTeamSize(Lua_State* L)	
	{	
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetTeamSize")
		Lua_PushNumber(L,pHuman->__GetTeamMemberCount());
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnLeaveTeam
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				������뿪�������
	*				û�в���
	*/
	INT LuaFnLeaveTeam(Lua_State* L)	
	{
	LUA_ENTER_FUNCTION

	SceneID_t sceneId = Lua_ValueToNumber(L,1);
	ObjID_t selfId = Lua_ValueToNumber(L,2);

	BEGINHUMANDEFINE("LuaFnLeaveTeam")
	pHuman->GetTeamInfo()->DelMember(pHuman->GetGUID());
	Lua_PushNumber(L,1);
	return 1;
	ENDHUMANDEFINE
	RETURNFALSE
	
	LUA_LEAVE_FUNCTION
	
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnMsg2Player
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				msg
	*					��������,�ַ���ָ������
	*				type
	*					CHAT_TYPE_NORMAL =0 ,	//��ͨ˵����Ϣ
	*					CHAT_TYPE_TEAM = 1,		//������Ϣ
	*					CHAT_TYPE_SCENE = 2,		//������Ϣ
	*					CHAT_TYPE_TELL = 3,		//˽����Ϣ
	*					CHAT_TYPE_SYSTEM = 4,		//ϵͳ��Ϣ
	*					CHAT_TYPE_CHANNEL = 5,		//�Խ�����Ƶ����Ϣ
	*					CHAT_TYPE_GUILD = 6,		//������Ϣ
	*					CHAT_TYPE_MENPAI = 7 ,		//������Ϣ
	*					CHAT_TYPE_SELF = 8,			//���ͻ���ʹ�õ���Ϣ
	*					
	*     ����˵�� :
	*				����Ϣ���͸����
	*				��Ϣ��ʾ�����½ǵ��������
	*				typeĬ��ֵΪCHAT_TYPE_NORMAL,��0
	*
	*				��Ǯ��ʾ��Ϣ����Ҫд��#{_MONEY%d}��ʽ
	*				���磺������#{_MONEY208934}�� 
	*				��仰�ڿͻ�����ʾ��ʱ����Զ������������ͭ�����������ö�Ӧ��ͼ���ʾ������
	*				
	*/
	INT LuaFnMsg2Player(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		CHAR*msg = const_cast<CHAR*>(Lua_ValueToString(L,3));
		ENUM_CHAT_TYPE type;
		
		if(Lua_GetTopIndex(L) < 4)
			type = CHAT_TYPE_NORMAL;
		else
			type = (ENUM_CHAT_TYPE)((INT)(Lua_ValueToNumber(L,4)));
		
		BEGINHUMANDEFINE("LuaFnMsg2Player")
		GCChat Msg ;
		INT Size = (INT)strlen(msg) ;
		Msg.SetChatType( type) ;
		Msg.SetContexSize( Size ) ;
		Msg.SetContex( msg ) ;
		Msg.SetSourNameSize( (BYTE)strlen(pHuman->GetName())) ;
		Msg.SetSourName( pHuman->GetName()) ;
		Msg.SetSourID(pHuman->GetID()) ;
		Msg.SetSourCamp(pHuman->GetCampData()->m_nCampID);

		pHuman->GetPlayer()->SendPacket( &Msg ) ;

		Lua_PushNumber(L,1);
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnMsg2Team
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				msg
	*					��������,�ַ���ָ������
	*     ����˵�� :
	*				����Ϣ���͸��������������г�Ա
	*				��������δ��д
	*/
	INT LuaFnMsg2Team(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		GUID_t selfId; selfId;
		const CHAR *msg; msg;
		LUA_LEAVE_FUNCTION
		return 0 ;
		
	}

	/****************************************************************************
	 *	������Ա��	
	 *	��дʱ�䣺	2006-1-17	17:53
	 *	�������ƣ�	LuaFnIsTeamFollow
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *
	 *	����˵����	�õ� selfId ��Ӧ������Ƿ�����Ӹ���״̬
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnIsTeamFollow(Lua_State* L)	
	{
	LUA_ENTER_FUNCTION
		
		SceneID_t sceneId = Lua_ValueToNumber(L, 1);
		ObjID_t selfId = Lua_ValueToNumber(L, 2);

		BEGINHUMANDEFINE("LuaFnIsTeamFollow")
		Lua_PushNumber(L, pHuman->__GetTeamFollowFlag());
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-12-14
	*     �������� : LuaFnGetFollowedMembersCount
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������			
	*     ����˵�� :
	*				�õ�����״̬�Ķ�������
	*				û�в���
	*/
	INT LuaFnGetFollowedMembersCount(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetFollowedMembersCount")
		Lua_PushNumber(L,pHuman->__GetFollowedMembersCount());
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE
		
	    LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     �������� : LuaFnGetFollowedMember
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				memberIndex
	*					�ڸ�������е��±�����,��0��ʼ,����			

	*     ����˵�� :
	*				�õ�����״̬��memberIndex�±�Ķ���objId
	*				û�в���
	*/
	INT LuaFnGetFollowedMember(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT memberIndex = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnGetFollowedMember")
			Obj_Human* pMember = pHuman->__GetFollowedMember(memberIndex)->m_pHuman;
			if( pMember!=NULL && pMember->GetGUID()==pHuman->__GetFollowedMember(memberIndex)->m_GUID )
			{
				Lua_PushNumber(L, (INT)pMember->GetID());
				return 1 ;
			}
		ENDHUMANDEFINE
		RETURNFALSE
		
	    LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-12-14
	*     �������� : LuaFnStopTeamFollow
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������			
	*     ����˵�� :
	*				ֹͣ����,���selfId��־�����
	*				û�в���
	*/
	INT LuaFnStopTeamFollow(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnStopTeamFollow")
		pHuman->__StopTeamFollow();
		return 0;
		ENDHUMANDEFINE
		RETURNFALSE
		
	    LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-12-14
	*     �������� : LuaFnClearFollowedMembers
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������			
	*     ����˵�� :
	*				������еĸ�������б�
	*				û�в���
	*/
	INT LuaFnClearFollowedMembers(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnClearFollowedMembers")
		
		pHuman->__ClearFollowedMembers();
		return 0;

		ENDHUMANDEFINE
		RETURNFALSE
		
	    LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-12-14
	*     �������� : LuaFnGetTeamSceneMemberCount
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������			
	*     ����˵�� :
	*				������ͬ������Ա���� human
	*				û�в���
	*/
	INT LuaFnGetTeamSceneMemberCount(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetTeamSceneMemberCount")
		
		Lua_PushNumber(L,pHuman->__GetTeamSceneMemberCount());
		return 1;

		ENDHUMANDEFINE
		RETURNFALSE
		
	    LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	
	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-12-14
	*     �������� : LuaFnGetTeamSceneMember
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������	
	*				memberIndex
	*					�ڶ����е��±�����,��0��ʼ,����			
	*     ����˵�� :
	*				ȡ�ö�����ĳͬ������Ա�� OBJID
	*				û�в���
	*/
	INT LuaFnGetTeamSceneMember(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);	
		INT memberIndex = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnGetTeamSceneMember")
		
		Lua_PushNumber(L,pHuman->__GetTeamSceneMember(memberIndex));
		return 1;

		ENDHUMANDEFINE
		RETURNFALSE
		
	    LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-12-14
	*     �������� : LuaFnSetTeamFollowSpeed
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������			
	*     ����˵�� :
	*				������Ӹ�����ٶ�
	*				û�в���
	*/
	INT LuaFnSetTeamFollowSpeed(Lua_State* L)
	{

		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		FLOAT fSpeed = Lua_ValueToNumber(L,3);
	
		BEGINHUMANDEFINE("LuaFnSetTeamFollowSpeed")
		
		pHuman->__SetTeamFollowSpeed(fSpeed);
		return 1;

		ENDHUMANDEFINE
		RETURNFALSE
		
	    LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-12-14
	*     �������� : LuaFnIsTeamLeader
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������			
	*     ����˵�� :
	*				selfId����Ƿ�Ϊ�ӳ�
	*				����1,��
	*				����0,��
	*				û�в���
	*/
	INT LuaFnIsTeamLeader(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);	

		BEGINHUMANDEFINE("LuaFnIsTeamLeader")
		
		Lua_PushNumber(L,pHuman->__IsTeamLeader());
		return 1;

		ENDHUMANDEFINE
		RETURNFALSE
		
	    LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-12-14
	*     �������� : LuaFnGetTeamMemberGuid
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������	
	*				memberIndex
	*					�ڶ����е��±�����,��0��ʼ,����			
	*     ����˵�� :
	*				���selfId����Team�е�memberIndex���±���ҵ�Guid
	*				û�в���
	*/
	INT LuaFnGetTeamMemberGuid(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);	
		INT memberIndex = Lua_ValueToNumber(L,3);	

		BEGINHUMANDEFINE("LuaFnGetTeamMemberGuid")
		
		Lua_PushNumber(L,pHuman->__GetTeamMember(memberIndex)->m_GUID);
		return 1;

		ENDHUMANDEFINE
		RETURNFALSE
		
	    LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-1-10
	*     �������� : LuaFnHasTeam
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������	
	*     ����˵�� :
	*				�ж�selfId����Ƿ������״̬
	*				û�в���
	*/
	INT LuaFnHasTeam(Lua_State* L)
	{	
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);	

		BEGINHUMANDEFINE("LuaFnHasTeam")
		
		Lua_PushNumber(L,pHuman->GetTeamInfo()->HasTeam());
		return 1;

		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION

		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-3-17
	*     �������� : LuaFnGetNearTeamCount
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������	
	*     ����˵�� :
	*				ȡ��selfId��Ҹ����Ķ�������(�����Լ�)
	*				�����Ҳ�����ĳ�����飬�򷵻�-1
	*/
	INT LuaFnGetNearTeamCount(Lua_State* L)
	{	
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);	

		BEGINHUMANDEFINE("LuaFnGetNearTeamCount")
		if( !pHuman->GetTeamInfo()->HasTeam() )
		{
			RETURNFALSE
		}
		SCANOPERATOR_ACTIVETEAMMATES_INIT	tInit;
		tInit.m_TeamID		=	pHuman->__GetTeamID() ;
		tInit.m_MemberGUID	=	pHuman->GetGUID() ;
		tInit.m_fRadius		=	g_Config.m_ConfigInfo.m_CanGetExpRange;
		tInit.m_nZoneRadius	=	MAX_RADIUS;
		tInit.m_bScanHuman	=	TRUE;
		tInit.m_Position	=	*pHuman->getWorldPos();
		tInit.m_ZoneID		=	pHuman->getZoneID();
		tInit.m_pScene		=	pScene;

		ScanOperator_ActiveTeammates	ScanOp;
		ScanOp.Init(&tInit);
		pScene->Scan(&ScanOp);
	
		pScene->m_aHumanList.m_Count = ScanOp.m_nActiveTeammateCount ;
		for( INT i=0; i<ScanOp.m_nActiveTeammateCount; i++ )
		{
			pScene->m_aHumanList.m_aHuman[i] = ScanOp.m_aActiveTeammate[i] ;
		}

 		Lua_PushNumber(L,pScene->m_aHumanList.m_Count) ;
		return 1;

		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION

		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-1-10
	*     �������� : LuaFnGetNearTeamMember
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������	
	*				index
	*					�������ѵ�������
	*     ����˵�� :
	*				ȡ��selfId��Ҹ������ѵ�objid(�����Լ�)
	*/
	INT LuaFnGetNearTeamMember(Lua_State* L)
	{	
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);	
		INT index = Lua_ValueToNumber(L,3);	

		BEGINHUMANDEFINE("LuaFnGetNearTeamMember")
		if( index>=pScene->m_aHumanList.m_Count || index<0 )
		{
			RETURNFALSE
		}
		Obj_Human* pTeammate = pScene->m_aHumanList.m_aHuman[index] ;
		if( pTeammate==NULL )
		{
			RETURNFALSE
		}
		
		Lua_PushNumber(L,pTeammate->GetID());
		return 1;

		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-3-33
	*     �������� : LuaFnGetMonsterGroupID
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					��Ҫȡ�����ݵĹ����objid
	*     ����˵�� :
	*				ȡ��selfId�����GroupID, monster.ini��������
	*/
	INT LuaFnGetMonsterGroupID(Lua_State* L)
	{	
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);	

		BEGINMONSTERDEFINE("LuaFnGetMonsterGroupID")
		
		Lua_PushNumber(L,pMonster->GetGroupID());
		return 1;

		ENDMONSTERDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION

		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-3-33
	*     �������� : LuaFnGetMonsterTeamID
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					��Ҫȡ�����ݵĹ����objid
	*     ����˵�� :
	*				ȡ��selfId�����TeamID, monster.ini��������
	*/
	INT LuaFnGetMonsterTeamID(Lua_State* L)
	{	
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);	

		BEGINMONSTERDEFINE("LuaFnGetMonsterTeamID")
		
		Lua_PushNumber(L,pMonster->GetTeamID());
		return 1;

		ENDMONSTERDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION

		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-3-23
	*     �������� : LuaFnGetMonsterDataID
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					��Ҫȡ�����ݵĹ����objid
	*     ����˵�� :
	*				ȡ��selfId�����DataID, monster.ini��������
	*/
	INT LuaFnGetMonsterDataID(Lua_State* L)
	{	
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);	

		BEGINMONSTERDEFINE("LuaFnGetMonsterDataID")
		
		Lua_PushNumber(L,pMonster->GetDataID());
		return 1;

		ENDMONSTERDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION

		RETURNFALSE
	}


}

#endif
	
