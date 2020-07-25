#include "stdafx.h"
#include "CArcherArrow.h"
#include "CBitmapMgr.h"
#include "CCamera2D.h"
#include "CSpace.h"
#include "CCollider.h"



CArcherArrow::CArcherArrow(CGameWorld & _rGameWorld, float _fX, float _fY, float _fToX, float _fToY, ARCHER::E_DIRECTION _eArcherDir)
	:
	CObj(_rGameWorld, _fX, _fY, ARCHER_BOW_OUTPUT_WIDTH, ARCHER_BOW_OUTPUT_HEIGHT, _fToX, _fToY, ARCHER_ARROW_SPEED),
	m_eArcherDir(_eArcherDir)
{
	m_hDCKeyAtlas[ARCHER::DIR_LEFT] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("ARCHER_BOW_LEFT"));
	m_hDCKeyAtlas[ARCHER::DIR_RIGHT] = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("ARCHER_BOW_RIGHT"));

	m_pColliders[COLLIDER::TYPE_WALL] = new CCollider(_rGameWorld, this, 0.f, 0.f, 40.f, 40.f);
	m_pColliders[COLLIDER::TYPE_DAMAGED] = m_pColliders[COLLIDER::TYPE_WALL];

	_anim_info stAnimInfo;
	stAnimInfo.iState = GetAnimState(_fToX, _fToY);
	stAnimInfo.iCountToRepeat = 1;
	stAnimInfo.fTotalTime = 5.f;	// 5초후 사라지도록 코딩!
	stAnimInfo.iStartFrameIndex = 4;
	stAnimInfo.iFrameCount = 1;

	SetNewAnimInfo(stAnimInfo);
}

CArcherArrow::~CArcherArrow()
{
	Release();
}

int CArcherArrow::Update(float _fDeltaTime)
{
	MoveByDeltaTime(_fDeltaTime);
	if (UpdateAnim(_fDeltaTime) == 1) {
		SetValid(false);
		return 1;
	}
	return 0;
}

void CArcherArrow::LateUpdate(void)
{
	UpdateCollidedObjs();
	m_pColliders[COLLIDER::TYPE_WALL]->LateUpdate();
}

void CArcherArrow::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	RECT& rcDrawArea = GetRect();

	// 그릴 영역을 스크린 좌표로 변환한다.
	pair<float, float>& pairLeftTop = _pCamera->GetScreenPoint(rcDrawArea.left, rcDrawArea.top);
	pair<float, float>& pairRightBottom = _pCamera->GetScreenPoint(rcDrawArea.right, rcDrawArea.bottom);

	RECT rcCollider = { pairLeftTop.first, pairLeftTop.second, pairRightBottom.first, pairRightBottom.second };
	if (!IsCollided(GetGameWorld().GetViewSpace()->GetRect(), rcCollider)) return;

	GdiTransparentBlt(_hdc,
		pairLeftTop.first,			// 출력 시작좌표 X
		pairLeftTop.second,			// 출력 시작좌표 Y
		pairRightBottom.first - pairLeftTop.first,					// 출력 크기 (1은 빈여백을 없애기 위한 추가 픽셀이다.)
		pairRightBottom.second - pairLeftTop.second,				// 출력 크기 (1은 빈여백을 없애기 위한 추가 픽셀이다.)
		m_hDCKeyAtlas[m_eArcherDir],
		GetAnimX(),
		GetAnimY(),
		m_iWidth,
		m_iHeight,
		RGB(0, 255, 255));
	g_iRenderCount++;
}

void CArcherArrow::Release(void)
{
	DeleteSafe(m_pColliders[COLLIDER::TYPE_WALL]);
	m_pColliders[COLLIDER::TYPE_DAMAGED] = nullptr;
}

int CArcherArrow::GetAnimState(float _fToX, float _fToY) const
{
	float fDegree = GetPositiveDegreeByVector(_fToX, _fToY);
	// 22.5
	if (258.75f <= fDegree && fDegree < 281.25f)		return 0;
	else if (281.25f <= fDegree && fDegree < 303.75f)	return 1;
	else if (303.75f <= fDegree && fDegree < 326.25f)	return 2;
	else if (326.25f <= fDegree && fDegree < 348.75f)	return 3;
	else if (11.25f <= fDegree && fDegree < 33.75f)		return 5;
	else if (33.75f <= fDegree && fDegree < 56.25f)		return 6;
	else if (56.25f <= fDegree && fDegree < 78.75f)		return 7;
	else if (78.75f <= fDegree && fDegree < 101.25f)	return 8;
	else if (101.25f <= fDegree && fDegree < 123.75f)	return 7;
	else if (123.75f <= fDegree && fDegree < 146.25f)	return 6;
	else if (146.25f <= fDegree && fDegree < 168.75f)	return 5;
	else if (168.75f <= fDegree && fDegree < 191.25f)	return 4;
	else if (191.25f <= fDegree && fDegree < 213.75f)	return 3;
	else if (213.75f <= fDegree && fDegree < 236.25f)	return 2;
	else if (236.25f <= fDegree && fDegree < 258.75f)	return 1;
	return 4;
}
