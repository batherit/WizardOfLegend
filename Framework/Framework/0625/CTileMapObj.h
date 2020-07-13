#pragma once
class CTileMapObj
{
public:
	CTileMapObj(const _obj_render_info& _rObjRenderInfo, int _iPivotRow, int _iPivotCol);
	virtual ~CTileMapObj();

public:
	void SetPivotPoint(int _iRow, int _iCol) { m_stPivotPoint.iRow = _iRow; m_stPivotPoint.iCol = _iCol; }
	virtual RECT GetRect(void) const = 0;
	virtual RECT GetRowColRect(void) const = 0;
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera) = 0;

protected:
	const _obj_render_info& m_rObjRenderInfo;
	_pivot_point m_stPivotPoint;
};

