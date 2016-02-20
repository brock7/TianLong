#include "StdAfx.h"
#include "../RenderSystem.h"
#include "Camera_Scene.h"
#include "TDMath.h"
#include <OgreCamera.h>
#include <OgreRoot.h>
#include "TDUtil.h"
#include "TDException.h"
#include "Core/WXSystem.h"
#include "Core/TerrainData.h"
#include "../Global.h"
#include "TDProfile.h"
#include "TDVariable.h"
//============================================================================================
FLOAT CCamera_Scene::STATIC_PITCH	= -38.5f*TDU_PI/180.0f;	//�̶��ӽ�
FLOAT CCamera_Scene::MIN_DISTANCE	= 6.0f;					//���������������ܹ���������������
FLOAT CCamera_Scene::MAX_DISTANCE	= 14.0f;				//���������������ܹ�Զ���������Զ����
FLOAT CCamera_Scene::MIN_ANGLE		= 0.1f*TDU_PI/180.0f;	//�Ƕ���С�������
FLOAT CCamera_Scene::STEP_ANGLE		= 1.0f*TDU_PI/180.0f;	//�Ƕ���С������Χ

//��ר�þֲ�����
static	FLOAT	fCurBias			= 0;
static	FLOAT	fBiasStep			= 0;
static	BOOL	bCurDirIsUp			= TRUE;
static	UINT	uAlreadyVibTimes	= 0;


CCamera_Scene::CCamera_Scene(Ogre::Camera* pOgreCamera, const fVector3& fvPos, FLOAT fAddHeight, FLOAT fPitch, FLOAT fDistance, FLOAT fDirection)
	: CCamera(pOgreCamera)
{
	//-----------------------------------------------
	//���г�ʼ������
	SetLookAt(fvPos);
	SetAddHeight(fAddHeight);
	SetPitch(fPitch);
	SetDistance(fDistance);
	SetDirection(fDirection);

	m_status = NORMAL_STATUS;
}

CCamera_Scene::~CCamera_Scene(VOID)
{
}

VOID CCamera_Scene::SetLookAt(const fVector3& fvPos)
{
	fVector3 fvTemp;
	if(!CRenderSystem::GetMe()->Axis_Trans(CRenderSystem::AX_GAME, 
		fVector3(fvPos.x, fvPos.y, fvPos.z), CRenderSystem::AX_GFX, fvTemp)) return;

	//λ����ͬ�����۾�Ҳ���ûع���ֱ�ӷ���
	if(fvTemp == m_fvLookatPos && STATIC_PITCH-m_fPitch<MIN_ANGLE && m_status == NORMAL_STATUS) return;

	m_fvLookatPos = fvTemp;
	Update();
}

VOID CCamera_Scene::SetAddHeight(FLOAT fAddHeight)
{
	TDAssert(fAddHeight >= 0.0f);


	m_fAddHeight = fAddHeight;

	Update();
}

VOID CCamera_Scene::SetPitch(FLOAT fPitch)
{
	m_fPitch = fPitch;
	Update();
}

VOID CCamera_Scene::SetDistance(FLOAT fDistance)
{
	TDAssert(fDistance>0.0f);

	//ȡ�÷���ϵ��
	fVector3 fvScale = CRenderSystem::GetMe()->GetSacle();

	//Attention!!!
	//һ����˵��fvScale������ֵ����һ���ģ�so...
	m_fDistance = fDistance*fvScale.y;
	Update();
}

FLOAT CCamera_Scene::GetDistance(VOID) const
{
	//ȡ�÷���ϵ��
	fVector3 fvScale = CRenderSystem::GetMe()->GetSacle();

	return m_fDistance/fvScale.y;
}

VOID CCamera_Scene::SetDirection(FLOAT fDirection)
{
	m_fDirection = fDirection;

	Update();
}

VOID CCamera_Scene::Update(VOID)
{
	//-----------------------------------------
	//�����Ƿ����
	if(CRenderSystem::GetMe()->GetRenderSystem()->getTerrainData()->isEmpty()) return;

	//-----------------------------------------
	//ȡ�÷���ϵ��
	fVector3 fvScale = CRenderSystem::GetMe()->GetSacle();

	//-----------------------------------------
	//����������ĵ�ת��Ϊ��Ⱦϵͳ����ϵ
	fVector3 fvGfxLookatPos = m_fvLookatPos;

	//���Ӹ��ӵĸ߶�(һ��������߶�)
	fvGfxLookatPos.y += m_fAddHeight*fvScale.y;

	//-------------------------------------------------
	//��������ε��������������ӽ���Pitch
	m_fPitch = _GetValidPitch(
						CRenderSystem::GetMe()->GetRenderSystem()->getTerrainData(),	//��������
						fvGfxLookatPos,												//��ע��
						m_fDistance,												//����
						m_fDirection,												//��ת�Ƕ�
						m_fPitch,													//Ŀǰ��Pitch(�����ӽ���ֵ)
						STATIC_PITCH,												//Ҫ������Pitch
						m_pOgreCamera->getNearClipDistance()
						);

	//-------------------------------------------------
	//������۾����ڵ�λ��
	fVector3 vEyePos = _GetEyePos(fvGfxLookatPos, m_fDistance, m_fDirection, m_fPitch);

	//-------------------------------------------------
	//���ݵ�ǰ�����״̬����ƫ��
	_UpdateCurStatus(vEyePos, fvGfxLookatPos);
	
	//-------------------------------------------------
	//����OGRE���
	m_pOgreCamera->setPosition(Ogre::Vector3(vEyePos.x, vEyePos.y, vEyePos.z));
	m_pOgreCamera->lookAt(Ogre::Vector3(fvGfxLookatPos.x, fvGfxLookatPos.y, fvGfxLookatPos.z));

	BOOL bHave;
	FLOAT fCamera_FOV = g_pVaribleSys->GetAs_Float("Camera_FOV", &bHave);
	float fAR;
	if(bHave)
		fAR = fCamera_FOV;
	else
		fAR = 45.0;


//	float fAR = m_pOgreCamera->getFOVy().valueDegrees();
	m_pOgreCamera->setFOVy(Ogre::Radian(Ogre::Degree(fAR).valueRadians()));


	//-------------------------------------------------
	//Info

	//--- for debug
	//fVector3 vGameEye;
	//CRenderSystem::GetMe()->Axis_Trans(CRenderSystem::AX_GFX, vEyePos, CRenderSystem::AX_GAME, vGameEye);

	//char szTemp[32];
	//_snprintf(szTemp, 32, "%.1f,%.1f,%.1f", 
	//	vGameEye.x, vGameEye.y, vGameEye.z);

	//CRenderSystem::GetMe()->SetDebugString("Eye", szTemp);
	//--- for debug



	CHAR szTemp[MAX_PATH];

	_snprintf(szTemp, MAX_PATH, "%.1f", -m_fPitch*180/TDU_PI);
	CRenderSystem::GetMe()->SetDebugString("CamPitch", szTemp);

	_snprintf(szTemp, MAX_PATH, "%.1f", m_fDistance);
	CRenderSystem::GetMe()->SetDebugString("CamDist", szTemp);

	//fVector3 fvEyeGame;
	//CRenderSystem::GetMe()->Axis_Trans(tGfxSystem::AX_GFX, vEyePos, 
	//	tGfxSystem::AX_GAME, fvEyeGame);

	//fVector3 fvEyeTerrain;
	//CRenderSystem::GetMe()->Axis_Trans(tGfxSystem::AX_PLAN, fvEyeGame, 
	//	tGfxSystem::AX_GAME, fvEyeTerrain);

//	_snprintf(szTemp, MAX_PATH, "%.1f[%s]", fvEyeGame.y-fvEyeTerrain.y, (bValid?"TRUE":"FALSE"));
//	CRenderSystem::GetMe()->SetDebugString("CamHeight", szTemp);


}

//===============================================================================

fVector3 CCamera_Scene::_GetEyePos(
							const fVector3& fvLookAt,
							FLOAT fDistance,
							FLOAT fDirection,
							FLOAT fPitch)
{
	FLOAT y		= fDistance * ::sin(-fPitch);
	FLOAT r_xy	= fDistance * ::cos(-fPitch);
	FLOAT x		= -r_xy * ::sin(fDirection);
	FLOAT z		= -r_xy * ::cos(fDirection);

	return fVector3 (	fvLookAt.x + x, 
						fvLookAt.y + y,
						fvLookAt.z + z);
}

FLOAT CCamera_Scene::_GetValidPitch(
							const WX::TerrainData* pTerrainData,	//��������
							const fVector3& fvLookAt,				//��ע��
							FLOAT fDistance,						//����
							FLOAT fDirection,						//��ת�Ƕ�
							FLOAT fPitchNow,						//Ŀǰ��Pitch(�����ӽ���ֵ)
							FLOAT fPitchApproch,					//Ҫ������Pitch
							FLOAT fCameraNearDis)					//�۾��Ľ��ü�����
{
	TDAssert(pTerrainData && !pTerrainData->isEmpty());
	
	//-----------------------------------------------
	//��ǰ�۾�λ��
	fVector3 fvCurrentEye = _GetEyePos(fvLookAt, fDistance, fDirection, fPitchNow);
	//�жϵ�ǰ�۾��Ƿ����
	BOOL bIsCurrentValid = _IsValidEye(pTerrainData, fvCurrentEye, fCameraNearDis);

	if(bIsCurrentValid)
	{
		//��ǰPitch�Ƿ��㹻�ӽ�����Pitch
		BOOL bNearEnough = (::abs(fPitchApproch-fPitchNow) <= MIN_ANGLE);

		//��ǰλ�ú���ֱ�ӷ���
		if(bNearEnough)
			return fPitchApproch;
		else
		{
			//������Pitch����һ��STEP
			FLOAT fPitch = fPitchNow+STEP_ANGLE;
			if(fPitch > fPitchApproch) fPitch = fPitchApproch;

			//�۾�
			fVector3 fvTempEye = _GetEyePos(fvLookAt, fDistance, fDirection, fPitch);
			//�жϵ�ǰ�۾��Ƿ����
			BOOL bIsTempValid = _IsValidEye(pTerrainData, fvTempEye, fCameraNearDis);
			if(bIsTempValid) return fPitch;
			else return fPitchNow;
		}

	}
	//�������������������λ��
	else
	{
		//����STEP�ۼӵķ���������Ϸ����۾�
		INT nNum = 0;
		FLOAT fPitch;
		for(fPitch = fPitchNow-STEP_ANGLE; fPitch > -(TDU_PI/2.0f-MIN_ANGLE); fPitch-=STEP_ANGLE)
		{
			//�۾�λ��
			fVector3 fvTempEye = _GetEyePos(fvLookAt, fDistance, fDirection, fPitch);
			//�жϵ�ǰ�۾��Ƿ����
			BOOL bIsTempValid = _IsValidEye(pTerrainData, fvTempEye, fCameraNearDis);

			nNum++;
			if(bIsTempValid) break;
		}
		return fPitch;
	}


	return 0;
}

BOOL CCamera_Scene::_IsValidEye(
							const WX::TerrainData* pTerrainData,	//��������
							const fVector3& fvEye,					//�۾���
							FLOAT fCameraNearDis)					//������
{
	//������ײ��
	Ogre::Sphere ballNear;	
	ballNear.setCenter(Ogre::Vector3(fvEye.x, fvEye.y, fvEye.z));
	ballNear.setRadius(fCameraNearDis);

	CAMERA_INTER_GRIDS gridsTerrain;
	if(!_GetInterPatch(
			pTerrainData, 
			fvEye, 
			gridsTerrain))
	{
		//�۾��ڵ�������
		return TRUE;
	}

	//�������Ƿ��ڵ�������
	for(INT i=0; i<(INT)gridsTerrain.m_fvGrid.size(); i++)
	{
		const Ogre::AxisAlignedBox& theGridBox = gridsTerrain.m_fvGrid[i];

		//�Ƿ��ཻ
		if(Ogre::Math::intersects(ballNear, theGridBox)) return FALSE;
	}

	return TRUE;
}

BOOL CCamera_Scene::_GetInterPatch(
							const WX::TerrainData* pTerrainData,		//��������
							const fVector3& fvEyePos,					//�۾���ͶӰλ��
							CAMERA_INTER_GRIDS& theInterPatch)			//��������
{
	theInterPatch.m_fvGrid.clear();

	//�Ƿ��ڵ�������
	if(!pTerrainData || !(pTerrainData->isValidCoord(fvEyePos.x, fvEyePos.z))) return FALSE;

	//ȡ�������Grid����
	std::pair<INT, INT> pJunction = pTerrainData->getJunction(fvEyePos.x, fvEyePos.z);

	//����ĸ���Grid
	theInterPatch.m_fvGrid.reserve(4);

	FLOAT fPatchX = pTerrainData->mScale.x*1.0f;
	FLOAT fPatchZ = pTerrainData->mScale.z*1.0f;

	//ȡ�ø����İ˸�������
	for(INT i=-1; i<=0; i++)
	{
		for(INT j=-1; j<=0; j++)
		{
			INT nGridX = pJunction.first  + i;
			INT nGridZ = pJunction.second + j;

			if(pTerrainData->isValidGrid(nGridX, nGridZ))
			{
				Ogre::AxisAlignedBox newGridBox;
				const WX::TerrainData::GridInfo& grid = pTerrainData->getGridInfo(nGridX, nGridZ);

				//ת��Ϊһ���󶨺�
				FLOAT fMaxY = -std::numeric_limits<FLOAT>::max();

				FLOAT fTemp;
				fTemp = pTerrainData->getHeight(nGridX, nGridZ);
				if(fTemp > fMaxY) fMaxY = fTemp;

				fTemp = pTerrainData->getHeight(nGridX+1, nGridZ);
				if(fTemp > fMaxY) fMaxY = fTemp;

				fTemp = pTerrainData->getHeight(nGridX, nGridZ+1);
				if(fTemp > fMaxY) fMaxY = fTemp;

				fTemp = pTerrainData->getHeight(nGridX+1, nGridZ+1);
				if(fTemp > fMaxY) fMaxY = fTemp;

				newGridBox.setExtents(	
					nGridX*(pTerrainData->mScale.x) + pTerrainData->mPosition.x,
					-std::numeric_limits<FLOAT>::max(), 
					nGridZ*(pTerrainData->mScale.z) + pTerrainData->mPosition.z,
					
					(nGridX+1)*(pTerrainData->mScale.x) + pTerrainData->mPosition.x, 
					fMaxY, 
					(nGridZ+1)*(pTerrainData->mScale.z) + pTerrainData->mPosition.z);
										
				theInterPatch.m_fvGrid.push_back(newGridBox);
			}
		}
	}

	if(theInterPatch.m_fvGrid.size() == 2 || theInterPatch.m_fvGrid.size() == 4) return TRUE;

	return FALSE;
}

//���ü���һ����
VOID CCamera_Scene::AddOneVib(FLOAT VibRange, FLOAT StepLen, UINT Times )
{
	if( VibRange <=0 || Times <= 0 || StepLen<=0)
		return;

	if(m_status == VIBRATION_STATUS)
	{
		//�Ժ��ٿ��Ƕ���𵴵��ں�
		return;
	}

	m_CurVibParam.fMaxBiasY	=	VibRange;
	m_CurVibParam.fMinBiasY	=	-VibRange;
	m_CurVibParam.nTimes	=	Times;
	m_CurVibParam.fBiasStep =	StepLen;

	_SetStatus(VIBRATION_STATUS);

}

//���õ�ǰ�����״̬
BOOL CCamera_Scene::_SetStatus(CAMERA_STATUS nStatus )
{
	switch(nStatus)
	{
	case NORMAL_STATUS:
		{
			switch(m_status)
			{
			case VIBRATION_STATUS:
				{
					//...
					m_CurVibParam.fMaxBiasY	=	0.0;
					m_CurVibParam.fMinBiasY	=	0.0;
					m_CurVibParam.nTimes	=	0;
					m_CurVibParam.fBiasStep =	0.0;

					fCurBias			= 0.0;
					fBiasStep			= 0.0;
					bCurDirIsUp			= TRUE;
					uAlreadyVibTimes	= 0;
				}
				break;
			default:
				TDAssert(0);
				return FALSE;
			}
			m_status = nStatus;
			return TRUE;
		}
		break;
	case VIBRATION_STATUS:
		{
			switch(m_status)
			{
			case NORMAL_STATUS:
				{
					//��ʼ��һ����
					fCurBias			= 0.0;
					fBiasStep			= m_CurVibParam.fBiasStep;
					bCurDirIsUp			= TRUE;
					uAlreadyVibTimes	= 0;
				}
				break;
			default:
				TDAssert(0);
				return FALSE;
			}
			m_status = nStatus;
			return TRUE;
		}
		break;
	default:
		TDAssert(0);
		return FALSE;
	}
	return TRUE;
}

//���ݵ�ǰ״̬ˢ�������
VOID CCamera_Scene::_UpdateCurStatus( fVector3& vEyePos, fVector3& vLookatPos)
{
	switch(m_status)
	{
	case NORMAL_STATUS:
		{
			return;
		}
		break;
	case VIBRATION_STATUS:
		{
			if( uAlreadyVibTimes == m_CurVibParam.nTimes)
			{
				_SetStatus(NORMAL_STATUS);
				return;
			}

			//���㱾��ƫ��
			fCurBias += fBiasStep;

			//���ݵ�ǰλ�ø��ķ���
			if( bCurDirIsUp == FALSE && fCurBias > 0)
			{
				uAlreadyVibTimes++;
				bCurDirIsUp	=	TRUE;
			}
			else if( bCurDirIsUp == TRUE && fCurBias < 0)
			{
				bCurDirIsUp	=	FALSE;
			}

			//���ݵ�ǰλ�ø����´β���
			if(fCurBias > m_CurVibParam.fMaxBiasY)
			{
				fCurBias	=	m_CurVibParam.fMaxBiasY;
				fBiasStep	=	-m_CurVibParam.fBiasStep;
			}
			else if(fCurBias < m_CurVibParam.fMinBiasY)
			{
				fCurBias	=	m_CurVibParam.fMinBiasY;
				fBiasStep	=	m_CurVibParam.fBiasStep;
			}

			//�����µ�λ��
			vLookatPos.y += fCurBias;
			vEyePos.y	 += fCurBias;	
			return;
		}
		break;
	default:
		TDAssert(0);
		return;
	}
	return;
}

