/*
=============================================================================
                    CAdvancedListCtrl 사용 가이드
=============================================================================

1. 기본 초기화
-----------
    CAdvancedListCtrl m_listCtrl;
    
    // 리스트 컨트롤 생성 (5행, 5열)
    CRect rcList(10, 10, 700, 400);
    m_listCtrl.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT | LVS_OWNERDRAWFIXED,
                      rcList, this, IDC_LIST_CTRL);
    
    // 초기화
    m_listCtrl.Initialize(5, 5);  // 5행, 5열

2. 헤더 설정
-----------
    // 헤더 텍스트 설정
    m_listCtrl.SetHeaderText(0, _T("ID"));
    m_listCtrl.SetHeaderText(1, _T("Name"));
    
    // 헤더 너비 설정
    m_listCtrl.SetHeaderWidth(0, 100);
    m_listCtrl.SetHeaderWidth(1, 150);
    
    // 헤더 색상 설정
    m_listCtrl.SetHeaderBackColor(0, RGB(100, 150, 200));
    m_listCtrl.SetHeaderTextColor(0, RGB(255, 255, 255));
    
    // 헤더 셀 타입 설정 (버튼, 체크박스 등)
    m_listCtrl.SetHeaderCellType(0, CELL_CHECKBOX);

3. 셀 데이터 설정
-----------
    // 텍스트 입력
    m_listCtrl.SetCellText(0, 0, _T("Data1"));
    
    // 셀 색상 설정
    m_listCtrl.SetCellBackColor(0, 0, RGB(255, 255, 200));  // 노란색 배경
    m_listCtrl.SetCellTextColor(0, 0, RGB(255, 0, 0));      // 빨간색 글자
    
    // 셀 타입 설정
    m_listCtrl.SetCellType(0, 0, CELL_TEXT);

4. 체크박스 사용
-----------
    // 체크박스 설정
    m_listCtrl.SetCellCheckBox(0, 1, FALSE);  // 미선택
    
    // 체크박스 값 가져오기
    BOOL bChecked = m_listCtrl.GetCellCheckBox(0, 1);
    
    // 체크박스 토글
    if (bChecked)
        m_listCtrl.SetCellCheckBox(0, 1, FALSE);
    else
        m_listCtrl.SetCellCheckBox(0, 1, TRUE);

5. 콤보박스 사용
-----------
    // 콤보박스 아이템 설정
    std::vector<CString> items;
    items.push_back(_T("Item1"));
    items.push_back(_T("Item2"));
    items.push_back(_T("Item3"));
    m_listCtrl.SetCellComboBox(0, 2, items, 0);  // 0번 항목 선택
    
    // 콤보박스 아이템 추가
    m_listCtrl.AddComboItem(0, 2, _T("Item4"));
    
    // 선택된 항목 인덱스 가져오기
    int nSelected = m_listCtrl.GetCellComboBoxSelected(0, 2);
    
    // 선택된 항목 텍스트 가져오기
    CString strSelected = m_listCtrl.GetCellComboBoxText(0, 2);

6. 버튼 사용
-----------
    // 버튼 설정
    m_listCtrl.SetCellButton(0, 3, _T("Delete"));
    
    // 버튼 클릭 이벤트 처리 (커스텀 클래스에서 오버라이드)
    void OnButtonClicked(int row, int col)
    {
        if (col == 3)  // Delete 버튼
        {
            MessageBox(_T("Delete button clicked!"));
        }
    }

7. 행 설정
-----------
    // 행 배경색 설정
    m_listCtrl.SetRowBackColor(0, RGB(200, 200, 200));
    
    // 행 글자색 설정
    m_listCtrl.SetRowTextColor(0, RGB(255, 0, 0));

8. 셀 활성화/비활성화
-----------
    // 셀 비활성화 (클릭 불가, 회색 표시)
    m_listCtrl.SetCellEnabled(0, 1, FALSE);
    
    // 셀 활성화
    m_listCtrl.SetCellEnabled(0, 1, TRUE);

9. 데이터 조회
-----------
    // 행 개수
    int nRows = m_listCtrl.GetRowCount();
    
    // 열 개수
    int nCols = m_listCtrl.GetColCount();
    
    // 셀 텍스트 가져오기
    CString strText = m_listCtrl.GetCellText(0, 0);
    
    // 셀 포인터 가져오기
    CellInfo* pCell = m_listCtrl.GetCell(0, 0);
    if (pCell)
    {
        // 셀 정보 접근
        CString text = pCell->text;
        COLORREF color = pCell->bgColor;
    }

10. 반복문 예제
-----------
    // 모든 행과 열 반복
    for (int row = 0; row < m_listCtrl.GetRowCount(); row++)
    {
        for (int col = 0; col < m_listCtrl.GetColCount(); col++)
        {
            CString strData = m_listCtrl.GetCellText(row, col);
            // 데이터 처리
        }
    }
    
    // 특정 행의 모든 열 반복
    int nRow = 0;
    for (int col = 0; col < m_listCtrl.GetColCount(); col++)
    {
        CString strData = m_listCtrl.GetCellText(nRow, col);
    }

11. 이벤트 처리 (커스텀 클래스에서 오버라이드)
-----------
    class CMyListCtrl : public CAdvancedListCtrl
    {
    public:
        virtual void OnButtonClicked(int row, int col)
        {
            CString msg;
            msg.Format(_T("Button clicked at row %d, col %d"), row, col);
            MessageBox(msg);
        }
        
        virtual void OnCheckBoxChanged(int row, int col, BOOL bChecked)
        {
            CString msg;
            msg.Format(_T("CheckBox at row %d, col %d is %s"), 
                      row, col, bChecked ? _T("checked") : _T("unchecked"));
            MessageBox(msg);
        }
        
        virtual void OnComboBoxSelectionChanged(int row, int col, int nSelected)
        {
            CString msg;
            msg.Format(_T("ComboBox at row %d, col %d selected index %d"), 
                      row, col, nSelected);
            MessageBox(msg);
        }
        
        virtual void OnCellSelected(int row, int col)
        {
            CString msg;
            msg.Format(_T("Cell selected at row %d, col %d"), row, col);
            // MessageBox(msg);
        }
    };

12. 전체 예제
-----------
    void CMyDialog::InitializeListCtrl()
    {
        // 초기화 (10행, 5열)
        m_listCtrl.Initialize(10, 5);
        
        // 헤더 설정
        m_listCtrl.SetHeaderText(0, _T("ID"));
        m_listCtrl.SetHeaderText(1, _T("Name"));
        m_listCtrl.SetHeaderText(2, _T("Select"));
        m_listCtrl.SetHeaderText(3, _T("Status"));
        m_listCtrl.SetHeaderText(4, _T("Action"));
        
        // 헤더 너비 설정
        int nWidths[] = {50, 100, 80, 100, 80};
        for (int i = 0; i < 5; i++)
        {
            m_listCtrl.SetHeaderWidth(i, nWidths[i]);
            m_listCtrl.SetHeaderBackColor(i, RGB(100, 150, 200));
            m_listCtrl.SetHeaderTextColor(i, RGB(255, 255, 255));
        }
        
        // 데이터 입력
        for (int row = 0; row < m_listCtrl.GetRowCount(); row++)
        {
            // ID
            CString id;
            id.Format(_T("%d"), row + 1);
            m_listCtrl.SetCellText(row, 0, id);
            
            // Name
            CString name;
            name.Format(_T("Item%d"), row + 1);
            m_listCtrl.SetCellText(row, 1, name);
            
            // 체크박스
            m_listCtrl.SetCellCheckBox(row, 2, FALSE);
            
            // 콤보박스
            std::vector<CString> items;
            items.push_back(_T("Active"));
            items.push_back(_T("Inactive"));
            m_listCtrl.SetCellComboBox(row, 3, items, 0);
            
            // 버튼
            m_listCtrl.SetCellButton(row, 4, _T("Delete"));
            
            // 행 색상 (짝수/홀수 구분)
            if (row % 2 == 0)
                m_listCtrl.SetRowBackColor(row, RGB(255, 255, 255));
            else
                m_listCtrl.SetRowBackColor(row, RGB(240, 240, 245));
        }
    }

=============================================================================
                            주요 함수 정리
=============================================================================

초기화:
    void Initialize(int nMaxRows, int nMaxCols);

헤더:
    void SetHeaderText(int col, const CString& text);
    void SetHeaderBackColor(int col, COLORREF color);
    void SetHeaderTextColor(int col, COLORREF color);
    void SetHeaderWidth(int col, int width);
    void SetHeaderCellType(int col, CellType type);

셀:
    void SetCellText(int row, int col, const CString& text);
    void SetCellBackColor(int row, int col, COLORREF color);
    void SetCellTextColor(int row, int col, COLORREF color);
    void SetCellType(int row, int col, CellType type);
    void SetCellEnabled(int row, int col, BOOL bEnabled);

체크박스:
    void SetCellCheckBox(int row, int col, BOOL bChecked);
    BOOL GetCellCheckBox(int row, int col) const;

콤보박스:
    void SetCellComboBox(int row, int col, const std::vector<CString>& items, int nSelected = 0);
    void AddComboItem(int row, int col, const CString& item);
    int GetCellComboBoxSelected(int row, int col) const;
    CString GetCellComboBoxText(int row, int col) const;

버튼:
    void SetCellButton(int row, int col, const CString& text);

행:
    void SetRowBackColor(int row, COLORREF color);
    void SetRowTextColor(int row, COLORREF color);
    int GetRowCount() const;
    int GetColCount() const;

조회:
    CString GetCellText(int row, int col) const;
    CellInfo* GetCell(int row, int col);
    RowInfo* GetRow(int row);
    HeaderInfo* GetHeader(int col);

*/
