// HelperDlg.h : ͷ�ļ�
//

#pragma once
#include "SplitWnd.h"
#include "explorer_helper.h"
#include "TreeCtrlEx.h"
#include "CaptionButtonDialog.h"

class CHelperItem;
class CCaptionButton;
// CHelperDlg �Ի���
class CHelperDlg : public CCaptionButtonDlg
{
public:
	// go to spec item
	void		GotoHelperItem(HTREEITEM hItem);

// ����
public:
	CHelperDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CHelperDlg();

// �Ի�������
	enum { IDD = IDD_HELPER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

protected:
	// resize all dialog control.
	void		RepositionDlg(void);
	// refresh all items
	void		RefreshHelpItems(void);
	// on item tree select item changed.
	void		OnItemSelectChanged(void);
	// on pin button pushed
	void		OnPinButtonPushed(void);
	// draw grip size
	void		DrawGripSize(void);

private:
	// refresh helper items to a tree item
	void		_refreshHelpItem(HTREEITEM hItem, CHelperItem& thisItem);

// ʵ��
protected:
	HICON			m_hIcon;
	CSplitWnd		m_staSplitVer;
	CTreeCtrlEx		m_treeItems;
	CExplorer_help	m_webHelper;
	CCaptionButton*	m_pCaptionBtn;

	INT				m_nLeftWidth;

	// ���ɵ���Ϣӳ�亯��
	DECLARE_MESSAGE_MAP()

	// virtual function
	virtual BOOL OnInitDialog();
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual void OnCancel();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	// process message
	afx_msg void	OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void	OnSize(UINT nType, int cx, int cy);
	afx_msg void	OnTvnSelchangedTreeItems(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void	OnClose();
	afx_msg LRESULT OnWMU_Hitcapbutton(WPARAM wParam, LPARAM lParam);
public:
	DECLARE_EVENTSINK_MAP()
	void BeforeNavigate2ExplorerHelper(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData, VARIANT* Headers, BOOL* Cancel);
};
