//-----------------------------------------------------------------------------
// �ļ��� : LuaFnTbl_DoAction.h
// ģ��	:	Script
// ����	 :  ��ָ����ɫ��ָ����Ϊ����غ���
// �޸���ʷ:
//-----------------------------------------------------------------------------
#ifndef __LUAFNTBL_DOACTION_H__
#define __LUAFNTBL_DOACTION_H__

#include "LuaInterface.h"
#include "LuaFnMacro.h"
#include "SkillInfo.h"
#include "GameStruct_Skill.h"
#include "StdImpact000.h"
#include "ScriptLogic.h"
#include "Impact_Core.h"
#include "ActionDelegator.h"
#include "GCMyBagList.h"
using namespace Combat_Module;
using namespace Impact_Module;
using namespace Script_Module;
using namespace Action_Module;

namespace LuaFnTbl
{

	/****************************************************************************
	 *	������Ա��	
	 *	��дʱ�䣺	2006-5-17	11:29
	 *	�������ƣ�	LuaFnDoAction
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *
	 *				nActionID
	 *					�������
	 *				TargetID
	 *					Ŀ���ţ�û��Ŀ���� -1 �Ϳ����ˣ�
	 *	����˵����	��Ŀ��������
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT	LuaFnDoAction(Lua_State* L) 
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId		=	Lua_ValueToNumber(L,1);
		ObjID_t selfId		=	Lua_ValueToNumber(L,2);
		SkillID_t nActionID	=	Lua_ValueToNumber(L,3);
		ObjID_t TargetID	=	Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnDoAction")

			pHuman->DoAction( nActionID, TargetID );
			return 0;

		ENDHUMANDEFINE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-1-10
	*     �������� : LuaFnUnitDoEmoteAction
	*     ����˵�� : 
	*				sceneID
	*					�������
	*				nSelfID
	*					������ID,�޷�������
	*				nSkillID
	*					���ܱ��
	*				nTargetID
	*					Ŀ����
	*     ����˵�� :
	*/
	INT	LuaFnUnitDoEmoteAction(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		=	Lua_ValueToNumber(L,1);
		ObjID_t nSelfID		=	Lua_ValueToNumber(L,2);
		SkillID_t nSkillID	=	Lua_ValueToNumber(L,3);
		ObjID_t nTargetID	=	Lua_ValueToNumber(L,4);

		BEGINSCENEDEFINE("LuaFnUnitDoEmoteAction")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			GetGlobalActionDelegator().RegisterEmoteAction(rMe, nSkillID);
		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-1-10
	*     �������� : LuaFnUnitUseSkill
	*     ����˵�� : 
	*				sceneID
	*					�������
	*				nSelfID
	*					������ID,�޷�������
	*				nSkillID
	*					���ܱ��
	*				nTargetID
	*					Ŀ����
	*				fPos_X
	*					Ŀ��λ�õ�X����
	*				fPos_Z
	*					Ŀ��λ�õ�Z����
	*				fDir
	*					Ŀ�귽��
	*				bPassSkillCheck
	*					�Ƿ���Լ��ܵ�����������Ʒ����
	*     ����˵�� :
	*/
	INT	LuaFnUnitUseSkill(Lua_State* L) 
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId	=	Lua_ValueToNumber(L,1);
		ObjID_t nSelfID		=	Lua_ValueToNumber(L,2);
		SkillID_t nSkillID	=	Lua_ValueToNumber(L,3);
		BYTE nSkillLevel	=	Lua_ValueToNumber(L,4);
		ObjID_t nTargetID	=	Lua_ValueToNumber(L,5);
		FLOAT fPos_X		=	Lua_ValueToNumber(L,6);
		FLOAT fPos_Z		=	Lua_ValueToNumber(L,7);
		FLOAT fDir		=	Lua_ValueToNumber(L,8);
		BOOL bPassSkillCheck = 	((0==Lua_ValueToNumber(L,9))?FALSE:TRUE);

		BEGINSCENEDEFINE("LuaFnUnitUseSkill")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			WORLD_POS pos;
			pos.m_fX = fPos_X;
			pos.m_fZ = fPos_Z;
			rMe.Skill_ExecFromScript(nSkillID, nSkillLevel, nTargetID, pos, fDir, bPassSkillCheck);			
		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-3-7
	*     �������� : LuaFnSendSpecificImpactToUnit
	*     ����˵�� : 
	*				sceneID
	*					�������
	*				nSelfID
	*					������ID,�޷�������
	*				nSenderID
	*					Ч��������ID,�޷�������
	*				nTargetID
	*					Ŀ���ɫID,�޷�������
	*				nImpactDataIndex
	*					Ч����������,�޷�������
	*				nDelayTime
	*					�ӳ�ʱ��
	*     ����˵�� :
	*				
	*/
	INT	LuaFnSendSpecificImpactToUnit(Lua_State* L)
	{
		__ENTER_FUNCTION

		SceneID_t sceneId				=	Lua_ValueToNumber(L,1);
		ObjID_t nSelfID				=	Lua_ValueToNumber(L,2);
		ObjID_t nSenderID			=	Lua_ValueToNumber(L,3);
		ObjID_t nTargetID			=	Lua_ValueToNumber(L,4);
		INT nImpactDataIndex	=	Lua_ValueToNumber(L,5);
		INT nDelayTime			=	Lua_ValueToNumber(L,6);

		BEGINSCENEDEFINE("LuaFnSendSpecificImpactToUnit")
			Obj* pSelf = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pSelf)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pSelf->GetObjType()))
			{
				RETURNFALSE
			}
			Obj* pSender = (Obj*) (pScene->GetObjManager()->GetObj(nSenderID));
			if(NULL==pSender)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pSender->GetObjType()))
			{
				RETURNFALSE
			}
			Obj* pTarget = (Obj*) (pScene->GetObjManager()->GetObj(nTargetID));
			if(NULL==pTarget)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pTarget->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rTar = *(Obj_Character*)pTarget;
			BOOL bRet = g_ImpactCore.SendImpactToUnit(rTar, nImpactDataIndex, nSenderID, nDelayTime);
			Lua_PushNumber(L, bRet);
			return 1;
		ENDSCENEDEFINE
		RETURNFALSE

		__LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2008-1-11
	*     �������� : LuaFnTransferUser
	*     ����˵�� : 
	*				sceneID
	*					�������
	*				nSelfID
	*					������ID,�޷�������
	*				nItemIndex
	*					��Ʒ������
	*				nDelayTime
	*					�ӳ�ʱ��
	*     ����˵�� :���㴫�ͽӿ�
	*				
	*/

	INT	LuaFnTransferUser(Lua_State* L)
	{
		__ENTER_FUNCTION

	
		SceneID_t sceneId				=	Lua_ValueToNumber(L,1);
		ObjID_t selfId				=	Lua_ValueToNumber(L,2);
		INT nItemUseCount	=	Lua_ValueToNumber(L,3);
		INT nDelayTime			=	Lua_ValueToNumber(L,4);
	
        INT nBagIndex=0;
		INT nItemIndex =0;
		BEGINSCENEDEFINE("LuaFnTransferUser")
		Obj_Human* pHuman = pScene->GetHumanManager()->GetHuman(selfId);
		if(NULL==pHuman)
		{
			RETURNFALSE
		}
		if(FALSE==IsCharacterObj(pHuman->GetObjType()))
		{
			RETURNFALSE
		}
	
        //ȷ������ǻ�״̬
		if( FALSE==pHuman->IsActiveObj() || FALSE==pHuman->IsAlive() )
		{
			RETURNFALSE

		}
		//����������������
		if(pScene->GetSceneType() == SCENE_TYPE_COPY)
		{
			RETURNFALSE

		}
		TargetingAndDepletingParams_T& rParams = pHuman->GetTargetingAndDepletingParams();
		nBagIndex = rParams.GetBagIndexOfDepletedItem();
		nItemIndex = rParams.GetItemIndexOfDepletedItem(); 
		if(INVALID_ID==nBagIndex)
		{
			nBagIndex = HumanItemLogic::GetItemPosByType(pHuman, nItemIndex);
		}
		Item *pItemClass=HumanItemLogic::GetItem(pHuman,nBagIndex);
		if(NULL==pItemClass)
		{
			RETURNFALSE
		}
		enum ItemParamValue itempv=IPV_INT;
		if(pItemClass->GetItemParam(4,itempv)<=0 && pItemClass->GetItemParam(8,itempv)<=0)
		{
			const WORLD_POS* pCur = pHuman->getWorldPos();
			WORLD_POS posCur = *pCur;
			if( !pScene->GetMap()->IsCanGo( posCur, pHuman->GetDriverLevel() ) )
			{
				RETURNFALSE
			}
			int nCount_SceneID=0;
			nCount_SceneID=(nItemUseCount<<16)+(INT)pScene->SceneID();
			SceneID_t NewSceneID=GETLOW(nCount_SceneID);
			int UseCount=GETHIGH(nCount_SceneID);
			HumanItemLogic::SetBagItemParam(pHuman,nBagIndex,0,itempv,nCount_SceneID);
			HumanItemLogic::SetBagItemParam(pHuman,nBagIndex,4,itempv,(INT)posCur.m_fX);
			HumanItemLogic::SetBagItemParam(pHuman,nBagIndex,8,itempv,(INT)posCur.m_fZ);
			GCItemInfo	 Msg;
			Msg.setID(nBagIndex);
			Msg.setIsNull(FALSE);
			pItemClass->SaveValueTo(Msg.getItem());
			pHuman->GetPlayer()->SendPacket(&Msg);
		
		}
		else
		{
			Obj* pObj = pHuman ;
			Obj_Character* pCharacter = (Obj_Character*)pObj ;
			WORLD_POS New ;
			int nCount_SceneID=pItemClass->GetItemParam(0,itempv);
			float fx=(float)pItemClass->GetItemParam(4,itempv);
			float fz=(float)pItemClass->GetItemParam(8,itempv);
			SceneID_t NewSceneID=GETLOW(nCount_SceneID);
			int UseCount=GETHIGH(nCount_SceneID);
			if(UseCount <= 0)
			{
				RETURNFALSE
			}

			New.m_fX = fx;
			New.m_fZ = fz;
			if(  pScene->SceneID()!=NewSceneID )
			{//�л�����
				Obj_Human* pTargetHuman = (Obj_Human*)pCharacter ;
				pTargetHuman->ChangeScene( pScene->SceneID(),NewSceneID, New, 9 );
			}
			else
			{//ͬ������ת��
				WORLD_POS Old = *pObj->getWorldPos();
				pScene->GetMap()->VerifyPos( &New ) ;
				if( !pScene->GetMap()->IsCanGo( New, pCharacter->GetDriverLevel() ) )
				{
					RETURNFALSE
				}
				((Obj_Human*)(pCharacter))->GetHumanAI()->PushCommand_Idle( ) ;
				pCharacter->Teleport( &New ) ;

			}
			UseCount--;
			if(UseCount == 0)
			{
					HumanItemLogic::DecItemLayCount(pHuman,nBagIndex,1);
			}
			nCount_SceneID=(UseCount<<16)+NewSceneID;
			HumanItemLogic::SetBagItemParam(pHuman,nBagIndex,0,itempv,nCount_SceneID);
			GCItemInfo	 Msg;
			Msg.setID(nBagIndex);
			Msg.setIsNull(FALSE);
			pItemClass->SaveValueTo(Msg.getItem());
			pHuman->GetPlayer()->SendPacket(&Msg);

		}
	
		Lua_PushNumber(L, 1);
		return 1;
		ENDSCENEDEFINE
			RETURNFALSE

			__LEAVE_FUNCTION
			RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-3-7
	*     �������� : LuaFnCancelSpecificImpact
	*     ����˵�� : 
	*				sceneID
	*					�������
	*				nSelfID
	*					������ID,�޷�������
	*				nImpactDataIndex
	*					Ч����������, �޷������ͣ��ڱ�׼Ч�������ѯ
	*     ����˵�� :
	*				
	*/
	INT	LuaFnCancelSpecificImpact(Lua_State* L)
	{
		__ENTER_FUNCTION

		SceneID_t sceneId				=	Lua_ValueToNumber(L,1);
		ObjID_t nSelfID				=	Lua_ValueToNumber(L,2);
		INT nImpactDataIndex	=	Lua_ValueToNumber(L,3);
		
		BEGINSCENEDEFINE("LuaFnCancelSpecificImpact")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			INT nRet = rMe.Impact_DispelSpecificImpactByDataIndex(nImpactDataIndex, 1);
			Lua_PushNumber(L, nRet);
			return 1;

		ENDSCENEDEFINE
		RETURNFALSE

		__LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-3-7
	*     �������� : LuaFnVerifyUsedItem
	*     ����˵�� : 
	*				sceneID
	*					�������
	*				nSelfID
	*					������ID,�޷�������
	*     ����˵�� :
	*				
	*/
	INT	LuaFnVerifyUsedItem(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		=	Lua_ValueToNumber(L,1);
		ObjID_t nSelfID		=	Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnVerifyUsedItem")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			if(Obj::OBJ_TYPE_HUMAN!=pObj->GetObjType())
			{
				RETURNFALSE
			}
			Obj_Human& rMe = *(static_cast<Obj_Human*>(pObj));
			BOOL bRet = rMe.VerifyItem();
			//return the result
			Lua_PushNumber(L, bRet);
			return	1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-3-7
	*     �������� : LuaFnDepletingUsedItem
	*     ����˵�� : 
	*				sceneID
	*					�������
	*				nSelfID
	*					������ID,�޷�������
	*     ����˵�� :
	*				
	*/
	INT	LuaFnDepletingUsedItem(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		=	Lua_ValueToNumber(L,1);
		ObjID_t nSelfID		=	Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnDepletingUsedItem")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			BOOL bRet = rMe.DepletingUsedItem();
			//return the result
			Lua_PushNumber(L, bRet);
			return	1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-3-7
	*     �������� : LuaFnGetTargetObjID
	*     ����˵�� : 
	*				sceneID
	*					�������
	*				nSelfID
	*					������ID,�޷�������
	*     ����˵�� :
	*				
	*/
	INT	LuaFnGetTargetObjID(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		=	Lua_ValueToNumber(L,1);
		ObjID_t nSelfID		=	Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGetTargetObjID")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			
			//return the result
			Lua_PushNumber(L, rParams.GetTargetObj());
			return	1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-3-7
	*     �������� : LuaFnGetTargetPosition_X
	*     ����˵�� : 
	*				sceneID
	*					�������
	*				nSelfID
	*					������ID,�޷�������
	*     ����˵�� :
	*				
	*/
	INT	LuaFnGetTargetPosition_X(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		=	Lua_ValueToNumber(L,1);
		ObjID_t nSelfID		=	Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGetTargetPosition_X")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			
			//return the result
			Lua_PushNumber(L, rParams.GetTargetPosition().m_fX);
			return	1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-3-7
	*     �������� : LuaFnGetTargetPosition_Z
	*     ����˵�� : 
	*				sceneID
	*					�������
	*				nSelfID
	*					������ID,�޷�������
	*     ����˵�� :
	*				
	*/
	INT	LuaFnGetTargetPosition_Z(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		=	Lua_ValueToNumber(L,1);
		ObjID_t nSelfID		=	Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGetTargetPosition_Z")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			
			//return the result
			Lua_PushNumber(L, rParams.GetTargetPosition().m_fZ);
			return	1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	
	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-3-7
	*     �������� : LuaFnGetTargetBagSlot
	*     ����˵�� : 
	*				sceneID
	*					�������
	*				nSelfID
	*					������ID,�޷�������
	*     ����˵�� :
	*				
	*/
	INT	LuaFnGetTargetBagSlot(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		=	Lua_ValueToNumber(L,1);
		ObjID_t nSelfID		=	Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGetTargetBagSlot")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			
			//return the result
			Lua_PushNumber(L, rParams.GetTargetBagSlot());
			return 1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/****************************************************************************
	 *	������Ա��	
	 *	��дʱ�䣺	2006-4-11	19:29
	 *	�������ƣ�	LuaFnGetBagIndexOfUsedItem
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *
	 *	����˵����	?
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnGetBagIndexOfUsedItem(Lua_State* L)
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L, 1);
		ObjID_t selfId = Lua_ValueToNumber(L, 2);

		BEGINHUMANDEFINE("LuaFnGetBagIndexOfUsedItem")
			TargetingAndDepletingParams_T& rParams = pHuman->GetTargetingAndDepletingParams();
			
			Lua_PushNumber(L, rParams.GetBagIndexOfDepletedItem());
			return 1;
		ENDHUMANDEFINE

		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-3-7
	*     �������� : LuaFnGetLowSectionOfTargetPetGuid
	*     ����˵�� : 
	*				sceneID
	*					�������
	*				nSelfID
	*					������ID,�޷�������
	*     ����˵�� :
	*				
	*/
	INT	LuaFnGetLowSectionOfTargetPetGuid(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		=	Lua_ValueToNumber(L,1);
		ObjID_t nSelfID		=	Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGetLowSectionOfTargetPetGuid")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			
			//return the result
			Lua_PushNumber(L, rParams.GetTargetPetGuid().GetLowSection());
			return	1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-3-7
	*     �������� : LuaFnGetHighSectionOfTargetPetGuid
	*     ����˵�� : 
	*				sceneID
	*					�������
	*				nSelfID
	*					������ID,�޷�������
	*     ����˵�� :
	*				
	*/
	INT	LuaFnGetHighSectionOfTargetPetGuid(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		=	Lua_ValueToNumber(L,1);
		ObjID_t nSelfID		=	Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnGetHighSectionOfTargetPetGuid")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			
			//return the result
			Lua_PushNumber(L, rParams.GetTargetPetGuid().GetHighSection());
			return	1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	
	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-3-7
	*     �������� : LuaFnIsScriptDynamicParamIndexLegal
	*     ����˵�� : 
	*				sceneID
	*					�������
	*				nSelfID
	*					������ID,�޷�������
	*				nIndex
	*					��������
	*     ����˵�� :
	*				
	*/
	INT	LuaFnIsScriptDynamicParamIndexLegal(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId		=	Lua_ValueToNumber(L,1);
		ObjID_t nSelfID		=	Lua_ValueToNumber(L,2);
		INT nIndex		=	Lua_ValueToNumber(L,3);

		BEGINSCENEDEFINE("LuaFnIsScriptDynamicParamIndexLegal")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			
			//return the result
			Lua_PushNumber(L, rParams.IsDynamicParamIndexLegal(nIndex));
			return	1;
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-3-7
	*     �������� : LuaFnGetScriptDynamicParamByIndex
	*     ����˵�� : 
	*				sceneID
	*					�������
	*				nSelfID
	*					������ID,�޷�������
	*				nIndex
	*					��������
	*     ����˵�� :
	*				
	*/
	INT	LuaFnGetScriptDynamicParamByIndex(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId		=	Lua_ValueToNumber(L,1);
		ObjID_t nSelfID		=	Lua_ValueToNumber(L,2);
		INT nIndex		=	Lua_ValueToNumber(L,3);
		INT nRet = 0;
		BEGINSCENEDEFINE("LuaFnGetScriptDynamicParamByIndex")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			if(TRUE==rParams.IsDynamicParamIndexLegal(nIndex))
			{
				//return the specific param
				Lua_PushNumber(L, rParams.GetDynamicParamByIndex(nIndex));
				return 1 ;
			}
			else
			{
				Lua_PushNumber(L, -1);
				return 1 ;
			}
		ENDSCENEDEFINE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-3-7
	*     �������� : LuaFnSetScriptDynamicParamByIndex
	*     ����˵�� : 
	*				sceneID
	*					�������
	*				nSelfID
	*					������ID,�޷�������
	*				nIndex
	*					��������
	*				nValue
	*					����ֵ
	*     ����˵�� :
	*				
	*/
	INT	LuaFnSetScriptDynamicParamByIndex(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId	=	Lua_ValueToNumber(L,1);
		ObjID_t nSelfID	=	Lua_ValueToNumber(L,2);
		INT nIndex	=	Lua_ValueToNumber(L,3);
		INT nValue	=	Lua_ValueToNumber(L,4);
		BEGINSCENEDEFINE("LuaFnSetScriptDynamicParamByIndex")
			Obj* pObj = (Obj*) (pScene->GetObjManager()->GetObj(nSelfID));
			if(NULL==pObj)
			{
				RETURNFALSE
			}
			if(FALSE==IsCharacterObj(pObj->GetObjType()))
			{
				RETURNFALSE
			}
			Obj_Character& rMe = *(Obj_Character*)pObj;
			SkillInfo_T& rSkillInfo = rMe.GetSkillInfo();
			TargetingAndDepletingParams_T& rParams = rMe.GetTargetingAndDepletingParams();
			rParams.SetDynamicParamByIndex(nIndex, nValue);
		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	INT	LuaFnSetDriverLevel(Lua_State* L)
	{
		__ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId				=	Lua_ValueToNumber(L,2);
		INT nDriverLevel			=	Lua_ValueToNumber(L,3);

		BEGINCHARCTERDEFINE("LuaFnSetDriverLevel")
		Assert( nDriverLevel>=0 && nDriverLevel <=10 );
		pCharacter->SetDriverLevel( nDriverLevel );
		return 0;
		ENDCHARCTERDEFINE

		__LEAVE_FUNCTION
		return 0;
	}

	INT	LuaFnGetDriverLevel(Lua_State* L)
	{
		__ENTER_FUNCTION

		SceneID_t sceneId			=	Lua_ValueToNumber(L,1);
		ObjID_t selfId				=	Lua_ValueToNumber(L,2);

		BEGINCHARCTERDEFINE("LuaFnGetDriverLevel")
		Lua_PushNumber(L, pCharacter->GetDriverLevel());
		return 1 ;
		ENDCHARCTERDEFINE
		__LEAVE_FUNCTION
		RETURNFALSE		
	}









}

#endif
