#pragma once

#include <afxcmn.h>
#include <vector>
#include <map>

// 셀 타입 정의
enum CellType
{
    CELL_TEXT,      // 일반 텍스트
    CELL_BUTTON,    // 버튼
    CELL_CHECKBOX,  // 체크박스
    CELL_COMBOBOX   // 콤보박스
};

// 셀 정보 구조체
struct CellInfo
{
    CellType type;
    CString text;
    COLORREF bgColor;
    COLORREF textColor;
    BOOL bChecked;
    std::vector<CString> comboItems;
    int comboSelectedIndex;
    BOOL bEnabled;

    CellInfo() : type(CELL_TEXT), bgColor(RGB(255, 255, 255)), 
                 textColor(RGB(0, 0, 0)), bChecked(FALSE), 
                 comboSelectedIndex(0), bEnabled(TRUE) {}
};

// 행 정보 구조체
struct RowInfo
{
    std::vector<CellInfo> cells;
    COLORREF bgColor;
    COLORREF textColor;

    RowInfo() : bgColor(RGB(255, 255, 255)), textColor(RGB(0, 0, 0)) {}
};

// 헤더 정보 구조체
struct HeaderInfo
{
    CString text;
    COLORREF bgColor;
    COLORREF textColor;
    int width;
    CellType cellType;

    HeaderInfo() : bgColor(RGB(200, 200, 200)), textColor(RGB(0, 0, 0)), 
                   width(100), cellType(CELL_TEXT) {}
};

class CAdvancedListCtrl : public CListCtrl
{
    DECLARE_DYNAMIC(CAdvancedListCtrl)

public:
    CAdvancedListCtrl();
    virtual ~CAdvancedListCtrl();

public:
    // 초기화 함수
    void Initialize(int nMaxRows, int nMaxCols);

    // 헤더 관련 함수
    void SetHeaderText(int col, const CString& text);
    void SetHeaderBackColor(int col, COLORREF color);
    void SetHeaderTextColor(int col, COLORREF color);
    void SetHeaderWidth(int col, int width);
    void SetHeaderCellType(int col, CellType type);
    CString GetHeaderText(int col) const;
    COLORREF GetHeaderBackColor(int col) const;
    COLORREF GetHeaderTextColor(int col) const;

    // 셀 관련 함수
    void SetCellText(int row, int col, const CString& text);
    CString GetCellText(int row, int col) const;
    void SetCellBackColor(int row, int col, COLORREF color);
    void SetCellTextColor(int row, int col, COLORREF color);
    void SetCellType(int row, int col, CellType type);
    
    // 체크박스 관련
    void SetCellCheckBox(int row, int col, BOOL bChecked);
    BOOL GetCellCheckBox(int row, int col) const;

    // 콤보박스 관련
    void SetCellComboBox(int row, int col, const std::vector<CString>& items, int nSelected = 0);
    void AddComboItem(int row, int col, const CString& item);
    int GetCellComboBoxSelected(int row, int col) const;
    CString GetCellComboBoxText(int row, int col) const;

    // 버튼 관련
    void SetCellButton(int row, int col, const CString& text);
    
    // 행 관련 함수
    void SetRowBackColor(int row, COLORREF color);
    void SetRowTextColor(int row, COLORREF color);
    int GetRowCount() const { return m_nMaxRows; }
    int GetColCount() const { return m_nMaxCols; }

    // 셀 활성화/비활성화
    void SetCellEnabled(int row, int col, BOOL bEnabled);
    BOOL GetCellEnabled(int row, int col) const;

    // 전체 조회 함수
    CellInfo* GetCell(int row, int col);
    RowInfo* GetRow(int row);
    HeaderInfo* GetHeader(int col);

    // 이벤트 핸들러 (오버라이드 가능)
    virtual void OnButtonClicked(int row, int col) {}
    virtual void OnCheckBoxChanged(int row, int col, BOOL bChecked) {}
    virtual void OnComboBoxSelectionChanged(int row, int col, int nSelected) {}
    virtual void OnCellSelected(int row, int col) {}

protected:
    virtual void PreSubclassWindow();
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

    afx_msg void OnPaint();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

    DECLARE_MESSAGE_MAP()

private:
    void DrawCell(CDC* pDC, int row, int col, const CRect& rect);
    void DrawHeader(CDC* pDC);
    void DrawCheckBox(CDC* pDC, const CRect& rect, BOOL bChecked, BOOL bEnabled);
    void DrawButton(CDC* pDC, const CRect& rect, const CString& text, BOOL bPressed);
    void DrawComboBox(CDC* pDC, const CRect& rect, const CString& text, BOOL bEnabled);
    void DrawText(CDC* pDC, const CRect& rect, const CString& text, COLORREF textColor);
    
    CRect GetCellRect(int row, int col);
    CRect GetHeaderRect(int col);
    int GetColFromPoint(CPoint point);
    int GetRowFromPoint(CPoint point);
    void HandleCheckBoxClick(int row, int col);
    void HandleButtonClick(int row, int col);
    void HandleComboBoxClick(int row, int col);

    int m_nMaxRows;
    int m_nMaxCols;
    int m_nHeaderHeight;
    int m_nRowHeight;
    
    std::vector<RowInfo> m_rows;
    std::vector<HeaderInfo> m_headers;
    
    int m_nSelectedRow;
    int m_nSelectedCol;
    int m_nPressedRow;
    int m_nPressedCol;
    BOOL m_bMouseDown;
};
