#include "stdafx.h"
#include "CBottomHole.h"
#include "CBitmapMgr.h"
#include "CCamera2D.h"
#include "CSpace.h"
#include "CCollider.h"



CBottomHole::CBottomHole(CGameWorld & _rGameWorld, float _fX, float _fY, float _fLifeTime)
	:
	CObj(_rGameWorld, _fX, _fY, BOTTOM_HOLE_WIDTH, BOTTOM_HOLE_HEIGHT),
	m_fLifeTime(_fLifeTime)
{
	SetRenderLayer(0);
	m_hDCKeyAtlas = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("BOTTOM_HOLE"));
	m_pColliders[COLLIDER::TYPE_WALL] = new CCollider(_rGameWorld, this, 0.f, 0.f, BOTTOM_HOLE_WIDTH >> 1, BOTTOM_HOLE_HEIGHT >> 1);
}

CBottomHole::~CBottomHole()
{
	DeleteSafe(m_pColliders[COLLIDER::TYPE_WALL]);
}

int CBottomHole::Update(float _fDeltaTime)
{
	DO_IF_IS_NOT_VALID_OBJ(this) return 1;

	if ((m_fElapsedTime += _fDeltaTime) >= m_fLifeTime) {
		SetValid(false);
		return 1;
	}
	return 0;
}

void CBottomHole::Render(HDC & _hdc, CCamera2D * _pCamera)
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
		m_hDCKeyAtlas,
		0,
		0,
		m_iWidth,
		m_iHeight,
		RGB(255, 0, 255));
	g_iRenderCount++;
}

void CBottomHole::ReactToCollider(CObj * _pCollider, POINT & _ptCollisionPoint, RECT& _rcCollisionRect)
{
	switch (_pCollider->GetObjType()) {
	case COLLIDER::TYPE_WALL:
		SetValid(false);
		break;
	}
}
