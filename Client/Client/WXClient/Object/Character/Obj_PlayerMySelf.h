// Obj_PlayerMySelf.h
//
// ��ʾ�Լ��Ľ�ɫ��
//
//////////////////////////////////////////////////////

#pragma once

#include "Obj_PlayerOther.h"

/*
�����������
*/
class CObject_PlayerMySelf : public CObject_PlayerOther
{
public:
	//// ѹ��һ��ָ��
	//virtual BOOL				PushCommand(const SCommand_Object *pCmd );

	// ���ָ��ķ���
	virtual BOOL				FillMouseCommand_Left( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill );
	// �Ҽ�ָ��ķ���
	virtual BOOL				FillMouseCommand_Right( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill );

	virtual VOID				Tick_UpdateInfoBoard(VOID);

public:
	//���ؽ�ɫ����
	virtual CHARACTER_TYPE	GetCharacterType( VOID ) const { return CT_PLAYERMYSELF;	}
	BOOL					IsInVisualFiled( CObject_Character* pObj );

	//------------------------------------------
	//�߼����ݷ����ı�
	//------------------------------------------
protected:
	friend class CCharacterData;
	virtual VOID			SetPosition(const fVector3& fvPosition);
	virtual VOID			OnDataChanged_RaceID(VOID);
	virtual VOID			OnDataChanged_HairMesh(VOID);
	virtual VOID			OnDataChanged_HairColor(VOID);
	virtual VOID			OnDataChanged_FaceMesh(VOID);
	virtual VOID			OnDataChanged_Equip(HUMAN_EQUIP point);
	virtual VOID			OnDataChanged_FaceImage(VOID);			// ͷ��ı��¼�����, 2006-3-9
	virtual VOID			OnDataChanged_Die(VOID);
	virtual VOID			OnDataChanged_Level(VOID);
protected:
	//����UI��ʾ���߼�����
	CObject_PlayerOther*	m_pAvatar;
	//����UI��ʾ���߼�����
	CObject_PlayerOther*	m_pHeadAvatar;

	//------------------------------------------
	//
	//------------------------------------------
public:
	INT		GetSimulantLogicCount(VOID)const{return m_nSimulantLogicCount; }
	VOID	SetSimulantLogicCount(INT nLogicCount){ m_nSimulantLogicCount = nLogicCount; }

	virtual VOID	ResetLogicCount(VOID);

protected:
	INT		m_nSimulantLogicCount;

	//------------------------------------------
	//���ļ̳�
	//------------------------------------------
public:
	CObject_PlayerMySelf( VOID );
	virtual ~CObject_PlayerMySelf( VOID );

	///���ݳ�ʼ�����壬��ͬ������Ⱦ��
	virtual	VOID			Initial(VOID*);
	virtual VOID			Release(VOID);

protected:
	WX_DECLARE_DYNAMIC(CObject_PlayerMySelf);
	//TD_DECLARE_LOGICAL(true);
};
