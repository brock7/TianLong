#pragma once
#include "TDGameInterface.h"
#include "type.h"
#include "../DataPool/DetailAttrib_Player.h"
#include "../Object/Item/Obj_Item.h"

class CObject_ProjTex_MouseTarget;
class tActionItem;
class CGameInterface : public tGameInterfaceBase
{
public:
	//------------------------------------------------
	//ϵͳ����
	//------------------------------------------------
	// �������ϵ��
	
	//��ĳ������ѡ��Ϊ��ҪĿ��
	virtual VOID	Object_SelectAsMainTarget(INT idObj, int iType = 0);
	//��ʾ�Ҽ��˵�
	virtual VOID	Object_ShowContexMenu(INT idObj,BOOL showMyself = FALSE);

	//------------------------------------------------
	// ���

	//�ƶ���������ĳ��λ��
	virtual VOID	Player_MoveTo(const fVector2&  fvAimPos);
	virtual VOID	Player_AUTOHit( const int AUTOHit );
	//ʹ�ü���(˲��)
	virtual VOID	Player_UseSkill( INT idSkill);
	//ʹ�ü���(��ĳ��Obj)
	virtual VOID	Player_UseSkill( INT idSkill, INT idObj);
	//ʹ�ü���(��ĳ����Χ)
	virtual VOID	Player_UseSkill( INT idSkill, const fVector2& fvPos);
	//ʹ�ü���(��ĳ������)
	virtual VOID	Player_UseSkill( INT idSkill, FLOAT fDir);
	//ʹ�ü���(��ĳ������)
	virtual VOID	Player_UseSkill( INT idSkill, const GUID_t& guid);
	//�ϳ�ĳ�䷽()
	virtual VOID	Player_UseLifeAbility( INT idPrescription, INT nMakeCount );
	virtual VOID	Player_UseLifeAbility( INT idPrescription );
	//�������ϵ�װ��
	virtual VOID	Player_DestroyEquip(const tObject_Item* pEquip);
	//ж�����ϵ�װ��
	virtual VOID	Player_UnEquip(const tObject_Item* pEquip);
	//��������
	virtual VOID	Player_AskLevelUp(VOID);
	//����ϴһ�����Ե�
	virtual VOID	Player_AskCleanBat1Point(VOID);
	//̸��(��ĳ��NPC)
	virtual VOID	Player_Speak(INT idObj);

	//------------------------------------------------
	//��ͼ��ĳ��TripperObj���в�������������Զ�����߹�ȥ
	virtual VOID	TripperObj_Active( UINT nId );
	//��ͼ����ǰ������ĵڼ���
	virtual VOID	ItemBox_PickItem(tObject_Item* pItem);

	//------------------------------------------------
	//ʹ�ð��������Ʒ_ͨ��
	virtual VOID	PacketItem_UserItem(tActionItem* pActionItem, int nTargetServerID, fVector2& fvPos);
	//ʹ�ð��������Ʒ_����
	virtual VOID	PacketItem_UserItem_ToTarget( INT nIndex, UINT nTargetID);
	//ʹ�ð��������Ʒ_װ��
	virtual VOID	PacketItem_UserEquip( INT nIndex );
	//ʹ�ð��������Ʒ_��ʯ
	virtual VOID	PacketItem_UserGem(INT nGemIndex, INT EquipIndex);
	//���ٰ��������Ʒ
	virtual VOID	PacketItem_Destroy(INT nIndex);

	//------------------------------------------------
	//����GMָ��
	virtual VOID	GM_SendCommand( LPCSTR strCommand );

	virtual VOID    SendChatCommand(INT nChannelID, INT nChatType, INT nTeamID, LPCTSTR szContex, LPCTSTR szTargetName);

	//------------------------------------------------
	//����

	//����ȫ����Ⱦ��
	virtual VOID	Scene_SetPostFilter(LPCTSTR szFilterName);
	//����LightMap
	virtual VOID	Scene_SetLightmapQuality(INT nQuality);

	//------------------------------------------------
	//���õ�ǰ�����
	virtual VOID			Skill_SetActive(tActionItem* pActiveSkill);
	virtual tActionItem*	Skill_GetActive(VOID);

	//�ָ���ȱʡ���ܲ����������˳������������������������״̬
	virtual VOID			Skill_CancelAction(VOID);

	//------------------------------------------------
	//������Ʒ
	virtual VOID			Booth_BuyItem( tObject_Item* pItem );
	//������Ʒ
	virtual VOID			Booth_SellItem(tObject_Item* pItem );
	//��ɽ���
	virtual VOID			Booth_Close(VOID);
	//����
	virtual VOID			Booth_Repair(BOOL bAll, INT bagindex, BOOL isInBag = TRUE);
	//��������״̬
	virtual VOID			Booth_EnterRepair(VOID);

	//-------------------------------------------------
	//���³ƺ�
	virtual VOID			Title_UpdateCurTitle(BYTE titleType, SHORT titleData);

	virtual VOID			Stall_OpenStallSale(INT nObjId);

	//------------------------------------------------
	//��Ӫ�ж�
	virtual ENUM_RELATION	GetCampType( CObject *pObj_A, CObject *pObj_B );

protected:
	static CGameInterface* s_pMe;

	INT		Make_Count;
	INT		Make_Prescription;

public:
	CGameInterface();
	virtual ~CGameInterface();
	static CGameInterface*	GetMe(VOID) { return s_pMe; }
	
	//------------------------------------------------
	//���ļ̳�
	//------------------------------------------------
	virtual VOID	Initial(VOID*);
	virtual VOID	Tick(VOID);
	virtual VOID	Release(VOID);


	WX_DECLARE_DYNAMIC(CGameInterface);
};