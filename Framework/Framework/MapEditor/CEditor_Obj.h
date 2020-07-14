#pragma once
class CEditor_Obj
{
public:
	CEditor_Obj(const _map_render_info& _rMapRenderInfo, int _iPivotRow, int _iPivotCol, MAP_OBJ::E_TYPE _eObjType); // 외부 생성용
	CEditor_Obj(const _map_render_info& _rMapRenderInfo); // 저장 파일 로드용
	virtual ~CEditor_Obj();

public:
	void SetPivotPoint(int _iRow, int _iCol) { m_stPivotPoint.iRow = _iRow; m_stPivotPoint.iCol = _iCol; }
	virtual RECT GetRect(void) const = 0;
	virtual RECT GetRowColRect(void) const = 0;
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera) = 0;
	virtual void SaveInfo(FILE * _fpOut);
	virtual void LoadInfo(FILE * _fpIn);

protected:
	MAP_OBJ::E_TYPE m_eObjType = MAP_OBJ::TYPE_END;
	const _map_render_info& m_rMapRenderInfo;
	_pivot_point m_stPivotPoint;
};

