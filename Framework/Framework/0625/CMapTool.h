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
	void Ready(void);
	void Update(void);
	void LateUpdate(void);
	void Render(void);
	void Release(void);

public:
	//const CObj* GetViewSpace(void) const { return m_pViewSpace; }

private:
	CCamera2D* m_pCamera;
	//CObj* m_pViewSpace;
	CMapEditor* m_pMapEditor;

};

