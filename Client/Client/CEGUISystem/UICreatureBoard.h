/****************************\
*							*
*	 ��ɫͷ���ϵ���ϢUI		*
*							*
\***************************/
#pragma once
#include "TDUISystem.h"
#include "ceguistring.h"

namespace CEGUI
{
	class Window;
};

enum PLAYERINFO_STATE
{
	STATE_NORMAL = 0,
	STATE_DIE,
};

class CCreatureBoard : public tCreatureBoard
{
public:
	// �ƺ�����
	enum TITLE_TYPE
	{
		DIMNESS_TITLE = 0,
		BANGPAI_TITLE,
		SELF_ADD_TITLE,
		MENPAI_TITLE,
		MOOD_TITLE,
		PLAYERSHOP_TITLE,

		MAX_TITLE,
	};
	enum  BANGPAI_POSITION
	{
		NO_POS = 0,
		EXCELLENCE_POS,	    // ��Ӣ
		OFFICEHOLDER_POS,	// ��Ա
		CHAIRMAN_POS,		// ����

		MAX_POS,
	};

	//ɾ������
	virtual VOID				Destroy(VOID);
	//��������Ļ�ϵ�����(����)
	virtual VOID				SetPosition(FLOAT nX, FLOAT nY);
	//��ʾ/����
	virtual VOID				Show(BOOL bShow);

	//��������
	virtual VOID				SetElement_Name(LPCTSTR szName);	
	//���óƺ�
	virtual VOID                SetElement_Title(LPCTSTR szTitle, INT nType = 0);

	//���öӳ����
	virtual VOID				SetElement_LeaderFlag(BOOL bShow);

	//���ð�̯�ϵ�����
	virtual VOID				SetElement_SaleText(LPCTSTR szSaleText);
	//�����Ƿ���ʾ��̯��Ϣ
	virtual VOID				SetElement_SaleSign(BOOL bShow);

	virtual VOID				SetElement_PaoPaoText( LPCTSTR szPaoPao );

	virtual BOOL				isShow(){return m_bShow?TRUE:FALSE;};

	//�����߼�����ID
	virtual VOID				SetElement_ObjId(INT nObjId);

	CEGUI::Window*				GetMainWindow(void) { return m_pWindow; }

	virtual fVector2			GetPosition(void) const;

	virtual VOID				SetInfoState( int nState );

	virtual void				SetInfoDistance( float nDistance ) { m_nDistance = nDistance; }; // ���������ǵ�λ�ã�����һ����Χ��λ�û�������ֱ䵭

	virtual VOID				SetTitleType(INT nType) { m_nTitleType = nType; }
	virtual VOID				SetBangpaiFlag(INT nBangpaiFlag) { m_nBangpaiFlag = nBangpaiFlag; }

protected:
	//���񡰰�̯"��ť����֪ͨ
	bool						handleSaleSignClicked(const CEGUI::EventArgs& e);
	//�������ƣ����Ƕӳ���־��
	void						_UpdateNameElement(void);
	//�ӻ����ȡ������������
	void reset(void);


protected:
	CEGUI::Window*		m_pWindow;				// ����ָ��
	CEGUI::Window*		m_pNameWindow;			// ����ָ��
	CEGUI::Window*		m_pTitleWindow;			// �ƺŴ���ָ��
	CEGUI::Window*		m_pSaleWindow;			// ��̯ͷ��������
	CEGUI::Window*		m_pLeadFlag;			// �ӳ�ͼ��
	CEGUI::Window*		m_pPaoPao;				// ��������
	CEGUI::Window*		m_pBangpaiFlag;			// ����Ȩ�ޱ�־

	BOOL				m_bShow;				//�Ƿ���ʾ 

	//��Ӧ���߼������ID
	BOOL				m_bShowStallSign;
	INT					m_nObjId;
	//�������
	STRING				m_strName;
	//�ӳ���־
	BOOL				m_bLeaderFlag;

	INT					m_nTitleType;			// �ƺ�����
	INT					m_nBangpaiFlag;			// ���ɳƺ��ϵ�Ȩ�ޱ��

	int					m_InfoState;
	float				m_nDistance;

	float				m_fLastX, m_fLastY;
	int					m_nDebugIndex;
	void debugInfo(void);

private:
	friend class CUICreatureBoardSystem;
	CCreatureBoard(int nIndex);
	virtual ~CCreatureBoard();
};