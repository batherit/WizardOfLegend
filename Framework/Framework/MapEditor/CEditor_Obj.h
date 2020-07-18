#pragma once
class CEditor_Obj
{
public:
	CEditor_Obj(CGameWorld& _rGameWorld, const _map_render_info& _rMapRenderInfo, int _iPivotRow, int _iPivotCol, MAP_OBJ::E_TYPE _eObjType); // �ܺ� ������
	CEditor_Obj(CGameWorld& _rGameWorld, const _map_render_info& _rMapRenderInfo); // ���� ���� �ε��
	virtual ~CEditor_Obj();

public:
	void SetPivotPoint(int _iRow, int _iCol) { m_stPivotPoint.iRow = _iRow; m_stPivotPoint.iCol = _iCol; }
	void MovePivotPoint(int _iDeltaRow, int _iDeltaCol) { m_stPivotPoint.iRow += _iDeltaRow; m_stPivotPoint.iCol += _iDeltaCol; }
	_pivot_point GetPivotPoint(void) const { return m_stPivotPoint; }
	virtual RECT GetRect(void) const = 0;
	virtual RECT GetRowColRect(void) const = 0;
	virtual void Render(HDC& _hdc, CCamera2D* _pCamera) = 0;
	virtual void SaveInfo(FILE * _fpOut);
	virtual void LoadInfo(FILE * _fpIn);
	virtual void MakeMapData(FILE* _fpOut);
	void SetGroupID(int _iGroupID) { m_iGroupID = _iGroupID; }
	int GetGroupID(void) const { return m_iGroupID; }
	void SetDrawLayer(int _iDrawLayer) { m_iDrawLayer = _iDrawLayer; }
	int GetDrawLayer(void) const { return m_iDrawLayer; }

protected:
	CGameWorld& m_rGameWorld;
	MAP_OBJ::E_TYPE m_eObjType = MAP_OBJ::TYPE_END;
	const _map_render_info& m_rMapRenderInfo;
	_pivot_point m_stPivotPoint;
	int m_iGroupID = -1;
	int m_iDrawLayer = 0;
};

