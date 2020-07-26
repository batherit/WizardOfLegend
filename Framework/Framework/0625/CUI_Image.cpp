#include "stdafx.h"
#include "CUI_Image.h"
#include "CCamera2D.h"
#include "CSpace.h"


CUI_Image::CUI_Image(CGameWorld & _rGameWorld, float _fX, float _fY, size_t _iWidth, size_t _iHeight)
	:
	CObj(_rGameWorld, _fX, _fY, _iWidth, _iHeight)
{
}

CUI_Image::~CUI_Image()
{
}

void CUI_Image::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	if (m_hDCImage) {
		if (m_bIsCameraUsing) {
			RECT& rcDrawArea = GetRect();

			// 그릴 영역을 스크린 좌표로 변환한다.
			const pair<int, int>& pairLeftTop = _pCamera->GetScreenPoint(rcDrawArea.left, rcDrawArea.top);
			const pair<int, int>& pairRightBottom = _pCamera->GetScreenPoint(rcDrawArea.right, rcDrawArea.bottom);

			//RECT rcCollider = { pairLeftTop.first, pairLeftTop.second, pairRightBottom.first, pairRightBottom.second };
			//if (!IsCollided(GetGameWorld().GetViewSpace()->GetRect(), rcCollider)) return;

			GdiTransparentBlt(_hdc,
				pairLeftTop.first,			// 출력 시작좌표 X
				pairLeftTop.second,			// 출력 시작좌표 Y
				pairRightBottom.first - pairLeftTop.first,					// 출력 크기 (1은 빈여백을 없애기 위한 추가 픽셀이다.)
				pairRightBottom.second - pairLeftTop.second,				// 출력 크기 (1은 빈여백을 없애기 위한 추가 픽셀이다.)
				m_hDCImage,
				0,
				0,
				m_iWidth,
				m_iHeight,
				RGB(255, 0, 255));
			g_iRenderCount++;
		}
		else {
			BitBlt(_hdc,
				GetRect().left, GetRect().top,
				GetRect().right - GetRect().left,
				GetRect().bottom - GetRect().top,
				m_hDCImage,
				0, 0, SRCCOPY);
		}
	}
}
