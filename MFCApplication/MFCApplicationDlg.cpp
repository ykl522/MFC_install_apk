
// MFCApplicationDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication.h"
#include "MFCApplicationDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplicationDlg 对话框



CMFCApplicationDlg::CMFCApplicationDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplicationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, Device_List, m_listBox);
	DDX_Control(pDX, Install_Status, m_text_install_status);
	DDX_Control(pDX, USB_Devices, m_usbDevices);
}

BEGIN_MESSAGE_MAP(CMFCApplicationDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_LBN_SELCHANGE(Device_List, &CMFCApplicationDlg::OnLbnSelchangeList)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDOK, &CMFCApplicationDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(USB_Devices, &CMFCApplicationDlg::OnCbnSelchangeDevices)
//	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(Look_Info, &CMFCApplicationDlg::OnBnClickedInfo)
END_MESSAGE_MAP()


// CMFCApplicationDlg 消息处理程序

BOOL CMFCApplicationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}

	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
	CString cmd = ExecuteCmd(_T("adb.exe devices"));
	int index = cmd.Find(_T("List of devices attached"));
	//CString iStr;
	//iStr.Format(_T("%d"), index);
	//MessageBox(iStr);
	cmd = cmd.Mid(index + 24);
	//TCHAR* psz = StringToChar(cmd);
	//std::string str = psz;
	//char *chr = new char[cmd.GetLength()];
	//WideCharToMultiByte(CP_ACP, 0, cmd.GetBuffer(), -1, chr, cmd.GetLength(), NULL, NULL);
	//std::string str = chr;
	cmd.Replace(L"\r\n", L"");
	cmd.Replace(L"\t", L"");
	//cmd = cmd.Replace("\\t", "");
	vector<std::string> devices = SplitCString((LPCSTR)(CStringA(cmd)), "device");
	//MessageBox(devices[0]);
	for (auto device : devices) {
		if (device.size() > 0) {
			//m_listBox.AddString((CString)device.c_str());
			m_usbDevices.AddString((CString)device.c_str());
		}
	}
	m_usbDevices.SetCurSel(0);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCApplicationDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCApplicationDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCApplicationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCApplicationDlg::OnLbnSelchangeList()
{
	// TODO: 在此添加控件通知处理程序代码
	CString path;
	m_listBox.GetText(m_listBox.GetCurSel(), path);
	if (path.Find(_T(".apk")) != -1) {
		m_text_install_status.SetWindowTextW(_T("已选中一个安装包"));
	}
	else {
		m_text_install_status.SetWindowTextW(_T("请选中一个安装包"));
	}
	
}

CString CMFCApplicationDlg::ExecuteCmd(CString str)
{
	SECURITY_ATTRIBUTES sa;
	HANDLE hRead, hWrite;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(&hRead, &hWrite, &sa, 0))
	{
		MessageBox(_T("Error on CreatePipe()!"));
		return NULL;
	}
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	si.hStdError = hWrite;
	si.hStdOutput = hWrite;
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	TCHAR* cmdline = StringToChar(str);
	if (!CreateProcess(NULL, cmdline, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi))
	{
		MessageBox(_T("Error on CreateProcess()!"));
		return NULL;
	}
	CloseHandle(hWrite);

	char buffer[4096];
	memset(buffer, 0, 4096);
	CString output;
	DWORD byteRead;
	while (true)
	{
		if (ReadFile(hRead, buffer, 4095, &byteRead, NULL) == NULL)
		{
			break;
		}
		output += buffer;
	}
	
	return output;
}


TCHAR* CMFCApplicationDlg::StringToChar(CString& str)
{
	int len = str.GetLength();
	TCHAR* tr = str.GetBuffer(len);
	str.ReleaseBuffer();
	return tr;
}

vector<std::string> CMFCApplicationDlg::SplitCString(std::string str, std::string pattern)
{
	std::string::size_type pos;
    std::vector<std::string> result;
    str += pattern;//扩展字符串以方便操作
    int size = str.size();
    for (int i = 0; i < size; i++)
    {
        pos = str.find(pattern, i);
        if (pos < size)
        {
            std::string s = str.substr(i, pos - i);
            result.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }
    return result;
}




void CMFCApplicationDlg::OnDropFiles(HDROP hDropInfo)
{
	TCHAR szPath[MAX_PATH] = { 0 };
	UINT nCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	for (UINT i = 0; i < nCount; i++)
	{
		DragQueryFile(hDropInfo, i, szPath, MAX_PATH);

		if (m_listBox.FindString(0, szPath) < 0)//已存在则不添加
		{
			m_listBox.AddString(szPath);
		}
	}

	CDialogEx::OnDropFiles(hDropInfo);
}


void CMFCApplicationDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	
	CString path;
	m_listBox.GetText(m_listBox.GetCurSel(), path);
	if (path.Find(_T(".apk")) != -1) {
		m_text_install_status.SetWindowTextW(_T("安装中请稍候..."));
	}
	else {
		m_text_install_status.SetWindowTextW(_T("请选择一个apk安装包"));
		return;
	}
	CString cmdResult;
	if (usbSelected.GetLength() > 0) {
		cmdResult = ExecuteCmd(_T("adb.exe -s ") + usbSelected + _T(" install -r ") + path);
	}
	else {
		cmdResult = ExecuteCmd(_T("adb.exe install -r ") + path);
	}
	m_text_install_status.SetWindowTextW(cmdResult);
	//MessageBox(cmd);
	//CDialogEx::OnOK();
}


void CMFCApplicationDlg::OnCbnSelchangeDevices()
{
	CString ComSelItem(_T(""));//获取当前下拉框的值 
	m_usbDevices.GetLBText(m_usbDevices.GetCurSel(), ComSelItem);//获取CComBox下拉框当前选中的值
	CMFCApplicationDlg::usbSelected = ComSelItem;
	//MessageBox(usbSelected);
}


//void CMFCApplicationDlg::OnRButtonDown(UINT nFlags, CPoint point)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	MessageBox(_T("haha"));
//
//	CDialogEx::OnRButtonDown(nFlags, point);
//}


void CMFCApplicationDlg::OnBnClickedInfo()
{
	// TODO: 在此添加控件通知处理程序代码
	CString path;
	m_listBox.GetText(m_listBox.GetCurSel(), path);
	if (path.Find(_T(".apk")) != -1) {
		m_text_install_status.SetWindowTextW(_T("解析中请稍候..."));
	}
	else {
		m_text_install_status.SetWindowTextW(_T("请选择一个apk安装包"));
		return;
	}
	CString cmdResult = ExecuteCmd(_T("aapt.exe dump badging ") + path);
	cmdResult = cmdResult.Left(cmdResult.Find(_T("uses-permission")));
	//MessageBox(cmdResult);
	m_text_install_status.SetWindowTextW(cmdResult);
}
