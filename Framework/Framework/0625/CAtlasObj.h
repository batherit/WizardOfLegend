#pragma once
#include "CObj.h"

class CMapLoader;

class CAtlasObj :
	public CObj
{
public:
	CAtlasObj(FILE* _fpIn, CGameWorld& _rGameWorld, CMapLoader& _rMap);
	~CAtlasObj();

public:
	virtual void Render(HDC & _hdc, CCamera2D * _pCamera) override;

private:
	CMapLoader& m_rMap;
	_atlas_obj_info m_stAtlasObjInfo;
};

