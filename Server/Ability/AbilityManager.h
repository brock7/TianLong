/********************************************************************************
 *	�ļ�����	AbilityManager.h
 *	ȫ·����	d:\Prj\Server\Server\Ability\AbilityManager.h
 *	����ʱ�䣺	
 *
 *	����˵����	���ڹ������������ʵ�����䷽���Լ������
 *	�޸ļ�¼��	2005 �� 11 �� 6 ��	16:52��������һ�½ṹ
*********************************************************************************/

#ifndef __ABILITYMANAGER_H__
#define __ABILITYMANAGER_H__

#include "Type.h"
#include "GameDefine.h"

#define MAX_ABILITY						MAX_CHAR_ABILITY_NUM

class Ability;
class Player;
struct _AbilityExpTable;
struct _ABILITY_PRESCRIPTION_TBL;

class AbilityManager
{
public:
	AbilityManager();
	~AbilityManager();

	enum AbilityClass
	{
		ABILITY_CLASS_INVALID			= -1,	// �Ƿ�
		ABILITY_CLASS_NOUSE				= 0,	// ռλ����
		ABILITY_CLASS_COOKING,					// ���
		ABILITY_CLASS_PHARMACY,					// ��ҩ
		ABILITY_CLASS_INLAY,					// ��Ƕ
		ABILITY_CLASS_FOUNDRY,					// ����
		ABILITY_CLASS_TAILOR,					// ����
		ABILITY_CLASS_ARTWORK,					// ����
		ABILITY_CLASS_GATHERMINE,				// �ɿ�
		ABILITY_CLASS_GATHERMEDIC,				// ��ҩ
		ABILITY_CLASS_FISH,						// ����
		ABILITY_CLASS_PLANT,					// ��ֲ
		ABILITY_CLASS_SHAOLINDRUG,				// ������ҩ�����⣩
		ABILITY_CLASS_HOLYFIRE,					// ���̣�ʥ������
		ABILITY_CLASS_BREWING,					// ؤ���ƣ�
		ABILITY_CLASS_THICKICE,					// ��ɽ����������
		ABILITY_CLASS_INSECTCULTURING,			// �����ƹƣ�
		ABILITY_CLASS_POISON,					// ���ޣ��ƶ���
		ABILITY_CLASS_INCANTATION,				// ���ң��Ʒ���
		ABILITY_CLASS_ALCHEMY,					// �䵱��������
		ABILITY_CLASS_THAUMATURGY,				// ��ң�����Ŷݼף�
		ABILITY_CLASS_ENGINEERING,				// ����ѧ
		ABILITY_CLASS_FIGHTGHOST,				// ����
		ABILITY_CLASS_SEARCHTSTORE,				// �ڱ�
		ABILITY_CLASS_TRADE,					// ����
		ABILITY_CLASS_HAGGLE,					// ɱ��
		ABILITY_CLASS_EXCESSPROFIT,				// ����
		ABILITY_CLASS_PROCESSING,				// �����õ��Ľ�����
		ABILITY_CLASS_PHARMACOLOGY,				// ҩ��
		ABILITY_CLASS_REGIMEN,					// ������
		ABILITY_CLASS_BUDDHOLOGY,				// ��
		ABILITY_CLASS_FIREMAKING,				// �ɻ���
		ABILITY_CLASS_BEGSKILL,					// ������
		ABILITY_CLASS_ICEMAKING,				// �ɱ���
		ABILITY_CLASS_VENATIONFORMULA,			// �����پ�
		ABILITY_CLASS_INSECTENTICING,			// ������
		ABILITY_CLASS_MENTALTELEPATHY,			// ������
		ABILITY_CLASS_TAOISM,					// ����
		ABILITY_CLASS_BODYBUILDING,				// ���շ��

		ABILITY_CLASS_SIZE,
	};

	VOID								CleanUp();

	BOOL								Init();

	// ���
	Ability*							GetAbility( AbilityID_t aid )
	{
		INT idx;
		idx = (INT)aid;

		if( idx<=ABILITY_CLASS_INVALID || idx>=ABILITY_CLASS_SIZE )
		{
			Assert(idx>ABILITY_CLASS_INVALID && idx<ABILITY_CLASS_SIZE);
			return NULL;
		}

		return m_pAbility[idx];
	}

	BYTE								MaxAbilityNum() { return (BYTE)ABILITY_CLASS_SIZE; }

	BOOL								InitAbility( const CHAR* filename );
	BOOL								InitPrescriptionList( const CHAR* filename );
	BOOL								InitAbilityExpTable( const CHAR* filename );

	_AbilityExpTable*					GetAbilityExpTbl()
	{
		return m_pAbilityExpTbl;
	}

	INT									GetAbilityExpTblRowCount()
	{
		return m_AbilityExpTblCount;
	}

	const _ABILITY_PRESCRIPTION_TBL*	GetPrescriptionList( )
	{
		return m_pPrescriptionList;
	}
private:
	Ability*							m_pAbility[MAX_ABILITY];
	_AbilityExpTable*					m_pAbilityExpTbl;
	INT									m_AbilityExpTblCount;
	_ABILITY_PRESCRIPTION_TBL*			m_pPrescriptionList;
};

extern AbilityManager*					g_pAbilityManager;

#endif // __ABILITYMANAGER_H__
