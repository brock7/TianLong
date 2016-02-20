#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// �ļ�����StdImpact004.cpp
// ����˵����Ч��--������ħ����ŭ�����������ֱ���޸�
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "StdImpact004.h"
#include "Obj_Human.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		BOOL StdImpact004_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION
			SetHpModification(rImp, Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_HP));
			SetMpModification(rImp, Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_MP));
			SetRageModification(rImp, Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_RAGE));
			SetStrikePointModification(rImp, Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_STRIKE_POINT));
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID StdImpact004_T::OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			if(FALSE==rMe.IsAlive() || FALSE==rMe.IsActiveObj())
			{
				return;
			}
			Obj_Character * pImpactSender = (Obj_Character*)Impact_GetCaster(rMe,rImp);
			//process HP
			INT nHP = GetHpModification(rImp);
			rMe.HealthIncrement(nHP, pImpactSender);
			if(NULL!=pImpactSender)
			{
				pImpactSender->OnHealTarget(rImp.GetSkillID(), rMe, nHP);
			}
			// ֻ��Human�Ż��õ���δ��� 
			if (rMe.GetObjType() == Obj::OBJ_TYPE_HUMAN)
			{
				//process MP
				INT nMP = GetMpModification(rImp);
				rMe.ManaIncrement(nMP, pImpactSender);

				//Process Rage
				INT nRage = GetRageModification(rImp);
				rMe.RageIncrement(nRage, pImpactSender);

				//Process Strike Point
				INT nStrikePoint = GetStrikePointModification(rImp);
				rMe.StrikePointIncrement(nStrikePoint, pImpactSender);
			}
			__LEAVE_FUNCTION
		}
		BOOL StdImpact004_T::RefixPowerByRate(OWN_IMPACT & rImp, INT nRate) const
		{
			__ENTER_FUNCTION
			nRate += 100;
			SetHpModification(rImp, Float2Int((GetHpModification(rImp)*nRate)/100.0f));
			SetMpModification(rImp, Float2Int((GetMpModification(rImp)*nRate)/100.0f));
			SetRageModification(rImp, Float2Int((GetRageModification(rImp)*nRate)/100.0f));
			SetStrikePointModification(rImp, Float2Int((GetStrikePointModification(rImp)*nRate)/100.0f));
			__LEAVE_FUNCTION
			return TRUE;
		}
	};
};

