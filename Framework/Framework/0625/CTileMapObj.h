#pragma once
class CTileMapObj
{
public:
	CTileMapObj(const _obj_render_info& _rObjRenderInfo, int _iPivotRow, int _iPivotCol, MAP_OBJ::E_TYPE _eObjType);
	virtual ~CTileMapObj();

public:
	void SetPivotPoint(int _iRow, int _iCol) { m_stPivotPoint.iRow = _iRow; m_stPivotPoint.iCol = _iCol; }
	virtual RECT GetRect(void) const = 0;
	virtual RECT GetRowColRect(void) const = 0;
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera) = 0;

protected:
	MAP_OBJ::E_TYPE m_eObjType = MAP_OBJ::TYPE_END;
	const _obj_render_info& m_rObjRenderInfo;
	_pivot_point m_stPivotPoint;
};

