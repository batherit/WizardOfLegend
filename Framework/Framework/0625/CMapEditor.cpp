#include "stdafx.h"
#include "CMapEditor.h"
#include "UI_Button.h"
#include "CCamera2D.h"
#include "CAtlasLoader.h"
#include "CTileMapObj.h"
#include "CAtlasObj.h"


CMapEditor::CMapEditor(CGameWorld& _rGameWorld)
	:
	m_rGameWorld(_rGameWorld)
{
	// 파일로부터 맵툴 데이터 로드
	GenerateAtlasFromFile();

	// 맵 제어 버튼
	m_vecEditorButtons.emplace_back(new CUI_Button<CMapEditor>(m_rGameWorld, 50 * 1, WINCY - 55, 40, 25, TEXT("W--"), this, &CMapEditor::ChangeMapWidth, new int(-5)));
	m_vecEditorButtons.emplace_back(new CUI_Button<CMapEditor>(m_rGameWorld, 50 * 2, WINCY - 55, 40, 25, TEXT("W-"), this, &CMapEditor::ChangeMapWidth, new int(-1)));
	m_vecEditorButtons.emplace_back(new CUI_Button<CMapEditor>(m_rGameWorld, 50 * 3, WINCY - 55, 40, 25, TEXT("W+"), this, &CMapEditor::ChangeMapWidth, new int(1)));
	m_vecEditorButtons.emplace_back(new CUI_Button<CMapEditor>(m_rGameWorld, 50 * 4, WINCY - 55, 40, 25, TEXT("W++"), this, &CMapEditor::ChangeMapWidth, new int(5)));
	m_vecEditorButtons.emplace_back(new CUI_Button<CMapEditor>(m_rGameWorld, 50 * 1, WINCY - 25, 40, 25, TEXT("H--"), this, &CMapEditor::ChangeMapHeight, new int(-5)));
	m_vecEditorButtons.emplace_back(new CUI_Button<CMapEditor>(m_rGameWorld, 50 * 2, WINCY - 25, 40, 25, TEXT("H-"), this, &CMapEditor::ChangeMapHeight, new int(-1)));
	m_vecEditorButtons.emplace_back(new CUI_Button<CMapEditor>(m_rGameWorld, 50 * 3, WINCY - 25, 40, 25, TEXT("H+"), this, &CMapEditor::ChangeMapHeight, new int(1)));
	m_vecEditorButtons.emplace_back(new CUI_Button<CMapEditor>(m_rGameWorld, 50 * 4, WINCY - 25, 40, 25, TEXT("H++"), this, &CMapEditor::ChangeMapHeight, new int(5)));

	// 카메라 제어 버튼
	m_vecEditorButtons.emplace_back(new CUI_Button<CMapEditor>(m_rGameWorld, 50 * 1, WINCY - 85, 40, 25, TEXT("Cent"), this, &CMapEditor::MoveCameraToMapCenter, nullptr));

	// 아틀라스 비저블 토큰 버튼
	TCHAR szID[10];
	for (int id = 0; id < m_stMapRenderInfo.vecAtlasLoaders.size(); id++) {
		swprintf_s(szID, TEXT("Atlas_%d"), id);
		m_vecEditorButtons.emplace_back(new CUI_Button<CMapEditor>(m_rGameWorld, WINCX - 100, 60 + id * 30, 60, 25, szID, this, &CMapEditor::ToggleAtlas, new int(id)));
	}
	
	TO_GAMEWORLD(m_rGameWorld).GetCamera()->SetXY(GetMapMiddleX(), GetMapMiddleY());
}


CMapEditor::~CMapEditor()
{
	Release();
}

void CMapEditor::GenerateAtlasFromFile(void)
{
	FILE* fpIn = nullptr;
	errno_t err = fopen_s(&fpIn, "../MapToolDatas/Atlas.txt", "rt");
	if (!err) {
		int iAtlasNum = 0;
		fscanf_s(fpIn, "%d ", &iAtlasNum);			// 로드할 아틀라스 수
		_atlas_loader_info stAtlasInfo;
		for (int i = 0; i < iAtlasNum; i++) {
			// 문자열은 fgets로 하는게 안전한 것 같다.
			do {
				fgets(stAtlasInfo.szAtlasFileDirectory, sizeof(stAtlasInfo.szAtlasFileDirectory), fpIn);
			} while (strcmp(stAtlasInfo.szAtlasFileDirectory, "\n") == 0);	// 개행문자는 거른다.
			stAtlasInfo.szAtlasFileDirectory[strlen(stAtlasInfo.szAtlasFileDirectory) - 1] = '\0';	// 개행문자 제거
			int size = strlen(stAtlasInfo.szAtlasFileDirectory);
			fscanf_s(fpIn, " %d %d %d %d %f %d %d",
				&stAtlasInfo.iID,
				&stAtlasInfo.eLoaderType,
				&stAtlasInfo.iAtlasWidth,
				&stAtlasInfo.iAtlasHeight,
				&stAtlasInfo.fAtlasRatio,
				&stAtlasInfo.iTileWidth,
				&stAtlasInfo.iTileHeight);
			// i를 아틀라스 로더의 아이디로 삼는다.
			m_stMapRenderInfo.vecAtlasLoaders.emplace_back(new CAtlasLoader(i, stAtlasInfo));
		}
	}
	if (fpIn) fclose(fpIn);
}

void CMapEditor::Update(float _fDeltaTime)
{
	CKeyMgr* pKeyMgrInst = CKeyMgr::GetInstance();
	
	// 맵 드래그 관련(우클릭으로 맵을 드래그한다.)
	if (pKeyMgrInst->IsKeyDown(KEY::KEY_RBUTTON)) {
		pKeyMgrInst->SetOldClickedPoint(GetClientCursorPoint());
	}
	if (pKeyMgrInst->IsKeyPressing(KEY::KEY_RBUTTON)) {
		POINT ptOldClickedPoint = pKeyMgrInst->GetOldClickedPoint();
		POINT ptCurClickedPoint = GetClientCursorPoint();
		CObj* pCamera = TO_GAMEWORLD(m_rGameWorld).GetCamera();
		POINT ptDeltaMove = {
			(ptOldClickedPoint.x - ptCurClickedPoint.x) / TO_CAMERA2D(pCamera)->GetZoomMultiple(),
			(ptOldClickedPoint.y - ptCurClickedPoint.y) / TO_CAMERA2D(pCamera)->GetZoomMultiple()
		};
		TO_CAMERA2D(pCamera)->MoveTo(ptDeltaMove.x, ptDeltaMove.y);
		pKeyMgrInst->SetOldClickedPoint(ptCurClickedPoint);
	}
	if (pKeyMgrInst->IsKeyUp(KEY::KEY_RBUTTON)) {
		pKeyMgrInst->SetOldClickedPoint(POINT{0, 0});
	}

	
	if (pKeyMgrInst->IsKeyDown(KEY::KEY_LBUTTON)) {
		if (m_iVisibleAtlasID != -1) {
			// 클릭시 타일 검출 관련. (Detect Tile)
			m_stDetectedAtlasObj = m_stMapRenderInfo.vecAtlasLoaders[m_iVisibleAtlasID]->GetDetectedTileRowCol(GetClientCursorPoint());
			m_stMapRenderInfo.vecAtlasLoaders[m_iVisibleAtlasID]->SetVisible(false);
			m_iVisibleAtlasID = -1;
		}
		else {
			// 타일맵에 타일을 칠한다. (Paint Tile)
			CObj* pCamera = TO_GAMEWORLD(m_rGameWorld).GetCamera();
			POINT ptScreenPoint = GetClientCursorPoint();
			// 스크린 커서 좌표 -> 월드 커서 좌표 변환
			pair<float, float> pairWorldPoint = TO_CAMERA2D(pCamera)->GetWorldPoint(ptScreenPoint.x, ptScreenPoint.y);
			POINT ptWorldPoint;
			ptWorldPoint.x = pairWorldPoint.first;
			ptWorldPoint.y = pairWorldPoint.second;

			if (IsPointInRect(GetMapRect(), ptWorldPoint)) {
				int iCol = pairWorldPoint.first / GetTileWidth();
				int iRow = pairWorldPoint.second / GetTileHeight();

				m_listMapObjs.emplace_back(new CAtlasObj(m_stMapRenderInfo, iRow, iCol, m_stDetectedAtlasObj));
			}
		}
	}

	for (auto& pButton : m_vecEditorButtons) {
		pButton->Update(_fDeltaTime);
	}
}

void CMapEditor::Render(HDC & _hdc, CCamera2D* _pCamera)
{
	RenderTileBoard(_hdc, _pCamera);

	for (auto& pObj : m_listMapObjs) {
		pObj->Render(_hdc, _pCamera);
	}

	for (auto& pButton : m_vecEditorButtons) {
		pButton->Render(_hdc, nullptr);
	}
	for (auto& pAtlas : m_stMapRenderInfo.vecAtlasLoaders) {
		pAtlas->Render(_hdc, _pCamera);
	}
	RenderZeroPoint(_hdc, _pCamera);
	RenderDetectedTile(_hdc, _pCamera);
}

void CMapEditor::Release(void)
{
	for (auto& pButton : m_vecEditorButtons) {
		DeleteSafe(pButton);
	}
	m_vecEditorButtons.clear();
	m_vecEditorButtons.shrink_to_fit();
}

void CMapEditor::RenderTileBoard(HDC & _hdc, CCamera2D * _pCamera)
{
	pair<float, float> pairConvPoint;
	// 가로줄 그리기
	for (int i = 0; i < m_stMapRenderInfo.iMapHeight + 1; i++) {
		pairConvPoint = _pCamera->GetScreenPoint(GetMapLeft(), GetMapTop() + GetTileHeight() * i);
		MoveToEx(_hdc, pairConvPoint.first, pairConvPoint.second, nullptr);

		pairConvPoint = _pCamera->GetScreenPoint(GetMapRight(), GetMapTop() + GetTileHeight() * i);
		LineTo(_hdc, pairConvPoint.first, pairConvPoint.second);
	}

	// 세로줄 그리기
	for (int i = 0; i < m_stMapRenderInfo.iMapWidth + 1; i++) {
		pairConvPoint = _pCamera->GetScreenPoint(GetMapLeft() + GetTileWidth() * i, GetMapTop());
		MoveToEx(_hdc, pairConvPoint.first, pairConvPoint.second, nullptr);

		pairConvPoint = _pCamera->GetScreenPoint(GetMapLeft() + GetTileWidth() * i, GetMapBottom());
		LineTo(_hdc, pairConvPoint.first, pairConvPoint.second);
	}
}

void CMapEditor::RenderZeroPoint(HDC & _hdc, CCamera2D * _pCamera)
{
	pair<float, float> pairScreenPoint = _pCamera->GetScreenPoint(0.f, 0.f);

	Ellipse(_hdc, 
		pairScreenPoint.first - (GetTileWidth() >> 2) * _pCamera->GetZoomMultiple(),
		pairScreenPoint.second - (GetTileHeight() >> 2) * _pCamera->GetZoomMultiple(),
		pairScreenPoint.first + (GetTileWidth() >> 2) * _pCamera->GetZoomMultiple(),
		pairScreenPoint.second + (GetTileHeight() >> 2) * _pCamera->GetZoomMultiple());
}

void CMapEditor::RenderDetectedTile(HDC & _hdc, CCamera2D * _pCamera)
{
	// 검출된 타일이 없다면 그리지 않는다.
	if (m_stDetectedAtlasObj.iAtlasID < 0) return;

	// 검출된 타일이 속한 아틀라스 비트맵을 가져온다.
	HDC memdc = CreateCompatibleDC(_hdc);
	HBITMAP m_bitmapOldAtlas = (HBITMAP)SelectObject(memdc, m_stMapRenderInfo.vecAtlasLoaders[m_stDetectedAtlasObj.iAtlasID]->GetBitmap());

	// 검출된 타일의 너비와 높이
	_atlas_loader_info stAtlasInfo = m_stMapRenderInfo.vecAtlasLoaders[m_stDetectedAtlasObj.iAtlasID]->GetAtlasInfo();

	// 검출된 타일 출력 좌표
	int fX = WINCX - GetTileWidth() - 30;
	int fY = WINCY - GetTileHeight() - 30;

	StretchBlt(_hdc,
		fX,			// 출력 시작좌표 X
		fY,			// 출력 시작좌표 Y
		GetTileWidth(),					// 출력 크기
		GetTileHeight(),				// 출력 크기
		memdc, 
		m_stDetectedAtlasObj.rcOutputArea.left,
		m_stDetectedAtlasObj.rcOutputArea.top,
		m_stDetectedAtlasObj.rcOutputArea.right - m_stDetectedAtlasObj.rcOutputArea.left,
		m_stDetectedAtlasObj.rcOutputArea.bottom - m_stDetectedAtlasObj.rcOutputArea.top,
		SRCCOPY);
	DeleteDC(memdc);
}

void CMapEditor::ChangeMapWidth(void* _pDeltaWidth)
{
	int iDeltaWidth = GetMapWidth() + *static_cast<int*>(_pDeltaWidth);
	SetMapWidth(Clamp(iDeltaWidth, 1, 1000));
}

void CMapEditor::ChangeMapHeight(void* _pDeltaHeight)
{
	int iDeltaHeight = GetMapHeight() + *static_cast<int*>(_pDeltaHeight);
	SetMapHeight(Clamp(iDeltaHeight, 1, 1000));
}

void CMapEditor::MoveCameraToMapCenter(void *)
{
	TO_GAMEWORLD(m_rGameWorld).GetCamera()->SetXY(GetMapMiddleX(), GetMapMiddleY());
}

void CMapEditor::ToggleAtlas(void * _pID)
{
	int iID = *static_cast<int*>(_pID);

	bool bIsVisible = m_stMapRenderInfo.vecAtlasLoaders[iID]->IsVisible();

	for (int i = 0; i < m_stMapRenderInfo.vecAtlasLoaders.size(); i++) {
		m_stMapRenderInfo.vecAtlasLoaders[i]->SetVisible(false);
	}
	
	if (bIsVisible) {
		m_stMapRenderInfo.vecAtlasLoaders[iID]->SetVisible(false);
		m_iVisibleAtlasID = -1;			// 현재 보여지고 있는 아틀라스가 없다.
	}
	else {
		m_stMapRenderInfo.vecAtlasLoaders[iID]->SetVisible(true);
		m_iVisibleAtlasID = iID;		// 현재 보여지고 있는 아틀라스 아이디를 등록. => 타일 검출에 쓰임.
	}
}
