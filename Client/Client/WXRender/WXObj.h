#pragma once

#include "TDGfxSystem.h"
#include "FairyObject.h"

namespace Fairy
{
	class TerrainProjector;
};

class CFairyObj_ProjTex;
//��Fairy��ֱ���������Ⱦʵ��
class CFairyObject : public tEntityNode
{
public:
	Fairy::ObjectPtr	GetFairyObj(VOID) { return m_pFairyObj; }
	//�õ�����
	virtual LPCTSTR GetType(VOID) const	 
	{ 
		if( m_pFairyObj) return m_pFairyObj->getType().c_str();
		else return "";
	}
	//����Ⱦ�����ٽڵ�
	virtual VOID	Destroy(VOID);

	//����MeshFile
	virtual VOID	SetMeshFile(LPCTSTR szMeshFile)			;
	//��������ģ����Դ��
	virtual VOID	SetActorFile(LPCTSTR szActorFile)		;
	//����λ�� (��Ϸ��������)
	virtual VOID	SetPosition(const fVector3& vPos)		;
	//������ת�Ƕ�
	virtual VOID	SetOrientation(const fVector3& vRotate)	;
	//�����Ƿ�ɼ�
	virtual VOID	SetVisible(BOOL bVisible)				;
	
	//����Ͷ������
	virtual VOID	SetProjTexture(PROJTEX_TYPE type, BOOL bShow, FLOAT fHeight=1.7f);
	
	//�����Ƿ��ܱ����ѡ��
	virtual VOID	SetRayQuery(BOOL bQuery) { m_bRayQuery = bQuery; }
	virtual	BOOL	GetRayQuery(VOID) const	{ return m_bRayQuery; }

	//��������ģ�͵�����
	virtual VOID	SetActorObjectProperty(LPCTSTR szName, LPCTSTR szValue);
	virtual VOID	GetActorObjectProperty(LPCTSTR szName, STRING& szValue);
	//ȡ������ģ���ϵ�ĳ�ؼ���λ��
	virtual VOID	GetActorLocator(LPCTSTR szName, fVector3& fvPosition);

	//ȡ��"ͷ��״̬��"����Ļ�ϵ�λ��,�������FALSE����ʾ����Ļ֮��,����û�иõ�
	virtual BOOL	GetInfoBoardPos(iVector2& ivPos);

	//�������ﶯ������֪ͨ
	virtual VOID	SetAnimationEndEvent(FUNC_ONANIMATIONEND pFunc, DWORD dwParam);

	//------------------------------------------------------------
	//����/��ʽ
public:
	//����ĳ��ʽ
	// bAnim		- TRUE(���ö���)/FALSE(������ʽ)
	// szSkillName	- ������������ʽ��
	// bLoop		- �Ƿ�ѭ��
	virtual VOID	Skill_Enter(BOOL bAnim, LPCTSTR szSkillName, BOOL bLoop);
	//����ȱʡ����
	virtual VOID	Skill_SetDefaultAnim(LPCTSTR szAnimName);

public:
	CFairyObject(LPCTSTR szType);
	virtual ~CFairyObject(VOID);

protected:
	//����FairyObject
	Fairy::ObjectPtr					m_pFairyObj;
	//����Ͷ����Ӱ
	std::vector< CFairyObj_ProjTex* >	m_vProjTex;
	//�Ƿ��ܱ����ѡ��
	BOOL								m_bRayQuery;
	//��ʾ����
	BOOL								m_bVisible;


	//ѡ��
	Fairy::TerrainProjector*	m_pUnitSelect;
	//ѡ�񻷹ҽӵ�
	Ogre::SceneNode*			m_pUnitSelectNode;
	//��Ӱ�ߵ�
	Fairy::TerrainProjector*	m_pShadowBlob;
	//��Ӱ�ߵ�ҽӵ�
	Ogre::SceneNode*			m_pShadowBlobNode;
	//����
	STRING						m_strType;
};
