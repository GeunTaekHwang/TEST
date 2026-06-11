#include "stdafx.h"
#include "AdvancedListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CAdvancedListCtrl, CListCtrl)

CAdvancedListCtrl::CAdvancedListCtrl()
    : m_nMaxRows(0), m_nMaxCols(0), m_nHeaderHeight(30), m_nRowHeight(25),
      m_nSelectedRow(-1), m_nSelectedCol(-1), m_nPressedRow(-1), m_nPressedCol(-1),
      m_bMouseDown(FALSE)
{
}

CAdvancedListCtrl::~CAdvancedListCtrl()
{
}

BEGIN_MESSAGE_MAP(CAdvancedListCtrl, CListCtrl)
    ON_WM_PAINT()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

void CAdvancedListCtrl::Initialize(int nMaxRows, int nMaxCols)
{
    m_nMaxRows = nMaxRows;
    m_nMaxCols = nMaxCols;

    // 데이터 초기화
    m_rows.clear();
    m_headers.clear();

    m_rows.resize(nMaxRows);
    m_headers.resize(nMaxCols);

    for (int i = 0; i < nMaxRows; i++)
    {
        m_rows[i].cells.resize(nMaxCols);
    }

    // 리스트 컨트롤 스타일 설정
    DWORD dwStyle = GetStyle();
    SetWindowLong(m_hWnd, GWL_STYLE, dwStyle | LVS_REPORT | LVS_OWNERDRAWFIXED);

    // 컬럼 추가
    for (int i = 0; i < nMaxCols; i++)
    {
        InsertColumn(i, _T(""), LVCFMT_LEFT, 100);
    }

    // 아이템 추가
    for (int i = 0; i < nMaxRows; i++)
    {
        InsertItem(i, _T(""));
    }

    Invalidate();
}

void CAdvancedListCtrl::PreSubclassWindow()
{
    CListCtrl::PreSubclassWindow();

    // 오너드로우 스타일 설정
    DWORD dwStyle = GetStyle();
    SetWindowLong(m_hWnd, GWL_STYLE, dwStyle | LVS_OWNERDRAWFIXED);
}

BOOL CAdvancedListCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
    cs.style |= LVS_REPORT | LVS_OWNERDRAWFIXED | LVS_SINGLESEL;
    return CListCtrl::PreCreateWindow(cs);
}

void CAdvancedListCtrl::OnPaint()
{
    CPaintDC dc(this);

    CRect rcClient;
    GetClientRect(&rcClient);

    // 더블 버퍼링을 위한 메모리 DC 생성
    CDC memDC;
    memDC.CreateCompatibleDC(&dc);
    CBitmap memBitmap;
    memBitmap.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
    CBitmap* pOldBitmap = memDC.SelectObject(&memBitmap);

    // 배경 그리기
    memDC.FillSolidRect(rcClient, RGB(255, 255, 255));

    // 헤더 그리기
    DrawHeader(&memDC);

    // 행과 셀 그리기
    int yOffset = m_nHeaderHeight;
    for (int row = 0; row < m_nMaxRows; row++)
    {
        int xOffset = 0;
        for (int col = 0; col < m_nMaxCols; col++)
        {
            CRect cellRect;
            cellRect.left = xOffset;
            cellRect.top = yOffset;
            cellRect.right = xOffset + m_headers[col].width;
            cellRect.bottom = yOffset + m_nRowHeight;

            DrawCell(&memDC, row, col, cellRect);

            // 셀 경계선 그리기
            CPen pen(PS_SOLID, 1, RGB(192, 192, 192));
            CPen* pOldPen = memDC.SelectObject(&pen);
            memDC.MoveTo(cellRect.right, cellRect.top);
            memDC.LineTo(cellRect.right, cellRect.bottom);
            memDC.MoveTo(cellRect.left, cellRect.bottom);
            memDC.LineTo(cellRect.right, cellRect.bottom);
            memDC.SelectObject(pOldPen);

            xOffset += m_headers[col].width;
        }
        yOffset += m_nRowHeight;
    }

    // 메모리 DC를 화면 DC에 복사
    dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);

    // 리소스 정리
    memDC.SelectObject(pOldBitmap);
    memBitmap.DeleteObject();
    memDC.DeleteDC();
}

void CAdvancedListCtrl::DrawHeader(CDC* pDC)
{
    int xOffset = 0;
    for (int col = 0; col < m_nMaxCols; col++)
    {
        CRect headerRect;
        headerRect.left = xOffset;
        headerRect.top = 0;
        headerRect.right = xOffset + m_headers[col].width;
        headerRect.bottom = m_nHeaderHeight;

        // 헤더 배경 색상
        pDC->FillSolidRect(headerRect, m_headers[col].bgColor);

        // 헤더 텍스트 그리기
        HeaderInfo& header = m_headers[col];
        if (header.cellType == CELL_CHECKBOX)
        {
            // 체크박스 헤더 (선택 안함 상태)
            DrawCheckBox(pDC, headerRect, FALSE, TRUE);
        }
        else if (header.cellType == CELL_BUTTON)
        {
            DrawButton(pDC, headerRect, header.text, FALSE);
        }
        else if (header.cellType == CELL_COMBOBOX)
        {
            DrawComboBox(pDC, headerRect, header.text, TRUE);
        }
        else
        {
            DrawText(pDC, headerRect, header.text, header.textColor);
        }

        // 헤더 경계선
        CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
        CPen* pOldPen = pDC->SelectObject(&pen);
        pDC->MoveTo(headerRect.right, headerRect.top);
        pDC->LineTo(headerRect.right, headerRect.bottom);
        pDC->MoveTo(headerRect.left, headerRect.bottom);
        pDC->LineTo(headerRect.right, headerRect.bottom);
        pDC->SelectObject(pOldPen);

        xOffset += m_headers[col].width;
    }
}

void CAdvancedListCtrl::DrawCell(CDC* pDC, int row, int col, const CRect& rect)
{
    if (row < 0 || row >= m_nMaxRows || col < 0 || col >= m_nMaxCols)
        return;

    CellInfo& cell = m_rows[row].cells[col];

    // 배경 색상 설정 (행 색상 우선, 없으면 셀 색상)
    COLORREF bgColor = cell.bgColor;
    if (m_rows[row].bgColor != RGB(255, 255, 255))
        bgColor = m_rows[row].bgColor;

    pDC->FillSolidRect(rect, bgColor);

    // 선택된 셀 강조
    if (m_nSelectedRow == row && m_nSelectedCol == col)
    {
        CPen pen(PS_SOLID, 2, RGB(0, 0, 255));
        CPen* pOldPen = pDC->SelectObject(&pen);
        CRect selectRect = rect;
        selectRect.DeflateRect(1, 1);
        pDC->Rectangle(selectRect);
        pDC->SelectObject(pOldPen);
    }

    // 셀 타입별 그리기
    switch (cell.type)
    {
    case CELL_TEXT:
    {
        COLORREF textColor = cell.textColor;
        if (m_rows[row].textColor != RGB(0, 0, 0))
            textColor = m_rows[row].textColor;
        DrawText(pDC, rect, cell.text, textColor);
        break;
    }
    case CELL_CHECKBOX:
        DrawCheckBox(pDC, rect, cell.bChecked, cell.bEnabled);
        break;
    case CELL_BUTTON:
    {
        BOOL bPressed = (m_nPressedRow == row && m_nPressedCol == col && m_bMouseDown);
        DrawButton(pDC, rect, cell.text, bPressed);
        break;
    }
    case CELL_COMBOBOX:
        DrawComboBox(pDC, rect, cell.comboSelectedIndex >= 0 && 
                     cell.comboSelectedIndex < (int)cell.comboItems.size() ?
                     cell.comboItems[cell.comboSelectedIndex] : cell.text, cell.bEnabled);
        break;
    }
}

void CAdvancedListCtrl::DrawText(CDC* pDC, const CRect& rect, const CString& text, COLORREF textColor)
{
    pDC->SetTextColor(textColor);
    pDC->DrawText(text, (CRect&)rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void CAdvancedListCtrl::DrawCheckBox(CDC* pDC, const CRect& rect, BOOL bChecked, BOOL bEnabled)
{
    int size = 16;
    CRect checkRect;
    checkRect.left = rect.left + (rect.Width() - size) / 2;
    checkRect.top = rect.top + (rect.Height() - size) / 2;
    checkRect.right = checkRect.left + size;
    checkRect.bottom = checkRect.top + size;

    // 체크박스 배경
    COLORREF boxColor = bEnabled ? RGB(255, 255, 255) : RGB(240, 240, 240);
    pDC->FillSolidRect(checkRect, boxColor);

    // 체크박스 테두리
    CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
    CPen* pOldPen = pDC->SelectObject(&pen);
    pDC->Rectangle(checkRect);

    // 체크 표시
    if (bChecked)
    {
        CPen checkPen(PS_SOLID, 2, RGB(0, 0, 255));
        pDC->SelectObject(&checkPen);
        pDC->MoveTo(checkRect.left + 3, checkRect.top + 8);
        pDC->LineTo(checkRect.left + 6, checkRect.bottom - 3);
        pDC->LineTo(checkRect.right - 3, checkRect.top + 3);
    }

    pDC->SelectObject(pOldPen);
}

void CAdvancedListCtrl::DrawButton(CDC* pDC, const CRect& rect, const CString& text, BOOL bPressed)
{
    // 버튼 배경
    COLORREF bgColor = bPressed ? RGB(100, 100, 100) : RGB(200, 200, 200);
    pDC->FillSolidRect(rect, bgColor);

    // 버튼 테두리
    CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
    CPen* pOldPen = pDC->SelectObject(&pen);
    pDC->Rectangle(rect);
    pDC->SelectObject(pOldPen);

    // 버튼 텍스트
    pDC->SetTextColor(bPressed ? RGB(255, 255, 255) : RGB(0, 0, 0));
    pDC->DrawText(text, (CRect&)rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void CAdvancedListCtrl::DrawComboBox(CDC* pDC, const CRect& rect, const CString& text, BOOL bEnabled)
{
    COLORREF bgColor = bEnabled ? RGB(255, 255, 255) : RGB(240, 240, 240);
    pDC->FillSolidRect(rect, bgColor);

    // 콤보박스 테두리
    CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
    CPen* pOldPen = pDC->SelectObject(&pen);
    pDC->Rectangle(rect);
    pDC->SelectObject(pOldPen);

    // 텍스트
    CRect textRect = rect;
    textRect.DeflateRect(3, 0, 20, 0);
    pDC->SetTextColor(RGB(0, 0, 0));
    pDC->DrawText(text, textRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

    // 드롭다운 버튼
    CRect btnRect = rect;
    btnRect.left = btnRect.right - 18;
    btnRect.DeflateRect(1, 1);
    pDC->FillSolidRect(btnRect, RGB(200, 200, 200));

    CPen btnPen(PS_SOLID, 1, RGB(0, 0, 0));
    pDC->SelectObject(&btnPen);
    pDC->Rectangle(btnRect);
    pDC->SelectObject(pOldPen);

    // 드롭다운 화살표
    int cx = btnRect.CenterPoint().x;
    int cy = btnRect.CenterPoint().y;
    pDC->MoveTo(cx - 3, cy - 2);
    pDC->LineTo(cx + 3, cy - 2);
    pDC->LineTo(cx, cy + 2);
    pDC->LineTo(cx - 3, cy - 2);
}

CRect CAdvancedListCtrl::GetCellRect(int row, int col)
{
    CRect rect;
    if (row < 0 || row >= m_nMaxRows || col < 0 || col >= m_nMaxCols)
        return rect;

    rect.top = m_nHeaderHeight + row * m_nRowHeight;
    rect.bottom = rect.top + m_nRowHeight;

    int xOffset = 0;
    for (int i = 0; i < col; i++)
        xOffset += m_headers[i].width;

    rect.left = xOffset;
    rect.right = xOffset + m_headers[col].width;

    return rect;
}

CRect CAdvancedListCtrl::GetHeaderRect(int col)
{
    CRect rect;
    if (col < 0 || col >= m_nMaxCols)
        return rect;

    int xOffset = 0;
    for (int i = 0; i < col; i++)
        xOffset += m_headers[i].width;

    rect.left = xOffset;
    rect.right = xOffset + m_headers[col].width;
    rect.top = 0;
    rect.bottom = m_nHeaderHeight;

    return rect;
}

int CAdvancedListCtrl::GetColFromPoint(CPoint point)
{
    int xOffset = 0;
    for (int col = 0; col < m_nMaxCols; col++)
    {
        if (point.x >= xOffset && point.x < xOffset + m_headers[col].width)
            return col;
        xOffset += m_headers[col].width;
    }
    return -1;
}

int CAdvancedListCtrl::GetRowFromPoint(CPoint point)
{
    if (point.y < m_nHeaderHeight)
        return -1;

    int row = (point.y - m_nHeaderHeight) / m_nRowHeight;
    if (row >= m_nMaxRows)
        return -1;
    return row;
}

void CAdvancedListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
    int row = GetRowFromPoint(point);
    int col = GetColFromPoint(point);

    if (row >= 0 && col >= 0)
    {
        m_nSelectedRow = row;
        m_nSelectedCol = col;
        OnCellSelected(row, col);

        CellInfo& cell = m_rows[row].cells[col];

        if (cell.bEnabled)
        {
            switch (cell.type)
            {
            case CELL_CHECKBOX:
                m_bMouseDown = TRUE;
                m_nPressedRow = row;
                m_nPressedCol = col;
                break;
            case CELL_BUTTON:
                m_bMouseDown = TRUE;
                m_nPressedRow = row;
                m_nPressedCol = col;
                break;
            case CELL_COMBOBOX:
                m_bMouseDown = TRUE;
                m_nPressedRow = row;
                m_nPressedCol = col;
                break;
            default:
                break;
            }
        }

        Invalidate();
    }

    CListCtrl::OnLButtonDown(nFlags, point);
}

void CAdvancedListCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (m_bMouseDown)
    {
        int row = GetRowFromPoint(point);
        int col = GetColFromPoint(point);

        if (row == m_nPressedRow && col == m_nPressedCol && row >= 0 && col >= 0)
        {
            CellInfo& cell = m_rows[row].cells[col];

            switch (cell.type)
            {
            case CELL_CHECKBOX:
                cell.bChecked = !cell.bChecked;
                OnCheckBoxChanged(row, col, cell.bChecked);
                break;
            case CELL_BUTTON:
                OnButtonClicked(row, col);
                break;
            case CELL_COMBOBOX:
                HandleComboBoxClick(row, col);
                break;
            default:
                break;
            }
        }

        m_bMouseDown = FALSE;
        m_nPressedRow = -1;
        m_nPressedCol = -1;
        Invalidate();
    }

    CListCtrl::OnLButtonUp(nFlags, point);
}

void CAdvancedListCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
    CListCtrl::OnMouseMove(nFlags, point);
}

BOOL CAdvancedListCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    return CListCtrl::OnMouseWheel(nFlags, zDelta, pt);
}

void CAdvancedListCtrl::HandleComboBoxClick(int row, int col)
{
    CellInfo& cell = m_rows[row].cells[col];
    if (cell.comboItems.size() > 0)
    {
        cell.comboSelectedIndex = (cell.comboSelectedIndex + 1) % cell.comboItems.size();
        OnComboBoxSelectionChanged(row, col, cell.comboSelectedIndex);
    }
}

// 헤더 관련 함수
void CAdvancedListCtrl::SetHeaderText(int col, const CString& text)
{
    if (col >= 0 && col < m_nMaxCols)
    {
        m_headers[col].text = text;
        Invalidate();
    }
}

void CAdvancedListCtrl::SetHeaderBackColor(int col, COLORREF color)
{
    if (col >= 0 && col < m_nMaxCols)
    {
        m_headers[col].bgColor = color;
        Invalidate();
    }
}

void CAdvancedListCtrl::SetHeaderTextColor(int col, COLORREF color)
{
    if (col >= 0 && col < m_nMaxCols)
    {
        m_headers[col].textColor = color;
        Invalidate();
    }
}

void CAdvancedListCtrl::SetHeaderWidth(int col, int width)
{
    if (col >= 0 && col < m_nMaxCols)
    {
        m_headers[col].width = width;
        Invalidate();
    }
}

void CAdvancedListCtrl::SetHeaderCellType(int col, CellType type)
{
    if (col >= 0 && col < m_nMaxCols)
    {
        m_headers[col].cellType = type;
        Invalidate();
    }
}

CString CAdvancedListCtrl::GetHeaderText(int col) const
{
    if (col >= 0 && col < m_nMaxCols)
        return m_headers[col].text;
    return _T("");
}

COLORREF CAdvancedListCtrl::GetHeaderBackColor(int col) const
{
    if (col >= 0 && col < m_nMaxCols)
        return m_headers[col].bgColor;
    return RGB(255, 255, 255);
}

COLORREF CAdvancedListCtrl::GetHeaderTextColor(int col) const
{
    if (col >= 0 && col < m_nMaxCols)
        return m_headers[col].textColor;
    return RGB(0, 0, 0);
}

// 셀 관련 함수
void CAdvancedListCtrl::SetCellText(int row, int col, const CString& text)
{
    if (row >= 0 && row < m_nMaxRows && col >= 0 && col < m_nMaxCols)
    {
        m_rows[row].cells[col].text = text;
        Invalidate();
    }
}

CString CAdvancedListCtrl::GetCellText(int row, int col) const
{
    if (row >= 0 && row < m_nMaxRows && col >= 0 && col < m_nMaxCols)
        return m_rows[row].cells[col].text;
    return _T("");
}

void CAdvancedListCtrl::SetCellBackColor(int row, int col, COLORREF color)
{
    if (row >= 0 && row < m_nMaxRows && col >= 0 && col < m_nMaxCols)
    {
        m_rows[row].cells[col].bgColor = color;
        Invalidate();
    }
}

void CAdvancedListCtrl::SetCellTextColor(int row, int col, COLORREF color)
{
    if (row >= 0 && row < m_nMaxRows && col >= 0 && col < m_nMaxCols)
    {
        m_rows[row].cells[col].textColor = color;
        Invalidate();
    }
}

void CAdvancedListCtrl::SetCellType(int row, int col, CellType type)
{
    if (row >= 0 && row < m_nMaxRows && col >= 0 && col < m_nMaxCols)
    {
        m_rows[row].cells[col].type = type;
        Invalidate();
    }
}

void CAdvancedListCtrl::SetCellCheckBox(int row, int col, BOOL bChecked)
{
    if (row >= 0 && row < m_nMaxRows && col >= 0 && col < m_nMaxCols)
    {
        m_rows[row].cells[col].type = CELL_CHECKBOX;
        m_rows[row].cells[col].bChecked = bChecked;
        Invalidate();
    }
}

BOOL CAdvancedListCtrl::GetCellCheckBox(int row, int col) const
{
    if (row >= 0 && row < m_nMaxRows && col >= 0 && col < m_nMaxCols)
        return m_rows[row].cells[col].bChecked;
    return FALSE;
}

void CAdvancedListCtrl::SetCellComboBox(int row, int col, const std::vector<CString>& items, int nSelected)
{
    if (row >= 0 && row < m_nMaxRows && col >= 0 && col < m_nMaxCols)
    {
        m_rows[row].cells[col].type = CELL_COMBOBOX;
        m_rows[row].cells[col].comboItems = items;
        m_rows[row].cells[col].comboSelectedIndex = nSelected;
        Invalidate();
    }
}

void CAdvancedListCtrl::AddComboItem(int row, int col, const CString& item)
{
    if (row >= 0 && row < m_nMaxRows && col >= 0 && col < m_nMaxCols)
    {
        m_rows[row].cells[col].comboItems.push_back(item);
        Invalidate();
    }
}

int CAdvancedListCtrl::GetCellComboBoxSelected(int row, int col) const
{
    if (row >= 0 && row < m_nMaxRows && col >= 0 && col < m_nMaxCols)
        return m_rows[row].cells[col].comboSelectedIndex;
    return -1;
}

CString CAdvancedListCtrl::GetCellComboBoxText(int row, int col) const
{
    if (row >= 0 && row < m_nMaxRows && col >= 0 && col < m_nMaxCols)
    {
        const CellInfo& cell = m_rows[row].cells[col];
        if (cell.comboSelectedIndex >= 0 && cell.comboSelectedIndex < (int)cell.comboItems.size())
            return cell.comboItems[cell.comboSelectedIndex];
    }
    return _T("");
}

void CAdvancedListCtrl::SetCellButton(int row, int col, const CString& text)
{
    if (row >= 0 && row < m_nMaxRows && col >= 0 && col < m_nMaxCols)
    {
        m_rows[row].cells[col].type = CELL_BUTTON;
        m_rows[row].cells[col].text = text;
        Invalidate();
    }
}

void CAdvancedListCtrl::SetRowBackColor(int row, COLORREF color)
{
    if (row >= 0 && row < m_nMaxRows)
    {
        m_rows[row].bgColor = color;
        Invalidate();
    }
}

void CAdvancedListCtrl::SetRowTextColor(int row, COLORREF color)
{
    if (row >= 0 && row < m_nMaxRows)
    {
        m_rows[row].textColor = color;
        Invalidate();
    }
}

void CAdvancedListCtrl::SetCellEnabled(int row, int col, BOOL bEnabled)
{
    if (row >= 0 && row < m_nMaxRows && col >= 0 && col < m_nMaxCols)
    {
        m_rows[row].cells[col].bEnabled = bEnabled;
        Invalidate();
    }
}

BOOL CAdvancedListCtrl::GetCellEnabled(int row, int col) const
{
    if (row >= 0 && row < m_nMaxRows && col >= 0 && col < m_nMaxCols)
        return m_rows[row].cells[col].bEnabled;
    return FALSE;
}

CellInfo* CAdvancedListCtrl::GetCell(int row, int col)
{
    if (row >= 0 && row < m_nMaxRows && col >= 0 && col < m_nMaxCols)
        return &m_rows[row].cells[col];
    return NULL;
}

RowInfo* CAdvancedListCtrl::GetRow(int row)
{
    if (row >= 0 && row < m_nMaxRows)
        return &m_rows[row];
    return NULL;
}

HeaderInfo* CAdvancedListCtrl::GetHeader(int col)
{
    if (col >= 0 && col < m_nMaxCols)
        return &m_headers[col];
    return NULL;
}
