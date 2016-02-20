#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// �ļ�����ShaoLin.cpp
// ����˵��������:���ֵĽӿ�ʵ��
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "ShaoLin.h"

namespace MenPai_Module
{
	//class ShaoLin_T: public MenPai_T
	VOID ShaoLin_T::OnDamage(Obj_Human& rMe, INT nDamage) const
	{
		if(0>=nDamage)
		{
			return;
		}
		
		INT nRage = TransferValueToRage(nDamage);
		rMe.RefixRageRegeneration(nRage);
		rMe.RageIncrement(nRage, NULL);
	}

	INT ShaoLin_T::TransferValueToRage(INT nValue) const
	{
		if(0>=nValue)
		{
			return 0;
		}
		return nValue/20;
	}
		
};


