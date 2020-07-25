#include "stdafx.h"
#include "CUI_Minimap.h"
#include "CCamera2D.h"
#include "CMapLoader.h"
#include "CMapObjsGroup.h"
#include "CBitmapMgr.h"



CUI_Minimap::CUI_Minimap(CGameWorld& _rGameWorld, CMapLoader* _pMapLoader, CObj* _pPicturePointObj)
	:
	CObj(_rGameWorld, 250.f, 300.f, WINCX * 0.2f, WINCY * 0.2f),
	m_pMapLoader(_pMapLoader),
	m_pCamera(new CCamera2D(_rGameWorld, _pPicturePointObj))
{
	m_pCamera->SetZoomMultiple(0.08f);
	m_hDCPlayerPos = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("UI_PLAYER_MINIMAP"));
}

CUI_Minimap::~CUI_Minimap()
{
	DeleteSafe(m_pCamera);
}

int CUI_Minimap::Update(float _fDeltaTime)
{
	if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_M))
		ToggleMinimap();
	return 0;
}

void CUI_Minimap::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	if (IsVisible()) {
		m_pCamera->AdjustPosToOwner();

		size_t iOffsetX = m_fX - (WINCX >> 1);
		size_t iOffsetY = m_fY - (WINCY >> 1);

		for (auto& pGroup : m_pMapLoader->GetCollidersGroups()) {
			for (auto& pObj : pGroup->GetMapObjs()) {
				RECT& rcDrawArea = pObj->GetRect();

				// 그릴 영역을 스크린 좌표로 변환한다.
				const pair<int, int>& pairLeftTop = m_pCamera->GetScreenPoint(rcDrawArea.left, rcDrawArea.top);
				const pair<int, int>& pairRightBottom = m_pCamera->GetScreenPoint(rcDrawArea.right, rcDrawArea.bottom);

				RECT rcCollider = { pairLeftTop.first + iOffsetX, pairLeftTop.second + iOffsetY, pairRightBottom.first + iOffsetX, pairRightBottom.second + iOffsetY };
				if (!IsCollided(GetRect(), rcCollider)) continue;

				HBRUSH hBrush = CreateSolidBrush(RGB(122, 122, 122));
				HBRUSH hOldBrush = (HBRUSH)SelectObject(_hdc, hBrush);

				Rectangle(_hdc, rcCollider.left, rcCollider.top, rcCollider.right, rcCollider.bottom);

				SelectObject(_hdc, hOldBrush);
				DeleteObject(hBrush);

				GdiTransparentBlt(
					_hdc,
					m_fX - (UI_PLAYER_MINIMAP_WIDHT >> 1),
					m_fY - (UI_PLAYER_MINIMAP_HEIGHT >> 1),
					UI_PLAYER_MINIMAP_WIDHT,
					UI_PLAYER_MINIMAP_HEIGHT,
					m_hDCPlayerPos,
					0, 0,
					UI_PLAYER_MINIMAP_WIDHT,
					UI_PLAYER_MINIMAP_HEIGHT,
					RGB(255, 0, 255));

				g_iRenderCount++;
			}
		}
	}
}

void CUI_Minimap::ToggleMinimap(void)
{
	if (IsVisible()) SetVisible(false);
	else SetVisible(true);
}
