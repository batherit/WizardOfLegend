#include "stdafx.h"
#include "CMonsterSpawner.h"
#include "CBitmapMgr.h"
#include "CCamera2D.h"
#include "CMonster_SwordMan.h"
#include "CWOL_World.h"
#include "CSpace.h"



CMonsterSpawner::CMonsterSpawner(CGameWorld & _rGameWorld, list<CObj*>& _listMonsters, float _fX, float _fY, SPAWN::E_TYPE _eType, int _iGroupID)
	:
	CSpawner(_rGameWorld, _fX, _fY),
	m_listMonsters(_listMonsters),
	m_eType(_eType)
{
	m_iGroupID = _iGroupID;

	_anim_info stAnimInfo;
	stAnimInfo.iState = 0;
	switch (_eType) {
	case SPAWN::TYPE_SWORDMAN:
		m_hDCSpawnSprite = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("SPAWN_CARD_SWORDMAN"));
		stAnimInfo.iCountToRepeat = 1;
		stAnimInfo.fTotalTime = 1.2f;
		stAnimInfo.iStartFrameIndex = 0;
		stAnimInfo.iFrameCount = 32;
		SetWidth(150); SetHeight(230);
		break;
	case SPAWN::TYPE_ARCHER:
		m_hDCSpawnSprite = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("SPAWN_CARD_ARCHER"));
		stAnimInfo.iCountToRepeat = 1;
		stAnimInfo.fTotalTime = 1.2f;
		stAnimInfo.iStartFrameIndex = 0;
		stAnimInfo.iFrameCount = 32;
		SetWidth(150); SetHeight(230);
		break;
	case SPAWN::TYPE_WIZARD:
		m_hDCSpawnSprite = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("SPAWN_CARD_WIZARD"));
		stAnimInfo.iCountToRepeat = 1;
		stAnimInfo.fTotalTime = 1.2f;
		stAnimInfo.iStartFrameIndex = 0;
		stAnimInfo.iFrameCount = 32;
		SetWidth(150); SetHeight(230);
		break;
	case SPAWN::TYPE_WIZARDBALL:
		m_hDCSpawnSprite = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("SPAWN_CARD_WIZARDBALL"));
		stAnimInfo.iCountToRepeat = 1;
		stAnimInfo.fTotalTime = 1.2f;
		stAnimInfo.iStartFrameIndex = 0;
		stAnimInfo.iFrameCount = 32;
		SetWidth(150); SetHeight(230);
		break;
	}
	SetNewAnimInfo(stAnimInfo);
}

CMonsterSpawner::~CMonsterSpawner()
{
}

int CMonsterSpawner::Update(float _fDeltaTime)
{
	if (UpdateAnim(_fDeltaTime) == 1) {
		
		SetValid(false);
		return 1;
	}

	if (!m_bIsSpawend && GetAnimProgress() >= 0.875f) {
		switch (m_eType) {
		case SPAWN::TYPE_SWORDMAN:
			m_listMonsters.emplace_back(new CMonster_SwordMan(GetGameWorld(), GetX(), GetY(), m_iGroupID, TO_WOL(GetGameWorld()).GetPlayer()));
			break;
		case SPAWN::TYPE_ARCHER:

			break;
		case SPAWN::TYPE_WIZARD:

			break;
		case SPAWN::TYPE_WIZARDBALL:

			break;
		}
		m_bIsSpawend = true;
	}
	return 0;
}

void CMonsterSpawner::Render(HDC & _hdc, CCamera2D * _pCamera)
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
