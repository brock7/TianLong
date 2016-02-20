#include "stdafx.h"

#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Player.h"

#include "Ability.h"
#include "AbilityManager.h"

#include "CGAskLeanAbility.h"
#include "GCAbilityLevel.h"
#include "LogDefine.h"
#include "Scene.h"

UINT CGAskLeanAbilityHandler::Execute( CGAskLeanAbility* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION


	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer;
	Assert( pGamePlayer );

	Obj_Human* pHuman = pGamePlayer->GetHuman();
	Assert( pHuman );

	Assert(pPacket);

	//�ж����������Ƿ����㣨Money,Exp,SkillExp��
	AbilityID_t nId		= pPacket->GetAbilityTeacherInfo();
	UINT nNeedMoney		= (UINT)pPacket->GetNeedMoney();
	UINT nNeedExp		= (UINT)pPacket->GetNeedExp();
	INT nNeedSkillExp	= pPacket->GetNeedSkillExp();
	INT nMyLevel		= pHuman->__GetAbilityLevel(nId);
	Ability* pAbility	= g_pAbilityManager->GetAbility(nId);


	INT SkillExp = pHuman->__GetAbilityExp(nId);

	if( nNeedMoney		<= pHuman->GetDB()->GetMoney() && 
		nNeedExp		<= pHuman->GetDB()->GetExp() &&
		nNeedSkillExp	<= pHuman->__GetAbilityExp(nId) &&
		nMyLevel		<= pAbility->AbilityLevelLimit())
	{
		// ��������
		INT nMyLevel = pHuman->__GetAbilityLevel(nId);
		pHuman->__SetAbilityLevel(nId, nMyLevel + 1);
		
		// �۳���Ǯ�;���ֵ
		UINT uSpareExp   = pHuman->GetDB()->GetExp()	- (UINT)nNeedExp;
		UINT uSpareMoney = pHuman->GetDB()->GetMoney()	- (UINT)nNeedMoney;
		pHuman->GetDB()->SetExp(uSpareExp );
		pHuman->GetDB()->SetMoney(uSpareMoney );


		MONEY_LOG_PARAM	MoneyLogParam;
		MoneyLogParam.CharGUID	=	pHuman->GetGUID();
		MoneyLogParam.OPType	=	MONEY_LEARN_ABILITY_COST;
		MoneyLogParam.Count		=	nNeedMoney;
		MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
		MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
		MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
		SaveMoneyLog(&MoneyLogParam);

		//�����޸Ŀͻ�������ܵ���Ϣ
		//�޸�����ܵȼ�
		GCAbilityLevel MsgLevel;
		MsgLevel.setAbilityId(nId);
		MsgLevel.setLevel(nMyLevel + 1);
		pPlayer = pHuman->GetPlayer();
		pPlayer->SendPacket( &MsgLevel ) ;
	}
	else
	{
		//����ʧ�ܵ���ʾ
		;
	}
	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
