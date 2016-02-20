#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// �ļ�����GaiBang.cpp
// ����˵��������:���̵Ľӿ�ʵ��
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "GaiBang.h"

namespace MenPai_Module
{
	//class GaiBang_T: public MenPai_T
	VOID GaiBang_T::OnDepleteStrikePoints(Obj_Human& rMe, INT nStrikePoint) const
	{
		if(0>=nStrikePoint)
		{
			return;
		}
		
		INT nRage = TransferValueToRage(nStrikePoint);
		rMe.RefixRageRegeneration(nRage);
		rMe.RageIncrement(nRage,NULL);
	}

	INT GaiBang_T::TransferValueToRage(INT nValue) const
	{
		if(0>=nValue)
		{
			return 0;
		}
		return nValue*10;
	}
		
};


