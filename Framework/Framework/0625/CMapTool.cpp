#include "stdafx.h"
#include "CMapTool.h"
#include "Player.h"
#include "Monster.h"
#include "CSpace.h"
#include "CTimer.h"
#include "CCamera2D.h"
#include "CMapEditor.h"

CMapTool::CMapTool()
	:
	m_pCamera(nullptr),
	m_pMapEditor(nullptr)
	/*,
	m_pViewSpace(nullptr)*/
{
}


CMapTool::~CMapTool()
{
	Release();
}

void CMapTool::Ready(void)
{
	// �������� �ݵ�� �����ֱ�!
	//m_hDC = GetDC(g_hWND);

	srand((time_t)time(nullptr));

	m_pCamera = new CCamera2D(*this, nullptr, WINCX >> 1, WINCY >> 1);
	m_pMapEditor = new CMapEditor(*this);

	//m_pViewSpace = new CSpace(*this, WINCX >> 1, WINCY >> 1, static_cast<int>(WINCX), static_cast<int>(WINCY));
}

void CMapTool::Update(void)
{
	float fDeltaTime = GetTimer()->GetElapsedTimePerFrame();
	m_pCamera->Update(fDeltaTime);
	m_pMapEditor->Update(fDeltaTime);
}

void CMapTool::LateUpdate(void)
{
}

void CMapTool::Render(void)
{
	ClearWindow();

	// TODO: ���⿡ �׸� �׸��� �ڵ带 �ۼ��ϼ���~
	//m_pViewSpace->Render(GetBackbufferDC(), m_pCamera);
	m_pCamera->Render(GetBackbufferDC());
	m_pMapEditor->Render(GetBackbufferDC(), m_pCamera);

	RenderWindow();
}

void CMapTool::Release(void)
{
	DeleteSafe(m_pCamera);
	DeleteSafe(m_pMapEditor);
}

LRESULT CMapTool::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_MOUSEWHEEL:
	{
		int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);

		if (zDelta > 0) {
			m_pCamera->ZoomIn(0.2f);
		}
		else {
			m_pCamera->ZoomOut(0.2f);
		}
		break;
	}
	}
	return 0;
}
