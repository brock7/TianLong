// ObjectCommand.h
//
//////////////////////////////////////////////////////

#pragma once

#include "Type.h"

enum ENUM_OBJECT_COMMAND
{
	OBJECT_COMMAND_INVALID	= -1,
	OBJECT_COMMAND_ACTION,				// ����
	OBJECT_COMMAND_STOP_ACTION,			// ֹͣ����
	OBJECT_COMMAND_MOVE,				// �ƶ�
	OBJECT_COMMAND_STOP_MOVE,			// ��ֹ�ƶ�
	OBJECT_COMMAND_MAGIC_SEND,			// ��������
	OBJECT_COMMAND_MAGIC_CHARGE,		// ��������
	OBJECT_COMMAND_MAGIC_CHANNEL,		// ��������
	OBJECT_COMMAND_ABILITY,				// �����

	OBJECT_COMMAND_NUMBERS
};

/////////////////////////////////////////////
// CObjectCommand
/////////////////////////////////////////////
class CObjectCommand
{
public:
	CObjectCommand(VOID);
	virtual ~CObjectCommand(VOID);

	virtual VOID CleanUp(VOID)
	{
		m_nCommandID = OBJECT_COMMAND_INVALID;
	}

	INT GetCommandID(VOID)const
	{
		return m_nCommandID;
	}

protected:
	VOID SetCommandID(INT nCommandID)
	{
		m_nCommandID = nCommandID;
	}

private:
	INT		m_nCommandID;		// ENUM_OBJECT_COMMAND
};

/////////////////////////////////////////////
// CObjectCommand_Logic �߼�ָ��
/////////////////////////////////////////////
class CObjectCommand_StopLogic;
class CObjectCommand_Logic :
	public CObjectCommand
{
public:
	CObjectCommand_Logic(VOID);
	virtual ~CObjectCommand_Logic(VOID);

	virtual VOID CleanUp(VOID);

	virtual BOOL Modify(const CObjectCommand_StopLogic *pCmd);

	UINT GetStartTime(VOID)const
	{
		return m_uStartTime;
	}

	INT GetLogicCount(VOID)const
	{
		return m_nLogicCount;
	}

protected:
	VOID SetStartTime(UINT uTime)
	{
		m_uStartTime = uTime;
	}

	VOID SetLogicCount(INT nLogicCount)
	{
		m_nLogicCount = nLogicCount;
	}

private:
	UINT		m_uStartTime;		// ��ʼʱ��
	INT			m_nLogicCount;		// �߼�����
};

/////////////////////////////////////////////
// CObjectCommand_StopLogic �ж��߼�ָ��
/////////////////////////////////////////////
class CObjectCommand_StopLogic :
	public CObjectCommand
{
public:
	CObjectCommand_StopLogic(VOID);
	virtual ~CObjectCommand_StopLogic(VOID);

	virtual VOID CleanUp(VOID);

	INT GetLogicCount(VOID)const
	{
		return m_nLogicCount;
	}

protected:
	VOID SetLogicCount(INT nLogicCount)
	{
		m_nLogicCount = nLogicCount;
	}

private:
	INT			m_nLogicCount;		// �߼�����
};

/////////////////////////////////////////////
// CObjectCommand_Action ����
/////////////////////////////////////////////
class CObjectCommand_Action :
	public CObjectCommand_Logic
{
public:
	CObjectCommand_Action(VOID);
	virtual ~CObjectCommand_Action(VOID);

	BOOL Init(UINT uStartTime, INT nLogicCount, INT nActionID);
	virtual VOID CleanUp(VOID);

	INT GetActionID(VOID)const
	{
		return m_nActionID;
	}

private:
	INT			m_nActionID;		// ����ID
};

/////////////////////////////////////////////
// CObjectCommand_StopAction ��ֹ����
/////////////////////////////////////////////
class CObjectCommand_StopAction :
	public CObjectCommand_StopLogic
{
public:
	CObjectCommand_StopAction(VOID);
	virtual ~CObjectCommand_StopAction(VOID);

	BOOL Init(INT nLogicCount, UINT uEndTime);
	virtual VOID CleanUp(VOID);

	UINT GetEndTime(VOID)const
	{
		return m_uEndTime;
	}

private:
	UINT		m_uEndTime;			// ����ʱ��
};

/////////////////////////////////////////////
// CObjectCommand_Move �ƶ�
/////////////////////////////////////////////
class CObjectCommand_StopMove;
class CObjectCommand_Move :
	public CObjectCommand_Logic
{
public:
	CObjectCommand_Move(VOID);
	virtual ~CObjectCommand_Move(VOID);

	BOOL Init(UINT uStartTime, INT nLogicCount, INT nNodeCount, const WORLD_POS *paposNode);
	virtual VOID CleanUp(VOID);

	virtual BOOL Modify(const CObjectCommand_StopLogic *pCmd);

	INT GetNodeCount(VOID)const
	{
		return m_nNodeCount;
	}

	const WORLD_POS *GetNodeList(VOID)const
	{
		return m_paposNode;
	}

	BOOL ModifyTargetPos(const BYTE numPos, const WORLD_POS* targetPos);

private:
	INT			m_nNodeCount;		// �ڵ���Ŀ
	WORLD_POS	*m_paposNode;		// �ڵ�����
};

/////////////////////////////////////////////
// CObjectCommand_StopMove ��ֹ�ƶ�
/////////////////////////////////////////////
class CObjectCommand_StopMove :
	public CObjectCommand_StopLogic
{
public:
	CObjectCommand_StopMove(VOID);
	virtual ~CObjectCommand_StopMove(VOID);

	BOOL Init(INT nLogicCount, INT nEndNodeIndex, const WORLD_POS *pPos);
	virtual VOID CleanUp(VOID);

	INT GetEndNodeIndex(VOID)const
	{
		return m_nEndNodeIndex;
	}

	const WORLD_POS *GetEndPos(VOID)const
	{
		return &m_posEndPos;
	}

private:
	INT			m_nEndNodeIndex;		// ��ֹ�Ľڵ�����(����ܽڵ�Ϊ1����ô��ֹ�ڵ�ֻ����Ϊ0)
	WORLD_POS	m_posEndPos;			// ��ֹ��λ��(����ֹ�Ľڵ���������ĳ��ֵ�����)
};

/////////////////////////////////////////////
// CObjectCommand_MagicSend ��������
/////////////////////////////////////////////
class CObjectCommand_MagicSend :
	public CObjectCommand_Logic
{
public:
	CObjectCommand_MagicSend(VOID);
	virtual ~CObjectCommand_MagicSend(VOID);

	BOOL Init(UINT uStartTime,
		INT nLogicCount,
		INT nMagicID,
		ObjID_t nTargetID,
		const WORLD_POS *pTargetPos,
		FLOAT fTargetDir);

	virtual VOID CleanUp(VOID);

	INT GetMagicID(VOID)const
	{
		return m_nMagicID;
	}

	ObjID_t GetTargetObjID(VOID)const
	{
		return m_nTargetObjID;
	}

	const WORLD_POS *GetTargetPos(VOID)const
	{
		return &m_posTarget;
	}

	FLOAT GetTargetDir(VOID)const
	{
		return m_fTargetDir;
	}

private:
	INT			m_nMagicID;			// ����ID
	ObjID_t		m_nTargetObjID;		// Ŀ���ɫ
	WORLD_POS	m_posTarget;		// Ŀ��λ��
	FLOAT		m_fTargetDir;		// Ŀ�귽��
};

/////////////////////////////////////////////
// CObjectCommand_MagicCharge ��������
/////////////////////////////////////////////
class CObjectCommand_StopMagicCharge;
class CObjectCommand_MagicCharge :
	public CObjectCommand_Logic
{
public:
	CObjectCommand_MagicCharge(VOID);
	virtual ~CObjectCommand_MagicCharge(VOID);

	BOOL Init(UINT uStartTime,
		INT nLogicCount,
		INT nMagicID,
		ObjID_t nTargetID,
		const WORLD_POS *pTargetPos,
		FLOAT fTargetDir,
		UINT uTotalTime);

	virtual VOID CleanUp(VOID);

	virtual BOOL Modify(const CObjectCommand_StopLogic *pCmd);

	INT GetMagicID(VOID)const
	{
		return m_nMagicID;
	}

	ObjID_t GetTargetObjID(VOID)const
	{
		return m_nTargetObjID;
	}

	const WORLD_POS *GetTargetPos(VOID)const
	{
		return &m_posTarget;
	}

	FLOAT GetTargetDir(VOID)const
	{
		return m_fTargetDir;
	}

	UINT GetTotalTime(VOID)const
	{
		return m_uTotalTime;
	}

	UINT GetEndTime(VOID)const
	{
		return m_uEndTime;
	}

private:
	INT			m_nMagicID;			// ����ID
	ObjID_t		m_nTargetObjID;		// Ŀ���ɫ
	WORLD_POS	m_posTarget;		// Ŀ��λ��
	FLOAT		m_fTargetDir;		// Ŀ�귽��
	UINT		m_uTotalTime;		// ��ʱ��
	UINT		m_uEndTime;			// ����ʱ��
};

/////////////////////////////////////////////
// CObjectCommand_MagicChannel ��������
/////////////////////////////////////////////
class CObjectCommand_StopMagicChannel;
class CObjectCommand_MagicChannel :
	public CObjectCommand_Logic
{
public:
	CObjectCommand_MagicChannel(VOID);
	virtual ~CObjectCommand_MagicChannel(VOID);

	BOOL Init(UINT uStartTime,
		INT nLogicCount,
		INT nMagicID,
		ObjID_t nTargetID,
		const WORLD_POS *pTargetPos,
		FLOAT fTargetDir,
		UINT uTotalTime);

	virtual VOID CleanUp(VOID);

	virtual BOOL Modify(const CObjectCommand_StopLogic *pCmd);

	INT GetMagicID(VOID)const
	{
		return m_nMagicID;
	}

	ObjID_t GetTargetObjID(VOID)const
	{
		return m_nTargetObjID;
	}

	const WORLD_POS *GetTargetPos(VOID)const
	{
		return &m_posTarget;
	}

	FLOAT GetTargetDir(VOID)const
	{
		return m_fTargetDir;
	}

	UINT GetTotalTime(VOID)const
	{
		return m_uTotalTime;
	}

	UINT GetEndTime(VOID)const
	{
		return m_uEndTime;
	}

private:
	INT			m_nMagicID;			// ����ID
	ObjID_t		m_nTargetObjID;		// Ŀ���ɫ
	WORLD_POS	m_posTarget;		// Ŀ��λ��
	FLOAT		m_fTargetDir;		// Ŀ�귽��
	UINT		m_uTotalTime;		// ��ʱ��
	UINT		m_uEndTime;			// ����ʱ��
};

/////////////////////////////////////////////
// CObjectCommand_Ability �����
/////////////////////////////////////////////
class CObjectCommand_StopAbility;
class CObjectCommand_Ability :
	public CObjectCommand_Logic
{
public:
	CObjectCommand_Ability(VOID);
	virtual ~CObjectCommand_Ability(VOID);

	BOOL Init(UINT uStartTime, INT nLogicCount, INT nAbilityID, INT nPrescriptionID, ObjID_t nTargetObjID);
	virtual VOID CleanUp(VOID);

	virtual BOOL Modify(const CObjectCommand_StopLogic *pCmd);

	INT GetAbilityID(VOID)const
	{
		return m_nAbilityID;
	}

	INT GetPrescriptionID(VOID)const
	{
		return m_nPrescriptionID;
	}

	ObjID_t GetTargetObjID(VOID)const
	{
		return m_nTargetObjID;
	}

	UINT GetEndTime(VOID)const
	{
		return m_uEndTime;
	}

private:
	INT			m_nAbilityID;		// �����ID
	INT			m_nPrescriptionID;	// �䷽ID
	ObjID_t		m_nTargetObjID;		// Ŀ���ɫID
	UINT		m_uEndTime;			// ����ʱ��
};


/////////////////////////////////////////////////////////////////////////////////////////
// ת������
/////////////////////////////////////////////////////////////////////////////////////////
struct SCommand_Object;
extern CObjectCommand *NewObjectCommand(const SCommand_Object *pCmd);
extern VOID DeleteObjectCommand(CObjectCommand *pCmd);
