#include "stdafx.h"
#include "CMonsterSpawner.h"
#include "CBitmapMgr.h"
#include "CCamera2D.h"
#include "CMonster_SwordMan.h"
#include "CMonster_Archer.h"
#include "CMonster_Wizard.h"
#include "CMonster_WizardBall.h"
#include "CWOL_World.h"
#include "CSpace.h"
#include "CSpawnerGenerator.h"




CMonsterSpawner::CMonsterSpawner(CGameWorld & _rGameWorld, list<CObj*>& _listMonsters, float _fTimeToDelay, float _fX, float _fY, SPAWN::E_TYPE _eType, int _iGroupID, CSpawnerGenerator* _pSpawnerGenerator)
	:
	m_fTimeToDelay(_fTimeToDelay),
	CSpawner(_rGameWorld, _fX, _fY),
	m_listMonsters(_listMonsters),
	m_eType(_eType),
	m_pSpawnerGenerator(_pSpawnerGenerator)
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
	if ((m_fElapsedTime += _fDeltaTime) >= m_fTimeToDelay) {
		if (UpdateAnim(_fDeltaTime) == 1) {

			SetValid(false);
			return 1;
		}

		if (!m_bIsSpawend && GetAnimProgress() >= 0.875f) {
			switch (m_eType) {
			case SPAWN::TYPE_SWORDMAN:
				m_listMonsters.emplace_back(new CMonster_SwordMan(GetGameWorld(), GetX(), GetY(), m_iGroupID, TO_WOL(GetGameWorld()).GetPlayer(), m_pSpawnerGenerator));
				break;
			case SPAWN::TYPE_ARCHER:
				m_listMonsters.emplace_back(new CMonster_Archer(GetGameWorld(), GetX(), GetY(), m_iGroupID, TO_WOL(GetGameWorld()).GetPlayer(), m_pSpawnerGenerator));
				break;
			case SPAWN::TYPE_WIZARD:
				m_listMonsters.emplace_back(new CMonster_Wizard(GetGameWorld(), GetX(), GetY(), m_iGroupID, TO_WOL(GetGameWorld()).GetPlayer(), m_pSpawnerGenerator));
				break;
			case SPAWN::TYPE_WIZARDBALL:
				m_listMonsters.emplace_back(new CMonster_WizardBall(GetGameWorld(), GetX(), GetY(), m_iGroupID, TO_WOL(GetGameWorld()).GetPlayer(), m_pSpawnerGenerator));
				break;
			}
			m_bIsSpawend = true;
		}
	}
	return 0;
}

void CMonsterSpawner::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	if (m_fElapsedTime < m_fTimeToDelay) return;
	RECT& rcDrawArea = GetRect();

	// 그릴 영역을 스크린 좌표로 변환한다.
	pair<float, float>& pairLeftTop = _pCamera->GetScreenPoint(rcDrawArea.left, rcDrawArea.top);
	pair<float, float>& pairRightBottom = _pCamera->GetScreenPoint(rcDrawArea.right, rcDrawArea.bottom);

	RECT rcCollider = { pairLeftTop.first, pairLeftTop.second, pairRightBottom.first, pairRightBottom.second };
	if (!IsCollided(GetGameWorld().GetViewSpace()->GetRect(), rcCollider)) return;

	GdiTransparentBlt(_hdc,
		pairLeftTop.first,			// 출력 시작좌표 X
		pairLeftTop.second,			// 출력 시작좌표 Y
		pairRightBottom.first - pairLeftTop.first + 1,					// 출력 크기 (1은 빈여백을 없애기 위한 추가 픽셀이다.)
		pairRightBottom.second - pairLeftTop.second + 1,				// 출력 크기 (1은 빈여백을 없애기 위한 추가 픽셀이다.)
		m_hDCSpawnSprite,
		GetAnimX(),
		GetAnimY(),
		m_iWidth,
		m_iHeight,
		RGB(255, 0, 255));
	g_iRenderCount++;
}
