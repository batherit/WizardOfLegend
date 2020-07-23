#include "stdafx.h"
#include "CUI_SkillKeyBinding.h"
#include "CBitmapMgr.h"
#include "CUI_SkillKeyButton.h"
#include "CUI_Image.h"
#include "CPlayerWOL.h"
#include "CState.h"




CUI_SkillKeyBinding::CUI_SkillKeyBinding(CGameWorld & _rGameWorld, CObj * _pPlayer)
	:
	CObj(_rGameWorld, WINCX - (UI_SKILLKEY_BINDING_MENU_WIDTH >> 1) - 50 , WINCY >> 1, UI_SKILLKEY_BINDING_MENU_WIDTH, UI_SKILLKEY_BINDING_MENU_HEIGHT),
	m_pPlayer(_pPlayer)
{
	m_hDCSkillKeyBinding = CBitmapMgr::GetInstance()->GetBitmapMemDC(TEXT("INVENTORY"));
	SetVisible(false);
	// 버튼키 바인딩 해줄 것
	// 플레이어의 스킬바와 동기화
	for (int i = 0; i < SKILL::KEY_END; i++) {
		// 77 => 슬롯 간 간격
		m_pSkillKeyButtons[i] = new CUI_SkillKeyButton(_rGameWorld, this, GetRect().left + 97 + 77 * i, GetRect().top + 112, TO_PLAYER_WOL(_pPlayer), static_cast<SKILL::E_KEY>(i));
	}
	m_pDesc = new CUI_Image(_rGameWorld, GetRect().left + 289, GetRect().top + 677, UI_SKILLKEY_BINDING_DESC_WIDTH, UI_SKILLKEY_BINDING_DESC_HEIGHT);
}

CUI_SkillKeyBinding::~CUI_SkillKeyBinding()
{
	Release();
}

int CUI_SkillKeyBinding::Update(float _fDeltaTime)
{
	if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_TAB)) {
		if (IsVisible()) {
			m_pDesc->Clear();
			m_iSkillKeyToSwapIndex = 0;
			SetVisible(false);
		}
		else {
			SetVisible(true);
		}
	}

	if (!IsVisible()) return 0;

	// TODO : 버튼 업데이트
	for (int i = 0; i < SKILL::KEY_END; i++) {
		m_pSkillKeyButtons[i]->Update(_fDeltaTime);
	}
	return 0;
}

void CUI_SkillKeyBinding::Render(HDC & _hdc, CCamera2D * _pCamera)
{
	if (IsVisible()) {
		RECT& rcDrawArea = GetRect();

		BitBlt(_hdc,
			rcDrawArea.left,			// 출력 시작좌표 X
			rcDrawArea.top,			// 출력 시작좌표 Y
			rcDrawArea.right - rcDrawArea.left + 1,
			rcDrawArea.bottom - rcDrawArea.top + 1,
			m_hDCSkillKeyBinding,
			0,
			0,
			SRCCOPY);
			//m_iWidth,
			//m_iHeight,
			//RGB(255, 0, 255));

		for (int i = 0; i < SKILL::KEY_END; i++) {
			m_pSkillKeyButtons[i]->Render(_hdc, _pCamera);
		}

		m_pDesc->Render(_hdc, _pCamera);
	}
}

void CUI_SkillKeyBinding::Release(void)
{
	m_pPlayer = nullptr;
	for (int i = 0; i < SKILL::KEY_END; i++) {
		DeleteSafe(m_pSkillKeyButtons[i]);
	}
	DeleteSafe(m_pDesc);
}

int CUI_SkillKeyBinding::UpdateSkillInfo(SKILL::E_KEY _eSkillKeyType)
{
	CState<CPlayerWOL>* pSkillState = TO_PLAYER_WOL(m_pPlayer)->GetSkill(_eSkillKeyType);
	if (pSkillState) {
		m_pDesc->SetHDC(pSkillState->GetStateHDC(STATE_HDC::STATE_HDC_DESC));
	}
	else {
		m_pDesc->Clear();
	}
	return 0;
}

void CUI_SkillKeyBinding::SelectButton(SKILL::E_KEY _eSkillKeyType)
{
	m_eSkillKeyToSwap[m_iSkillKeyToSwapIndex] = _eSkillKeyType;
	if (++m_iSkillKeyToSwapIndex == 2) {
		TO_PLAYER_WOL(m_pPlayer)->SwapSkillKey(m_eSkillKeyToSwap[0], m_eSkillKeyToSwap[1]);
		m_iSkillKeyToSwapIndex = 0;
	}
}
