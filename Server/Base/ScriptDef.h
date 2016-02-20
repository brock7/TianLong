// ScriptDef.h
//
/////////////////////////////////////////////////////////

#ifndef __SCRIPTDEF_H__
#define __SCRIPTDEF_H__

#define ABILITY_LOGIC_SCRIPT			701601
#define GEM_COMPOUND_SCRIPT				701602

#define	DEF_EVENT_ENTRY_FUNC_NAME		("OnDefaultEvent") //�ű����뺯��
#define	DEF_ON_KILL_OBJECT_FUNC_NAME	("OnKillObject")
#define	DEF_ON_ITEM_CHANGED_FUNC_NAME	("OnItemChanged")
#define	DEF_ON_PET_CHANGED_FUNC_NAME	("OnPetChanged")
#define	DEF_ON_ENTER_AREA_FUNC_NAME		("OnEnterArea")
#define	DEF_ON_LEAVE_AREA_FUNC_NAME		("OnLeaveArea")
#define DEF_EVENT_ON_TIMER				("OnTimer")

#define DEF_MISSION_ACCEPT				("OnMissionAccept") //��������
#define DEF_MISSION_ABANDON				("OnAbandon") //��������
#define DEF_MISSION_REFUSE				("OnMissionRefuse") // �ܾ���������
#define DEF_MISSION_SUBMIT				("OnMissionSubmit") //������ɺ��ύ����
#define DEF_MISSION_CHECK				("OnMissionCheck") //��������������
#define DEF_MISSION_CONTINUE			("OnMissionContinue") //����û��ɣ�����

// ���������ͨ�ϳ��䷽ʹ��
// ����ʹ�ü�麯��
#define DEF_ABILITY_CHECK				("AbilityCheck")
// �ϳɽ����������������
#define DEF_ABILITY_CONSUME				("AbilityConsume")
// �ϳɳɹ���������Ʒ
#define DEF_ABILITY_PRODUCE				("AbilityProduce")

// �ϳɽ���ʱ�ĳɹ��ʼ���
#define DEF_ABILITY_SUCCESSFUL_CHECK	("CheckForResult")
// �䷽�ϳɽ���ʱ������������
#define DEF_ABILITY_GAIN_EXPERIENCE		("GainExperience")

// �ϳɳɹ�ʱ�����Ʒ��Ʒ�ʣ�Quality��
#define DEF_ABILITY_CALC_QUALITY		("CalcQuality")

// �����Ǳ�ʯ����
#define DEF_GEM_COUNT_CHECK				("TooManyGems")
#define DEF_IS_GEM_FIT_EQUIP			("IsGemFitEquip")
#define DEF_GEM_CONFLICT_CHECK			("IsGemConflict")
#define DEF_GEM_EMBED_PROCESS			("EmbedProc")

#define DEF_GEM_COMPOUND				("GemCompound")

#endif