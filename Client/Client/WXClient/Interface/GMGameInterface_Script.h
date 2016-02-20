#pragma once

namespace LuaPlus
{
	class LuaState;
	class LuaObject;
}
class CObject_Character;
namespace SCRIPT_SANDBOX
{

//��½�����У����ڵ��ȷ����Ĳ���(-- for debug)
INT	Lua_ServerLogin(LuaPlus::LuaState* state);

//Ҫ���˳�����
INT Lua_QuitApplication(LuaPlus::LuaState* state);

//Ҫ���/�رռ�����
INT	Lua_ToggleSkillBook(LuaPlus::LuaState* state);
//Ҫ��򿪼�����
INT Lua_OpenSkillBook(LuaPlus::LuaState* state);

//Ҫ���/�رճ������
INT	Lua_TogglePetPage(LuaPlus::LuaState* state);
//Ҫ���/�ر��Զ���ֽ���
INT Lua_ToAUTOHit(LuaPlus::LuaState* state);
//Ҫ�������ܽ���
INT Lua_OpenLifePage(LuaPlus::LuaState* state);

//Ҫ�����ͨ���ܽ���
INT Lua_OpenCommonSkillPage(LuaPlus::LuaState* state);
//�򿪰���
INT Lua_ToggleContainer(LuaPlus::LuaState* state);  //fujia 2007.10.24
INT Lua_ToggleContainer1(LuaPlus::LuaState* state);
INT Lua_ToggleContainer2(LuaPlus::LuaState* state);

//������
INT Lua_ToggleMission(LuaPlus::LuaState* state);

//�����ʾ��Ϣ
INT Lua_PushDebugMessage(LuaPlus::LuaState* state);

//�õ�ĳ������������Ŀ
INT	Lua_GetActionNum(LuaPlus::LuaState* state);

//�������ݳ��ڴӷ���������������ʦ��������Ϣ
//INT Lua_GetMenpaiInfo(LuaPlus::LuaState* state);

//ͨ���������õ���ͬ�ȼ�����ID
INT GetStudySkillLevelId(int SkillId,int skillLevel);

//�õ�ѧϰ���ܵĽ�ǮҪ��
INT Lua_GetStudySkillMoney(LuaPlus::LuaState* state);

//�õ�����������Ϣ
INT Lua_GetStudySkillMS(LuaPlus::LuaState* state);

//�õ�ѧϰ���ܵĵȼ�Ҫ��
INT Lua_GetStudySkillLevel(LuaPlus::LuaState* state);

//�õ���������
INT Lua_GetStudySkillName(LuaPlus::LuaState* state);

//�õ�����ͼ��
INT Lua_GetSkillImage(LuaPlus::LuaState* state);

//�����ķ�������Ҫ�Ľ�Ǯ
INT Lua_GetUplevelXinfaSpendMoney(LuaPlus::LuaState* state);

//�����ķ�������Ҫ�ľ���ֵ
INT Lua_GetUplevelXinfaSpendExp(LuaPlus::LuaState* state);

//������ǰ�ķ��ȼ�
//INT Lua_GetXinfaLevel(LuaPlus::LuaState* state);

INT Lua_GetCurrentTime( LuaPlus::LuaState* state);

//��õ�ǰ�����λ�ã���Ļ���꣩
INT Lua_GetCurMousePos( LuaPlus::LuaState* state);

//��������
//INT Lua_MenpaiFrameJoin(LuaPlus::LuaState* state);

//ѧϰ�����ķ� 
INT lua_SkillsStudyFrame_study(LuaPlus::LuaState* state);

//�õ���������
INT	Lua_EnumAction(LuaPlus::LuaState* state);

//��ʾ�Ҽ��˵�
INT	Lua_ShowContexMenu(LuaPlus::LuaState* state);

//���񴰿�ѡ���
INT Lua_QuestFrameOptionClicked(LuaPlus::LuaState* state);

//�����Ƿ�Show��״̬
INT	Lua_IsWindowShow(LuaPlus::LuaState* state);

//��ʼ�ϳ�
INT Lua_ComposeItem_Begin(LuaPlus::LuaState* state);

//ȡ���ϳ�
INT Lua_ComposeItem_Cancel(LuaPlus::LuaState* state);

//��������
INT Lua_QuestFrameAcceptClicked(LuaPlus::LuaState* state);

//л������
INT Lua_QuestFrameRefuseClicked(LuaPlus::LuaState* state);

//��������
INT Lua_QuestFrameMissionContinue(LuaPlus::LuaState* state);

//�������
INT Lua_QuestFrameMissionComplete(LuaPlus::LuaState* state);

//�������
INT Lua_QuestFrameMissionAbnegate(LuaPlus::LuaState* state);

//Ҫ���/�رպϳɽ���
INT	Lua_ToggleComposeWnd(LuaPlus::LuaState* state);
//ö�����е��䷽
INT	Lua_EnumMethod(LuaPlus::LuaState* state); 
// �õ�ĳһ���䷽������
INT Lua_GetMethodNumber( LuaPlus::LuaState* state); 
//������Ϣ���
INT Lua_AxTrace(LuaPlus::LuaState* state);

// �õ���ǰ�����óߴ�
INT Lua_GetSceneSize( LuaPlus::LuaState* state); 

// �õ���ǰ����id
INT Lua_GetSceneID( LuaPlus::LuaState* state); 

// �õ���ǰС��ͼ�óߴ�
INT Lua_GetMinimapSize( LuaPlus::LuaState* state);

// ����������Ϣ
INT Lua_SendChatMessage( LuaPlus::LuaState* state);

INT Lua_GetCurChatProperty( LuaPlus::LuaState* state );
// �رմ򿪴��ͼ
INT Lua_ToggleLargeMap( LuaPlus::LuaState* state );
INT Lua_ToggleSceneMap( LuaPlus::LuaState* state );
INT Lua_OpenMinimap( LuaPlus::LuaState* state );
INT Lua_UpdateMinimap( LuaPlus::LuaState* state );
// �ұ�
INT Lua_ResetCamera( LuaPlus::LuaState* state );
//�ű�����Կ���������ɫ�Ĺ��ܺ���

//�򿪹ر����˽��׽���
INT Lua_OpenBooth( LuaPlus::LuaState* state );
INT Lua_CloseBooth( LuaPlus::LuaState* state);
INT Lua_RepairAll( LuaPlus::LuaState* state);
INT Lua_RepairOne( LuaPlus::LuaState* state);
INT Lua_GetCurrentSceneName( LuaPlus::LuaState* state );
INT Lua_GetCurrentSceneNameById( LuaPlus::LuaState* state );
INT Lua_GetOperationType( LuaPlus::LuaState* state );
INT Lua_GetShopType( LuaPlus::LuaState* state );


//�򿪹رճƺŽ���
INT Lua_OpenTitleList( LuaPlus::LuaState* state);
INT Lua_CloseTitleList( LuaPlus::LuaState* state); 

INT Lua_OpenCharacter( LuaPlus::LuaState* state );

INT Lua_OpenEquip( LuaPlus::LuaState* state );

INT Lua_GetActionInfo( LuaPlus::LuaState* state );

// ��װ������
INT Lua_OpenSelfEquip( LuaPlus::LuaState* state);

INT Lua_AutoHit(LuaPlus::LuaState* state);
//��������
INT Lua_AskLevelUp(LuaPlus::LuaState* state);
//����gmָ��
INT Lua_SendGMCommand(LuaPlus::LuaState* state);

INT Lua_SendAUTOMove(LuaPlus::LuaState* state);

INT Lua_ShowSupterTooltip( LuaPlus::LuaState* state);

INT Lua_Get_XParam_INT(LuaPlus::LuaState* state);

INT Lua_Get_XParam_STR(LuaPlus::LuaState* state);

INT Lua_Clear_XSCRIPT(LuaPlus::LuaState* state);

INT Lua_Send_XSCRIPT(LuaPlus::LuaState* state);

INT Lua_Set_XSCRIPT_Parameter(LuaPlus::LuaState* state);

INT Lua_Set_XSCRIPT_Function_Name(LuaPlus::LuaState* state);

INT Lua_Set_XSCRIPT_ScriptID(LuaPlus::LuaState* state);

INT Lua_Set_XSCRIPT_ParamCount(LuaPlus::LuaState* state);

//--------------------------------------------------------------------------
// ������

INT Lua_ShowTeamFuncMenu(LuaPlus::LuaState* state);		// ��Ա��ӳ��Ĺ��ܲ˵�.
INT Lua_ShowTeamInfoDlg(LuaPlus::LuaState* state);		// ��ʾ������Ϣ�Ի���.
INT Lua_FlashTeamButton(LuaPlus::LuaState* state);		// ��˸��ť.
INT Lua_SetCurSelMember(LuaPlus::LuaState* state);		// ���õ�ǰѡ��Ķ�Ա

// ������
//--------------------------------------------------------------------------


// ����һ��ȫ�ֱ���
INT Lua_SetGlobalInteger(LuaPlus::LuaState* state);
// ���һ��ȫ�ֱ���
INT Lua_GetGlobalInteger(LuaPlus::LuaState* state);

// Ҫװ������ϸ��Ϣ
INT Lua_AskEquipDetial(LuaPlus::LuaState* state);

// ������Ʒ
INT Lua_DiscardItem(LuaPlus::LuaState* state);

// ȡ������
INT Lua_DiscardItemCancelLocked(LuaPlus::LuaState* state);

// ��������
INT Lua_PlaySound(LuaPlus::LuaState* state);

// �ر����д���
INT	Lua_CloseAllWindow( LuaPlus::LuaState* state);

// target���ڵ����Ĳ˵�
INT	Lua_OpenTargetMenu( LuaPlus::LuaState* state);

INT Lua_GetMousePos( LuaPlus::LuaState* state);

// ��ʾϵͳ��ʾ�Ի���2006��3��27
INT Lua_ShowSystemTipInfo( LuaPlus::LuaState* state);

// �򿪸��Ķ�������Ի���2006��3��27
INT Lua_OpenChangeMinorPasswordDlg( LuaPlus::LuaState* state);

// �򿪽������롣2006��3��27
INT Lua_OpenUnLockeMinorPasswordDlg( LuaPlus::LuaState* state);

// ������������档2006��3��28
INT Lua_OpenSetMinorPasswordDlg( LuaPlus::LuaState* state);

// �������롣2006��3��28
INT Lua_SendSetMinorPassword( LuaPlus::LuaState* state);

// �޸����롣2006��3��28
INT Lua_SendModifyMinorPassword( LuaPlus::LuaState* state);

// �������롣2006��3��28
INT Lua_UnLockMinorPassword( LuaPlus::LuaState* state);

// ǿ�ƽ������롣2006��3��28
INT Lua_ForceUnLockMinorPassword( LuaPlus::LuaState* state);

// �޸����롣2006��3��28
INT Lua_ModifyMinorPassword( LuaPlus::LuaState* state);


//// �������֡����������ֵ�ID 2006��3��28
//INT Lua_PlayMusic( LuaPlus::LuaState* state);
//
//// ֹͣ���֡����������ֵľ�� 2006��3��28
//INT Lua_StopMusic( LuaPlus::LuaState* state);//


class Character
{
public:
	
	
	//ȡ��HP return (percent)
	INT		GetHPPercent(LuaPlus::LuaState* state);
	//ȡ��MP return (percent)
	INT		GetMPPercent(LuaPlus::LuaState* state);
	//ȡ��Rage return (percent)
	INT		GetRagePercent(LuaPlus::LuaState* state);

	INT		GetScenePos( LuaPlus::LuaState* state);

	
	//----------------------------------------------------------------------
	// ������

	//�������
	INT	SendTeamRequest(LuaPlus::LuaState* state);

	//����������
	INT	SendTeamApply(LuaPlus::LuaState* state);

	// ������
	//----------------------------------------------------------------------

	// ����
	INT		Follow(LuaPlus::LuaState* state);
	
// ���е�����
	// �õ���ɫ��λ��
	virtual INT		GetPos( LuaPlus::LuaState* state);
	//ȡ������
	virtual INT		GetName(LuaPlus::LuaState* state);
	//�����Ƿ���� "target", "pet", ...
	virtual INT		IsPresent(LuaPlus::LuaState* state);
	// �õ���ɫ���ϵ�buff������
	virtual INT     GetBuffNumber( LuaPlus::LuaState* state );

	// �õ��ȼ�
	INT				GetLevel(LuaPlus::LuaState* state);
	INT				GetData(LuaPlus::LuaState* state);
	INT				SendAskDetail(LuaPlus::LuaState* state);
	INT				Set_To_Private(LuaPlus::LuaState* state);
	INT				Close_Before_TargetEquip_UI(LuaPlus::LuaState* state);
	INT				TargetEquip_ChangeModel(LuaPlus::LuaState* state);

	virtual INT		GetBuffIconNameByIndex( LuaPlus::LuaState* state );
	virtual INT		GetDialogNpcName( LuaPlus::LuaState* state );
	virtual INT		GetShopNpcName( LuaPlus::LuaState* state );
	//virtual INT		GetXinfaNpcName(LuaPlus::LuaState* state);


public:

	 // ��ǰ��target �Ƿ��Ƕ���2006-4-3
	 INT	IsTargetTeamMember(LuaPlus::LuaState* state);

	 // ��ǰ��target �Ƿ�Ϸ�2006��4��12
	 INT	IsTargetValide(LuaPlus::LuaState* state);

	 // main target ������ʾ������Ϣ��2006��4��12

	// ��ʾ����
	INT TargetFrame_Update_Name_Team(LuaPlus::LuaState* state);

	// ��ʾhp
	INT TargetFrame_Update_HP_Team(LuaPlus::LuaState* state);

	// ��ʾmp
	INT	TargetFrame_Update_MP_Team(LuaPlus::LuaState* state);

	// ��ʾŭ��
	INT TargetFrame_Update_Rage_Team(LuaPlus::LuaState* state);

	// ��ʾ�ȼ�
	INT TargetFrame_Update_Level_Team(LuaPlus::LuaState* state);

	// ��ʾͷ�����֡�
	INT TargetFrame_Update_Icon_Team(LuaPlus::LuaState* state);

	INT IsEnemy( LuaPlus::LuaState* state );			//�Ƿ��ǵ���

protected:
	Character() {};
	virtual ~Character() {};

private:
	CObject_Character*		_GetTargetAsCharacter(VOID);

public:
	//��ѡ�����
	static Character s_Target;
};

//�ű�����Կ�����ҵĹ��ܺ���
class PlayerMySelf : public Character
{
public:


	// �õ���ǰ�ĳƺ�.
	INT	GetCurTitle(LuaPlus::LuaState* state);

	// ��ѡ��ƺ�
	INT SetNullAgname(LuaPlus::LuaState* state);
	
	//----------------------------------------------------------------------------------------------------------------------------------------------
	// ������
	
	// �뿪����
	INT LeaveTeam(LuaPlus::LuaState* state);	

	// ��ɢ����
	INT DismissTeam(LuaPlus::LuaState* state);	

	// ɾ��һ��ѡ�еĶ�Ա
	INT KickTeamMember(LuaPlus::LuaState* state);
	
	// ɾ����ǰѡ�ж�Ա
	INT KickTeamMemberByIndex(LuaPlus::LuaState* state);			

	//�Խ�����
	INT	CreateTeamSelf(LuaPlus::LuaState* state);

	//�����ӳ���Ϣ
	INT	AppointLeader(LuaPlus::LuaState* state);

	// �Ƿ�����Ӹ���״̬
	INT InTeamFollowMode(LuaPlus::LuaState* state);

	// ȡ������
	INT StopFollow(LuaPlus::LuaState* state);

	// ��Ӹ���
	INT TeamFrame_AskTeamFollow(LuaPlus::LuaState* state);

	// ͬ��������(����������)
	INT	SendAgreeJoinTeam(LuaPlus::LuaState* state);
	// �ܾ��������(����������)
	INT SendRejectJoinTeam(LuaPlus::LuaState* state);

	// ͬ��������(��������������)
	INT	SendAgreeJoinTeam_Apply(LuaPlus::LuaState* state);

	// �ܾ��������(��������������)
	INT SendRejectJoinTeam_Apply(LuaPlus::LuaState* state);

	// ͬ��������(��Ա����ĳ�˼������)
	INT	SendAgreeJoinTeam_TeamMemberInvite(LuaPlus::LuaState* state);

	// �ܾ��������(��Ա����ĳ�˼������)
	INT SendRejectJoinTeam_TeamMemberInvite(LuaPlus::LuaState* state);

	// ͬ�������Ӹ���ģʽ
	INT SendAgreeTeamFollow(LuaPlus::LuaState* state);

	// �ܾ�������Ӹ���ģʽ
	INT SendRefuseTeamFollow(LuaPlus::LuaState* state);

	// �õ���Աguid
	INT GetTeamMemberGUID(LuaPlus::LuaState* state);

	// ������
	//-----------------------------------------------------------------------------------------------------------------------------------------------


	//-----------------------------------------------------------------------------------------------------------------------------------------------
	// װ�����Խ���
	//

	// �����ֶ��������Ե��� 
	INT SendAskManualAttr(LuaPlus::LuaState* state);


	//
	// װ�����Խ���
	//-----------------------------------------------------------------------------------------------------------------------------------------------



	virtual INT		GetName(LuaPlus::LuaState* state);
	virtual INT		GetData( LuaPlus::LuaState* state );
	// �õ���ɫ��ĳ������ܵ���Ϣ
	virtual INT		GetAbilityInfo( LuaPlus::LuaState* state );
	// �õ���ɫ��ĳ��ս�����ܵ���Ϣ
	virtual INT		GetSkillInfo( LuaPlus::LuaState* state );
	// �õ���ɫ��ĳ���ķ�����Ϣ
	virtual INT		GetSkillClassInfo( LuaPlus::LuaState* state );
	// ����ѧϰ�����
	virtual INT		AskLeanAbility(LuaPlus::LuaState* state);

	// �õ���ɫ��λ��
	virtual INT		GetPos( LuaPlus::LuaState* state);
	// ��ɫ�Ƿ����
	virtual INT		IsPresent(LuaPlus::LuaState* state);

	//�ƺ����
	virtual INT		GetAgnameNum(LuaPlus::LuaState* state);
	virtual INT		EnumAgname(LuaPlus::LuaState* state);
	virtual INT		GetCurrentAgname(LuaPlus::LuaState* state);
	virtual INT		AskChangeCurrentAgname(LuaPlus::LuaState* state);


	
	// �õ���ɫ���ϵ�buff������
	virtual INT     GetBuffNumber( LuaPlus::LuaState* state );
	virtual INT		GetBuffIconNameByIndex( LuaPlus::LuaState* state );
	virtual INT		GetBuffToolTipsByIndex( LuaPlus::LuaState* state );
	virtual INT		DispelBuffByIndex( LuaPlus::LuaState* state );
	virtual INT		GetBuffTimeTextByIndex( LuaPlus::LuaState* state );

	// ����Ի������
	virtual INT		SendReliveMessage_OutGhost( LuaPlus::LuaState* state );
	virtual INT		SendReliveMessage_Relive( LuaPlus::LuaState* state );

	// �Ҽ��˵����
	virtual INT		ShowMySelfContexMenu(LuaPlus::LuaState* state);
	// ���ѡ���Լ�
	virtual INT		SelectMyselfAsTarget(LuaPlus::LuaState* state);

	// ���ö������� 2006-4-29
	INT	SetSupperPassword(LuaPlus::LuaState* state);

protected:
	PlayerMySelf() {};
	virtual ~PlayerMySelf() {};

public:
	static PlayerMySelf s_MySelf;
};

//���ݳ�
class DataPool
{
public:
	//ȡ�õ�ǰ�Ի�npc����
	INT		GetNPCEventList_Num(LuaPlus::LuaState* state);
	INT		GetNPCEventList_Item(LuaPlus::LuaState* state);

	//-----------------------------------------------------------
	//������Ϣ
	INT		GetMissionInfo_Num(LuaPlus::LuaState* state);
	INT		GetMissionInfo_Text(LuaPlus::LuaState* state);
	INT		GetMissionInfo_Bonus(LuaPlus::LuaState* state);
	INT		GetMissionInfo_Kind(LuaPlus::LuaState* state);

	//����������Ϣ
	INT		GetMissionDemand_Num(LuaPlus::LuaState* state);
	INT		CloseMissionFrame(LuaPlus::LuaState* state);
	INT		GetMissionDemand_Text(LuaPlus::LuaState* state);
	INT		GetMissionDemand_Item(LuaPlus::LuaState* state);
	//Continue
	INT		GetMissionContinue_Num(LuaPlus::LuaState* state);
	INT		GetMissionContinue_Text(LuaPlus::LuaState* state);
	INT		GetMissionContinue_Bonus(LuaPlus::LuaState* state);

	//��ҵ�ǰ������Ϣ
	INT		GetPlayerMission_Num(LuaPlus::LuaState* state);
	INT		GetPlayerMission_InUse(LuaPlus::LuaState* state);
	INT		GetPlayerMission_Description(LuaPlus::LuaState* state);
	INT		GetPlayerMissionDemand_Num(LuaPlus::LuaState* state);
	INT		GetPlayerMissionDemand_Item(LuaPlus::LuaState* state);
	INT		GetPlayerMissionCustom_Num(LuaPlus::LuaState* state);
	INT		GetPlayerMissionCustom(LuaPlus::LuaState* state);
	INT		GetPlayerMissionBonus_Num(LuaPlus::LuaState* state);
	INT		GetPlayerMissionBonus_Item(LuaPlus::LuaState* state);
	INT		GetPlayerMission_Memo(LuaPlus::LuaState* state);
	INT		GetPlayerMission_Name(LuaPlus::LuaState* state);
	INT		GetPlayerMission_BillName(LuaPlus::LuaState* state);
	INT		GetPlayerMission_Info(LuaPlus::LuaState* state);

	INT		GetPlayerMission_ItemCountNow(LuaPlus::LuaState* state);

	INT		GetPlayerMission_Variable(LuaPlus::LuaState* state);
	INT		GetPlayerMission_Display(LuaPlus::LuaState* state);
	INT		GetPlayerMission_RemainTime(LuaPlus::LuaState* state);
	INT		Mission_Abnegate_Popup(LuaPlus::LuaState* state);
	INT		Mission_Abnegate_Special_Quest_Popup(LuaPlus::LuaState* state);
	INT		EnumPlayerMission_ItemAction(LuaPlus::LuaState* state);
	INT		GetPlayerMission_DataRound(LuaPlus::LuaState* state);

	INT		GetPlayerMissionDemandKill_Num(LuaPlus::LuaState* state);
	INT		GetPlayerMissionDemand_NPC(LuaPlus::LuaState* state);
/*	INT		GetPlayerMissionBonus_Num(LuaPlus::LuaState* state);
	INT		GetPlayerMissionBonus_Item(LuaPlus::LuaState* state);
	INT		GetPlayerMission_Memo(LuaPlus::LuaState* state);
*/	INT		GetPlayerMission_Level(LuaPlus::LuaState* state);
	INT		GetPlayerMission_Kind(LuaPlus::LuaState* state);
	INT		GetPlayerMission_IsElite(LuaPlus::LuaState* state);


	//ȡ���䷽����
	INT		GetPrescrList_Num(LuaPlus::LuaState* state);
	INT		GetPrescrList_Item(LuaPlus::LuaState* state);
	INT		GetPrescrList_Item_LifeAbility(LuaPlus::LuaState* state);
	INT		GetPrescrList_Item_Result(LuaPlus::LuaState* state);
	INT		GetPrescrList_Item_Requirement(LuaPlus::LuaState* state);

	// ��û��ܵ���Ϣ 
	INT		GetBoothList_Num(LuaPlus::LuaState* state); // �õ��ж��ٸ�����
	INT		GetBoothGood_ID( LuaPlus::LuaState* state ); // �õ������id 
	INT		GetBoothGood_Name(LuaPlus::LuaState* state); // �õ����������
	INT		GetBoothGood_Price(LuaPlus::LuaState* state);// �õ�����ļ۸�
	

	//-----------------------------------------------------------------------------------------------------------------------------------
	// ������. 
	
	//------------------------------
	//��Щ��������ߵ�ͷ�������ʾ.
	INT		GetTeamMemGUIDByUIIndex(LuaPlus::LuaState* state);			// �õ���Ա����Ϣ
	INT		GetTeamMemInfo(LuaPlus::LuaState* state);			// �õ���Ա����Ϣ
	INT		GetTeamMemCount(LuaPlus::LuaState* state);			// �õ���Ա�ĸ���
	INT	    IsTeamLeader(LuaPlus::LuaState* state);				// �Ƿ��Ƕӳ�
	INT		SelectAsTargetByUIIndex(LuaPlus::LuaState* state);	// ͨ������ͷ��, ѡ�ж�����Ϊtarget



	//------------------------------
	// ���������.

	//��Щ�����������Ϣ����ʹ��.  ����ӿ�.   
	INT		GetInviteTeamCount(LuaPlus::LuaState* state);		// �õ��������ĸ���.
	INT     GetInviteTeamMemberInfo(LuaPlus::LuaState* state);	// �õ��������Ϣ.	
	INT		GetInviteTeamMemberCount(LuaPlus::LuaState* state);	// �õ�ĳ���������Ķ�Ա�ĸ���.


	
	//-----------------------------
	// �򿪶�����Ϣ.

	
	// ���ڶ����е��Լ�����Ϣ, ��ͨ��������Ϣ����, ÿ�δ򿪽����ʱ��Ҫˢ���Լ�����Ϣ
	INT     SetSelfInfo(LuaPlus::LuaState* state);

	// �ӳ��򿪶�����Ϣʱ, ѡ�е�����
	INT     SetSelectTeamMember_Leader(LuaPlus::LuaState* state);

	// ͨ�������õ���Ա��Ϣ, ����Ա�򿪽���ʹ��.
	INT	GetTeamMemInfoByIndex(LuaPlus::LuaState* state);	// �õ���Ա����Ϣ

	// �õ���Ա��uiģ������
	INT	GetTeamMemUIModelNameByIndex(LuaPlus::LuaState* state);

	// �õ������ߵ�uiģ������
	INT GetApplyMemberUIModelName(LuaPlus::LuaState* state);		

	// �õ�����uiģ������
	INT GetInviteTeamMemberUIModelName(LuaPlus::LuaState* state);	

	// �õ���Աuiģ������
	INT	GetTeamMemUIModelName(LuaPlus::LuaState* state);		

	// ��Ա���Լ��Ƿ���ͬһ��������
	INT	IsTeamMemberInScene(LuaPlus::LuaState* state);		

    


	//------------------------------
    // �������˽���

	//��Щ�����������Ϣ����ʹ��.  ����ӿ�. 
	INT		GetApplyMemberCount(LuaPlus::LuaState* state);		// �õ������߸���.
	INT     GetApplyMemberInfo(LuaPlus::LuaState* state);		// �õ������ߵĸ���.		
	INT     SetCurSelApply(LuaPlus::LuaState* state);			// ���õ�ǰѡ���������.
	INT     EraseApply(LuaPlus::LuaState* state);				// ɾ��һ��������
	INT     ClearAllApply(LuaPlus::LuaState* state);			// ������е�������.



	//------------------------------
	// �������ݽӿ�.

	// ������ӶԻ���򿪱��.
	INT		SetTeamFrameOpenFlag(LuaPlus::LuaState* state);		// ���ö���Ի���򿪱��


	//--------------------------------------------------------------------------------------------------------------------------------------



	//��������
	INT		GetPacketItem( LuaPlus::LuaState* state );
	INT		GetPacketItemMaxNumber( LuaPlus::LuaState* state );

	// ����ϵͳ�ӿ�

	INT		GetFriendNumber( LuaPlus::LuaState* state );	// �õ�ĳһ��ĺ�������
	INT		GetFriend( LuaPlus::LuaState* state );			//�õ�ĳһ��ĺ��ѵľ�������

	INT		AddFriend( LuaPlus::LuaState* state );			// ���һ�����ѵ�ĳһ��
	INT		DelFriend( LuaPlus::LuaState* state );			// ɾ��һ������

	INT     ThrowToBlackList( LuaPlus::LuaState* state );
	INT     ThrowToList( LuaPlus::LuaState* state );

	INT		EditMood( LuaPlus::LuaState* state );
	INT		ShowFriendInfo( LuaPlus::LuaState* state );
	INT		ShowChatInfo(LuaPlus::LuaState* state);
	INT		SetMood( LuaPlus::LuaState* state );
	INT		GetMood( LuaPlus::LuaState* state );
	INT		OpenFriendList(LuaPlus::LuaState* state);

	INT		GetMailNumber( LuaPlus::LuaState* state );
	INT		GetMail( LuaPlus::LuaState* state );

	INT 	OpenMail(LuaPlus::LuaState* state);

	INT		SendMail( LuaPlus::LuaState* state );

	INT		GetNextMail(LuaPlus::LuaState* state );

	INT     GetFriendByName( LuaPlus::LuaState* state );

	INT		OpenHistroy( LuaPlus::LuaState* state );
//�ı䷢�ͷ�ɫ
	INT		Change_MyHairColor( LuaPlus::LuaState* state );
	INT		Change_MyHairStyle( LuaPlus::LuaState* state );
	INT		Change_MyHairStyle_Item( LuaPlus::LuaState* state );
	INT		Get_MyHairColor( LuaPlus::LuaState* state );
	INT		Get_MyHairStyle( LuaPlus::LuaState* state );
//���String�Ƿ�Ϸ�
	INT		Check_StringCode( LuaPlus::LuaState* state );

public:

	INT		IsCanDoAction( LuaPlus::LuaState* state );
	INT		Change_RectifyColor( LuaPlus::LuaState* state );
	INT		Change_GetColorLumination(LuaPlus::LuaState* state);

public:

	// ��ʾ����
	INT TargetFrame_Update_Name_Team(LuaPlus::LuaState* state);

	// ��ʾhp
	INT TargetFrame_Update_HP_Team(LuaPlus::LuaState* state);

	// ��ʾmp
	INT	TargetFrame_Update_MP_Team(LuaPlus::LuaState* state);

	// ��ʾŭ��
	INT TargetFrame_Update_Rage_Team(LuaPlus::LuaState* state);

	// ��ʾ�ȼ�
	INT TargetFrame_Update_Level_Team(LuaPlus::LuaState* state);

	// ��ʾͷ�����֡�
	INT TargetFrame_Update_Icon_Team(LuaPlus::LuaState* state);

	INT	ToggleShowPetList(LuaPlus::LuaState* state);

	INT	GetNPCIDByServerID(LuaPlus::LuaState* state);

	INT GetUIColor(LuaPlus::LuaState* state);
	INT GetPlayerSkill_ID(LuaPlus::LuaState* state);
	INT GetPlayerSkill_Level(LuaPlus::LuaState* state);
	INT GetNPCobj_ID(LuaPlus::LuaState* state);
	INT GetPlayerSkill_Type(LuaPlus::LuaState* state);
	INT GetPlayerSkill_Num(LuaPlus::LuaState* state);
	// ���ö�������
	INT	SetSupperPass(LuaPlus::LuaState* state);

public:
	static DataPool s_DataPool;
	static LuaPlus::LuaObject* s_pMetaTable;
};

//��ǰ���ڶԻ����������ʦ
class AbilityTeacher
{
public:
	//ȡ�õ�ǰ��ʦ�Ƿ����
	INT		IsPresent(LuaPlus::LuaState* state);
	//�õ���ǰ��ʦ���̵ܽ������ID
	INT		GetAbilityID(LuaPlus::LuaState* state);
	//�õ���������һ������Ҫ�ľ���ֵ
	INT		GetNeedExp(LuaPlus::LuaState* state);
	//�õ���������һ������Ҫ�Ľ�Ǯ
	INT		GetNeedMoney(LuaPlus::LuaState* state);
	//�õ���������һ������Ҫ��������
	INT		GetNeedSkillExp(LuaPlus::LuaState* state);
	// ��÷�����������һЩ����
	INT		GetServerData(LuaPlus::LuaState* state);

public:
	static AbilityTeacher s_AbilityTeacher;
	static LuaPlus::LuaObject* s_pMetaTable;
};

class Pet
{
public:
	//ȡ�õ�ǰ�����Ƿ����
	INT		IsPresent(LuaPlus::LuaState* state);
	INT		GetPet_Count(LuaPlus::LuaState* state);
	INT		GetPetList_Appoint(LuaPlus::LuaState* state);
	INT		Go_Fight(LuaPlus::LuaState* state);
	INT		Go_Relax(LuaPlus::LuaState* state);
	INT		Go_Free(LuaPlus::LuaState* state);
	INT		Feed(LuaPlus::LuaState* state);
	INT		Dome(LuaPlus::LuaState* state);

	INT		GetName(LuaPlus::LuaState* state);
	INT		GetID(LuaPlus::LuaState* state);
	INT		GetConsort(LuaPlus::LuaState* state);
	INT		GetSex(LuaPlus::LuaState* state);
	INT		GetNaturalLife(LuaPlus::LuaState* state);
	INT		GetMaxLife(LuaPlus::LuaState* state);
	INT		GetLoyalgGade(LuaPlus::LuaState* state);
	INT		GetLevel(LuaPlus::LuaState* state);
	INT		GetType(LuaPlus::LuaState* state);
	INT		GetHappy(LuaPlus::LuaState* state);
	INT		GetStrAptitude(LuaPlus::LuaState* state);
	INT		GetPFAptitude(LuaPlus::LuaState* state);
	INT		GetDexAptitude(LuaPlus::LuaState* state);
	INT		GetIntAptitude(LuaPlus::LuaState* state);
	INT		GetStaAptitude(LuaPlus::LuaState* state);
	
	INT		GetHP(LuaPlus::LuaState* state);
	INT		GetMaxHP(LuaPlus::LuaState* state);
	INT		GetMP(LuaPlus::LuaState* state);
	INT		GetMaxMP(LuaPlus::LuaState* state);

	INT		GetStr(LuaPlus::LuaState* state);
	INT		GetInt(LuaPlus::LuaState* state);
	INT		GetDex(LuaPlus::LuaState* state);
	INT		GetPF(LuaPlus::LuaState* state);
	INT		GetSta(LuaPlus::LuaState* state);

	INT		GetBasic(LuaPlus::LuaState* state);
	INT		GetPotential(LuaPlus::LuaState* state);
	INT		GetExp(LuaPlus::LuaState* state);

	INT		GetPhysicsAttack(LuaPlus::LuaState* state);
	INT		GetMagicAttack(LuaPlus::LuaState* state);
	INT		GetPhysicsRecovery(LuaPlus::LuaState* state);
	INT		GetMagicRecovery(LuaPlus::LuaState* state);

	INT		GetMiss(LuaPlus::LuaState* state);
	INT		GetShootProbability(LuaPlus::LuaState* state);

	INT		Add_Attribute(LuaPlus::LuaState* state);
	INT		Change_Name(LuaPlus::LuaState* state);

	INT		GetIsFighting(LuaPlus::LuaState* state);
	INT		SetModel(LuaPlus::LuaState* state);
	INT		GetDeathWarrant(LuaPlus::LuaState* state);
	
	//yangjun add for petskillstudy
	INT		SetSkillStudyModel(LuaPlus::LuaState* state);
	INT		ShowTargetPet(LuaPlus::LuaState* state);
	INT		SkillStudy_Do(LuaPlus::LuaState* state);
	INT		SkillStudyUnlock(LuaPlus::LuaState* state);
	INT		SkillStudy_MenPaiSkill_Created(LuaPlus::LuaState* state);
	INT		SkillStudy_MenPaiSkill_Clear(LuaPlus::LuaState* state);
	INT		GetGUID(LuaPlus::LuaState* state);
	INT		ShowPetList(LuaPlus::LuaState* state);

	//yangjun add for petcontexmenu
	INT		ShowMyPetContexMenu(LuaPlus::LuaState* state);
	INT		HandlePetMenuItem(LuaPlus::LuaState* state);

	//yangjun add for petImpact
	INT		GetPetPortraitByIndex(LuaPlus::LuaState* state);
	INT		GetPetImpactNum(LuaPlus::LuaState* state);
	INT		GetPetImpactIconNameByIndex(LuaPlus::LuaState* state);

	INT		Lua_EnumPetSkill(LuaPlus::LuaState* state);
	INT		Select_Pet(LuaPlus::LuaState* state);
	INT		GetAIType(LuaPlus::LuaState* state);
	INT		GetSkillPassive(LuaPlus::LuaState* state);
	INT		Free_Confirm(LuaPlus::LuaState* state);

public:
	static Pet s_Pet;
	static LuaPlus::LuaObject* s_pMetaTable;
};


class TargetPet
{
public:
	//ȡ�õ�ǰ�����Ƿ����
	INT		IsPresent(LuaPlus::LuaState* state);
//	INT		GetPet_Count(LuaPlus::LuaState* state);
//	INT		GetPetList_Appoint(LuaPlus::LuaState* state);
//	INT		Go_Fight(LuaPlus::LuaState* state);
//	INT		Go_Relax(LuaPlus::LuaState* state);

	INT		GetName(LuaPlus::LuaState* state);
	INT		GetID(LuaPlus::LuaState* state);
	INT		GetConsort(LuaPlus::LuaState* state);
	INT		GetSex(LuaPlus::LuaState* state);
	INT		GetNaturalLife(LuaPlus::LuaState* state);
	INT		GetMaxLife(LuaPlus::LuaState* state);
	INT		GetLoyalgGade(LuaPlus::LuaState* state);
	INT		GetLevel(LuaPlus::LuaState* state);
	INT		GetType(LuaPlus::LuaState* state);
	INT		GetHappy(LuaPlus::LuaState* state);
	INT		GetStrAptitude(LuaPlus::LuaState* state);
	INT		GetPFAptitude(LuaPlus::LuaState* state);
	INT		GetDexAptitude(LuaPlus::LuaState* state);
	INT		GetIntAptitude(LuaPlus::LuaState* state);
	INT		GetStaAptitude(LuaPlus::LuaState* state);
	
	INT		GetHP(LuaPlus::LuaState* state);
	INT		GetMaxHP(LuaPlus::LuaState* state);
	INT		GetMP(LuaPlus::LuaState* state);
	INT		GetMaxMP(LuaPlus::LuaState* state);

	INT		GetStr(LuaPlus::LuaState* state);
	INT		GetInt(LuaPlus::LuaState* state);
	INT		GetDex(LuaPlus::LuaState* state);
	INT		GetPF(LuaPlus::LuaState* state);
	INT		GetSta(LuaPlus::LuaState* state);

	INT		GetBasic(LuaPlus::LuaState* state);
	INT		GetPotential(LuaPlus::LuaState* state);
	INT		GetExp(LuaPlus::LuaState* state);

	INT		GetPhysicsAttack(LuaPlus::LuaState* state);
	INT		GetMagicAttack(LuaPlus::LuaState* state);
	INT		GetPhysicsRecovery(LuaPlus::LuaState* state);
	INT		GetMagicRecovery(LuaPlus::LuaState* state);

	INT		GetMiss(LuaPlus::LuaState* state);
	INT		GetShootProbability(LuaPlus::LuaState* state);

//	INT		Add_Attribute(LuaPlus::LuaState* state);
//	INT		Change_Name(LuaPlus::LuaState* state);

	INT		GetIsFighting(LuaPlus::LuaState* state);
	INT		SetModel(LuaPlus::LuaState* state);
	INT		CopyMyPet(LuaPlus::LuaState* state);
	INT		GetDeathWarrant(LuaPlus::LuaState* state);
	INT		GetAIType(LuaPlus::LuaState* state);

public:
	static TargetPet s_TargetPet;
	static LuaPlus::LuaObject* s_pMetaTable;
};

class Bank
{
public:
	//�ĵ������Ƿ����
	INT IsPresent(LuaPlus::LuaState* state);
	//�򿪴�Ǯ���棨�����Ǯ�����Ľ��棩
	INT OpenSaveFrame(LuaPlus::LuaState* state);
	//��ȡǮ���棨����ȡǮ�����Ľ��棩
	INT OpenGetFrame(LuaPlus::LuaState* state);
	//��Ǯ��ת��
	INT GetInputMoney(LuaPlus::LuaState* state);
	//���ʹ�Ǯ��Ϣ
	INT SaveMoneyToBank(LuaPlus::LuaState* state);
	//����ȡǮ��Ϣ
	INT GetMoneyFromBank(LuaPlus::LuaState* state);
	//�������еĽ�Ǯ��
	INT GetBankMoney(LuaPlus::LuaState* state);
	//�������е�Ԫ����
	INT GetBankRMB(LuaPlus::LuaState* state);
	//���ʹ�Ԫ����Ϣ
	INT SaveRMBToBank(LuaPlus::LuaState* state);
	//����ȡԪ����Ϣ
	INT GetRMBFromBank(LuaPlus::LuaState* state);
	//����һ���ƶ�������Ʒ����������Ϣ
	INT MoveItemToPacket(LuaPlus::LuaState* state);
	//��������ӵ�е�������ĸ���
	INT GetRentBoxNum(LuaPlus::LuaState* state);
	//����ָ��������Ŀ�ʼ���͸�����
	INT GetRentBoxInfo(LuaPlus::LuaState* state);
	//���õ�ǰ��������
	INT SetCurRentIndex(LuaPlus::LuaState* state);
	//ת������
	INT TransformCoin(LuaPlus::LuaState* state);
	//�ر�����
	INT Close(LuaPlus::LuaState* state);
	//��ò�����Ʒ
	INT EnumItem(LuaPlus::LuaState* state);
public:
	static Bank s_Bank;
	static LuaPlus::LuaObject* s_pMetaTable;
};

class Exchange
{
public:
	// �ĵ�����Ƿ����
	INT IsPresent(LuaPlus::LuaState* state);
	// ���뽻��
	INT SendExchangeApply(LuaPlus::LuaState* state);
	// �򿪽��׶Ի���
	INT OpenExchangeFrame(LuaPlus::LuaState* state);
	// �����Լ����뽻�׵���Ʒ����
	INT GetItemNum(LuaPlus::LuaState* state);
	// ����Action
	INT GetItemAction(LuaPlus::LuaState* state);
	// ������Ǯ��
	INT GetMoney(LuaPlus::LuaState* state);
	// ��������
	INT LockExchange(LuaPlus::LuaState* state);
	//��������״̬
	INT IsLocked(LuaPlus::LuaState* state);
	// ȷ������
	INT AcceptExchange(LuaPlus::LuaState* state);
	// ����ȡ��
	INT ExchangeCancel(LuaPlus::LuaState* state);
	// �򿪳�����棨��ʱ�����򿪽�Ǯ�����
	INT OpenPetFrame(LuaPlus::LuaState* state);
	// ��������ý�Ǯ
	INT GetMoneyFromInput(LuaPlus::LuaState* state);
	//�������������б��Ƿ�Ϊ��
	INT IsStillAnyAppInList(LuaPlus::LuaState* state);
	//�������Լ����׵��˵�����
	INT GetOthersName(LuaPlus::LuaState* state);
	//ѡ��һ�������б��еĳ�����ȷ����Ĳ���
	INT AddPet(LuaPlus::LuaState* state);
	// ��ó���ĸ���
	INT GetPetNum(LuaPlus::LuaState* state);
	//
	INT EnumPet(LuaPlus::LuaState* state);
	//�ӽ�����������һ������
	INT DelSelectPet(LuaPlus::LuaState* state);
	//��ʾ������ϸ��Ϣ
	INT ViewPetDesc(LuaPlus::LuaState* state);

public:
	static Exchange s_Exchange;
	static LuaPlus::LuaObject* s_pMetaTable;
};


class LifeAbility
{
public:
	// �õ�����ܵ�����
//	INT GetLifeAbility_Count(LuaPlus::LuaState* state);
	// ͨ�����ȥ��Ability�ı��
	INT GetLifeAbility_Number(LuaPlus::LuaState* state);
	// ͨ�����ȥ����䷽������Ҫ��������
	INT GetPrescr_Material_Number(LuaPlus::LuaState* state);
	// ͨ�����ϵı�ţ���ѯ���ϵ���ϸ��Ϣ������ͼ������ơ�
	INT	GetPrescr_Material(LuaPlus::LuaState* state);
	// ͨ�����ȥ��ڼ����䷽�ı��
	INT GetPrescrList_Item_FromNum(LuaPlus::LuaState* state);
	// ��������ԭ���ܹ����Ժϳɶ��ٸ�����䷽��
	INT GetPrescr_Item_Maximum(LuaPlus::LuaState* state);
	// ����ԭ��������
	INT GetPrescr_Material_Hold_Count(LuaPlus::LuaState* state);
	// ˢ�ºϳɽ���
	INT Update_Synthesize( LuaPlus::LuaState* state);
	// �򿪱�ʯ�ϳ�/��Ƕ����
	INT Open_Compose_Gem_Page( LuaPlus::LuaState* state);
	// ��ʼ��Ƕ
	INT	Do_Enchase( LuaPlus::LuaState* state);
	// ��ʼ�ϳ�
	INT	Do_Combine( LuaPlus::LuaState* state);
	// ����װ���ϣ���i�ű�ʯ��ͼ����Ϣ
	INT	GetEquip_Gem( LuaPlus::LuaState* state);
	// ����װ���ϣ��ܹ��ж��ٿű�ʯ��
	INT GetEquip_GemCount( LuaPlus::LuaState* state);
	// ����װ���ϣ��ܹ�������Ƕ���ٸ���ʯ	
//	INT GetEquip_GemCount( LuaPlus::LuaState* state);
	// ���ر�ʯ�ļ���
//	INT Compound_Preparation( LuaPlus::LuaState* state );
	// �Ƿ������Ƕ
	INT	Can_Enchase( LuaPlus::LuaState* state );
	// �Ƿ���Ժϳ�
	INT Can_Combine( LuaPlus::LuaState* state );
	// ��Ʒ���Էŵ���Ƕ�������λ�á�
	INT Enchase_Preparation( LuaPlus::LuaState* state );
	// ��Ʒ���Էŵ��ϳɽ������λ�á�
	INT Compound_Preparation( LuaPlus::LuaState* state );
	// ����Ʒ��ĳλ�õ���Ʒ�ûҡ�
	INT Lock_Packet_Item( LuaPlus::LuaState* state );
	// ȡ���䷽����ϸ����
	INT	GetPrescr_Explain(LuaPlus::LuaState* state);
	// ����ܽ�����ĵ�Npc
	INT GetNpcId( LuaPlus::LuaState* state );

public:
	static LifeAbility s_LifeAbility;
	static LuaPlus::LuaObject* s_pMetaTable;
};

//�п��ܽ������ֿ�������
//class Stall

class StallSale
{
public:
	//�ĵ�����Ƿ����
	INT IsPresent(LuaPlus::LuaState* state);
	//��������̯λ��
	INT GetPosTax(LuaPlus::LuaState* state);
	//��������˰��
	INT GetTradeTax(LuaPlus::LuaState* state);
	//�ύ��Ʒ�۸�
	INT ReferItemPrice(LuaPlus::LuaState* state);
	// ����̯λ����
	INT ModifStallName(LuaPlus::LuaState* state);
	// ����Item�۸�
	INT ModifItemPrice(LuaPlus::LuaState* state);
	// ��̯����
	INT CloseStall(LuaPlus::LuaState* state);
	// ��Ʒ�����¶���
	INT ItemReprice(LuaPlus::LuaState* state);
	// ���Ϳ�ʼ��̯����Ϣ��ʼ��̯
	INT AgreeBeginStall(LuaPlus::LuaState* state);
	//ɾ�������ϵ�һ����Ʒ���¼ܣ�
	INT DeleteItem(LuaPlus::LuaState* state);
	//���ָ��λ�õĶ���
	INT GetPrice(LuaPlus::LuaState* state);
	//��̯λ��Ϣ
	INT OpenMessageSale(LuaPlus::LuaState* state);
	//����̯λ���ܣ�����
	INT ApplyAd(LuaPlus::LuaState* state);
	//�ر�
	INT CloseStallMessage(LuaPlus::LuaState* state);
	//���̯λ��
	INT GetStallName(LuaPlus::LuaState* state);
	//��ù����
	INT GetAdvertise(LuaPlus::LuaState* state);
	//�����Լ���GUID
	INT GetGuid(LuaPlus::LuaState* state);
	// ��ó���ĸ���
	INT GetPetNum(LuaPlus::LuaState* state);
	// ��ó���
	INT EnumPet(LuaPlus::LuaState* state);
	// �����ϼ�
	INT PetUpStall(LuaPlus::LuaState* state);
	// ������ļ۸�
	INT PetReprice(LuaPlus::LuaState* state);
	// ���ȱʡҳ
	INT GetDefaultPage(LuaPlus::LuaState* state);
	// ����ȱʡҳ
	INT SetDefaultPage(LuaPlus::LuaState* state);
	// ��ʾ����
	INT ViewPetDesc(LuaPlus::LuaState* state);
	//����ѡ�е��Լ�̯λ�ϵĳ���
	INT SetSelectPet(LuaPlus::LuaState* state);

public:
	static StallSale s_StallSale;
	static LuaPlus::LuaObject* s_pMetaTable;
};

class StallBuy
{
public:
	//�ĵ�����Ƿ����
	INT IsPresent(LuaPlus::LuaState* state);
	//��������˰��
	INT GetTradeTax(LuaPlus::LuaState* state);
	//�򿪱��˵�̯λ
	INT OpenStall(LuaPlus::LuaState* state);
	//���ָ��λ�õĶ���
	INT GetPrice(LuaPlus::LuaState* state);
	//������Ʒ
	INT BuyItem(LuaPlus::LuaState* state);
	//��̯λ��Ϣ
	INT OpenMessageBuy(LuaPlus::LuaState* state);
	//���̯λ��
	INT GetStallName(LuaPlus::LuaState* state);
	//��ù����
	INT GetAdvertise(LuaPlus::LuaState* state);
	//�ر�
	INT CloseStallMessage(LuaPlus::LuaState* state);
	//�����Լ���GUID
	INT GetGuid(LuaPlus::LuaState* state);
	//����̯��������
	INT GetStallerName(LuaPlus::LuaState* state);
	// ��ó���ĸ���
	INT GetPetNum(LuaPlus::LuaState* state);
	// ��ó���
	INT EnumPet(LuaPlus::LuaState* state);
	// ���ȱʡҳ
	INT GetDefaultPage(LuaPlus::LuaState* state);
	//�������
	INT BuyPet(LuaPlus::LuaState* state);

public:
	static StallBuy s_StallBuy;
	static LuaPlus::LuaObject* s_pMetaTable;
};

class StallBbs
{
public:
	//�����Ϣ������
	INT GetMessageNum(LuaPlus::LuaState* state);
	//���
	INT EnumMessage(LuaPlus::LuaState* state);
	//���һ���µ���Ϣ
	INT AddMessage(LuaPlus::LuaState* state);
	//�ظ�һ����Ϣ
	INT ReplyMessage(LuaPlus::LuaState* state);


public:
	static StallBbs s_StallBbs;
	static LuaPlus::LuaObject* s_pMetaTable;
};

class MissionReply
{
public:
	//�ĵ�����Ƿ����
	INT IsPresent(LuaPlus::LuaState* state);
	//�򿪳����б�
	INT OpenPetFrame(LuaPlus::LuaState* state);
	//���"Continue�Ĳ���"
	INT OnContinue(LuaPlus::LuaState* state);
	//
	INT EnumItem(LuaPlus::LuaState* state);


public:
	static MissionReply s_MissionReply;
	static LuaPlus::LuaObject* s_pMetaTable;
};

class NpcShop
{
public:
	//�ر�NpcShop
	INT Close(LuaPlus::LuaState* state);
	//����̵�NpcID
	INT GetNpcId(LuaPlus::LuaState* state);
	//����̵����Ʒ
	//INT EnumCallBackItem(LuaPlus::LuaState* state);
	//��ûع���Ʒ������
	INT GetCallBackNum(LuaPlus::LuaState* state);
	//����̵����Ʒ�۸�
	INT EnumItemPrice(LuaPlus::LuaState* state);
	//����̵���Ʒ��������
	INT EnumItemMaxOverlay(LuaPlus::LuaState* state);
	//����̵���Ʒ����
	INT EnumItemName(LuaPlus::LuaState* state);
	//һ�ι�������Ʒ
	//INT BulkBuyItem(LuaPlus::LuaState* state);
	//����̵����ϸ����
	//INT GetShopType(LuaPlus::LuaState* state);
	// ��õ�ǰ�������װ������������۸񣬣���Ҫ�������Npc������۸�ϵ�����ܲ��������������
	//INT GetRepairAllPrice(LuaPlus::LuaState* state);

	//�õ�NPC����Ӫ
	INT GetNpcCamp(LuaPlus::LuaState* state);

public:
	static NpcShop s_NpcShop;
	static LuaPlus::LuaObject* s_pMetaTable;
};

//����̵�
class PlayerShop
{
public:
	INT CreateShop(LuaPlus::LuaState* state);
	// ����̵������
	INT GetShopNum(LuaPlus::LuaState* state);
	// 
	INT EnumShop(LuaPlus::LuaState* state);
	// 
	INT AskOpenShop(LuaPlus::LuaState* state);
	// 
	INT EnumItem(LuaPlus::LuaState* state);
	// 
	INT EnumShopInfo(LuaPlus::LuaState* state);
	// 
	INT GetSelectIndex(LuaPlus::LuaState* state);
	// ������̵�ȡ����Ʒ���Լ��ı���
	INT RetackItem(LuaPlus::LuaState* state);
	//
	INT InputMoney(LuaPlus::LuaState* state);
	//�ϼܣ��¼ܣ�
	INT UpStall(LuaPlus::LuaState* state);
	//
	INT	SetCurSelectPage(LuaPlus::LuaState* state);
	//
	INT	GetCurSelectPage(LuaPlus::LuaState* state);
	//
	INT DealMoney(LuaPlus::LuaState* state);
	// ��������ύ
	INT ApplyMoney(LuaPlus::LuaState* state);
	//
	INT InfoMoney(LuaPlus::LuaState* state);
	//
	INT GetStallNum(LuaPlus::LuaState* state);
	//
	INT BuyItem(LuaPlus::LuaState* state);
	//
	INT IsSelectOnSale(LuaPlus::LuaState* state);
	//�¼�
	INT DownSale(LuaPlus::LuaState* state);
	//��յ�ǰ��ѡ��
	INT ClearSelect(LuaPlus::LuaState* state);
	//��ý�Ǯ
	INT GetMoney(LuaPlus::LuaState* state);
	//�����ҵָ��
	INT GetCommercialFactor(LuaPlus::LuaState* state);
	//��õ���
	INT GetShopInfo(LuaPlus::LuaState* state);
	//���һ��Stall�Ƿ�Open
	INT IsOpenStall(LuaPlus::LuaState* state);
	//����һ����̨
	INT OpenStall(LuaPlus::LuaState* state);
	//����һ��Stall������
	INT AskStallData(LuaPlus::LuaState* state);
	//����
	INT EnumPet(LuaPlus::LuaState* state);
	//���õ�ǰѡ�еĳ���
	INT SetSelectPet(LuaPlus::LuaState* state);
	//��õ�ǰ�ܿ����̵������
	INT GetCanOpenShopType(LuaPlus::LuaState* state);
	//��õ�ǰѡ�еĳ���Index
	INT SetCurSelectPetIndex(LuaPlus::LuaState* state);
	//���ѡ����Ʒ���߳���ļ۸�
	INT GetObjPrice(LuaPlus::LuaState* state);
	//�޸��̵������
	INT Modify(LuaPlus::LuaState* state);
	//���ѡ�е���Ʒ�����������
	INT GetObjName(LuaPlus::LuaState* state);
	//��ʾ������ϸ��Ϣ
	INT ViewPetDesc(LuaPlus::LuaState* state);
	//�������ѡ��� Position,,�ڽ��в�����ʱ�򣬿���ʹ�ü�����ֵ�ǲ���Ϊ��1 ����־�ǲ��ǽ�����ѡ����Ʒ
	INT ClearSelectPos(LuaPlus::LuaState* state);
	//�̳��̵�
	INT Transfer(LuaPlus::LuaState* state);
	// �鿴��ǰ���̵��ǲ��Ǵ����̵�״̬
	INT IsSaleOut(LuaPlus::LuaState* state);
	// �����̵�
	INT BuyShop(LuaPlus::LuaState* state);
	//�ر�
	INT CloseShop(LuaPlus::LuaState* state);
	//�򿪣��رգ�Message
	INT OpenMessage(LuaPlus::LuaState* state);
	//���Message������
	INT GetMessageNum(LuaPlus::LuaState* state);
	//��ǰҳ��Message������
	INT GetCurPageMessageNum(LuaPlus::LuaState* state);
	//���Message
	INT EnumMessage(LuaPlus::LuaState* state);
	//�����ϻ���
	INT DealFriend(LuaPlus::LuaState* state);
	//��úϻ��˵�����
	INT GetFriendNum(LuaPlus::LuaState* state);
	//��úϻ���
	INT EnumFriend(LuaPlus::LuaState* state);
	//���ţ�����������
	INT ChangeShopNum(LuaPlus::LuaState* state);
	// ��÷��ʵ��̵����Npc�ĵ�ǰ����ID����������CareObject��
	INT GetNpcId(LuaPlus::LuaState* state);

public:
	static PlayerShop s_PlayerShop;
	static LuaPlus::LuaObject* s_pMetaTable;
};

//�������
class Friend
{
public:
	//�ر�
	INT Close(LuaPlus::LuaState* state);
	//�ر�
	
	//��ѯ����
	INT EnumName(LuaPlus::LuaState* state);
	
	INT OpenMenu( LuaPlus::LuaState* state);
	
	INT AskTeam(LuaPlus::LuaState* state);
	INT InviteTeam(LuaPlus::LuaState* state);

	INT OpenGrouping( LuaPlus::LuaState* state);

	// �õ��ж�������Ϣ
	INT GetHistroyNumber( LuaPlus::LuaState* state);
	// �õ���ʷ����
	INT GetHistroyData(LuaPlus::LuaState* state);

	INT	SetCurrentTeam( LuaPlus::LuaState* state );
	INT GetCurrentTeam( LuaPlus::LuaState* state );

	INT SetCurrentSelect( LuaPlus::LuaState* state );
	INT GetCurrentSelect( LuaPlus::LuaState* state );
	//���˽����һ����ͬ�⡱�͡���ͬ�⡱����
	INT CallOf(LuaPlus::LuaState* state);
	// �����ʾ����Ĳ���
	INT ViewFeel(LuaPlus::LuaState* state);
	//���������ǲ������Լ���ͷ��
	INT IsMoodInHead(LuaPlus::LuaState* state);

	INT IsPlayerIsFriend(LuaPlus::LuaState* state);

public:
	static Friend s_Friend;
	static LuaPlus::LuaObject* s_pMetaTable;
	static int m_nCurTeam; // ��ǰѡ�е��飬
	static int m_nCurSelect;
	Friend()
	{
		m_nCurTeam = 1; 
		m_nCurSelect = 0;
	};

};


//---------------------------------------------------------------------------------------------------------------------------------------
//
// ��¼��Ϣ��������.
//
class CGameProduce_Login
{

public: 

	CGameProduce_Login();
	~CGameProduce_Login();

public:
	

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// ѡ�������
	//

	//�õ�����������ĸ���.
	INT GetAreaCount(LuaPlus::LuaState* state);
	
	//�õ����������������.
	INT GetAreaName(LuaPlus::LuaState* state);

	//�õ�������������ṩ��.

	//�õ������������, login server �ĸ���.
	INT GetAreaLoginServerCount(LuaPlus::LuaState* state);

	//�õ��������������Ϣ
	INT GetAreaServerInfo(LuaPlus::LuaState* state);

	// �õ�login server ����Ϣ.
	INT GetAreaLoginServerInfo(LuaPlus::LuaState* state);

	// ѡ��һ��������
	INT SelectLoginServer(LuaPlus::LuaState* state);

	// �Զ�ѡ��һ���뵱ǰѡ������繩Ӧ��ƥ���login server
	INT AutoSelLoginServer(LuaPlus::LuaState* state);





	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// �ʺ��������
	//

	// ���ӵ�login server
	INT ConnectToLoginServer(LuaPlus::LuaState* state);

	// �˵�������ѡ�����
	INT ExitToSelectServer(LuaPlus::LuaState* state);

	// ��֤�û���������
	INT CheckAccount(LuaPlus::LuaState* state);

	


	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// ����ѡ�����
	//

	// ������ѡ������л����ʺ��������
	INT ChangeToAccountInputDlgFromSelectRole(LuaPlus::LuaState* state);

	// ������ѡ������л������ﴴ������
	INT ChangeToCreateRoleDlgFromSelectRole(LuaPlus::LuaState* state);

	// �����ﴴ�������л�������ѡ�����
	INT ChangeToSelectRoleDlgFromCreateRole(LuaPlus::LuaState* state);

	// �õ���ɫ�ĸ���
	INT GetRoleCount(LuaPlus::LuaState* state);

	// �õ���ɫ����Ϣ
	INT GetRoleInfo(LuaPlus::LuaState* state);

	// �õ���ɫ��uiģ������
	INT GetRoleUIModleName(LuaPlus::LuaState* state);

	// ������ɫ
	INT CreateRole(LuaPlus::LuaState* state);

	// ɾ����ɫ
	INT DelRole(LuaPlus::LuaState* state);

	// ���ͽ�����Ϸ��Ϣ
	INT SendEnterGameMsg(LuaPlus::LuaState* state);

	// ѯ���Ƿ�ɾ����ɫ
	INT DelRole_YesNo(LuaPlus::LuaState* state);

	// ѯ���Ƿ��˳���Ϸ
	INT ExitGame_YesNo(LuaPlus::LuaState* state);

	// ѯ���Ƿ��˵��ʺ��������
	INT ExitToAccountInput_YesNo(LuaPlus::LuaState* state);

	// ɾ��ѡ��Ľ�ɫ
	INT DelSelRole(LuaPlus::LuaState* state);

	


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// ����ͷ�񲿷�.
	//

	// �õ�Ů����ͷ��ĸ���
	INT GetWomanFaceCount(LuaPlus::LuaState* state);

	// �õ�������ͷ��ĸ���
	INT GetManFaceCount(LuaPlus::LuaState* state);

	// �õ�Ů����ͷ�������
	INT GetWomanFaceName(LuaPlus::LuaState* state);

	// �õ�������ͷ�������
	INT GetManFaceName(LuaPlus::LuaState* state);

	// �õ�����ͷ�������
	INT GetFaceName(LuaPlus::LuaState* state);

	// ����ͷ��id
	INT SetFaceId(LuaPlus::LuaState* state);



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// �������β���.
	//

	
	// �õ�����model�ĸ���
	INT GetFaceModelCount(LuaPlus::LuaState* state);

	// �õ�����ģ�͵�����
	INT GetFaceModelName(LuaPlus::LuaState* state);

	// ��������ģ��id
	INT SetFaceModelId(LuaPlus::LuaState* state);



	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// ���ڷ��β���.
	//
	
	// �õ�����model�ĸ���
	INT GetHairModelCount(LuaPlus::LuaState* state);

	// �õ�����ģ�͵�����
	INT GetHairModelName(LuaPlus::LuaState* state);

	// ��������ģ��id
	INT SetHairModelId(LuaPlus::LuaState* state);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// ְҵ��Ӫ���֡�
	//

	// �õ������Ӫ
	INT GetPlayerCamp(LuaPlus::LuaState* state);

	// ���������Ӫ
	INT SetPlayerCamp(LuaPlus::LuaState* state);

	// �������ְҵ
	INT SetPlayerMenpai(LuaPlus::LuaState* state);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// ����״̬���֡�
	//
	
	// �ر��������ӡ�2006��4��12
	INT CloseNetConnect(LuaPlus::LuaState* state);


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// �������֡�
	//
	
	//  ��ʼ�������֡�2006��4��29
	INT PlayMusic(LuaPlus::LuaState* state);

	//  ֹͣ�������֡�2006��4��29
	INT StopMusic(LuaPlus::LuaState* state);


public:
	static CGameProduce_Login s_GameProduceLogin;
	static LuaPlus::LuaObject* s_pMetaTable;
};

};//namespace SCRIPT_SANDBOX


