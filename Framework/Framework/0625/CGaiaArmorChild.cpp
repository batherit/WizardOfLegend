#include "stdafx.h"
#include "CGaiaArmorChild.h"
#include "CBitmapMgr.h"
#include "CCamera2D.h"
#include "CScene.h"
#include "CGaiaArmor.h"
#include "CHitEffect.h"
#include "CSpace.h"



CGaiaArmorChild::CGaiaArmorChild(CGameWorld & _rGameWorld, CGaiaArmor * _pGaiaArmorParent)
	:
	CObj(_rGameWorld, 0.f, 0.f, GAIA_ARMOR_WIDTH, GAIA_ARMOR_HEIGHT),
	m_pGaiaArmorParent(_pGaiaArmorParent)
{
	SetRenderLayer(1);
	SetObjType(OBJ::TYPE_PLAYER_SKILL);
	SetDamage(3);
	SetDamageOffset(1);

	m_hDCKeyAtlas = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("SKILL_GAIA_ARMOR"));

	m_pColliders[COLLIDER::TYPE_DAMAGED] = this;
}

CGaiaArmorChild::~CGaiaArmorChild()
{
	Release();
}

int CGaiaArmorChild::Update(float _fDeltaTime)
{
	// ���̾� �Ƹ� �ڽ� ��ġ�� ���̾� �Ƹ� �θ� ��������
	return 0;
}

void CGaiaArmorChild::LateUpdate(void)
{
}

void CGaiaArmorChild::Render(HDC & _hdc, CCamera2D * _pCamera)
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
		GetSpriteIndex() * GAIA_ARMOR_WIDTH,
		0,
		m_iWidth,
		m_iHeight,
		RGB(255, 0, 255));
}

void CGaiaArmorChild::Release(void)
{
}

int CGaiaArmorChild::GetSpriteIndex(void)
{
	float fToX = GetX() - m_pGaiaArmorParent->GetX();
	float fToY = GetY() - m_pGaiaArmorParent->GetY();
	float fDegree = GetPositiveDegreeByVector(fToX, fToY);

	if (75.f <= fDegree && fDegree < 105.f) return 0;
	else if (45.f <= fDegree && fDegree < 75.f) return 1;
	else if (15.f <= fDegree && fDegree < 45.f) return 2;
	else if (315.f <= fDegree && fDegree < 345.f) return 4;
	else if (285.f <= fDegree && fDegree < 315.f) return 5;
	else if (255.f <= fDegree && fDegree < 285.f) return 6;
	else if (225.f <= fDegree && fDegree < 255.f) return 7;
	else if (195.f <= fDegree && fDegree < 225.f) return 8;
	else if (165.f <= fDegree && fDegree < 195.f) return 9;
	else if (135.f <= fDegree && fDegree < 165.f) return 10;
	else if (105.f <= fDegree && fDegree < 135.f) return 11;
	else return 3;
}
