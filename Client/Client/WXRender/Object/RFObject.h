#pragma once
#include "..\WxRender\RenderSystem.h"

namespace Ogre
{
	class SceneNode;
}
class CWXObj_ProjTex;

class CWXObject : public tEntityNode
{
public:
	//����Ⱦ�����ٽڵ�
	virtual VOID	Destroy(VOID);

	//���ø�������
	virtual VOID	SetData(UINT nData)		{ m_nData = nData; }
	virtual UINT	GetData(VOID) const		{ return m_nData; }

	//����λ�� (��Ϸ��������)
	virtual VOID	SetPosition(const fVector3& vPos) {}
	//������ת�Ƕ�
	virtual VOID	SetOrientation(const fVector3& vRotate) {}
	//ֱ����������
	virtual VOID	SetWXObjectProperty(const char* szPropertyName, const char* szPropertyValue){}
	//�����Ƿ�ɼ�
	virtual VOID	SetVisible(BOOL bVisible){}

	//�����Ƿ��ܱ����ѡ��
	virtual VOID	SetRayQuery(BOOL bQuery) { }
	virtual BOOL	GetRayQuery(VOID) const { return FALSE; }
	//���ѡ�����ȼ�
	virtual VOID			RayQuery_SetLevel(RAYQUERY_LEVEL nLevel) { m_nRayQueryLevel = nLevel; }
	virtual RAYQUERY_LEVEL	RayQuery_GetLevel(VOID) const { return m_nRayQueryLevel; }

	//����һ����Ч
	virtual VOID	Attach_Effect(LPCTSTR szEffectName) {}
	virtual VOID	Detach_Effect(void) {}

	virtual UINT	AddEffect( LPCSTR effectName, LPCSTR locatorName ){ return NULL; }
	virtual VOID	DelEffect( UINT effect ){}
	virtual VOID	DelAllEffect( VOID ){}

	virtual VOID	SetLeftWeaponEffect(LPCTSTR szEffectName, UINT color) {}
	virtual VOID	SetRightWeaponEffect(LPCTSTR szEffectName, UINT color) {}

	virtual VOID	SetRightWeaponMat(LPCTSTR szWeaponMat, LPCTSTR szMatName) {};
	virtual VOID	SetLeftWeaponMat(LPCTSTR szWeaponMat, LPCTSTR szMatName) {};
	//����һ��Ͷ������
	virtual VOID	Attach_ProjTexture(PROJTEX_TYPE type, BOOL bShow, FLOAT Ring_Range=100.0f, FLOAT fHeight=1.7f) {}

	//����MeshFile
	virtual VOID	StaObj_SetMeshFile(LPCTSTR szMeshFile)	{}
	//���ý�ɫģ����Դ��
	virtual VOID	Actor_SetFile(LPCTSTR szActorFile)	{}
	//���ý�ɫģ�͵�����
	virtual VOID	Actor_SetObjectProperty(LPCTSTR szName, LPCTSTR szValue) {}
	//�õ���ɫģ�͵�����
	virtual VOID	Actor_GetObjectProperty(LPCTSTR szName, STRING& szValue) {}
	//ȡ������ģ���ϵ�ĳ�ؼ���λ��
	virtual VOID	Actor_GetLocator(LPCTSTR szName, fVector3& fvPosition) {}
	//ȡ��"ͷ��״̬��"����Ļ�ϵ�λ��,�������FALSE����ʾ����Ļ֮��,����û�иõ�
	virtual BOOL	Actor_GetInfoBoardPos(fVector2& ivPos, const fVector3 *pvObjPos = NULL,const float fObligeHeight = -1) {return FALSE;}
	//������ʽ
	// bAnim		- TRUE(���ö���)/FALSE(������ʽ)
	// szSkillName	- ������������ʽ��
	// bLoop		- �Ƿ�ѭ��
	// fFuseParam	- ���������ںϲ��� 
	virtual VOID	Actor_EnterSkill(BOOL bAnim, LPCTSTR szSkillName, BOOL bLoop, FLOAT fFuseParam) {}
	virtual VOID	Actor_SetHairColor(UINT HairColor) {}
	// �л������Ĳ����ٶ�
	// fRate		- ���ű���
	virtual VOID	Actor_ChangeActionRate(FLOAT fRate){}
	//����ȱʡ����
	virtual VOID	Actor_SetDefaultAnim(LPCTSTR szAnimName) {}
	//����VisibleFlag
	virtual VOID	Actor_SetUIVisibleFlag(VOID) {}
	//����͸����
	virtual VOID	Actor_SetTransparent(FLOAT fTransparency, FLOAT fTime){}
	// ����������Ч��Ŀ���
	virtual VOID	Actor_SetEffectExtraTransformInfos(UINT uEffect, fVector3& fvPosition){}
	//�����Ƿ�������Hover״̬
	virtual VOID	Actor_SetMouseHover(BOOL bHover) {} 

public:
	CWXObject();
	virtual ~CWXObject();

protected:
	UINT				m_nData;
	//���ѡ�����ȼ�,��Խ��Խ����ѡ��
	RAYQUERY_LEVEL		m_nRayQueryLevel;
};