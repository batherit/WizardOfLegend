#pragma once
#include "CEditor_Obj.h"
class CEditor_Door :
	public CEditor_Obj
{
public:
	CEditor_Door(const _map_render_info & _rMapRenderInfo, int _iPivotRow, int _iPivotCol, MAP_OBJ::E_TYPE _eDoorType); // �ܺ� ������
	CEditor_Door(const _map_render_info & _rMapRenderInfo);		// ���� �ε��
	~CEditor_Door();

public:
	// CTileMapObj��(��) ���� ��ӵ�
	virtual RECT GetRect(void) const override;
	virtual RECT GetRowColRect(void) const override;
	virtual void Render(HDC & _hdc, CCamera2D * _pCamera) override;
	virtual void SaveInfo(FILE * _fpOut);
	virtual void LoadInfo(FILE * _fpIn);

private:
	TCHAR* m_tDoorBitmapKey = L"";
	int m_iCoveredWidthTiles = -1;		// �ش� �̹����� Ÿ�ϸʿ��� �ʺ� �󸶳� �����ϴ��� (Ÿ�� ����) => Ÿ�ϰ��� ��� 1�� �� ���̴�.
	int m_iCoveredHeightTiles = -1;		// �ش� �̹����� Ÿ�ϸʿ��� ���̸� �󸶳� �����ϴ��� (Ÿ�� ����) => Ÿ�ϰ��� ��� 1�� �� ���̴�.
};

