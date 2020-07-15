#pragma once
#include "CGameWorld.h"

class CCamera2D;
class CMapLoader;

class CWOL_World :
	public CGameWorld
{
public:
	CWOL_World();
	~CWOL_World();

	// CGameWorld��(��) ���� ��ӵ�
	virtual LRESULT OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) override;
	virtual void Ready(void) override;
	virtual void Update(void) override;
	virtual void LateUpdate(void) override;
	virtual void Render(void) override;
	virtual void Release(void) override;

private:
	CCamera2D* m_pCamera;
	CMapLoader* m_pMap;
};

