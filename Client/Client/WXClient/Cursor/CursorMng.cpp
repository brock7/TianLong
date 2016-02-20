#include "StdAfx.h"
#include ".\cursormng.h"
#include "..\resource.h"
#include "..\Network\NetManager.h"
#include "..\gamecommand.h"
#include "..\object\Object.h"
#include "..\Interface\GMGameInterface.h"
#include "..\Procedure\GameProcedure.h"
#include "..\Object\ObjectManager.h"
#include "..\Object\Character\Obj_Character.h"
#include "..\Object\Character\Obj_PlayerMySelf.h"
#include "..\Object\TripperObj\GMTripperObj_ItemBox.h"
#include "DB_Struct.h"
#include "..\Global.h"
#include "TDUtil.h"
#include "..\Event\GMEventSystem.h"
#include "TDActionSystem.h"
#include "TDTimeSystem.h"
#include "..\Action\GMActionSystem.h"
#include "..\DataPool\GMDP_CharacterData.h"
#include "..\DataPool\GMDataPool.h"
#include "..\Action\GMActionSystem_Item.h"
#include "CGUseItem.h"
#include "..\Object\Item\Obj_Item_Medicine.h"
#include "..\Action\GMActionSystem_PetSkill.h"
#include "..\interface\GMGameInterface_Script.h"
#include "..\Input\GMInputSystem.h"
#include "CGExchangeApplyI.h"
#include "CGRelation.h"

WX_IMPLEMENT_DYNAMIC(CCursorMng, GETCLASS(tCursorSystem));
CCursorMng* CCursorMng::s_pMe = NULL;
CCursorMng::CCursorMng(VOID)
{
	s_pMe = this;

	m_bShow = TRUE;
	m_nCursorState	= CURSOR_NORMAL;
	m_hUICursor = NULL;
	m_cmdCurrent_Left.Cleanup();
	m_cmdCurrent_Right.Cleanup();

	m_cmdCurrent_Left.m_typeMouse = SCommand_Mouse::MCT_NULL;
	m_cmdCurrent_Right.m_typeMouse = SCommand_Mouse::MCT_NULL;
}

CCursorMng::~CCursorMng(VOID)
{
}

VOID CCursorMng::SetCursor(ENUM_CURSOR_TYPE nType )
{
	m_nCursorState		= nType;
	::PostMessage( g_hMainWnd, WM_SETCURSOR, 0, HTCLIENT );
}

INT CCursorMng::GetCursor_State(VOID)
{
//	if(!m_bShow) return CURSOR_NORMAL;

	return m_nCursorState;
}

HCURSOR CCursorMng::GetCursor(VOID)
{
	if(!m_bShow) return NULL;

	return m_hCursor[m_nCursorState];
}

//��ӦWM_SETCURSOR
VOID CCursorMng::OnSetCursor(VOID)
{
	if(m_bShow)
	{
		if(m_hUICursor)
		{
			::SetCursor(m_hUICursor);
		}
		else
		{
			::SetCursor(GetCursor());
		}
	}
}

VOID CCursorMng::ShowCursor(BOOL bShow)
{
	if(bShow == m_bShow) return;

	m_bShow = bShow;
	::ShowCursor(m_bShow);
}


// ���ǰ����Ϲҽӵ�����
VOID CCursorMng::MouseCommand_Active(SCommand_Mouse& cmd)
{
	switch(cmd.m_typeMouse)
	{
	case SCommand_Mouse::MCT_PLAYER_MOVETO:
		{
			static UINT uPrevMoveTime = 0;
			if(CGameProcedure::s_pTimeSystem->GetTimeNow() > uPrevMoveTime + 100)
			{
				uPrevMoveTime = CGameProcedure::s_pTimeSystem->GetTimeNow();
				CGameProcedure::s_pGameInterface->Player_MoveTo(fVector2(cmd.m_afParam[0], cmd.m_afParam[1]));
	//			AxTrace(0, 0, "Active: MoveTo(Pos=%.1f, %.1f)", (float)cmd.m_afParam[0], (float)cmd.m_afParam[1]);
			}
		}
		break;

	case SCommand_Mouse::MCT_PLAYER_SELECT:
		{
			// 
			CGameProcedure::s_pGameInterface->Object_SelectAsMainTarget((INT)cmd.m_adwParam[0], 1);
			
			
			if(	m_cmdCurrent_Left.m_typeMouse == SCommand_Mouse::MCT_SKILL_DIR ||
				m_cmdCurrent_Left.m_typeMouse == SCommand_Mouse::MCT_SKILL_AREA)
			{
				m_cmdCurrent_Left.m_typeMouse = SCommand_Mouse::MCT_NULL;
				m_cmdCurrent_Left.m_apParam[0] = NULL;
				m_cmdCurrent_Left.m_afParam[1] = NULL;
				//�ָ������
				CActionSystem::GetMe()->SetDefaultAction(CGameProcedure::s_pGameInterface->Skill_GetActive());
			}
//			AxTrace(0, 0, "Active: SelectPlayer(ID=%d)", (int)cmd.m_afParam[0]);
		}
		break;

	case SCommand_Mouse::MCT_SKILL_OBJ:
		{
			//����ѡ��
			CGameProcedure::s_pGameInterface->Object_SelectAsMainTarget((INT)cmd.m_adwParam[1]);

			tActionItem* pItem = (tActionItem*)cmd.m_apParam[0];
			if(!pItem) break;

			//���빥��
			if(pItem->GetType() == AOT_PET_SKILL)
			{
				int nPetSkillID = ((PET_SKILL*)(((CActionItem_PetSkill*)pItem)->GetImpl()))->m_pDefine->m_nID;
				CDataPool::GetMe()->Pet_SendUseSkillMessage(nPetSkillID, 
					INVALID_ID, cmd.m_afParam[1], cmd.m_afParam[2]);
			}
			else
			{
				CGameProcedure::s_pGameInterface->Player_UseSkill(
							((tActionItem*)cmd.m_apParam[0])->GetID(),
							(INT)cmd.m_adwParam[1]);
			}
//			AxTrace(0, 0, "Active: SkillObj(Skill=%d, ID=%d)", (int)cmd.m_afParam[0], (int)cmd.m_adwParam[1]);
		}
		break;

	case SCommand_Mouse::MCT_SKILL_AREA:
		{
			tActionItem* pItem = (tActionItem*)cmd.m_apParam[0];
			if(!pItem) break;

			if(pItem->GetType() == AOT_PET_SKILL)
			{
				int nPetSkillID = ((PET_SKILL*)(((CActionItem_PetSkill*)pItem)->GetImpl()))->m_pDefine->m_nID;
				CDataPool::GetMe()->Pet_SendUseSkillMessage(nPetSkillID, 
					INVALID_ID, cmd.m_afParam[1], cmd.m_afParam[2]);
			}
			else
			{
				CGameProcedure::s_pGameInterface->Player_UseSkill(
							((tActionItem*)cmd.m_apParam[0])->GetID(),
							fVector2(cmd.m_afParam[1], cmd.m_afParam[2]));
			}
//			AxTrace(0, 0, "Active: SkillArea(Skill=%d, Pos=%.1f,%.1f)", 
//				(int)cmd.m_afParam[0], (int)cmd.m_adwParam[1], (int)cmd.m_afParam[2]);

			//�ָ�����Action
			CActionSystem::GetMe()->SetDefaultAction(CGameProcedure::s_pGameInterface->Skill_GetActive());
		}
		break;

	case SCommand_Mouse::MCT_CANCEL_SKILL:
		{
			//�ָ�����Action
			CActionSystem::GetMe()->SetDefaultAction(CGameProcedure::s_pGameInterface->Skill_GetActive());
		}
		break;

	case SCommand_Mouse::MCT_SKILL_DIR:
		{
			TDThrow("Not support any more!");
		}
		break;

	case SCommand_Mouse::MCT_HIT_TRIPPEROBJ:
		{
			CGameProcedure::s_pGameInterface->TripperObj_Active(cmd.m_adwParam[0]);
//			AxTrace(0, 0, "Active: TripObj(ID=%d)", (int)cmd.m_adwParam[0]);
		}
		break;

	case SCommand_Mouse::MCT_SPEAK:
		{
			//����ѡ��
			CGameProcedure::s_pGameInterface->Object_SelectAsMainTarget((INT)cmd.m_adwParam[0]);
			
			//̸��
			CGameProcedure::s_pGameInterface->Player_Speak(cmd.m_adwParam[0]);

//			AxTrace(0, 0, "Active: Speak(ID=%d)", (int)cmd.m_adwParam[0]);
		}
		break;

	case SCommand_Mouse::MCT_CONTEXMENU:
		{
			//����ѡ��
			CGameProcedure::s_pGameInterface->Object_SelectAsMainTarget((INT)cmd.m_adwParam[0], 1);
			
			//��ʾ�Ҽ��˵�
			CGameProcedure::s_pGameInterface->Object_ShowContexMenu(cmd.m_adwParam[0]);
//			AxTrace(0, 0, "Active: ContexMenu(ID=%d)", (int)cmd.m_adwParam[0]);
		}
		break;

	case SCommand_Mouse::MCT_REPAIR:
		{
			//do nothing...
		}
		break;

	case SCommand_Mouse::MCT_CANCLE_REPAIR:
		{
			//ȡ������
			CActionSystem::GetMe()->SetDefaultAction(CGameInterface::GetMe()->Skill_GetActive());
		}
		break;

    
	case SCommand_Mouse::MCT_UI_USE_IDENTIFY:
		{
			// ʹ�ü�������
			
			break;
		}
	case SCommand_Mouse::MCT_UI_USE_CANCEL_IDENTIFY:
		{
			// ȡ��ʹ�ü�������
			CDataPool::GetMe()->Identify_UnLock();
			break;
		}
	case SCommand_Mouse::MCT_UI_USE_EXCHANGE:
		{
			if(TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall())
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�����ڰ�̯����");
				return ;
			}
			CGameProcedure::s_pGameInterface->Object_SelectAsMainTarget((INT)cmd.m_adwParam[0]);
			CObject* pChar = (CObject*) CObjectManager::GetMe()->GetMainTarget();
			if(pChar && g_theKernel.IsKindOf(pChar->GetClass(), GETCLASS(CObject_PlayerOther)))
			{
				//���ж������ǲ��ǺϷ�
				if(pChar)
				{
					const fVector3 pMePos = CObjectManager::GetMe()->GetMySelf()->GetPosition();
					const fVector3 pOtherPos = ((CObject*)pChar)->GetPosition();

					FLOAT fDistance = 
						TDU_GetDist(fVector2(pMePos.x, pMePos.z),
						fVector2(pOtherPos.x, pOtherPos.z));

					if( EXCHANGE_MAX_DISTANCE < fDistance )
					{
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"����̫Զ����������ʧ��");
						return;
					}
				}

				//�������Ѿ���������ҷ��ͽ�������
				if ( ((CObject_Character*)pChar)->CharacterLogic_Get() == CObject_Character::CHARACTER_LOGIC_DEAD )
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"�������Ѿ�������������뽻��");
					return ;
				}

				CGExchangeApplyI msg;
				msg.SetObjID(pChar->GetServerID());
				CNetManager::GetMe()->SendPacket(&msg);
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"���������Ѿ�����");
				CGameProcedure::s_pGameInterface->Skill_CancelAction();
				return ;
			}
		}
		break;
	case SCommand_Mouse::MCT_UI_USE_CANCEL_EXCHANGE:
		{
			CActionSystem::GetMe()->SetDefaultAction(CGameInterface::GetMe()->Skill_GetActive());
			break;
		}
	case SCommand_Mouse::MCT_SALE:
		break;
	case SCommand_Mouse::MCT_CANCEL_SALE:
		{
			CActionSystem::GetMe()->SetDefaultAction(CGameInterface::GetMe()->Skill_GetActive());
			break;
		}
	case SCommand_Mouse::MCT_BUYMULT:
		break;
	case SCommand_Mouse::MCT_CANCEL_BUYMULT:
		{
			CActionSystem::GetMe()->SetDefaultAction(CGameInterface::GetMe()->Skill_GetActive());
			break;
		}
	case SCommand_Mouse::MCT_UI_USE_ADDFRIEND:
		{
			//����ѡ��
			CGRelation Msg;
			Msg.GetRelation()->CleanUp();
			Msg.GetRelation()->m_Type = REQ_ADDFRIEND;
			REQUEST_ADD_RELATION_WITH_GROUP* pFriend = &(Msg.GetRelation()->m_AddRelationWithGroup);
			pFriend->CleanUp();
			CGameProcedure::s_pGameInterface->Object_SelectAsMainTarget((INT)cmd.m_adwParam[0]);
			CObject_Character* pCharObj = (CObject_Character*)CObjectManager::GetMe()->GetMainTarget();
			if( pCharObj == NULL )
				break;
			if( pCharObj == CObjectManager::GetMe()->GetMySelf() ) // ������Լ����Ͳ���
				break;
			if( g_theKernel.IsKindOf( pCharObj->GetClass(), GETCLASS(CObject_PlayerOther) ) ) // ��������
			{
				ENUM_RELATION sCamp = CGameInterface::GetMe()->GetCampType( 
					pCharObj, 
					(CObject*)CObjectManager::GetMe()->GetMySelf() );
				if( sCamp != RELATION_FRIEND ) // �����ͬһ��Ӫ��
				{
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "��ЧĿ��");
					break;
				}
			}
			else
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "��ЧĿ��");
				break;
			}
			pFriend->SetTargetName( pCharObj->GetCharacterData()->Get_Name() );
			pFriend->SetGroup( SCRIPT_SANDBOX::Friend::m_nCurTeam );
			pFriend->SetRelationType( RELATION_TYPE_FRIEND );
			CNetManager::GetMe()->SendPacket( &Msg );
		}
		break;
	case SCommand_Mouse::MCT_UI_USE_CANCEL_ADDFRIEND:
		CActionSystem::GetMe()->SetDefaultAction(CGameInterface::GetMe()->Skill_GetActive());
		break;
	case SCommand_Mouse::MCT_USE_ITEM:
		{
			//ʹ����Ʒ
			CGameProcedure::s_pGameInterface->PacketItem_UserItem((
				tActionItem*)cmd.m_apParam[0], 
				cmd.m_adwParam[1],
				fVector2(cmd.m_afParam[2], cmd.m_afParam[3]));

			//�ָ�����Action
			CActionSystem::GetMe()->SetDefaultAction(CGameProcedure::s_pGameInterface->Skill_GetActive());
		}
		break;

	case SCommand_Mouse::MCT_CANCEL_USE_ITEM:
		{
			//�ָ�����Action
			CActionSystem::GetMe()->SetDefaultAction(CGameProcedure::s_pGameInterface->Skill_GetActive());
		}
		break;

	case SCommand_Mouse::MCT_LEAP:
		{
			//�Ṧ...
			//SkillID_t id = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->GetDefaultFlySkillID();
			//CObjectManager::GetMe()->GetMySelf()->CharacterLogic_GetAI()->PushCommand_Fly(id, cmd.m_afParam[0], cmd.m_afParam[1]);
		}
		break;
	case SCommand_Mouse::MCT_CATCH_PET:
		{
			CGameProcedure::s_pGameInterface->Player_UseSkill( cmd.m_adwParam[0], int( cmd.m_adwParam[1] ) );
		}
		break;
	default:
		AxTrace(0, 0, "Active: ERROR!(%d)", cmd.m_typeMouse);
		break;
	}
}

VOID CCursorMng::Initial( VOID * pHandle)
{
	HINSTANCE hInstance = *( ( HINSTANCE*) pHandle );

	m_hCursor[ CURSOR_WINBASE ]		= ::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_ARROW));
	m_hCursor[ CURSOR_NORMAL ]		= ::LoadCursorFromFile("..\\Data\\Cursor\\Normal.ani");
	m_hCursor[ CURSOR_ATTACK ]		= ::LoadCursorFromFile("..\\Data\\Cursor\\Attack.ani");
	m_hCursor[ CURSOR_AUTORUN ]		= ::LoadCursorFromFile("..\\Data\\Cursor\\AutoRun.ani");
	m_hCursor[ CURSOR_PICKUP ]		= ::LoadCursorFromFile("..\\Data\\Cursor\\PickUp.ani");
	m_hCursor[ CURSOR_UNREACHABLE ] = ::LoadCursorFromFile("..\\Data\\Cursor\\UnReachable.ani");
	m_hCursor[ CURSOR_MINE ]		= ::LoadCursorFromFile("..\\Data\\Cursor\\Mine.ani");
	m_hCursor[ CURSOR_HERBS ]		= ::LoadCursorFromFile("..\\Data\\Cursor\\Herbs.ani");
	m_hCursor[ CURSOR_FISH ]		= ::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_ARROW )); 
	m_hCursor[ CURSOR_SPEAK ]		= ::LoadCursorFromFile("..\\Data\\Cursor\\Speak.ani");
	m_hCursor[ CURSOR_INTERACT ]	= ::LoadCursorFromFile("..\\Data\\Cursor\\Interact.ani");
	m_hCursor[ CURSOR_REPAIR ]		= ::LoadCursorFromFile("..\\Data\\Cursor\\Repair.ani");
	m_hCursor[ CURSOR_HOVER ]		= ::LoadCursorFromFile("..\\Data\\Cursor\\Hover.ani");
	m_hCursor[ CURSOR_IDENTIFY ]	= ::LoadCursorFromFile("..\\Data\\Cursor\\Identify.ani");
	m_hCursor[ CURSOR_ADDFRIEND ]	= ::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_ARROW ));
	m_hCursor[ CURSOR_EXCHANGE ]	= ::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_ARROW));
	m_hCursor[ CURSOR_CATCH_PET ]	= ::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_ARROW));
}

VOID CCursorMng::MouseCommand_Set(BOOL bHoverInUI, CObject* pSelectObj, const fVector3& fvPos, tActionItem* pActiveSkill)
{
	m_cmdCurrent_Left.m_typeMouse	= SCommand_Mouse::MCT_NULL;
	m_cmdCurrent_Right.m_typeMouse	= SCommand_Mouse::MCT_NULL;
	BOOL bCtrlDown = CInputSystem::GetMe()->IsKeyDown(KC_LCONTROL) || CInputSystem::GetMe()->IsKeyDown(KC_RCONTROL);

	//��UI�Ͽ�
	if(bHoverInUI)
	{
		if(!pActiveSkill) return;

		switch(pActiveSkill->GetType())
		{
		//����װ��
		case AOT_MOUSECMD_REPARE:
			{
				//����Ҵ���
				m_cmdCurrent_Left.m_typeMouse  = SCommand_Mouse::MCT_REPAIR;
				//�Ҽ�ȡ��
				m_cmdCurrent_Right.m_typeMouse = SCommand_Mouse::MCT_CANCLE_REPAIR;		
			}
			break;

		//����װ��
		case AOT_MOUSECMD_IDENTIFY:
			{
				//�������
				m_cmdCurrent_Left.m_typeMouse  = SCommand_Mouse::MCT_UI_USE_IDENTIFY;
				//�Ҽ�ȡ��
				m_cmdCurrent_Right.m_typeMouse = SCommand_Mouse::MCT_UI_USE_CANCEL_IDENTIFY;		
			}
			break;

		case AOT_MOUSECMD_ADDFRIEND:
			{
				//�������
				m_cmdCurrent_Left.m_typeMouse  = SCommand_Mouse::MCT_UI_USE_ADDFRIEND;
				//�Ҽ�ȡ��
				m_cmdCurrent_Right.m_typeMouse = SCommand_Mouse::MCT_UI_USE_CANCEL_ADDFRIEND;		
			}
			break;
		case AOT_MOUSECMD_EXCHANGE:
			{
				//�������
				m_cmdCurrent_Left.m_typeMouse  = SCommand_Mouse::MCT_UI_USE_EXCHANGE;
				//�Ҽ�ȡ��
				m_cmdCurrent_Right.m_typeMouse = SCommand_Mouse::MCT_UI_USE_CANCEL_EXCHANGE;		
			}
			break;
		case AOT_MOUSECMD_SALE:
			{
				//�������
				m_cmdCurrent_Left.m_typeMouse  = SCommand_Mouse::MCT_SALE;
				//�Ҽ�ȡ��
				m_cmdCurrent_Right.m_typeMouse = SCommand_Mouse::MCT_CANCEL_SALE;		
			}
			break;
		case AOT_MOUSECMD_BUYMULT:
			{
				//�������
				m_cmdCurrent_Left.m_typeMouse  = SCommand_Mouse::MCT_BUYMULT;
				//�Ҽ�ȡ��
				m_cmdCurrent_Right.m_typeMouse = SCommand_Mouse::MCT_CANCEL_BUYMULT;		
			}
			break;

		default:
			break;
		}
	}
	else
	{

		//������Ӧobj���ָ��
		if ( pSelectObj != NULL )
		{
			pSelectObj->FillMouseCommand_Left( &m_cmdCurrent_Left, pActiveSkill );
			pSelectObj->FillMouseCommand_Right( &m_cmdCurrent_Right, pActiveSkill );
		}

		if ( m_cmdCurrent_Left.m_typeMouse == SCommand_Mouse::MCT_NULL )
		{
			if ( pActiveSkill != NULL )
			{
				switch(pActiveSkill->GetType())
				{
				case AOT_SKILL:
				case AOT_PET_SKILL:
					{
						const void* pSkillImpl = pActiveSkill->GetImpl();
						if(!pSkillImpl)
						{
							m_cmdCurrent_Left.m_typeMouse = bCtrlDown ? SCommand_Mouse::MCT_LEAP : SCommand_Mouse::MCT_PLAYER_MOVETO;
							m_cmdCurrent_Left.m_afParam[0] = fvPos.x;
							m_cmdCurrent_Left.m_afParam[1] = fvPos.z;
							return;
						}

						//ѡ������
						ENUM_SELECT_TYPE typeSel = (ENUM_SELECT_TYPE)( pActiveSkill->GetType()==AOT_SKILL ? 
							((SCLIENT_SKILL*)pSkillImpl)->m_pDefine->m_nSelectType : 
							((PET_SKILL*)pSkillImpl)->m_pDefine->m_nSelectType);


						switch(typeSel)
						{
						//��ѡ��ɫ
						case SELECT_TYPE_CHARACTER:
							{
								m_cmdCurrent_Left.m_typeMouse = bCtrlDown ? SCommand_Mouse::MCT_LEAP : SCommand_Mouse::MCT_PLAYER_MOVETO;
								m_cmdCurrent_Left.m_afParam[0] = fvPos.x;
								m_cmdCurrent_Left.m_afParam[1] = fvPos.z;
							}
							break;

						//��ѡ��Χ
						case SELECT_TYPE_POS:
							{
								//���漼��
								m_cmdCurrent_Left.m_typeMouse = SCommand_Mouse::MCT_SKILL_AREA;
								m_cmdCurrent_Left.m_apParam[0] = (VOID*)pActiveSkill;
								m_cmdCurrent_Left.m_afParam[1] = fvPos.x;
								m_cmdCurrent_Left.m_afParam[2] = fvPos.z;
								FLOAT fRingRange = (FLOAT)( pActiveSkill->GetType()==AOT_SKILL ? 
									((SCLIENT_SKILL*)pSkillImpl)->m_pDefine->m_fDamageRange : 
								((PET_SKILL*)pSkillImpl)->m_pDefine->m_fDamageRange);
								m_cmdCurrent_Left.m_afParam[5] = fRingRange * 100;

								//ȡ������
								m_cmdCurrent_Right.m_typeMouse = SCommand_Mouse::MCT_CANCEL_SKILL;
							}
							break;
						//����
						case SELECT_TYPE_DIR:
							{
								//���漼��
								m_cmdCurrent_Left.m_typeMouse = SCommand_Mouse::MCT_SKILL_DIR;
								m_cmdCurrent_Left.m_apParam[0] = (VOID*)pActiveSkill;
								
								//���㷽��
								
								//����λ��
								fVector3 fvPlayer = CObjectManager::GetMe()->GetMySelf()->GetPosition();
								//���λ��
								fVector3 fvTarget = fvPos;

								m_cmdCurrent_Left.m_afParam[1] = TDU_GetYAngle(
										fVector2(fvPlayer.x, fvPlayer.z),
										fVector2(fvTarget.x, fvTarget.z));
							}
							break;
						case SELECT_TYPE_SELF:
							//ERROR...
							break;
						}
					}
					break;

				case AOT_ITEM:
					{
						if(!pActiveSkill || pActiveSkill->GetType() != AOT_ITEM) break;
						CObject_Item* pItem = (CObject_Item*)(((CActionItem_Item*)pActiveSkill)->GetItemImpl());
						if(!pItem) break;
						//�������ܹ�ʹ�õ���Ʒ
						if(pItem->GetItemClass() != ICLASS_COMITEM) break;

						BOOL bAreaItem = ((CObject_Item_Medicine*)pItem)->IsAreaTargetType();

						m_cmdCurrent_Left.m_typeMouse = SCommand_Mouse::MCT_USE_ITEM;
						m_cmdCurrent_Left.m_apParam[0] = (VOID*)pActiveSkill;
						m_cmdCurrent_Left.m_adwParam[1] = -1;
						m_cmdCurrent_Left.m_afParam[2] = fvPos.x;
						m_cmdCurrent_Left.m_afParam[3] = fvPos.z;
						m_cmdCurrent_Left.m_adwParam[4] = bAreaItem;

						m_cmdCurrent_Right.m_typeMouse = SCommand_Mouse::MCT_CANCEL_USE_ITEM;
					}
					break;

				//����������
				case AOT_MOUSECMD_REPARE:
					{
						//����Ҵ���
						m_cmdCurrent_Left.m_typeMouse  = SCommand_Mouse::MCT_REPAIR;
						//�Ҽ�ȡ��
						m_cmdCurrent_Right.m_typeMouse = SCommand_Mouse::MCT_CANCLE_REPAIR;		
					}
					break;

				//����װ��
				case AOT_MOUSECMD_IDENTIFY:
					{
						//�������
						m_cmdCurrent_Left.m_typeMouse  = SCommand_Mouse::MCT_UI_USE_IDENTIFY;
						//�Ҽ�ȡ��
						m_cmdCurrent_Right.m_typeMouse = SCommand_Mouse::MCT_UI_USE_CANCEL_IDENTIFY;		
					}
					break;
				case AOT_MOUSECMD_ADDFRIEND:
					{
						//�������
						m_cmdCurrent_Left.m_typeMouse  = SCommand_Mouse::MCT_UI_USE_ADDFRIEND;
						//�Ҽ�ȡ��
						m_cmdCurrent_Right.m_typeMouse = SCommand_Mouse::MCT_UI_USE_CANCEL_ADDFRIEND;		
					}
					break;
				case AOT_MOUSECMD_EXCHANGE:
					{
						//�������
						m_cmdCurrent_Left.m_typeMouse  = SCommand_Mouse::MCT_UI_USE_EXCHANGE;
						//�Ҽ�ȡ��
						m_cmdCurrent_Right.m_typeMouse = SCommand_Mouse::MCT_UI_USE_CANCEL_EXCHANGE;		
					}
					break;
				case AOT_MOUSECMD_CATCHPET:
					{
						m_cmdCurrent_Left.m_typeMouse = SCommand_Mouse::MCT_CATCH_PET;
						m_cmdCurrent_Left.m_afParam[0] = fvPos.x;
						m_cmdCurrent_Left.m_afParam[1] = fvPos.z;
					}
					break;
				case AOT_MOUSECMD_SALE:
					{
						//�������
						m_cmdCurrent_Left.m_typeMouse  = SCommand_Mouse::MCT_SALE;
						//�Ҽ�ȡ��
						m_cmdCurrent_Right.m_typeMouse = SCommand_Mouse::MCT_CANCEL_SALE;		
					}
					break;
				case AOT_MOUSECMD_BUYMULT:
					{
						//���
						m_cmdCurrent_Left.m_typeMouse  = SCommand_Mouse::MCT_BUYMULT;
						//�Ҽ�
						m_cmdCurrent_Right.m_typeMouse = SCommand_Mouse::MCT_CANCEL_BUYMULT;		
					}
					break;
				default:
					//�ƶ�
					{
						m_cmdCurrent_Left.m_typeMouse = SCommand_Mouse::MCT_PLAYER_MOVETO;
						m_cmdCurrent_Left.m_afParam[0] = fvPos.x;
						m_cmdCurrent_Left.m_afParam[1] = fvPos.z;
					}
					break;
				}
			}
		}

		//ѡ�����
		if ( m_cmdCurrent_Right.m_typeMouse == SCommand_Mouse::MCT_NULL )
		{
			m_cmdCurrent_Right.m_typeMouse = SCommand_Mouse::MCT_PLAYER_SELECT;
			m_cmdCurrent_Right.m_anParam[0] = INVALID_ID;
		}
	}
}

