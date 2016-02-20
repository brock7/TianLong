//-----------------------------------------------------------------------------
// �ļ��� : LuaFnTbl_Attr.h
// ģ��	:	Script
// ����	 :  ������غ���
// �޸���ʷ:
//-----------------------------------------------------------------------------

#ifndef __LUAFNTBL_ATTR_H__
#define __LUAFNTBL_ATTR_H__

#include "LuaInterface.h"
#include "LuaFnMacro.h"
#include "GCNotifyChangeScene.h"
#include "GCDetailAttrib_Pet.h"
#include "GCChat.h"
#include "AI_Human.h"
#include "HumanItemLogic.h"
#include "PetManager.h"
#include "Obj_Pet.h"
#include "ItemOperator.h"
#include "AI_Monster.h"

#include "GCDetailSkillList.h"
#include "GamePlayer.h"

namespace LuaFnTbl
{
	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnSetPos
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
    *					���øú�������ID,short����
	*				selfId
	*					������ID,�޷�������
	*				x
	*					��Ҫwarp������λ�õ�X����ֵ
	*				z
	*					��Ҫwarp������λ�õ�Z����ֵ
	*     ����˵�� :
	*				��ɫ��ͬ����ת��
	*				���ý�ɫ(��Һ͹���)��ת����ǰ��ͼ�ϵ�ĳ��
	*/
	INT LuaFnSetPos(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		if(!Lua_IsNumber(L,1) || !Lua_IsNumber(L,2) || !Lua_IsNumber(L,3) || !Lua_IsNumber(L,4))
		{
			RETURNFALSE
		}

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		FLOAT x = Lua_ValueToNumber(L,3);
		FLOAT z = Lua_ValueToNumber(L,4);
		INT nDriverLevel = Lua_ValueToNumber(L,5);

		BEGINCHARCTERDEFINE("LuaFnSetPos")

		WORLD_POS Old = *pCharacter->getWorldPos() ;
		WORLD_POS New(x,z);

		pScene->GetMap()->VerifyPos( &New ) ;
		if( !pScene->GetMap()->IsCanGo( New,nDriverLevel ) )
		{
			return 0;
		}

		((Obj_Human*)(pCharacter))->GetHumanAI()->PushCommand_Idle( ) ;
		pCharacter->Teleport( &New ) ;

		return 0;
		ENDCHARCTERDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnSetRevPos
	*     ����˵�� : 
	*				L
	*				LuaStateִ�л���ָ��
	*				sceneId
	*				���øú�������ID,short
	*				selfId
	*				������ID,�޷�������
	*				x
	*					λ��x����
	*				z
	*					λ��z����
	*     ����˵�� :
	*				������ҵ������㣨�ٴε��뵱ǰ�����λ�ã�
	*				��������д�ϸ��
	*/
	INT LuaFnSetRevPos(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
	
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId	= Lua_ValueToNumber(L,2);
		FLOAT x		= Lua_ValueToNumber(L,3);
		FLOAT z		= Lua_ValueToNumber(L,4);

		BEGINCHARCTERDEFINE("LuaFnSetRevPos")
			if( pCharacter->GetObjType()!=Obj::OBJ_TYPE_MONSTER )
				return 0 ;
			Obj_Monster* pMonster = (Obj_Monster*)pCharacter ;
			WORLD_POS pos(x,z) ;
			pMonster->SetRespawnPos(&pos) ;
		ENDCHARCTERDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnNewWorld
	*     ����˵�� : 
	*				L
	*				LuaStateִ�л���ָ��
	*				sceneId
	*				���øú�������ID,short
	*				selfId
	*				������ID,�޷�������
	*				targetSceneId
	*				��Ҫ��ת����ID,short
	*				x
	*					λ��x����
	*				z
	*					λ��z����
	*     ����˵�� :
	*				�����л�����
	*				��selfId����Ҵ�sceneId��־�ĳ�����ת��targetSceneId��־�ĳ�����(x,z)λ��
	*				ʾ��:NewWorld(sceneId,selfId,4,100,102)
	*/
	INT LuaFnNewWorld(Lua_State* L)
	{
	LUA_ENTER_FUNCTION
		
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		SceneID_t targetSceneId = Lua_ValueToNumber(L,3);
		FLOAT x = Lua_ValueToNumber(L,4);
		FLOAT z = Lua_ValueToNumber(L,5);

		BEGINHUMANDEFINE("LuaFnNewWorld")
		if( pHuman->getScene()->SceneID() == targetSceneId )
		{
			return 0 ;
		}

		WORLD_POS New(x,z);
		pHuman->ChangeScene( pHuman->getScene()->SceneID(), targetSceneId, New, 9 ) ;

		return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetWorldPos
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
    *				�õ���ɫ(character)��ǰ����
	*				����x��z��FLOAT����ֵ
	*				ʾ��: x,z = GetWorldPos(sceneId,selfId) 
	*/
	INT LuaFnGetWorldPos(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINCHARCTERDEFINE("LuaFnGetWorldPos")
		Lua_PushNumber(L,pCharacter->getWorldPos()->m_fX);
		Lua_PushNumber(L,pCharacter->getWorldPos()->m_fZ);

		return 2;
		ENDCHARCTERDEFINE

		Lua_PushNumber(L,-1);
		Lua_PushNumber(L,-1);
		return 2;

		LUA_LEAVE_FUNCTION

		Lua_PushNumber(L,-1);
		Lua_PushNumber(L,-1);
		return 2;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnSetPatrolId
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				����Obj_Monster��Ѳ��·��
	*				ʾ��:SetPatrolId(sceneId,selfId,0)
	*				patrolId����Чֵ�� 
	*				0 �� ��sceneId��ָ����_patrolpoint.ini�ļ�PATROLNUMBER(������)֮���ֵ
	*/
	INT LuaFnSetPatrolId(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT patrolId = Lua_ValueToNumber(L,3);

		BEGINMONSTERDEFINE("LuaFnSetPatrolId")
		if(patrolId >= 0)
		{
			pMonster->SetPatrolId(patrolId);
			pMonster->GetMonsterAI()->StartPatrol(TRUE);
		}
		else
		{
			pMonster->GetMonsterAI()->StopPatrol();
			pMonster->setDir( pMonster->GetRespawnDir() );
			pMonster->SetPatrolId(INVALID_ID);
		}
		return 0;
		ENDMONSTERDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnIsCaptain
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				�жϵ�ǰ����Ƿ�Ϊ�ӳ�������ֵΪ0�����Ƕӳ�������ֵΪ1���ӳ���
	*				û�в���
	*/
	INT LuaFnIsCaptain(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnIsCaptain")
		if(pHuman->GetTeamInfo()->Leader()->m_GUID == pHuman->GetGUID())
		{
			Lua_PushNumber(L,1);
		}
		else
		{
			Lua_PushNumber(L,0);
		}
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE
		
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddXinFa
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				xinfaId
	*					�ķ����
	*     ����˵�� :
	*				����ǰ��Ҽ�ĳ���ķ�
	*				û�в���
	*/
	//INT LuaFnAddXinFa(Lua_State* L)	
	//{
	//	
	//	LUA_ENTER_FUNCTION
	//
	//	SceneID_t sceneId = Lua_ValueToNumber(L,1);
	//	ObjID_t selfId = Lua_ValueToNumber(L,2);
	//	INT xinfaId = Lua_ValueToNumber(L,3);

	//	BEGINCHARCTERDEFINE("LuaFnAddXinFa")

	//	BOOL bRet = ((Obj_Human*)pCharacter)->Skill_SetXinFaLevel(xinfaId,1);
	//	Lua_PushNumber(L,bRet);
	//	if(bRet)
	//	{
	//		Obj_Human* pHuman	=	(Obj_Human*)pCharacter;
	//		Assert(pHuman);
	//		
	//			XINFA_LOG_PARAM XinFaLogParam;
	//			XinFaLogParam.CharGUID	=	pHuman->GetGUID();
	//			XinFaLogParam.SceneID	=	pHuman->getScene()->SceneID();
	//			XinFaLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
	//			XinFaLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
	//			XinFaLogParam.OPType	=	XINFA_OP_SCRIPT_STUDY;
	//			XinFaLogParam.Count		=	pHuman->Skill_GetXinFaList().m_Count;
	//			XinFaLogParam.XinFaID_Nouse	=	xinfaId;
	//			XinFaLogParam.Level		=	1;
	//			SaveXinFaLog(&XinFaLogParam);
	//	}
	//	return 1;
	//	ENDCHARCTERDEFINE
	//	RETURNFALSE

	//	LUA_LEAVE_FUNCTION
	//	RETURNFALSE
	//}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddSkill
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				skillId
	*					���ܱ��
	*     ����˵�� :
	*				����ǰ��Ҽ�ĳ������
	*				û�в���
	*/
	INT LuaFnAddSkill(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
			
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT skillId = Lua_ValueToNumber(L,3);
		INT skillLevel = Lua_ValueToNumber(L,4);

		BEGINCHARCTERDEFINE("LuaFnAddSkill")	
		BOOL bRet = ((Obj_Human*)pCharacter)->Skill_ModifySkill(skillId, skillLevel, 1);
		Lua_PushNumber(L,bRet);
		
		if(bRet)
		{
			Obj_Human* pHuman = (Obj_Human*)pCharacter;
			Assert(pHuman);

			SKILL_LOG_PARAM SkillLogParam;
			SkillLogParam.CharGUID	=	pHuman->GetGUID();
			SkillLogParam.SceneID	=	pHuman->getScene()->SceneID();
			SkillLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
			SkillLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
			SkillLogParam.OPType	=	SKILL_OP_SCRIPT_STUDY;
			SkillLogParam.Count		=	pHuman->Skill_GetSkillList().m_Count;
			SkillLogParam.SkillID	=	skillId;
			SkillLogParam.Level		=	1;

			SaveSkillLog(&SkillLogParam);
		}

		return 1;
		ENDCHARCTERDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnDelXinFa
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				xinfaId
	*					�ķ����
	*     ����˵�� :
	*				ɾ����ǰ��ҵ�ĳ���ķ�
	*				û�в���
	*/
	//INT LuaFnDelXinFa(Lua_State* L)	
	//{
	//	LUA_ENTER_FUNCTION
	//	
	//	SceneID_t sceneId = Lua_ValueToNumber(L,1);
	//	ObjID_t selfId = Lua_ValueToNumber(L,2);
	//	INT xinfaId = Lua_ValueToNumber(L,3);

	//	BEGINCHARCTERDEFINE("LuaFnDelXinFa")	

	//		BOOL bRet  =((Obj_Human*)pCharacter)->Skill_SetXinFaLevel(xinfaId,-1);	
	//		Lua_PushNumber(L,bRet);
	//		if(bRet)
	//		{
	//			Obj_Human* pHuman	=	(Obj_Human*)pCharacter;
	//			Assert(pHuman);
	//			XINFA_LOG_PARAM XinFaLogParam;
	//			XinFaLogParam.CharGUID	=	pHuman->GetGUID();
	//			XinFaLogParam.SceneID	=	pHuman->getScene()->SceneID();
	//			XinFaLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
	//			XinFaLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
	//			XinFaLogParam.OPType	=	XINFA_OP_SCRIPT_ABANDON;
	//			XinFaLogParam.Count		=	pHuman->Skill_GetXinFaList().m_Count;
	//			XinFaLogParam.XinFaID	=	xinfaId;
	//			XinFaLogParam.Level		=	-1;
	//			SaveXinFaLog(&XinFaLogParam);
	//		}

	//		return 1;
	//	ENDCHARCTERDEFINE
	//	RETURNFALSE

	//	LUA_LEAVE_FUNCTION
	//	RETURNFALSE
	//}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnDelSkill
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				skillId
	*					���ܱ��				
	*     ����˵�� :
	*				ɾ����ǰ��ҵ�ĳ������
	*				û�в���
	*/
	INT LuaFnDelSkill(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT skillId = Lua_ValueToNumber(L,3);
		INT skillLevel = Lua_ValueToNumber(L,4);

		BEGINCHARCTERDEFINE("LuaFnDelSkill")
		
		BOOL bRet = ((Obj_Human*)pCharacter)->Skill_ModifySkill(skillId, skillLevel, -1);
		Lua_PushNumber(L,bRet);
		
		if(bRet)
		{
			Obj_Human* pHuman = (Obj_Human*)pCharacter;
			Assert(pHuman);

			SKILL_LOG_PARAM SkillLogParam;
			SkillLogParam.CharGUID	=	pHuman->GetGUID();
			SkillLogParam.SceneID	=	pHuman->getScene()->SceneID();
			SkillLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
			SkillLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
			SkillLogParam.OPType	=	SKILL_OP_SCRIPT_STUDY;
			SkillLogParam.Count		=	pHuman->Skill_GetSkillList().m_Count;
			SkillLogParam.SkillID	=	skillId;
			SkillLogParam.Level		=	-1;
			SaveSkillLog(&SkillLogParam);
		}

		return 1;
		ENDCHARCTERDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnHaveXinFa
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				xinfaId
	*					�ķ����				
	*     ����˵�� :
	*				�жϵ�ǰ����Ƿ���ĳ���ķ���δѧ���ü��ܷ���-1�����򷵻��ķ��ȼ�
	*				û�в���
	*/
	//INT LuaFnHaveXinFa(Lua_State* L)	
	//{
	//	LUA_ENTER_FUNCTION

	//	SceneID_t sceneId = Lua_ValueToNumber(L,1);
	//	ObjID_t selfId = Lua_ValueToNumber(L,2);
	//	INT xinfaId = Lua_ValueToNumber(L,3);

	//	BEGINCHARCTERDEFINE("LuaFnHaveXinFa")
	//	if(FALSE==((Obj_Human*)pCharacter)->Skill_GetXinFaLevel(xinfaId))
	//	{
	//		Lua_PushNumber(L,((Obj_Human*)pCharacter)->Skill_GetXinFaLevel(xinfaId));//: �����ǲ���������?
	//		return	1;
	//	}
	//	ENDCHARCTERDEFINE
	//	RETURNFALSE

	//	LUA_LEAVE_FUNCTION
	//	RETURNFALSE
	//}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnHaveSkill
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				skillId
	*					���ܱ��				
	*     ����˵�� :
	*				�жϵ�ǰ����Ƿ���ĳ�����ܡ�δѧ���ü��ܷ���-1�����򷵻ؼ��ܵȼ���//? ����û�еȼ�
	*/
	INT LuaFnHaveSkill(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT skillId = Lua_ValueToNumber(L,3);

		BEGINCHARCTERDEFINE("LuaFnHaveSkill")
		if(pCharacter->Skill_HaveSkill(skillId))
		{
			Lua_PushNumber(L,1);//: �����ǲ���������?
			return 1;
		}
		else
		{
			Lua_PushNumber(L,-1);
			return 1;
		}
		ENDCHARCTERDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnUseSkill
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				skillId
	*					���ܱ��				
	*     ����˵�� :
	*				�д���߻���ȶ
	*/
	INT LuaFnUseSkill(Lua_State* L)	
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT skillId = Lua_ValueToNumber(L,3);

		BEGINCHARCTERDEFINE("LuaFnUseSkill")
			AssertEx(FALSE,"�˺���������") ;
			if(1)
			{
				Lua_PushNumber(L,1);//: �����ǲ���������?
				return 1;
			}
			ENDCHARCTERDEFINE
				RETURNFALSE

	LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetCurCamp
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				�����ҵĵ�ǰ��Ӫ
	*				û�в���
	*/
	INT LuaFnGetCurCamp(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetCurCamp")
		Lua_PushNumber(L,pHuman->GetCampID());
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnSetCurCamp
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				campIndex
	*					��Ӫ���,INT����
	*     ����˵�� :
	*				�޸������Ӫ,��������δ��д
	*/
	INT LuaFnSetCurCamp (Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT campIndex = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnSetCurCamp")
		pHuman->SetBaseCampID(campIndex);//
		return 0;
		ENDHUMANDEFINE
		
		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/****************************************************************************
	 *	������Ա��	
	 *	��дʱ�䣺	2006-3-10	19:42
	 *	�������ƣ�	LuaFnGetGUID
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *
	 *	����˵����	�õ������ߵ� GUID
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnGetGUID(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINCHARCTERDEFINE("LuaFnGetGUID")	
		Lua_PushNumber(L, ((Obj_Human*)pCharacter)->GetGUID());
		return 1;
		ENDCHARCTERDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetExp
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				�����ҵĵ�ǰս������ֵ
	*/
	INT LuaFnGetExp(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINCHARCTERDEFINE("LuaFnGetExp")	
		Lua_PushNumber(L, ((Obj_Human*)pCharacter)->GetExp());
		return 1;
		ENDCHARCTERDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա :
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddExp
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				exp
	*					����ֵ����,INT
	*     ����˵�� :
	*				������ҵ�ս������ֵ
	*/
	INT LuaFnAddExp(Lua_State* L)	
	{	
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT exp = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnAddExp")
		pHuman->SetAlterExp( exp ) ;
		return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetHp
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				�����ҵ�����ֵ
	*				������ҵ�HPֵ
	*/
	INT LuaFnGetHp(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINCHARCTERDEFINE("LuaFnGetHp")	
		Lua_PushNumber(L, pCharacter->GetHP());
		return 1;
		ENDCHARCTERDEFINE
		RETURNFALSE
		
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnRestoreHp
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				�ָ���ɫ������ȫ��
	*				�ѽ�ɫ������ֵ��ΪMaxHPֵ
	*/
	INT LuaFnRestoreHp(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
			
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINCHARCTERDEFINE("LuaFnRestoreHp")
		pCharacter->HealthIncrement(pCharacter->GetMaxHP());
		return 0;
		ENDCHARCTERDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetMp
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				��ý�ɫ������ֵ
	*				���ؽ�ɫ��MPֵ
	*/
	INT LuaFnGetMp(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINCHARCTERDEFINE("LuaFnGetMp")
		Lua_PushNumber(L, pCharacter->GetMP());
		return 1;
		ENDCHARCTERDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnRestoreMp
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				�ָ���ҵ�����ȫ��
	*/
	INT LuaFnRestoreMp(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
			
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnRestoreMp")
		pHuman->SetMP(pHuman->GetMaxMP());
		return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetCon
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				�����ҵ�����ֵ
	*/
	INT LuaFnGetCon(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetCon")
		Lua_PushNumber(L,pHuman->GetCon());
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetSex
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				�����ҵ��Ա�
	*				����ֵΪ0��Ů
	*				����ֵΪ1����
	*/
	INT LuaFnGetSex(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		
		BEGINHUMANDEFINE("LuaFnGetSex")
		Lua_PushNumber(L,pHuman->GetSex());
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetName
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				��ý�ɫ(Obj_Character)���֣�����һ���ַ���
	*				��ɫ������Һ�Obj_Monster
	*				ʾ��: npcName = GetName(sceneId,npcId)
	*/
	INT LuaFnGetName(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		
		BEGINCHARCTERDEFINE("LuaFnGetName")
		Lua_PushString(L,pCharacter->GetName());
		return 1;
		ENDCHARCTERDEFINE
		Lua_PushString(L,"");
		return 1;
		
		LUA_LEAVE_FUNCTION

		Lua_PushString(L,"");
		return 1;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetMoney
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				�����ҵ��ֽ�
	*				�������ӵ�еĽ�Ǯ��
	*/
	INT LuaFnGetMoney(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetMoney")
		Lua_PushNumber(L,pHuman->GetMoney());	
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnCostMoney
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				payment
	*					��Ǯ����
	*     ����˵�� :
	*				�۳���ҽ�Ǯ
	*				�ɹ�����1
	*				�Ժ��Ƿ��Ƿ����޸ĺ����ҽ�Ǯ��?
	*				ĿǰpaymentӦ��Ϊ��ֵ
	*				ʧ�ܷ���-1
	*/
	INT LuaFnCostMoney(Lua_State* L)	
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		SceneID_t targetId = Lua_ValueToNumber(L,2);
		ObjID_t selfId = Lua_ValueToNumber(L,3);
		UINT payment = Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnCostMoney")
		if( pHuman->GetMoney() >= payment)
		{
			pHuman->SetMoney( pHuman->GetMoney() - payment);
			

			MONEY_LOG_PARAM	MoneyLogParam;
			MoneyLogParam.CharGUID	=	pHuman->GetGUID();
			MoneyLogParam.TargetGUID=	targetId;
			MoneyLogParam.OPType	=	MONEY_SCRIPT_COST;	
			MoneyLogParam.Count		=	payment;
			MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
			MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
			MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
			SaveMoneyLog(&MoneyLogParam);

			Lua_PushNumber(L,1);	
			return 1;
		}
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}				

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnAddMoney
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				profit
	*					��Ǯ������Ŀ
	*     ����˵�� :
	*				������ҽ�Ǯ
	*				����1,�ɹ�
	*				�Ժ��Ƿ��Ƿ����޸ĺ����ҽ�Ǯ��?
	*				����-1,ʧ��
	*/
	INT LuaFnAddMoney(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		UINT profit = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnAddMoney")
		pHuman->SetMoney( pHuman->GetMoney() + profit);

		MONEY_LOG_PARAM	MoneyLogParam;
		MoneyLogParam.CharGUID	=	pHuman->GetGUID();
		MoneyLogParam.OPType	=	MONEY_SCRIPT_COST;	
		MoneyLogParam.Count		=	profit;
		MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
		MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
		MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
		SaveMoneyLog(&MoneyLogParam);

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
	*     �������� : LuaFnGetLevel
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				ȡ����ҵ�ǰ�ȼ�
	*				Level�ĺϷ���Χ��?-?
	*				����-1,ʧ��
	*/
	INT LuaFnGetLevel(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINCHARCTERDEFINE("LuaFnGetLevel")	
		Lua_PushNumber(L,pCharacter->GetLevel());
		return 1;
		ENDCHARCTERDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	

	/** -----------------------------------------------------------------------
	*     ������Ա : ��
	*     ��дʱ�� : 2005-10-20
	*     �������� : LuaFnSetLevel
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				level
	*					��ɫ����,INT
	*     ����˵�� :
	*				���������ҵȼ�
	*				Level�ĺϷ���Χ��1-60
	*				����-1,ʧ��
	*/
	INT LuaFnSetLevel(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT level = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnSetLevel")	
		
		AssertEx(level > 0 && level <= DEFAULT_WASHPOINT_LEVEL,"level wrong");
		if( level <=0 || level > DEFAULT_WASHPOINT_LEVEL)
		{
			RETURNFALSE
		}
		
		pHuman->SetLevel(level);

		Lua_PushNumber(L,1);
		return 1;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		
		RETURNFALSE
	}

	/****************************************************************************
	 *	������Ա��	
	 *	��дʱ�䣺	2006-3-7	16:36
	 *	�������ƣ�	LuaFnCanLevelUp
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *
	 *	����˵����	�жϸ�����Ƿ��������
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnCanLevelUp(Lua_State* L)	
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L, 1);
		ObjID_t selfId = Lua_ValueToNumber(L, 2);

		BEGINHUMANDEFINE("LuaFnCanLevelUp")
			if( pHuman->GetExp() >= (UINT)g_LevelExpTbl.Get(pHuman->GetLevel() - 1) )
			{
				Lua_PushNumber(L, TRUE);
			}
			else
			{
				Lua_PushNumber(L, FALSE);
			}

			return 1;
		ENDHUMANDEFINE
		RETURNFALSE

	LUA_LEAVE_FUNCTION

		RETURNFALSE
	}


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-11-28
	*     �������� : LuaFnGetItemTableIndexByIndex
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				itemIndex
	*					��Ʒ�����ϵ�λ������
	*     ����˵�� :
	*				ȡ��ǰλ�õ���Ʒ����
	*				
	*/
	INT LuaFnGetItemTableIndexByIndex(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		UINT itemIndex = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnGetItemTableIndexByIndex")
			
		Item*	pItem = 	HumanItemLogic::GetItem(pHuman, itemIndex);
		INT itemType = pItem->GetItemTableIndex();

		Lua_PushNumber(L, itemType);
		return 1;

		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/****************************************************************************
	 *	������Ա��	
	 *	��дʱ�䣺	2006-3-30	14:47
	 *	�������ƣ�	LuaFnGetMaterialStartBagPos
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *
	 *	����˵����	�õ������в��ϵ���ʼλ��
	 *	�޸ļ�¼��
	*****************************************************************************/
	//INT LuaFnGetMaterialStartBagPos(Lua_State* L)	
	//{
	//LUA_ENTER_FUNCTION

	//	SceneID_t sceneId = Lua_ValueToNumber(L, 1);
	//	ObjID_t selfId = Lua_ValueToNumber(L, 2);

	//	BEGINHUMANDEFINE("LuaFnGetMaterialStartBagPos")
	//		Lua_PushNumber(L, MAT_CONTAINER_OFFSET);
	//		return 1;
	//	ENDHUMANDEFINE
	//	RETURNFALSE

	//LUA_LEAVE_FUNCTION

	//	RETURNFALSE
	//}

	/****************************************************************************
	 *	������Ա��	
	 *	��дʱ�䣺	2006-3-30	14:47
	 *	�������ƣ�	LuaFnGetMaterialEndBagPos
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *
	 *	����˵����	�õ������в��ϵĽ���λ��
	 *	�޸ļ�¼��
	*****************************************************************************/
	//INT LuaFnGetMaterialEndBagPos(Lua_State* L)	
	//{
	//LUA_ENTER_FUNCTION

	//	SceneID_t sceneId = Lua_ValueToNumber(L, 1);
	//	ObjID_t selfId = Lua_ValueToNumber(L, 2);

	//	BEGINHUMANDEFINE("LuaFnGetMaterialEndBagPos")
	//		Lua_PushNumber(L, TASK_CONTAINER_OFFSET-1);
	//		return 1;
	//	ENDHUMANDEFINE
	//	RETURNFALSE

	//LUA_LEAVE_FUNCTION

	//	RETURNFALSE
	//}

	/****************************************************************************
	 *	������Ա��	
	 *	��дʱ�䣺	2006-3-30	19:30
	 *	�������ƣ�	LuaFnGetItemCountInBagPos
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				uBagPos
	 *					������λ��
	 *
	 *	����˵����	�õ�����ĳ������Ʒ������
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnGetItemCountInBagPos(Lua_State* L)
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L, 1);
		ObjID_t selfId = Lua_ValueToNumber(L, 2);
		UINT uBagPos = Lua_ValueToNumber(L, 3);

		BEGINHUMANDEFINE("LuaFnGetItemCountInBagPos")

			Item* pItem = HumanItemLogic::GetItem(pHuman, uBagPos);

			Lua_PushNumber(L, pItem->GetLayedNum());
			return 1;

		ENDHUMANDEFINE
			RETURNFALSE

			LUA_LEAVE_FUNCTION

			RETURNFALSE
	}


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetItemCount
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				itemTypeSn
	*					ItemType��9λ����(ʵ��Ϊ8λ)
	*     ����˵�� :
	*				�жϵ�ǰ����ж��ٸ�����
	*				ֻ����ҵ�װ������Ʒ���в���
	*/
	INT LuaFnGetItemCount(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT itemTypeSn = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnGetItemCount")
		INT count =0;
				
		count = HumanItemLogic::CalcEquipItemCount(pHuman,itemTypeSn);
		
		count += HumanItemLogic::CalcBagItemCount(pHuman,itemTypeSn);

		Lua_PushNumber(L,count);
		return 1;

		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-1-22
	*     �������� : LuaFnHaveItemInBag
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				itemTypeSn
	*					ItemType��9λ����(ʵ��Ϊ8λ)
	*     ����˵�� :
	*				�жϵ�ǰ��Ұ����Ƿ�ӵ��ĳ������
	*				����ɹ�,���ش���0
	*				���ʧ��,����-1
	*/
	INT LuaFnHaveItemInBag (Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT itemTypeSn = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnHaveItemInBag")
		if(HumanItemLogic::CalcBagItemCount(pHuman,itemTypeSn))
		{
			Lua_PushNumber(L,1);
			return 1;			
		}
		else
		{
			Lua_PushNumber(L,-1);
			return 1;			
		}

		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnHaveItem
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				itemTypeSn
	*					ItemType��9λ����(ʵ��Ϊ8λ)
	*     ����˵�� :
	*				�жϵ�ǰ����Ƿ�ӵ��ĳ������
	*				����ɹ�,���ش���0
	*				���ʧ��,����-1
	*/
	INT LuaFnHaveItem (Lua_State* L)
	{
	LUA_ENTER_FUNCTION
	
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT itemTypeSn = Lua_ValueToNumber(L,3);
		
		BEGINHUMANDEFINE("LuaFnHaveItem")
		//INT count = 0;
	
		//�ȴ�װ��������
		for(INT i=0;i<HEQUIP_NUMBER;i++)
		{
			if( pHuman->GetDB()->GetEquipItem((HUMAN_EQUIP)i)->m_ItemIndex == itemTypeSn)
			{
				Lua_PushNumber(L,1);
				return 1;	
			}
		}
	
		if(HumanItemLogic::CalcBagItemCount(pHuman,itemTypeSn))
		{
			Lua_PushNumber(L,1);
			return 1;			
		}
		
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnDelItem
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				itemTypeSn
	*					ItemType��9λ����(ʵ��Ϊ8λ)
	*				delCount
	*					ɾ����Ŀ
	*     ����˵�� :
	*				ɾ����ǰ��ҵ�ĳ������	
	*				������ָ�������	
	*				������װ���ĵ���ɾ��
	*				ɾ���ɹ�,����1
	*				ɾ��ʧ��,����-1
	*/
	INT LuaFnDelItem (Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
			
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT itemIndex = Lua_ValueToNumber(L,3);
		INT delCount = Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnDelItem")
		//TSerialHelper help(itemIndex);
		//_ITEM_TYPE	itemType =	help.GetItemTypeStruct();

		//pHuman->EraseItem(itemType,delCount);
		ITEM_LOG_PARAM	ItemLogParam;
		ItemLogParam.OpType	=	ITEM_SCRIPT_DELETE;
		if( HumanItemLogic::EraseItem(&ItemLogParam,pHuman,itemIndex,delCount)==TRUE )
		{
			Lua_PushNumber(L,1);
			return 1;
		}
		else
		{
			Lua_PushNumber(L,0);
			return 1;
		}

		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnEraseItem
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				bagIndex
	*					�����ĸ���
	*     ����˵�� :
	*				ɾ����ǰ��ұ���ĳһ��ĵ���
	*				ɾ���ɹ�,����1
	*				ɾ��ʧ��,����-1
	*/
	INT LuaFnEraseItem (Lua_State* L)	
	{
		LUA_ENTER_FUNCTION
			
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		UINT bagIndex = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnEraseItem")
		
		ITEM_LOG_PARAM	ItemLogParam;
		ItemLogParam.OpType	=	ITEM_SCRIPT_DELETE;
		if( HumanItemLogic::EraseItem(&ItemLogParam,pHuman,bagIndex) == TRUE )
		{
			SaveItemLog(&ItemLogParam);
			Lua_PushNumber(L,1);
			return 1;
		}
		else
		{
			Lua_PushNumber(L,0);
			return 1;
		}

		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetMenPai
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				��ȡ��ҵ�ǰ��������ID
	*/
	INT LuaFnGetMenPai(Lua_State* L)
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetMenPai")
		Lua_PushNumber(L,pHuman->GetMenPai());
		return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-10-13
	*     �������� : LuaFnSetMenPai
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				menpaiId
	*					����ID,INT
	*     ����˵�� :
	*				������ҵ�ǰ��������ΪmenpaiId
	*/
	INT LuaFnSetMenPai(Lua_State* L)
	{
		LUA_ENTER_FUNCTION
		
		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT menpaiId = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnSetMenPai")
		
		AssertEx(menpaiId < MATTRIBUTE_NUMBER && menpaiId >= MATTRIBUTE_SHAOLIN ,"��������Խ��");
		if ( menpaiId >= MATTRIBUTE_NUMBER && menpaiId < MATTRIBUTE_SHAOLIN)
		{
			RETURNFALSE
		}

		pHuman->SetMenPai(menpaiId);
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : yangei
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetMenPaiMaster
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				��ȡ������ڰ��İ������ƣ��ַ�����
	*				������û��ʵ��
	*/
	INT LuaFnGetMenPaiMaster(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetMenPaiMaster")
		//pHuman->GetMenPaiName();
		//Lua_PushNumber(L,number);
		//Lua_PushString(L,str);
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetMenPaiFigure
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				��������ڰ���еĽ׼���0���ڣ�1�ӳ���2���ϣ�4����
	*				������û��ʵ��
	*/
	INT LuaFnGetMenPaiFigure(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		
		BEGINHUMANDEFINE("LuaFnGetMenPaiFigure")
		//pHuman->GetMenPaiName();
		//Lua_PushNumber(L,number);
		//Lua_PushString(L,str);
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnGetMenPaiTitle
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*     ����˵�� :
	*				��������ڰ���е�ͷ�Σ�����ͷ���в����������
	*				������û��ʵ��
	*/
	INT LuaFnGetMenPaiTitle(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		
		BEGINHUMANDEFINE("LuaFnGetMenPaiTitle")
		//pHuman->GetMenPaiName();
		//Lua_PushNumber(L,number);
		//Lua_PushString(L,str);
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}


	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnDestroyMenPai
	*     ����˵�� : 
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					���øú�������ID,short
	*				selfId
	*					������ID,�޷�������
	*				menpaiId
	*					�����
	*     ����˵�� :
	*				��ɢ���
	*				������û��ʵ��
	*/
	INT LuaFnDestroyMenPai(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT menpaiId = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnDestroyMenPai")
		//INT value = pHuman->GetTask(misId); 
		//Lua_PushNumber(L,value);
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	
	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnSetNpcCurCamp
	*     ����˵�� : 
	*     ����˵�� :
	*				�ı�NPC����Ӫ,��������δ��д
	*/
	INT LuaFnSetNpcCurCamp(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		INT npcIndex; npcIndex;
		INT campId; campId;

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2005-9-28
	*     �������� : LuaFnRestoreNpcCamp
	*     ����˵�� : 
	*     ����˵�� :
	*				�ָ�NPCԭ�е���Ӫ,��������δ��д
	*/
	INT LuaFnRestoreNpcCamp(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		INT npcindex; npcindex;

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-1-23
	*     �������� : LuaFnSetDamage
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				selfId
	*					�Լ���ObjID
	*				targetId
	*					Ŀ���ObjID
	*				damage
	*					��Ҫ���õ��˺�
	*     ����˵�� :
	*				����ĳ��obj���˺�
	*/
	INT LuaFnSetDamage(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		ObjID_t targetId = Lua_ValueToNumber(L,3);
		INT damage = Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnSetDamage")
			Obj* pTarObj = pScene->GetObjManager()->GetObj(targetId) ;
			if( pTarObj && IsCharacterObj(pTarObj->GetObjType()) )
			{
				Obj_Character* pTarChar = (Obj_Character*)pTarObj ;	
				pTarChar->HealthIncrement(-damage, pHuman);
			}
		return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-3-7
	*     �������� : LuaFnGmKillObj
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				selfId
	*					�Լ���ObjID
	*				targetId
	*					Ŀ���ObjID
	*     ����˵�� :
	*				ֱ��ɱ��ָ����obj;
	*/
	INT LuaFnGmKillObj(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		ObjID_t targetId = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnGmKillObj")
			Obj* pTarObj = pScene->GetObjManager()->GetObj(targetId) ;
			if( pTarObj && IsCharacterObj(pTarObj->GetObjType()) )
			{
				Obj_Character* pTarChar = (Obj_Character*)pTarObj ;	
				pTarChar->GM_ForceDie(pHuman);
			}
		return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION
		return 0 ;
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-1-23
	*     �������� : LuaFnFindMonsterByGUID
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				guid
	*					��ҪѰ�ҵĹ���GUID
	*     ����˵�� :
	*				Ѱ�ҳ����е�ĳ������,����GUID
	*/
	INT LuaFnFindMonsterByGUID(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		GUID_t guid = Lua_ValueToNumber(L,2);

		BEGINSCENEDEFINE("LuaFnFindMonsterByGUID")
			ObjID_t objid = pScene->GetMonsterManager()->FindMonsterByGUID(guid) ;
			Lua_PushNumber(L,objid);
		return 1;
		ENDSCENEDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnGetHumanMaxVigor
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*     ����˵�� :
	*				�õ���һ������ֵ
	*/
	//INT LuaFnGetHumanMaxVigor(Lua_State* L)	
	//{
	//	LUA_ENTER_FUNCTION

	//	SceneID_t sceneId = Lua_ValueToNumber(L,1);
	//	ObjID_t humanId = Lua_ValueToNumber(L,2);

	//	BEGINSCENEDEFINE("LuaFnGetHumanMaxVigor")
	//	Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
	//	if ( pHuman != NULL )
	//	{
	//		Lua_PushNumber(L,pHuman->GetMaxVigor());
	//		return 1;
	//	}
	//	ENDSCENEDEFINE
	//	RETURNFALSE

	//	LUA_LEAVE_FUNCTION
	//	RETURNFALSE
	//}
	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnGetHumanMaxEnergy
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*     ����˵�� :
	*				�õ���Ҿ������ֵ
	*/
	//INT LuaFnGetHumanMaxEnergy(Lua_State* L)	
	//{
	//	LUA_ENTER_FUNCTION

	//	SceneID_t sceneId = Lua_ValueToNumber(L,1);
	//	ObjID_t humanId = Lua_ValueToNumber(L,2);

	//	BEGINSCENEDEFINE("LuaFnGetHumanMaxEnergy")
	//	Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
	//	if ( pHuman != NULL )
	//	{
	//		Lua_PushNumber(L,pHuman->GetMaxEnergy());
	//		return 1;
	//	}
	//	ENDSCENEDEFINE
	//	RETURNFALSE

	//	LUA_LEAVE_FUNCTION
	//	RETURNFALSE
	//}
	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnGetHumanVigor
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*     ����˵�� :
	*				�õ���һ���ֵ
	*/
	//INT LuaFnGetHumanVigor(Lua_State* L)	
	//{
	//	LUA_ENTER_FUNCTION

	//	SceneID_t sceneId = Lua_ValueToNumber(L,1);
	//	ObjID_t humanId = Lua_ValueToNumber(L,2);

	//	BEGINSCENEDEFINE("LuaFnGetHumanVigor")
	//	Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
	//	if ( pHuman != NULL )
	//	{
	//		Lua_PushNumber(L,pHuman->GetVigor());
	//		return 1;
	//	}
	//	ENDSCENEDEFINE
	//	RETURNFALSE

	//	LUA_LEAVE_FUNCTION
	//	RETURNFALSE
	//}
	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnGetHumanEnergy
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*     ����˵�� :
	*				�õ���Ҿ���ֵ
	*/
	//INT LuaFnGetHumanEnergy(Lua_State* L)	
	//{
	//	LUA_ENTER_FUNCTION

	//	SceneID_t sceneId = Lua_ValueToNumber(L,1);
	//	ObjID_t humanId = Lua_ValueToNumber(L,2);

	//	BEGINSCENEDEFINE("LuaFnGetHumanEnergy")
	//	Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
	//	if ( pHuman != NULL )
	//	{
	//		Lua_PushNumber(L,pHuman->GetEnergy());
	//		return 1;
	//	}
	//	ENDSCENEDEFINE
	//	RETURNFALSE

	//	LUA_LEAVE_FUNCTION
	//	RETURNFALSE
	//}
	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnSetHumanMaxVigor
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*				value
	*					��ֵ��ֵ
	*     ����˵�� :
	*				������һ������ֵ
	*/
	//INT LuaFnSetHumanMaxVigor(Lua_State* L)	
	//{
	//	LUA_ENTER_FUNCTION

	//	SceneID_t sceneId = Lua_ValueToNumber(L,1);
	//	ObjID_t humanId = Lua_ValueToNumber(L,2);
	//	INT value	= Lua_ValueToNumber(L,3);

	//	BEGINSCENEDEFINE("LuaFnSetHumanMaxVigor")
	//	Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
	//	if ( pHuman != NULL )
	//	{
	//		pHuman->SetMaxVigor(value);
	//		return 0;
	//	}
	//	ENDSCENEDEFINE

	//	LUA_LEAVE_FUNCTION
	//	return 0;
	//}
	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnSetHumanMaxEnergy
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*				value
	*					��ֵ��ֵ
	*     ����˵�� :
	*				������Ҿ������ֵ
	*/
	//INT LuaFnSetHumanMaxEnergy(Lua_State* L)	
	//{
	//	LUA_ENTER_FUNCTION

	//	SceneID_t sceneId = Lua_ValueToNumber(L,1);
	//	ObjID_t humanId = Lua_ValueToNumber(L,2);
	//	INT value	= Lua_ValueToNumber(L,3);

	//	BEGINSCENEDEFINE("LuaFnSetHumanMaxEnergy")
	//	Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
	//	if ( pHuman != NULL )
	//	{
	//		pHuman->SetMaxEnergy(value);
	//		return 0;
	//	}
	//	ENDSCENEDEFINE

	//	LUA_LEAVE_FUNCTION

	//	return 0 ;
	//}
	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnSetHumanVigor
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*				value
	*					��ֵ��ֵ
	*     ����˵�� :
	*				������һ���
	*/
	//INT LuaFnSetHumanVigor(Lua_State* L)	
	//{
	//	LUA_ENTER_FUNCTION

	//	SceneID_t sceneId = Lua_ValueToNumber(L,1);
	//	ObjID_t humanId = Lua_ValueToNumber(L,2);
	//	INT value	= Lua_ValueToNumber(L,3);

	//	BEGINSCENEDEFINE("LuaFnSetHumanVigor")
	//	Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
	//	if ( pHuman != NULL )
	//	{
	//		pHuman->SetVigor(value);
	//		return 0;
	//	}
	//	ENDSCENEDEFINE

	//	LUA_LEAVE_FUNCTION
	//	return 0 ;
	//}
	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnSetHumanEnergy
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*				value
	*					��ֵ��ֵ
	*     ����˵�� :
	*				������Ҿ���
	*/
	//INT LuaFnSetHumanEnergy(Lua_State* L)	
	//{
	//	LUA_ENTER_FUNCTION

	//	SceneID_t sceneId = Lua_ValueToNumber(L,1);
	//	ObjID_t humanId = Lua_ValueToNumber(L,2);
	//	INT value	= Lua_ValueToNumber(L,3);

	//	BEGINSCENEDEFINE("LuaFnSetHumanEnergy")
	//	Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
	//	if ( pHuman != NULL )
	//	{
	//		pHuman->SetEnergy(value);
	//		return 0;
	//	}
	//	ENDSCENEDEFINE

	//	LUA_LEAVE_FUNCTION
	//	return 0 ;
	//}
	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnGetHumanVigorRegeneRate
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*     ����˵�� :
	*				�õ���һ����ָ��ٶ�
	*/
	//INT LuaFnGetHumanVigorRegeneRate(Lua_State* L)	
	//{
	//	LUA_ENTER_FUNCTION

	//	SceneID_t sceneId = Lua_ValueToNumber(L,1);
	//	ObjID_t humanId = Lua_ValueToNumber(L,2);

	//	BEGINSCENEDEFINE("LuaFnGetHumanVigorRegeneRate")
	//	Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
	//	if ( pHuman != NULL )
	//	{
	//		Lua_PushNumber(L,pHuman->GetVigorRegeneRate());
	//		return 1;
	//	}
	//	ENDSCENEDEFINE
	//	RETURNFALSE

	//	LUA_LEAVE_FUNCTION
	//	RETURNFALSE
	//}
	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnGetHumanEnergyRegeneRate
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*     ����˵�� :
	*				�õ���Ҿ����ָ��ٶ�
	*/
	//INT LuaFnGetHumanEnergyRegeneRate(Lua_State* L)	
	//{
	//	LUA_ENTER_FUNCTION

	//	SceneID_t sceneId = Lua_ValueToNumber(L,1);
	//	ObjID_t humanId = Lua_ValueToNumber(L,2);

	//	BEGINSCENEDEFINE("LuaFnGetHumanEnergyRegeneRate")
	//	Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
	//	if ( pHuman != NULL )
	//	{
	//		Lua_PushNumber(L,pHuman->GetEnergyRegeneRate());
	//		return 1;
	//	}
	//	ENDSCENEDEFINE
	//	RETURNFALSE

	//	LUA_LEAVE_FUNCTION
	//	RETURNFALSE
	//}
	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnSetHumanVigorRegeneRate
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*				value
	*					��ֵ��ֵ
	*     ����˵�� :
	*				������һ����ָ��ٶ�
	*/
	//INT LuaFnSetHumanVigorRegeneRate(Lua_State* L)	
	//{
	//	LUA_ENTER_FUNCTION

	//	SceneID_t sceneId = Lua_ValueToNumber(L,1);
	//	ObjID_t humanId = Lua_ValueToNumber(L,2);
	//	INT value	= Lua_ValueToNumber(L,3);

	//	BEGINSCENEDEFINE("LuaFnSetHumanVigorRegeneRate")
	//	Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
	//	if ( pHuman != NULL )
	//	{
	//		pHuman->SetVigorRegeneRate(value);
	//		return 0;
	//	}
	//	ENDSCENEDEFINE

	//	LUA_LEAVE_FUNCTION
	//	return 0 ;
	//}
	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnSetHumanEnergyRegeneRate
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*				value
	*					��ֵ��ֵ
	*     ����˵�� :
	*				������Ҿ����ָ��ٶ�
	*/
	//INT LuaFnSetHumanEnergyRegeneRate(Lua_State* L)	
	//{
	//	LUA_ENTER_FUNCTION

	//	SceneID_t sceneId = Lua_ValueToNumber(L,1);
	//	ObjID_t humanId = Lua_ValueToNumber(L,2);
	//	INT value	= Lua_ValueToNumber(L,3);

	//	BEGINSCENEDEFINE("LuaFnSetHumanEnergyRegeneRate")
	//	Obj_Human *pHuman = pScene->GetHumanManager()->GetHuman( humanId );
	//	if ( pHuman != NULL )
	//	{
	//		pHuman->SetEnergyRegeneRate(value);
	//		return 0;
	//	}
	//	ENDSCENEDEFINE

	//	LUA_LEAVE_FUNCTION
	//	return 0 ;
	//}
	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnSetHumanHairColor
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*				value
	*					��ֵ��ֵ
	*     ����˵�� :
	*				�������ͷ����ɫ
	*/
	INT LuaFnSetHumanHairColor(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		UINT value	= Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnSetHumanHairColor")
			pHuman->__SetHairColor(value);
			return 0;
		ENDHUMANDEFINE

		LUA_LEAVE_FUNCTION

		return 0 ;
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnGetHumanHairColor
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*				value
	*					��ֵ��ֵ
	*     ����˵�� :
	*				�������ͷ����ɫ
	*/
	INT LuaFnGetHumanHairColor(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetHumanHairColor")
			UINT uValue = pHuman->__GetHairColor();
			Lua_PushNumber(L,uValue);
			return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}
	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnSetHumanHairModel
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*				uHairStyle
	*					��ֵ��ֵ
	*     ����˵�� :
	*				�������ͷ��ģ��
	*/
	INT LuaFnSetHumanHairModel(Lua_State* L)	
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L, 1);
		ObjID_t selfId = Lua_ValueToNumber(L, 2);
		UINT uHairStyle = Lua_ValueToNumber(L, 3);

		BEGINHUMANDEFINE("LuaFnSetHumanHairModel")
			pHuman->__SetHairModel(uHairStyle);
			Lua_PushNumber(L,1);
			return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/****************************************************************************
	 *	������Ա��	
	 *	��дʱ�䣺	2006-5-15	14:36
	 *	�������ƣ�	LuaFnChangeHumanHairModel
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *				uHairStyle
	 *					����ֵ
	 *
	 *	����˵����	����ҵķ��ͻ��� uHairStyle
	 *	�޸ļ�¼��
	*****************************************************************************/
	INT LuaFnChangeHumanHairModel(Lua_State* L)	
	{
	LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L, 1);
		ObjID_t selfId = Lua_ValueToNumber(L, 2);
		UINT uHairStyle = Lua_ValueToNumber(L, 3);

		BEGINHUMANDEFINE("LuaFnChangeHumanHairModel")
			if ( uHairStyle >= g_HairStyleTbl.m_uCount )
			{
				Assert( uHairStyle < g_HairStyleTbl.m_uCount );
				Lua_PushNumber(L, FALSE);
				return 1;
			}

			_HAIR_STYLE_TYPE_* pHairStyleType;
			pHairStyleType = &(g_HairStyleTbl.m_HairStyle[uHairStyle]);
			if ( pHairStyleType->m_nIndex != uHairStyle )
			{
				AssertEx( pHairStyleType->m_nIndex == uHairStyle, "Invalid Hair Style, Please Check the Table!" );
				Lua_PushNumber(L, FALSE);
				return 1;
			}

			if ( pHairStyleType->m_nUseScope < 2 )
			{
				AssertEx( pHairStyleType->m_nUseScope >= 2, "This Hair Style Can't be Used in this Scenario!" );
				Lua_PushNumber(L, FALSE);
				return 1;
			}

			if ( pHairStyleType->m_nRaceID != pHuman->GetSex() )
			{
				AssertEx( pHairStyleType->m_nRaceID == pHuman->GetSex(), "This Hair Style is NOT Suit this Player's Gender!" );
				Lua_PushNumber(L, FALSE);
				return 1;
			}

			INT nCostCount;
			nCostCount = pHairStyleType->m_nItemCostCount;
			if ( nCostCount > 0 )
			{
				if ( nCostCount > HumanItemLogic::CalcBagItemCount(pHuman, pHairStyleType->m_uItemSerial) )
				{
					Lua_PushNumber(L, FALSE);
					return 1;
				}
				
				ITEM_LOG_PARAM ItemLogParam;
				ItemLogParam.OpType	= ITEM_SCRIPT_DELETE;
				HumanItemLogic::EraseItem(&ItemLogParam, pHuman, pHairStyleType->m_uItemSerial, nCostCount);
			}

			pHuman->__SetHairModel(uHairStyle);
			Lua_PushNumber(L, TRUE);
			return 1;
		ENDHUMANDEFINE
		RETURNFALSE

	LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/** -----------------------------------------------------------------------
	*     ������Ա : 
	*     ��дʱ�� : 2006-02-20
	*     �������� : LuaFnGetHumanHairModel
	*     ����˵�� :
	*				L
	*					LuaStateִ�л���ָ��
	*				sceneId
	*					������
	*				humanId
	*					���ID
	*				value
	*					��ֵ��ֵ
	*     ����˵�� :
	*				�������ͷ��ģ��
	*/
	INT LuaFnGetHumanHairModel(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetHumanHairModel")
			UINT uValue = pHuman->__GetHairModel();
			Lua_PushNumber(L,uValue);
			return 1;
		ENDHUMANDEFINE
		RETURNFALSE

		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	/****************************************************************************
	 *	������Ա��	
	 *	��дʱ�䣺	2006-5-15	14:26
	 *	�������ƣ�	LuaFnGetHumanGoodBadValue
	 *	����˵����
	 *				sceneId
	 *					�������
	 *				selfId
	 *					������ ID���޷�������
	 *
	 *	����˵����	�õ� selfId ���ƶ�ֵ
	 *	�޸ļ�¼��
	*****************************************************************************/
	//INT LuaFnGetHumanGoodBadValue(Lua_State* L)	
	//{
	//LUA_ENTER_FUNCTION

	//	SceneID_t sceneId = Lua_ValueToNumber(L, 1);
	//	ObjID_t selfId = Lua_ValueToNumber(L, 2);

	//	BEGINHUMANDEFINE("LuaFnGetHumanGoodBadValue")
	//		INT nValue = pHuman->GetRMBMoney();
	//		Lua_PushNumber(L, nValue);
	//		return 1;
	//	ENDHUMANDEFINE
	//	RETURNFALSE

	//LUA_LEAVE_FUNCTION
	//	RETURNFALSE
	//}


	INT LuaFnGetSkillMenpaiNS(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT skillId = Lua_ValueToNumber(L,3);

		SkillTemplateData_T const* pSkillTemplate = g_SkillTemplateDataMgr.GetInstanceByID(skillId);
		if( pSkillTemplate )
		{
			Lua_PushNumber( L, pSkillTemplate->GetMenPai() );
			return 1;
		}

		RETURNFALSE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	INT LuaFnGetSkillLevelNS(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT skillId = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnGetSkillLevelNS")
		if(pHuman->Skill_HaveSkill(skillId))
		{
			Lua_PushNumber( L, pHuman->Skill_GetSkillLevel( skillId ) );
			return 1;
		}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	INT LuaFnHaveSkillAndLevelNS(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT skillId = Lua_ValueToNumber(L,3);
		INT skillLvl = Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnHaveSkillAndLevelNS")
		Assert( skillLvl>0 && skillLvl <=12 ); 

		if(pHuman->Skill_HaveSkill(skillId,skillLvl))
		{
			Lua_PushNumber( L, 1 );
			return 1;
		}
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	INT LuaFnCheckStudySkillNS(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT skillId = Lua_ValueToNumber(L,3);
		INT skillLvl = Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnCheckStudySkillNS")
		Assert( skillLvl>0 && skillLvl <=12 );

		if( pHuman->Skill_GetCount() < MAX_CHAR_SKILL_NUM )
		{
			SkillTemplateData_T const* pSkillTemplate = g_SkillTemplateDataMgr.GetInstanceByID(skillId);
			if( pSkillTemplate )
			{
				if( pSkillTemplate->GetClassByUser() == 0 &&
					pSkillTemplate->GetMenPai() == pHuman->GetMenPai() &&
					pSkillTemplate->GetSkillMaxLevel() >= skillLvl )
				{
					INT nNeedLevel = 0;
					if( pSkillTemplate->GetSkillClass() > 0 )
					{
						ID_t nInstance = pSkillTemplate->GetSkillInstance( skillLvl - 1 );
						const SkillInstanceData_T* pSkillInstanceData = g_SkillDataMgr.GetInstanceByID(nInstance);
						if( pSkillInstanceData )
						{
							nNeedLevel = pSkillInstanceData->GetStudyLevel();
						}
					}
					if( pHuman->GetLevel() >= nNeedLevel )
					{
						if( pHuman->Skill_HaveSkill(skillId) == FALSE )
						{
							Lua_PushNumber( L, 1 );
							return 1;
						}
						if( pHuman->Skill_HaveSkill(skillId,skillLvl) == FALSE &&
							pHuman->Skill_HaveSkill(skillId,skillLvl-1) == TRUE )
						{
							Lua_PushNumber( L, 1 );
							return 1;
						}
					}
				}
			}
		}
			
		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	INT LuaFnGetSkillNameNS(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT skillId = Lua_ValueToNumber(L,3);
		INT skillLvl = Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnGetSkillNameNS")
		Assert( skillLvl>0 && skillLvl <=12 );

		Lua_PushString( L, pHuman->Skill_GetName(skillId, skillLvl) );
		return 1;

		ENDHUMANDEFINE
		return 0;
		LUA_LEAVE_FUNCTION
		return 0;
	}

	INT LuaFnGetStudyMoneyNS(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT skillId = Lua_ValueToNumber(L,3);
		INT skillLvl = Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnGetStudyMoneyNS")
		Assert( skillLvl>0 && skillLvl <=12 );

		SkillTemplateData_T const* pSkillTemplate = g_SkillTemplateDataMgr.GetInstanceByID(skillId);
		if( pSkillTemplate )
		{
			if( pSkillTemplate->GetClassByUser() == 0 &&
				pSkillTemplate->GetMenPai() == pHuman->GetMenPai() &&
				pSkillTemplate->GetSkillMaxLevel() >= skillLvl )
			{
				ID_t nInstance;
				if( pSkillTemplate->GetSkillClass() > 0 )
				{
					nInstance = pSkillTemplate->GetSkillInstance( skillLvl - 1 );
				}
				else
				{
					nInstance = pSkillTemplate->GetSkillInstance( 0 );
				}
				const SkillInstanceData_T* pSkillInstanceData = g_SkillDataMgr.GetInstanceByID(nInstance);
				if( pSkillInstanceData )
				{
					Lua_PushNumber( L, pSkillInstanceData->GetStudyMoney() );
					return 1;
				}
			}
		}

		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	INT LuaFnSaveLog(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		const char * faile = Lua_ValueToString(L,3);

		BEGINHUMANDEFINE("LuaFnSaveLog")

		g_pLog->FastSaveLog( LOG_FILE_1, "%s: faile obj=%d scene=%d", 
		faile,selfId, sceneId) ;
		return 1;

		ENDHUMANDEFINE
			RETURNFALSE
			LUA_LEAVE_FUNCTION
			RETURNFALSE
	}


	//�����м�����Ʒ��Ϣ��Ŀǰ���1��
	INT LuaFnGetWebShopInfoNum(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetWebShopInfoNum")

		BOOL bHaveInfo = FALSE;
		if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].IsSucceed == FALSE &&
			pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Id > 0 )
		{
			if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Money > 0 )
				bHaveInfo = TRUE;

			if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_1 > 0 && pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_1 > 0 )
				bHaveInfo = TRUE;

			if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_2 > 0 && pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_2 > 0 )
				bHaveInfo = TRUE;

			if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_3 > 0 && pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_3 > 0 )
				bHaveInfo = TRUE;

			if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_4 > 0 && pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_4 > 0 )
				bHaveInfo = TRUE;

			if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_5 > 0 && pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_5 > 0 )
				bHaveInfo = TRUE;
		}

		Lua_PushNumber( L, bHaveInfo );
		return 1;

		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	//�õ�ĳ����Ʒ�����ٸ���Ʒ����
	INT LuaFnGetWebShopItemNum(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetWebShopInfoNum")

		INT nItems = 0;
		if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].IsSucceed == FALSE &&
			pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Id > 0 )
		{
			if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_1 > 0 && pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_1 > 0 )
				nItems ++;

			if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_2 > 0 && pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_2 > 0 )
				nItems ++;

			if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_3 > 0 && pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_3 > 0 )
				nItems ++;

			if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_4 > 0 && pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_4 > 0 )
				nItems ++;

			if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_5 > 0 && pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_5 > 0 )
				nItems ++;
		}

		Lua_PushNumber( L, nItems );
		return 1;

		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	//�õ�ĳ����Ʒ�����ٸ�Ԫ������
	INT LuaFnGetWebShopMoney(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnGetWebShopMoney")

		INT nMoney = 0;
		if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].IsSucceed == FALSE &&
			pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Id > 0 )
		{
			nMoney = pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Money;
		}

		Lua_PushNumber( L, nMoney );
		return 1;

		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	//�õ�ĳ����Ʒ��ĳ����Ʒ����������1��ʼ
	INT LuaFnGetWebShopItem(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT		nIndex = Lua_ValueToNumber(L,3);

		BEGINHUMANDEFINE("LuaFnGetWebShopItem")

		INT nItemIndex = 1;
		if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].IsSucceed == FALSE &&
			pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Id > 0 )
		{
			if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_1 > 0 && pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_1 > 0 )
			{
				if( nIndex == nItemIndex )
				{
					Lua_PushNumber(L, pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_1);
					Lua_PushNumber(L, pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_1);
					return 2;
				}
				else
				{
					nItemIndex ++;
				}
			}

			if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_2 > 0 && pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_2 > 0 )
			{
				if( nIndex == nItemIndex )
				{
					Lua_PushNumber(L, pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_2);
					Lua_PushNumber(L, pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_2);
					return 2;
				}
				else
				{
					nItemIndex ++;
				}
			}

			if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_3 > 0 && pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_3 > 0 )
			{
				if( nIndex == nItemIndex )
				{
					Lua_PushNumber(L, pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_3);
					Lua_PushNumber(L, pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_3);
					return 2;
				}
				else
				{
					nItemIndex ++;
				}
			}

			if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_4 > 0 && pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_4 > 0 )
			{
				if( nIndex == nItemIndex )
				{
					Lua_PushNumber(L, pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_4);
					Lua_PushNumber(L, pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_4);
					return 2;
				}
				else
				{
					nItemIndex ++;
				}
			}

			if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_5 > 0 && pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_5 > 0 )
			{
				if( nIndex == nItemIndex )
				{
					Lua_PushNumber(L, pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_5);
					Lua_PushNumber(L, pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_5);
					return 2;
				}
				else
				{
					nItemIndex ++;
				}
			}
		}

		return 0;

		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}

	//����Ʒ�ŵ�������
	INT LuaFnWebShopReceive(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);

		BEGINHUMANDEFINE("LuaFnWebShopReceive")

		if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].IsSucceed == TRUE )
			RETURNFALSE

		pScene->mItemList.Init();
		if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_1 > 0 && pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_1 > 0 )
		{
			TSerialHelper help( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_1 );
			_ITEM_TYPE itemType = help.GetItemTypeStruct();
			if( !itemType.isNull() )
			{
				pScene->mItemList.AddType( itemType, pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_1 );
			}
		}
		if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_2 > 0 && pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_2 > 0 )
		{
			TSerialHelper help( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_2 );
			_ITEM_TYPE itemType = help.GetItemTypeStruct();
			if( !itemType.isNull() )
			{
				pScene->mItemList.AddType( itemType, pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_2 );
			}
		}
		if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_3 > 0 && pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_3 > 0 )
		{
			TSerialHelper help( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_3 );
			_ITEM_TYPE itemType = help.GetItemTypeStruct();
			if( !itemType.isNull() )
			{
				pScene->mItemList.AddType( itemType, pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_3 );
			}
		}
		if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_4 > 0 && pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_4 > 0 )
		{
			TSerialHelper help( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_4 );
			_ITEM_TYPE itemType = help.GetItemTypeStruct();
			if( !itemType.isNull() )
			{
				pScene->mItemList.AddType( itemType, pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_4 );
			}
		}
		if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_5 > 0 && pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_5 > 0 )
		{
			TSerialHelper help( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Item_5 );
			_ITEM_TYPE itemType = help.GetItemTypeStruct();
			if( !itemType.isNull() )
			{
				pScene->mItemList.AddType( itemType, pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].ItemCount_5 );
			}
		}

		if( HumanItemLogic::CanReceiveItemList( pHuman, pScene->mItemList ) == FALSE )
		{
			RETURNFALSE
		}

		//�õ���Ʒ
		ITEM_LOG_PARAM ItemLogParam;
		ItemLogParam.OpType		= ITEM_CREATE_FROM_SCRIPT;
		ItemLogParam.CharGUID	= pHuman->GetGUID();
		ItemLogParam.SceneID	= pHuman->getScene()->SceneID();
		ItemLogParam.XPos		= pHuman->getWorldPos()->m_fX;
		ItemLogParam.ZPos		= pHuman->getWorldPos()->m_fZ;
		HumanItemLogic::ReceiveItemList( &ItemLogParam, pHuman, pScene->mItemList, 0 );
		SaveItemLog(&ItemLogParam);

		if( pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Money > 0 )
		{
			pHuman->SetRMB( pHuman->GetRMB() + pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0].Money );
		}

		_WEB_SHOPINFO* pws = (_WEB_SHOPINFO*)&(pHuman->GetDB()->GetPrivateInfoDB()->pi.wsInfo[0]);
		pws->IsSucceed = TRUE;

		//return TRUE;
		Lua_PushNumber( L, 1 );
		return 1;

		ENDHUMANDEFINE
		RETURNFALSE
		LUA_LEAVE_FUNCTION
		RETURNFALSE
	}


	/*
		����˫������״̬
		nExpTime			����ʱ��
		nExpDoubleNum		���鱶��

	*/
	INT LuaFnSetDoubleExp(Lua_State* L)	
	{
		LUA_ENTER_FUNCTION

		SceneID_t sceneId = Lua_ValueToNumber(L,1);
		ObjID_t selfId = Lua_ValueToNumber(L,2);
		INT nExpTime = Lua_ValueToNumber(L,3);
		INT nExpDoubleNum = Lua_ValueToNumber(L,4);

		BEGINHUMANDEFINE("LuaFnSetDoubleExp")
		Assert( nExpTime>0 && nExpDoubleNum >1 );
		nExpTime=nExpTime*1000;
		//����Ѿ����ù�˫������
		if(pHuman->m_ExpTimer.IsSetTimer())
		{
			//������鱶��С���Ѿ����ù��ľͲ�����������
			if(nExpDoubleNum < GETLOW(pHuman->GetDoubleExpTime_Num()))
			{
				Lua_PushNumber( L, 0 );
				return 1;

			}
			//������鱶����Ⱦ�����ʱ��
			if(nExpDoubleNum == GETLOW(pHuman->GetDoubleExpTime_Num()))
			{
				pHuman->m_ExpTimer.SetTermTime(pHuman->m_ExpTimer.GetTermTime()+nExpTime);
			}
			//������鱶�������Ѿ����ù��ľ͸���˫������
			if(nExpDoubleNum > GETLOW(pHuman->GetDoubleExpTime_Num()))
			{
				pHuman->m_ExpTimer.CleanUp();
				pHuman->m_ExpTimer.BeginTimer(nExpTime,g_pTimeManager->CurrentTime());
				pHuman->SetDoubleExpTime_Num(((nExpTime/1000)<<16)+nExpDoubleNum);
			}

		}
		else
		{
			//����˫������;
			pHuman->m_ExpTimer.BeginTimer(nExpTime,g_pTimeManager->CurrentTime());
			pHuman->SetDoubleExpTime_Num(((nExpTime/1000)<<16)+nExpDoubleNum);


		}
		Lua_PushNumber( L, 1 );
		return 1;
		ENDHUMANDEFINE
			RETURNFALSE
			LUA_LEAVE_FUNCTION
			RETURNFALSE
	}
















}

#endif
