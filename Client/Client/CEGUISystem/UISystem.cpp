#include "stdafx.h"
#include "UISystem.h"
//#include <OgreRoot.h>//�Ƶ���UISystem.h��
#include <CEGUISystem.h>
#include <OgreCEGUIRenderer.h>
#include <TDException.h>
#include <CEGUISchemeManager.h>
#include <CEGUIWindowManager.h>
#include <CEGUIFontManager.h>
#include "ceguiwindow.h"
#include "uiwindowmng.h"
#include "TDKernel.h"
#include <CEGUIScriptModule.h>
#include "UIScript.h"
#include "TDScriptSystem.h"
#include "UIIconsMng.h"
#include <CEGUIImageset.h>
#include <OgreCEGUITexture.h>
#include <OgreTexture.h>
#include <OgreHardwarePixelBuffer.h>
#include <OgreTextureManager.h>
#include "ceguianimatemanager.h"
#include "ogretimer.h"
#include "UIString.h"
#include "TDInputSystem.h"
#include "TDGfxSystem.h"
#include "..\WxRender\RenderSystem.h"
#include "TDCursorSystem.h"
#include "../CEGUIFalagardEx/FalActionButton.h"
#include "TDActionSystem.h"
#include <CEGUIEventArgs.h>
#include "UICreatureBoardSystem.h"
#include "TDTimeSystem.h"
#include "TDProfile.h"
#include "TDEventDefine.h"
#include "TDFakeObjSystem.h"
#include "elements\CEGUIStaticImage.h"
#include "CEGUIIme.h"
#include "AxTrace.h"
#include "windows.h"
#include "TDGameInterface.h"
#include <CEGUIHyperLinkManager.h>
#include "UIString.h"
#include "TDVariable.h"
#include "TDWorldSystem.h"

WX_IMPLEMENT_DYNAMIC(CUISystem, GETCLASS(tUISystem));
CUISystem* CUISystem::s_pMe = NULL;
CUISystem::CUISystem(VOID)
{
	s_pMe = this;	
	//m_pBtnInDragging = NULL;
	m_szNameBtnInDragging = "";
	m_hDraggingCursor = NULL;
	m_bIsDragEnd = false;
	m_bActivePaoPao = true;
}

CUISystem::~CUISystem(VOID)
{
}

VOID CUISystem::InjectInput(VOID)
{
	CEGUI::System& ceguiSystem = CEGUI::System::getSingleton();
	InputEventQueue& queueInput = g_pInputSys->GetInputQueue();
	POINT pt = g_pInputSys->MouseGetPos();

	for(INT i=0; i<(INT)queueInput.size(); i++)
	{
		InputEvent& event = queueInput[i];
		if(event.bProcessed) continue;	//�Ѿ���������¼�

		switch(event.nEvent)
		{
		//---------------------------------------------------
		//���
		case EVENT_MOUSE_MOVE:
		case EVENT_MOUSE_LDRAG_MOVE:
		case EVENT_MOUSE_RDRAG_MOVE:
			{
				POINT pt = g_pInputSys->MouseGetPos();	
				DWORD dwMouseFlag = g_pInputSys->MouseGetFlag();
				if(ceguiSystem.injectMousePosition((FLOAT)pt.x, (FLOAT)pt.y))
					event.bProcessed = true;
			}
			break;

		case EVENT_MOUSE_LBDOWN:
			{
				//������϶���,�����϶�����
				if(!m_szNameBtnInDragging.empty())
				{
					CEGUI::FalagardActionButton *pBtnInDragging = (CEGUI::FalagardActionButton *)CEGUI::WindowManager::getSingleton().getWindow(m_szNameBtnInDragging);
					//if(m_pBtnInDragging)
					if(pBtnInDragging)
					{
						CEGUI::Window* pWindow = ceguiSystem.getTargetWindow(CEGUI::Point((FLOAT)pt.x, (FLOAT)pt.y));
						if( pWindow )
						{
							OnDragEnd(pWindow);
							event.bProcessed = true;
						}
					}
				}
				else
				{
					if(ceguiSystem.injectMouseButtonDown( CEGUI::LeftButton ))
						event.bProcessed = true;
				}
			}
			break;

		case EVENT_MOUSE_LBUP:
			{
				if(CEGUI::Window::getCaptureWindow())
				{
					if(ceguiSystem.injectMouseButtonUp( CEGUI::LeftButton ))
					{
						event.bProcessed = true;
						//���������Click�¼�
						for(INT j=0; j<(INT)queueInput.size(); j++) 
						{
							if(queueInput[j].nEvent == EVENT_MOUSE_LBCLICK || queueInput[j].nEvent == EVENT_MOUSE_LBUP) //
								queueInput[j].bProcessed = true;
						}
					}
				}
				if(!m_szNameBtnInDragging.empty())
				{
					CEGUI::FalagardActionButton *pBtnInDragging = (CEGUI::FalagardActionButton *)CEGUI::WindowManager::getSingleton().getWindow(m_szNameBtnInDragging);
					if(pBtnInDragging) // ��������϶�
					{
						CEGUI::Window* pWindow = ceguiSystem.getTargetWindow(CEGUI::Point((FLOAT)pt.x, (FLOAT)pt.y));
						if( pWindow && pWindow->testClassName((CEGUI::utf8*)"FalagardActionButton") ) // ������ܵ���ActionButton
						{
							OnDragEnd(pWindow);
							event.bProcessed = true;
							break;
						}
					}
				}
			}
			break;
		case EVENT_MOUSE_LBCLICK:
			{
				if(ceguiSystem.injectMouseButtonUp( CEGUI::LeftButton ))
				{
					event.bProcessed = true;
					//���������Click�¼�
					for(INT j=0; j<(INT)queueInput.size(); j++) 
					{
						if(queueInput[j].nEvent == EVENT_MOUSE_LBCLICK || queueInput[j].nEvent == EVENT_MOUSE_LBUP) //
							queueInput[j].bProcessed = true;
					}
				}
			}
			break;

		case EVENT_MOUSE_RBDOWN:
			{
				if(ceguiSystem.injectMouseButtonDown( CEGUI::RightButton ))
				{
					event.bProcessed = true;
				}
			}
			break;

		case EVENT_MOUSE_RBCLICK:
			{
                if(ceguiSystem.injectMouseButtonUp( CEGUI::RightButton ))
				{
					event.bProcessed = true;

					//���������Click�¼�
					for(INT j=0; j<(INT)queueInput.size(); j++) 
					{
						if(queueInput[j].nEvent == EVENT_MOUSE_RBCLICK || queueInput[j].nEvent == EVENT_MOUSE_RBUP)
							queueInput[j].bProcessed = true;
					}
				}

				//�Ҽ�������ָ���ȱʡ���ܲ����������˳������������������������״̬
				g_pGameInterface->Skill_CancelAction();
			}
			break;

		case EVENT_MOUSE_RBUP:
			{
				if(ceguiSystem.injectMouseButtonUp( CEGUI::RightButton ))
				{
					event.bProcessed = true;
				}
			}
			break;

		case EVENT_MOUSE_LBDOWNREPEAT:
			{
			}
			break;

		default:
			continue;
		}

		if(!event.bProcessed)
		{
			//����δ��Handle��Keybord Event������λ���ж��Ƿ�Ӧ�ò���
			if(event.nEvent == EVENT_KEY_DOWN || event.nEvent == EVENT_KEY_UP)
			{
			}
			else
			//����δ��Handle��Mouse Event������λ���ж��Ƿ�Ӧ�ò���
			{
				CEGUI::Window* pWindow = ceguiSystem.getTargetWindow(CEGUI::Point((FLOAT)pt.x, (FLOAT)pt.y));

				if(	event.nEvent == EVENT_MOUSE_LBDOWN || event.nEvent == EVENT_MOUSE_LBUP ||
					event.nEvent == EVENT_MOUSE_LBCLICK || event.nEvent == EVENT_MOUSE_LDBCLICK ||
					event.nEvent == EVENT_MOUSE_LBDOWNREPEAT)
				{
					if(pWindow && !(pWindow->getMouseLButtonHollow()))
						event.bProcessed = true;
					if(pWindow && pWindow->testClassName((const CEGUI::utf8*)"FalagardChatBoard"))
						event.bProcessed = false;
				}
				else if(event.nEvent == EVENT_MOUSE_RBDOWN || event.nEvent == EVENT_MOUSE_RBUP ||
					event.nEvent == EVENT_MOUSE_RBCLICK || event.nEvent == EVENT_MOUSE_RDBCLICK)
				{
					if(pWindow && !(pWindow->getMouseRButtonHollow()))
						event.bProcessed = true;
				}
				else if(pWindow && !(pWindow->getMouseHollow()))
				{
					event.bProcessed = true;
				}
			}
		}
	}
}

BOOL CUISystem::MessageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return Ime::ImeMsgProc(message, wParam, lParam);
}

BOOL CUISystem::IsMouseHover(VOID)
{
	if(m_szNameBtnInDragging != "")
	{
		CEGUI::FalagardActionButton *pBtnInDragging = (CEGUI::FalagardActionButton *)CEGUI::WindowManager::getSingleton().getWindow(m_szNameBtnInDragging);
		//if(m_pBtnInDragging) 
		if(pBtnInDragging) 
			return true;
	}

	POINT pt = g_pInputSys->MouseGetPos();

	CEGUI::System& ceguiSystem = CEGUI::System::getSingleton();
	CEGUI::Window* pWindow = ceguiSystem.getTargetWindow(CEGUI::Point((FLOAT)pt.x, (FLOAT)pt.y), 0);
	if(pWindow && !(pWindow->getMouseHollow() || pWindow->getMouseLButtonHollow()))
	{
		if(pWindow->testClassName((CEGUI::utf8*)"FalagardChatBoard") && getChatBoard_HyplinkContex(pt.x,pt.y).empty())
		{
			return false;
		}

		return true;
	}

	return false;
}

BOOL CUISystem::IsMouseHover_ChatBoard(LONG x, LONG y)
{
	CEGUI::System& ceguiSystem = CEGUI::System::getSingleton();
	CEGUI::Window* pWindow = ceguiSystem.getTargetWindow(CEGUI::Point((FLOAT)x, (FLOAT)y));

	const char *name = pWindow->getName().c_str();
	if(pWindow && pWindow->testClassName((CEGUI::utf8*)"FalagardChatBoard"))
		return TRUE;
	else
		return FALSE;
}

STRING CUISystem::getChatBoard_HyplinkContex(LONG x, LONG y)
{
	if(IsMouseHover_ChatBoard(x,y))
	{
		CEGUI::System& ceguiSystem = CEGUI::System::getSingleton();
		CEGUI::Window* pWindow = ceguiSystem.getTargetWindow(CEGUI::Point((FLOAT)x, (FLOAT)y));

		CEGUI::String strUtf = CEGUI::HyperLinkManager::getSingleton().getHyperLinkContex(pWindow, CEGUI::Point((FLOAT)x, (FLOAT)y));
		STRING strMbc;
		CUIStringSystem::utf8_to_mbcs(strUtf.c_str(), strMbc);

		return strMbc;
	}

	return STRING("");
}

VOID CUISystem::Initial(VOID*)
{
	//----------------------------------------------------------------------
	//����ȫ�ֱ���
	g_pScriptSys = (tScriptSystem*)g_pKernel->GetNode("bin\\script");
	TDAssert(g_pScriptSys);

	g_pEventSys = (tEventSystem*)g_pKernel->GetNode("bin\\event");
	TDAssert(g_pEventSys);

	g_pActionSys = (tActionSystem*)g_pKernel->GetNode("bin\\action");
	TDAssert(g_pActionSys);

	g_pInputSys = (tInputSystem*)g_pKernel->GetNode("bin\\input");
	TDAssert(g_pInputSys);

	g_pGfxSystem = (CRenderSystem*)g_pKernel->GetNode("bin\\gfx");
	TDAssert(g_pGfxSystem);

	g_pObjectSystem = (tObjectSystem*)g_pKernel->GetNode("bin\\objman");
	TDAssert(g_pObjectSystem);

	g_pCursorSystem = (tCursorSystem*)g_pKernel->GetNode("bin\\cursor");
	TDAssert(g_pCursorSystem);

	g_pDebuger = (tDebuger*)g_pKernel->GetNode("bin\\debuger");
	//TDAssert(g_pDebuger);

	g_pWorldSystem = ( tWorldSystem* ) g_pKernel->GetNode("bin\\worldman");
	TDAssert( g_pWorldSystem );

	g_pTimer = (tTimeSystem*)g_pKernel->GetNode("bin\\time");
	TDAssert(g_pTimer);

	g_pFakeSystem = (tFakeObjSystem*)g_pKernel->GetNode("bin\\fake");
	TDAssert(g_pFakeSystem);

	g_pVariableSystem = (tVariableSystem*)g_pKernel->GetNode("bin\\var");
	TDAssert(g_pVariableSystem);

	g_pGameInterface = (tGameInterfaceBase*)g_pKernel->GetNode("bin\\interface");
	TDAssert(g_pVariableSystem);

	g_pDataBaseSystem = (tDataBaseSystem*)g_pKernel->GetNode("bin\\dbc");
	TDAssert(g_pDataBaseSystem);

	g_hMainWnd = g_pGfxSystem->GetWindow();

	//----------------------------------------------------------------------
	//��ʼ��CEGUI
	m_pOgreRoot = Ogre::Root::getSingletonPtr();
	TDAssert(m_pOgreRoot);

	Ogre::RenderWindow* pWindow = (Ogre::RenderWindow*)m_pOgreRoot->getRenderTarget("");

	m_pCEGUIScript = new CEGUI::CGameUIScript;
	m_pCEGUIRender = new CEGUI::OgreCEGUIRenderer((Ogre::RenderTarget*)pWindow, 
		Ogre::RENDER_QUEUE_OVERLAY, false, 0, m_pOgreRoot->getSceneManager("FairyMainScene"));

	m_pCEGUISystem = new CEGUI::System( m_pCEGUIRender, m_pCEGUIScript, (const CEGUI::utf8*)"");

	//ʹ��UI��Դ��
	m_pCEGUISystem->getResourceProvider()->setDefaultResourceGroup("General");
	
	//�����ܿ��ļ�
	CEGUI::SchemeManager::getSingleton().loadScheme("Schema\\WoWLookSkin.scheme.xml");

	m_pCEGUISystem->setTooltip("WoWLook/Tooltip");

	//----------------------------------------------------------------------
	//�ҽӳ����ӹ�����
	CEGUI::HyperLinkManager::getSingleton().subscribeEvent("HyperLinkActive",CEGUI::Event::Subscriber(&CUISystem::handleHyperLinkActive, CUISystem::GetMe()));
	CEGUI::HyperLinkManager::getSingleton().subscribeEvent("HyperLinkLeftActive",CEGUI::Event::Subscriber(&CUISystem::handleHyperLinkLeftActive, CUISystem::GetMe()));
	CEGUI::HyperLinkManager::getSingleton().subscribeEvent("HyperLinkRightActive",CEGUI::Event::Subscriber(&CUISystem::handleHyperLinkRightActive, CUISystem::GetMe()));
	CEGUI::HyperLinkManager::getSingleton().subscribeEvent("HyperLinkInactive",CEGUI::Event::Subscriber(&CUISystem::handleHyperLinkInactive, CUISystem::GetMe()));

	//----------------------------------------------------------------------
	//��ʼ������������
	new CEGUI::AnimateManager( "schema\\wowanimate.xml" );

	//���������ʼ��
	Ime::InitializeImm((unsigned int)g_hMainWnd);
	
	//----------------------------------------------------------------------
	//�ı�������
	m_pStringSystem = new CUIStringSystem;
	m_pStringSystem->Init();

	//----------------------------------------------------------------------
	//��ʼ��ͼ�������
	m_pIconsMng = new CUIIconsManager;
	TDAssert( m_pIconsMng);
	m_pIconsMng->Initial();

	//----------------------------------------------------------------------
	//��ʼ�����ֹ�����
	m_pWindowMng = new CUIWindowMng;
	TDAssert( m_pWindowMng );
	m_pWindowMng->Init();
	//----------------------------------------------------------------------

	//��ʼ����ɫ��Ϣ�������
	m_pCreatureBoardSystem = new CUICreatureBoardSystem;
	m_pCreatureBoardSystem->Initial();

	//----------------------------------------------------------------------
	//�϶�����
	//m_pBtnInDragging = NULL;
	m_szNameBtnInDragging= "";

	/*RECT rect;
	GetClientRect(g_hMainWnd, &rect);
	g_pEventSys->PushEvent(GE_CHAT_ADJUST_MOVE_CTL, rect.right-rect.left, rect.bottom-rect.top);*/

}

VOID CUISystem::Release(VOID)
{
	if(m_pCreatureBoardSystem)
		m_pCreatureBoardSystem->Release();
	SAFE_DELETE( m_pCreatureBoardSystem );
	SAFE_DELETE( m_pIconsMng );
	SAFE_DELETE( m_pWindowMng );
	SAFE_DELETE( m_pStringSystem );

	delete CEGUI::AnimateManager::getSingletonPtr();

	Ime::UninitializeImm();

	SAFE_DELETE( m_pCEGUISystem );
	SAFE_DELETE( m_pCEGUIRender );
	SAFE_DELETE( m_pCEGUIScript );
}

VOID CUISystem::Tick(VOID)
{
	static UINT s_nLastTime = 0;

	UINT nTimeNow = g_pTimer->GetTimeNow();

	if(s_nLastTime == 0)
	{
		s_nLastTime = nTimeNow;
		return;
	}
    
	if( CEGUI::System::getSingletonPtr() )
		CEGUI::System::getSingleton().injectTimePulse(g_pTimer->CalSubTime(s_nLastTime, nTimeNow)/1000.0f); 

	s_nLastTime = nTimeNow;

	if( m_pCreatureBoardSystem )
		m_pCreatureBoardSystem->Tick();

	static UINT s_nLastFreeTime = g_pTimer->GetTimeNow();
	if( g_pTimer->CalSubTime( s_nLastFreeTime, nTimeNow ) > 6000 ) // ÿ�����ͷ�һ��
	{
		CEGUI::ImagesetManager::getSingleton().FreeUnuseImageset();
		s_nLastFreeTime = nTimeNow;
	}
//	AxVar(0, 0, AX_FLOAT32, "elapsed", &fElapsedTime);
}

/// ���ڴ�С�����ı�
VOID CUISystem::OnSizeChange(UINT message, WPARAM wParam, LPARAM lParam)
{
	//�û������󻯻�����С��
	if(WM_SIZE == message)
	{
		m_pCEGUIRender->_notifySizeChanged();
	}
	//�û��϶����ڱ�Ե����
	else if(WM_EXITSIZEMOVE == message)
	{
		m_pCEGUIRender->_notifySizeChanged();
	}
}

bool CUISystem::handleActionDragDropStarted(const CEGUI::EventArgs &e)
{
	const CEGUI::WindowEventArgs& eWindow = (const CEGUI::WindowEventArgs&)e;
	CEGUI::Window* pWindow = eWindow.window;
	if(!(pWindow->testClassName((CEGUI::utf8*)"FalagardActionButton"))) return false;

	OnDragBeging((CEGUI::FalagardActionButton*)pWindow);

	return false;
}

bool CUISystem::handleActionButtonMouseEnter(const CEGUI::EventArgs &e)
{
	//AxTrace(0,0,"handleActionButtonMouseEnter");
	const CEGUI::WindowEventArgs& eWindow = (const CEGUI::WindowEventArgs&)e;
	CEGUI::Window* pWindow = eWindow.window;
	if(!pWindow || !(pWindow->testClassName((CEGUI::utf8*)"FalagardActionButton"))) return false;

	//�õ��߼�����
	CEGUI::IFalagardActionButton* pIBtn = (CEGUI::IFalagardActionButton*)(CEGUI::FalagardActionButton*)pWindow;
	tActionItem* pActionItem = (tActionItem*)(g_pActionSys->GetAction((INT)(INT_PTR)(pIBtn->getLogicItemData())));
	if(pActionItem)
	{
		//�����߼��ӿ�
		pActionItem->NotifyTooltipsShow();
	}
	return false;
}

bool CUISystem::handleActionButtonMouseLeave(const CEGUI::EventArgs &e)
{
	const CEGUI::WindowEventArgs& eWindow = (const CEGUI::WindowEventArgs&)e;
	CEGUI::Window* pWindow = eWindow.window;
	if(!pWindow || !(pWindow->testClassName((CEGUI::utf8*)"FalagardActionButton"))) return false;

	//�õ��߼�����
	CEGUI::IFalagardActionButton* pIBtn = (CEGUI::IFalagardActionButton*)(CEGUI::FalagardActionButton*)pWindow;
	tActionItem* pActionItem = (tActionItem*)(g_pActionSys->GetAction((INT)(INT_PTR)(pIBtn->getLogicItemData())));
	
	if(pActionItem)
	{
		//�����߼��ӿ�
		pActionItem->NotifyTooltipsHide();
	}
	return false;
}

bool CUISystem::handleActionButtonParentHidden(const CEGUI::EventArgs& e)
{
	const CEGUI::WindowEventArgs& eWindow = (const CEGUI::WindowEventArgs&)e;
	CEGUI::Window* pWindow = eWindow.window;
	if(!pWindow || !(pWindow->testClassName((CEGUI::utf8*)"FalagardActionButton"))) return false;

	//�õ��߼�����
	CEGUI::IFalagardActionButton* pIBtn = (CEGUI::IFalagardActionButton*)(CEGUI::FalagardActionButton*)pWindow;
	tActionItem* pActionItem = (tActionItem*)(g_pActionSys->GetAction((INT)(INT_PTR)(pIBtn->getLogicItemData())));
	
	if(pActionItem)
	{
		//�����߼��ӿ�
		pActionItem->NotifyTooltipsHide();
	}
	return false;
}

bool CUISystem::handleMeshWindowShown(const CEGUI::EventArgs &e)
{
	const CEGUI::WindowEventArgs& eWindow = (const CEGUI::WindowEventArgs&)e;
	CEGUI::Window* pWindow = eWindow.window;
	if(!pWindow || !(pWindow->testClassName((CEGUI::utf8*)"FalagardMeshWindow"))) return false;

	CEGUI::Size sz = pWindow->getAbsoluteSize();
	g_pFakeSystem->OnUIShown(pWindow->getName().c_str(), sz.d_width/sz.d_height);

	return false;
}

bool CUISystem::handleMeshWindowHiden(const CEGUI::EventArgs &e)
{
	const CEGUI::WindowEventArgs& eWindow = (const CEGUI::WindowEventArgs&)e;
	CEGUI::Window* pWindow = eWindow.window;
	if(!pWindow || !(pWindow->testClassName((CEGUI::utf8*)"FalagardMeshWindow"))) return false;

	g_pFakeSystem->OnUIHiden(pWindow->getName().c_str());

	return false;
}

VOID CUISystem::OnDragBeging(CEGUI::FalagardActionButton* pBtnDraging)
{
	CEGUI::IFalagardActionButton* pIBtn = (CEGUI::IFalagardActionButton*)pBtnDraging;
	tActionItem* pActionItem = (tActionItem*)g_pActionSys->GetAction((INT)(pIBtn->getLogicItemData()));
	if(!pActionItem) return;

	//ui��������
	g_pInputSys->SetCapture(ICS_UI);

	//�������Ϊ��Ӧ��Image
	if(m_hDraggingCursor)
	{
		::DestroyIcon(m_hDraggingCursor);
		m_hDraggingCursor = NULL;
	}

	m_hDraggingCursor = CUIIconsManager::GetMe()->CreateAsWinCursor(pActionItem->GetIconName());
	if(m_hDraggingCursor)
	{
		g_pCursorSystem->EnterUICursor(m_hDraggingCursor);
	}

	//����������ק�е�ActionButton
	//m_pBtnInDragging = (CEGUI::FalagardActionButton*)pBtnDraging;
	m_szNameBtnInDragging = ((CEGUI::Window*)pBtnDraging)->getName();
}

VOID CUISystem::OnDragEnd(CEGUI::Window* pWindowTarget)
{	
	//�ͷŲ���
	g_pInputSys->SetCapture(ICS_NONE);
	//�ָ����
	if(m_hDraggingCursor)
	{
		g_pCursorSystem->LeaveUICursor();
		::DestroyIcon(m_hDraggingCursor);
		m_hDraggingCursor = NULL;
	}
	g_pCursorSystem->SetCursor(CURSOR_NORMAL);

	//֪ͨ������ק��ActionButton
	if(m_szNameBtnInDragging != "")
	{
		CEGUI::FalagardActionButton *pBtnInDragging = (CEGUI::FalagardActionButton *)CEGUI::WindowManager::getSingleton().getWindow(m_szNameBtnInDragging);
		CEGUI::IFalagardActionButton* pIBtn = (CEGUI::IFalagardActionButton*)pBtnInDragging;

		if(pBtnInDragging)
		{
			pIBtn->notifyDragingEnd();
		}

		//�ж��Ƿ�����
		BOOL bBeDestroy = (pWindowTarget && pWindowTarget->getMouseHollow());

		//���������һ��actionbutton�ϣ�ȡ�ø�ActionButton��DragAcceptName
		CEGUI::IFalagardActionButton* pITargetBtn = NULL;
		if(pWindowTarget && pWindowTarget->testClassName((CEGUI::utf8*)"FalagardActionButton"))
		{
			pITargetBtn = (CEGUI::IFalagardActionButton*)(CEGUI::FalagardActionButton*)pWindowTarget;
			////pITargetBtn->notifyDragDrop_Droped(m_pBtnInDragging);
			pITargetBtn->notifyDragDrop_Droped(pBtnInDragging);
		}

		//֪ͨ�߼�ϵͳ
		tActionItem* pActionItem = (tActionItem*)g_pActionSys->GetAction((INT)(pIBtn->getLogicItemData()));
		if(pActionItem)
		{
			pActionItem->NotifyDragDropDragged(bBeDestroy, pITargetBtn ? (LPCTSTR)(pITargetBtn->getDragAcceptName()) : NULL, ((CEGUI::FalagardActionButton*)pIBtn)->getDragAcceptName());
		}
		//m_pBtnInDragging = NULL;
		m_szNameBtnInDragging = "";
		ChangeDragFlag(true);
	}

	CEGUI::WindowManager::WindowIterator itAllWindow = CEGUI::WindowManager::getSingleton().getIterator();
	for(itAllWindow.toStart(); !itAllWindow.isAtEnd(); itAllWindow++)
	{
		CEGUI::Window* pWindow = itAllWindow.getCurrentValue();
		if(pWindow && pWindow->isVisible() && pWindow->testClassName((CEGUI::utf8*)"FalagardActionButton"))
		{
			CEGUI::IFalagardActionButton* pIBtn = 
				(CEGUI::IFalagardActionButton*)(CEGUI::FalagardActionButton *)pWindow;

			pIBtn->notifyDragingEnd();
		}
	}

}

tCreatureBoard*	CUISystem::CreateCreatureBoard(VOID)
{
	return (tCreatureBoard*)m_pCreatureBoardSystem->CreateCreatureBoard();
}

VOID CUISystem::AddNewBeHitBoard( bool bDouble, LPCTSTR szInfo, FLOAT fStartX, FLOAT fStartY, int nType, int nMoveType )
{
	m_pCreatureBoardSystem->AddNewBeHit( bDouble, szInfo, fStartX, fStartY, nType, nMoveType );
}

//ĳһ��UI�Ƿ��ڴ���
BOOL CUISystem::IsWindowShow(LPCTSTR szUIName)
{
	return m_pWindowMng->IsWindowShow(szUIName);
}

VOID CUISystem::FakeObj_Show(LPCTSTR szWinName, LPCTSTR szName)
{
	//ȡ��UI����
	CEGUI::Window* pMeshWindow = CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)szWinName);
	if(!pMeshWindow || !(pMeshWindow->testClassName((CEGUI::utf8*)"FalagardMeshWindow"))) return;

	//���չ̶�����������ȡ����Ⱦ����
	STRING strImageName = szName;
	strImageName += FAKE_RENDERTEXTURE;

	//ȡ������������Ӧ������Ⱦ���Ѿ�����
	CEGUI::Texture* rttTexture = 
		((CEGUI::OgreCEGUIRenderer*)m_pCEGUIRender)->createTexture((CEGUI::utf8*)strImageName.c_str());
	TDAssert(rttTexture);

	//����UI�õ�Imageset
	CEGUI::Imageset* rttImageSet = 
		CEGUI::ImagesetManager::getSingleton().createImageset(strImageName.c_str(), rttTexture);
	rttImageSet->setAutoFree( false );
	rttImageSet->defineImage((CEGUI::utf8*)"View", 
		CEGUI::Point(0.0f, 0.0f),
		CEGUI::Size(rttTexture->getWidth(), rttTexture->getHeight()),
		CEGUI::Point(0.0f,0.0f));

	//������ʾ����

	((CEGUI::StaticImage*)pMeshWindow)->setImage(&(rttImageSet->getImage((CEGUI::utf8*)"View")));
}

VOID CUISystem::FakeObj_Close(LPCTSTR szWinName, LPCTSTR szName)
{
	//ȡ��UI����
	CEGUI::Window* pMeshWindow = CEGUI::WindowManager::getSingleton().getWindow((CEGUI::utf8*)szWinName);
	if(!(pMeshWindow->testClassName((CEGUI::utf8*)"FalagardMeshWindow"))) return;

	//֪ͨUI
	((CEGUI::StaticImage*)pMeshWindow)->setImage(0);

	//����UI��Imageset
	STRING strImageName = szName;
	strImageName += FAKE_RENDERTEXTURE;

	CEGUI::ImagesetManager::getSingleton().destroyImageset((CEGUI::utf8*)strImageName.c_str());
}

VOID CUISystem::Debug_SaveFontTexture(VOID)
{
#if 1
	//Make sure have out dir
	::CreateDirectory("DebugOut", NULL);

	CEGUI::FontManager* pFontManager = CEGUI::FontManager::getSingletonPtr();
	CEGUI::FontManager::FontIterator it = pFontManager->getIterator();

	for(it.toStart(); !it.isAtEnd(); it++)
	{
		const CEGUI::String&  strName = it.getCurrentKey();
		CEGUI::FontBase* pFont = it.getCurrentValue();

		//------------------------------------------------------------------
		//Create Directiory
		CHAR szFontOut[MAX_PATH];
		strncpy(szFontOut, ".\\DebugOut", MAX_PATH);
		PathAppend(szFontOut, strName.c_str());
		CreateDirectory(szFontOut, NULL);

		std::vector< CEGUI::Imageset* > vImagesets;
		INT nShouldLen = pFont->debug_GetFontImageset(vImagesets);
		if(nShouldLen != vImagesets.size()) vImagesets.resize(nShouldLen);
		pFont->debug_GetFontImageset(vImagesets);

		//------------------------------------------------------------------
		for(INT i=0; i<(INT)vImagesets.size(); i++)
		{
			const CEGUI::OgreCEGUITexture* pTexture = (const CEGUI::OgreCEGUITexture*)vImagesets[i]->getTexture();
			const Ogre::TexturePtr pOgreTex = pTexture->getOgreTexture();
			const Ogre::HardwarePixelBufferSharedPtr pPixelBuf = pOgreTex->getBuffer(0, 0);
			
			Ogre::Image imageOut;
			imageOut.load("_TEMP_.tga", "General");
//			imageOut.loadDynamicImage(

			imageOut.resize((Ogre::ushort)pPixelBuf->getWidth(), (Ogre::ushort)pPixelBuf->getHeight());
			pPixelBuf->blitToMemory(imageOut.getPixelBox());

			CHAR szFileName[MAX_PATH];
			strncpy(szFileName, szFontOut, MAX_PATH);
			PathAppend(szFileName, vImagesets[i]->getName().c_str());
			strcat(szFileName, ".tga");
			imageOut.save(szFileName);
		}
	}
#endif
}

VOID CUISystem::ChangeDragFlag(bool Flag) 
{
	m_bIsDragEnd = Flag;
}

BOOL CUISystem::IsDragFlag(VOID)
{
	return m_bIsDragEnd;
}

STRING CUISystem::ParserString_VarParam(LPCTSTR strID, ... )
{
	STRING strR = "";
	if(m_pStringSystem)
	{

		va_list ptr;
		
		va_start(ptr, strID);
		strR = m_pStringSystem->ParserString_VarParam(strID, ptr);
		va_end(ptr);
	}

	return strR;
}

VOID CUISystem::ParserString_NoColor(const STRING& strSrc, STRING& strOut, BOOL bControl)
{
	if(m_pStringSystem) return m_pStringSystem->ParserString_NoColor(strSrc, strOut, bControl);
}

STRING CUISystem::ParserString_NoColor_VarParam(LPCTSTR strID, ... )
{
	STRING strR = "δ֪����";

	if(m_pStringSystem)
	{
		va_list ptr;
		va_start(ptr, strID);
		STRING strColor = m_pStringSystem->ParserString_VarParam(strID, ptr);
		va_end(ptr);
		m_pStringSystem->ParserString_NoColor(strColor, strR);
	}

	return strR;
}

STRING CUISystem::ParserString_NoVarParam(LPCTSTR strID)
{
	STRING strR = "";

	if(m_pStringSystem)
	{
		m_pStringSystem->ParserString_NoVarParam(strID);
	}

	return strR;
}

STRING CUISystem::ParserString_NoColor_NoVarParam(LPCTSTR strID)
{
	STRING strR = "";
	if(m_pStringSystem)
	{
		STRING strColor = m_pStringSystem->ParserString_NoVarParam(strID);
		m_pStringSystem->ParserString_NoColor(strColor, strR);
	}
	return strR;
}

BOOL CUISystem::CloseAllWindow()
{
	CUIWindowMng::GetMe()->CloseAllWindow();
	return TRUE;
}

bool CUISystem::handleHyperLinkActive(const CEGUI::EventArgs &e)
{
	AxTrace(0,0,"handleHyperLinkActive");
	return true;
}

bool CUISystem::handleHyperLinkLeftActive(const CEGUI::EventArgs &e)
{
	AxTrace(0,0,"handleHyperLinkLeftActive");
	const CEGUI::HyperLinkEventArgs& eHyperLink = (const CEGUI::HyperLinkEventArgs&)e;

	STRING strWnd, strLink,strtype,strSID,strX,strY;
	CUIStringSystem::utf8_to_mbcs(eHyperLink.windowName.c_str(), strWnd);
	CUIStringSystem::utf8_to_mbcs(eHyperLink.hyperLinkName.c_str(), strLink);
	strtype = strLink;
	//��������
	STRING::size_type nValidEnd = strLink.find("#", 0);
	if(nValidEnd != STRING::npos) 
	{
		STRING::size_type nFinalEnd	= strLink.size();
		strtype = strtype.substr(nValidEnd, nFinalEnd);
		if (strcmp(strtype.c_str(),"#w") == 0) //�ж��Ƿ�ΪѰ·���곬����eg: #aB{sid,x,y##w}(x,y)#aE
		{
			strLink = strLink.substr(0,nValidEnd);
			STRING::size_type nValidEnd = strLink.find(",", 0);
			if(nValidEnd == STRING::npos) 
			{
				return false;
			}
			STRING::size_type nFinalEnd	= strLink.size();
			strSID = strLink.substr(0,nValidEnd);     //����Ŀ�곡��ID
			if (atoi(strSID.c_str()) != g_pWorldSystem->GetActiveSceneID())
			{
				g_pEventSys->PushEvent(GE_NEW_DEBUGMESSAGE,"����û����Ŀ�곡����");
				return false;
			}
			strLink = strLink.substr(nValidEnd+1,nFinalEnd);
			
			nValidEnd = strLink.find(",", 0);
			if(nValidEnd == STRING::npos) 
			{
				return false;
			}
			nFinalEnd	= strLink.size();
			strX = strLink.substr(0,nValidEnd);
			strY = strLink.substr(nValidEnd+1,nFinalEnd);

			g_pGameInterface->Player_MoveTo(fVector2(atoi(strX.c_str()),atoi(strY.c_str())));
		}else if (strcmp(strtype.c_str(),"#z") == 0)  //�ж��Ƿ�Ϊ˵��Ʒ������eg: #aB{playID,itemID##z}name#aE
		{
			if(g_pInputSys->IsKeyDown(KC_LCONTROL))
			{

			}
		}else
		{
			return false;
		}
	}else if(CEGUI::WindowManager::getSingleton().isWindowPresent(eHyperLink.windowName))
	{
		CEGUI::Window* pWindow = CEGUI::WindowManager::getSingleton().getWindow(eHyperLink.windowName);

		if(pWindow->testClassName((CEGUI::utf8*)"FalagardChatBoard"))
		{
			g_pEventSys->PushEvent(GE_CHAT_CHANGE_PRIVATENAME, strLink.c_str());
		}
	}

	return true;
}

bool CUISystem::handleHyperLinkRightActive(const CEGUI::EventArgs &e)
{
	AxTrace(0,0,"handleHyperLinkRightActive");
	const CEGUI::HyperLinkEventArgs& eHyperLink = (const CEGUI::HyperLinkEventArgs&)e;

	STRING strWnd, strLink;
	CUIStringSystem::utf8_to_mbcs(eHyperLink.windowName.c_str(), strWnd);
	CUIStringSystem::utf8_to_mbcs(eHyperLink.hyperLinkName.c_str(), strLink);

	if(CEGUI::WindowManager::getSingleton().isWindowPresent(eHyperLink.windowName))
	{
		CEGUI::Window* pWindow = CEGUI::WindowManager::getSingleton().getWindow(eHyperLink.windowName);
		if(pWindow->testClassName((CEGUI::utf8*)"FalagardChatBoard"))
		{
			if(g_pInputSys->IsKeyDown(KC_LCONTROL))
				g_pEventSys->PushEvent(GE_CHAT_CONTEX_MENU, strLink.c_str());
		}
	}
	return true;
}

bool CUISystem::handleHyperLinkInactive(const CEGUI::EventArgs &e)
{
	AxTrace(0,0,"handleHyperLinkInactive");

	STRING strR = "HelpWindow";
	const CEGUI::HyperLinkEventArgs& eHyperLink = (const CEGUI::HyperLinkEventArgs&)e;

	STRING strUtf8,strUtf9;
	CUIStringSystem::utf8_to_mbcs(eHyperLink.hyperLinkName.c_str(), strUtf9);

	//g_pEventSys->PushEvent(GE_NEW_DEBUGMESSAGE, "mouse on hyperlink.");
	//g_pEventSys->PushEvent(GE_NEW_DEBUGMESSAGE, strUtf9.c_str());
	CUIStringSystem::utf8_to_mbcs(eHyperLink.windowName.c_str(), strUtf8);
	//g_pEventSys->PushEvent(GE_NEW_DEBUGMESSAGE, strUtf8.c_str());

	if(eHyperLink.windowName == "HelpWindow")
	{
	}
	else
	{
		return false;
	}

	return true;
}

BOOL CUISystem::IsIMEEditboxActive(LPCTSTR szWinName)
{
	if(Ime::GetActiveIMEEditBox())
	{
		CEGUI::Window* pWin = Ime::GetActiveIMEEditBox();

		if( 0 == strcmp(pWin->getName().c_str(), szWinName))
			return TRUE;
	}
	return FALSE;
}

BOOL CUISystem::IsIMEEditboxEmpty(VOID)
{
	if(Ime::GetActiveExitBox()->empty())
	{
		return TRUE;
	}
	return FALSE;
}

INT CUISystem::GetCurrentIMEState()
{
	return Ime::GetState();
}

STRING CUISystem::getTalkTemplate(const STRING& strKey, UINT index)
{
	return m_pStringSystem->getTalkTemplate(strKey, index);
}

STRING CUISystem::ModifyChatTemplate(const STRING& strTemp, const STRING& strTalker, const STRING& strTarget)
{
	return m_pStringSystem->ModifyChatTemplate(strTemp,strTalker,strTarget);
}

STRING CUISystem::getTalkRandHelpMsg()
{
	return m_pStringSystem->getTalkRandHelpMsg();
}

void CUISystem::OnPushEcsape()
{
	CUIWindowMng::GetMe()->OnEscape();
}


//�����ַ����еķǷ������ַ� 2006-3-22
BOOL	CUISystem::CheckStringFilter(const STRING& strIn)
{
	if(m_pStringSystem)
	{
		return m_pStringSystem->_CheckStringFilter(strIn);
	}

	return   FALSE;
}
//���˽ض��ַ���<ERROR>
BOOL	CUISystem::CheckStringCode(const STRING& strIn, STRING& strOut) 
{
	if(m_pStringSystem)
	{
		return m_pStringSystem->_CheckStringCode(strIn,strOut);
	}

	return   FALSE;
}

//��ȫƥ�����
BOOL	CUISystem::CheckStringFullCompare(const STRING& strIn, const STRING& strFilterType, BOOL bUseAllTable)
{
	if(m_pStringSystem)
	{
		return m_pStringSystem->_CheckStringFullCompare(strIn, strFilterType, bUseAllTable);
	}

	return   FALSE;
}
INT CUISystem::OgreColor_Lumination( UINT OgreColor )
{
	CEGUI::colour CEGUIColor;

	FLOAT fColor_a,fColor_r,fColor_g,fColor_b;

	fColor_a = (FLOAT)COLOR_A(OgreColor)/ 255.0f;
	fColor_r = (FLOAT)COLOR_R(OgreColor)/ 255.0f;
	fColor_g = (FLOAT)COLOR_G(OgreColor)/ 255.0f;
	fColor_b = (FLOAT)COLOR_B(OgreColor)/ 255.0f;

	CEGUIColor.setAlpha( fColor_a);
	CEGUIColor.setRed(fColor_r);
	CEGUIColor.setGreen(fColor_g);
	CEGUIColor.setBlue(fColor_b);

//	FLOAT xx = CEGUIColor.getLumination();

	return (INT)(CEGUIColor.getLumination() * 240);
}

UINT CUISystem::Lumination_OgreColor( UINT OgreColor, INT Lumination )
{
	CEGUI::colour CEGUIColor;

	
	FLOAT fColor_a,fColor_r,fColor_g,fColor_b,fLumination;

	fColor_a = (FLOAT)COLOR_A(OgreColor)/ 255.0f;
	fColor_r = (FLOAT)COLOR_R(OgreColor)/ 255.0f;
	fColor_g = (FLOAT)COLOR_G(OgreColor)/ 255.0f;
	fColor_b = (FLOAT)COLOR_B(OgreColor)/ 255.0f;
	fLumination = (FLOAT)Lumination / 240.0f;

	CEGUIColor.setAlpha( fColor_a);
	CEGUIColor.setRed( fColor_r );
	CEGUIColor.setGreen( fColor_g );
	CEGUIColor.setBlue( fColor_b );

	if( CEGUIColor.getLumination() != Lumination )
	{
		CEGUIColor.setHSL(CEGUIColor.getHue(),CEGUIColor.getSaturation(), fLumination );
	}
	
	INT Color_a,Color_r,Color_g,Color_b;
	Color_a = CEGUIColor.getAlpha() * 0xFF;
	Color_r = CEGUIColor.getRed() * 0xFF;
	Color_g = CEGUIColor.getGreen() * 0xFF;
	Color_b = CEGUIColor.getBlue() * 0xFF;

	return MAKE_COLOR(Color_r,Color_g,Color_b,Color_a);

}
