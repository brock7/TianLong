#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// �ļ�����StdImpact028.cpp
// ����˵����
//
// Ч���������ֵĸ�ʽ��|Ч��ID|����ʱ��|����������|����������|����������|����������
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "StdImpact028.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		VOID StdImpact028_T::OnDamages(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character* const pAttacker, INT* const pDamageList, SkillID_t nSkillID) const
		{
			__ENTER_FUNCTION
			if(TRUE==rImp.IsFadeOut())
			{
				return;
			}
			INT nIncreaceHP = 0;
			INT nDamage = 0;
			INT nValue = 0;
			// �����ı���<0���ʾ�������Ͷ�Ӧ�˺���һ�����ʵ���ֵת���ɱ����ߵ�HP
			// �����ı���>0���ʾ�������Ͷ�Ӧ�˺���һ�����ʵ���ֵ���ӵ�ʩ������
			
			if(0!=GetColdRefix(rImp))
			{
				nValue = Float2Int((pDamageList[DAMAGE_COLD] * GetColdRefix(rImp))/100.0f);
				if(0<=nValue)
				{
					pDamageList[DAMAGE_COLD] += nValue;
				}
				else
				{
					pDamageList[DAMAGE_COLD]=0;
					nIncreaceHP += -nValue;
				}
			}
			if(0!=GetFireRefix(rImp))
			{
				nValue = Float2Int((pDamageList[DAMAGE_FIRE] * GetFireRefix(rImp))/100.0f);
				if(0<=nValue)
				{
					pDamageList[DAMAGE_FIRE] += nValue;
				}
				else
				{
					pDamageList[DAMAGE_FIRE]=0;
					nIncreaceHP += -nValue;
				}
			}
			if(0!=GetLightRefix(rImp))
			{
				nValue = Float2Int((pDamageList[DAMAGE_LIGHT] * GetLightRefix(rImp))/100.0f);
				if(0<=nValue)
				{
					pDamageList[DAMAGE_LIGHT] += nValue;
				}
				else
				{
					pDamageList[DAMAGE_LIGHT]=0;
					nIncreaceHP += -nValue;
				}
			}
			if(0!=GetPoisonRefix(rImp))
			{
				nValue = Float2Int((pDamageList[DAMAGE_POISON] * GetPoisonRefix(rImp))/100.0f);
				if(0<=nValue)
				{
					pDamageList[DAMAGE_POISON] += nValue;
				}
				else
				{
					pDamageList[DAMAGE_POISON]=0;
					nIncreaceHP += -nValue;
				}
			}
			// ��ת������HP�ӵ��Լ�����
			if(0<nIncreaceHP)
			{
				rMe.HealthIncrement(nIncreaceHP, &rMe);
			}
			__LEAVE_FUNCTION
		}
	};
};

