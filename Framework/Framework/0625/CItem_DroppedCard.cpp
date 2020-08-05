#include "stdafx.h"
#include "CItem_DroppedCard.h"
#include "CWOL_World.h"
#include "CState.h"
#include "CPlayerWOL.h"
#include "CCamera2D.h"
#include "CSpace.h"



CItem_DroppedCard::CItem_DroppedCard(CGameWorld & _rGameWorld, float _fX, float _fY, CState<CPlayerWOL>* _pSkillState)
	:
	CObj(_rGameWorld, _fX, _fY, ITEM_DROPPED_CARD_WIDTH, ITEM_DROPPED_CARD_HEIGHT),
	m_pSkillState(_pSkillState)
{
	SetRenderLayer(1);
	m_hDCKey = _pSkillState->GetStateHDC(STATE_HDC::STATE_HDC_CARD);
}

CItem_DroppedCard::~CItem_DroppedCard()
{
	Release();
}

int CItem_DroppedCard::Update(float _fDeltaTime)
{
	CObj* pPlayer = TO_WOL(GetGameWorld()).GetPlayer();
	float fDist = GetVectorLength(pPlayer->GetX() - GetX(), pPlayer->GetY() - GetY());

	
	if (fDist < 200.0f) {
		if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_F)) {
			if (Used()) {
				m_pSkillState = nullptr;
			}
			else {
				DeleteSafe(m_pSkillState);
			}
			CSoundMgr::Get_Instance()->PlaySound(TEXT("GET_SKILL.mp3"), CSoundMgr::UI);
			SetValid(false);
		}
	}
	return 0;
}

void CItem_DroppedCard::Render(HDC & _hdc, CCamera2D * _pCamera)
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
		m_hDCKey,
		0,
		0,
		m_iWidth,
		m_iHeight,
		RGB(255, 0, 255));
	g_iRenderCount++;
}

void CItem_DroppedCard::Release(void)
{
	DeleteSafe(m_pSkillState);
}

bool CItem_DroppedCard::Used(void)
{
	return TO_PLAYER_WOL(TO_WOL(GetGameWorld()).GetPlayer())->AquireSkillState(m_pSkillState);
}
