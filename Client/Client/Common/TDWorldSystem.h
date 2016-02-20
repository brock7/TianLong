#pragma once
#include "TDNode.h"
#include "TDGfxSystem.h"
#include "TDSound.h"
#include "TDObjectSystem.h"
//======================================================================
//��Ϸ�����ӿ�
struct _DBC_SCENE_DEFINE;
struct MAP_POS_DEFINE
{
	fVector3	pos;
	DWORD		dwSceneID; // ���ڳ���id
	char		name[ 128 ];
	INT			nServerID;
};

//�����ṩ�ĵ��θ߶Ȳ�ѯ�ӿ�(ʹ����Ⱦϵͳ���꣬���ǵ��κ�������)
typedef bool (__stdcall* FUNC_GETTERRAINHEIGHT)(float x, float z, float& y);

class tScene
{
public:
	//ȡ�ó�������
	virtual const _DBC_SCENE_DEFINE*		GetSceneDefine(VOID) const  = 0;
	//����λ���Ƿ�Ϸ�
	virtual BOOL							IsValidPosition(const fVector2& fvPos) = 0;
	//�����߼�����ȡ�õ��θ߶ȣ����������棬�����������Ƿ�������FALSE
	virtual BOOL							GetMapHeight(const fVector2& fvMapPos, FLOAT& fHeight) = 0;
};

//======================================================================
//��Ϸ����ӿ�
//
class tWorldSystem : public tNode
{
public:
	enum WORLD_STATION
	{
		WS_NOT_ENTER,				//û�н����κγ���
		WS_ENTER_ING,				//���ڽ���һ������(�Ѿ�������CGEnterScene)
		WS_ENTER,					//�ɹ�����һ������
		WS_RELIVE,					//��������
		WS_ASK_CHANGE_SCANE,		//Ҫ���л���һ���µĳ���(�Ѿ�������CGAskChangeScene)
	};	
public:
	/**
		����ĳ����
		\param nSceneID	
			����ID/��ҳ���id

		\param nCityLevel
			��ҳ��еȼ������С��0����ʾ����ͨ����!
	*/
	virtual BOOL							EnterScene(INT nSceneID, INT nCityLevel)= 0;
	///ȡ�õ�ǰ����ĳ���
	virtual tScene*							GetActiveScene(void)		= 0;
	///ȡ�õ�ǰ״̬
	virtual WORLD_STATION					GetStation(void) const		= 0;
	//�����ṩ�ĵ��θ߶Ȳ�ѯ�ӿ�(ʹ����Ⱦϵͳ���꣬���ǵ��κ�������)
	virtual FUNC_GETTERRAINHEIGHT			GetTerrainHeightFunc(void)	= 0;
	//��ǰ������ServerID
	virtual INT								GetActiveSceneID(void) const = 0;


	virtual std::list< MAP_POS_DEFINE >* GetObjectListByClass( INT nType ) = 0;
	virtual void setMinimapSerachRange( int nRange ) = 0;
	virtual void UpdateMinimapData() = 0;
	virtual void MinimapNeedFresh() = 0;


	virtual fVector3 GetMyselfPos() = 0;
	virtual int GetMyselfDir() = 0;
	virtual const CHAR* GetSceneName( INT nSceneID ) = 0;
	WX_DECLARE_DYNAMIC(tWorldSystem);
};