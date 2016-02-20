#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// �ļ�����StdImpact043.cpp
// ����˵����Ч��--����Ŀ��ʱ�����Լ���ŭ����������һ��ʱ���ڷ���������
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "StdImpact043.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		VOID StdImpact043_T::MarkModifiedAttrDirty(OWN_IMPACT & rImp, Obj_Character & rMe) const
		{
			__ENTER_FUNCTION
			if(0!=GetResistColdRefix(rImp))
			{
				rMe.MarkResistColdRefixDirtyFlag();
			}
			if(0!=GetResistFireRefix(rImp))
			{
				rMe.MarkResistFireRefixDirtyFlag();
			}
			if(0!=GetResistLightRefix(rImp))
			{
				rMe.MarkResistLightRefixDirtyFlag();
			}
			if(0!=GetResistPoisonRefix(rImp))
			{
				rMe.MarkResistPoisonRefixDirtyFlag();
			}
			__LEAVE_FUNCTION
		}
		VOID StdImpact043_T::RefixRageRegeneration(OWN_IMPACT const& rImp, INT& rRageRegeneration) const
		{
			__ENTER_FUNCTION
			rRageRegeneration += GetRageRegenerateRefix(rImp);
			__LEAVE_FUNCTION
		}
		BOOL StdImpact043_T::GetIntAttrRefix(OWN_IMPACT & rImp, Obj_Character& rMe, CharIntAttrRefixs_T::Index_T nIdx, INT & rIntAttrRefix) const
		{
			__ENTER_FUNCTION
			switch (nIdx)
			{
				case CharIntAttrRefixs_T::REFIX_RESIST_COLD:
					if(0!=GetResistColdRefix(rImp))
					{
						rIntAttrRefix += GetResistColdRefix(rImp);
						return TRUE;
					}
					break;
				case CharIntAttrRefixs_T::REFIX_RESIST_FIRE:
					if(0!=GetResistFireRefix(rImp))
					{
						rIntAttrRefix += GetResistFireRefix(rImp);
						return TRUE;
					}
					break;
				case CharIntAttrRefixs_T::REFIX_RESIST_LIGHT:
					if(0!=GetResistLightRefix(rImp))
					{
						rIntAttrRefix += GetResistLightRefix(rImp);
						return TRUE;
					}
					break;
				case CharIntAttrRefixs_T::REFIX_RESIST_POISON:
					if(0!=GetResistPoisonRefix(rImp))
					{
						rIntAttrRefix += GetResistPoisonRefix(rImp);
						return TRUE;
					}
					break;
				default:
					break;
			}
			__LEAVE_FUNCTION
			return FALSE;
		}
	};
};
