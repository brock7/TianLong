///////////////////////////////////////////////////////////////////////////////
// �ļ�����StdImpact004.h
// ����˵����Ч��--˲��-������ħ����ŭ�����������ֱ���޸�
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef STDIMPACT004_H
#define STDIMPACT004_H

#include "Type.h"
#include "ImpactLogic.h"
#include "GameStruct_Impact.h"
#include "Obj_Character.h"

namespace Combat_Module
{
	namespace Impact_Module
	{
		typedef class StdImpact004_T : public ImpactLogic_T
		{
			public:
				enum
				{
					ID = STD_IMPACT_004,
				};
				class OwnImpactParamIndex_T
				{
					public:
						enum
						{
							IDX_HP=0,
							IDX_MP,
							IDX_RAGE,
							IDX_STRIKE_POINT,				
						};
				};
				class ImpactDataDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_HP=0,
							IDX_MP,
							IDX_RAGE,
							IDX_STRIKE_POINT,
						};
				};
				StdImpact004_T() {};
				~StdImpact004_T() {};
				BOOL IsOverTimed(VOID) const {return FALSE;};
				BOOL IsIntervaled(VOID) const {return FALSE;};
				BOOL InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const;

				INT GetHpModification(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_HP);}
				VOID SetHpModification(OWN_IMPACT& rImp, INT nValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_HP, nValue);}

				INT GetMpModification(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_MP);}
				VOID SetMpModification(OWN_IMPACT& rImp, INT nValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_MP, nValue);}

				INT GetRageModification(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_RAGE);}
				VOID SetRageModification(OWN_IMPACT& rImp, INT nValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_RAGE, nValue);}

				INT GetStrikePointModification(OWN_IMPACT const& rImp) const {return rImp.GetParamByIndex(OwnImpactParamIndex_T::IDX_STRIKE_POINT);}
				VOID SetStrikePointModification(OWN_IMPACT& rImp, INT nValue) const {rImp.SetParamByIndex(OwnImpactParamIndex_T::IDX_STRIKE_POINT, nValue);}
			public: //message handler
				VOID OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const;
				BOOL RefixPowerByRate(OWN_IMPACT& rImp, INT nRate) const;
			protected:
			private:
		} ModifyHpMpRageStrikePointByValue_T;
	};
};
#endif //STDIMPACT004_H
