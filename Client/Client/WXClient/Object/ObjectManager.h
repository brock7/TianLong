/****************************************\
*										*
* 			   ���������				*
*										*
\****************************************/

#pragma once
#include "TDObjectSystem.h"
#include "Type.h"
class CObject;
class CObject_Character;
class CObject_PlayerMySelf;
class CObject_PlayerOther;
class CObject_PlayerNPC;
class CObject_Surface;
class CObjectLoadQueue;

class CObjectManager :
	public tObjectSystem
{


	
protected:
	static CObjectManager*			s_pMe;

public:
	static CObjectManager*  GetMe( VOID ){ return s_pMe; }
	CObject_PlayerOther* GetTarget_Avatar( VOID ) { return m_pMainTargetAvatar;}

public:
	//��ݲ�����ȡ������Լ�
	CObject_PlayerMySelf*	GetMySelf( VOID ){ return m_pMySelf; }

public:
	CObjectManager( VOID );
	virtual ~CObjectManager( VOID );

	virtual VOID	Initial( VOID* );
	virtual VOID	Release( VOID );

	virtual VOID	Tick( VOID );

public:
	// ����������,������������
	virtual tObject*		NewObject(LPCTSTR szClass, INT idServer=-1, tObject* pParent=NULL);
	// ��������,�����������ϲ��
	virtual BOOL			DestroyObject(tObject* pObject);
	// ����ID�õ�ĳ����
	virtual tObject*		FindObject( INT id );
	// ��ȡһ��SERVER OBJ�����ָ��
	virtual tObject*		FindServerObject( INT idServer );
	// �����Ʒ
	virtual tObject_Item*	FindItem(INT id);
	// ������������
	virtual VOID			ClearAllObject( VOID );
	/// ������Ŀ��������id�Ƿ���ȡ��ѡ������
	virtual VOID			SetMainTarget(INT id, DESTROY_MAIN_TARGET_TYPE delType = DEL_OBJECT);
	//ȡ�õ�ǰѡ������
	virtual tObject*		GetMainTarget(VOID);

	//ȡ�õ�ǰѡ������
	virtual tObject*		GetMainTarget(GUID_t& guid);

	//�������һ��������Ϊ��Ŀ��
	void					Debug_LockNearestTarget(void);
	//�������һ��������Ϊ��Ŀ�깥��
	void					Debug_LockNearestTargetToHit(void);
	//����ĳojbect��ServerID
	virtual BOOL			SetObjectServerID( INT idObj, INT idServer );
	// ������Ļ�������ѡ�е�����
	virtual tObject*		GetMouseOverObject( INT nX, INT nY, fVector3& fvMouseHitPlan );
	//����ĳ�����ָ���¼� id-����ID		bCare-���Ļ���ȡ������
	virtual VOID			CareObject(INT id, BOOL bCare, std::string szSign);
	//�ӱ�����Դ���ж�ȡ��Ʒ����
	virtual LPCTSTR			ItemNameByTBIndex(UINT idTable);
	//��Դ���ض���
	CObjectLoadQueue*		GetLoadQueue(void) { return m_pLoadQueue; }
	//�����������ֻ�ö�Ӧ��objcet
	virtual CObject_Character*		FindCharacterByName(std::string  szName);
	VOID					SetTargetAvatarAsTarget( VOID );
	//��Ӧ�ֱ��ʸı�
	void					OnScreenSizeChanged(void);
	//��Ӧ���ǵȼ����,��������npc�ĵȼ�
	void					OnMySelfLevelUp(void);
	//��������NPC
	void					SetLoadNPCDirect(BOOL b);

public:
	// ����������,������������
	virtual CObject_PlayerOther*	NewPlayerOther( INT idServer );

	// ����������,������������
	virtual CObject_PlayerNPC*		NewPlayerNPC( INT idServer );

	// ������������
	virtual CObject_Surface*		NewSurfaceObj( LPCTSTR szClass );


public:

	// ���õ�ǰѡ����Ƕ���
	void SetCurSelTeam(GUID_t id);
	GUID_t GetCurSelTeam();

	// ���ö����Ƿ��ڵ�ǰ���ӷ�Χ
	void SetPresent(BOOL  bIsPresent);
	
	// ��ǰѡ����Ƿ��Ƕ���2006��4��1
	BOOL m_bIsSelTeam;

	// ѡ��Ķ����Ƿ�����ǰ2006��4��1
	BOOL m_bIsSelTeamInPresent;

	// ��ǰѡ����Ƿ��Ƕ���2006��4��1
	BOOL IsSelTeam();

	// ѡ��Ķ����Ƿ�����ǰ2006��4��1
	BOOL IsSelTeamInPresent();

	// ѡ�ж��ѵ�gudi
	GUID_t					m_GUID;
	
	// ��ѡ��mainTarget
	void UnSelMainTarget(void);
	CObject*						m_pNearestTargetOLD;

protected:
	CObject_PlayerMySelf*			m_pMySelf;				// ���ѱ���
	CObject*						m_pObjectRoot;			// ������ڵ�
		CObject*					m_pLogicalObject;		// �߼�����ڵ�
			CObject*				m_pPlayerOther;			// ������ҽڵ�
			CObject*				m_pNPC;					// NPC�ڵ�
			CObject*				m_pSurface;				// �ر�����ڵ�
		CObject*					m_pAlogicalObject;		// ���߼�����ڵ�
	
	typedef std::map< INT, CObject* > CObjectMap;
	CObjectMap						m_hmObject;				// ����ID������HASH��
	CObjectMap						m_hmServerObject;		// ����ServerID������HASH��

	CObject*						m_pMainTarget;			// ��Ŀ�����
	CObject_PlayerOther*			m_pMainTargetAvatar;	// ����UI��ʾ���߼�����.

	//��UI���ĵ��߼�����
	struct OBJECT_BECARED
	{
		INT			id;					//����ID
		FLOAT		fLastDistance;		//������֮��ľ���
	};
	//Ŀǰ�����ĵ��߼�����
	std::map< std::string, OBJECT_BECARED >	m_mapCaredObject;

	//��Դ���ض���
	CObjectLoadQueue*				m_pLoadQueue;

protected:
	VOID AttachObject( tObject *pObject );
	VOID DetchObject( tObject *pObject );

private:
	INT							IDFactory( VOID );

protected:
	WX_DECLARE_DYNAMIC(CObjectManager);
};

