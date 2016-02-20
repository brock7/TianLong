#include "stdafx.h"
#include "UIGlobal.h"
#include <TDException.h>
#include "UIWindowMng.h"
#include <CEGUIWindowManager.h>
#include "ceguiwindow.h"
#include <CEGUISystem.h>
#include "TDDataBase.h"
#include "TDDBC_Struct.h"
#include "TDKernel.h"
#include "TDScriptSystem.h"
#include "UIScript.h"
#include "LuaPlus.h"
#include "UILuaControl.h"
#include <OgreMemoryManager.h>
#include "UIString.h"
#include "../CEGUIFalagardEx/FalActionButton.h"
#include "UISystem.h"
#include "TDProfile.h"
#include "../CEGUIFalagardEx/FalSuperTooltip.h"
#include "../CEGUIFalagardEx/FalagardMeshWindow.h"
#include <process.h>
#include "TDObjectSystem.h"
#include "TDVariable.h"
#include "AxTrace.h"
#include "windows.h"
#include "tdeventdefine.h"
CEGUI::Window*  CUIWindowMng::m_pBackGroundSheet = NULL; // �ͻ��˴��ڵװ�
CUIWindowMng*	CUIWindowMng::s_pMe	= NULL;
CUIWindowItem::CUIWindowItem(const _DBC_UI_LAYOUTDEFINE* pLayoutDef)
{
	TDAssert(pLayoutDef);

	m_dwID				= pLayoutDef->nID;
	m_strWindowName		= pLayoutDef->szName;
	m_strLayoutFileName = pLayoutDef->szLayout;
	m_strScriptFileName = pLayoutDef->szScript;
	m_nIsCanDemise		= pLayoutDef->nDemise;


	m_pWindow			= NULL;
	
	m_bShow = FALSE;
	m_bLayoutLoaded = FALSE;
}

CUIWindowItem::~CUIWindowItem()
{
	//�ͷ�Lua����
	g_pScriptSys->DestroyEnvironment(m_pScriptEnv);
	m_pScriptEnv = NULL;

	//�ͷ�luaʵ��
	for(INT i=0; i<(INT)m_vAllControl.size(); i++)
	{
		if(m_vAllControl[i])
		{
			LUA_CONTROL::Window::DestroyControl(m_vAllControl[i]);
			//LUA_CONTROL::Window* pWindow =  m_vAllControl[i];
			//delete pWindow;
			m_vAllControl[i] = NULL;
		}
	}
	m_vAllControl.clear();

	//�ͷ�ceguiʵ��
	CEGUI::WindowManager::getSingleton().destroyWindow( m_pWindow );
	m_pWindow = NULL;
}

bool CUIWindowItem::callbackProperty(CEGUI::Window* window, CEGUI::String& propname, CEGUI::String& propvalue, void* userdata)
{
	if(propname == (CEGUI::utf8*)"Text" && !propvalue.empty())
	{
		//�ַ���ת��
		CEGUI::String strUtf32;

		STRING strMbcs;
		CUIStringSystem::utf8_to_mbcs(STRING(propvalue.c_str()), strMbcs);
		CUIStringSystem::GetMe()->ParserString_Runtime(strMbcs, strUtf32);
		propvalue = strUtf32;
	}

	return true;
}

VOID CUIWindowItem::PreLoadWindow(VOID)
{
	//----------------------------------------------------------------------------
	//�����ű����л���
	m_pScriptEnv = g_pScriptSys->NewEnvironment(m_strWindowName.c_str(), m_strScriptFileName.c_str());

	//---------------------------------------
	//����ִ��
	m_pScriptEnv->DoFile(m_strScriptFileName.c_str());

	//---------------------------------------
	//ע�����ú���

	//ע��"this"����
	LuaObject metaUIWindowItem = g_pScriptSys->GetLuaState()->GetGlobals().GetByName("MetaTable_UIWindowItem");

	LuaObject objThis = g_pScriptSys->GetLuaState()->BoxPointer(this);
	objThis.SetMetaTable(metaUIWindowItem);
	m_pScriptEnv->GetLuaObject()->SetObject("this", objThis);

	//---------------------------------------
	//ִ�����е�***_PreLoad����
	CHAR szTemp[MAX_PATH] = {0};
	_snprintf(szTemp, MAX_PATH, "%s_PreLoad", m_strWindowName.c_str());
	m_pScriptEnv->DoFunction(szTemp);

	m_bLayoutLoaded = FALSE;
}

VOID CUIWindowItem::LoadWindow(VOID)
{
	TDAssert(m_pWindow == NULL);
	if(m_bLayoutLoaded) return;

	//----------------------------------------------------------------------------
	//���ز����ļ�
	m_pWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout(m_strLayoutFileName, "", "", 
		callbackProperty);
	TDAssert(m_pWindow);
	//��ʼ״̬������
	m_pWindow->hide();
	CUIWindowMng::GetClientScreen()->addChildWindow(m_pWindow);

	//ע��ؼ�
	_RegisterControlToScript(m_pWindow);

	//---------------------------------------
	//ִ�����е�***_OnLoad����
	CHAR szTemp[MAX_PATH] = {0};
	_snprintf(szTemp, MAX_PATH, "%s_OnLoad", m_strWindowName.c_str());
	m_pScriptEnv->DoFunction(szTemp);

	m_bLayoutLoaded = TRUE;
}

VOID CUIWindowItem::_RegisterControlToScript(CEGUI::Window* pWindow)
{
	//����UserData,���ڻ�˷����
	pWindow->setUserData(this);

	//Register Me
	LUA_CONTROL::Window* pTempControl = LUA_CONTROL::Window::CreateControl(pWindow);

	LuaObject objThis = g_pScriptSys->GetLuaState()->BoxPointer(pTempControl);
	objThis.SetMetaTable(*(pTempControl->GetMetaTable()));

	CEGUI::String strTemp = pWindow->getName();
	m_pScriptEnv->GetLuaObject()->SetObject(strTemp.c_str(), objThis);
	
	m_vAllControl.push_back(pTempControl);

	//��ActionButton���⴦��
	if(pWindow->testClassName((CEGUI::utf8*)"FalagardActionButton"))
	{
		CEGUI::IFalagardActionButton* pActionButton = (CEGUI::IFalagardActionButton*)(CEGUI::FalagardActionButton*)pWindow;

		//DrawStarted
		pActionButton->subscribeDragDropStartedEvent(CEGUI::Event::Subscriber(&CUISystem::handleActionDragDropStarted, CUISystem::GetMe()));
		//MouseEnter
		pActionButton->subscribeMouseEnterEvent(CEGUI::Event::Subscriber(&CUISystem::handleActionButtonMouseEnter, CUISystem::GetMe()));
		//MouseLeave
		pActionButton->subscribeMouseLeaveEvent(CEGUI::Event::Subscriber(&CUISystem::handleActionButtonMouseLeave, CUISystem::GetMe()));
		//ParentHidden
		pWindow->subscribeEvent((CEGUI::utf8*)"ParentHidden", CEGUI::Event::Subscriber(&CUISystem::handleActionButtonParentHidden, CUISystem::GetMe()));
	}
	else if(pWindow->testClassName((CEGUI::utf8*)"FalagardMeshWindow"))
	{
		CEGUI::IFalagardMeshWindow* pMeshWindow = (CEGUI::IFalagardMeshWindow*)(CEGUI::FalagardMeshWindow*)pWindow;

		pMeshWindow->subscribeShownEvent(CEGUI::Event::Subscriber(&CUISystem::handleMeshWindowShown, CUISystem::GetMe()));
		pMeshWindow->subscribeHidenEvent(CEGUI::Event::Subscriber(&CUISystem::handleMeshWindowHiden, CUISystem::GetMe()));
	}

	//Register Child
	for(INT i=0; i<(INT)pWindow->getChildCount(); i++)
	{
		_RegisterControlToScript(pWindow->getChildAtIdx(i));
	}

	return;
}

VOID CUIWindowItem::_TransWindowText(CEGUI::Window* pWindow)
{
	CEGUI::String str = pWindow->getTextOriginal();
	if(!str.empty())
	{
		STRING strMBCS_ToTrans;
		CUIStringSystem::utf8_to_mbcs(str.c_str(), strMBCS_ToTrans);

		// ����specialת����string���Ѿ���utf32��ʽ��
		CEGUI::String strMBCS_Transed_Done;

		CUIStringSystem::GetMe()->ParserString_Runtime(strMBCS_ToTrans, strMBCS_Transed_Done);

		pWindow->setText(strMBCS_Transed_Done);
	}
	//Register Child
	for(INT i=0; i<(INT)pWindow->getChildCount(); i++)
	{
		_TransWindowText(pWindow->getChildAtIdx(i));
	}
}

VOID CUIWindowItem::Show(BOOL bShow)
{
	if(bShow == m_bShow) return;


	//��ʾ
	if(bShow)
	{
		m_pWindow->show();
		m_pWindow->activate();
	}
	else
	{
		m_pWindow->hide();

		//��Ӧ���ڹر�ʱһЩ���Ӵ���
		onWindowHidden();
	}

	//��ֹ��Ļ��˸
	CUIWindowMng::GetClientScreen()->requestRedraw();

	
	m_bShow = bShow;
	// ���ô���,������ʾ���ǹرմ���,����Ҫ�������ô��ڸ���
}

void CUIWindowItem::onWindowHidden(void)
{
	//�����ǰsupertooltips
}

INT CUIWindowItem::LUA_RegisterEvent(LuaPlus::LuaState* pState)
{
	LuaStack args(pState);
	if (!(args[2].IsString())) return 0;

	if(m_bLayoutLoaded) 
	{
		TDThrow("%s Must register event in \"***PreLoad\" Function ", m_strWindowName.c_str());
	}

	STRING strEventName = args[2].GetString();

	g_pEventSys->RegisterEventHandle(strEventName, _OnGameEvent, (DWORD)(DWORD_PTR)this);
	return 0;
}

INT CUIWindowItem::LUA_Show(LuaPlus::LuaState* pState)
{
	CUIWindowMng::GetMe()->DemiseWindow( this, TRUE );
	Show(TRUE);
	return 0;
}

INT CUIWindowItem::LUA_Hide(LuaPlus::LuaState* pState)
{
	CUIWindowMng::GetMe()->DemiseWindow( this, FALSE );
	Show(FALSE);
	return 0;
}

//��ʾ/����
INT	 CUIWindowItem::LUA_TogleShow(LuaPlus::LuaState* pState)
{
	if(m_bShow) 
	{
		CUIWindowMng::GetMe()->DemiseWindow( this, FALSE );
		Show(FALSE);
	}
	else 
	{
		CUIWindowMng::GetMe()->DemiseWindow( this, TRUE );
		Show(TRUE);
	}
	return 0;
}
	
INT	CUIWindowItem::LUA_IsVisible(LuaPlus::LuaState* pState)
{
	pState->PushBoolean(m_bShow==TRUE);

	return 1;
}

//ת�����д����ϵ�����
INT	CUIWindowItem::LUA_TransAllWindowText(LuaPlus::LuaState* pState)
{
	_TransWindowText(m_pWindow);
	return 0;
}

//������ĳ�߼�Obj�ľ���(�������˻������񴰿ڣ������볬��һ��������Զ��ر�)
INT CUIWindowItem::LUA_CareObject(LuaPlus::LuaState* pState)
{
	LuaStack args(pState);
	if (!(args[2].IsInteger()))
	{
		TDThrow("LUA: CUIWindowItem::LUA_CareObject[2] Wrong Param1");
	}
	if (!(args[3].IsInteger())) 
	{
		TDThrow("LUA: CUIWindowItem::LUA_CareObject[3] Wrong Param1");
	}
	if (!(args[4].IsString()))
	{
		TDThrow("LUA: CUIWindowItem::LUA_CareObject[4] Wrong Param1");
	}
	//Object��ID
	INT nObjID = args[2].GetInteger();

	//�Ƿ����
	INT	bCare = args[3].GetInteger()==1;

	//˭�ڹ���
	std::string str = args[4].GetString();

	g_pObjectSystem->CareObject(nObjID, bCare, str);

	return 0;
}

VOID WINAPI	CUIWindowItem::_OnGameEvent(const EVENT* pEvent, UINT dwOwnerData)
{
	TDAssert(pEvent);
	
	//--------------------------------------------------------
	//�ַ�
	CUIWindowItem* pWinItem = (CUIWindowItem*)(DWORD_PTR)(dwOwnerData);
	if(!pWinItem) return;

	//����
	if(!(pWinItem->m_bLayoutLoaded))
	{
		pWinItem->LoadWindow();
	}

	//--------------------------------------------------------
	//����
	for(INT i=0; i<(INT)pEvent->vArg.size(); i++)
	{
		CHAR szTemp[MAX_PATH];
		_snprintf(szTemp, MAX_PATH, "arg%d", i);
		g_pScriptSys->GetLuaState()->GetGlobals().SetString(szTemp, pEvent->vArg[i].c_str());
	}

	//--------------------------------------------------------
	//���ýű�
	CHAR szFunctionName[MAX_PATH];
	_snprintf(szFunctionName, MAX_PATH, "%s_OnEvent", 
		pWinItem->m_strWindowName.c_str());

	CHAR szFunctionParam[MAX_PATH];
	_snprintf(szFunctionParam, MAX_PATH, "\"%s\"", 
		pEvent->pEventDef->szEvent);

	pWinItem->m_pScriptEnv->DoFunction(szFunctionName, szFunctionParam);
}

VOID CUIWindowItem::FireUIEvent(LPCTSTR szEventHandle, CEGUI::Window* pWindow)
{
	//--------------------------------------------------------
	//����
	g_pScriptSys->GetLuaState()->GetGlobals().SetString("arg0", pWindow->getName().c_str());

	m_pScriptEnv->DoString(szEventHandle);
}
VOID CUIWindowItem::MoveTo( CEGUI::Point&  newPos )
{
	m_pWindow->setPosition( newPos );
}
void CUIWindowItem::PositionSelf()
{
	if( m_pWindow )
	{
		CEGUI::MouseCursor& cursor = CEGUI::MouseCursor::getSingleton();
        CEGUI::Rect screen( CEGUI::System::getSingleton().getRenderer()->getRect());
        CEGUI::Rect tipRect( m_pWindow->getUnclippedPixelRect());
        const CEGUI::Image* mouseImage = cursor.getImage();

        CEGUI::Point mousePos(cursor.getPosition());
        CEGUI::Size mouseSz(0,0);

        if (mouseImage)
        {
            mouseSz = mouseImage->getSize();
        }

        CEGUI::Point tmpPos(mousePos.d_x - tipRect.getWidth() - 5, mousePos.d_y - tipRect.getHeight() - 5);
        tipRect.setPosition(tmpPos);

        // if tooltip would be off the right of the screen,
        // reposition to the other side of the mouse cursor.
        if ( tipRect.d_right < 0 )
        {
            tmpPos.d_x = mousePos.d_x + mouseSz.d_width + 5;
        }

        // if tooltip would be off the bottom of the screen,
        // reposition to the other side of the mouse cursor.
        if ( tipRect.d_bottom < 0 )
        {
            tmpPos.d_y = mousePos.d_y + mouseSz.d_height + 5;
        }

        // set final position of tooltip window.
        m_pWindow->setPosition( CEGUI::Absolute, tmpPos);
	}
}
///////////////////////////////////////////////
// CUIWindowMng
CUIWindowMng::CUIWindowMng(VOID)
{
	s_pMe = this;
}

CUIWindowMng::~CUIWindowMng(VOID)
{
	WINDOWLIST::iterator it;
	for(it=m_aWindowList.begin(); it!=m_aWindowList.end(); it++)
	{
		if(*(it))
		{
			delete (CUIWindowItem*)(*it);
			*it = NULL;
		}
	}
	m_aWindowList.clear();
}

// ��ʼ�����ڹ�����
VOID CUIWindowMng::Init(VOID)
{
	//����uiϵͳ�װ�
//	PROFILE_PUSH(Create_MainBoard);

	m_pBackGroundSheet = CEGUI::WindowManager::getSingleton().createWindow((CEGUI::utf8*)"DefaultGUISheet", (CEGUI::utf8*)"__BACKGOUNDSHEET__");
	m_pBackGroundSheet->setMouseHollow(true);
	m_pBackGroundSheet->moveToFront();
	//m_pBackGroundSheet->captureInput();
	m_pBackGroundSheet->setDistributesCapturedInputs(true);

	CEGUI::System::getSingleton().setGUISheet( m_pBackGroundSheet );

//	PROFILE_POP(Create_MainBoard);

	//����UI
	tDataBaseSystem* pUIDBC = (tDataBaseSystem*)g_pKernel->GetNode("bin\\dbc");
	TDAssert(pUIDBC);
	const tDataBase* pDBC = pUIDBC->GetDataBase(DBC_UI_LAYOUTDEFINE);

	for(INT i=0; i<(INT)pDBC->GetRecordsNum(); i++)
	{
		const _DBC_UI_LAYOUTDEFINE* pLine = (const _DBC_UI_LAYOUTDEFINE*)((tDataBase*)pDBC)->Search_LineNum_EQU(i);

		//����һ���µ�WindowItem
		CUIWindowItem* pItem = new CUIWindowItem(pLine);
		//Ԥ���ش���
		pItem->PreLoadWindow();

		m_aWindowList.push_back( pItem );
	}
}

// �ر����д���
VOID CUIWindowMng::HideAllWindow()
{
	WINDOWLIST::iterator  it;
	for( it = m_aWindowList.begin(); it != m_aWindowList.end(); it ++ )
	{
		(*it)->Show(FALSE);
	}
}

BOOL CUIWindowMng::IsWindowShow(LPCTSTR szUIName)
{
	TDAssert(szUIName);

	WINDOWLIST::iterator  it;
	for( it=m_aWindowList.begin(); it != m_aWindowList.end(); it ++ )
	{
		if(strcmp((*it)->GetWindowName(), szUIName) == 0)
			return (*it)->IsWindowShow();
	}

	return FALSE;
}
void CUIWindowMng::DemiseWindowTest( CUIWindowItem* pItem, BOOL bShow )
{
	if( pItem->IsCanDemise() && bShow )
	{
		switch( pItem->GetDemiseType() )
		{
		case 2:
			{
				WINDOWLIST::iterator it;
				for( it = m_aWindowList.begin(); it != m_aWindowList.end(); it ++ )
				{
					if( (*it)->GetDemiseType() == 2 )  // �ص����е���2�Ĵ���
					{
						// ����򿪣��������Լ��������Ѿ����ˣ��Ͳ�����
						if( bShow && ( (*it) == pItem ) && (*it)->IsWindowShow() )
							return;
						if( (*it)->IsWindowShow() )
							(*it)->Show( FALSE );
					}
				}
			}
			break;
		case 3:
			{
				WINDOWLIST::iterator it;
				for( it = m_aWindowList.begin(); it != m_aWindowList.end(); it ++ )
				{
					if( (*it)->GetDemiseType() == 3 )  // �ص����е���3�Ĵ���
					{
						// ����򿪣��������Լ��������Ѿ����ˣ��Ͳ�����
						if( bShow && ( (*it) == pItem ) && (*it)->IsWindowShow() )
							return;
						if( (*it)->IsWindowShow() )
							(*it)->Show( FALSE );
					}
				}
			}
			break;
		case 4:
		case 6:
			{
				CloseAllWindow();
				break;
			}
		case 5: // ����Ӧ����
			{
				pItem->PositionSelf();
			}
			break;
		default:
			break;
		}
	}
}
// ���ô��ڲ���
VOID CUIWindowMng::DemiseWindow( CUIWindowItem* pItem, BOOL bShow )
{
	DemiseWindowTest( pItem, bShow );
	return;
}

bool CUIWindowMng::CloseAllWindow()
{
	bool bCloseOne = false;
	WINDOWLIST::iterator it;
	for( it = m_aWindowList.begin(); it != m_aWindowList.end(); it ++ )
	{
		if( (*it)->GetDemiseType() != 0 && (*it)->GetDemiseType() != 6 &&  (*it)->IsWindowShow() )  // �ص����еĲ���0��6�Ĵ���
		{
			// ����򿪣��������Լ��������Ѿ����ˣ��Ͳ�����
			if( (*it)->IsWindowShow() )
			{
				(*it)->Show( FALSE );
				bCloseOne = true;
			}
		}
	}

	return bCloseOne;
}

void CUIWindowMng::OnEscape() // ��Ӧesc��
{
	//  [4/6/2006] yangjun
	//if(CloseAllWindow()) return; //�߻��޸Ŀ�ݼ������������Ҫ�رմ��ڣ������о����ˡ�

	// ���û�д�����ʾ�ˣ��Ϳ���ϵͳ�˵�
	g_pEventSys->PushEvent( GE_TOGLE_SYSTEMFRAME );
}