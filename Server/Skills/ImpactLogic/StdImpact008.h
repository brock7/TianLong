///////////////////////////////////////////////////////////////////////////////
// �ļ�����StdImpact008.h
// ����˵����Ч��--ǿ�ƹ���ı乥��Ŀ�꣬��Է������Ч
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT008_H
#define STDIMPACT008_H

#include "Type.h"
#include "ImpactLogic.h"
#include "GameStruct_Impact.h"
#include "Obj_Character.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact008_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_008,
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_TargetMe=0,
						};
				};
				
				StdImpact008_T() {};
				~StdImpact008_T() {};
				BOOL IsOverTimed(VOID) const {return FALSE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};
				ObjID_t GetTargetMeFlag(OWN_IMPACT const& rImp) const 
				{
					return Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_TargetMe);
				};
			public: //message handler
				VOID OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const;
			protected:
			private:
		} DI_ForceMonsterChangeTarget_T;
	};
};
#endif //STDIMPACT008_H
