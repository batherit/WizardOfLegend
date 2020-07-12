#include "stdafx.h"
#include "CMapEditor.h"
#include "UI_Button.h"


CMapEditor::CMapEditor(CGameWorld& _rGameWorld)
	:
	m_rGameWorld(_rGameWorld)
{
	m_vecEditorButtons.emplace_back(new CUI_Button<CMapEditor>(m_rGameWorld, 0, 0, 100, 50, TEXT("테스트 버튼"), this, &CMapEditor::TestEvent));
}


CMapEditor::~CMapEditor()
{
	Release();
}

void CMapEditor::Update(float _fDeltaTime)
{
	for (auto& pButton : m_vecEditorButtons) {
		pButton->Update(_fDeltaTime);
	}
}

void CMapEditor::Render(const HDC & _hdc, CCamera2D* _pCamera)
{
	for (auto& pButton : m_vecEditorButtons) {
		pButton->Render(_hdc, nullptr);
	}
}

void CMapEditor::Release(void)
{
	for (auto& pButton : m_vecEditorButtons) {
		DeleteSafe(pButton);
	}
	m_vecEditorButtons.clear();
	m_vecEditorButtons.shrink_to_fit();
}

void CMapEditor::TestEvent()
{
	
}
