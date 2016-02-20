#pragma once
#include "..\WxRender\RenderSystem.h"
#include <OgreVector3.h>
#include <OgreTexture.h>

namespace Ogre
{
	class Camera;
	class Viewport;
	class RenderTexture;
};

class tEntityNode;
//����UI��ʾ��ģ������Ⱦ��Ĺ�����
class CFakeObjectManager
{
public:
	//����UI��ʾ��ģ����Ⱦ����
	struct FakeObject
	{
		STRING					strName;			//����
		Ogre::Camera*			pCamera;			//Ogre�����
		Ogre::RenderTexture*	pRenderTexture;		//������Ⱦ��Texture
		Ogre::Viewport*			pViewPort;			//������Ⱦ��View
		tEntityNode*			pEntityNode;		//��ʾ�ڵ㣬ֻ������ʾ�������𴴽�/ɾ��
		Ogre::TexturePtr		pTexturePtr;		//��Ⱦ��������
		float					fCameraHeight;		//������߶�
		float					fCameraDistance;	//���������
		float					fCameraPitch;		//������Ƕ�
	};
	typedef std::map< STRING, FakeObject > FakeObjectMap;

public:
	//����һ������ڵ�
	void	CreateObject(LPCTSTR szObjName, tEntityNode* pNode, LPCTSTR szCameraName);
	//����
	void	DestroyObject(LPCTSTR szObjName);
	//���ø�ģ���Ƿ�����UI����ʾ�����ڴ򿪺͹ر�ViewPort��ˢ�£�ȱʡ����ʾ
	void	SetVisible(LPCTSTR szObjName, BOOL bVisible, float fAspectRatio);
	//����������Ⱦ��ģ�ͣ�����ģ����Ϊ�ɼ���������Ϊ���ɼ�
	void	SetActiveObject(FakeObject* pObj);

	//����ViewPort����Object
	FakeObject*		GetObjectAsViewPort(Ogre::Viewport* pViewPort);

	VOID Debug_SetFakeCamera(int nType, float fValue);

public:
	CFakeObjectManager(void);
	~CFakeObjectManager(void);

	static CFakeObjectManager*	GetMe(VOID) { return s_pMe; }

	//��ʼ��
	VOID		Initial(VOID);
	//�ͷ���Դ
	VOID		Release(VOID);

protected:
	//�õ�ָ����Ⱦ�����ڵ㣬���û�оʹ������µģ�������Map��
	FakeObjectMap::iterator		_GetFakeNode(LPCTSTR szNodeName,tEntityNode* pNode, LPCTSTR szCameraName);
	//���������
	void						_UpdateCamera(FakeObject& node);

	void						_DestroyFakeNode(FakeObject& obj);

protected:
	static CFakeObjectManager*	s_pMe;

	//-------------------------------------------
	//��ʾ����
	FakeObjectMap			m_mapObject;
	//�ڳ����е�λ��
	Ogre::Vector3 			m_fvPosition;			

	//����ViewPort���������ݱ�
	std::map< Ogre::Viewport*, FakeObject* >	m_mapIndexOfViewPort;
};
