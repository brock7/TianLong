#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// �ļ�����MingJiao.cpp
// ����˵��������:���̵Ľӿ�ʵ��
//
// �޸ļ�¼��
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "MingJiao.h"

namespace MenPai_Module
{
	//class MingJiao_T: public MenPai_T
	VOID MingJiao_T::OnDamageTarget(Obj_Human& rMe, INT nDamage) const
	{
		if(0>=nDamage)
		{
			return;
		}
		
		INT nRage = TransferValueToRage(nDamage);
		rMe.RefixRageRegeneration(nRage);
		rMe.RageIncrement(nRage,NULL);
	}

	INT MingJiao_T::TransferValueToRage(INT nValue) const
	{
		if(0>=nValue)
		{
			return 0;
		}
		return nValue/20;
	}
		
};

