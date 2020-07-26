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
	SetDamage(12);
	SetDamageOffset(3);

	m_hDCKeyAtlas = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("SKILL_ICE_CRYSTAL"));

	m_pColliders[COLLIDER::TYPE_DAMAGED] = this;
}

CIceCrystalChild::~CIceCrystalChild()
{
	Release();
}

int CIceCrystalChild::Update(float _fDeltaTime)
{
	// ũ����Ż �ڽ� ��ġ�� ũ����Ż �θ� ��������
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
	// �������ε����� �θ� ȸ������ 
	return m_pIceCrystalParent->GetDegree() / 20.f; // 18���
}
