// AdvancedListCtrlDlg.cpp
#include "stdafx.h"
#include "AdvancedListCtrlDlg.h"

IMPLEMENT_DYNAMIC(CAdvancedListCtrlDlg, CDialogEx)

CAdvancedListCtrlDlg::CAdvancedListCtrlDlg(CWnd* pParent)
    : CDialogEx(IDD, pParent)
{
}

CAdvancedListCtrlDlg::~CAdvancedListCtrlDlg()
{
}

BOOL CAdvancedListCtrlDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 리스트 컨트롤 초기화
    CRect rcList(10, 10, 700, 400);
    m_listCtrl.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_OWNERDRAWFIXED,
                      rcList, this, IDC_LIST_CTRL);

    // 리스트 컨트롤 설정 (5행 5열)
    InitializeListCtrl();

    // 테스트 데이터 입력
    FillTestData();

    return TRUE;
}

void CAdvancedListCtrlDlg::InitializeListCtrl()
{
    // 리스트 컨트롤 초기화 (5행, 5열)
    m_listCtrl.Initialize(5, 5);

    // 헤더 설정
    m_listCtrl.SetHeaderText(0, _T("ID"));
    m_listCtrl.SetHeaderText(1, _T("Name"));
    m_listCtrl.SetHeaderText(2, _T("Select"));
    m_listCtrl.SetHeaderText(3, _T("Status"));
    m_listCtrl.SetHeaderText(4, _T("Action"));

    // 헤더 너비 설정
    m_listCtrl.SetHeaderWidth(0, 60);
    m_listCtrl.SetHeaderWidth(1, 100);
    m_listCtrl.SetHeaderWidth(2, 80);
    m_listCtrl.SetHeaderWidth(3, 100);
    m_listCtrl.SetHeaderWidth(4, 80);

    // 헤더 색상 설정 (진한 파란색 배경, 흰색 글자)
    for (int col = 0; col < m_listCtrl.GetColCount(); col++)
    {
        m_listCtrl.SetHeaderBackColor(col, RGB(100, 150, 200));
        m_listCtrl.SetHeaderTextColor(col, RGB(255, 255, 255));
    }

    // 컬럼별 셀 타입 설정
    for (int row = 0; row < m_listCtrl.GetRowCount(); row++)
    {
        // 0: 텍스트 (ID)
        m_listCtrl.SetCellType(row, 0, CELL_TEXT);

        // 1: 텍스트 (Name)
        m_listCtrl.SetCellType(row, 1, CELL_TEXT);

        // 2: 체크박스 (Select)
        m_listCtrl.SetCellCheckBox(row, 2, FALSE);

        // 3: 콤보박스 (Status)
        std::vector<CString> statusItems;
        statusItems.push_back(_T("Active"));
        statusItems.push_back(_T("Inactive"));
        statusItems.push_back(_T("Pending"));
        m_listCtrl.SetCellComboBox(row, 3, statusItems, 0);

        // 4: 버튼 (Action)
        m_listCtrl.SetCellButton(row, 4, _T("Delete"));
    }
}

void CAdvancedListCtrlDlg::FillTestData()
{
    // 테스트 데이터 입력
    for (int row = 0; row < m_listCtrl.GetRowCount(); row++)
    {
        // ID 입력
        CString id;
        id.Format(_T("%d"), row + 1);
        m_listCtrl.SetCellText(row, 0, id);

        // Name 입력
        CString name;
        name.Format(_T("Item %d"), row + 1);
        m_listCtrl.SetCellText(row, 1, name);

        // 체크박스 초기값
        m_listCtrl.SetCellCheckBox(row, 2, row % 2 == 0);

        // 콤보박스 선택 (기본값: Active)
        m_listCtrl.SetCellComboBox(row, 3, 
            {_T("Active"), _T("Inactive"), _T("Pending")}, 
            row % 3);

        // 행 배경색 설정 (짝수 행: 흰색, 홀수 행: 연한 회색)
        if (row % 2 == 0)
            m_listCtrl.SetRowBackColor(row, RGB(255, 255, 255));
        else
            m_listCtrl.SetRowBackColor(row, RGB(240, 240, 245));
    }
}

void CAdvancedListCtrlDlg::OnDestroy()
{
    CDialogEx::OnDestroy();
}

BEGIN_MESSAGE_MAP(CAdvancedListCtrlDlg, CDialogEx)
    ON_WM_DESTROY()
END_MESSAGE_MAP()
