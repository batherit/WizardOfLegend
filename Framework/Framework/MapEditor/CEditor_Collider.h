#pragma once
#include "CEditor_Obj.h"
class CEditor_Collider :
	public CEditor_Obj
{
public:
	CEditor_Collider(const _map_render_info & _rMapRenderInfo, int _iPivotRow, int _iPivotCol);
	CEditor_Collider(const _map_render_info& _rMapRenderInfo);
	~CEditor_Collider();

public:
	// CTileMapObj을(를) 통해 상속됨
	virtual RECT GetRect(void) const override;
	virtual RECT GetRowColRect(void) const override;
	virtual void Render(HDC & _hdc, CCamera2D * _pCamera) override;
	virtual void SaveInfo(FILE * _fpOut);
	virtual void LoadInfo(FILE * _fpIn);
	virtual void MakeMapData(FILE * _fpOut);
};

