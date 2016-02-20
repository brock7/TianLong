
#pragma once

#include "Obj_Character.h"

enum ENUM_NPC_TYPE
{
	NPC_TYPE_INVALID	= -1,
	NPC_TYPE_MONSTER,
	NPC_TYPE_NPC,
	NPC_TYPE_PET,

	NPC_TYPE_NUMBERS
};
/**
	NPC��ɫ�࣬��̳г� PlayerOther
*/
struct _DBC_CREATURE_ATT;
class CObject_PlayerNPC : public CObject_Character
{
public:
	//���ؽ�ɫ����
	virtual CHARACTER_TYPE		GetCharacterType( VOID ) const { return CT_MONSTER;	}
	ENUM_NPC_TYPE				GetNpcType(VOID)const;
	VOID						SetNpcType(ENUM_NPC_TYPE eType);
	const _DBC_CREATURE_ATT*	GetCreatureAtt(void) const { return m_pCreatureInfo; }

	// ������GUID
	virtual VOID				SetServerGUID(GUID_t guid) { m_GUIDServer = guid; }
	virtual GUID_t				GetServerGUID(VOID )const { return m_GUIDServer; }

	// ���ָ��ķ���
	virtual BOOL				FillMouseCommand_Left( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill );
	// �Ҽ�ָ��ķ���
	virtual BOOL				FillMouseCommand_Right( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill );

protected:
	// RenderInterface����/ɾ��
	virtual VOID				CreateRenderInterface(VOID);

	virtual INT					CalcCharModelID( VOID )const;

	virtual UINT				GetIdleInterval(VOID)const;

	virtual VOID				UpdateCharRace(VOID);

protected:
	virtual VOID				OnDataChanged_RaceID(VOID);

protected:
	//�����ݱ��е��߼����ݽṹ
	const _DBC_CREATURE_ATT*	m_pCreatureInfo;

	// GUID ����������
	GUID_t						m_GUIDServer; 

	ENUM_NPC_TYPE				m_eNpcType;

public:
	virtual INT					CannotBeAttack( VOID )const;

	/// ���ƶ�NPC�Ի�ͣ��������Ҫ����NPC��ת������ frankwu
public:
	VOID						SetNeedFaceToPlayer(BOOL bValue) { m_bNeedFaceToPlayer = bValue; }
	BOOL						GetNeedFaceToPlayer(VOID) const { return m_bNeedFaceToPlayer; }
	/// �Ƿ��ת�����
	BOOL						IsCanFaceToPlayer(VOID);
	/// �Ƿ��ת�����
	BOOL						IsDisplayBoard(VOID);
	// ͷ����Ϣ��߶�
	FLOAT						GetBoardHeight(VOID);
	// ѡ�л���С
	virtual FLOAT				GetProjtexRange(VOID);
	// ��Ӱ��С
	virtual FLOAT				GetShadowRange(VOID);

	LPCSTR						GetPortrait(VOID);

protected:
	VOID						FaceToPlayer(VOID);
	VOID						StopFaceToPlayer(VOID);

	VOID						ProcessFaceToPlayer(VOID); // ����NPCת��

private:
	BOOL						m_bNeedFaceToPlayer;
	BOOL						m_bFaceToPlayer;
	UINT						m_uStartFaceToPlayerTime;
	///��Obj��ת֮ǰ�ĽǶ�,��ʼ��Ϊ<0.0f, 0.0f, 0.0f>
	fVector3					m_fvPreRotation;

protected:
	virtual VOID				CharacterLogic_OnStopped(BOOL bFinished);
	virtual BOOL				Tick_Idle(UINT uElapseTime);
	const fVector3&				GetPreRotation(VOID)const				{ return m_fvPreRotation;			}

	//--------------------------------------------------------
	//���ļ̳�
	//--------------------------------------------------------
public:
	CObject_PlayerNPC();
	virtual ~CObject_PlayerNPC();

	///���ݳ�ʼ�����壬��ͬ������Ⱦ��
	virtual	VOID			Initial(VOID*);

public:
	WX_DECLARE_DYNAMIC(CObject_PlayerNPC);
	//TD_DECLARE_LOGICAL(true);
};
