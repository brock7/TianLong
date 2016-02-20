#include "StdAfx.h"
#include "../NetWork/NetManager.h"
#include "GMObjectLoadQueue.h"
#include "SocketOutputStream.h"
#include "SocketInputStream.h"
#include "ObjectManager.h"
#include "../Procedure/GameProcedure.h"
#include "TDTimeSystem.h"
#include "TDUtil.h"
#include "TDException.h"
#include "CGCharAskBaseAttrib.h"
#include "Character/Obj_PlayerMySelf.h"
#include "..\WxRender\RenderSystem.h"

CObjectLoadQueue::CObjectLoadQueue()
{
	m_LoadDirect = FALSE;
}

CObjectLoadQueue::~CObjectLoadQueue()
{
}

void CObjectLoadQueue::SetLoadNPCDirect(BOOL b)
{
	m_LoadDirect = b;
}

BOOL CObjectLoadQueue::TryAddLoadTask(INT idObj, INT ObjType)
{
	//����Լ����������
	CObject* pObj = (CObject*)CObjectManager::GetMe()->FindObject(idObj);
	if(!pObj || pObj==CObjectManager::GetMe()->GetMySelf()) return FALSE;
	if( m_LoadDirect )
	{ //��������
		CGCharAskBaseAttrib msgAskBaseAttrib;
		msgAskBaseAttrib.setTargetID(pObj->GetServerID());
		CNetManager::GetMe()->SendPacket(&msgAskBaseAttrib);
	}
	else
	{
		//�Լ�λ��
		fVector3 fvPosMySelf = CObjectManager::GetMe()->GetMySelf()->GetPosition();
		float fDistanceSq = TDU_GetDistSq(pObj->GetPosition(), fvPosMySelf);

		m_mapLoadTask.insert(std::make_pair(fDistanceSq, idObj));
	}

	return TRUE;
}

void CObjectLoadQueue::Tick(void)
{
	//ÿ�崦��ĸ���
	const INT WORK_SPEED = 1;
	if(!m_mapLoadTask.empty())
	{
		int nDoRealWork = 0;
		do
		{
			//������Ҫ���ص�OBJ
   			INT idObj = m_mapLoadTask.begin()->second;

			CObject* pObj = (CObject*)CObjectManager::GetMe()->FindObject(idObj);
			if(pObj)
			{
				//������Ϣ
				CGCharAskBaseAttrib msgAskBaseAttrib;
					msgAskBaseAttrib.setTargetID(pObj->GetServerID());
				CNetManager::GetMe()->SendPacket(&msgAskBaseAttrib);

				nDoRealWork++;
			}
			else
			{
				//��Object�Ѿ���ɾ��
				int a=0;
			}

			//�Ӷ�����ɾ��
			m_mapLoadTask.erase(m_mapLoadTask.begin());

			//������ʵ�������˳�
			if(nDoRealWork>=WORK_SPEED || m_mapLoadTask.empty()) break;
		}while(true);
	}

	char szTemp[32];
	_snprintf(szTemp, 32, "%d", m_mapLoadTask.size());
	CGameProcedure::s_pGfxSystem->SetDebugString("ObjQueue", szTemp);
}

//���ĳһ��ָ��Object�Ķ���
void CObjectLoadQueue::ClearTask(INT idObj)
{
	//NOT SUPPORT ANYMORE!!
	//
}

void CObjectLoadQueue::ClearAllTask(void)
{
	m_mapLoadTask.clear();
}