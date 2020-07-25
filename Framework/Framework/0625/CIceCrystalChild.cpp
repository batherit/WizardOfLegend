#include "stdafx.h"
#include "CIceCrystalChild.h"
#include "CBitmapMgr.h"
#include "CCamera2D.h"
#include "CSpace.h"
#include "CIceCrystal.h"



CIceCrystalChild::CIceCrystalChild(CGameWorld & _rGameWorld, CIceCrystal* _pIceCrystalParent)
	:
	CObj(_rGameWorld, 0.f, 0.f, ICE_CRYSTAL_WIDTH, ICE_CRYSTAL_HEIGHT),
	m_pIceCrystalParent(_pIceCrystalParent)
{
	m_hDCKeyAtlas = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("SKILL_ICE_CRYSTAL"));

	m_pColliders[COLLIDER::TYPE_DAMAGED] = this;
}

CIceCrystalChild::~CIceCrystalChild()
{
	Release();
}

int CIceCrystalChild::Update(float _fDeltaTime)
{
	// 크리스탈 자식 위치는 크리스탈 부모가 결정해줌
	return 0;
}

void CIceCrystalChild::LateUpdate(void)
{
	UpdateCollidedObjs();
	//m_pColliders[COLLIDER::TYPE_DAMAGED]->LateUpdate();
}

void CIceCrystalChild::Render(HDC & _hdc, CCamera2D * _pCamera)
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
		GetSpriteIndex() * ICE_CRYSTAL_WIDTH,
		0,
		m_iWidth,
		m_iHeight,
		RGB(200, 230, 250));
}

void CIceCrystalChild::Release(void)
{
	m_pIceCrystalParent = nullptr;
}

int CIceCrystalChild::GetSpriteIndex(void)
{
	// 프레임인덱스는 부모 회전각에 
	return m_pIceCrystalParent->GetDegree() / 20.f; // 18등분
}
