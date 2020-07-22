#include "stdafx.h"
#include "CCoin.h"
#include "CCamera2D.h"
#include "CSpace.h"
#include "CBitmapMgr.h"
#include "CUI_GoldText.h"




CCoin::CCoin(CGameWorld & _rGameWorld, float _fX, float _fY, int iMoneyAmount)
	:
	CObj(_rGameWorld, _fX, _fY, COIN_OUTPUT_WIDTH, COIN_OUTPUT_HEIGHT)
{
	SetMoney(iMoneyAmount);

	m_hDCKey = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("COIN"));

	_anim_info stAnimInfo;
	if (iMoneyAmount <= 5) {
		stAnimInfo.iState = 1;
	}
	else if (iMoneyAmount <= 10) {
		stAnimInfo.iState = 2;
	}
	else stAnimInfo.iState = 0;
	stAnimInfo.fTotalTime = GetNumberMinBetweenMax(0.5f, 1.0f);
	stAnimInfo.iCountToRepeat = 0;
	stAnimInfo.iFrameCount = 2;
	stAnimInfo.iStartFrameIndex = 0;
	SetNewAnimInfo(stAnimInfo);

	m_pPlayer = TO_WOL(GetGameWorld()).GetPlayer();
}

CCoin::~CCoin()
{
}

int CCoin::Update(float _fDeltaTime)
{
	if (!m_pPlayer) {
		SetValid(false);
		return 1;
	}

	UpdateAnim(_fDeltaTime);
	if ((m_fElapsedTime += _fDeltaTime) > 1.f) {
		float fT = (m_fElapsedTime - 1.f) / 0.5f;	// 0.5�ʸ��� �÷��̾�� ����
		if (fT < 1.f) {
			POINT ptCoinPos;
			ptCoinPos.x = GetX() * (1.f - fT) + m_pPlayer->GetX() * fT;
			ptCoinPos.y = GetY() * (1.f - fT) + m_pPlayer->GetY() * fT;
			SetXY(ptCoinPos.x, ptCoinPos.y);
		}
		else {
			m_pPlayer->SetMoney(m_pPlayer->GetMoney() + m_iMoney);
			TO_WOL(GetGameWorld()).GetListUIs().emplace_back(
				new CUI_GoldText(
					GetGameWorld(),
					GetX() + GetNumberMinBetweenMax(-10.f, 10.f),
					GetY() + GetNumberMinBetweenMax(-10.f, 10.f),
					GetMoney())
			);
			SetValid(false);
			return 1;
		}
 	}
	return 0;
}

void CCoin::Render(HDC & _hdc, CCamera2D * _pCamera)
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
		m_hDCKey,
		GetAnimX(),
		GetAnimY(),
		m_iWidth,
		m_iHeight,
		RGB(255, 0, 255));
	g_iRenderCount++;
}
