/*************************************************************************
�ļ���	: 	AIScript.h
�汾�� :	0.0.1
��  ��	:	����Ѳ�ߵ���ش���
*************************************************************************/

#ifndef _PATROL_H_
#define _PATROL_H_

#include "Type.h"
#include "GameStruct.h"

struct PatrolPath
{
	struct PatrolUnit
	{
		INT m_nSettleTime;				// �õ��ͣ��ʱ��
		ScriptID_t m_ScriptID;			// �õ���¼��ű�
		WORLD_POS m_PatrolPoint;		// �õ��ʵ������
		PatrolUnit()
		{
			m_nSettleTime = 0;
			m_ScriptID = -1;
		}
		VOID Cleanup(VOID)
		{
			m_nSettleTime = 0;
			m_ScriptID = -1;
		}
	};
		
	INT m_nPatrolPointCount;
	PatrolUnit* m_PatrolUnitList;

	PatrolPath()
	{
		m_nPatrolPointCount = 0;
		m_PatrolUnitList = NULL;
	}
	~PatrolPath()
	{
		CleanUp();
	}
	VOID CleanUp( VOID )
	{
		m_nPatrolPointCount = 0;
		SAFE_DELETE(m_PatrolUnitList);
	}
};

class Scene;
class PatrolPathMgr
{
public:

	PatrolPathMgr()
	{
		m_nPatrolPathCount = 0;
		m_PatrolPathList = NULL;
	}

	~PatrolPathMgr()
	{
		CleanUp();
	}

	VOID		Init( Scene* pScene );
	VOID		CleanUp( VOID );
	VOID		LoadPatrolPoint(const CHAR* filename);
	// ���ݴ����patrolPointIndexֵ�õ���һ��Ҫ�����patrolPointIndex
	INT			GetPatrolPoint(INT patrolPathIndex, INT patrolPointIndex, 
		BOOL& baHead, WORLD_POS& rTar, INT& rSettleTime, ScriptID_t& rScriptID);
	BOOL		FindPatrolID(INT PatrolID) const;

private:

	INT			m_nPatrolPathCount;
	PatrolPath* m_PatrolPathList;
	Scene*		m_pScene;


};

#endif