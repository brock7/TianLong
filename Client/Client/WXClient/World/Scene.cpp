#include "StdAfx.h"
#include <Float.h>
#include "WorldManager.h"
#include "Scene.h"
#include "TDException.h"
#include "TDDBC_Struct.h"
#include "TDDebuger.h"
#include "TLBB_DBC.h"
#include "TDUtil.h"
#include "../WxRender/ResourceProvider.h"
#include "..\Global.h"
#include "..\Procedure\GameProcedure.h"
#include "..\Object\ObjectManager.h"
#include "..\Object\Object.h"
#include "..\Object\Character\Obj_PlayerMySelf.h"
#include "..\Object\Obj_Building.h"
#include "..\Sound\GMSoundSystem.h"
#include "..\dbc\GMDataBase.h"
#include "..\Variable\GMVariable.h"
#include "..\WxRender\RenderSystem.h"

CScene::CScene(const _DBC_SCENE_DEFINE* pDefScene, bool bUserCity)
{
	TDAssert(pDefScene);
	m_pTheDefine = pDefScene;
	m_pBackSound = NULL;
	m_pEnvSoundList.resize(0);
	m_bUserCity = bUserCity;
	m_Weights = NULL;
	m_Dist = NULL;
}

CScene::~CScene()
{
	SAFE_DELETE_ARRAY( m_Weights );
	SAFE_DELETE_ARRAY( m_Dist );
}

VOID CScene::Initial(VOID)
{
	//------------
	//������������
	//------------
	const _DBC_SCENE_DEFINE* pSceneDefine = GetSceneDefine();

	//Ԥ�ȷ�����ڴ�
	m_nZoneXSize = pSceneDefine->nXSize / SIZE_OF_ZONE;
	if(m_nZoneXSize*SIZE_OF_ZONE < pSceneDefine->nXSize) m_nZoneXSize++;

	m_nZoneZSize = pSceneDefine->nZSize / SIZE_OF_ZONE;
	if(m_nZoneZSize*SIZE_OF_ZONE < pSceneDefine->nZSize) m_nZoneZSize++;

	m_theZoneBuf.resize(m_nZoneXSize * m_nZoneZSize);

	//������������
	for(INT i=0; i<(INT)m_theZoneBuf.size(); i++)
	{
		CZone& theZone = m_theZoneBuf[i];

		//�����ʼ��
		theZone.Initial();
	}

	//m_nPathXSize = (INT)((float)pSceneDefine->nXSize / SIZE_OF_PATH);
	//m_nPathZSize = (INT)((float)pSceneDefine->nZSize / SIZE_OF_PATH);
	//m_thePathBuf.

	//����Region��������
	LoadRegionBinary(m_pTheDefine->szRegionFile);
	m_WalkCollisionMng.ClearData();
	m_WalkCollisionMng.LoadCollisionTriInfoFromFile(m_pTheDefine->szCollisionfile);

	//���ػ�����Ч
	LoadEnvSound();

	//������ҽ���������
	if(m_bUserCity)
	{
		//���ұ����������п��ܻ���ֵ���ҽ�����
		DBC_DEFINEHANDLE(s_pCityBuilding, DBC_CITY_BUILDING);

		unsigned int nLineNum = s_pCityBuilding->GetRecordsNum();
		for(unsigned int i=0; i<nLineNum; i++)
		{
			const _DBC_CITY_BUILDING* pDefine = (const _DBC_CITY_BUILDING*)s_pCityBuilding->Search_LineNum_EQU((int)i);
			//�Ƿ������ڸó��еĽ�����
			if(pDefine->nCityID != m_pTheDefine->nServerID) continue;

			//����
			BUILDING newBuilding;
			newBuilding.pDefine = pDefine;
			newBuilding.nLevel = -1;
			newBuilding.nObjID = -1;
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
			m_allBuilding.insert(std::make_pair(pDefine->nID, newBuilding));
		}
	}
}

bool CScene::UpdateUserCity(int nNewLevel) //const _DBC_SCENE_DEFINE* pDefScene)
{
	//������ҳ���
	if(!m_bUserCity) return false;

	//���ҳ��г���
	DBC_DEFINEHANDLE(s_pSceneDBC, DBC_SCENE_DEFINE);
	const _DBC_SCENE_DEFINE* pSceneDef = 0;
	for(int i=0; i<(int)s_pSceneDBC->GetRecordsNum(); i++)
	{
		const _DBC_SCENE_DEFINE* pTempSceneDef = (const _DBC_SCENE_DEFINE*)s_pSceneDBC->Search_LineNum_EQU(i);

		//Id�͵ȼ���ͬ
		if(pTempSceneDef->nServerID==m_pTheDefine->nServerID && pTempSceneDef->nCityLevel==nNewLevel)
		{
			pSceneDef = pTempSceneDef;
			break;
		}
	}
	if(!pSceneDef) return false;

	//ͬһ������
	if(m_pTheDefine->nLocalID == pSceneDef->nLocalID) return false;

	//��С������ͬ
	if(	pSceneDef->nXSize != m_pTheDefine->nXSize ||
		pSceneDef->nZSize != m_pTheDefine->nZSize) return false;

	//������������
	for(INT i=0; i<(INT)m_theZoneBuf.size(); i++)
	{
		CZone& theZone = m_theZoneBuf[i];
		//�����ʼ��
		theZone.Initial();
	}

	//�Ƿ���ͬһ���赲��
	if(_stricmp(m_pTheDefine->szRegionFile, pSceneDef->szRegionFile))
	{
		LoadRegionBinary(m_pTheDefine->szRegionFile);
	}

	//�Ƿ���ͬһ��������
	if(_stricmp(m_pTheDefine->szCollisionfile, pSceneDef->szCollisionfile))
	{
		m_WalkCollisionMng.ClearData();
		m_WalkCollisionMng.LoadCollisionTriInfoFromFile(m_pTheDefine->szCollisionfile);
	}

	//�Ƿ���ͬһ����������
	if(m_pTheDefine->nBackSound != pSceneDef->nBackSound)
	{
		//���ٱ���������Դ
		ControlBkgSound(FALSE);
	}

	//���س�����Դ
	m_pTheDefine = pSceneDef;
	EnterScene();

	//������Ч
	LoadEnvSound();

	return true;
}

bool CScene::UpdateCityBuilding(int nBuildingID, int nBuildingLevel)
{
	//������ҳ���
	if(!m_bUserCity) return false;

	//�ý������Ƿ����
	BUILDING_REGISTER::iterator itFind = m_allBuilding.find(nBuildingID);
	if(itFind == m_allBuilding.end()) return false;

	BUILDING& theBuilding = itFind->second;

	//�ȼ���ͬ
	if(theBuilding.nLevel == nBuildingLevel) return true;

	//�����δ����
	CObject_Building* pBuilding = (CObject_Building*)CObjectManager::GetMe()->FindObject(theBuilding.nObjID);
	if(!pBuilding)
	{
		pBuilding = (CObject_Building*)CObjectManager::GetMe()->NewObject("CObject_Building");
		if(!pBuilding) return false;
		
		//������Դ
		pBuilding->SetLogicData_Define(theBuilding.pDefine);

		//����
		theBuilding.nObjID = pBuilding->GetID();
	}

	//���õȼ�
	pBuilding->SetLogicData_Level(nBuildingLevel);

	return true;
}

VOID CScene::EnterScene(VOID)
{
	//----------------
	//����Ⱦ�㴴������
	//----------------

	//���س����ļ�
	CGameProcedure::s_pGfxSystem->Scene_Load(m_pTheDefine->nLocalID, m_pTheDefine->szWXObjectName);

	//Ԥ��������Npc��Դ
	char szPreLoadFile[MAX_PATH];
	strncpy(szPreLoadFile, m_pTheDefine->szWXObjectName, MAX_PATH);
	char* pSzExt = ::PathFindExtension(szPreLoadFile);
	if(pSzExt)
	{
		*pSzExt = 0;
		//***_monster.preload
		strncat(szPreLoadFile, "_monster.preload", MAX_PATH);

		CGameProcedure::s_pGfxSystem->Scene_PreLoad(szPreLoadFile);
	}

	//----------------
	//���ű�������  , ʹ��ϵͳ����ı����������Ƿ񲥷�
	//----------------	
	STRING strVar;
	CVariableSystem::GetMe()->GetVariable(CSoundSystemFMod::szBKSoundVar, strVar);
	if(strVar == "1")
	{
		ControlBkgSound(TRUE);
	}
}

VOID CScene::LeaveScene(VOID)
{
	//���ٱ���������Դ
	ControlBkgSound(FALSE);
	//���ٻ�����Ч
	DestroyEnvSound();

	//-------------------
	// ���������ϵ�����
	for(INT i=0; i<(INT)m_theZoneBuf.size(); i++)
	{
		CZone& theZone = m_theZoneBuf[i];

		register std::set< INT >::iterator it = theZone.m_setRegisterObject.begin();
		while(it != theZone.m_setRegisterObject.end())
		{
			//��������
			CObject* pObj = (CObject*)CObjectManager::GetMe()->FindObject(*it);
			if(pObj && !(pObj->GetFakeObjectFlag()))
			{
				pObj->Release();
				CObjectManager::GetMe()->DestroyObject( pObj );
			}
			it++;
		}
	}
	m_theZoneBuf.clear();
}

void CScene::DestroyEnvSound(void)
{
	for(int i=0; i<(int)m_pEnvSoundList.size(); i++)
	{
		EnvSound_t& snd = m_pEnvSoundList[i];

		if(snd.pSoundSource)
		{
			CSoundSystemFMod::GetMe()->Source_Destroy(snd.pSoundSource);
			snd.pSoundSource = 0;
		}
	}
	m_pEnvSoundList.clear();
}

//����λ���Ƿ�Ϸ�
BOOL CScene::IsValidPosition(const fVector2& fvPos)
{
	if(fvPos.x < 0.0f || fvPos.x > (FLOAT)(m_pTheDefine->nXSize)) return FALSE;
	if(fvPos.y < 0.0f || fvPos.y > (FLOAT)(m_pTheDefine->nZSize)) return FALSE;
	return true;
}

VOID CScene::Tick(VOID)
{
	//�Ƿ������Ż�����
	if(CVariableSystem::GetMe()->GetAs_Int(CSoundSystemFMod::sz3DSoundVar) == 1)
	{
		if(!CObjectManager::GetMe()->GetMySelf()) return;

		//������Ч�ľ������
		fVector2 vMyPos(CObjectManager::GetMe()->GetMySelf()->GetPosition().x, CObjectManager::GetMe()->GetMySelf()->GetPosition().z);
		FLOAT fDistance = 0.0;
		for(UINT i = 0; i<m_pEnvSoundList.size(); i++)
		{
			//�������
			fVector2 vSoundPos((FLOAT)m_pEnvSoundList[i].nPosx, (FLOAT)m_pEnvSoundList[i].nPosz);
			fDistance = TDU_GetDist(vMyPos,vSoundPos);

			if(fDistance<=m_pEnvSoundList[i].nDistance)
			{//�����Ч
				if(m_pEnvSoundList[i].pSoundSource == NULL)
				{
					m_pEnvSoundList[i].pSoundSource =  CSoundSystemFMod::GetMe()->Source_Create(CSoundSourceFMod::type_circumstance, TRUE, TRUE);
					if(m_pEnvSoundList[i].pSoundSource == 0) continue; 

					tSoundBuffer* pSndBuffer = CSoundSystemFMod::GetMe()->Buffer_Create(m_pEnvSoundList[i].nSoundID); 
					if(pSndBuffer)
					{
						m_pEnvSoundList[i].pSoundSource->SetBuffer(pSndBuffer);
						m_pEnvSoundList[i].pSoundSource->SetPos(fVector3(vSoundPos.x, 0, vSoundPos.y));
						m_pEnvSoundList[i].pSoundSource->SetLooping(TRUE);
						m_pEnvSoundList[i].pSoundSource->Play();
					}
				}
			}
			else if(m_pEnvSoundList[i].pSoundSource != NULL)
			{//ɾ����Ч
				CSoundSystemFMod::GetMe()->Source_Destroy(m_pEnvSoundList[i].pSoundSource);
				m_pEnvSoundList[i].pSoundSource	= NULL;
			}
		}
	}
}

//���Ʊ�������
VOID CScene::ControlBkgSound(BOOL bEnable)
{
	if(bEnable && !m_pBackSound)
	{
		//���ɱ�������
		m_pBackSound = CSoundSystemFMod::GetMe()->Source_Create(CSoundSourceFMod::type_background, FALSE, FALSE);

		tSoundBuffer* pSndBuffer = CSoundSystemFMod::GetMe()->Buffer_Create(m_pTheDefine->nBackSound);
		if(pSndBuffer)
		{
			m_pBackSound->SetBuffer(pSndBuffer);
			m_pBackSound->SetLooping(TRUE);
			m_pBackSound->Play();
		}
	}
	else if(!bEnable && m_pBackSound)
	{
		CSoundSystemFMod::GetMe()->Source_Destroy(m_pBackSound);
		m_pBackSound = 0;
	}
}

//���Ʊ�����������
VOID CScene::ControlBkgSoundVol()
{
	if(m_pBackSound)
	{
		m_pBackSound->UpdateVolume();
	}
}

//���ƻ���������
VOID CScene::ControlEnvSoundVol()
{
	for(UINT i=0; i<m_pEnvSoundList.size(); i++)
	{
		if(m_pEnvSoundList[i].pSoundSource)
		{
			m_pEnvSoundList[i].pSoundSource->UpdateVolume();
		}
	}
}

//���ƻ�����
VOID CScene::ControlEnvSound(BOOL bEnable)
{
	if(bEnable)
	{
		LoadEnvSound();
	}
	else
	{
		for(UINT i=0; i<m_pEnvSoundList.size(); i++)
		{
			if(m_pEnvSoundList[i].pSoundSource)
			{
				//ɾ����Ч
				CSoundSystemFMod::GetMe()->Source_Destroy(m_pEnvSoundList[i].pSoundSource);
				m_pEnvSoundList[i].pSoundSource	= 0;
			}
		}
	}
}

VOID CScene::LoadRegionBinary(LPCTSTR szRegionFile)
{
	if(!szRegionFile || szRegionFile[0] == '\0') return;
	m_theRegion.clear();

	char* lpAddress = NULL;
	DWORD dwSize = CGameProcedure::s_pResourceProvider->loadResource(szRegionFile, lpAddress, "General");
	if(dwSize > 0)
	{
		INT			iRegionCount = 0;
		INT			iPointCount  = 0;
		INT         iId			 = 0;
		INT			FlyLevel	 = 0;
		bool        NewVer       = false;
		FLOAT       fTemp = 0;
		fVector2	fvp;
		char*		pPoint = lpAddress;
		//�жϰ汾
		int			iVerLen = (int)sizeof("REGION_EDIT_01");
		char*		pVer = new char[iVerLen];
		memcpy(pVer, pPoint, iVerLen-1);
		pVer[iVerLen-1] = 0;

		if( strcmp( pVer, "REGION_EDIT_01" ) == 0 )
		{
			pPoint += (iVerLen-1);
			NewVer = true;
		}

		delete [] pVer;

		// ��ȡ����ĸ���
		memcpy(&iRegionCount, pPoint, sizeof(iRegionCount));
		pPoint += sizeof(iRegionCount);

		m_theRegion.reserve( iRegionCount );
		for(INT i = 0; i < iRegionCount; i++)
		{
			//��ȡ�����id
			//memcpy(&(iId), pPoint, sizeof(iId));
			pPoint += sizeof(INT); //skip regionid
			CRegion cr;					
			cr.m_RegionID = i;

			if (NewVer)
			{
				memcpy(&FlyLevel, pPoint, sizeof(FlyLevel));
				pPoint += sizeof(FlyLevel);
				cr.m_nPassRegionLevel = FlyLevel;
			}
			
			// ��ȡ��ĸ���.
			memcpy(&iPointCount, pPoint, sizeof(iPointCount));
			pPoint += sizeof(iPointCount);

			for(INT k = 0; k < iPointCount; k++)
			{
				// ��ȡx�� z ����
				memcpy(&fvp.x, pPoint, sizeof(fvp.x)); pPoint += sizeof(fvp.x);
				memcpy(&fvp.y, pPoint, sizeof(fvp.y)); pPoint += sizeof(fvp.y);
				memcpy(&fTemp, pPoint, sizeof(fTemp)); pPoint += sizeof(fTemp);

				cr.AddPoint(&fvp);
			}
			m_theRegion.push_back(cr);
		}
		CGameProcedure::s_pResourceProvider->unloadResource( lpAddress, dwSize );
	}

	//��Regionע�ᵽZone
	RegisteAllRegion();

	LoadDijk();
}

VOID CScene::RegisteAllRegion(VOID)
{	
	fVector2 sp,ep;
	INT sGridX=0,sGridZ=0,eGridX=0,eGridZ=0;	
	FLOAT div=0,dividend=0;
	INT tempint0;
	FLOAT fb,fk;
	//���ж����	
	std::vector<CRegion>::iterator itCRegion;
	for(itCRegion = m_theRegion.begin();itCRegion!=m_theRegion.end();itCRegion++)
	{	
		CRegion pCRegion = *itCRegion;
		//���б�
		for(INT i =0;i<(INT)(pCRegion.m_vfPoints.size());i++)
		{
			//�����յ�
			sp = pCRegion.m_vfPoints[i];			
			ep = pCRegion.m_vfPoints[(i+1)%(INT)(pCRegion.m_vfPoints.size())];
			sGridX=GetZoneX(sp.x);
			eGridX=GetZoneX(ep.x);
			sGridZ=GetZoneZ(sp.y);
			eGridZ=GetZoneZ(ep.y);
			if(sGridZ==eGridZ)
			{
				//Zoneˮƽ�����,��ͬһZone�ڱ�Ȼ��ͬһˮƽZONE
				for(tempint0 = min(sGridX,eGridX);tempint0<=max(sGridX,eGridX);tempint0++)
				{						
					CZone * pcz;
					pcz = GetZone(tempint0,sGridZ);
					if(pcz!=NULL)
					{
						pcz->AddRegion(pCRegion.m_RegionID);						
					}						
				}				
			}
			else if(sGridX==eGridX)
			{
				//Zone��ֱ�����			
				for(tempint0 = min(sGridZ,eGridZ);tempint0<=max(sGridZ,eGridZ);tempint0++)
				{
					CZone * pcz;
					pcz = GetZone(sGridX,tempint0);
					if(pcz!=NULL)
					{
						pcz->AddRegion(pCRegion.m_RegionID);
					}						
				}
			}
			else
			{		
				//б�ߵ����,�������ڵ������и����α�
				//����б�ʺ�λ��					
				fk = (sp.y-ep.y)/(sp.x-ep.x);
				fb = ep.y - ep.x*fk;					
				for (tempint0=min(sGridZ,eGridZ)+1;tempint0<=max(sGridZ,eGridZ);tempint0++)
				{	
					//�������α�ͬ��ֱ���ߵĽ���,���е����µ�����zoneע��
					FLOAT cross= (tempint0*SIZE_OF_ZONE-fb)/fk;					

					CZone * pcz;
					pcz = GetZone(GetZoneX(cross),tempint0);
					if(pcz!=NULL)
					{
						pcz->AddRegion(pCRegion.m_RegionID);
					}
					pcz = GetZone(GetZoneX(cross),tempint0-1);
					if(pcz!=NULL)
					{
						pcz->AddRegion(pCRegion.m_RegionID);
					}

				}					
				for (tempint0=min(sGridX,eGridX)+1;tempint0<=max(sGridX,eGridX);tempint0++)
				{	
					//�������α�ͬ��ֱ���ߵĽ���,���е���������zoneע��
					FLOAT cross = ((tempint0)*SIZE_OF_ZONE)*fk+fb;					
					CZone * pcz;
					pcz = GetZone(tempint0,GetZoneZ(cross));
					if(pcz!=NULL)
					{
						pcz->AddRegion(pCRegion.m_RegionID);

					}
					pcz = GetZone(tempint0-1,GetZoneZ(cross));
					if(pcz!=NULL)
					{
						pcz->AddRegion(pCRegion.m_RegionID);
					}						
				}
			}				
		}
	}	
}

CZone* CScene::GetZone(INT nXPos, INT nZPos)
{
	//����λ�÷Ƿ�
	if(	nXPos<0 || nXPos >= m_nZoneXSize ||
		nZPos<0 || nZPos >= m_nZoneZSize )
	{
		//Warning.... �����嶨���λ�ò��ڵ�ͼ֮��
		return NULL;
	}

	return &(m_theZoneBuf[ nZPos*m_nZoneXSize + nXPos ]);
}


//������Ч
BOOL CScene::LoadEnvSound()
{
	//ֻ��һ��
	if(m_pEnvSoundList.size() != 0)
	{
		//���ٻ�����Ч
		DestroyEnvSound();
	}

	//��������
	STRING	strFileName = m_pTheDefine->szRegionFile;
	STRING::size_type nValidEnd = strFileName.find(".region", 0);
	if(nValidEnd == STRING::npos) 
	{
		return FALSE;
	}
	STRING::size_type nFinalEnd	= strFileName.size();
	strFileName.replace(nValidEnd, nFinalEnd, ".esd");

	//������Ч�б�
	DBC::DBCFile* m_pEnvSoundDefine = new DBC::DBCFile(DBC_ENV_SOUND);
	if(!(CDataBase::Util_OpenDBCFromTxt(strFileName.c_str(), m_pEnvSoundDefine)))
	{
		//�˳���û�л�����Ч
		delete m_pEnvSoundDefine;
		return TRUE;
	}

	//����������Ч�б�
 	m_pEnvSoundList.resize((INT)m_pEnvSoundDefine->GetRecordsNum());
	for(INT i=0; i<(INT)m_pEnvSoundDefine->GetRecordsNum(); i++)
	{
		_DBC_ENV_SOUND* pLine			= (_DBC_ENV_SOUND*)(m_pEnvSoundDefine)->Search_Posistion(i,0);
		m_pEnvSoundList[i].nID			= pLine->nID;
		m_pEnvSoundList[i].nSoundID		= pLine->nSoundID;	
		m_pEnvSoundList[i].nPosx		= pLine->nXPos;
		m_pEnvSoundList[i].nPosz		= pLine->nZPos;
		m_pEnvSoundList[i].nDistance	= pLine->nDistance;
		m_pEnvSoundList[i].pSoundSource	= 0;
	}

	delete m_pEnvSoundDefine;

	return TRUE;

}

BOOL CScene::GetMapHeight(const fVector2& fvMapPos, FLOAT& fHeight)
{
	//��������Ƿ�Ƿ�
	if(fvMapPos.x < 0 || fvMapPos.x > GetSceneDefine()->nXSize) return FALSE;
	if(fvMapPos.y < 0 || fvMapPos.y > GetSceneDefine()->nZSize) return FALSE;

	fVector3 fvTerrain;
	if(!CGameProcedure::s_pGfxSystem->Axis_Trans(CRenderSystem::AX_PLAN, 
			fVector3(fvMapPos.x, 0.0f, fvMapPos.y), CRenderSystem::AX_GAME, fvTerrain)) return FALSE;

	FLOAT fBuildHeight;
	BOOL bOnBuild = m_WalkCollisionMng.GetHeight(fvMapPos.x, fvMapPos.y, fBuildHeight);
	if(bOnBuild && fBuildHeight>=fvTerrain.y)
	{
		fHeight = fBuildHeight;
	}
	else
	{
		fHeight = fvTerrain.y;
	}
	return TRUE;
}

VOID CScene::LoadDijk()
{
	//return;

	//��������
	STRING	strFileName = m_pTheDefine->szRegionFile;
	STRING::size_type nValidEnd = strFileName.find(".region", 0);
	if(nValidEnd == STRING::npos) 
	{
		return ;
	}
	STRING::size_type nFinalEnd	= strFileName.size();
	strFileName.replace(nValidEnd, nFinalEnd, ".Dijkstra");
	STRING	leftFileName ="..\\..\\Public\\Scene\\";
	strFileName = leftFileName + strFileName;
	FILE* pFile = fopen( strFileName.c_str(), "rb" );

	if(NULL == pFile)
	{
		// ���ܴ��ļ�.
		return;
	}
	//�İ汾��Ϣ
	int			iVerLen = (int)sizeof("DIJKSTRA_EDIT_01");
	char*		pVer = new char[iVerLen];
	::fread( pVer, 1, iVerLen, pFile );
	delete [] pVer;
	INT dwPoint =0;
	fVector3   position;
	::fread(&dwPoint, sizeof(dwPoint), 1, pFile);
	fVector2 v;
	//1. �������еĵ�
	m_Points.clear();
	for( INT dw = 0; dw < dwPoint; dw ++ )
	{
		::fread(&position.x, sizeof(position.x), 1, pFile);
		::fread(&position.z, sizeof(position.z), 1, pFile);
		::fread(&position.y, sizeof(position.y), 1, pFile);
		v.x = position.x;
		v.y = position.z;
		m_Points.push_back( v );
	}
	if( m_Points.size() == 0 ) return;

	//2. ������
	SAFE_DELETE_ARRAY( m_Weights );
	m_Weights = new FLOAT[(m_Points.size()+2)*(m_Points.size()+2)];
	//memset( m_Weights, 0, (m_Points.size()+2)*(m_Points.size()+2)*sizeof(FLOAT) );
	float* JuLi = new float[dwPoint*dwPoint];
	::fread(JuLi, sizeof(float),dwPoint*dwPoint, pFile);
	//3. ����·��Ȩ��
	size_t s = 0;
	for( s = 0; s < m_Points.size()-1; s ++ )
	{
		for( size_t t = s+1; t < m_Points.size(); t ++ )
		{
			//�������
			m_Weights[(s+1) + (t+1)*(m_Points.size()+2)] = JuLi[s + t * dwPoint];
			m_Weights[(t+1) + (s+1)*(m_Points.size()+2)] = JuLi[s + t * dwPoint];
		}
		m_Weights[(s+1) + (s+1)*(m_Points.size()+2)] = 0.0f; //�Լ����Լ�Ȩ���� 0
	}
	delete [] JuLi;
	for (int i=0;i<dwPoint+2;++i)
	{
		for (int j=0;j<dwPoint+2;++j)
		{
			if (i == 0 || i == dwPoint+1 || j == 0 || j == dwPoint+1)
			{
				m_Weights[j + i*(dwPoint+2)] = FLT_MAX;
			}
		}
	}
	m_Weights[(s+1) + (s+1)*(m_Points.size()+2)] = 0.0f;
	m_Weights[(s+2) + (s+2)*(m_Points.size()+2)] = 0.0f; //�Լ����Լ�Ȩ���� 0
	m_Weights[0] = 0.0f;

	//4. ��ʼ�����·��
	SAFE_DELETE_ARRAY( m_Dist );
	m_Dist = new FLOAT[m_Points.size()+2]; //���·�����洦
}








