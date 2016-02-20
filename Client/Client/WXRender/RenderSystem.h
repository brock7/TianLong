/****************************************\
*										*
*			    ͼ�����ϵͳ			*
*										*
\****************************************/
#pragma once

#include "TDNode.h"
#include "TDGfxSystem.h"
#include "TDMath.h"
#include "TDEventSystem.h"
#include "..\WxRender\RenderSystem.h"

struct EVENT;

class CCamera;
class CDebugPanel;
class CProcessBar;
class CHelpPanel;
class CFakeObjectManager;
namespace WX 
{ 
	class System;
	class TerrainGrid;
	class PostFilterManager;
	class EnviromentObject;
};

namespace Ogre
{
	class RaySceneQuery;
};

class CRenderSystem : public tNode
{
public:
	//	����ת�����
	enum AXIS_TYPE 
	{
		AX_GAME,		//��Ϸ�������꣬����Ϊ��λ
		AX_PLAN,		//��Ϸ�������꣬����Ϊ��λ,����������Y����
		AX_GFX,			//��Ⱦϵͳ�ж������꣬һ��������Ϊ��λ
		AX_SCREEN,		//��Ļ�ϵ��������꣬�������λ��,����
	};

	//	�������غ���
	enum CAMERA_TYPE
	{
		CHAR_VIEW,
		SCENE
	};
	//----------------------------------------------------------------
	//	ϵͳ��غ���
	//----------------------------------------------------------------
public:
	/// ��ʼ����Ⱦ��
	virtual VOID		Initial(VOID*);
	/// ������Ⱦ��
	virtual VOID		Release(VOID);
	//�߼���ѭ����
	virtual VOID		Tick(VOID);
	/// ��Ⱦ����
	virtual VOID		RenderFrame(VOID);
	/// ��ȾLoading����
	virtual VOID		RenderLodingFrame(LPCTSTR szLoding);
	/// ��ӦWM_PAINT��Ϣ
	virtual VOID		OnPaint(VOID);
	/// ���ڴ�С�����ı�
	virtual VOID		OnSizeChange(UINT message, WPARAM wParam, LPARAM lParam);
	/// ȡ����Ⱦ����
	virtual HWND		GetWindow(VOID) const;
	//����ǰ��Ļ��ͼ���ļ���
	virtual BOOL		PrintScreen(LPTSTR szReturnBuf, INT nBufSize);

	//�����ػ��������ڿ��Ƹ�����ȾЧ���Ŀ���
	static VOID	WINAPI	_OnVariableChangedEvent(const EVENT* pEvent, UINT dwOwnerData);

	//----------------------------------------------------------------
	//	����ת�����
	//----------------------------------------------------------------
public:

	// ��һ������ת��Ϊ��һ������,ת���ɳɹ�����TRUE
	// ��Դ����Ƿ���ʱ�򣬷���FALSE
	virtual BOOL		Axis_Trans(	AXIS_TYPE typeSource, const fVector3& fvSource,
									AXIS_TYPE typeTar,			fVector3& fvTarget);

	//��������Ƿ�Ϸ�
	virtual BOOL		Axis_CheckValid(AXIS_TYPE typeSource, const fVector3& fvAxis);

	//----------------------------------------------------------------
	//	�������غ���
	//----------------------------------------------------------------
public:
	//���õ�ǰ�����
	virtual VOID		Camera_SetCurrent(CAMERA_TYPE theType);

	/// �ƶ����������λ�õ�ĳһ��,һ����������ڵ�λ��(������ά����ϵ)
	virtual VOID		Camera_SetLookAt(const fVector3& fvPos);
	
	/// ��������ӿ� [0.0f, 1.0f]  0-������� 1-��Զ����
	virtual VOID		Camera_SetZoom(FLOAT fZoom);
	virtual VOID		Camera_SetZoom(BOOL bUp,INT multiple);
	virtual FLOAT		Camera_GetZoom(VOID) const;

	/// ����������� [0, 2��)�� ��z��ָ��ķ���Ϊ0�ȣ���ʱ��Ϊ������
	virtual VOID		Camera_Direction(FLOAT fDir);
	virtual VOID		Camera_AddDirection(FLOAT fMultiple);
	virtual FLOAT		Camera_GetDirection(VOID) const;

	// ȡ����Ļ����
	virtual VOID		Camera_GetWindowToViewportRay(INT nX, INT nY, fRay& fRay);

	// ��һ��
	virtual VOID		Camera_VibrateOneTime();

	//-----------------------------------------------------------------------
	//
	// ����ѡ��, ����, �����õ���������ӿ�.
	//

	// �����۾���λ��
	virtual VOID		Camera_SetEyePos(const fVector3& fvPos);
	// ���ù۲���λ��
	virtual VOID		Camera_SetLookAtPos(const fVector3& fvPos);

	//----------------------------------------------------------------
	//������Ϣ
	//----------------------------------------------------------------
public:
	//--- for debug
	/// �������Deubg��Ϣ
	virtual VOID		PushDebugString(LPCTSTR szMsg, ...);
	/// ����Debug״̬��Ϣ
	virtual VOID		SetDebugString(LPCTSTR szName, LPCTSTR szValue);
	/// ����ѡ��������Ϣ
	virtual	VOID		SetHitObjText(LPCTSTR szHitObjText);
	//--- for debug

	static const CHAR CRenderSystem::sz32BitTexturesVar[];	//��ɫ������������(16/32λ)
	static const CHAR CRenderSystem::szDefaultTextureFilteringVar[];		//����(��/��/��)
	static const CHAR CRenderSystem::szLightmapQualityVar[];	//������Ӱ(��/��/��)
	static const CHAR CRenderSystem::szAnimationQualityVar[];	//��̬���嶯��(��/��)
	static const CHAR CRenderSystem::szPostFilterEnabledVar[];	//ȫ������Ч��(��/��)
	static const CHAR CRenderSystem::szFSAAVar[];	//�����(1/2/4)
	static const CHAR CRenderSystem::szShadowTechniqueVar[];	//������Ӱ(��/��)
	static const CHAR CRenderSystem::szvsyncVar[];	//��ֱͬ��(��/��)

	//----------------------------------------------------------------
	//�������
	//----------------------------------------------------------------
public:
	//Ԥ���س���
	virtual VOID			Scene_PreLoad(const STRING& theScenePreload);
	//���س���
	virtual VOID			Scene_Load(INT nID, const STRING& theSceneDefine);
	//��ʾ��������
	virtual VOID			Scene_ShowGridSwitch(INT nZoneSize);
	//��ʾ����ȫ����ȾЧ��
	virtual VOID			Scene_SetPostFilter(LPCTSTR szFilterName);
	//����/�˳�����ȫ����ȾЧ��
	virtual VOID			Scene_SetPostFilter_Death(BOOL bEnter);
	//����Lightmap
	virtual VOID			Scene_SetLightmapQuality(INT nQuality);
	//���û�����Ч(λ��)
	virtual VOID			Scene_SetEnviromentInfo(const fVector3& fvPos);
	//���ò���32λ/16λת��
	virtual VOID			Scene_Enable32BitTextures(BOOL bSetting=TRUE);
	//���ò��������Ǹ�������3������������2������˫����1(Զ��������)��
	virtual VOID			Scene_SetTextureFiltering(INT nOptions);
	//���þ�̬���嶯��
	virtual VOID			Scene_SetAnimationQuality(INT nOptions);
	//����ȫ������Ч��
	virtual VOID			Scene_SetPostFilterEnabled(BOOL bSetting);
	//��������ʵʱ��Ӱ
	virtual VOID			Scene_SetShadowTechnique(BOOL bSetting);


	//��ĳ������һ������
	virtual VOID			Debug_AddParticle(LPCTSTR szParticleName, const fVector3& fvPos);
	//
	virtual VOID			Debug_SetFakeCamera(int nType, float fValue);
	//��ʾ/����DebugPanel
	virtual VOID			Debug_ToggleShowDebugPanel(VOID);

	//----------------------------------------------------------------
	// ��������Ⱦ�������
public:
	/// ����һ������Ⱦʵ��  ETYPE_*
	virtual tEntityNode*	New_EntityObject(tEntityNode::EntityType type);
	//ɾ��һ����Ⱦ��
			VOID			Destroy_EntityObject(tEntityNode* pNode);
	///Ѱ�Һ����Ӵ���ʵ��
	// nX, nY - ���λ��
	virtual tEntityNode*	Find_HitWXObject(INT nX, INT nY);

	//----------------------------------------------------------------
	//UI��ʾģ�����
public:
	//����һ��Fake��Ⱦ����
	virtual VOID			FakeObj_Create(LPCTSTR  szName, tEntityNode* pNode, LPCTSTR szCameraName);
	//����һ��Fake��Ⱦ����
	virtual VOID			FakeObj_Destroy(LPCTSTR szObjName);
	//����/��ʾFake��Ⱦ����
	virtual VOID			FakeObj_Show(LPCTSTR szObjName, BOOL bVisible, float fAspectRatio);

	//-----------------------------------------------------------
	//����
protected:
	//	���� ��Ⱦϵͳ
	WX::System* m_pRenderSystem;
	//��ogre���������һ����װ,ʹ����ÿ���
	CCamera* m_pCamera_Current;
	//����ѡ�����������
	CCamera* m_pCamera_CharView;
	//�����˳������ӽ��������
	CCamera* m_pCamera_Scene;
	//��������
	WX::TerrainGrid* m_pTerrainGrid;
	//������ߺͳ�������
	Ogre::RaySceneQuery	*m_pRaySceneQuery;
	//PostFilterϵͳ(ȫ����Ⱦ��)
	WX::PostFilterManager* m_pPostFilterManager;
	//������Ч
	WX::EnviromentObject*  m_pEnviromentObject;

	//������Ч
	BOOL m_bDeathMode;
	INT	m_nDeachModePercent; //[0,100]
	//���԰�
	CDebugPanel			*m_pDebugPanel;

	// FakeManager
	CFakeObjectManager*  m_pFakeObjectManager;
	//-----------------------------------------------------------
	//��Ⱦ���ڲ�����
	//-----------------------------------------------------------
public:
	//�õ���Ⱦϵͳ
	WX::System*		GetRenderSystem(VOID) { return m_pRenderSystem; }
	//��Ϸ��������Ⱦ����֮��ķ�������
	fVector3			GetSacle(VOID) const;

	// �������þɵ���ʾ���ã� ȫ����ʾ�� 2006��4��10��
	// ��¼����֮�� ������Ϸ�У� �������������
	// void ResetOldShowSetting();

protected:

public:
	CRenderSystem(VOID);
	virtual ~CRenderSystem(VOID);
	static CRenderSystem*		GetMe(VOID) { return s_pMe; }

protected:
	static CRenderSystem*	s_pMe;

	WX_DECLARE_DYNAMIC(CRenderSystem);
};

