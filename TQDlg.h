
// TQDlg.h: 头文件
//

#pragma once



// TQDlg 对话框
class TQDlg : public CDialogEx
{
// 构造
public:
	TQDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TQ_DIALOG };
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

	int TQDlg::RetrieveObjects(char* pdatabuffer, unsigned long datalen, int* pobject_pos);

	afx_msg void OnBnClickedOpen();
	afx_msg void OnBnClickedWrite();

};
