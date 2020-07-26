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

			// �׸� ������ ��ũ�� ��ǥ�� ��ȯ�Ѵ�.
			const pair<int, int>& pairLeftTop = _pCamera->GetScreenPoint(rcDrawArea.left, rcDrawArea.top);
			const pair<int, int>& pairRightBottom = _pCamera->GetScreenPoint(rcDrawArea.right, rcDrawArea.bottom);

			//RECT rcCollider = { pairLeftTop.first, pairLeftTop.second, pairRightBottom.first, pairRightBottom.second };
			//if (!IsCollided(GetGameWorld().GetViewSpace()->GetRect(), rcCollider)) return;

			GdiTransparentBlt(_hdc,
				pairLeftTop.first,			// ��� ������ǥ X
				pairLeftTop.second,			// ��� ������ǥ Y
				pairRightBottom.first - pairLeftTop.first,					// ��� ũ�� (1�� �󿩹��� ���ֱ� ���� �߰� �ȼ��̴�.)
				pairRightBottom.second - pairLeftTop.second,				// ��� ũ�� (1�� �󿩹��� ���ֱ� ���� �߰� �ȼ��̴�.)
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
