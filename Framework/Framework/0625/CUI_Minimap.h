#pragma once
#include "CObj.h"
class CUI_Minimap :
	public CObj
{
public:
	CUI_Minimap(CGameWorld& _rGameWorld, CMapLoader* _pMapLoader, CObj* _pPicturePointObj);
	virtual ~CUI_Minimap();

	virtual int Update(float _fDeltaTime) override;
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera) override;
	void ToggleMinimap(void);

public:
	CCamera2D* m_pCamera = nullptr;
	CMapLoader* m_pMapLoader = nullptr;
	HDC m_hDCPlayerPos = nullptr;
};

