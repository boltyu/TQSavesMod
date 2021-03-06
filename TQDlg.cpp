
// TQDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "TQ.h"
#include "TQDlg.h"
#include <Windows.h>
#include <string.h>
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define OBJECT_TEXT_MAX	100
#define OBJECT_NUMBER	6
#define	NUMBER_MONEY	0
#define NUMBER_LEVEL	1
#define NUMBER_EXP		2
#define NUMBER_MODIFIER	3
#define NUMBER_SKILL	4
#define NUMBER_DEATH	5

#define IDC_NUMBER_MONEY IDC_MYEDIT-1000

CButton open_file;
CMFCEditBrowseCtrl file_path;
char databuffer[1000000];
int object_pos[10];
DWORD lenofread;



class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif
	protected:
		virtual void DoDataExchange(CDataExchange* pDX);

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

TQDlg::TQDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TQ_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void TQDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILEPATH, file_path);
	DDX_Control(pDX, IDC_OPEN, open_file);
}

BEGIN_MESSAGE_MAP(TQDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN, &TQDlg::OnBnClickedOpen)
	//ON_STN_CLICKED(IDC_MYSTATIC, &TQDlg::OnStnClickedMystatic)

	ON_BN_CLICKED(IDC_WRITE, &TQDlg::OnBnClickedWrite)
END_MESSAGE_MAP()

BOOL TQDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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


	SetIcon(m_hIcon, TRUE);	
	SetIcon(m_hIcon, FALSE);
	
	TCHAR documentPath[200] = TEXT("");
	SHGetSpecialFolderPathW(NULL, documentPath, CSIDL_PERSONAL, FALSE);
	CString dp;
	dp += documentPath;
	dp += _T("\\My Games\\Titan Quest - Immortal Throne\\SaveData\\Main\\");
	SetDlgItemTextW(IDC_FILEPATH, dp);

	return TRUE;
}

void TQDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void TQDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR TQDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void TQDlg::OnBnClickedOpen()
{
	int iResult;
	CString filePath;
	if (GetDlgItemTextW(IDC_FILEPATH, filePath) > 4)
	{
		HANDLE hFile;
		hFile = CreateFile(filePath, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL/* | FILE_FLAG_OVERLAPPED*/, NULL);	//CreateFile ref: https://docs.microsoft.com/en-us/windows/desktop/fileio/opening-a-file-for-reading-or-writing
		if (hFile == INVALID_HANDLE_VALUE)
		{
			SetDlgItemTextW(IDC_FILEPATH, _T("Open Failed"));
			return;
		}
		else
		{
			OVERLAPPED ol = { 0 };
			memset(databuffer, 0, 1000000);		// Max buffer 1M
			BY_HANDLE_FILE_INFORMATION fileinfo;
			GetFileInformationByHandle(hFile, &fileinfo);
			if (ReadFile(hFile, databuffer, 1000000, &lenofread, &ol) == FALSE)
			{
				iResult = GetLastError();
				SetDlgItemTextW(IDC_FILEPATH, _T("Read Failed"));
				return;
			}
			else
			{
				RetrieveObjects(databuffer, lenofread, object_pos);

				char object_num_text[10];
				memset(object_num_text,0,10);
				int *object_value = (int*)&databuffer[object_pos[NUMBER_MONEY]];
				_itoa_s(*object_value, object_num_text,10,10);
				CString object_num_edit;
				object_num_edit = object_num_text;
				SetDlgItemTextW(IDC_MYEDIT, object_num_edit);
				object_value = (int*)&databuffer[object_pos[NUMBER_EXP]];
				_itoa_s(*object_value, object_num_text, 10, 10);
				object_num_edit = object_num_text;
				SetDlgItemTextW(IDC_EXPEDIT, object_num_edit);
				object_value = (int*)&databuffer[object_pos[NUMBER_LEVEL]];
				_itoa_s(*object_value, object_num_text, 10, 10);
				object_num_edit = object_num_text;
				SetDlgItemTextW(IDC_LVEDIT, object_num_edit);
				object_value = (int*)&databuffer[object_pos[NUMBER_SKILL]];
				_itoa_s(*object_value, object_num_text, 10, 10);
				object_num_edit = object_num_text;
				SetDlgItemTextW(IDC_SKEDIT, object_num_edit);
				object_value = (int*)&databuffer[object_pos[NUMBER_MODIFIER]];
				_itoa_s(*object_value, object_num_text, 10, 10);
				object_num_edit = object_num_text;
				SetDlgItemTextW(IDC_STEDIT, object_num_edit);

				CloseHandle(hFile);
			}
		}
	}
	else
	{
		SetDlgItemTextW(IDC_FILEPATH, _T("Bad PATH"));
		return;
	}
	
}

void TQDlg::OnBnClickedWrite()
{
	CString filePath;
	GetDlgItemTextW(IDC_FILEPATH, filePath);
	/*int backup_num = 0;
	filePath += (WCHAR)(backup_num + 0x30);*/
	HANDLE hFile;
	/*for (;backup_num < 10; backup_num++)
	{*/
		hFile = CreateFile(filePath, GENERIC_WRITE, NULL, NULL, OPEN_EXISTING /*CREATE_NEW*/, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			SetDlgItemTextW(IDC_FILEPATH, _T("Write Failed"));
			return;
		}
	/*	else
			break;
		return;
	}*/
	int *object_value;
	char object_value_text[10];

	object_value = (int*)&databuffer[object_pos[NUMBER_MONEY]];
	GetDlgItemTextA(this->m_hWnd, IDC_MYEDIT, object_value_text, 10);
	*object_value = atoi(object_value_text);

	object_value = (int*)&databuffer[object_pos[NUMBER_EXP]];
	GetDlgItemTextA(this->m_hWnd, IDC_EXPEDIT, object_value_text, 10);
	*object_value = atoi(object_value_text);

	object_value = (int*)&databuffer[object_pos[NUMBER_LEVEL]];
	GetDlgItemTextA(this->m_hWnd, IDC_LVEDIT, object_value_text, 10);
	*object_value = atoi(object_value_text);
	
	object_value = (int*)&databuffer[object_pos[NUMBER_MODIFIER]];
	GetDlgItemTextA(this->m_hWnd, IDC_STEDIT, object_value_text, 10);
	*object_value = atoi(object_value_text);

	object_value = (int*)&databuffer[object_pos[NUMBER_SKILL]];
	GetDlgItemTextA(this->m_hWnd, IDC_SKEDIT, object_value_text, 10);
	*object_value = atoi(object_value_text);

	int* repaire_header = (int*)databuffer;
	*repaire_header = 0x0d;
	OVERLAPPED ol = { 0 };
	DWORD lenofwrite = 0;
	WriteFile(hFile, databuffer, lenofread, &lenofwrite, &ol);
	CloseHandle(hFile);

}


/*	function	:	get object's position in the savefile
	parameters	:
		databuffer[read]	:	all data in the file
		datalen[read]		:	bytes of data
		object_pos[write]	:	retrieved object's position in the file
	return		:   0 finish
*/
int TQDlg::RetrieveObjects(char* pdatabuffer, unsigned long datalen, int* pobject_pos)
{
	char object_text[OBJECT_NUMBER*OBJECT_TEXT_MAX];	// specific the number of objects in security
	int object_strlen[OBJECT_NUMBER];

	memset(object_text, 0, OBJECT_NUMBER * OBJECT_TEXT_MAX);
	strcpy_s(&object_text[NUMBER_MONEY * OBJECT_TEXT_MAX], OBJECT_TEXT_MAX-1, "money");
	strcpy_s(&object_text[NUMBER_EXP * OBJECT_TEXT_MAX], OBJECT_TEXT_MAX-1, "currentStats.experiencePoints");
	strcpy_s(&object_text[NUMBER_LEVEL * OBJECT_TEXT_MAX], OBJECT_TEXT_MAX-1, "currentStats.charLevel");
	strcpy_s(&object_text[NUMBER_MODIFIER * OBJECT_TEXT_MAX], OBJECT_TEXT_MAX-1, "modifierPoints");
	strcpy_s(&object_text[NUMBER_SKILL * OBJECT_TEXT_MAX], OBJECT_TEXT_MAX-1, "skillPoints");
	strcpy_s(&object_text[NUMBER_DEATH * OBJECT_TEXT_MAX], OBJECT_TEXT_MAX-1, "numberOfDeath");

	unsigned long loopcount = 864;	//skip header(about) of save
	int objectcount = 0;

	for (; objectcount < OBJECT_NUMBER; objectcount++)
	{
		pobject_pos[objectcount] = 0;
		object_strlen[objectcount] = strnlen_s(&object_text[objectcount * OBJECT_TEXT_MAX], OBJECT_TEXT_MAX);
		for (; loopcount < datalen; loopcount++)
		{
			if (pdatabuffer[loopcount] == object_text[objectcount * OBJECT_TEXT_MAX + pobject_pos[objectcount]])
			{
				pobject_pos[objectcount]++;
				if (pobject_pos[objectcount] == object_strlen[objectcount])
				{
					pobject_pos[objectcount] = loopcount + 1;	// found
					break;
				}
			}
			else
				pobject_pos[objectcount] = 0;
		}
	}
	return 0;
}