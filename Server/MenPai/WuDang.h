///////////////////////////////////////////////////////////////////////////////
// �ļ�����WuDang.h
// ����˵��������:�䵱�Ľӿ�����
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WUDANG_H
#define WUDANG_H

#include "MenPai.h"

namespace MenPai_Module
{
	class WuDang_T: public MenPai_T
	{
		public:
			enum
			{
				ID= MenPai_T::ID_WUDANG,
			};
			
			WuDang_T(){};
			~WuDang_T(){};
			MenPai_T::MenPaiID_T GetID(VOID) const {return (MenPai_T::MenPaiID_T)ID;};
			//Add Rage And Strike point part here

		protected:
		private:
	};
};
#endif //WUDANG_H


