#include "stdafx.h"
#include "CHitEffect.h"
#include "CCamera2D.h"
#include "CSpace.h"
#include "CBitmapMgr.h"



CHitEffect::CHitEffect(CGameWorld & _rGameWorld, float _fX, float _fY)
	:
	CObj(_rGameWorld, _fX, _fY, HIT_EFFECT_OUTPUT_WIDTH, HIT_EFFECT_OUTPUT_HEIGHT)
{
	m_hDCKeyAtlas = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("HIT_EFFECT"));
	_anim_info stAnimInfo;
	stAnimInfo.iState = rand() % 4;
	stAnimInfo.fTotalTime = 0.2f;
	stAnimInfo.iFrameCount = 5;
	stAnimInfo.iCountToRepeat = 1;
	stAnimInfo.iStartFrameIndex = 0;

	SetNewAnimInfo(stAnimInfo);
}

CHitEffect::~CHitEffect()
{
}

int CHitEffect::Update(float _fDeltaTime)
{
	DO_IF_IS_VALID_OBJ(this) {
		if (UpdateAnim(_fDeltaTime) == 1) {
			SetValid(false);
			return 1;
		}
	}
	return 0;
}

void CHitEffect::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	RECT& rcDrawArea = GetRect();

	// 그릴 영역을 스크린 좌표로 변환한다.
	const pair<int, int>& pairLeftTop = _pCamera->GetScreenPoint(rcDrawArea.left, rcDrawArea.top);
	const pair<int, int>& pairRightBottom = _pCamera->GetScreenPoint(rcDrawArea.right, rcDrawArea.bottom);

	RECT rcCollider = { pairLeftTop.first, pairLeftTop.second, pairRightBottom.first, pairRightBottom.second };
	if (!IsCollided(GetGameWorld().GetViewSpace()->GetRect(), rcCollider)) return;

	GdiTransparentBlt(_hdc,
		pairLeftTop.first,			// 출력 시작좌표 X
		pairLeftTop.second,			// 출력 시작좌표 Y
		pairRightBottom.first - pairLeftTop.first,					// 출력 크기 (1은 빈여백을 없애기 위한 추가 픽셀이다.)
		pairRightBottom.second - pairLeftTop.second,				// 출력 크기 (1은 빈여백을 없애기 위한 추가 픽셀이다.)
		m_hDCKeyAtlas,
		GetAnimX(),
		GetAnimY(),
		m_iWidth,
		m_iHeight,
		RGB(255, 250, 255));
	g_iRenderCount++;
}
