#pragma once
#include "falagardscenemap.h"
#include "IFalagardMinimapWindow.h"
#include <vector>
namespace CEGUI
{
	namespace FalagardRadarProperties
	{
		class IconImage: public Property
		{
			public:
			IconImage() : Property(
				"IconImage",
				"Property to get/set Minimap name status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};

		class AnimateImage: public Property
		{
			public:
			AnimateImage() : Property(
				"AnimateImage",
				"Property to get/set Minimap name status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
		class AutoReset: public Property
		{
			public:
			AutoReset() : Property(
				"AutoReset",
				"Property to get/set Minimap name status.",
				"")
			{}

			String   get(const PropertyReceiver* receiver) const;
			void   set(PropertyReceiver* receiver, const String& value);
		};
	}
	struct MINIMAP_FLAG_DATA
	{
		Point   pos;
		String  name;
		Size    size;
		Window* m_pWindow;
		MINIMAP_FLAG_DATA()
		{
			pos = Point( 0,0 );
			name.clear();
			size = Size( 0,0 );
		}
	};
	class FalagardRadar : public FalagardSceneMap
	{
		enum MINIMAP_IMAGE_TYPE
		{
			IMAGE_TYPE_Animy	= 0, // ����
			IMAGE_TYPE_ExpNpc	= 1, // ����npc
			IMAGE_TYPE_Team		= 2, // ����
			IMAGE_TYPE_Player	= 3, // ������
			IMAGE_TYPE_ExpObj	= 4, // ������
			IMAGE_TYPE_Active	= 5, // ������
			IMAGE_TYPE_ScenePos = 6, // ������ת��
			IMAGE_TYPE_Flash	= 7, // �����
			IMAGE_TYPE_Pet		= 8, // ����
			IMAGE_TYPE_NUM,
			IMAGE_TYPE_MYSELF,
		};
		
		enum MINIMAP_POS_TYPE
		{
			POS_TYPE_INVALIDE = -1,
			POS_TYPE_IMAGE = 0,
			POS_TYPE_ANIMATE,
		};
		const Image*		d_AnimyImage;		// ��ʾ���˵�ͼ��
		const Image*		d_ExpNpcImage;		// ����npc��ͼ��
		const Image*		d_TeammateImage;	// ���ѵ�ͼ��
		const Image*		d_PlayerImage;		// �����ҵ�ͼ��
		const Image*		d_ExpObjImage;		// ������Ʒ��ͼ��
		const Image*		d_MyselfImage;		// ����Լ���ͼ��
		const Image*		d_PetImage;			// �����ͼ��
		const Image*		d_ScenePosImage;	// ���������
		const Image*		d_FlashImage;		// ��Ƶ�

		String				d_PlayerAnimateName;// �Լ��ķ��򶯻�����
		String				d_ActiveAnimateName;// �����Ķ�������
		String				d_FlashAnimateName;	// �����Ķ�������

		typedef std::vector< MINIMAP_FLAG_DATA* >  RENDER_POS;
// ���е���б�
		int			m_aPointerTimer[ IMAGE_TYPE_NUM ]; // ����Ĺ���ָ��
		RENDER_POS			m_aAnimyPos;		// ����
		RENDER_POS			m_aExpNpcPos;		// ����npc
		RENDER_POS			m_aTeammatePos;		// ����
		RENDER_POS			m_aPlayerPos;		// �������
		RENDER_POS			m_aExpObjPos;		// ������Ʒ��
		RENDER_POS  		m_aScenePos;		// �������͵�
		RENDER_POS  		m_aActivePos;		// ������
		RENDER_POS  		m_aFlashPos;		// �����
		RENDER_POS  		m_aPetPos;			// ����


		float				d_MAXJuLi;
		float				d_ReturnTimer;		// �Զ��ָ��ļ�ʹ��
		String				m_objTooltip;

		bool				m_bMouseInWindow;

		bool				m_bIsAutoReset;
		
	public:
		static const utf8   WidgetTypeName[];       
	public:
		FalagardRadar(const String& type, const String& name);
		~FalagardRadar(void);

	public:
		void setTypeImage( int nType, const Image* pImage ); //����image
		const Image* getTypeImage( int nType );

		void setTypeName( int nType, const String& str ); // ���ö�������
		String getTypeName( int nType );
		virtual void setMAXJuLi( const float Maxjuli ){d_MAXJuLi = Maxjuli;} // ������ʾ�뾶
		virtual	void updateSelf(float elapsed); 
		
		void		UpdateViewRect(); // ���ݵ�ǰ����λ����ֵ�ɼ���Χ

		int			GetTypeByName( const String& str );
		String		GetNameByType( int nType );

		void		setAutoReset( bool bAutoReset ){ m_bIsAutoReset = bAutoReset; };

		RENDER_POS* GetListByType( int nType );
	public:
		virtual void setMinimapData( int nType, float fX, float fY, String& szName  );
		virtual void Clear();
		// �����Լ���λ��
		virtual void setMyselfPos( const Point& pos );
		virtual void setMyselfDir( const int nDirect );

		virtual void CreateSceneMap( const String& strFileName, const int nUserSharp );


		virtual void	onMouseMove( MouseEventArgs &e );
		virtual void    onMouseEnters( MouseEventArgs& e );
		virtual void    onMouseLeaves( MouseEventArgs& e );

		
	public:
		static FalagardRadarProperties::IconImage			d_IconImageProperty;
		static FalagardRadarProperties::AnimateImage		d_AnimateImageProperty;
		static FalagardRadarProperties::AutoReset			d_AutoRestProperty;
	private:
		void updateFlash();
		void updateActive();
		void updateMyselfDir();
		void updateMyselfPos();
		MINIMAP_FLAG_DATA*  getMiniMapFlagData( int nType, int nIndex );

		int GetType( int nType );

		Point GetActiveArrowImagePos( Rect& rcView, Point& posStart, Point& posEnd );
		const Image* GetActiveArrowImage( Rect& rcView, Point& posStart, Point& posEnd );
	};

	class FALAGARDBASE_API FalagardRadarFactory : public WindowFactory
    {
    public:
        FalagardRadarFactory(void) : WindowFactory(FalagardRadar::WidgetTypeName) { }
        ~FalagardRadarFactory(void){}
        Window* createWindow(const String& name);
        void destroyWindow(Window* window);
    };
}