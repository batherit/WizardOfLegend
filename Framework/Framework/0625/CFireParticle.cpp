#include "stdafx.h"
#include "CFireParticle.h"
#include "CCamera2D.h"
#include "CSpace.h"
#include "CBitmapMgr.h"


CFireParticle::CFireParticle(CGameWorld & _rGameWorld, float _fX, float _fY)
	:
	CObj(_rGameWorld, _fX, _fY, FIRE_PARTICLE_WIDTH, FIRE_PARTICLE_HEIGHT)
{
	m_hDCKeyAtlas = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("FIRE_PARTICLE"));
	_anim_info stAnimInfo;
	stAnimInfo.iState = rand() % 4;
	stAnimInfo.fTotalTime = 0.5f;
	stAnimInfo.iFrameCount = 5;
	stAnimInfo.iCountToRepeat = 1;
	stAnimInfo.iStartFrameIndex = 0;

	SetNewAnimInfo(stAnimInfo);
}

CFireParticle::~CFireParticle()
{
}

int CFireParticle::Update(float _fDeltaTime)
{
	DO_IF_IS_VALID_OBJ(this) {
		if (UpdateAnim(_fDeltaTime) == 1) {
			SetValid(false);
			return 1;
		}
	}
	return 0;
}

void CFireParticle::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	RECT& rcDrawArea = GetRect();

	// �׸� ������ ��ũ�� ��ǥ�� ��ȯ�Ѵ�.
	pair<float, float>& pairLeftTop = _pCamera->GetScreenPoint(rcDrawArea.left, rcDrawArea.top);
	pair<float, float>& pairRightBottom = _pCamera->GetScreenPoint(rcDrawArea.right, rcDrawArea.bottom);

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
		RGB(255,0 , 255));
	g_iRenderCount++;
}
