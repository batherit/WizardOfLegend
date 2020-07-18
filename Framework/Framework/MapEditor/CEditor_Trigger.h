#pragma once
#include "CEditor_Obj.h"
class CEditor_Trigger :
	public CEditor_Obj
{
public:
	CEditor_Trigger(CGameWorld& _rGameWorld, const _map_render_info & _rMapRenderInfo, int _iPivotRow, int _iPivotCol); // �ܺ� ������
	CEditor_Trigger(CGameWorld& _rGameWorld, const _map_render_info& _rMapRenderInfo); // ���� �ε��
	~CEditor_Trigger();

public:
	// CTileMapObj��(��) ���� ��ӵ�
	virtual RECT GetRect(void) const override;
	virtual RECT GetRowColRect(void) const override;
	virtual void Render(HDC & _hdc, CCamera2D * _pCamera) override;
	virtual void SaveInfo(FILE * _fpOut);
	virtual void LoadInfo(FILE * _fpIn);
	virtual void MakeMapData(FILE* _fpOut);
//
//public:
//	_pivot_point GetEndPoint(void) const { return m_stEndPoint; }
//	void SetEndPoint(int _iRow, int _iCol) { m_stEndPoint.iRow = _iRow; m_stEndPoint.iCol = _iCol; }
//
//private:
//	_pivot_point m_stEndPoint;
};

