#include "stdafx.h"
#include "CMapEditor.h"
#include "UI_Button.h"
#include "CCamera2D.h"


CMapEditor::CMapEditor(CGameWorld& _rGameWorld)
	:
	m_rGameWorld(_rGameWorld)
{

	m_vecEditorButtons.emplace_back(new CUI_Button<CMapEditor>(m_rGameWorld, 50 * 1, WINCY - 55, 40, 25, TEXT("W--"), this, &CMapEditor::ChangeMapWidth, new int(-5)));
	m_vecEditorButtons.emplace_back(new CUI_Button<CMapEditor>(m_rGameWorld, 50 * 2, WINCY - 55, 40, 25, TEXT("W-"), this, &CMapEditor::ChangeMapWidth, new int(-1)));
	m_vecEditorButtons.emplace_back(new CUI_Button<CMapEditor>(m_rGameWorld, 50 * 3, WINCY - 55, 40, 25, TEXT("W+"), this, &CMapEditor::ChangeMapWidth, new int(1)));
	m_vecEditorButtons.emplace_back(new CUI_Button<CMapEditor>(m_rGameWorld, 50 * 4, WINCY - 55, 40, 25, TEXT("W++"), this, &CMapEditor::ChangeMapWidth, new int(5)));
	m_vecEditorButtons.emplace_back(new CUI_Button<CMapEditor>(m_rGameWorld, 50 * 1, WINCY - 25, 40, 25, TEXT("H--"), this, &CMapEditor::ChangeMapHeight, new int(-5)));
	m_vecEditorButtons.emplace_back(new CUI_Button<CMapEditor>(m_rGameWorld, 50 * 2, WINCY - 25, 40, 25, TEXT("H-"), this, &CMapEditor::ChangeMapHeight, new int(-1)));
	m_vecEditorButtons.emplace_back(new CUI_Button<CMapEditor>(m_rGameWorld, 50 * 3, WINCY - 25, 40, 25, TEXT("H+"), this, &CMapEditor::ChangeMapHeight, new int(1)));
	m_vecEditorButtons.emplace_back(new CUI_Button<CMapEditor>(m_rGameWorld, 50 * 4, WINCY - 25, 40, 25, TEXT("H++"), this, &CMapEditor::ChangeMapHeight, new int(5)));
	
	TO_GAMEWORLD(m_rGameWorld).GetCamera()->SetXY(GetMapMiddleX(), GetMapMiddleY());
}


CMapEditor::~CMapEditor()
{
	Release();
}

void CMapEditor::Update(float _fDeltaTime)
{
	for (auto& pButton : m_vecEditorButtons) {
		pButton->Update(_fDeltaTime);
	}
}

void CMapEditor::Render(HDC & _hdc, CCamera2D* _pCamera)
{
	RenderZeroPoint(_hdc, _pCamera);
	RenderTileBoard(_hdc, _pCamera);
	for (auto& pButton : m_vecEditorButtons) {
		pButton->Render(_hdc, nullptr);
	}
}

void CMapEditor::Release(void)
{
	for (auto& pButton : m_vecEditorButtons) {
		DeleteSafe(pButton);
	}
	m_vecEditorButtons.clear();
	m_vecEditorButtons.shrink_to_fit();
}

void CMapEditor::RenderTileBoard(const HDC & _hdc, CCamera2D * _pCamera)
{
	pair<float, float> pairConvPoint;
	// 가로줄 그리기
	for (int i = 0; i < m_iMapHeight + 1; i++) {
		pairConvPoint = _pCamera->GetScreenPoint(GetMapLeft(), GetMapTop() + GetTileHeight() * i);
		MoveToEx(_hdc, pairConvPoint.first, pairConvPoint.second, nullptr);

		pairConvPoint = _pCamera->GetScreenPoint(GetMapRight(), GetMapTop() + GetTileHeight() * i);
		LineTo(_hdc, pairConvPoint.first, pairConvPoint.second);
	}

	// 세로줄 그리기
	for (int i = 0; i < m_iMapWidth + 1; i++) {
		pairConvPoint = _pCamera->GetScreenPoint(GetMapLeft() + GetTileWidth() * i, GetMapTop());
		MoveToEx(_hdc, pairConvPoint.first, pairConvPoint.second, nullptr);

		pairConvPoint = _pCamera->GetScreenPoint(GetMapLeft() + GetTileWidth() * i, GetMapBottom());
		LineTo(_hdc, pairConvPoint.first, pairConvPoint.second);
	}

	size_t h1 = GetMapTop() + GetTileHeight() * m_iMapHeight;
	size_t h2 = GetMapBottom();
}

void CMapEditor::RenderZeroPoint(const HDC & _hdc, CCamera2D * _pCamera)
{
	pair<float, float> pairScreenPoint = _pCamera->GetScreenPoint(0.f, 0.f);

	Ellipse(_hdc, 
		pairScreenPoint.first - (GetTileWidth() >> 2) * _pCamera->GetZoomMultiple(),
		pairScreenPoint.second - (GetTileHeight() >> 2) * _pCamera->GetZoomMultiple(),
		pairScreenPoint.first + (GetTileWidth() >> 2) * _pCamera->GetZoomMultiple(),
		pairScreenPoint.second + (GetTileHeight() >> 2) * _pCamera->GetZoomMultiple());
}

void CMapEditor::ChangeMapWidth(void* _pDeltaWidth)
{
	int iDeltaWidth = *static_cast<int*>(_pDeltaWidth);
	SetMapWidth(GetMapWidth() + iDeltaWidth);
}

void CMapEditor::ChangeMapHeight(void* _pDeltaHeight)
{
	int iDeltaHeight = *static_cast<int*>(_pDeltaHeight);
	SetMapHeight(GetMapHeight() + iDeltaHeight);
}
