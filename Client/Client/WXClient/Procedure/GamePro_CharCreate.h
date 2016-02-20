/****************************************\
*										*
* 			   ���ﴴ������				*
*										*
\****************************************/
#pragma once

#include "GameProcedure.h"
#include "CharacterFaceMng.h"
#include "FaceModelMng.h"
#include "HairModelMng.h"

class CObject_PlayerOther;
class CGamePro_CharCreate : public CGameProcedure
{

public:
	

	//��¼״̬
	enum PLAYER_CHAR_CREATE_STATUS
	{
		CHAR_CREATE_CREATE_OK = 0 ,	// ������ɫ�ɹ�
		CHAR_CREATE_CREATEING,		// ������ɫ
	};


	PLAYER_CHAR_CREATE_STATUS	m_Status;
	void					CallInit(){ Init(); }

	CObject_PlayerOther*	m_pAvatar[2];						// ����UI��ʾ���߼�����.
	TCHAR					m_szBufModelName1[128];				// Ůģ������.
	TCHAR					m_szBufModelName2[128];				// ��ģ������.


	TCHAR					m_szCreateRoleName[128];			// Ҫ�����Ľ�ɫ������
	int						m_iFaceColor1;						// �����沿��ɫ
	int 					m_iFaceModle1;						// �����沿ģ��
	int 					m_iHairColor1;						// ����ͷ����ɫ
	int 					m_iHairModle1;						// ����ͷ��ģ��
	int 					m_iSex;								// �����Ա�
	int						m_iFaceId;							// ����ͷ��
	CampID_t				m_iCamp;
	int						m_iMenpai;

	

	// ������ɫ
	int						CreateRole();

	// �л�����ɫѡ�����
	void					ChangeToRoleSel();


public:

	//////////////////////////////////////////////////////////////////////////////
	//
	//
	// ͷ�������.
	CCharacterFaceMng	m_FaceMng;

	// �Ƿ��ȡͷ����Ϣ.
	int					m_iIsLoadFace;

	// ����ͷ��id
	void	SetFace(int iFaceId);	

	// ͨ���Ա����������ͷ��id
	void SetFaceByRaceAndIndex(int iRace, int iIndex);	

	/////////////////////////////////////////////////////////////////////////////
	//
	// 
	//������Ϣ������
	CFaceModelMng		m_FaceModelmng;

	// �Ƿ��ȡ����ģ����Ϣ.
	int					m_iIsLoadFaceModel;

	// ��������id
	void	SetFaceModel(int iFaceModelId);	

	// ͨ���Ա����������ͷ��id
	void SetFaceModelByRaceAndIndex(int iRace, int iIndex);	


	////////////////////////////////////////////////////////////////////////////
	//
	//
	// ������Ϣ������
	CHairModelMng		m_HairModelMng;

	// �Ƿ��ȡ����ģ����Ϣ.
	int					m_iIsLoadHairModel;

	// ��������id
	void	SetHairModel(int iHairModelId);	

	// ͨ���Ա����������ͷ��id
	void SetHairModelByRaceAndIndex(int iRace, int iIndex);	





protected:
	virtual VOID	Init(VOID);
	virtual VOID	Tick(VOID);
	virtual VOID	Render(VOID);
	virtual VOID	Release(VOID);
	virtual VOID	MouseInput(VOID);
	//��������
	virtual VOID	ProcessInput(VOID);

public:
	CGamePro_CharCreate(VOID);
	virtual ~CGamePro_CharCreate(VOID);

	// ��������ģ��
	int CreateModel();

	// �л���ѡ���������
	void ChangeToSelectRole();
};
