// AdvancedListCtrlDlg.h
#pragma once
#include "AdvancedListCtrl.h"

class CAdvancedListCtrlDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CAdvancedListCtrlDlg)

public:
    CAdvancedListCtrlDlg(CWnd* pParent = NULL);
    virtual ~CAdvancedListCtrlDlg();

    enum { IDD = IDD_ADVANCEDLISTCTRLDLG };

protected:
    virtual BOOL OnInitDialog();
    afx_msg void OnDestroy();
    DECLARE_MESSAGE_MAP()

private:
    CAdvancedListCtrl m_listCtrl;

    void InitializeListCtrl();
    void FillTestData();
};
