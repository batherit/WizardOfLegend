#pragma once
#include "CTileMapObj.h"
class CCollider :
	public CTileMapObj
{
public:
	CCollider(const _map_render_info & _rMapRenderInfo, int _iPivotRow, int _iPivotCol);
	~CCollider();

public:
	// CTileMapObj��(��) ���� ��ӵ�
	virtual RECT GetRect(void) const override;
	virtual RECT GetRowColRect(void) const override;
	virtual void Render(HDC & _hdc, CCamera2D * _pCamera) override;
};
