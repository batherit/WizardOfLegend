#pragma once
#include "CEditor_Obj.h"
class CEditor_SpawnPoint :
	public CEditor_Obj
{
public:
	CEditor_SpawnPoint(CGameWorld& _rGameWorld, const _map_render_info & _rMapRenderInfo, int _iX, int _iY);
	CEditor_SpawnPoint(CGameWorld& _rGameWorld, const _map_render_info& _rMapRenderInfo);
	~CEditor_SpawnPoint();

public:
	// CTileMapObj을(를) 통해 상속됨
	virtual RECT GetRect(void) const override;
	virtual RECT GetRowColRect(void) const override;
	virtual void Render(HDC & _hdc, CCamera2D * _pCamera) override;
	virtual void SaveInfo(FILE * _fpOut);
	virtual void LoadInfo(FILE * _fpIn);
	virtual void MakeMapData(FILE * _fpOut);
	void SetSpawnPoint(int _iX, int _iY) { m_ptSpawnPoint.x = _iX, m_ptSpawnPoint.y = _iY; }

private:
	POINT m_ptSpawnPoint = {0, 0};
};

