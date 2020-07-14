#pragma once
#include "CObj.h"

class CMap;

class CAtlasObj :
	public CObj
{
public:
	CAtlasObj(CGameWorld& _rGameWorld, CMap& _rMap, const _atlas_obj_info& _rAtlasObjInfo, float _fX, float _fY, size_t _iWidth, size_t _iHeight);
	~CAtlasObj();

public:
	virtual void Render(HDC & _hdc, CCamera2D * _pCamera) override;

private:
	CMap& m_rMap;
	const _atlas_obj_info m_stAtlasObjInfo;
};

