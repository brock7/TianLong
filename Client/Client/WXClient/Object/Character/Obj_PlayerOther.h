
#pragma once

#include "Obj_PlayerNPC.h"
/**
	������ҽ�ɫ��
*/

class CObject_Item_Equip;
class CObject_PlayerOther : public CObject_PlayerNPC
{
public:
	//���ؽ�ɫ����
	virtual CHARACTER_TYPE	GetCharacterType( VOID ) const { return CT_PLAYEROTHER;	}
	//�����������ϸ��Ϣ������
	virtual VOID			SendEuipRequest(VOID);
	// ָ�����, �����ƽ�ɫ��Ψһ�ӿ�
	virtual RC_RESULT		OnCommand(const SCommand_Object *pCmd );

	// ���ָ��ķ���
	virtual BOOL			FillMouseCommand_Left( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill );
	// �Ҽ�ָ��ķ���
	virtual BOOL			FillMouseCommand_Right( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill );


	//--------------------------------------------------------
	//װ����Ʒ
	// װ��װ������Ʒ
			VOID			EquipItem_BodyPart(HUMAN_EQUIP nPart, INT nID );	
	// װ��ҽ�����Ʒ
			VOID			EquipItem_BodyLocator(HUMAN_EQUIP nPart, INT nID );	
	//ж����Ʒ(��ʾȱʡ����)
	virtual VOID			UnEquipItem( HUMAN_EQUIP nPart);

	//------------------------------------------
	//�߼����ݷ����ı�
	//------------------------------------------
protected:
	friend class CCharacterData;
	virtual VOID			OnDataChanged_RaceID(VOID);
	virtual VOID			OnDataChanged_FaceMesh(VOID);
	virtual VOID			OnDataChanged_HairMesh(VOID);
	virtual VOID			OnDataChanged_HairColor(VOID);
	virtual VOID			OnDataChanged_EquipVer(VOID);
	virtual VOID			OnDataChanged_Equip(HUMAN_EQUIP point);
//	virtual VOID			OnDataChanged_Titles(INT index);
	virtual VOID			OnDataChanged_FaceImage(VOID);			// ͷ��ı��¼�����, 2006-3-9

protected:
	VOID					UpdateFaceMesh(VOID);
	VOID					UpdateHairMesh(VOID);
	VOID					UpdateHairColor(VOID);
	VOID					UpdateEquip(HUMAN_EQUIP point);

protected:
	virtual VOID			OnHideWeapon(INT nAppointedWeaponID);
	virtual VOID			OnShowWeapon(VOID);

protected:
	// RenderInterface����/ɾ��
	virtual VOID			CreateRenderInterface(VOID);

	virtual INT				CalcCharModelID( VOID )const;

	virtual UINT			GetIdleInterval(VOID)const;

	virtual VOID			UpdateCharRace(VOID);

public:
	//�����ܲ��ٶ�
			VOID			Debug_ChangeSpeed(FLOAT fStep);
	//�����ܲ�Ƶ��
			VOID			Debug_ChangeRate(FLOAT fStep);
			FLOAT			Debug_GetRate(VOID) { return m_fDebugRate; }
protected:

	//CharRace���еĶ���
	const _DBC_CHAR_RACE*	m_pCharRace;

	//-- for debug
	//��ɫ���������ٶ�
	FLOAT					m_fDebugRate;
	//-- for debug

	//--------------------------------------------------------
	//���ļ̳�
	//--------------------------------------------------------
public:
	CObject_PlayerOther();
	virtual ~CObject_PlayerOther();

	///���ݳ�ʼ�����壬��ͬ������Ⱦ��
	virtual	VOID			Initial(VOID*);

protected:
	WX_DECLARE_DYNAMIC(CObject_PlayerOther);
	//TD_DECLARE_LOGICAL(true);
};
