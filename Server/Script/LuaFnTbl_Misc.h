//-----------------------------------------------------------------------------
// �ļ��� : LuaFnTbl_Misc.h
// ģ��	:	Script
// ����	 :  ������غ���
// �޸���ʷ:
//-----------------------------------------------------------------------------
#ifndef __LUAFNTBL_MISC_H__
#define __LUAFNTBL_MISC_H__

#include "LuaFnMacro.h"
#include "TimeManager.h"
#include "GWChat.h"
#include "CGChat.h"
#include "FileDataMgr.h"
#include "GUIDManager.h"
#include "GCDelObject.h"
#include "Scene.h"
#include "ServerManager.h"
#include "SceneTimer.h"
#include "PacketFactoryManager.h"
#include "HumanItemLogic.h"
#include "GCCharAllTitles.h"
#include "GCBankBegin.h"

extern INT	g_Command_AlwaysLoadScript ;

namespace LuaFnTbl
{
/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnSetTimer
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*				 selfId
	*					������ID,�޷�������
	*				 scriptId
	*					ִ�нű�ID,short����
	*				 funcName
	*					��ʱ���ص���������,�ַ�������
	*				 tickTime
	*					��ʱ�����ü��ʱ��
	*     ����˵�� :
	*				 ����Ҵ򿪼�ʱ��,ʱ�䵽ʱ���Զ�����OnTimer����	
	*				 ��ұ������ϲ�û�м�ʱ�����ű�������Ҫ��ʱ��ʱ��ϵͳ���롰���á�����ϵͳ����
	*                ���أ�1,��ʾ����
	*/
	INT LuaFnSetTimer(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		ScriptID_t scriptId = Lua_ValueToNumber(L,3);
		const CHAR* funcName = Lua_ValueToString(L,4);
		INT tickTime = Lua_ValueToNumber(L,5);

		BEGINHUMANDEFINE("LuaFnSetTimer")
		if(pHuman->GetUseTimerCount() < pHuman->GetMaxTimerCount())
		{
			INT sceneTimerIndex = pScene->GetSceneTimer()->GetTimer();
			AssertEx(sceneTimerIndex>=0 && sceneTimerIndex < g_Config.m_ConfigInfo.m_MaxTimerCount,"No Timer resource for allocate");

			if(sceneTimerIndex>=0 && sceneTimerIndex < g_Config.m_ConfigInfo.m_MaxTimerCount)
			{
				pHuman->AddTimer(sceneTimerIndex);
				pScene->GetSceneTimer()->StartTheTimer(sceneTimerIndex,selfId,scriptId,funcName,tickTime);

				Lua_PushNumber(L,sceneTimerIndex);
				return 1;
			}
		}
		ENDHUMANDEFINE

		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnStopTimer
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				sceneTimerIndex
	*					��ʱ���ڳ�����ʱ�����е������±�,��SetTimerʱ����		
	*     ����˵�� :
    *				�رյ�ǰ��ҵļ�ʱ��	//ÿ��������������԰�������ʱ��
	*/
	INT LuaFnStopTimer(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		INT sceneTimerIndex = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnStopTimer")
		pScene->GetSceneTimer()->FreeTimer(sceneTimerIndex);
		return 0;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-11-24
	*     �������� : LuaFnCheckTimer
	*     ����˵�� : 
	*				 L
	*					LuaStateִ�л���ָ��
	*				 sceneId
	*					���øú�������ID,short
	*				 TimeIndex
	*					��ʱ������
	*     ����˵�� : �ж�TimeIndex�ż�ʱ���Ƿ��Ѿ���ʹ����
	*                ����1,��ʾ��ʹ�ã�����0����ʾû��ʹ��; ����-1����ʾ�쳣
	*/
	INT LuaFnCheckTimer(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		INT TimeIndex = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnCheckTimer")
		INT nRet = pScene->GetSceneTimer()->CheckTimer(TimeIndex) ;
		Lua_PushNumber(L,nRet);
		return 1;
		ENDSCENEDEFINE

		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetGameTime
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
    *				��ȡ��ǰ�������ʱ�䣬��λΪ��
	*				�õ�ǰʱ���ȥ����ʱ��
	*				û�в���
	*/
	INT LuaFnGetGameTime(Lua_State* L)	
	{	
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		
		BEGINHUMANDEFINE("LuaFnGetGameTime")
		Lua_PushNumber(L,g_pTimeManager->CurrentTime() / 1000  - pHuman->GetDB()->GetHumanDB()->m_LastLoginTime);
		return 1;
		ENDHUMANDEFINE

		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-1-10
	*     �������� : LuaFnGetCurrentTime
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*     ����˵�� :
	*				��ȡ��ǰʱ�䣬��λΪ��
	*/
	INT LuaFnGetCurrentTime(Lua_State* L)	
	{	
		Lua_PushNumber(L, g_pTimeManager->GetANSITime());
		return 1;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddGlobalCountNews
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				newstr
	*					��������,�ַ���ָ������
	*				count
	*					���ʹ���
	*     ����˵�� :
    *				��ȫ��������������Ź��棬���淢��ָ������
	*				û�в���
	*/
	INT LuaFnAddGlobalCountNews(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		CHAR* newstr = const_cast<CHAR*>(Lua_ValueToString(L,2));
		INT count = Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnAddGlobalCountNews")

		GWChat* pChatPacket = (GWChat*)(g_pPacketFactoryManager->CreatePacket(PACKET_GW_CHAT)) ;

		pChatPacket->SetSourGUID( INVALID_ID ) ;
		pChatPacket->SetChatType( CHAT_TYPE_SYSTEM ) ;
		pChatPacket->SetContexSize( count ) ;
		pChatPacket->SetContex( newstr ) ;

		g_pServerManager->SendPacket( pChatPacket, INVALID_ID ) ;

		return 0;

		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}
	
	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnPrintNum
	*     ����˵�� : 
	*				num
	*				��Ϣ�����ʾ����
	*     ����˵�� :
	*				������Ϣ��,���Խű���
	*/
	INT LuaFnPrintNum(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		INT num = Lua_ValueToNumber(L,1);

		CHAR msg[1024];
		memset(msg,0,1024) ;
		sprintf(msg,"%d\n",num);
		MyMessageBox(msg);
		return 0;

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnPrintStr
	*     ����˵�� : 
	*				strText
	*				��Ϣ�����ʾ�ַ���
	*     ����˵�� :
	*				������Ϣ��,���Խű���
	*/
	INT LuaFnPrintStr(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		const CHAR *strText = Lua_ValueToString(L,1);

		CHAR msg[1024];
		memset(msg,0,1024);
		sprintf(msg,"%s\n",strText);
		MyMessageBox(msg);
		return 0;

		LUA_LEAVE_FUNCTION
		return 0 ;
	}


	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnCallScriptFunction
	*     ����˵�� : 
	*     ����˵�� :
	*					���ÿɱ����������ű�����
	*/
	INT LuaFnCallScriptFunction(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		
		INT argc = Lua_GetTopIndex(L);

		AssertEx(argc>=3,"��������С��3");

		ScriptID_t scriptId = Lua_ValueToNumber(L,1);
		const CHAR* funcname = Lua_ValueToString(L,2);
		INT sceneId = Lua_ValueToNumber(L,3);

		Scene* pScene = g_pSceneManager->GetScene(sceneId);
		Assert( pScene && "CallScriptFunction pScene = null" );

		if(pScene)
		{
			INT nTopIndex = 0;
			pScene->GetLuaInterface()->mLua.SafeCallBegin( &nTopIndex ) ;
			
			BOOL bLoad = FALSE ;
			SFileData* pSFileData = (SFileData*)(pScene->GetLuaInterface()->m_ScriptTable.Get(scriptId)) ;
			if( pSFileData==NULL )
			{
				pSFileData = pScene->GetLuaInterface()->GetOwner()->GetScriptFileMgr()->GetFileData(scriptId);
				pScene->GetLuaInterface()->m_ScriptTable.Add( scriptId, pSFileData ) ;
				bLoad = TRUE ;
			}

#ifdef _DEBUG
			CHAR buf[32];
			tsnprintf(buf,32,"�ļ���%d",scriptId);
			AssertEx( pSFileData && "CallScriptFunction pFileData = null" ,buf);
#endif
			const CHAR* filename = pSFileData->GetFileName();
			AssertEx( filename && "CallScriptFunction filename = null",filename );

			if( g_Command_AlwaysLoadScript ) bLoad = TRUE ;
			BOOL bRet=TRUE ;
			if( bLoad )
			{
				bRet = pScene->GetLuaInterface()->mLua.Load( const_cast<CHAR*>(filename) ) ;
				AssertEx(bRet,filename);
				if( bRet == FALSE )
					return 0;
			}

			if ( pSFileData != NULL )
			{
				if ( filename != NULL )
				{
					if ( TRUE )
					{
						CHAR argv[10];
						memset(argv,0,10);
						
						char funcnameX[128];
						memset(funcnameX,0,128);
						sprintf(funcnameX,"x%.6d_%s",scriptId,funcname);
						char* fnname = funcnameX;

						#if _DEBUG
							lua_pushstring(L,filename);
							lua_setglobal(L,FILENAMEKEY);
						#endif

						if(!(pScene->GetLuaInterface()->PrefixCall(L,&fnname)))
						{
							Lua_PushNumber(L,-1);
							return 1;
						}

						LUA_ENTER_FUNCTION

						Lua_GetGlobal(L, fnname); 

						INT nBeginParamIndex = 3;
						for(INT i=nBeginParamIndex;i<=argc;i++)
						{
							switch ( Lua_GetValueType( L, i ) )
							{
							case LUA_TUSERDATA:
								argv[i-nBeginParamIndex] = 'p';
								//Lua_PushUserTag(L,Lua_ValueToUserData(L,i));
								break;
							case LUA_TNIL:
								argv[i-nBeginParamIndex] = 'n';
								Lua_PushNil(L);
								break;
							case LUA_TNUMBER:
								argv[i-nBeginParamIndex] = 'd';
								Lua_PushNumber(L,Lua_ValueToNumber(L,i));
								break;
							case LUA_TSTRING:
								argv[i-nBeginParamIndex] = 's';
								Lua_PushString(L,Lua_ValueToString(L,i));
								break;
							case LUA_TTABLE:
								argv[i-nBeginParamIndex] = 't';
								//Lua_PushNumber(L,Lua_ValueToString(L,i));
								break;
							case LUA_TFUNCTION:
							default:
								argv[i-nBeginParamIndex] = 'f';
								Lua_PushCFunction(L,Lua_ValueToCFunction(L,i));
								break;
							}
						}

						INT nResults = -1;
						INT ret = Lua_Call(L, argc - nBeginParamIndex + 1, nResults);

						if (ret != 0)
						{
							g_pLog->FastSaveLog( LOG_FILE_2,"Lua_Call %s %s!",filename,fnname);
						}

						int argnow = Lua_GetTopIndex(L);
						nResults = argnow - argc;//(INT)Lua_ValueToNumber(L,argc);		
						for(int i=1;i<=nResults;i++)
						{
							Lua_PushNumber(L,lua_tonumber(L,i +argc));
						}
						
						pScene->GetLuaInterface()->mLua.SafeCallEnd( argnow ) ;
						return nResults;

						LUA_LEAVE_FUNCTION
					}
				}
			}
		}
		
		Lua_PushNumber(L,-1);
		return 1;	

		LUA_LEAVE_FUNCTION

		Lua_PushNumber(L,-1);
		return 1;	
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-10-19
	*     �������� : LuaFnIsInDist
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					�Է���ID���,ObjID
	*				dist
	*					self��target����
	*     ����˵�� :
	*				�ж�selfId��targetId֮��ľ����Ƿ���dist��(����)
	*				Ŀǰ���ڻ�������,�ж���Һͱ�������֮��ľ���
	*/
	INT LuaFnIsInDist(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		ObjID_t targetId		=	Lua_ValueToNumber(L,3);
		FLOAT dist			=	Lua_ValueToNumber(L,4);

		BEGINSCENEDEFINE("LuaFnIsInDist")

		Obj* pSelfObj = (Obj*) (pScene->GetObjManager()->GetObj(selfId));
		Obj* pTargetObj = (Obj*) (pScene->GetObjManager()->GetObj(targetId));

		if(pSelfObj && pTargetObj)
		{
			FLOAT fTDist = MySqrt(pSelfObj->getWorldPos(),pTargetObj->getWorldPos());
			if(fTDist <= dist)
			{
				Lua_PushNumber(L,TRUE);
				return 1;
			}
			else
			{
				Lua_PushNumber(L,FALSE);
				return 1;
			}
		}
	
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	
	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-11-15
	*     �������� : LuaFnGetDist
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				targetId
	*					�Է���ID���,ObjID
	*     ����˵�� :
	*				�õ�selfId��targetId֮��ľ���
	*/
	INT LuaFnGetDist(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		ObjID_t targetId		=	Lua_ValueToNumber(L,3);
		FLOAT dist			=	Lua_ValueToNumber(L,4);

		BEGINSCENEDEFINE("LuaFnGetDist")

			Obj* pSelfObj = (Obj*) (pScene->GetObjManager()->GetObj(selfId));
			Obj* pTargetObj = (Obj*) (pScene->GetObjManager()->GetObj(targetId));

			if(pSelfObj && pTargetObj)
			{
				FLOAT fTDist = MySqrt(pSelfObj->getWorldPos(),pTargetObj->getWorldPos());
				
				Lua_PushNumber(L,fTDist);
				return 1;
			}


		RETURNFALSE
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-10-20
	*     �������� : LuaFnGuid2ObjId
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				guid
	*					GUID,GUID_t���ͣ�����������ҵ�GUIDת��
	*     ����˵�� :
	*				��Guidת��ΪobjId
	*/
	INT LuaFnGuid2ObjId(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		GUID_t guid		=	Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGuid2ObjId")
		Obj_Character* pCharacter = (Obj_Character*)(g_pGUIDManager->Get(guid)) ;
		if( pCharacter )
		{
			if( pCharacter->GetObjType()==Obj::OBJ_TYPE_HUMAN )
			{
				INT objId = pCharacter->GetID();
				Lua_PushNumber(L,objId);
				return 1;
			}
		}
		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-11-16
	*     �������� : LuaFnObjId2Guid
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� : ����selfId��GUID
	*/
	INT LuaFnObjId2Guid(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnObjId2Guid")
		if( pHuman )
		{
			GUID_t guid = pHuman->GetGUID() ;
			Lua_PushNumber(L,guid);
			return 1;
		}
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-10-21
	*     �������� : LuaFnRemoveMonster
	*     ����˵�� : 
	*     ����˵�� :
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				ɾ��Obj_Monster,��Obj_Monster�ӵ�ͼɾ��
	*/
	INT LuaFnRemoveMonster(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);

		AssertEx( FALSE, "�˽ӿ��Ѿ���������ʹ��LuaFnDeleteMonster" ) ;

		BEGINCHARCTERDEFINE("LuaFnRemoveMonster")
		pCharacter->SetActiveFlag( FALSE );
		pCharacter->CleanUp();
		pCharacter->getScene()->DeleteObject( pCharacter );

		return 0;
		ENDCHARCTERDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-11-2
	*     �������� : LuaFnAwardTitle
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				TitleIndex
	*					�ƺ�ID
	*				TitleId
	*					�ƺ�ID
	*     ����˵�� :
	*				ΪselfId����һ���ƺ�
	*/
	INT	LuaFnAwardTitle(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		INT TitleIndex		=	Lua_ValueToNumber(L,3);
		ObjID_t TitleId			=	Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnAwardTitle")
			switch(TitleIndex)
			{
			case 0:
				pHuman->SetLevelTitle(TitleId);
				break;
			case 1:
				pHuman->SetLifeTitle(TitleId);
				break;
			case 2:
				pHuman->SetRankTitle(TitleId);
				break;
			case 3:
				pHuman->SetStoryTitle(TitleId);
				break;
			case 4:
				pHuman->SetKillBossTitle(TitleId);
				break;
			case 5:
				pHuman->SetTreasureTitle(TitleId);
				break;
			
			default:
				{
					pHuman->SetDyTitle(TitleIndex-6, TitleId);
				}
				break;
			}

		ENDHUMANDEFINE
		return	0;
		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-11-2
	*     �������� : LuaFnDeleteTitle
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				TitleIndex
	*					�ƺ�ID
	*     ����˵�� :
	*				ɾ��ĳ���ƺ�
	*/
	INT	LuaFnDeleteTitle(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		INT TitleIndex		=	Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnDeleteTitle")
		switch(TitleIndex)
		{
			case 0:
				pHuman->SetLevelTitle(0);
				break;
			case 1:
				pHuman->SetLifeTitle(0);
				break;
			case 2:
				pHuman->SetRankTitle(0);
				break;
			case 3:
				pHuman->SetStoryTitle(0);
				break;
			case 4:
				pHuman->SetKillBossTitle(0);
				break;
			case 5:
				pHuman->SetTreasureTitle(0);
				break;

			default:
				{
					pHuman->SetDyTitle(TitleIndex-6, 0);
				}
				break;
		}


		ENDHUMANDEFINE
		return	0;
		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-11-3
	*     �������� : LuaFnDispatchAllTitle
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				���´��˵����гƺ�
	*/
	INT	LuaFnDispatchAllTitle(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		
		BEGINHUMANDEFINE("LuaFnDispatchAllTitle")
		//PACKET_ALL_TITLES_MSG
		GCCharAllTitles::TitleId titlelist[MAX_TITLE_ID_SIZE];
		int num = 0;
		if(pHuman->GetLevelTitle())
		{
			titlelist[num].m_titleid		=	_TITLE::LEVEL_TITLE;
			titlelist[num++].m_titleData	=	static_cast<SHORT>(pHuman->GetLevelTitle());
		}
		if(pHuman->GetLifeTitle())
		{
			titlelist[num].m_titleid		=	_TITLE::LIFE_TITLE;
			titlelist[num++].m_titleData	=	static_cast<SHORT>(pHuman->GetLifeTitle());
		}
		if(pHuman->GetRankTitle())
		{
			titlelist[num].m_titleid		=	_TITLE::RANK_TITLE;
			titlelist[num++].m_titleData	=	static_cast<SHORT>(pHuman->GetRankTitle());
		}
		if(pHuman->GetStoryTitle())
		{
			titlelist[num].m_titleid		=	_TITLE::STORY_TITLE;
			titlelist[num++].m_titleData	=	static_cast<SHORT>(pHuman->GetStoryTitle());
		}
		if(pHuman->GetKillBossTitle())
		{
			titlelist[num].m_titleid		=	_TITLE::KILLBOSS_TITLE;
			titlelist[num++].m_titleData	=	static_cast<SHORT>(pHuman->GetKillBossTitle());
		}
		if(pHuman->GetTreasureTitle())
		{
			titlelist[num].m_titleid		=	_TITLE::TREASURE_TITLE;
			titlelist[num++].m_titleData	=	static_cast<SHORT>(pHuman->GetTreasureTitle());
		}

		for(int i = 0; i<MAX_DYNAMIC_TITLE_SIZE; i++ )
		{
			if(pHuman->GetDyTitle(i))
			{
				titlelist[num].m_titleid		=	(_TITLE::DY_TITLE0 + i);
				titlelist[num++].m_titleData	=	pHuman->GetDyTitle(i);
			}
		}

		GCCharAllTitles::TitleString titlestrlist[MAX_STRING_TITLE_SIZE];
		int strnum = 0;
		const CHAR* szTemp = NULL;
		if(strlen(pHuman->GetNickName())!=0)
		{
			szTemp = pHuman->GetNickName();
			titlestrlist[strnum].m_titleid	=	_TITLE::NICK_TITLE;
			titlestrlist[strnum].m_strlength = (BYTE)strlen(szTemp);
			memset(titlestrlist[strnum].m_szTitle, 0, MAX_CHARACTER_TITLE);
			memcpy(titlestrlist[strnum].m_szTitle, szTemp, titlestrlist[strnum].m_strlength);
			strnum++;
		}
		if(strlen(pHuman->GetSpouseName())!=0)
		{
			szTemp = pHuman->GetSpouseName();
			titlestrlist[strnum].m_titleid	=	_TITLE::SPOUSE_TITLE;
			titlestrlist[strnum].m_strlength = (BYTE)strlen(szTemp);
			memset(titlestrlist[strnum].m_szTitle, 0, MAX_CHARACTER_TITLE);
			memcpy(titlestrlist[strnum].m_szTitle, szTemp, titlestrlist[strnum].m_strlength);
			strnum++;
		}
		if(strlen(pHuman->GetBangPaiName())!=0)
		{
			szTemp = pHuman->GetBangPaiName();
			titlestrlist[strnum].m_titleid	=	_TITLE::BANGPAI_TITLE;
			titlestrlist[strnum].m_strlength = (BYTE)strlen(szTemp);
			memset(titlestrlist[strnum].m_szTitle, 0, MAX_CHARACTER_TITLE);
			memcpy(titlestrlist[strnum].m_szTitle, szTemp, titlestrlist[strnum].m_strlength);
			strnum++;
		}
		if(strlen(pHuman->GetJieBaiName())!=0)
		{
			szTemp = pHuman->GetJieBaiName();
			titlestrlist[strnum].m_titleid	=	_TITLE::JIEBAI_TITLE;
			titlestrlist[strnum].m_strlength = (BYTE)strlen(szTemp);
			memset(titlestrlist[strnum].m_szTitle, 0, MAX_CHARACTER_TITLE);
			memcpy(titlestrlist[strnum].m_szTitle, szTemp, titlestrlist[strnum].m_strlength);
			strnum++;
		}
		if(strlen(pHuman->GetShiTuName())!=0)
		{
			szTemp = pHuman->GetShiTuName();
			titlestrlist[strnum].m_titleid	=	_TITLE::SHITU_TITLE;
			titlestrlist[strnum].m_strlength = (BYTE)strlen(szTemp);
			memset(titlestrlist[strnum].m_szTitle, 0, MAX_CHARACTER_TITLE);
			memcpy(titlestrlist[strnum].m_szTitle, szTemp, titlestrlist[strnum].m_strlength);
			strnum++;
		}
		if(strlen(pHuman->GetShangDianName())!=0)
		{
			szTemp = pHuman->GetShangDianName();
			titlestrlist[strnum].m_titleid	=	_TITLE::SHANGDIAN_TITLE;
			titlestrlist[strnum].m_strlength = (BYTE)strlen(szTemp);
			memset(titlestrlist[strnum].m_szTitle, 0, MAX_CHARACTER_TITLE);
			memcpy(titlestrlist[strnum].m_szTitle, szTemp, titlestrlist[strnum].m_strlength);
			strnum++;
		}

		GCCharAllTitles Msg;
		Msg.setTargetID(selfId);
		Msg.SetTitleIdNum(num);
		Msg.SetTitleIdList(titlelist);
		Msg.SetTitleStrNum(strnum);
		Msg.SetTitleStrList(titlestrlist);
		//PACKET_ALL_TITLES_MSG

		pHuman->GetPlayer()->SendPacket(&Msg);

		ENDHUMANDEFINE
		return	0;
		LUA_LEAVE_FUNCTION
		return 0 ;
	}



	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-11-3
	*     �������� : LuaFnGetTitle
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				TitleIndex
	*					�ƺ�ID
	*     ����˵�� :
	*				��ô��˵�ĳ���ƺ�
	*/
	INT	LuaFnGetTitle(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		INT TitleIndex		=	Lua_ValueToNumber(L,3);
		INT	TitleId		=	0;
		BEGINHUMANDEFINE("LuaFnGetTitle")
		switch(TitleIndex)
		{
			case 0:
				TitleId = pHuman->GetLevelTitle();
				break;
			case 1:
				TitleId = pHuman->GetLifeTitle();
				break;
			case 2:
				TitleId = pHuman->GetRankTitle();
				break;
			case 3:
				TitleId = pHuman->GetStoryTitle();
				break;
			case 4:
				TitleId = pHuman->GetKillBossTitle();
				break;
			case 5:
				TitleId = pHuman->GetTreasureTitle();
				break;

			default:
				{
					TitleId = pHuman->GetDyTitle(TitleIndex-6);
				}
				break;
		}

		Lua_PushNumber(L,TitleId);
		return	1;

		ENDHUMANDEFINE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-11-3
	*     �������� : LuaFnGetDyTitlesNum
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				��ô��˵Ķ�̬�ƺŸ���
	*/
	INT	LuaFnGetDyTitlesNum(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		INT	titlenum	=	0;
		BEGINHUMANDEFINE("LuaFnGetDyTitlesNum")
			for(int i = 0; i<MAX_DYNAMIC_TITLE_SIZE; i++ )
			{
				if(pHuman->GetDyTitle(i))
				{
					titlenum++;
				}
			}
			Lua_PushNumber(L,titlenum);
			return	1;
		ENDHUMANDEFINE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-11-3
	*     �������� : LuaFnAddDyTitle
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				TitleId
	*					�ƺ�ID
	*     ����˵�� :
	*				�����µĶ�̬�ƺţ�������˷��ڵ�һ��
	*/
	INT	LuaFnAddDyTitle(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		ObjID_t TitleId			=	Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnAddDyTitle")
			int i = 0;
			for(i = 0; i<MAX_DYNAMIC_TITLE_SIZE; i++ )
			{
				if(pHuman->GetDyTitle(i) == 0)
				{
					pHuman->SetDyTitle(i, TitleId); 
					break;
				}
			}
			if(i == MAX_DYNAMIC_TITLE_SIZE)
			{//���ǵ�һ��title
				pHuman->SetDyTitle(0, TitleId); 
			}

		ENDHUMANDEFINE
		return	0;
		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-11-4
	*     �������� : LuaFnAwardNickTitle
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				strText
	*					ǩ�������ʵ��ǳ�
	*     ����˵�� :
	*				���ô��˵�ǩ�������ʵ��ǳ�
	*/
	INT LuaFnAwardNickTitle(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		const CHAR *strText			=	Lua_ValueToString(L,3);

		BEGINHUMANDEFINE("LuaFnAwardNickTitle")
			pHuman->SetNickName(strText, (BYTE)strlen(strText));
		ENDHUMANDEFINE

			return	0;
		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-11-4
	*     �������� : LuaFnAwardSpouseTitle
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				strText
	*					��ż��
	*     ����˵�� :
	*				������ż��
	*/
	INT LuaFnAwardSpouseTitle(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		const CHAR *strText			=	Lua_ValueToString(L,3);

		BEGINHUMANDEFINE("LuaFnAwardSpouseTitle")
			pHuman->SetSpouseName(strText, (BYTE)strlen(strText));
		ENDHUMANDEFINE

			return	0;
		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-11-4
	*     �������� : LuaFnAwardJieBaiTitle
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				strText
	*					��ݳƺ�
	*     ����˵�� :
	*				���ý�ݳƺ�
	*/
	INT LuaFnAwardJieBaiTitle(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		const CHAR *strText			=	Lua_ValueToString(L,3);

		BEGINHUMANDEFINE("LuaFnAwardJieBaiTitle")
			pHuman->SetJieBaiName(strText, (BYTE)strlen(strText));
		ENDHUMANDEFINE

			return	0;
		LUA_LEAVE_FUNCTION
		return 0 ;
	}



	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-11-4
	*     �������� : LuaFnAwardBangPaiTitle
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				strText
	*					������
	*     ����˵�� :
	*				���ô��˵İ��ɳƺ�
	*/
	INT LuaFnAwardBangPaiTitle(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		const CHAR *strText			=	Lua_ValueToString(L,3);

		BEGINHUMANDEFINE("LuaFnAwardBangPaiTitle")
			pHuman->SetBangPaiName(strText, (BYTE)strlen(strText));
		ENDHUMANDEFINE
		
		return	0;
		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-11-4
	*     �������� : LuaFnAwardShiTuTitle
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				strText
	*					������
	*     ����˵�� :
	*				���ô��˵İ��ɳƺ�
	*/
	INT LuaFnAwardShiTuTitle(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		const CHAR *strText			=	Lua_ValueToString(L,3);

		BEGINHUMANDEFINE("LuaFnAwardShiTuTitle")
			pHuman->SetShiTuName(strText, (BYTE)strlen(strText));
		ENDHUMANDEFINE

			return	0;
		LUA_LEAVE_FUNCTION
			return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-11-4
	*     �������� : LuaFnAwardShangDianTitle
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				strText
	*					������
	*     ����˵�� :
	*				���ô��˵İ��ɳƺ�
	*/
	INT LuaFnAwardShangDianTitle(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		const CHAR *strText			=	Lua_ValueToString(L,3);

		BEGINHUMANDEFINE("LuaFnAwardShangDianTitle")
			pHuman->SetShangDianName(strText, (BYTE)strlen(strText));
		ENDHUMANDEFINE

			return	0;
		LUA_LEAVE_FUNCTION
			return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-11-8
	*     �������� : LuaFnEnableBankRentIndex
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*				RentIndex
	*					�򿪵�������
	*     ����˵�� :
	*				�򿪴��˵�������
	*/
	INT LuaFnEnableBankRentIndex(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		INT RentIndex		=	Lua_ValueToNumber(L,3);

		if(RentIndex<0&&RentIndex>5)
			return 0;
		BEGINHUMANDEFINE("LuaFnEnableBankRentIndex")
			//pHuman->SetBangPaiName(strText, (BYTE)strlen(strText));
			INT	CurEndIndex = pHuman->__GetBankEndIndex();
		switch(RentIndex)
		{
		case 1:
			{
			}
			break;
		case 2:
			{
				if(CurEndIndex <= RENTBOX2_START_INDEX)
					pHuman->__SetBankEndIndex(RENTBOX3_START_INDEX);
			}
			break;
		case 3:
			{
				if(CurEndIndex <= RENTBOX3_START_INDEX)
					pHuman->__SetBankEndIndex(RENTBOX4_START_INDEX);
			}
			break;
		case 4:
			{
				if(CurEndIndex <= RENTBOX4_START_INDEX)
					pHuman->__SetBankEndIndex(RENTBOX5_START_INDEX);
			}
			break;
		case 5:
			{
				if(CurEndIndex <= RENTBOX5_START_INDEX)
					pHuman->__SetBankEndIndex(MAX_BANK_SIZE);
			}
			break;
		default:
			{
			}
			break;
		}
		ENDHUMANDEFINE
		return 0;
		LUA_LEAVE_FUNCTION

			return 0 ;

	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-1-12
	*     �������� : LuaFnGetBankRentIndex
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				��õ�ǰ�������Ѵ򿪵�����
	*/
	INT LuaFnGetBankRentIndex(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		
		BEGINHUMANDEFINE("LuaFnGetBankRentIndex")
		INT	CurEndIndex = pHuman->__GetBankEndIndex();
		Lua_PushNumber(L, CurEndIndex);
		return 1;
		ENDHUMANDEFINE
		LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	INT LuaFnGetRMB(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId				=	Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetRMB")
			INT	CurRMB = pHuman->GetRMB();
		Lua_PushNumber(L, CurRMB);
		return 1;
		ENDHUMANDEFINE
			LUA_LEAVE_FUNCTION

			RETURNFALSE
	}

	INT LuaFnSetRMB(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId				=	Lua_ValueToNumber(L,2);
		INT NowRMB					=	Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnSetRMB")
			pHuman->SetRMB(NowRMB);
		return 0;
		ENDHUMANDEFINE
			LUA_LEAVE_FUNCTION

			RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-11-8
	*     �������� : LuaFnBankBegin
	*     ����˵�� : 
	*				SceneID
	*					�������
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				֪ͨ�ͻ��˿�ʼ�������н���
	*/
	INT LuaFnBankBegin(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId			=	Lua_ValueToNumber(L,2);
		ObjID_t targetId		=	Lua_ValueToNumber(L,3);
		BEGINHUMANDEFINE("LuaFnBankBegin")
		GCBankBegin Msg;
		Player *pPlayer = pHuman->GetPlayer();
		Msg.SetNPCObjID(targetId);
		pPlayer->SendPacket( &Msg ) ;

		ENDHUMANDEFINE

		return	0;
		LUA_LEAVE_FUNCTION
		return 0 ;
	}


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-11-14
	*     �������� : LuaSetPlayerDefaultReliveInfo
	*     ����˵�� : 
	*				sceneId
	*					�������
	*				playerId
	*					���볡�������ID
	*				pszHP
	*					HP������(%10=HP�ָ�10/100��10=HP�ָ�10��)
	*				pszMP
	*					MP������(%10=HP�ָ�10/100��10=HP�ָ�10��)
	*				pszSP
	*					SP������(%10=HP�ָ�10/100��10=HP�ָ�10��)
	*				defaultSceneID
	*					����ĳ���ID
	*				defaultPosX
	*					����X
	*				defaultPosZ
	*					����Z
	*     ����˵�� :
	*				��ҵ�ȱʡ������ص���������
	*/
	INT LuaSetPlayerDefaultReliveInfo(Lua_State* L)
	{
	LUA_ENTER_FUNCTION
		SceneID_t sceneId			= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId		= Lua_ValueToNumber( L, 2 );
		const char *pszHP			= Lua_ValueToString( L, 3 );
		const char *pszMP			= Lua_ValueToString( L, 4 );
		const char *pszSP			= Lua_ValueToString( L, 5 );
		SceneID_t defaultSceneID	= Lua_ValueToNumber( L, 6 );
		FLOAT defaultPosX		= (FLOAT)(Lua_ValueToNumber( L, 7 ));
		FLOAT defaultPosZ		= (FLOAT)(Lua_ValueToNumber( L, 8 ));

		BEGINHUMANDEFINE("LuaSetPlayerDefaultReliveInfo")

		Obj_Human::_RELIVE_INFO infoRelive;
		infoRelive.m_bHPUsePercent = (pszHP[0] == '%')?(TRUE):(FALSE);
		infoRelive.m_bMPUsePercent = (pszMP[0] == '%')?(TRUE):(FALSE);
		infoRelive.m_bSPUsePercent = (pszSP[0] == '%')?(TRUE):(FALSE);
		if ( infoRelive.m_bHPUsePercent )
		{
			infoRelive.m_nHPPercent	= atoi( &pszHP[1] );
		}
		else
		{
			infoRelive.m_nHP		= atoi( pszHP );
		}

		if ( infoRelive.m_bMPUsePercent )
		{
			infoRelive.m_nMPPercent	= atoi( &pszMP[1] );
		}
		else
		{
			infoRelive.m_nMP		= atoi( pszMP );
		}

		if ( infoRelive.m_bSPUsePercent )
		{
			infoRelive.m_nSPPercent	= atoi( &pszSP[1] );
		}
		else
		{
			infoRelive.m_nSP		= atoi( pszSP );
		}

		infoRelive.m_SceneID		= defaultSceneID;
		infoRelive.m_Pos.m_fX		= defaultPosX;
		infoRelive.m_Pos.m_fZ		= defaultPosZ;

		pHuman->SetReliveInfo( FALSE, &infoRelive );

		ENDHUMANDEFINE
		return	0;

	LUA_LEAVE_FUNCTION

		return 0 ;

	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-3-20
	*     �������� : LuaGetDayTime
	*     ����˵�� : 
	*					��
	*     ����˵�� :
	*				ȡ�õ�ǰʱ������������磺1140 ��ʾ2001��ĵ�140��
	*										  6099 ��ʾ2006��ĵ�99��
	*/
	INT LuaGetDayTime(Lua_State* L )
	{
	LUA_ENTER_FUNCTION

		UINT uRet = g_pTimeManager->GetDayTime() ;

		Lua_PushNumber(L, uRet) ;
		return	1;

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-3-23
	*     �������� : LuaGetObjCreateTime
	*     ����˵�� : 
	*				sceneId
    *					���øú�������ID,short����
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				ȡ��ObjID Ϊ selfId��obj�Ĵ���ʱ��
	*					
	*/
	INT LuaGetObjCreateTime(Lua_State* L)
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId		= Lua_ValueToNumber( L, 2 );

		BEGINSCENEDEFINE("LuaGetObjCreateTime")
		Obj* pObj = (Obj*)(pScene->GetObjManager()->GetObj(selfId));
		if( pObj )
		{
			UINT uRet = pObj->GetCreateTime() ;
			Lua_PushNumber(L, uRet) ;
			return	1;
		}
		ENDSCENEDEFINE
		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-3-28
	*     �������� : LuaFnUpdateAttr
	*     ����˵�� : 
	*				sceneId
	*					���øú�������ID,short����
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				ͨ���ʼ�ϵͳ������������
	*					
	*/
	INT LuaFnUpdateAttr(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId		= Lua_ValueToNumber( L, 2 );
		UINT param0		= Lua_ValueToNumber( L, 3 );	 
		UINT param1		= Lua_ValueToNumber( L, 4 );
		UINT param2		= Lua_ValueToNumber( L, 5 );
		UINT param3		= Lua_ValueToNumber( L, 6 );

		BEGINHUMANDEFINE("LuaFnUpdateAttr")
		switch(param1)
		{
		case MAIL_ATTR_GUILD:
			{
				pHuman->SetGuildID(param2);
				if(param2 == INVALID_ID)
				{
					pHuman->SetBangPaiName("",0);
					pHuman->UpdateTitlesToClient();
				}
			}
			break;
		case MAIL_ATTR_MONEY:
			{
				pHuman->SetMoney(pHuman->GetMoney()+(INT)param2);
				//��¼
				MONEY_LOG_PARAM	MoneyLogParam;
				MoneyLogParam.CharGUID	=	param3;
				MoneyLogParam.TargetGUID	=	pHuman->GetGUID();
				MoneyLogParam.OPType	=	MONEY_BUY_PLAYERSHOP_OUTCOME;	
				MoneyLogParam.Count		=	param2;
				MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
				MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
				MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
				SaveMoneyLog(&MoneyLogParam);
			}
		default:
			break;
		}

		ENDHUMANDEFINE
		return 0 ;
		LUA_LEAVE_FUNCTION

		return 0 ;
	}

	/****************************************************************************
	 *	������Ա��	
	 *	��дʱ�䣺	2006-4-26	17:08
	 *	�������ƣ�	LuaFnIsPasswordSetup
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				flag
	 *					��ǣ��������δ���ö���������Ч����� 1�����õ��������ö������룬0 ����Ҫ����
	 *
	 *	����˵����	�ж�����Ƿ����ö������룬0 Ϊδ���ã�1 Ϊ������
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnIsPasswordSetup(Lua_State* L)
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId		= Lua_ValueToNumber( L, 2 );
		BOOL flag		= Lua_ValueToNumber( L, 3 );	 

		BEGINHUMANDEFINE("LuaFnIsPasswordSetup")

			if( pHuman->__IsPasswordSetup() )
			{
				Lua_PushNumber(L, TRUE);
			}
			else
			{
				if( flag )
				{ // ���ýӿڷ���Ϣ OR_NEED_SETMINORPASSWORD
					pHuman->SendOperateResultMsg( OR_NEED_SETMINORPASSWORD );
				}

				Lua_PushNumber(L, FALSE);
			}

			return 1;

		ENDHUMANDEFINE

	LUA_LEAVE_FUNCTION

		return 0;
	}

	/****************************************************************************
	 *	������Ա��	
	 *	��дʱ�䣺	2006-4-26	17:21
	 *	�������ƣ�	LuaFnIsPasswordUnlocked
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				flag
	 *					��ǣ��������δ��������������Ч����� 1�����õ����߽����������룬0 ��ǿ�ƽ���
	 *
	 *	����˵����	�ж�����Ƿ�����������룬0 Ϊδ������1 Ϊ�ѽ���
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnIsPasswordUnlocked(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		= Lua_ValueToNumber( L, 1 );
		ObjID_t selfId		= Lua_ValueToNumber( L, 2 );
		BOOL flag		= Lua_ValueToNumber( L, 3 );	 

		BEGINHUMANDEFINE("LuaFnIsPasswordUnlocked")

			if( pHuman->__IsPasswordUnlock() )
			{
				Lua_PushNumber(L, TRUE);
			}
			else
			{
				if( flag )
				{ // ���ýӿڷ���Ϣ OR_NEED_UNLOCKMINORPASSWORD
					pHuman->SendOperateResultMsg( OR_NEED_UNLOCKMINORPASSWORD );
				}

				Lua_PushNumber(L, FALSE);
			}

			return 1;

		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION

		return 0;
	}


	/****************************************************************************
	 *	������Ա��	
	 *	��дʱ�䣺	
	 *	�������ƣ�	LuaFnSetCharacterTimer
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				nTime
	 *					ʱ����
	 *
	 *	����˵����	���� Character��ʱ�Ӽ���������0��ֹͣʱ��
	 *				ÿ����������ʱ���������ң�������� Scene.lua�����OnScenePlayerTimer����
	 *				����ǳ�����߹������������ҽű��� OnCharacterTimer����
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnSetCharacterTimer(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId	= Lua_ValueToNumber( L, 1 );
	    ObjID_t selfId		= Lua_ValueToNumber( L, 2 );
	    INT nTime			= Lua_ValueToNumber( L, 3 );

		BEGINCHARCTERDEFINE("LuaFnSetCharacterTimer")
			pCharacter->SetScriptTimer( nTime ) ;
			return 0;
		ENDCHARCTERDEFINE

		LUA_LEAVE_FUNCTION

		return 0;
	}

	/****************************************************************************
	 *	������Ա��	
	 *	��дʱ�䣺	
	 *	�������ƣ�	LuaFnSetCharacterDieTime
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					Monster ObjID���޷�������
	 *				nTime
	 *					ʱ����,��λ�����룩
	 *
	 *	����˵����	���� Character����ʧʱ��, ֻ��Monster��Pet��Ч
	 *				������ʧ��ʱ�򲻻�������估���������
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnSetCharacterDieTime(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId	= Lua_ValueToNumber( L, 1 );
	    ObjID_t selfId		= Lua_ValueToNumber( L, 2 );
		INT nTime			= Lua_ValueToNumber( L, 3 );

		BEGINCHARCTERDEFINE("LuaFnSetCharacterDieTime")
			if( pCharacter->GetObjType()==Obj::OBJ_TYPE_MONSTER 
				|| pCharacter->GetObjType()==Obj::OBJ_TYPE_PET )
			{
				pCharacter->SetDieTime( nTime ) ;
			}
			return 0;
		ENDCHARCTERDEFINE

		LUA_LEAVE_FUNCTION

		return 0;
	}












}

#endif