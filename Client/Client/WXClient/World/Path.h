#ifndef __PATH_H__
#define __PATH_H__

#pragma once

#include <vector>
#include <map>
#include "TDMath.h"


class CScene;
class CRegion;
class CPath
{
public:
	struct PathUnit
	{
		fVector2	fvStart;		//��ʼ��
		fVector2	fvTarget;		//Ŀ���
		INT			dwRequestIndex;	//������������·����ţ����ںͷ�����֮�佻���ı�ʾ
	};

	
protected:
	// �ݹ��������·����
    BOOL FindPath(
					fVector2 fvStart,			 // ��ǰ���ҵĿ�ʼ��
					BOOL bBegin         = FALSE, // �Ƿ�ʼ����
					INT iPreRegionIndex = -1,	 // ǰһ�ε��õ�region����
					INT iPreLineIndex   = -1,	 // ǰһ�ε��õ��赲������
					INT iPreVertexIndex = -1	 // ǰһ���ҵ��ĵ㡣
					);

	// �Ƿ񵽴��յ㡣
	BOOL IsReachDes(fVector2 fvStart,int level);

	
	// ������������õ�����ĵ�
	BOOL GetMinCrossPos(
						fVector2 fvStart,		// ֱ�ߵĿ�ʼ�ĵ㡣
						fVector2 fvEnd,			// ֱ�ߵĽ����ĵ㡣
						FLOAT& fFindx,			// �ҵ����ཻ�ĵ�x 
						FLOAT& fFindy,			// �ҵ����ཻ��y
						INT& iRegionIndex,      // �ҵ�����������
						INT& iLineIndex,		// �ҵ��ĵ��ߵ�����
						INT iExcludeRegion = -1 // �����ҵ�����
						);

	// ��������ֱ�ߵĽ��㣬 ���󽻵㡣
	BOOL CheckLineCross(
						fVector2		fv1Start,		// ֱ��1�Ŀ�ʼ�㡣
						fVector2		fv1End,			// ֱ��1�Ľ����㡣
						fVector2		fv2Start,		// ֱ��2�Ŀ�ʼ�㡣
						fVector2		fv2End,			// ֱ��2�Ľ����㡣
						FLOAT&			fcrossx,			// �ҵ��Ľ���x��
						FLOAT&			fcrossy			// �ҵ��Ľ���y
						);

	INT	 BuildDijkPath(const fVector2& fvCurrent, const fVector2& fvTarget, int nPassLevel);

	std::vector<PathUnit>		m_vPosStack;		//�ؼ���ļ��ϣ�ÿ������֮����һ��·����λ
	fVector2					m_TargetPos;		// ��·���յ�
	std::vector<INT>			m_PassVector;		// �����յ�����������������
	INT							m_iScene;
	std::vector<PathUnit>		m_vPosStack1;		//�ؼ���ļ��ϣ�ÿ������֮����һ��·����λ

	// ����һ��region�ߵ���һ��regionʱ�� ��������������¼����ǰһ��region���赲�ߵ�������
	INT							m_iChangeRegion;
	INT							m_iChangeLine;


public:
	CPath();
	virtual ~CPath();

	static INT			RequestIndexFactory(VOID);
	static BOOL			CheckCross(fVector2 fvmStart, fVector2 fvmEnd,const CRegion* pcr);
	static BOOL			CheckCross(fVector2 fvmStart, fVector2 fvmEnd,fVector2 fvnStart, fVector2 fvnEnd);
	static fVector2		fvSub(fVector2 fv1, fVector2 fv2);
	static FLOAT		fvMulti(fVector2 fv1, fVector2 fv2);
	static BOOL			fvEuqual(const fVector2& fv1, const fVector2& fv2);
	static BOOL			IsPointInUnreachRegion(const fVector2& fvTarget); // ��ǰ���Ƿ��ڲ�����������֮��

	// ����ӿ�ʼ�㵽�������·����
	BOOL				CreateMovePath(const fVector2& fvCurrent, const fVector2& fvTarget);

	//  ֱ�Ӳ���Ŀ�ĵء�
	BOOL                GetPathDirect(const fVector2& fvCurrent, const fVector2& fvTarget);

	// ���ҵ�ǰ��ֱ����һ�������Ƿ��ཻ������ཻ���ؽ��������������ֱཻ�ߵ�����ֵ
	BOOL	CheckCross(
		fVector2 fvStart,		// ֱ�ߵĿ�ʼ�㡣
		fVector2 fvEnd,			// ֱ�ߵĽ����㡣
		const CRegion* pcr,		// Ҫ���ҵ�ֱ�ߡ�
		FLOAT& fFindx,			// �ҵ��Ľ���x��
		FLOAT& fFindY,			// �ҵ��Ľ���y
		FLOAT& fFindDis,		// �ҵ���ֱ�ߵľ���
		INT& iFindLineIndex		// �ҵ���ֱ�ߵ�����
		);

	//ȡ��·���ε���Ŀ
	INT					GetUnitNum(VOID)const { return (INT)m_vPosStack.size(); }

	const PathUnit&		GetConstPathUnit(INT nIndex)const;


};

#endif 

