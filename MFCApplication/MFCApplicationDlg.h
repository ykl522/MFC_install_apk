
// MFCApplicationDlg.h: 头文件
//
#pragma once


// CMFCApplicationDlg 对话框
class CMFCApplicationDlg : public CDialogEx
{
// 构造
public:
	CMFCApplicationDlg(CWnd* pParent = nullptr);	// 标准构造函数
	CString CMFCApplicationDlg::ExecuteCmd(CString str);
	TCHAR* CMFCApplicationDlg::StringToChar(CString& str);
	vector<std::string> CMFCApplicationDlg::SplitCString(std::string str, std::string pattern);

public:
	CString usbSelected;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListBox m_listBox;
	afx_msg void OnLbnSelchangeList();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedOk();
	CStatic m_text_install_status;
	CComboBox m_usbDevices;
	afx_msg void OnCbnSelchangeDevices();
//	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedInfo();
};
