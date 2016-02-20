/****************************************\
*										*
*			  WXObject					*
*										*
\****************************************/

#pragma once
#include "RFObject.h"

#include "Core/WXObject.h"

class CWXObj_Object : public CWXObject
{
public:
	//����Ⱦ�����ٽڵ�
	virtual VOID	Destroy(VOID);

	//����λ�� (��Ϸ��������)
	virtual VOID	SetPosition(const fVector3& vPos);
	//������ת�Ƕ�
	virtual VOID	SetOrientation(const fVector3& vRotate);

	virtual VOID	SetLeftWeaponEffect(LPCTSTR szEffectName, UINT color) {};
	virtual VOID	SetRightWeaponEffect(LPCTSTR szEffectName, UINT color) {};

	virtual VOID	SetRightWeaponMat(LPCTSTR szWeaponMat, LPCTSTR szMatName) {};
	virtual VOID	SetLeftWeaponMat(LPCTSTR szWeaponMat, LPCTSTR szMatName) {};

	//�����Ƿ��ܱ����ѡ��
	virtual VOID	SetRayQuery(BOOL bQuery) { m_bRayQuery = bQuery; }
	virtual	BOOL	GetRayQuery(VOID) const	{ return m_bRayQuery; }

	//ֱ����������
	virtual VOID	SetWXObjectProperty(const char* szPropertyName, const char* szPropertyValue);

public:
	CWXObj_Object(LPCTSTR szType);
	virtual ~CWXObj_Object();

protected:
	//����WXObject
	WX::ObjectPtr					m_pWXObject;
	//�Ƿ��ܱ����ѡ��
	BOOL							m_bRayQuery;
};