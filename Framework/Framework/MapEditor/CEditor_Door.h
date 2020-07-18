#pragma once
#include "CEditor_Obj.h"
class CEditor_Door :
	public CEditor_Obj
{
public:
	CEditor_Door(CGameWorld& _rGameWorld, const _map_render_info & _rMapRenderInfo, int _iPivotRow, int _iPivotCol, MAP_OBJ::E_TYPE _eDoorType); // 외부 생성용
	CEditor_Door(CGameWorld& _rGameWorld, const _map_render_info & _rMapRenderInfo);		// 파일 로드용
	~CEditor_Door();

public:
	// CTileMapObj을(를) 통해 상속됨
	virtual RECT GetRect(void) const override;
	virtual RECT GetRowColRect(void) const override;
	virtual void Render(HDC & _hdc, CCamera2D * _pCamera) override;
	virtual void SaveInfo(FILE * _fpOut);
	virtual void LoadInfo(FILE * _fpIn);
	virtual void MakeMapData(FILE * _fpOut);

private:
	TCHAR* m_tDoorBitmapKey = L"";
	int m_iCoveredWidthTiles = -1;		// 해당 이미지가 타일맵에서 너비를 얼마나 차지하는지 (타일 단위) => 타일같은 경우 1이 될 것이다.
	int m_iCoveredHeightTiles = -1;		// 해당 이미지가 타일맵에서 높이를 얼마나 차지하는지 (타일 단위) => 타일같은 경우 1이 될 것이다.
};

