#include "stdafx.h"
#include "CWizardFire.h"
#include "CBitmapMgr.h"
#include "CCamera2D.h"
#include "CSpace.h"



CWizardFire::CWizardFire(CGameWorld & _rGameWorld, float _fX, float _fY, float _fToX, float _fToY)
	:
	CObj(_rGameWorld, _fX, _fY, WIZARD_FIRE_WIDTH, WIZARD_FIRE_HEIGHT, _fToX, _fToY, WIZARD_FIRE_SPEED)
{
	m_hDCKeyAtlas = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("WIZARD_FIRE"));

	m_pColliders[COLLIDER::TYPE_WALL] = this;
	m_pColliders[COLLIDER::TYPE_DAMAGED] = this;

	_anim_info stAnimInfo;
	stAnimInfo.iState = GetAnimState(_fToX, _fToY);
	stAnimInfo.iCountToRepeat = 0;	// ���� �ݺ�
	stAnimInfo.fTotalTime = 2.f;	// 2�� ��ȸ
	stAnimInfo.iStartFrameIndex =0;
	stAnimInfo.iFrameCount = 6;

	SetNewAnimInfo(stAnimInfo);
}

CWizardFire::~CWizardFire()
{
	Release();
}

int CWizardFire::Update(float _fDeltaTime)
{
	MoveByDeltaTime(_fDeltaTime);
	
	return UpdateAnim(_fDeltaTime);
}

void CWizardFire::LateUpdate(void)
{
	if (m_listCollidedObjs.size() > 0) {
		SetValid(false);
	}
}

void CWizardFire::Render(HDC & _hdc, CCamera2D * _pCamera)
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
		RGB(110, 70, 210));
	g_iRenderCount++;
}


void CWizardFire::Release(void)
{
}

int CWizardFire::GetAnimState(float _fToX, float _fToY) const
{
	float fDegree = GetPositiveDegreeByVector(_fToX, _fToY);

	if (191.25f <= fDegree && fDegree < 213.75f) return 0;
	else if (213.75f <= fDegree && fDegree < 236.25f) return 1;
	else if (236.25f <= fDegree && fDegree < 258.75f) return 2;
	else if (258.75f <= fDegree && fDegree < 281.25f) return 3;
	else if (281.25f <= fDegree && fDegree < 303.75f) return 4;
	else if (303.75f <= fDegree && fDegree < 326.25f) return 5;
	else if (326.25f <= fDegree && fDegree < 348.75f) return 6;
	else if (11.25f <= fDegree && fDegree < 33.75f) return 8;
	else if (33.75f <= fDegree && fDegree < 56.25f) return 9;
	else if (56.25f <= fDegree && fDegree < 78.75f) return 10;
	else if (78.75f <= fDegree && fDegree < 101.25f) return 11;
	else if (101.25f <= fDegree && fDegree < 123.75f) return 12;
	else if (123.75f <= fDegree && fDegree < 146.25f) return 13;
	else if (146.25f <= fDegree && fDegree < 168.75f) return 14;
	else if (168.75f <= fDegree && fDegree < 191.25f) return 15;
	else return 7;
}
