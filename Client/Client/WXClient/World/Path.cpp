#include "StdAfx.h"
#include "WorldManager.h"
#include <complex>
#include "Path.h"
#include "TDException.h"
#include "../Procedure/GameProcedure.h"
#include "TDUtil.h"
#include "TDDBC_Struct.h"
#include "..\Object\ObjectManager.h"
#include "..\Object\Character\Obj_PlayerMySelf.h"
#include "..\DataPool\GMDP_CharacterData.h"
#include "..\DBC\GMDataBase.h"
#include <float.h>

#define LINE_CROSS_RANGE		0.0001f

CPath::CPath()
{
	m_iScene = -1;
}

CPath::~CPath()
{
}

const CPath::PathUnit& CPath::GetConstPathUnit(INT nIndex)const
{
	if(m_vPosStack.empty() || nIndex >= (INT)m_vPosStack.size())
	{
		TDThrow("Invalid Path!");
	}

	return m_vPosStack[nIndex];
}

//����RequestIndex;
INT CPath::RequestIndexFactory(VOID)
{
	const INT MAX_REQUEST_INDEX = 0XFF;
	static INT s_nCurrentIndex = 0;

	if(s_nCurrentIndex == MAX_REQUEST_INDEX) 
		s_nCurrentIndex = 0;
	else 
		s_nCurrentIndex++;

	return s_nCurrentIndex;
}


BOOL CPath::CheckCross(fVector2 fvmStart, fVector2 fvmEnd,const CRegion* pcr)
{
	fVector2 fv0;
	fVector2 fv1;
	if (NULL==pcr)
	{
		return FALSE;
	}
	for (INT i=0;i<(INT)pcr->m_vfPoints.size();i++)
	{	
		fv0 = *(pcr->GetPoint(i));
		fv1 = *(pcr->GetPoint(i+1));
		if( !( fvEuqual(fvmStart,fv0) || fvEuqual(fvmStart,fv1) || fvEuqual(fvmEnd,fv0) || fvEuqual(fvmEnd,fv1) ))
		{
			if(CheckCross(fvmStart,fvmEnd,fv0,fv1))
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}


BOOL CPath::CheckCross(fVector2 fvMS, fVector2 fvME,fVector2 fvNS, fVector2 fvNE)
{
	if ( max(fvMS.x,fvME.x) <= min(fvNS.x,fvNE.x) )
	{
		return FALSE;
	}
	if ( min(fvMS.x,fvME.x) >= max(fvNS.x,fvNE.x) )
	{
		return FALSE;
	}
	if ( max(fvMS.y,fvME.y) <= min(fvNS.y,fvNE.y) )
	{
		return FALSE;
	}
	if ( min(fvMS.y,fvME.y) >= max(fvNS.y,fvNE.y) )
	{
		return FALSE;
	}

	FLOAT temp1 = fvMulti(fvSub(fvMS,fvNS),fvSub(fvNE,fvNS)) * fvMulti( fvSub(fvNE,fvNS),fvSub(fvME,fvNS));
	FLOAT temp2 = fvMulti(fvSub(fvNS,fvMS),fvSub(fvME,fvMS)) * fvMulti( fvSub(fvME,fvMS),fvSub(fvNE,fvMS));

	if((temp1>=0)&&(temp2>=0))
	{
		return TRUE;
	}
	return FALSE;	
}




fVector2 CPath::fvSub(fVector2 fv1, fVector2 fv2)
{
	fVector2 fvResult = fVector2(fv1.x-fv2.x,fv1.y-fv2.y);
	return fvResult;
}

FLOAT CPath::fvMulti(fVector2 fv1, fVector2 fv2)
{
	return (fv1.x*fv2.y - fv1.y*fv2.x);
}

BOOL CPath::fvEuqual(const fVector2& fv1, const fVector2& fv2)
{
	if ((abs(fv1.x-fv2.x)<1e-10) &&(abs(fv1.y-fv2.y)<1e-10))
	{
		return TRUE;
	}
	return FALSE;
}

// ����ӿ�ʼ�㵽�������·�� 2006-4-17
BOOL CPath::CreateMovePath(const fVector2& fvCurrent, const fVector2& fvTarget)
{
	//�õ���ǰ��������ȼ�
	INT nPassLevel = -1;
	INT nMountID = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_MountID();
	if(nMountID != INVALID_ID)
	{
		DBC_DEFINEHANDLE(s_pCharMountDBC, DBC_CHARACTER_MOUNT)
			const _DBC_CHARACTER_MOUNT *pCharMount = (const _DBC_CHARACTER_MOUNT*)(s_pCharMountDBC->Search_Index_EQU(nMountID));
		if(pCharMount != NULL)
		{
			nPassLevel = pCharMount->m_nPassLevel;
		}
	}
	// �ҵ�Ŀ��㡣
	m_TargetPos = fvTarget;
	// �����·��.
	m_vPosStack.clear();

	if(IsPointInUnreachRegion(m_TargetPos))
		return FALSE;

	BOOL ret = BuildDijkPath(fvCurrent, fvTarget,nPassLevel);
	//char szTemp[1024];
	//sprintf(szTemp, "Start:(%f,%f) End:(%f,%f)\n", fvCurrent.x, fvCurrent.y, fvTarget.x, fvTarget.y);
	//OutputDebugString( szTemp );
	//OutputDebugString("------------------------------------------------------------------\n");
	//for( INT i = 0; i < (INT)m_vPosStack1.size(); i ++ )
	//{
	//	PathUnit& pu = m_vPosStack1[i];
	//	sprintf( szTemp, "(%f,%f), (%f,%f)\n", pu.fvStart.x, pu.fvStart.y, pu.fvTarget.x, pu.fvTarget.y );
	//	OutputDebugString(szTemp);
	//}
	//OutputDebugString("------------------------------------------------------------------\n");

	// //����·��
	//ret = FindPath(fvCurrent, TRUE);
	//for( INT i = 0; i < (INT)m_vPosStack.size(); i ++ )
	//{
	//	PathUnit& pu = m_vPosStack[i];
	//	sprintf( szTemp, "(%f,%f), (%f,%f)\n", pu.fvStart.x, pu.fvStart.y, pu.fvTarget.x, pu.fvTarget.y );
	//	OutputDebugString(szTemp);
	//}
	//OutputDebugString("------------------------------------------------------------------\n");

	return ret ? ret : FindPath(fvCurrent, nPassLevel,TRUE);
}


// ��ǰ���Ƿ��ڲ�����������֮��
BOOL CPath::IsPointInUnreachRegion(const fVector2& fvTarget)
{
	//�õ���ǰ��������ȼ�
	INT nPassLevel = -1;
	INT nMountID = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_MountID();
	if(nMountID != INVALID_ID)
	{
		DBC_DEFINEHANDLE(s_pCharMountDBC, DBC_CHARACTER_MOUNT)
			const _DBC_CHARACTER_MOUNT *pCharMount = (const _DBC_CHARACTER_MOUNT*)(s_pCharMountDBC->Search_Index_EQU(nMountID));
		if(pCharMount != NULL)
		{
			nPassLevel = pCharMount->m_nPassLevel;
		}
	}
	CScene* pActiveScene = (CScene*)CWorldManager::GetMe()->GetActiveScene();

	//����յ��Ƿ�Ϸ�
	INT iCurZoneX = pActiveScene->GetZoneX(fvTarget.x);
	INT iCurZoneZ = pActiveScene->GetZoneZ(fvTarget.y);

	INT sGridX= iCurZoneX - 1;
	INT sGridZ= iCurZoneZ - 1;
	INT eGridX= iCurZoneX + 1;
	INT eGridZ= iCurZoneZ + 1;

	sGridX = max(0,sGridX);
	eGridX = min(pActiveScene->GetSizeX()-1, eGridX);

	sGridZ = max(0,sGridZ);
	eGridZ = min(pActiveScene->GetSizeZ()-1, eGridZ);

	// �ҵ���ʼ��ͽ����㾭��������zone�е�region
	CZone* pcz = NULL;
	std::vector<INT>	tempRegion;	
	for (INT i= sGridX;i<=eGridX;i++)
	{
		for (INT j= sGridZ;j<=eGridZ;j++)
		{
			CZone* pcz = pActiveScene->GetZone(i,j);
			for (std::list<INT>::iterator x= pcz->m_listRegion.begin();x!=pcz->m_listRegion.end();x++)
			{
				std::vector<INT>::iterator it = std::find(tempRegion.begin(),tempRegion.end(),(INT)*x);				
				if( it == tempRegion.end() )
				{
					tempRegion.push_back((INT)*x);
				}
			}
		}
	}

	INT nRegionSize = (INT)tempRegion.size();
	INT iRegionPointSize = 0;
	
	INT iInterSectCount  = 0;	// ���ߺ�ֱ�ߵ��ཻ������

	fVector2 f1s; 
	fVector2 f1e; 

	fVector2 f2s; 
	fVector2 f2e; 

	f2s = fvTarget;
	f2e.y = f2s.y;
	f2e.x = 1512.0f;

	// �������е�����
	for(INT i = 0 ;i < nRegionSize; i++ )
	{		
		const CRegion &cr = pActiveScene->m_theRegion[tempRegion[i]];
		if (nPassLevel > cr.m_nPassRegionLevel)
		{
			continue;
		}
		iInterSectCount = 0;
		// ���������ҵ�һ��ֱ��.
		iRegionPointSize = (INT)cr.m_vfPoints.size();
		for(INT j = 0; j < iRegionPointSize; j++)
		{
			if((iRegionPointSize - 1) == j)
			{
				f1s = cr.m_vfPoints[j];
				f1e = cr.m_vfPoints[0];
			}
			else
			{
				f1s = cr.m_vfPoints[j];
				f1e = cr.m_vfPoints[j + 1];
			}

			// ���ֱ���Ƿ��뵱ǰ��·���ཻ.
			if(CheckCross(f2s, f2e, f1s, f1e))
			{
				iInterSectCount++;
			}
		}

		if(iInterSectCount % 2)
		{
			return true;
		}
	}

	return FALSE;
}

//  ֱ�Ӳ���Ŀ�ĵ�
BOOL	CPath::GetPathDirect(const fVector2& fvCurrent, const fVector2& fvTarget)
{
	// �����·��.
	m_vPosStack.clear();

	// ���������򽻵��о�����̵�
	FLOAT fMinDis = 0;
	PathUnit newPathUnit;
	CScene* pActiveScene = (CScene*)CWorldManager::GetMe()->GetActiveScene();
	fVector2 fvStart = fvCurrent;
	fVector2 fvEnd = fvTarget;

	//����յ��Ƿ�Ϸ�
	INT sGridX= pActiveScene->GetZoneX(fvStart.x);
	INT sGridZ= pActiveScene->GetZoneZ(fvStart.y);
	INT eGridX= pActiveScene->GetZoneX(fvEnd.x);
	INT eGridZ= pActiveScene->GetZoneZ(fvEnd.y);
	if ((abs(sGridX-eGridX)>2) || (abs(sGridZ-eGridZ)>2))
	{		
		return FALSE;
	}

	// ��ֹ���ﻬ��.
	if (TDU_GetDist(fvStart,fvEnd) < 2.0f)
	{		
		return FALSE;
	}
	


	std::vector<INT>	tempRegion;	
	CScene* pActiveScene1 = (CScene*)CWorldManager::GetMe()->GetActiveScene();
	if(pActiveScene1)
	{
		int iSize  = (int)pActiveScene1->m_theRegion.size();
		for (int i = 0; i < iSize; i++)
		{		
			tempRegion.push_back(pActiveScene1->m_theRegion[i].m_RegionID);
		}
		
	}

	INT nRegionSize = (INT)tempRegion.size();
	INT iRegionPointSize = 0;
	fVector2 f1s; 
	fVector2 f1e; 

	BOOL  bInfinit1 = FALSE;
	BOOL  bInfinit2 = FALSE;
	FLOAT fK1 = 0;
	FLOAT fK2 = 0;
	FLOAT fB1 = 0;
	FLOAT fB2 = 0;

	FLOAT fTempDis = 0;

	FLOAT fTempX = fvTarget.x - fvCurrent.x;
	FLOAT fTempY = fvTarget.y - fvCurrent.y;

	FLOAT fFindX = 0;
	FLOAT fFindY = 0;

	FLOAT fTargetX = 0;
	FLOAT fTargetY = 0;

	// ������ֱ���غ�ʱ�õ�����ʱ����.
	FLOAT fDistance1 = 0;
	FLOAT fDistance2 = 0;
	FLOAT fDistance3 = 0;
	FLOAT x = 0;
	FLOAT y = 0;

	if(abs((FLOAT)fTempX) < 0.00001)
	{
		bInfinit1 = true;
	}
	else
	{
		// ��ֱ�ߵ�k��b����.
		fK1 = (fTempY * 1.0f) / (fTempX * 1.0f);
		fB1 = -fK1 * fvCurrent.x + fvCurrent.y;
	}


	// ��ʼ��ͽ�����ľ���.
	fTempX = fvTarget.x - fvCurrent.x;
	fTempY = fvTarget.y - fvCurrent.y;

	//fMinDis = sqrt((FLOAT)((fTempX * fTempX) + (fTempY)*(fTempY)) );
	fMinDis  = 1000.0f;
	fTargetX = fvTarget.x;
	fTargetY = fvTarget.y;

	// �������е�����
	for(INT i = 0 ;i < nRegionSize; i++ )
	{		
		const CRegion &cr = pActiveScene->m_theRegion[tempRegion[i]];
		
		// ���������ҵ�һ��ֱ��.
		iRegionPointSize = (INT)cr.m_vfPoints.size();
		for(INT j = 0; j < iRegionPointSize; j++)
		{
			if((iRegionPointSize - 1) == j)
			{
				f1s = cr.m_vfPoints[j];
				f1e = cr.m_vfPoints[0];
			}
			else
			{
				f1s = cr.m_vfPoints[j];
				f1e = cr.m_vfPoints[j + 1];
			}

			// ���ֱ���Ƿ��뵱ǰ��·���ཻ.
			if(CheckCross(fvCurrent, fvTarget, f1s, f1e))
			{
				fTempX = f1e.x - f1s.x;
				fTempY = f1e.y - f1s.y;

				if(abs((FLOAT)fTempX) < 0.00001)
				{
					bInfinit2 = true;
				}
				else
				{
					// ��ֱ�ߵ�k��b����.
					fK2 = (fTempY * 1.0f) / (fTempX * 1.0f);
					fB2 = -fK2 * f1s.x + f1s.y;
				}

				// �������ֱ�ߵ�б��һ��. �Ͱѿ�ʼ��ͽ����㶼�ŵ�������
				// Ŀǰ·���������ֻ��һ��ֱ��.
				// line1б���������, line1 ��ʼ����յ��ֱ�ߡ�
				if(bInfinit1)
				{
					// line2 ��б���������
					if(bInfinit2)
					{
						// ��ʼ��ͽ�����ľ���.
						fTempX = fvTarget.x - fvCurrent.x;
						fTempY = fvTarget.y - fvCurrent.y;

						fTempDis = sqrt((FLOAT)((fTempX * fTempX) + (fTempY)*(fTempY)) );
						if(fTempDis < fMinDis)
						{
							
							fMinDis = fTempDis;
							fTargetX = fvTarget.x;
							fTargetY = fvTarget.y;
							bInfinit2 = FALSE;
							continue;
						}
					}
					else // if(bInfinit2)
					{

						fFindX = fvTarget.x;
						fFindY = fK2 * fvTarget.x + fB2;
						fTempX = fvCurrent.x - (INT)fFindX;
						fTempY = fvCurrent.y - (INT)fFindY;
						fTempDis = sqrt((FLOAT)((fTempX * fTempX) + (fTempY)*(fTempY)));

						if(fTempDis < fMinDis)
						{
							
							fMinDis =fTempDis;
							fTargetX = fFindX;
							fTargetY = fFindY;
							bInfinit2 = FALSE;
							continue;
						}
					}
				}
				else if(bInfinit2) // if(bInfinit1) ֱ��2 ��б�������.
				{
					fFindX = f1s.x;
					fFindY = fK1 * fFindX + fB1;

					// ���ҵ���ֱ������ʼ��ľ���.
					fTempX = fvCurrent.x - (INT)fFindX;
					fTempY = fvCurrent.y - (INT)fFindY;
					fTempDis = sqrt((FLOAT)((fTempX * fTempX) + (fTempY)*(fTempY)));

					if(fTempDis < fMinDis)
					{
						// ������������С�� ����Ҫ�ҵ��յ�l
						fMinDis =fTempDis;
						fTargetX = fFindX;
						fTargetY = fFindY;
						bInfinit2 = FALSE;
						continue;
					}
				}
				else if(abs((FLOAT)(fK1 - fK2)) < 0.00001)
				{
					// ��ֱ���غ���ʼ��ͽ�����ľ���.
					fTempX = fvTarget.x - fvCurrent.x;
					fTempY = fvTarget.y - fvCurrent.y;
					fDistance1 = sqrt((FLOAT)((fTempX * fTempX) + (fTempY)*(fTempY)) );

					fTempX = f1s.x - fvCurrent.x;
					fTempY = f1s.y - fvCurrent.y;
					fDistance2 = sqrt((FLOAT)((fTempX * fTempX) + (fTempY)*(fTempY)) );

                    if(fDistance1 < fDistance2)
					{
						fTempDis = fDistance1;
						x = fvTarget.x;
						y = fvTarget.y;
					}
					else
					{
						fTempDis = fDistance2;
						x = f1s.x;
						y = f1s.y;
					}


					fTempX = f1e.x - fvCurrent.x;
					fTempY = f1e.y - fvCurrent.y;
					fDistance3 = sqrt((FLOAT)((fTempX * fTempX) + (fTempY)*(fTempY)) );

					if(fTempDis > fDistance3)
					{
						fTempDis = fDistance3;
						x = f1e.x;
						y = f1e.y;
					}

					if(fTempDis < fMinDis)
					{
						fMinDis =fTempDis;
						fTargetX = x;
						fTargetY = y;
						bInfinit2 = FALSE;
						continue;
					}
				}
				else
				{
					// ����ֱ�߱�׼�ཻ.
					fFindX = (fB2 - fB1) / (fK1 - fK2);
					fFindY = fK1 * fFindX + fB1;

					fTempX = fvCurrent.x - (INT)fFindX;
					fTempY = fvCurrent.y - (INT)fFindY;
					fTempDis = sqrt((FLOAT)((fTempX * fTempX) + (fTempY)*(fTempY)));

					if(fTempDis < fMinDis)
					{
						fMinDis =fTempDis;
						fTargetX = fFindX;
						fTargetY = fFindY;
						bInfinit2 = FALSE;
						continue;
					}
				}
			}
		}// for(INT j = 0; j < iRegionPointSize; j++) �������������еĵ�.
	}// for ( i = 0 ;i < nRegionSize; i++ ) �������е�����
	

	// ͣ��֮����Ҫ���ҵ����յ㣬 �ƺ�һ�ξ���
	// ��ֹ�յ�������߽������ �ߵ�������ȥ.
	FLOAT fOffsetX = fvTarget.x;
	FLOAT fOffsetY = fvTarget.y;

	FLOAT fDeltaX = fvTarget.x - fTargetX;
	FLOAT fDeltaY = fvTarget.y - fTargetY;

	FLOAT t = 0;

	if((abs((FLOAT)fDeltaX) > 0.0001 || abs((FLOAT)fDeltaY) > 0.0001))
	{
		fVector2 fFindPoint;

		fFindPoint.x = fTargetX;
		fFindPoint.y = fTargetY;
		t = sqrt(1.0f / ((FLOAT)(fDeltaX*fDeltaX + fDeltaY * fDeltaY)));
		t = -t;

		// �����������˲����������� �������߳�����
		if(IsPointInUnreachRegion(fFindPoint) && IsPointInUnreachRegion(fvCurrent))
		{
			t = -t;
		}//


		fOffsetX = t * fDeltaX + fTargetX;
		fOffsetY = t * fDeltaY + fTargetY;


		//���ڽӽ�����������,��ֹͻȻת��
		if(TDU_GetDistSq(fvCurrent, fVector2(fOffsetX, fOffsetY)) < 1.0f)	return FALSE;
	}

	m_vPosStack.clear();
	newPathUnit.fvStart	= fvCurrent;
	newPathUnit.fvTarget= fVector2(fOffsetX, fOffsetY);
	newPathUnit.dwRequestIndex = RequestIndexFactory();
	m_vPosStack.push_back(newPathUnit);//

	return true;
}

BOOL CPath::FindPath(fVector2 fvStart,  BOOL bBegin, INT iPreRegionIndex, INT iPreLineIndex, INT iPreVertexIndex)
{	
	BOOL bReturn = FALSE;
	//�õ���ǰ��������ȼ�
	INT nPassLevel = -1;
	INT nMountID = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_MountID();
	if(nMountID != INVALID_ID)
	{
		DBC_DEFINEHANDLE(s_pCharMountDBC, DBC_CHARACTER_MOUNT)
			const _DBC_CHARACTER_MOUNT *pCharMount = (const _DBC_CHARACTER_MOUNT*)(s_pCharMountDBC->Search_Index_EQU(nMountID));
		if(pCharMount != NULL)
		{
			nPassLevel = pCharMount->m_nPassLevel;
		}
	}
	// �ݹ������ȡ�
	static INT iDep = 0;
	if(iDep > 20)
	{
		iDep = 0;
		m_vPosStack.clear();
		return FALSE;
	}

	// ���Ӳ������.
	iDep++;

	if(bBegin)
	{
		iDep = 0;
		iPreRegionIndex = -1;	// ǰһ�ε��õ�region����
		iPreLineIndex   = -1;	// ǰһ�ε��õ��赲������
		iPreVertexIndex = -1;	// ǰһ���ҵ��ĵ㡣
		m_iChangeRegion = -1;
		m_iChangeLine   = -1;
		m_vPosStack.clear();
	}


	// �����Ƿ��Ѿ������յ㡣
	if(IsReachDes(fvStart,nPassLevel))
	{
		if(sqrt((fvStart.x - m_TargetPos.x)*(fvStart.x - m_TargetPos.x) + (fvStart.y - m_TargetPos.y)*(fvStart.y - m_TargetPos.y)) < 0.01)
		{
			return FALSE;
		}

		PathUnit newPathUnit;
		newPathUnit.fvStart	 = fvStart;
		newPathUnit.fvTarget = m_TargetPos;
		newPathUnit.dwRequestIndex = RequestIndexFactory();
		m_vPosStack.push_back(newPathUnit);
		
		return TRUE;
	}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
// ����·��
//

	CScene* pActiveScene = (CScene*)CWorldManager::GetMe()->GetActiveScene();
	fVector2 p1;
	fVector2 p2;

	FLOAT fFindX = 0;		// ���ҵĵ�x�� 
	FLOAT fFindY = 0;		// ���ҵĵ�y��

	INT iFindRegion = -1;   // �������ཻ�ҵ�����������
	INT iFindLine   = -1;	// �������ཻ�ҵ����ߵ�����

	// ��������յ�, �Ͳ�����һ���㡣
	if(-1 != iPreRegionIndex)
	{// ����һ�������ϲ��ҡ�
		
		INT iIndex1 = -1;
		INT iIndex2 = -1;
		INT iCheckLine = -1;
		
		// �жϵ�ǰ���뵱ǰ�赲�����Ƿ��ཻ������ཻ�������ҵ�ǰ������ ������ཻ�� ������һ������
		// �ҵ�Ҫ���ҵ�����
		const CRegion &CurRegion = pActiveScene->m_theRegion[iPreRegionIndex];
		
		// ����һ���㣬 ����������뵱ǰ��region�Ƿ��ཻ��
		fVector2 tempCheckPoint;
		tempCheckPoint.x = fvStart.x + LINE_CROSS_RANGE * (m_TargetPos.x - fvStart.x);
		tempCheckPoint.y = fvStart.y + LINE_CROSS_RANGE * (m_TargetPos.y - fvStart.y);

		if(CheckCross(tempCheckPoint, m_TargetPos, &CurRegion))
		{// �����ǰ����յ������뵱ǰ�����ཻ������������в���
			
			if(-1 != iPreVertexIndex)
			{// ǰһ���ҵ�����һ���߽�㡣

				// ����ҵ�����������ҵ����ߵ�������ͬ�� ���ǰ�������ķ�����ҵ㡣
				if(iPreVertexIndex == iPreLineIndex)
				{
					iCheckLine = iPreLineIndex - 1;
					if(iCheckLine < 0)
					{
						iCheckLine = (INT)CurRegion.m_vfPoints.size() - 1;
					}
				}
				else 
				{// ���� �Ͱ����������������ҡ�
					iCheckLine = iPreLineIndex + 1;
					iCheckLine = iCheckLine % (INT)CurRegion.m_vfPoints.size();
				}
			}
			else
			{ // ��������Ǵ�������region�߹�������������һ���ཻ�� û�в����������Ķ���������
				iCheckLine = iPreLineIndex;
			}

			// �ҵ������߽�㡣
			if(iCheckLine+1 == CurRegion.m_vfPoints.size())
			{
				// ��������������һ���㡣
				// ��һ������ĩβ�ĵ�
				// �ڶ������ǿ�ʼ�ĵ�
				iIndex1 = iCheckLine;
				iIndex2 = 0;
			}
			else
			{
				// ��һ����������ߵĵ�һ����
				// �ڶ�����������赲�ߵ���һ���㡣
				iIndex1 = iCheckLine;
				iIndex2 = iCheckLine + 1;
			}

			p1 = CurRegion.m_vfPoints[iIndex1];
			p2 = CurRegion.m_vfPoints[iIndex2];

			// Ҫ���ĵ�
			//fVector2 tempCheckPoint;

			// ��һ�β��ҵĵ��뵱ǰ�ĵ���һ������һ����
			if((iPreVertexIndex != iIndex1) &&(iPreVertexIndex != iIndex2))
			{// �����һ���ҵ��Ĳ��������߽�㡣
				FLOAT fDis1 = 0;
				FLOAT fDis2 = 0;

				fDis1 = (fvStart.x - p1.x)*(fvStart.x - p1.x) + (fvStart.y - p1.y)*(fvStart.y - p1.y);
				fDis2 = (fvStart.x - p2.x)*(fvStart.x - p2.x) + (fvStart.y - p2.y)*(fvStart.y - p2.y);

				if(fDis1 > fDis2)
				{
					INT      iChangeIndex = -1;
					fVector2 fvChangePoint;
					fvChangePoint = p1;
					p1 = p2;
					p2 = fvChangePoint;

					iChangeIndex = iIndex1;
					iIndex1 = iIndex2;
					iIndex2 = iChangeIndex;
				}


				if(GetMinCrossPos(fvStart, p1, fFindX, fFindY, iFindRegion, iFindLine, iPreRegionIndex))
				{// ��������������ཻ

					if((m_iChangeRegion == iFindRegion)&&(iFindLine == m_iChangeLine))
					{// �����ǰһ�������ཻ
						
						fVector2 fTestPoint;
						fTestPoint.x  = fvStart.x + LINE_CROSS_RANGE*(p2.x - fvStart.x);
						fTestPoint.y  = fvStart.y + LINE_CROSS_RANGE*(p2.y - fvStart.y);

						if(GetMinCrossPos(fTestPoint, p2, fFindX, fFindY, iFindRegion, iFindLine, iPreRegionIndex))
						{// ������һ���㡣
							m_iChangeRegion = iPreRegionIndex;
							m_iChangeLine   = iPreLineIndex;

							fVector2 fvEnd;
							PathUnit newPathUnit;

							fvEnd.x = fFindX;
							fvEnd.y = fFindY;

							if(IsPointInUnreachRegion(fvEnd))
							{
								//return TRUE;
							}

							newPathUnit.fvStart	 = fTestPoint;
							newPathUnit.fvTarget = fvEnd;
							newPathUnit.dwRequestIndex = RequestIndexFactory();
							m_vPosStack.push_back(newPathUnit);
							// �ҵ��ڶ����㣬
							bReturn = FindPath(newPathUnit.fvTarget, FALSE, iFindRegion, iFindLine, -1);
						}
						else
						{
							m_iChangeRegion = iPreRegionIndex;
							m_iChangeLine   = iPreLineIndex;
							
							
							if(IsPointInUnreachRegion(p2))
							{
								//return TRUE;
							}

							PathUnit newPathUnit;
							newPathUnit.fvStart	 = fTestPoint;
							newPathUnit.fvTarget = p2;
							newPathUnit.dwRequestIndex = RequestIndexFactory();
							m_vPosStack.push_back(newPathUnit);
							bReturn =FindPath(newPathUnit.fvTarget, FALSE, iPreRegionIndex, iCheckLine, iIndex2);

						}

					}
					else// �������ǰһ�������߹�����
					{
						fVector2 fTestPoint;
						fTestPoint.x  = fvStart.x + LINE_CROSS_RANGE*(p2.x - fvStart.x);
						fTestPoint.y  = fvStart.y + LINE_CROSS_RANGE*(p2.y - fvStart.y);

						INT iFindRegionP2 = -1;
						INT iFindLineP2   = -1;

						FLOAT fFindP2X = 0;
						FLOAT fFindP2Y = 0;

						if(GetMinCrossPos(fTestPoint, p2, fFindP2X, fFindP2Y, iFindRegionP2, iFindLineP2, iPreRegionIndex))
						{// ������һ���㡣
							
							// ������涼�ཻ�Ͳ������ˣ� �˳���
							return FALSE;
						}
						else 
						{// ��ڶ��������ཻ�����ҵڶ�����.

							if(IsPointInUnreachRegion(p2))
							{
								//return TRUE;
							}

                            PathUnit newPathUnit;
							newPathUnit.fvStart	 = fTestPoint;
							newPathUnit.fvTarget = p2;
							newPathUnit.dwRequestIndex = RequestIndexFactory();
							m_vPosStack.push_back(newPathUnit);
							bReturn = FindPath(newPathUnit.fvTarget, FALSE, iPreRegionIndex, iCheckLine, iIndex2);
						}

					}

				}// ��һ���������������ཻ����
				else
				{
					if(IsPointInUnreachRegion(p1))
					{
						//return TRUE;
					}


					//������ཻ��ȡ�����
					PathUnit newPathUnit;
					newPathUnit.fvStart	 = fvStart;
					newPathUnit.fvTarget = p1;
					newPathUnit.dwRequestIndex = RequestIndexFactory();
					m_vPosStack.push_back(newPathUnit);
					bReturn = FindPath(newPathUnit.fvTarget, FALSE, iPreRegionIndex, iCheckLine, iIndex1);

				}

			}
			// �����һ���ҵ��ĵ��һ���߽�� 
			if(iPreVertexIndex == iIndex1)
			{
			
				tempCheckPoint = p2;
				// ���������һ���㡣
				if(GetMinCrossPos(fvStart, tempCheckPoint, fFindX, fFindY, iFindRegion, iFindLine, iPreRegionIndex))
				{// �ҵ�һ���ཻ������

					// �ı�ǰһ�ε����������.
					m_iChangeRegion = iPreRegionIndex;
					m_iChangeLine   = iPreLineIndex;

					fVector2 fvEnd;
					PathUnit newPathUnit;

					fvEnd.x = fFindX;
					fvEnd.y = fFindY;

					if(IsPointInUnreachRegion(fvEnd))
					{
						//return TRUE;
					}
					newPathUnit.fvStart	 = fvStart;
					newPathUnit.fvTarget = fvEnd;
					newPathUnit.dwRequestIndex = RequestIndexFactory();
					m_vPosStack.push_back(newPathUnit);
					bReturn = FindPath(newPathUnit.fvTarget, FALSE, iFindRegion, iFindLine,-1);
				}
				else
				{
					if(IsPointInUnreachRegion(tempCheckPoint))
					{
						//return TRUE;
					}

					// ���ཻ�Ͱ�����ĵڶ��������¼·��
					PathUnit newPathUnit;
					newPathUnit.fvStart	 = fvStart;
					newPathUnit.fvTarget = tempCheckPoint;
					newPathUnit.dwRequestIndex = RequestIndexFactory();
					m_vPosStack.push_back(newPathUnit);
					bReturn = FindPath(newPathUnit.fvTarget, FALSE, iPreRegionIndex, iCheckLine,iIndex2);
				}

			}
			// �����һ���ҵ��ĵ�ڶ����߽�� 
			else if(iPreVertexIndex == iIndex2)
			{
				
				tempCheckPoint = p1;
				// ���������һ���㡣
				if(GetMinCrossPos(fvStart, tempCheckPoint, fFindX, fFindY, iFindRegion, iFindLine, iPreRegionIndex))
				{// �ҵ�һ���ཻ������

					// �ı�ǰһ�ε����������.
					m_iChangeRegion = iPreRegionIndex;
					m_iChangeLine   = iPreLineIndex;

					fVector2 fvEnd;
					PathUnit newPathUnit;

					fvEnd.x = fFindX;
					fvEnd.y = fFindY;

					if(IsPointInUnreachRegion(fvEnd))
					{
						//return TRUE;
					}

					newPathUnit.fvStart	 = fvStart;
					newPathUnit.fvTarget = fvEnd;
					newPathUnit.dwRequestIndex = RequestIndexFactory();
					m_vPosStack.push_back(newPathUnit);
					bReturn = FindPath(newPathUnit.fvTarget, FALSE, iFindRegion, iFindLine,-1);
				}
				else
				{
					
					if(IsPointInUnreachRegion(tempCheckPoint))
					{
						//return TRUE;
					}


					// ���ཻ�Ͱ�����ĵڶ��������¼·��
					PathUnit newPathUnit;
					newPathUnit.fvStart	 = fvStart;
					newPathUnit.fvTarget = tempCheckPoint;
					newPathUnit.dwRequestIndex = RequestIndexFactory();
					m_vPosStack.push_back(newPathUnit);
					bReturn = FindPath(newPathUnit.fvTarget, FALSE, iPreRegionIndex, iCheckLine,iIndex1);
				}

			}

		}//  �����ǰ����յ������뵱ǰ�����ཻ������������в���
		else
		{//  �����ǰ����յ������뵱ǰ�����ཻ�� �������ҵ�һ���µ���������
			
			// �ҵ�����ཻ������
			if(GetMinCrossPos(fvStart, m_TargetPos, fFindX, fFindY, iFindRegion, iFindLine, iPreRegionIndex))
			{
				m_iChangeRegion = iPreRegionIndex;
				m_iChangeLine   = iPreLineIndex;

				fVector2 fvEnd;
				PathUnit newPathUnit;

				fvEnd.x = fFindX;
				fvEnd.y = fFindY;

				if(IsPointInUnreachRegion(fvEnd))
				{
					//return TRUE;
				}

				newPathUnit.fvStart	 = fvStart;
				newPathUnit.fvTarget = fvEnd;
				newPathUnit.dwRequestIndex = RequestIndexFactory();
				m_vPosStack.push_back(newPathUnit);
				bReturn = FindPath(newPathUnit.fvTarget, FALSE, iFindRegion, iFindLine,-1);
			}

		}

	}// ��һ�������ϲ������ߵ㡣
	else
	{// ����һ������Ĳ�����������
		
		// �ҵ�����ཻ������
		if(GetMinCrossPos(fvStart, m_TargetPos, fFindX, fFindY, iFindRegion, iFindLine, iPreRegionIndex))
		{
			m_iChangeRegion = iPreRegionIndex;
			m_iChangeLine   = iPreLineIndex;

			fVector2 fvEnd;
			PathUnit newPathUnit;

			fvEnd.x = fFindX;
			fvEnd.y = fFindY;

			if(IsPointInUnreachRegion(fvEnd))
			{
				//return TRUE;
			}

			newPathUnit.fvStart	 = fvStart;
			newPathUnit.fvTarget = fvEnd;
			newPathUnit.dwRequestIndex = RequestIndexFactory();
			m_vPosStack.push_back(newPathUnit);
			bReturn = FindPath(newPathUnit.fvTarget, FALSE, iFindRegion, iFindLine,-1);
		}
	}

	return bReturn;
}

// �Ƿ񵽴��յ㡣
BOOL CPath::IsReachDes(fVector2 fvStart,int level)
{
	fVector2 tempStart;
	tempStart.x = fvStart.x + LINE_CROSS_RANGE * (m_TargetPos.x - fvStart.x);
	tempStart.y = fvStart.y + LINE_CROSS_RANGE * (m_TargetPos.y - fvStart.y);

	CScene* pActiveScene = (CScene*)CWorldManager::GetMe()->GetActiveScene();
	INT iRegionSize = (INT)pActiveScene->m_theRegion.size();
	// �������е�����
	for ( INT i = 0 ;i < iRegionSize; i++ )
	{		
		const CRegion &cr = pActiveScene->m_theRegion[i];	
		if (level > cr.m_nPassRegionLevel)
		{
			continue;
		}
		if(CheckCross(tempStart, m_TargetPos, &cr))
		{
			return FALSE;
		}
	}
	return TRUE;
}

// ������������õ�����ĵ�
BOOL CPath::GetMinCrossPos(fVector2 fvStart, fVector2 fvEnd, FLOAT& fFindx, FLOAT& fFindy, INT& iRegionIndex, INT& iLineIndex, INT iExcludeRegion)
{
	BOOL	bFind = FALSE;
	FLOAT	fCheckFindX = 0;
	FLOAT   fCheckFindY = 0;

	FLOAT   fDis = 10000.0f;
	FLOAT   fFindDis = 0;

	INT		iFindLineIndex = -1;

	CScene* pActiveScene = (CScene*)CWorldManager::GetMe()->GetActiveScene();
	INT iRegionSize = (INT)pActiveScene->m_theRegion.size();
	// �������е�����
	for ( INT i = 0 ;i < iRegionSize; i++ )
	{		
		// ��������ǲ����ҵľ��ƹ�ȥ��
		if(iExcludeRegion == pActiveScene->m_theRegion[i].m_RegionID)
		{
			continue;
		}

        const CRegion &cr = pActiveScene->m_theRegion[i];
		if(CheckCross(fvStart, fvEnd, &cr, fCheckFindX, fCheckFindY, fFindDis, iFindLineIndex))
		{
			if(fFindDis < fDis)
			{
				bFind  = true;
				fDis   = fFindDis;
				fFindx = fCheckFindX;
				fFindy = fCheckFindY;
				iLineIndex   = iFindLineIndex;
				iRegionIndex = pActiveScene->m_theRegion[i].m_RegionID;
			}
		}
	}//

	return bFind;
}

// ���ҵ�ǰ��ֱ����һ�������Ƿ��ཻ������ཻ���ؽ��������������ֱཻ�ߵ�����ֵ
BOOL CPath::CheckCross(fVector2 fvStart, fVector2 fvEnd, const CRegion* pcr, FLOAT& fFindX, FLOAT& fFindY, FLOAT& fFindDis, INT& iFindLineIndex)
{
	if(NULL == pcr) return FALSE;

	fVector2 p1;
	fVector2 p2;
	FLOAT fDistance = 10000.0f;
	FLOAT fDis      = 0;
	FLOAT fCurFindX = 0;
	FLOAT fCurFindY = 0;
	BOOL  bFind = FALSE;


	INT iPointSize = (INT)pcr->m_vfPoints.size();
	for(INT i = 0; i < iPointSize; i++)
	{
		if(i == iPointSize - 1)
		{
			p1 = pcr->m_vfPoints[i];
			p2 = pcr->m_vfPoints[0];
		}
		else
		{
			p1 = pcr->m_vfPoints[i];
			p2 = pcr->m_vfPoints[i+1];
		}

		if(CheckLineCross(fvStart, fvEnd, p1, p2, fCurFindX, fCurFindY))
		{
			bFind = true;
			fDis = sqrt((fvStart.x - fCurFindX)*(fvStart.x - fCurFindX) + (fvStart.y - fCurFindY)*(fvStart.y - fCurFindY));
			if(fDis < fDistance)
			{
				fDistance = fDis;
				fFindX    = fCurFindX;
				fFindY    = fCurFindY;
				iFindLineIndex = i;
				fFindDis  = fDis;

			}
		}

	}

	return bFind;
}


BOOL CPath::CheckLineCross(fVector2 fv1Start, fVector2 fv1End, fVector2 fv2Start, fVector2 fv2End, FLOAT& fcrossx, FLOAT& fcrossy)
{

	FLOAT fsx1 = fv1Start.x;
	FLOAT fsy1 = fv1Start.y; 
	FLOAT fex1 = fv1End.x;
	FLOAT fey1 = fv1End.y; 
	FLOAT fsx2 = fv2Start.x;
	FLOAT fsy2 = fv2Start.y;
	FLOAT fex2 = fv2End.x;
	FLOAT fey2 = fv2End.y;


	FLOAT k1 = 0;
	FLOAT b1 = 0;
	FLOAT k2 = 0;
	FLOAT b2 = 0;

	BOOL  bLine1infinite = FALSE;		// ֱ��1��б���Ƿ��������
	BOOL  bLine2infinite = FALSE;		// ֱ��2��б���Ƿ��������

	FLOAT tempY = 0;
	FLOAT tempX = 0;


	// ��ֱ�ߵ�k��bֵ
	if(abs(fex1 - fsx1) < 0.0001)
	{
		bLine1infinite = true;
	}
	else
	{
		k1 = (fey1 - fsy1) / (fex1 - fsx1);
		b1 = -fsx1 * k1 +  fsy1;
	}


	if(abs(fex2 - fsx2) < 0.0001)
	{
		bLine2infinite = true;
	}
	else
	{
		k2 = (fey2 - fsy2) / (fex2 - fsx2);
		b2 = -fsx2 * k2 +  fsy2;
	}

	
	// �ж�ֱ���Ƿ��ཻ
	if(bLine1infinite)
	{// ֱ��1�Ǵ��ߵ������
		if(bLine2infinite)
		{
			if(abs(fsx1 - fsx2) < 0.0001)
			{
				// ����ֱ�߶��Ǵ�ֱ�ߡ� ���xֵ��Ⱦ���ͬһ��ֱ���ϡ�
				// �����һ��ֱ�ߵĿ�ʼyֵ �ڵڶ���ֱ�ߵ�yֵ֮���һ��ֱ�ߵĿ�ʼ�������Ϊ�ǽ��㣨������ֱ�߿����кܴ��غϣ�
				if((fsy1 - fsy2)*(fsy1 - fey2) < 0)
				{
					fcrossx = fsx1;
					fcrossy = fsy1;
					return TRUE;
				}
				else if((fey1 - fsy2)*(fey1 - fey2) < 0)
				{
					fcrossx = fex1;
					fcrossy = fey1;
					return TRUE;
				}

				return FALSE;
				
			}
			else
			{
				// �����߶β���ͬһ��ֱ���ϡ�
				return FALSE;
			}
		}
		else // ֱ��2���Ǵ�ֱ�ߵ������
		{
			tempY = k2 * fsx1 + b2;
			if(((tempY - fsy2)*(tempY - fey2) < 0)&&((tempY - fsy1)*(tempY - fey1) < 0))
			{
				fcrossx = fsx1;
				fcrossy = tempY;
				return TRUE;
			}
			else
			{
				return FALSE;
			}
				
		}
	}
	else
	{// ֱ��1���Ǵ��ߵ������

		if(bLine2infinite)
		{
			tempY = k1 * fsx2 + b1;
			if(((tempY - fsy2)*(tempY - fey2) < 0)&&((tempY - fsy1)*(tempY - fey1) < 0))
			{
				fcrossx = fsx2;
				fcrossy = tempY;
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
		else// if(bLine2infinite)
		{

			if(abs(k1 - k2) < 0.0001)
			{// ����ֱ��ƽ��

				if(abs(b1 - b2) < 0.0001)
				{// ����ֱ���غ�
					
					if((fsy1 - fsy2)*(fsy1 - fey2) < 0)
					{
						fcrossx = fsx1;
						fcrossy = fsy1;
						return TRUE;
					}
					else if((fey1 - fsy2)*(fey1 - fey2) < 0)
					{
						fcrossx = fex1;
						fcrossy = fey1;
						return TRUE;
					}

					return FALSE;

				}// if(abs(b1 - b2) < 0.0001)
				else
				{
					return FALSE;

				}
			}
			else
			{
				tempX = (b1 - b2) / (k2 - k1);
				FLOAT ft1 = 0;
				FLOAT ft2 = 0;
				
				ft1 = (tempX - fsx1) / (fex1 - fsx1);
				ft2 = (tempX - fsx2) / (fex2 - fsx2);

				if((ft1 > -0.0002)&&(ft1 < 1.0002)&&(ft2 > -0.0002)&&(ft2 < 1.0002))
				{
					fcrossx = tempX;
					fcrossy = k1 * tempX + b1;
					return TRUE;
				}
				else
				{
					return FALSE;
				}
			}
		}
	}
}

INT	 CPath::BuildDijkPath(const fVector2& fvCurrent, const fVector2& fvTarget, int nPassLevel)
{
	m_vPosStack.clear();
	
	// �����Ƿ��Ѿ������յ㡣
	if(IsReachDes(fvCurrent,nPassLevel))
	{
		if(sqrt((fvCurrent.x - m_TargetPos.x)*(fvCurrent.x - m_TargetPos.x) + (fvCurrent.y - m_TargetPos.y)*(fvCurrent.y - m_TargetPos.y)) < 0.01)
			return 0;
		PathUnit newPathUnit;
		newPathUnit.fvStart	 = fvCurrent;
		newPathUnit.fvTarget = m_TargetPos;
		newPathUnit.dwRequestIndex = RequestIndexFactory();
		m_vPosStack.push_back(newPathUnit);
		return 1;
	}

	CScene* pActiveScene = (CScene*)CWorldManager::GetMe()->GetActiveScene();
	DWORD dwPointCount = (DWORD)pActiveScene->m_Points.size();
	if( dwPointCount == 0 ) return 0;

	//1. ������ʼ��ͽ��������֪�����Ȩ��
	for( DWORD dw = 0; dw < dwPointCount; dw ++ )
	{
		fVector2& thePoint = pActiveScene->m_Points[dw];
		FLOAT ws = FLT_MAX;
		FLOAT wd = FLT_MAX;
		BOOL bFindS = FALSE;
		BOOL bFindD = FALSE;
		for( size_t r = 0; r < pActiveScene->m_theRegion.size(); r ++ )
		{
			CRegion& re = pActiveScene->m_theRegion[r];
			if (nPassLevel > re.m_nPassRegionLevel)
			{
				continue;
			}
			if( bFindS == FALSE && CPath::CheckCross( fvCurrent, thePoint, &re ) )
				bFindS = TRUE;
			if( bFindD == FALSE && CPath::CheckCross( fvTarget, thePoint, &re ) )
				bFindD = TRUE;

			if( bFindS && bFindD ) break;
		}

		if( bFindS == FALSE ) //��ʼ��͵�ǰ�㻥ͨ
			ws = (fvCurrent.x-thePoint.x)*(fvCurrent.x-thePoint.x) + (fvCurrent.y-thePoint.y)*(fvCurrent.y-thePoint.y);

		if( bFindD == FALSE ) //��ֹ��͵�ǰ�㻥ͨ
			wd = (fvTarget.x-thePoint.x)*(fvTarget.x-thePoint.x) + (fvTarget.y-thePoint.y)*(fvTarget.y-thePoint.y);

		pActiveScene->m_Weights[(dw+1)] = ws;
		pActiveScene->m_Weights[(dw+1)*(dwPointCount+2)] = ws;
		pActiveScene->m_Weights[(dw+1)*(dwPointCount+2)+(dwPointCount+1)] = wd;
		pActiveScene->m_Weights[(dwPointCount+1)*(dwPointCount+2)+(dw+1)] = wd;
	}

	//2. ʵ�ʵĵ�������2
	dwPointCount += 2;

	//3. ��ʼ��·��...
	vector<vector<DWORD> > thePath;
	thePath.resize( dwPointCount );

	//5. ��ʼ��final��
	vector<BYTE> final;
	final.resize( dwPointCount );

	//6. ��ʼ�����·��
	pActiveScene->m_Dist[0] = 0;
	pActiveScene->m_Dist[dwPointCount-1] = FLT_MAX;
	final[0] = 0;
	final[dwPointCount-1] = 0;
	for( DWORD dw = 1; dw < dwPointCount-1; dw++ )
	{
		final[dw] = 0;
		pActiveScene->m_Dist[dw] = pActiveScene->m_Weights[dw];
	}

	//7. �������еĵ�
	FLOAT min = FLT_MAX;
	final[0] = 1;
	DWORD v;
	for( DWORD dw = 0; dw < dwPointCount-1; dw++ )
	{
		min = FLT_MAX;

		//Ѱ����̵ı�
		BOOL bFind = FALSE;
		for( DWORD w = 0; w < dwPointCount; w++ )
		{
			if( final[w] == 0 && pActiveScene->m_Dist[w] < min )
			{
				min = pActiveScene->m_Dist[w];
				v = w;
				bFind = TRUE;
			}
		}
		if(bFind == FALSE) return 0;
		final[v] = 1; // �����±� 

		for( DWORD w = 0; w < dwPointCount; w++ )
		{
			//���� dist[] ����
			if( final[w] == 0 && pActiveScene->m_Dist[v] + pActiveScene->m_Weights[v+w*dwPointCount] < pActiveScene->m_Dist[w] )
			{
				pActiveScene->m_Dist[w] = pActiveScene->m_Dist[v] + pActiveScene->m_Weights[v+w*dwPointCount]; //��Ҫ����w�㣬����Ҫ����v

				//����·��
				thePath[w] = thePath[v];
				thePath[w].push_back(v);
			}
		}
	}

	//8. ������
	vector<DWORD>& paths = thePath[dwPointCount-1];
	PathUnit pu;
	pu.fvStart = fvCurrent;
	for( DWORD dw = 0; dw < paths.size(); dw ++ )
	{
		pu.fvTarget = pActiveScene->m_Points[paths[dw]-1];
		m_vPosStack.push_back(pu);
		pu.fvStart = pu.fvTarget;
	}
	pu.fvTarget = fvTarget;
	m_vPosStack.push_back(pu);

	return (INT)m_vPosStack.size();
}















