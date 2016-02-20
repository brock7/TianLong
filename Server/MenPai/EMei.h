///////////////////////////////////////////////////////////////////////////////
// �ļ�����EMei.h
// ����˵��������:���ҵĽӿ�����
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef EMEI_H
#define EMEI_H

#include "MenPai.h"

namespace MenPai_Module
{
	class EMei_T: public MenPai_T
	{
		public:
			enum
			{
				ID= MenPai_T::ID_EMEI,
			};
			
			EMei_T(){};
			~EMei_T(){};
			MenPai_T::MenPaiID_T GetID(VOID) const {return (MenPai_T::MenPaiID_T)ID;};
			//Add Rage And Strike point part here
			virtual VOID OnHealTarget(Obj_Human& rMe, INT nHealedHp) const;

		protected:
		private:
			virtual INT TransferValueToRage(INT nDamage) const;
	};
};
#endif //EMEI_H


