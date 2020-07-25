#pragma once
#include "CEditor_Obj.h"
class CEditor_Collider :
	public CEditor_Obj
{
public:
	CEditor_Collider(CGameWorld& _rGameWorld, const _map_render_info & _rMapRenderInfo, int _iPivotRow, int _iPivotCol, int _iCoveredWidthTiles = 1, int _iCoveredHeightTiles = 1);
	CEditor_Collider(CGameWorld& _rGameWorld, const _map_render_info& _rMapRenderInfo);
	~CEditor_Collider();

public:
	// CTileMapObj��(��) ���� ��ӵ�
	virtual RECT GetRect(void) const override;
	virtual RECT GetRowColRect(void) const override;
	virtual void Render(HDC & _hdc, CCamera2D * _pCamera) override;
	virtual void SaveInfo(FILE * _fpOut);
	virtual void LoadInfo(FILE * _fpIn);
	virtual void MakeMapData(FILE * _fpOut);

private:
	int m_iCoveredWidthTiles = 1;		// �ش� �̹����� Ÿ�ϸʿ��� �ʺ� �󸶳� �����ϴ��� (Ÿ�� ����) => Ÿ�ϰ��� ��� 1�� �� ���̴�.
	int m_iCoveredHeightTiles = 1;
};

