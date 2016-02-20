/****************************\
*							*
*	 ���ڽ��ؼ��������		*
*							*
\***************************/

#pragma once
#include <CEGUIWindow.h>
#include "LuaPlus.h"
#include "TDActionSystem.h"
class tScriptSandbox;
class tActionItem;

namespace LUA_CONTROL
{

	//--------------------------------------------------
	//�ؼ�����
	class Window
	{
	public:
		static Window*	CreateControl(CEGUI::Window* pWindow);
		static VOID		DestroyControl(Window* pThis);

	public:
		static VOID		RegisterMetaTable(VOID);
		static VOID		DestroyMetaTable(VOID);

		virtual	LuaPlus::LuaObject*		GetMetaTable(VOID) { return s_pMetaTable; }

	public:
		//����ͨ������
		INT		setProperty(LuaPlus::LuaState* pState);
		//ȡ��ͨ������
		INT		getProperty(LuaPlus::LuaState* pState);

		//�õ���������
		INT		getType(LuaPlus::LuaState* pState)
		{
			pState->PushString(m_pWindow->getWidgetType().c_str());
			return 1;
		}

		//�õ���������
		INT		getName(LuaPlus::LuaState* pState)
		{
			pState->PushString(m_pWindow->getName().c_str());
			return 1;
		}

		//�õ���������
		INT		getText(LuaPlus::LuaState* pState);

		//��������
		INT		setText(LuaPlus::LuaState* pState);
		INT		setTextOriginal(LuaPlus::LuaState* pState);
		INT		TransText(LuaPlus::LuaState* pState);
		INT		setForce(LuaPlus::LuaState* pState);

		//��ʾ
		INT		Show(LuaPlus::LuaState* pState)
		{
			m_pWindow->setVisible(true);
			return 0;
		}
		//����
		INT		Hide(LuaPlus::LuaState* pState);

		//Disable
		INT		Disable(LuaPlus::LuaState* pState)
		{
			m_pWindow->setEnabled(false);
			return 0;
		}

		//Enable
		INT		Enable(LuaPlus::LuaState* pState)
		{
			m_pWindow->setEnabled(true);
			return 0;
		}

		INT		IsVisible( LuaPlus::LuaState* pState )
		{
			pState->PushBoolean( m_pWindow->isVisible() );
			return 1;
		}

		INT		SetToolTip( LuaPlus::LuaState* pState );

		//������̽���
		INT		CaptureInput( LuaPlus::LuaState* pState);

		INT		CenterWindow( LuaPlus::LuaState* pState );
		INT		SetWindowCenter( LuaPlus::LuaState* pState ); 
		INT		SetClippedByParent( LuaPlus::LuaState* pState );
		INT		AutoMousePosition( LuaPlus::LuaState* pState );
	
	public:
		Window(CEGUI::Window* pWindow) { m_pWindow = pWindow; }
		virtual ~Window() { }

	protected:
		CEGUI::Window*		m_pWindow;

		static LuaPlus::LuaObject*	s_pMetaTable;
	};


	//-------------------------------------------------------------
	//Progress Bar
	class ProgressBar : public Window
	{
	public:
		virtual	LuaPlus::LuaObject*		GetMetaTable(VOID) { return s_pMetaTable; }

	public:
		//���ý���
		INT		SetProgress(LuaPlus::LuaState* pState);

	protected:
		friend class Window;
		ProgressBar(CEGUI::Window* pWindow):Window(pWindow) { }
		virtual ~ProgressBar() {}

	protected:
		static LuaPlus::LuaObject*	s_pMetaTable;
	};

	//-------------------------------------------------------------
	//ActionButton
	class ActionButton : public Window, public tActionReference
	{
	public:
		virtual	LuaPlus::LuaObject*		GetMetaTable(VOID) { return s_pMetaTable; }

	public:
		//����ActionItem
		INT		Lua_SetActionItem(LuaPlus::LuaState* pState);
		//����
		INT		Lua_DoAction(LuaPlus::LuaState* pState);
		//����(������,һ���������һ��������)
		INT		Lua_DoSubAction(LuaPlus::LuaState* pState);
		//���밴��״̬
		INT		Lua_SetPushed(LuaPlus::LuaState* pState);
		//������˸״̬
		INT		Lua_SetFlash(LuaPlus::LuaState* pState);
		//�û�
		INT		Lua_Gloom(LuaPlus::LuaState* pState);
		//����
		INT		Lua_Bright(LuaPlus::LuaState* pState);


	public:
		//ActionSystem������
		//�߼�Action�Ѿ�������
		virtual VOID	BeDestroyed(VOID);
		//��ť����
		virtual VOID	SetCheck(BOOL bCheck);
		//��ť���ó�default��״̬
		virtual VOID	SetDefault(BOOL bDefault);
		//������ȴ
		virtual VOID	EnterCoolDown(INT fTime, FLOAT fPercent);
		//������˸
		virtual VOID	EnterFlash(BOOL bFlash);
		//��ʾ����
		virtual VOID	SetCornerChar(CORNER_NUMBER_POS pos, LPCTSTR szChar);
		//���ݸ���
		virtual VOID	Update(INT id);
		//����
		virtual	VOID	Disable(VOID);
		//����
		virtual	VOID	Enable(VOID);
		//���
		virtual	VOID	Gloom(VOID);
		//����
		virtual	VOID	Bright(VOID);
		//����
		virtual	VOID	Locked(VOID);
		//�������
		virtual	VOID	Unlock(VOID);


	protected:
		friend class Window;
		ActionButton(CEGUI::Window* pWindow):Window(pWindow), m_pActionItem(NULL) { }
		virtual ~ActionButton() {}

	protected:
		static LuaPlus::LuaObject*	s_pMetaTable;
		tActionItem*		m_pActionItem;
	};

	//-------------------------------------------------------------
	//PopMenu
	class PopMenu : public Window
	{
	public:
		virtual	LuaPlus::LuaObject*		GetMetaTable(VOID) { return s_pMetaTable; }

	public:
		//����ActionItem
		INT		OpenPopMenu(LuaPlus::LuaState* pState);
		//����
		INT		ClosePopMenu(LuaPlus::LuaState* pState);

		// ����λ��
		INT		SetPopMenuPos(LuaPlus::LuaState* pState);

	protected:
		friend class Window;
		PopMenu(CEGUI::Window* pWindow):Window(pWindow) { }
		virtual ~PopMenu() {}

	protected:
		static LuaPlus::LuaObject*	s_pMetaTable;
	};

	//-------------------------------------------------------------
	//ComplexWindow
	class ComplexWindow : public Window
	{
	public:
		virtual	LuaPlus::LuaObject*		GetMetaTable(VOID) { return s_pMetaTable; }

	public:
		//�������Ԫ��
		INT		AddTextElement(LuaPlus::LuaState* pState);
		//���ѡ��Ԫ��
		INT		AddOptionElement(LuaPlus::LuaState* pState);
		//���ItemԪ��
		INT		AddItemElement(LuaPlus::LuaState* pState);
		//���MoneyԪ��
		INT		AddMoneyElement(LuaPlus::LuaState* pState);
		//���ImpactԪ��
		INT		AddImpactElement(LuaPlus::LuaState* pState);
		//���Action ButtonԪ��
		INT		AddActionElement(LuaPlus::LuaState* pState);
		//�������Ԫ��
		INT		CleanAllElement(LuaPlus::LuaState* pState);
		//�����������ɫ
		INT		SetTextColour(LuaPlus::LuaState* pState);

		INT		PageEnd(LuaPlus::LuaState* pState);
	protected:
		friend class Window;
		ComplexWindow(CEGUI::Window* pWindow):Window(pWindow) { }
		virtual ~ComplexWindow() {}

	protected:
		static LuaPlus::LuaObject*	s_pMetaTable;
	};

	
	class ListBox : public Window
	{
	public:
		virtual	LuaPlus::LuaObject*		GetMetaTable(VOID) { return s_pMetaTable; }
	public:
		virtual INT AddItem( LuaPlus::LuaState* pState );
		virtual INT SetItemTooltip( LuaPlus::LuaState* pState );
		virtual INT SetItemText( LuaPlus::LuaState* pState );
		
		INT DelItem( LuaPlus::LuaState* pState );
		INT GetItem( LuaPlus::LuaState* pState );
		INT ClearListBox( LuaPlus::LuaState* pState );
		INT	GetFirstSelectItem( LuaPlus::LuaState* pState );
		INT	SetItemSelect( LuaPlus::LuaState* pState );
		INT SetItemSelectByItemID( LuaPlus::LuaState* pState );

	protected:
		friend class Window;
		ListBox(CEGUI::Window* pWindow):Window(pWindow) { }
		virtual ~ListBox() {}

	protected:
		static LuaPlus::LuaObject*	s_pMetaTable;
	};
	class ImageListBox : public ListBox
	{
	public:
		virtual	LuaPlus::LuaObject*		GetMetaTable(VOID) { return s_pMetaTable; }
	public:
		virtual INT AddItem( LuaPlus::LuaState* pState );
	protected:
		friend class Window;
		ImageListBox(CEGUI::Window* pWindow):ListBox(pWindow) { }
		virtual ~ImageListBox() {}

	protected:
		static LuaPlus::LuaObject*	s_pMetaTable;
	};

	class ImageListBoxEx : public ListBox
	{
	public:
		virtual	LuaPlus::LuaObject*		GetMetaTable(VOID) { return s_pMetaTable; }
	public:
		virtual INT AddItem( LuaPlus::LuaState* pState );
	protected:
		friend class Window;
		ImageListBoxEx(CEGUI::Window* pWindow):ListBox(pWindow) { }
		virtual ~ImageListBoxEx() {}

	protected:
		static LuaPlus::LuaObject*	s_pMetaTable;
	};

	class CheckButton: public Window
	{
	public:
		virtual	LuaPlus::LuaObject*		GetMetaTable(VOID) { return s_pMetaTable; }
	public:

	protected:
		friend class Window;
		CheckButton(CEGUI::Window* pWindow):Window(pWindow) { }
		virtual ~CheckButton() {}


		INT		SetCheck( LuaPlus::LuaState* pState );
		INT		GetCheck( LuaPlus::LuaState* pState );
	protected:
		static LuaPlus::LuaObject*	s_pMetaTable;
	};


	class ComboBox: public Window
	{
	public:
		virtual	LuaPlus::LuaObject*		GetMetaTable(VOID) { return s_pMetaTable; }
	public:

	protected:
		friend class Window;
		ComboBox(CEGUI::Window* pWindow):Window(pWindow) { }
		virtual ~ComboBox() {}


		INT		ComboBoxAddItem( LuaPlus::LuaState* pState );
		INT		ResetList( LuaPlus::LuaState* pState );
		INT		GetCurrentSelect( LuaPlus::LuaState* pState );
		INT		SetCurrentSelect( LuaPlus::LuaState* pState );

	protected:
		static LuaPlus::LuaObject*	s_pMetaTable;
	};

	class StaticImage: public Window
	{
	public:
		virtual	LuaPlus::LuaObject*		GetMetaTable(VOID) { return s_pMetaTable; }
	public:

	protected:
		friend class Window;
		StaticImage(CEGUI::Window* pWindow):Window(pWindow) { }
		virtual ~StaticImage() {}


		INT		SetImage( LuaPlus::LuaState* pState );
		INT		SetImageColor( LuaPlus::LuaState* pState );
		
	protected:
		static LuaPlus::LuaObject*	s_pMetaTable;
	};

	class ScrollBar: public Window
	{
	public:
		virtual	LuaPlus::LuaObject*		GetMetaTable(VOID) { return s_pMetaTable; }
	public:

	protected:
		friend class Window;
		ScrollBar(CEGUI::Window* pWindow):Window(pWindow) { }
		virtual ~ScrollBar() {}


		INT		GetPosition( LuaPlus::LuaState* pState );
		INT		SetPosition( LuaPlus::LuaState* pState );
	protected:
		static LuaPlus::LuaObject*	s_pMetaTable;
	};

	class SuperTooltip : public Window
	{
	public:
		virtual	LuaPlus::LuaObject*		GetMetaTable(VOID) { return s_pMetaTable; }
	public:

	protected:
		friend class Window;
		SuperTooltip(CEGUI::Window* pWindow):Window(pWindow) { }
		virtual ~SuperTooltip() {}

		INT		PositionSelf(LuaPlus::LuaState* pState);

	protected:
		static LuaPlus::LuaObject*	s_pMetaTable;
	};

	class MeshWindow: public Window
	{
	public:
		virtual	LuaPlus::LuaObject*		GetMetaTable(VOID) { return s_pMetaTable; }
		
	protected:
		friend class Window;
		MeshWindow(CEGUI::Window* pWindow):Window(pWindow) { }
		virtual ~MeshWindow() {}
	
	
		INT		SetFakeObject(LuaPlus::LuaState* pState);
		INT		RotateBegin(LuaPlus::LuaState* pState);
		INT		RotateEnd(LuaPlus::LuaState* pState);
		INT		ChangeActionBegin(LuaPlus::LuaState* pState);
	protected:
		static LuaPlus::LuaObject*	s_pMetaTable;
	};

	class ChatHistory : public Window
	{
	public:
		virtual	LuaPlus::LuaObject*		GetMetaTable(VOID) { return s_pMetaTable; }
		
	protected:
		friend class Window;
		ChatHistory(CEGUI::Window* pWindow):Window(pWindow) { }
		virtual ~ChatHistory() {}
	
		INT		InsertChatString(LuaPlus::LuaState* pState);
		INT		RemoveAllChatString(LuaPlus::LuaState* pState);
		INT		ScrollToEnd(LuaPlus::LuaState* pState);
		INT		ExtendClearRegion(LuaPlus::LuaState* pState);

	protected:
		static LuaPlus::LuaObject*	s_pMetaTable;
	};

	class ChatChannel : public Window
	{
	public:
		virtual	LuaPlus::LuaObject*		GetMetaTable(VOID) { return s_pMetaTable; }
		
	protected:
		friend class Window;
		ChatChannel(CEGUI::Window* pWindow):Window(pWindow) { }
		virtual ~ChatChannel() {}
	
		INT		ClearAllChannel(LuaPlus::LuaState* pState);
		INT		AddChannel(LuaPlus::LuaState* pState);

	protected:
		static LuaPlus::LuaObject*	s_pMetaTable;
	};
	class Scenemap:public Window
	{
	public:
		virtual	LuaPlus::LuaObject*		GetMetaTable(VOID) { return s_pMetaTable; }

	public:
		INT UpdateViewRect( LuaPlus::LuaState* pState );
		INT SetSceneFileName( LuaPlus::LuaState* pState );
		INT CloseSceneMap( LuaPlus::LuaState* pState );
		INT GetMouseScenePos( LuaPlus::LuaState* pState );
		INT	UpdateSceneMap( LuaPlus::LuaState* pState );
		INT SetSceneZoomMode( LuaPlus::LuaState* pState );
	protected:
		friend class Window;
		Scenemap(CEGUI::Window* pWindow):Window(pWindow) { }
		virtual ~Scenemap() {}

	protected:
		static LuaPlus::LuaObject*	s_pMetaTable;
	};
	
	class Worldmap:public Window
	{
	public:
		virtual	LuaPlus::LuaObject*		GetMetaTable(VOID) { return s_pMetaTable; }
	public:
		INT	InitWorldMap( LuaPlus::LuaState* pState );
		INT UpdateWorldMap( LuaPlus::LuaState* pState );
		INT GetCurrentSelectScene( LuaPlus::LuaState* pState );
	protected:
		void UpdatePlayer();
		CEGUI::String Worldmap::GetSceneNameByID( int id );
		friend class Window;
		Worldmap(CEGUI::Window* pWindow):Window(pWindow) { }
		virtual ~Worldmap() {}

	protected:
		static LuaPlus::LuaObject*	s_pMetaTable;
	};

	class InfoList:public Window
	{
	public:
		virtual	LuaPlus::LuaObject*		GetMetaTable(VOID) { return s_pMetaTable; }
	public:
		INT	AddInfo( LuaPlus::LuaState* pState );
	protected:
		friend class Window;
		InfoList(CEGUI::Window* pWindow):Window(pWindow) { }
		virtual ~InfoList() {}

	protected:
		static LuaPlus::LuaObject*	s_pMetaTable;
	};

	class CtrlList: public Window
	{
	public:
		virtual	LuaPlus::LuaObject*		GetMetaTable(VOID) { return s_pMetaTable; }
	public:
		INT AddColumn( LuaPlus::LuaState* pState );			// �����
		INT InsertColumn( LuaPlus::LuaState* pState );		// ����һ��
		INT AddNewItem( LuaPlus::LuaState* pState );		// ���һ��
		INT SetItemText( LuaPlus::LuaState* pState );		// ����һ����Ԫ���ı�
		INT SetItemData( LuaPlus::LuaState* pState );		// ����һ����Ԫ������
		INT DeleteItem( LuaPlus::LuaState* pState );		// ɾ��һ��
		INT RemoveAllItem( LuaPlus::LuaState* pState );		// ɾ������
		
		INT GetItemText( LuaPlus::LuaState* pState );		// �õ�һ����Ԫ���ı�
		INT GetItemCount( LuaPlus::LuaState* pState );		// �õ��ֶ�����
		INT GetColumnCount( LuaPlus::LuaState* pState);		// �õ��ж�����
		INT GetSelectItem( LuaPlus::LuaState* pState);		// �õ�ѡ�е���

	protected:
		friend class Window;
		CtrlList(CEGUI::Window* pWindow):Window(pWindow) { }
		virtual ~CtrlList() {}

	protected:
		static LuaPlus::LuaObject*	s_pMetaTable;
	};

	
	class SoftKeyWindow: public Window
	{
	public:
		virtual	LuaPlus::LuaObject*		GetMetaTable(VOID) { return s_pMetaTable; }
	public:
		INT SetAimEditBox( LuaPlus::LuaState* pState);	// �õ��ж�����

	protected:
		friend class Window;
		SoftKeyWindow(CEGUI::Window* pWindow):Window(pWindow) { }
		virtual ~SoftKeyWindow() {}

	protected:
		static LuaPlus::LuaObject*	s_pMetaTable;
	};

	class Minimap : public Scenemap
	{
	public:
		virtual	LuaPlus::LuaObject*		GetMetaTable(VOID) { return s_pMetaTable; }

	public:
		INT UpdateFlag( LuaPlus::LuaState* pState );
	protected:
		friend class Window;
		Minimap(CEGUI::Window* pWindow):Scenemap(pWindow) { }
		virtual ~Minimap() {}

	protected:
		static LuaPlus::LuaObject*	s_pMetaTable;
	};

	class AnimateWindow : public Window
	{
	public:
		virtual	LuaPlus::LuaObject*		GetMetaTable(VOID) { return s_pMetaTable; }

	public:
		INT Play( LuaPlus::LuaState* pState );
	protected:
		friend class Window;
		AnimateWindow(CEGUI::Window* pWindow):Window(pWindow) { }
		virtual ~AnimateWindow() {}

	protected:
		static LuaPlus::LuaObject*	s_pMetaTable;
	};
	

}
