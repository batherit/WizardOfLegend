#pragma once
#include "CGameWorld.h"

class CCamera2D;
class CMapEditor;

class CMapTool :
	public CGameWorld
{
public:
	CMapTool();
	~CMapTool();

public:
	// CGameWorld을(를) 통해 상속됨
	virtual LRESULT OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) override;
	void Ready(void);
	void Update(void);
	void LateUpdate(void);
	void Render(void);
	void Release(void);

public:
	//const CObj* GetViewSpace(void) const { return m_pViewSpace; }
	CCamera2D* GetCamera(void) { return m_pCamera; }
	CMapEditor* GetMapEditor(void) { return m_pMapEditor; }

private:
	CCamera2D* m_pCamera;
	//CObj* m_pViewSpace;
	CMapEditor* m_pMapEditor;
};

