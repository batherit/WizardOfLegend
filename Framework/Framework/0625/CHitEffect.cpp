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

	// �׸� ������ ��ũ�� ��ǥ�� ��ȯ�Ѵ�.
	const pair<int, int>& pairLeftTop = _pCamera->GetScreenPoint(rcDrawArea.left, rcDrawArea.top);
	const pair<int, int>& pairRightBottom = _pCamera->GetScreenPoint(rcDrawArea.right, rcDrawArea.bottom);

	RECT rcCollider = { pairLeftTop.first, pairLeftTop.second, pairRightBottom.first, pairRightBottom.second };
	if (!IsCollided(GetGameWorld().GetViewSpace()->GetRect(), rcCollider)) return;

	GdiTransparentBlt(_hdc,
		pairLeftTop.first,			// ��� ������ǥ X
		pairLeftTop.second,			// ��� ������ǥ Y
		pairRightBottom.first - pairLeftTop.first,					// ��� ũ�� (1�� �󿩹��� ���ֱ� ���� �߰� �ȼ��̴�.)
		pairRightBottom.second - pairLeftTop.second,				// ��� ũ�� (1�� �󿩹��� ���ֱ� ���� �߰� �ȼ��̴�.)
		m_hDCKeyAtlas,
		GetAnimX(),
		GetAnimY(),
		m_iWidth,
		m_iHeight,
		RGB(255, 250, 255));
	g_iRenderCount++;
}
