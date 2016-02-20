#include "stdafx.h"


#include "CGAskPrivateInfo.h"
#include "Log.h"
#include "Player.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"

#include "GCPrivateInfo.h"

UINT CGAskPrivateInfoHandler::Execute( CGAskPrivateInfo* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer ) ;

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman ) ;

	Scene* pScene = pHuman->getScene() ;
	if( pScene==NULL )
	{
		Assert(FALSE) ;
		return PACKET_EXE_ERROR ;
	}
	//����߳�ִ����Դ�Ƿ���ȷ
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;

	ObjID_t SelfID = pHuman->GetID();
	if( SelfID == INVALID_ID )
	{
		Assert(FALSE) ;
		return PACKET_EXE_ERROR ;
	}

	BOOL bSelf = FALSE;
	if( SelfID == pPacket->GetPlayerID() || pPacket->GetPlayerID() == INVALID_ID )
	{
		bSelf = TRUE;
	}

	if(bSelf)
	{
		//�����Լ�����Ϣ
		//const _PRIVATE_INFO_DB_LOAD *SelfInfo;
		//SelfInfo = pHuman->GetDB()->GetPrivateInfoDB();

		//GCPrivateInfo msg;
		//msg.SetSelf(1);
		//msg.SetAge(pHuman->GetDB()->GetPrivateInfoDB()->m_Age);
		//msg.SetBloodType(pHuman->GetDB()->GetPrivateInfoDB()->m_BloodType);
		//msg.SetCharGUID((UINT)pHuman->GetGUID());
		//msg.SetCityLength( (BYTE)strlen(pHuman->GetDB()->GetPrivateInfoDB()->m_City) );
		//msg.SetCity((CHAR *)&pHuman->GetDB()->GetPrivateInfoDB()->m_City);
		//msg.SetConsella(pHuman->GetDB()->GetPrivateInfoDB()->m_Consella);
		//msg.SetEmailInfoLength( (BYTE)strlen(pHuman->GetDB()->GetPrivateInfoDB()->m_EmailInfo) );
		//msg.SetEmailInfo((CHAR *)&pHuman->GetDB()->GetPrivateInfoDB()->m_EmailInfo);
		//msg.SetInfoType(pHuman->GetDB()->GetPrivateInfoDB()->m_InfoType);
		//msg.SetJobInfoLength( (BYTE)strlen(pHuman->GetDB()->GetPrivateInfoDB()->m_JobInfo) );
		//msg.SetJobInfo((CHAR *)&pHuman->GetDB()->GetPrivateInfoDB()->m_JobInfo);
		//msg.SetLuckWordLength( (BYTE)strlen(pHuman->GetDB()->GetPrivateInfoDB()->m_LuckWord) );
		//msg.SetLuckWord((CHAR *)&pHuman->GetDB()->GetPrivateInfoDB()->m_LuckWord);
		//msg.SetSchoolInfoLength( (BYTE)strlen(pHuman->GetDB()->GetPrivateInfoDB()->m_SchoolInfo) );
		//msg.SetSchoolInfo((CHAR *)&pHuman->GetDB()->GetPrivateInfoDB()->m_SchoolInfo);
		//msg.SetProvince(pHuman->GetDB()->GetPrivateInfoDB()->m_Province);
		//msg.SetSex(pHuman->GetDB()->GetPrivateInfoDB()->m_Sex);
		//msg.SetYearAnimal(pHuman->GetDB()->GetPrivateInfoDB()->m_YearAnimal);

		//pGamePlayer->SendPacket(&msg);
	}
	else
	{
		//���ͱ��˵���Ϣ
		//Obj_Character *pTarget = (Obj_Character*)(pScene->GetObjManager()->GetObj( pPacket->GetPlayerID() ));
		//if(pTarget->GetObjType() != Obj::OBJ_TYPE_HUMAN)
		//{
		//	g_pLog->FastSaveLog( LOG_FILE_1, "CGAskPrivateInfoHandler: %s ask not a human", pHuman->GetName() ) ;
		//	return	PACKET_EXE_CONTINUE;
		//}
		//Obj_Human* pTargetHuman = (Obj_Human*)pTarget;
		//if( pTargetHuman==NULL )
		//{
		//	Assert( pTargetHuman ) ;
		//	return PACKET_EXE_ERROR ;
		//}
		//
		//// ��ͬ��Ӫ�����ò鿴
		//if( pHuman->IsEnemy( pTargetHuman ) )
		//{
		//	g_pLog->FastSaveLog( LOG_FILE_1, "CGAskPrivateInfoHandler: %s cann't ask %s's DetailAttr ", pHuman->GetName(), pTargetHuman->GetName() ) ;
		//	return	PACKET_EXE_CONTINUE;
		//}

		//GCPrivateInfo msg;
		//msg.SetSelf(0);

		////��Ҫ����  GetPrivateInfoDB()->m_InfoType ��������ȷ���Ƿ���ʲô���ӵ�����
		//if( 2 == pTargetHuman->GetDB()->GetPrivateInfoDB()->m_InfoType)  //��ȫ����
		//{
		//	msg.SetLuckWordLength( (BYTE)strlen(pTargetHuman->GetDB()->GetPrivateInfoDB()->m_LuckWord) );
		//	msg.SetLuckWord((CHAR *)&pTargetHuman->GetDB()->GetPrivateInfoDB()->m_LuckWord);
		//	pGamePlayer->SendPacket(&msg);
		//	return PACKET_EXE_CONTINUE ;
		//}
		//if( 1 == pTargetHuman->GetDB()->GetPrivateInfoDB()->m_InfoType )  //���ѹ���
		//{
		//	GUID_t SelfGuid = pHuman->GetGUID();

		//	//Ӧ��ʹ�öԷ���Ϊ��������ж�
		//	if( ! pTargetHuman->GetHumanRelation()->IsFriend(SelfGuid) )
		//	{
		//		msg.SetLuckWordLength( (BYTE)strlen(pTargetHuman->GetDB()->GetPrivateInfoDB()->m_LuckWord) );
		//		msg.SetLuckWord((CHAR *)&pTargetHuman->GetDB()->GetPrivateInfoDB()->m_LuckWord);
		//		pGamePlayer->SendPacket(&msg);
		//		return PACKET_EXE_CONTINUE ;
		//	}
		//}
		////��ȫ����(�����ѹ���)
		//msg.SetAge(pTargetHuman->GetDB()->GetPrivateInfoDB()->m_Age);
		//msg.SetBloodType(pTargetHuman->GetDB()->GetPrivateInfoDB()->m_BloodType);
		//msg.SetCharGUID((UINT)pTargetHuman->GetGUID());
		//msg.SetCityLength( (BYTE)strlen(pTargetHuman->GetDB()->GetPrivateInfoDB()->m_City) );
		//msg.SetCity((CHAR *)&pTargetHuman->GetDB()->GetPrivateInfoDB()->m_City);
		//msg.SetConsella(pTargetHuman->GetDB()->GetPrivateInfoDB()->m_Consella);
		//msg.SetEmailInfoLength( (BYTE)strlen(pTargetHuman->GetDB()->GetPrivateInfoDB()->m_EmailInfo) );
		//msg.SetEmailInfo((CHAR *)&pTargetHuman->GetDB()->GetPrivateInfoDB()->m_EmailInfo);
		//msg.SetInfoType(pHuman->GetDB()->GetPrivateInfoDB()->m_InfoType);
		//msg.SetJobInfoLength( (BYTE)strlen(pTargetHuman->GetDB()->GetPrivateInfoDB()->m_JobInfo) );
		//msg.SetJobInfo((CHAR *)&pTargetHuman->GetDB()->GetPrivateInfoDB()->m_JobInfo);
		//msg.SetLuckWordLength( (BYTE)strlen(pTargetHuman->GetDB()->GetPrivateInfoDB()->m_LuckWord) );
		//msg.SetLuckWord((CHAR *)&pTargetHuman->GetDB()->GetPrivateInfoDB()->m_LuckWord);
		//msg.SetSchoolInfoLength( (BYTE)strlen(pTargetHuman->GetDB()->GetPrivateInfoDB()->m_SchoolInfo) );
		//msg.SetSchoolInfo((CHAR *)&pTargetHuman->GetDB()->GetPrivateInfoDB()->m_SchoolInfo);
		//msg.SetProvince(pTargetHuman->GetDB()->GetPrivateInfoDB()->m_Province);
		//msg.SetSex(pTargetHuman->GetDB()->GetPrivateInfoDB()->m_Sex);
		//msg.SetYearAnimal(pTargetHuman->GetDB()->GetPrivateInfoDB()->m_YearAnimal);

		//pGamePlayer->SendPacket(&msg);
	}

	g_pLog->FastSaveLog( LOG_FILE_1, "CGAskPrivateInfoHandler::ObjName=%s"
		,pHuman->GetName());

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
