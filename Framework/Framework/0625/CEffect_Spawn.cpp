#include "stdafx.h"
#include "CEffect_Spawn.h"
#include "CBitmapMgr.h"
#include "CCamera2D.h"



CEffect_Spawn::CEffect_Spawn(CGameWorld& _rGameWorld, CObj* _pOwner, EFFECT_SPAWN::E_TYPE _eType)
	:
	m_pOwner(_pOwner),
	CObj(_rGameWorld, 0.f, 0.f)
{
	SetX(_pOwner->GetX());
	SetY(_pOwner->GetY());

	_anim_info stAnimInfo;
	stAnimInfo.iState = 0;
	switch (_eType) {
	case EFFECT_SPAWN::EFFECT_SPAWN_PLAYER:
		m_hDCSpawnSprite = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("SPAWN_PLAYER"));
		stAnimInfo.iCountToRepeat = 1;
		stAnimInfo.fTotalTime = 0.5f;
		stAnimInfo.iStartFrameIndex = 0;
		stAnimInfo.iFrameCount = 8;
		SetWidth(225); SetHeight(730);
		SetY(_pOwner->GetY() - 225); // 225�� ������
		break;
	case EFFECT_SPAWN::EFFECT_SPAWN_ARCHER:
		m_hDCSpawnSprite = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("SPAWN_CARD_ARCHER"));
		stAnimInfo.iCountToRepeat = 1;
		stAnimInfo.fTotalTime = 0.5f;
		stAnimInfo.iStartFrameIndex = 0;
		stAnimInfo.iFrameCount = 32;
		SetWidth(150); SetHeight(230);
		break;
	case EFFECT_SPAWN::EFFECT_SPAWN_SWORDMAN:
		m_hDCSpawnSprite = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("SPAWN_CARD_SWORDMAN"));
		stAnimInfo.iCountToRepeat = 1;
		stAnimInfo.fTotalTime = 0.5f;
		stAnimInfo.iStartFrameIndex = 0;
		stAnimInfo.iFrameCount = 32;
		SetWidth(150); SetHeight(230);
		break;
	case EFFECT_SPAWN::EFFECT_SPAWN_WIZARD:
		m_hDCSpawnSprite = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("SPAWN_CARD_WIZARD"));
		stAnimInfo.iCountToRepeat = 1;
		stAnimInfo.fTotalTime = 0.5f;
		stAnimInfo.iStartFrameIndex = 0;
		stAnimInfo.iFrameCount = 32;
		SetWidth(150); SetHeight(230);
		break;
	case EFFECT_SPAWN::EFFECT_SPAWN_WIZARDBALL:
		m_hDCSpawnSprite = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("SPAWN_CARD_WIZARDBALL"));
		stAnimInfo.iCountToRepeat = 1;
		stAnimInfo.fTotalTime = 0.5f;
		stAnimInfo.iStartFrameIndex = 0;
		stAnimInfo.iFrameCount = 32;
		SetWidth(150); SetHeight(230);
		break;
	}
	SetNewAnimInfo(stAnimInfo);
}

CEffect_Spawn::~CEffect_Spawn()
{
}

int CEffect_Spawn::Update(float _fDeltaTime)
{
	return UpdateAnim(_fDeltaTime);
}

void CEffect_Spawn::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	RECT& rcDrawArea = GetRect();

	// �׸� ������ ��ũ�� ��ǥ�� ��ȯ�Ѵ�.
	pair<float, float>& pairLeftTop = _pCamera->GetScreenPoint(rcDrawArea.left, rcDrawArea.top);
	pair<float, float>& pairRightBottom = _pCamera->GetScreenPoint(rcDrawArea.right, rcDrawArea.bottom);

	//RECT rcCollider = { pairLeftTop.first, pairLeftTop.second, pairRightBottom.first, pairRightBottom.second };
	//if (!IsCollided(GetGameWorld().GetViewSpace()->GetRect(), rcCollider)) return;

	GdiTransparentBlt(_hdc,
		pairLeftTop.first,			// ��� ������ǥ X
		pairLeftTop.second,			// ��� ������ǥ Y
		pairRightBottom.first - pairLeftTop.first + 1,					// ��� ũ�� (1�� �󿩹��� ���ֱ� ���� �߰� �ȼ��̴�.)
		pairRightBottom.second - pairLeftTop.second + 1,				// ��� ũ�� (1�� �󿩹��� ���ֱ� ���� �߰� �ȼ��̴�.)
		m_hDCSpawnSprite,
		GetAnimX(),
		GetAnimY(),
		m_iWidth,
		m_iHeight,
		RGB(255, 0, 255));
	g_iRenderCount++;
}
