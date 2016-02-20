/****************************************\
*										*
*	   CRFObj_Actor(DObject)		*
*										*
\****************************************/

#pragma once
#include "RFObj.h"

namespace WX
{
    class LogicModel;
};

class DObject_NT;
class CWXObj_ProjTex;
class CRFObj_Actor : public CWXObj_Object
{
public:
	//�õ�����
	virtual EntityType	GetType(VOID) const	{ return ETYPE_ACTOR; }

	//����Ⱦ�����ٽڵ�
	virtual VOID		Destroy(VOID);
	//�����Ƿ�ɼ�
	virtual VOID		SetVisible(BOOL bVisible);
	//����һ��Ͷ������
	virtual VOID		Attach_ProjTexture(PROJTEX_TYPE type, BOOL bShow, FLOAT Ring_Range=100.0f, FLOAT fHeight=1.7f );
	virtual void		Attach_Object(tEntityNode *pObject, LPCTSTR szAttachLocator);
	virtual void		Detach_Object(tEntityNode *pObject);

	//���ý�ɫģ����Դ��
	virtual VOID	Actor_SetFile(LPCTSTR szActorFile);
	//���ý�ɫģ�͵�����
	virtual VOID	Actor_SetObjectProperty(LPCTSTR szName, LPCTSTR szValue);
	//�õ���ɫģ�͵�����
	virtual VOID	Actor_GetObjectProperty(LPCTSTR szName, STRING& szValue);
	//ȡ������ģ���ϵ�ĳ�ؼ���λ��
	virtual VOID	Actor_GetLocator(LPCTSTR szName, fVector3& fvPosition);
	//ȡ��"ͷ��״̬��"����Ļ�ϵ�λ��,�������FALSE����ʾ����Ļ֮��,����û�иõ�
	virtual BOOL	Actor_GetInfoBoardPos(fVector2& ivPos, const fVector3 *pvObjPos = NULL,const float fObligeHeight = -1);
	//������ʽ
	// bAnim		- TRUE(���ö���)/FALSE(������ʽ)
	// szSkillName	- ������������ʽ��
	// bLoop		- �Ƿ�ѭ��
	// fFuseParam	- ���������ںϲ��� 
	virtual VOID	Actor_EnterSkill(BOOL bAnim, LPCTSTR szSkillName, BOOL bLoop, FLOAT fFuseParam);
	virtual VOID	Actor_SetHairColor(UINT HairColor);
	// �л������Ĳ����ٶ�
	// fRate		- ���ű���
	virtual VOID	Actor_ChangeActionRate(FLOAT fRate);
	//����VisibleFlag
	virtual VOID	Actor_SetUIVisibleFlag(VOID);
	//����ȱʡ����
	virtual VOID	Actor_SetDefaultAnim(LPCTSTR szAnimName);
	//����͸����
	virtual VOID	Actor_SetTransparent(FLOAT fTransparency, FLOAT fTime);
	// ����������Ч��Ŀ���
	virtual VOID	Actor_SetEffectExtraTransformInfos(UINT uEffect, fVector3& fvPosition);
	//�����Ƿ�������Hover״̬
	virtual VOID	Actor_SetMouseHover(BOOL bHover);
	//���ý�ɫ��������֪ͨ
	virtual void	SetAnimationEndEvent(FUNC_ONANIMATIONEND pFunc, DWORD dwParam);
	//���ý�ɫ�������Խ���֪ͨ
	virtual void	SetAnimationCanBreakEvent(FUNC_ONANIMATIONEND pFunc, DWORD dwParam);
	//���ý�ɫ���������֪ͨ
	virtual void	SetAnimationHitEvent(FUNC_ONANIMATIONEND pFunc, DWORD dwParam);
	//���ý�ɫ���������֪ͨ
	virtual void	SetAnimationShakeEvent(FUNC_ONANIMATIONEND pFunc, DWORD dwParam);
	//����λ�� (��Ϸ��������)
	virtual VOID	SetPosition(const fVector3& vPos);

    virtual VOID    SetOrientation(const fVector3& vRotate);
	
	//
	virtual VOID	SetLeftWeaponEffect(LPCTSTR szEffectName, UINT color);
	virtual VOID	SetRightWeaponEffect(LPCTSTR szEffectName, UINT color);

	virtual VOID	SetRightWeaponMat(LPCTSTR szWeaponMat, LPCTSTR szMatName);
	virtual VOID	SetLeftWeaponMat(LPCTSTR szWeaponMat, LPCTSTR szMatName);
	//��Ч���
	virtual UINT	AddEffect( LPCSTR effectName, LPCSTR locatorName );
	virtual VOID	DelEffect( UINT effect );
	virtual VOID	DelAllEffect( VOID );

	WX::LogicModel*	GetActorImpl( VOID )
	{
		return mModel;
	}

protected:
    WX::LogicModel* mModel;
	
	WX::LogicModel*					m_leftWeaponModel;
	UINT								m_leftWeaponEffect;

	WX::LogicModel*					m_rightWeaponModel;
	UINT								m_rightWeaponEffect;

	BOOL			m_bVisible;

public:
	CRFObj_Actor();
	virtual ~CRFObj_Actor();
};
