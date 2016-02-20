#include "StdAfx.h"
#include "GamePro_Enter.h"
#include "..\Network\NetManager.h"
#include "..\DataPool\GMUIDataPool.h"
#include "..\DataPool\DataPoolCommandDef.h"
#include "..\GameCommand.h"
#include "..\World\WorldManager.h"
#include "..\WxRender\RenderSystem.h"
#include "..\Object\ObjectManager.h"
#include "..\Object\Character\Obj_PlayerMySelf.h"
#include "CGConnect.h"
#include "CGEnterScene.h"
#include "..\Global.h"
#include "TDVariable.h"
#include "TDEventDefine.h"
#include "GamePro_Login.h"

CGamePro_Enter::CGamePro_Enter()
{
	m_Status = ENTERSCENE_READY;
	m_dwEnterType = ENTER_TYPE_FIRST;
}

CGamePro_Enter::~CGamePro_Enter()
{
}

VOID CGamePro_Enter::Init(VOID)
{
	((CObjectManager*)(s_pObjectManager))->SetLoadNPCDirect(TRUE);
}

VOID CGamePro_Enter::Tick(VOID)
{
	CGameProcedure::Tick();
	switch(GetStatus())
	{

	case ENTERSCENE_CONNECTING:
		{
			//�ȴ��������Ļ�Ӧ
			SetStatus(ENTERSCENE_READY);
			break;
		}
	//���ͽ��볡��������
	case ENTERSCENE_READY:
		{
			INT nSceneID = s_pVariableSystem->GetAs_Int("Scene_ID");

			//���ͽ��볡��������
			CGEnterScene msg;
			msg.setEnterType((BYTE)m_dwEnterType);
			msg.setSceneID(nSceneID);
			fVector2 fvEnterPos = s_pVariableSystem->GetAs_Vector2("Scene_EnterPos");
			msg.setEnterPos(WORLD_POS(fvEnterPos.x, fvEnterPos.y));
			
			CNetManager::GetMe()->SendPacket(&msg);

			//�ȴ��������Ļ�Ӧ
			SetStatus(ENTERSCENE_REQUESTING);
		}
		break;

	//���ͽ��볡��������,�ȴ��������Ļ�Ӧ...
	case ENTERSCENE_REQUESTING:
		break;

	//������������볡��
	case ENTERSCENE_OK:
		{
			// �ر�ϵͳ����
			CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SYSTEM_INFO);
			// �ر��ʺ��������
			CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_COUNT_INPUT);
			// �ر�����ѡ�����
			CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SELECT_CHARACTOR);
			//���ó��������
			CGameProcedure::s_pGfxSystem->Camera_SetCurrent(CRenderSystem::SCENE);
			
			EnterScene();
		}
		break;

	//��������������볡��
	case ENTERSCENE_FAILED:
		{
			CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO_CLOSE_NET, "��������������볡��");	
		}
		break;
	}
}

VOID CGamePro_Enter::EnterScene(void)
{
	//------------------------------------------------------------------
	//���볡��
	/*
	|
	|	�����ǰ���Ѿ�Load�ó��������Ȼ��˳������ٳ��������е�Object����������
	|
	*/
	if(!(s_pWorldManager->EnterScene(m_nSceneID, m_nCityLevel)))
	{
		return;
	}

	//------------------------------------------------------------------
	//�����������
	INT idMySelfServer = (INT)(CGameProcedure::s_pVariableSystem->GetAs_Int("MySelf_ID"));
	CObject_PlayerMySelf* pMySelf = CObjectManager::GetMe()->GetMySelf();
	if(!pMySelf)
	{
		pMySelf = (CObject_PlayerMySelf*)CObjectManager::GetMe()->NewObject(_T("CObject_PlayerMySelf"), idMySelfServer);
	}

	//------------------------------------------------------------------
	//���������������

	//����ServerID
	CObjectManager::GetMe()->SetObjectServerID( pMySelf->GetID(), idMySelfServer );
	//���� GUID
	pMySelf->SetServerGUID((INT)(CGameProcedure::s_pVariableSystem->GetAs_Int("User_GUID")));

	//���ó�ʼλ��
	fVector2 fvMapPos = CGameProcedure::s_pVariableSystem->GetAs_Vector2("MySelf_Pos");
	fVector3 fvGame;
	CGameProcedure::s_pGfxSystem->Axis_Trans(
		CRenderSystem::AX_PLAN, fVector3(fvMapPos.x, 0.0f, fvMapPos.y),
		CRenderSystem::AX_GAME, fvGame);

	SObjectInit initPlayerMySelf;
	initPlayerMySelf.m_fvPos = fvGame;
	CObjectManager::GetMe()->GetMySelf()->Initial(&initPlayerMySelf);

	//��������λ�ã���ֹ��������������
	CObjectManager::GetMe()->GetMySelf()->SetMapPosition(fvMapPos);

	//ͬ�������
	CGameProcedure::s_pGfxSystem->Camera_SetLookAt(
		CObjectManager::GetMe()->GetMySelf()->GetPosition());

	//�����¼�
	s_pEventSystem->PushEvent(GE_PLAYER_ENTER_WORLD);

	// ����ѡ�����
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SELECT_CHARACTOR);

	// ��������
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_BACK_GROUND);
	
	//ת����ѭ��
	CGameProcedure::SetActiveProc((CGameProcedure*)CGameProcedure::s_pProcMain);

}

VOID CGamePro_Enter::Render(VOID)
{
	if(s_pGfxSystem) s_pGfxSystem->RenderFrame();
}

VOID CGamePro_Enter::Release(VOID)
{
	((CObjectManager*)(s_pObjectManager))->SetLoadNPCDirect(FALSE);
}

VOID CGamePro_Enter::MouseInput(VOID)
{

}
