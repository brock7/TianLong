

#pragma once

#include "falmodule.h"
#include "ceguiwindowfactory.h"
#include "falstaticimage.h"
#include "FalMinimapProperties.h"
#include "IFalagardSceneMapWindow.h"

namespace CEGUI
{

	namespace FalagardSceneMapProperties
	{
		class PlayerImage: public Property
		{
		public:
			PlayerImage() : Property(
				"PlayerImage",
				"Property to get/set Minimap name status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
		class MySelfImage: public Property
		{
		public:
			MySelfImage() : Property(
				"MySelfImage",
				"Property to get/set Minimap name status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};


	}
	/*
	���˼·
	�ѵ�ͼ�ֳ�256X256��С�飬��ͼ��ͼƬ��һ���̶��ı��������磬ÿ256���ص�ͼƬ��Ӧ��ͼ�ϵ�64��λ���������������������еĵ�ͼ�����������

	*/
	enum 
	{
		SCENEMAP_DISPLAY_MODE_FULLSCREEN = 0,	// ȫ��
		SCENEMAP_DISPLAY_MODE_ABSOLTE,		// �������ر���
	};
	class FALAGARDBASE_API FalagardSceneMap :	public FalagardStaticImage, public IFalagardSceneMapWindow
	{
	public:

		static const utf8   WidgetTypeName[];       

		static FalagardSceneMapProperties::PlayerImage		d_PlayerImageProperty;		// �����õ�ͼ��
		static FalagardSceneMapProperties::MySelfImage		d_MyselfImageProperty;		// �Լ�����Ҫ��ͼƬ����

		void   Close();

		virtual void onMouseMove( MouseEventArgs &e );
		virtual void onMouseButtonDown( MouseEventArgs &e );
		virtual void onMouseButtonUp( MouseEventArgs &e );
		virtual void onMouseLeaves( MouseEventArgs& e );
		virtual	void updateSelf(float elapsed); 
		virtual void onMouseEnters(MouseEventArgs& e);

		// �õ���������λ��
		virtual const Point GetMouseScenePos() const { return d_MouseScenePos; };
		//
		void setPlayerImage( const Image* pImage );

		void setMyselfImage( const String& name );

		//�����Լ��ķ���
		virtual void setMyselfDir( const int nDirect );
		// �����Լ���λ��
		virtual void setMyselfPos( const Point& pos );
		// ����һ�����ѵ�λ��
		virtual void addPlayerPos( const Point& pos, const String& name );
		// ������ѵ�����
		virtual void Clear();
		// ���ó����ĳߴ�
		virtual void SetSceneSize( Size size ) { d_SceneSize = size ; };
		//  ���ó������ļ�
		virtual void SetSceneFileName( const String& str, const int nUserSharp );
		// ������ʾ�뾶
		virtual void setMAXJuLi( const float Maxjuli ){d_MAXJuLi = Maxjuli;} 
		virtual void SetZoomMode( int nMode );// { m_nDisplayMode = nMode; };

		virtual void setMinimapData( int nType, float fX, float fY, String& szName  ){};
	protected:
		virtual void	onParentSized(WindowEventArgs& e);
		const Image*	getImageByName( const String& str, Rect rect , const char * Type); // �������ֵõ�image
		void			createBackground( const String& strFileName, const int nUserSharp );
		void			SetViewByCenter( const Point& pos ); // ���õ�ǰ���ڿɼ���ΧΪ�����λ��Ϊ����
		Rect			GetCurrentViewRect(); // �õ���ǰ�Ŀɼ���Χ
		Point			GetCurrentCenter();
		virtual	void	setSceneMapCenter();
	protected:

		struct SCENEMAP_FLAG_DATA
		{
			Point   pos;
			String  name;
		};
		typedef std::list< SCENEMAP_FLAG_DATA >  RENDER_POS;
		RENDER_POS	m_aPlayerPos;

		int			d_PlayerDirect;			// ����ĽǶ�
		Point		d_MyseflPos;
		bool		d_fresh;
		float		d_MAXJuLi;
		Image*		d_PlayerImage;
		String		d_MyselfAnimate;
		
		Window*		m_pFriendWindow[ 5 ]; // 5�����ѵô���
		Window*		m_pMySelfWindow; // �Լ��Ĵ���
		Window*		m_pLTWindow; // ���Ͻǵõ�ͼ����
		Window*		m_pRBWindow;// ���½ǵõ�ͼ����
		Window*		m_pBackground;

		int		m_nXCount;
		int		m_nYCount;
		Point	d_MouseScenePos;
		Point	m_PlayerPos;			// �ѷ�ʱ��¼��ǰ����λ��
		Point   m_WindowZero; // ���ڵ�����ԭ��
		// �����ߴ�
		Size	d_SceneSize;
		// ����������ͼ
		virtual void CreateSceneMap( const String& strFileName, const int nUserSharp );
		void ResetPlayerWindow( int nIndex, SCENEMAP_FLAG_DATA& data );
		Point ScenePosToWindowPos( const Point& pos ); // ��������ת����������
		Point ScreenPosToScenePos( const Point& pos ); // ��Ļ����ת������������f
		Point ScreenPosToScenePosRelative( const Point& pos ); // ��Ļ����ת������������
		Point ScreenPosToWindowPosRelative( const Point& pos ); // ��Ļ����ת������������
		Point ScreenPosToPicPos( const Point& pos ); // ��Ļ����ת����ͼƬ����
		void  ResetMyself();
		void  UpdateAllWindow();
		void  UpdateBackgroundItem();

		float m_fZoomValueX;		// ���ű������ɱ��
		float m_fZoomValueY;		// ���ű������ɱ��
		bool  m_bDraping;
		Point m_ptDrapOffset;
		int   m_nDisplayMode; // ��ʾģʽ����Ϊ�̶�������ȫ������
	public:
		FalagardSceneMap( const String& type, const String& name );
		~FalagardSceneMap();

		static const String EventNamespace;
		static const String MouseRButtonDownCtrl;

	};

	class FALAGARDBASE_API FalagardScenemapFactory : public WindowFactory
    {
    public:
        FalagardScenemapFactory(void) : WindowFactory(FalagardSceneMap::WidgetTypeName) { }
        ~FalagardScenemapFactory(void){}
        Window* createWindow(const String& name);
        void destroyWindow(Window* window);
    };
};